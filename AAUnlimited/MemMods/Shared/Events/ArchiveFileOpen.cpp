#include "StdAfx.h"
#include "Files\PersistentStorage.h"
namespace SharedInjections {
namespace ArchiveFile {



/*
* If false is returned, the original function will be executed.
* else, the function is aborted and the results from this function are used.
*/
bool __stdcall OpenFileEvent(wchar_t** paramArchive, wchar_t** paramFile, DWORD* readBytes, BYTE** outBuffer) {
	static wchar_t arch[1024];
	static wchar_t file[1024];

	const wchar_t *orig_paramArchive = arch;
	const wchar_t *orig_paramFile = file;
	const char *provider = NULL;
	const char *rewriter = "";
	const char *rewriter2 = "";

	if (Poser::OverrideFile(paramArchive, paramFile, readBytes, outBuffer)) {
		provider = "overridefile";
		goto done;
	}

	// NOTE: return value implies the name was rewritten
	if (Shared::ArchiveReplaceRules(paramArchive, paramFile, readBytes, outBuffer)) {
		rewriter = "replace";
	}

	if (Shared::TanOverride(paramArchive, paramFile, readBytes, outBuffer)) {
		provider = "tanoverride";
		goto done;
	}

	if (Shared::HairRedirect(paramArchive, paramFile, readBytes, outBuffer)) {
		rewriter2 = ",hairredirect";
	}

	if (Shared::ArchiveOverrideRules(*paramArchive, *paramFile, readBytes, outBuffer)) {
		provider = "archiveoverride";
		goto done;
	}


	if (g_Config.bUseShadowing) {
		if (Shared::OpenShadowedFile(*paramArchive, *paramFile, readBytes, outBuffer)) {
			provider = "shadowed";
			goto done;
		}
	}

	if (g_Config.bUsePPeX) {
		if (g_PPeX.ArchiveDecompress(*paramArchive, *paramFile, readBytes, outBuffer)) {
			provider = "ppex";
			goto done;
		}
	}

	if (g_Config.bUsePP2) {
		if (g_PP2.ArchiveDecompress(*paramArchive, *paramFile, readBytes, outBuffer)) {
			provider = "pp2";
			goto done;
		}
	}

done:;
	if (g_Config.bLogPPAccess) {
		LOGPRIONC(Logger::Priority::SPAM) "OpenFileEvent " <<
			"provider=" << (provider?provider:"pp") << " " <<
			"archive=" << std::wstring(*paramArchive) << " " <<
			"file=" << std::wstring(*paramFile);
		if ((*paramArchive != orig_paramArchive) || (orig_paramFile != *paramFile)) {
			LOGSPAM << "rewriter=" << rewriter << rewriter2 << " " <<
				" origarchive=" << std::wstring(orig_paramArchive) << " origfile=" << std::wstring(orig_paramFile);
		}
		if (provider)
			LOGSPAM << " size=" << std::dec << *readBytes;
		LOGSPAM << "\r\n";
	}

	return provider != NULL;
}

DWORD OpenFileNormalExit;
void __declspec(naked) OpenFileRedirect() {
	__asm {
		pushad
		push[esp + 0x20 + 0x10 + 0]
		push edi
		lea eax, [esp + 0x20 + 0xC + 8]
		push eax
		lea eax, [esp + 0x20 + 4 + 0xC]
		push eax
		call OpenFileEvent
		test al, al
		popad
		jz OpenFileRedirect_NormalExit
		mov al, 1
		ret
	OpenFileRedirect_NormalExit :
		push ebp
		mov ebp, esp
		and esp, -8
		jmp[OpenFileNormalExit]
	}
}



#define PPF_HANDLE ((HANDLE)-2)
std::set<std::wstring> PPFileList;
std::set<std::wstring>::iterator ppf_it;
HANDLE ppf_handle = INVALID_HANDLE_VALUE;

void RegisterPP(const wchar_t *name) {
	PPFileList.insert(name);
}

static BOOL WINAPI MyFC(HANDLE h) {
	if (h == ppf_handle) {
		ppf_handle = INVALID_HANDLE_VALUE;
		if (h == PPF_HANDLE)
			return TRUE;
	}
	return FindClose(h);
}

static BOOL WINAPI MyFN(HANDLE h, LPWIN32_FIND_DATAW data) {
	if (h == ppf_handle) {
		// We'll interject, but not just yet, wait for normal file list to finish
		if (h != PPF_HANDLE && ppf_it == PPFileList.begin() && FindNextFileW(h, data))
			return TRUE;
		if (ppf_it == PPFileList.end())
			return FALSE;
		wcscpy(data->cFileName, (*ppf_it).c_str());
		data->dwFileAttributes = FILE_ATTRIBUTE_ARCHIVE;
		ppf_it++;
		return TRUE;
	}
	return FindNextFileW(h, data);
}

static bool is_pp_path(const wchar_t *path) {
	int pplen = wcslen(path);
	if (pplen < 5)
		return false;
	return !wcscmp(path + pplen - 4, L"*.pp");
}

static HANDLE WINAPI MyFF(const wchar_t *path, LPWIN32_FIND_DATAW data) {
	HANDLE h = FindFirstFileW(path, data);
	if (!is_pp_path(path))
		return h;

	ppf_it = PPFileList.begin();

	if (h == INVALID_HANDLE_VALUE) {
		ppf_handle = h = PPF_HANDLE;
		if (!MyFN(h, data))
			return (ppf_handle = INVALID_HANDLE_VALUE);
	}

	ppf_handle = h;
	return h;
}

void DirScanInject()
{
	DWORD *ffaddr = (DWORD*)(General::GameBase + 0x2E31E0);
	if (General::IsAAEdit)
		ffaddr = (DWORD*)(General::GameBase + 0x2C41E0);

	Memrights rights(ffaddr, 12);

	ffaddr[0] = (DWORD)&MyFC;
	ffaddr[1] = (DWORD)&MyFF;
	ffaddr[2] = (DWORD)&MyFN;
}

void OpenFileInject() {
	if (General::IsAAEdit) {
		//bool someFunc(edi = DWORD* readBytes, wchar* archive, 
		//				someClass* globalClass, wchar* filename, BYTE** outBuffer) {
		/*AA2Edit.exe+1F89F0 - 55                    - push ebp
		AA2Edit.exe+1F89F1 - 8B EC                 - mov ebp,esp
		AA2Edit.exe+1F89F3 - 83 E4 F8              - and esp,-08 { 248 }
		AA2Edit.exe+1F89F6 - 83 EC 18              - sub esp,18 { 24 }
		AA2Edit.exe+1F89F9 - 33 C0                 - xor eax,eax
		AA2Edit.exe+1F89FB - 53                    - push ebx
		AA2Edit.exe+1F89FC - 8B 5D 14              - mov ebx,[ebp+14]
		AA2Edit.exe+1F89FF - 89 44 24 08           - mov [esp+08],eax
		AA2Edit.exe+1F8A03 - 89 44 24 0C           - mov [esp+0C],eax
		AA2Edit.exe+1F8A07 - 89 44 24 10           - mov [esp+10],eax
		AA2Edit.exe+1F8A0B - 89 44 24 14           - mov [esp+14],eax
		AA2Edit.exe+1F8A0F - 89 44 24 18           - mov [esp+18],eax*/
		DWORD address = General::GameBase + 0x1F89F0;
		DWORD redirectAddress = (DWORD)(&OpenFileRedirect);
		Hook((BYTE*)address,
			{ 0x55, 0x8B, 0xEC, 0x83, 0xE4, 0xF8 },						//expected values
			{ 0xE9, HookControl::RELATIVE_DWORD, redirectAddress, 0x90 },	//redirect to our function
			NULL);
		OpenFileNormalExit = General::GameBase + 0x1F89F6;
	}
	else if (General::IsAAPlay) {
		//bool someFunc(edi = DWORD* readBytes, wchar* archive, 
		//				someClass* globalClass, wchar* filename, BYTE** outBuffer) {
		/*AA2Play v12 FP v1.4.0a.exe+216470 - 55                    - push ebp
		AA2Play v12 FP v1.4.0a.exe+216471 - 8B EC                 - mov ebp,esp
		AA2Play v12 FP v1.4.0a.exe+216473 - 83 E4 F8              - and esp,-08 { 248 }
		AA2Play v12 FP v1.4.0a.exe+216476 - 83 EC 18              - sub esp,18 { 24 }
		AA2Play v12 FP v1.4.0a.exe+216479 - 33 C0                 - xor eax,eax
		AA2Play v12 FP v1.4.0a.exe+21647B - 53                    - push ebx
		AA2Play v12 FP v1.4.0a.exe+21647C - 8B 5D 14              - mov ebx,[ebp+14]
		AA2Play v12 FP v1.4.0a.exe+21647F - 89 44 24 08           - mov [esp+08],eax
		AA2Play v12 FP v1.4.0a.exe+216483 - 89 44 24 0C           - mov [esp+0C],eax
		AA2Play v12 FP v1.4.0a.exe+216487 - 89 44 24 10           - mov [esp+10],eax
		AA2Play v12 FP v1.4.0a.exe+21648B - 89 44 24 14           - mov [esp+14],eax
		AA2Play v12 FP v1.4.0a.exe+21648F - 89 44 24 18           - mov [esp+18],eax
		*/
		DWORD address = General::GameBase + 0x216470;
		DWORD redirectAddress = (DWORD)(&OpenFileRedirect);
		Hook((BYTE*)address,
			{ 0x55, 0x8B, 0xEC, 0x83, 0xE4, 0xF8 },						//expected values
			{ 0xE9, HookControl::RELATIVE_DWORD, redirectAddress, 0x90 },	//redirect to our function
			NULL);
		OpenFileNormalExit = General::GameBase + 0x216476;
	}
			
}

}
}
