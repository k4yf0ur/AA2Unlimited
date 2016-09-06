#include "OverrideFile.h"

#include "General\Util.h"
#include "General\ModuleInfo.h"
#include "Files\Logger.h"
#include "config.h"

OverrideFile::OverrideFile() : m_good(false) {
}

OverrideFile::OverrideFile(const TCHAR* fileName, bool absPath) : OverrideFile(OVERRIDE_ARCHIVE_PATH, fileName, absPath, true, true) {
	
}

OverrideFile::OverrideFile(const TCHAR* path, const TCHAR* fileName, bool absPath, bool tryAAPlay, bool tryAAEdit) : OverrideFile() {
	m_fileName = fileName;

	HANDLE file = NULL;
	if (absPath) {
		m_fileName = General::FindFileInPath(fileName);
		m_fullPath = fileName;
		file = CreateFile(m_fullPath.c_str(), FILE_READ_ACCESS, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (file != INVALID_HANDLE_VALUE && file != NULL) {
			m_good = true;
		}
	}
	else {
		if (tryAAPlay) {
			m_fullPath = General::BuildPlayPath(path, fileName);
			file = CreateFile(m_fullPath.c_str(), FILE_READ_ACCESS, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
			if (file != INVALID_HANDLE_VALUE && file != NULL) {
				m_good = true;
			}
		}
		if (!m_good && tryAAEdit) {
			m_fullPath = General::BuildEditPath(path, fileName);
			file = CreateFile(m_fullPath.c_str(), FILE_READ_ACCESS, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
			if (file != INVALID_HANDLE_VALUE && file != NULL) {
				m_good = true;
			}
		}
	}
	

	if (!m_good) return;

	DWORD hi;
	m_fileSize = ::GetFileSize(file, &hi);
	CloseHandle(file);
}

OverrideFile::~OverrideFile() {
	
}

bool OverrideFile::WriteToBuffer(BYTE* buffer) const {
	if (m_cache.size() > 0) {
		//if cached, copy the cache
		memcpy(buffer, &(m_cache[0]), m_fileSize);
		return true;
	}
	else {
		//we will have to read if from the file
		HANDLE file = CreateFile(m_fullPath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		int fileError = GetLastError();
		DWORD written;
		ReadFile(file, buffer, m_fileSize, &written, NULL);
		int readError = GetLastError();
		CloseHandle(file);
		if (written != m_fileSize) {
			LOGPRIO(Logger::Priority::WARN) << "failed to read from file " << m_fullPath << ": "
				"Open Error " << fileError << ", readError " << readError << "\r\n";
			return false;
		}
		return true;
	}
}