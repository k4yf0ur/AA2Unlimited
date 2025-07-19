#include "StdAfx.h"
#include "ExtraRooms.h"

#define NEW_HOOK 1
#define REGRESSIONS 1

namespace PlayInjections {
	namespace ExtraRooms {
		using namespace ExtClass;
		DWORD* extraRoomReturn;
		DWORD* extraRoomReturn2;
		DWORD* extraRoomReturn3;
		DWORD* extraRoomReturn4;
		DWORD* extraRoomReturn5;
		DWORD* extraRoomsPublicRoom;

		DWORD* extraRoomsLoopJump;

		DWORD* extraRoomsPrivateRoom;
		DWORD* extraRoomsPrivateRoom2;
		DWORD* extraRoomsPrivateRoom3;
		DWORD* extraRoomsPrivateRoom4;

		DWORD* extraRoomsPrivateReturn;
		DWORD* extraRoomsPrivateReturn2;
		DWORD* extraRoomsPrivateReturn3;
		DWORD* extraRoomsPrivateReturn4;
		DWORD* extraRoomsPrivateReturn5;
		DWORD* extraRoomsPrivateReturn6;
		DWORD* extraRoomsPrivateReturn7;
		DWORD* extraRoomsPrivateReturn8;
		DWORD* extraRoomsPrivateReturn9;
		DWORD* extraRoomsPrivateReturn10;
		DWORD* extraRoomsPrivateReturn11;
		DWORD* extraRoomsPrivateReturn12;
		DWORD* extraRoomsPrivateReturn13;
		DWORD* extraRoomsPrivateReturn14;
		DWORD* extraRoomsPrivateReturn15;
		DWORD* extraRoomsPrivateReturn16;
		DWORD* extraRoomsPrivateReturn17;
		DWORD* extraRoomsPrivateReturn18;
		DWORD* extraRoomsPrivateReturn19;

		DWORD* extraRoomsPrivateSuccess;
		DWORD* extraRoomsPrivateSuccess2;
		DWORD* extraRoomsPrivateSuccess3;
		DWORD* extraRoomsPrivateSuccess4;
		DWORD* extraRoomsPrivateSuccess5;
		DWORD* extraRoomsPrivateSuccess6;
		DWORD* extraRoomsPrivateSuccess7;
		DWORD* extraRoomsPrivateSuccess8;
		DWORD* extraRoomsPrivateSuccess9;
		DWORD* extraRoomsPrivateSuccess10;

		DWORD* extraRoomsPublicRoom2;

		const char* roomList[] = {
			"MP00", "MP01", "MP02", "MP03", "MP04", "MP05", "MP06", "MP07", "MP08", "MP09",
			"MP10", "MP11", "MP12", "MP13", "MP14", "MP15", "MP16", "MP17", "MP18", "MP19",
			"MP20", "MP21", "MP22", "MP23", "MP24", "MP25", "MP26", "MP27", "MP28", "MP29",
			"MP30", "MP31", "MP32", "MP33", "MP34", "MP35", "MP36", "MP37", "MP38", "MP39",
			"MP40", "MP41", "MP42", "MP43", "MP44", "MP45", "MP46", "MP47", "MP48", "MP49",
			"MP50", "MP51", "MP52", "MP53", "MP54", "MP55", "MP56", "MP57", "MP58", "MP59",
			"MP60", "MP61", "MP62", "MP63", "MP64", "MP65", "MP66", "MP67", "MP68", "MP69",
			"MP70", "MP71", "MP72", "MP73", "MP74", "MP75", "MP76", "MP77", "MP78", "MP79",
			"MP80", "MP81", "MP82", "MP83", "MP84", "MP85", "MP86", "MP87", "MP88", "MP89",
			"MP90", "MP91", "MP92", "MP93", "MP94", "MP95", "MP96", "MP97", "MP98", "MP99",
			"MP100", "MP101", "MP102", "MP103", "MP104", "MP105", "MP106", "MP107", "MP108", "MP109",
			"MP110", "MP111", "MP112", "MP113", "MP114", "MP115", "MP116", "MP117", "MP118", "MP119",
			"MP120", "MP121", "MP122", "MP123", "MP124", "MP125", "MP126", "MP127", "MP128", "MP129",
			"MP130", "MP131", "MP132", "MP133", "MP134", "MP135", "MP136", "MP137", "MP138", "MP139",
			"MP140", "MP141", "MP142", "MP143", "MP144", "MP145", "MP146", "MP147", "MP148", "MP149",
			"MP150", "MP151", "MP152", "MP153", "MP154", "MP155", "MP156", "MP157", "MP158", "MP159",
			"MP160", "MP161", "MP162", "MP163", "MP164", "MP165", "MP166", "MP167", "MP168", "MP169",
			"MP170", "MP171", "MP172", "MP173", "MP174", "MP175", "MP176", "MP177", "MP178", "MP179",
			"MP180", "MP181", "MP182", "MP183", "MP184", "MP185", "MP186", "MP187", "MP188", "MP189",
			"MP190", "MP191", "MP192", "MP193", "MP194", "MP195", "MP196", "MP197", "MP198", "MP199",
			"MP200", "MP201", "MP202", "MP203", "MP204", "MP205", "MP206", "MP207", "MP208", "MP209",
			"MP210", "MP211", "MP212", "MP213", "MP214", "MP215", "MP216", "MP217", "MP218", "MP219",
			"MP220", "MP221", "MP222", "MP223", "MP224", "MP225", "MP226", "MP227", "MP228", "MP229",
			"MP230", "MP231", "MP232", "MP233", "MP234", "MP235", "MP236", "MP237", "MP238", "MP239",
			"MP240", "MP241", "MP242", "MP243", "MP244", "MP245", "MP246", "MP247", "MP248", "MP249",
			"MP250", "MP251", "MP252", "MP253", "MP254", "MP255"
		};


		void __declspec(naked) extraRoomsRedirect() {
			__asm {
				// If (esi <= 0x28 || (esi > 0x3C && esi < 0x81)) => public
				// Else => private
				cmp esi, 0x28
				jle doPublic

				cmp esi, 0x3c
				jle doPrivate

				cmp esi, 0x80
				jb doPublic

				xor al, al
				jmp extraRoomsPrivateRoom

				doPrivate :
				xor al, al
					jmp extraRoomsPrivateRoom

					doPublic :
				jmp extraRoomsPublicRoom
			}
		}


		void extraRoomsInjection() {

			const DWORD offset1[]{ 0x164E55 };
			extraRoomsPublicRoom = (DWORD*)ExtVars::ApplyRule(offset1);
			const DWORD offset2[]{ 0x1652ED };
			extraRoomsPrivateRoom = (DWORD*)ExtVars::ApplyRule(offset2);


			DWORD address = General::GameBase + 0x164E49;
			DWORD redirectAddress = (DWORD)(&extraRoomsRedirect);
			Hook((BYTE*)address,
				{ 0x83, 0xFE, 0x28, 0x76, 0x07 },						//expected values
				{ 0xE9, HookControl::RELATIVE_DWORD, redirectAddress },	//redirect to our function
				NULL);
		}

		void __declspec(naked) extraRoomsSecondRedirect() {
			__asm {
				cmp esi, 0x29
				jl do_public

				cmp esi, 0x3C
				jle do_private

				cmp esi, 0x80
				jg do_private

				do_public :
				jmp extraRoomsPublicRoom2

					do_private :
				jmp extraRoomsPrivateRoom2
			}
		}

		void extraRoomsSecondInjection() {

			const DWORD offset1[]{ 0x175352 };
			extraRoomsPublicRoom2 = (DWORD*)ExtVars::ApplyRule(offset1);
			const DWORD offset2[]{ 0x175308 };
			extraRoomsPrivateRoom2 = (DWORD*)ExtVars::ApplyRule(offset2);


			DWORD address = General::GameBase + 0x175348;
			DWORD redirectAddress = (DWORD)(&extraRoomsSecondRedirect);
			Hook((BYTE*)address,
				{ 0x83, 0xFE, 0x29, 0x7C, 0x05 },						//expected values
				{ 0xE9, HookControl::RELATIVE_DWORD, redirectAddress },	//redirect to our function
				NULL);
		}


		void __declspec(naked) extraRoomsThirdRedirect() {
			__asm {
				add eax, -0x29
				cmp eax, 0x57
				jg privateRoom3
				cmp eax, 0x13
				jmp extraRoomReturn
				privateRoom3 :
				jmp extraRoomsPrivateRoom3
			}
		}

		void extraRoomsThirdInjection() {

			const DWORD offset1[]{ 0x101604 };
			extraRoomsPrivateRoom3 = (DWORD*)ExtVars::ApplyRule(offset1);
			const DWORD offset2[]{ 0x101587 };
			extraRoomReturn = (DWORD*)ExtVars::ApplyRule(offset2);

			DWORD address = General::GameBase + 0x101581;
			DWORD redirectAddress = (DWORD)(&extraRoomsThirdRedirect);
			Hook((BYTE*)address,
				{ 0x83, 0xC0, 0xD7, 0x83, 0xF8, 0x13 },						//expected values
				{ 0xE9, HookControl::RELATIVE_DWORD, redirectAddress, 0x90 },	//redirect to our function
				NULL);
		}

		void __declspec(naked) extraRoomsFourthRedirect() {
			__asm {

				add eax, -0x29
				cmp eax, 0x57
				jg privateRoom4
				cmp eax, 0x13
				jmp extraRoomReturn2
				privateRoom4 :
				jmp extraRoomsPrivateRoom4
			}
		}

		void extraRoomsFourthInjection() {

			const DWORD offset1[]{ 0x101BFE };
			extraRoomsPrivateRoom4 = (DWORD*)ExtVars::ApplyRule(offset1);
			const DWORD offset2[]{ 0x101B81 };
			extraRoomReturn2 = (DWORD*)ExtVars::ApplyRule(offset2);

			DWORD address = General::GameBase + 0x101B7B;
			DWORD redirectAddress = (DWORD)(&extraRoomsFourthRedirect);
			Hook((BYTE*)address,
				{ 0x83, 0xC0, 0xD7, 0x83, 0xF8, 0x13 },						//expected values
				{ 0xE9, HookControl::RELATIVE_DWORD, redirectAddress, 0x90 },	//redirect to our function
				NULL);
		}

		void __declspec(naked) extraRoomsFifthRedirect() {
			__asm {
				mov eax, [esi * 4 + roomList]
				jmp extraRoomReturn3
			}
		}

		void extraRoomsFifthInjection() {

			const DWORD offset2[]{ 0x101FD9 };
			extraRoomReturn3 = (DWORD*)ExtVars::ApplyRule(offset2);

			DWORD address = General::GameBase + 0x101FD2;
			DWORD redirectAddress = (DWORD)(&extraRoomsFifthRedirect);
			Hook((BYTE*)address,
				{ 0x8B, 0x04, 0xB5, 0x78, 0x67, 0x26, 0x01 },						//expected values
				{ 0xE9, HookControl::RELATIVE_DWORD, redirectAddress, 0x90, 0x90 },	//redirect to our function
				NULL);
		}

		void __declspec(naked) extraRoomsSixthRedirect() {
			__asm {
				cmp esi, 0xC4
				jl jumpBack
				jmp extraRoomReturn4

				jumpBack :
				jmp extraRoomsLoopJump
			}
		}

		void extraRoomsSixthInjection() {

			const DWORD offset1[]{ 0x101FBF };
			extraRoomsLoopJump = (DWORD*)ExtVars::ApplyRule(offset1);
			const DWORD offset2[]{ 0x101FF2 };
			extraRoomReturn4 = (DWORD*)ExtVars::ApplyRule(offset2);

			DWORD address = General::GameBase + 0x101FED;
			DWORD redirectAddress = (DWORD)(&extraRoomsSixthRedirect);
			Hook((BYTE*)address,
				{ 0x83, 0xFE, 0x3A, 0x7C, 0xCD },						//expected values
				{ 0xE9, HookControl::RELATIVE_DWORD, redirectAddress },	//redirect to our function
				NULL);
		}

		void __declspec(naked) extraRoomsPrivateRoomHRedirect() {
			__asm {
				cmp edi, 0x80
				jg successPath
				cmp edi, 0x2B
				mov edx, [ebp + 0x00]
				jmp extraRoomsPrivateReturn

				successPath :
				mov edx, [ebp + 0x00]
					lea eax, [ecx + ebp + 0x34]
					mov eax, [edx + 0x04]
					mov[eax + ebp + 0x2D], bl
					jmp extraRoomsPrivateSuccess
			}
		}


		void extraRoomsPrivateRoomHInjection() {
			//AA2Play.exe+59E73 - cmp edi,2B
			//AA2Play.exe + 59E76 - mov edx, [ebp + 00]

			const DWORD offset1[]{ 0x59E79 };
			extraRoomsPrivateReturn = (DWORD*)ExtVars::ApplyRule(offset1);
			const DWORD offset2[]{ 0x59E8B };
			extraRoomsPrivateSuccess = (DWORD*)ExtVars::ApplyRule(offset2);

			DWORD address = General::GameBase + 0x59E73;
			DWORD redirectAddress = (DWORD)(&extraRoomsPrivateRoomHRedirect);
			Hook((BYTE*)address,
				{ 0x83, 0xFF, 0x28, 0x8B, 0x55, 0x00 },						//expected values
				{ 0xE9, HookControl::RELATIVE_DWORD, redirectAddress, 0x90 },	//redirect to our function
				NULL);
		}

		void __declspec(naked) extraRoomsPrivateRoomHRedirect2() {
			__asm {
				cmp esi, 0x80
				jg privateRoom
				cmp esi, 0x28
				jl successPath
				jmp extraRoomsPrivateReturn3

				successPath :
				jmp extraRoomsPrivateSuccess2

					privateRoom :
				jmp extraRoomsPrivateReturn2
			}
		}

		void extraRoomsPrivateRoomHInjection2() {
			//cmp esi, 28
			//jl AA2Play.exe + EB1D6

			const DWORD offset1[]{ 0xEB205 };
			extraRoomsPrivateReturn2 = (DWORD*)ExtVars::ApplyRule(offset1);
			const DWORD offset2[]{ 0xEB1D6 };
			extraRoomsPrivateSuccess2 = (DWORD*)ExtVars::ApplyRule(offset2);
			const DWORD offset3[]{ 0xEB1D1 };
			extraRoomsPrivateReturn3 = (DWORD*)ExtVars::ApplyRule(offset3);


			DWORD address = General::GameBase + 0xEB1CC;
			DWORD redirectAddress = (DWORD)(&extraRoomsPrivateRoomHRedirect2);
			Hook((BYTE*)address,
				{ 0x83, 0xFE, 0x28, 0x7C, 0x05 },						//expected values
				{ 0xE9, HookControl::RELATIVE_DWORD, redirectAddress },	//redirect to our function
				NULL);
		}



		void __declspec(naked) extraRoomsPrivateRoomHRedirect3() {
			__asm {
				cmp eax, 0x80
				jg privateRoom
				cmp eax, 0x28
				jl successPath
				jmp extraRoomsPrivateReturn5

				successPath :
				jmp extraRoomsPrivateSuccess3

					privateRoom :
				jmp extraRoomsPrivateReturn4
			}
		}

		void extraRoomsPrivateRoomHInjection3() {
			//cmp eax, 28
			//jl AA2Play.exe + EB1D6

			const DWORD offset1[]{ 0xEB429 };
			extraRoomsPrivateReturn4 = (DWORD*)ExtVars::ApplyRule(offset1);
			const DWORD offset2[]{ 0xEB471 };
			extraRoomsPrivateSuccess3 = (DWORD*)ExtVars::ApplyRule(offset2);
			const DWORD offset3[]{ 0xEB46C };
			extraRoomsPrivateReturn5 = (DWORD*)ExtVars::ApplyRule(offset3);

			DWORD address = General::GameBase + 0xEB467;
			DWORD redirectAddress = (DWORD)(&extraRoomsPrivateRoomHRedirect3);
			Hook((BYTE*)address,
				{ 0x83, 0xF8, 0x28, 0x7C, 0x05 },						//expected values
				{ 0xE9, HookControl::RELATIVE_DWORD, redirectAddress },	//redirect to our function
				NULL);
		}

		void __declspec(naked) extraRoomsPrivateRoomHRedirect4() {
			__asm {
				cmp edx, 0x80
				jg privateRoom
				cmp edx, 0x28
				jl publicRoom
				jmp extraRoomsPrivateReturn7

				privateRoom :
				jmp extraRoomsPrivateReturn6

					publicRoom :
				jmp extraRoomsPrivateSuccess4
			}
		}

		void extraRoomsPrivateRoomHInjection4() {
			//AA2Play.exe + 1751A6 - cmp edx, 28
			//AA2Play.exe + 1751A9 - jl AA2Play.exe + 1751E0
			//AA2Play.exe+1751AB - cmp edx,3C


			const DWORD offset1[]{ 0x1751B0 };
			extraRoomsPrivateReturn6 = (DWORD*)ExtVars::ApplyRule(offset1);
			const DWORD offset2[]{ 0x1751E0 };
			extraRoomsPrivateSuccess4 = (DWORD*)ExtVars::ApplyRule(offset2);
			const DWORD offset3[]{ 0x1751AB };
			extraRoomsPrivateReturn7 = (DWORD*)ExtVars::ApplyRule(offset3);

			DWORD address = General::GameBase + 0x1751A6;
			DWORD redirectAddress = (DWORD)(&extraRoomsPrivateRoomHRedirect4);
			Hook((BYTE*)address,
				{ 0x83, 0xF8, 0x28, 0x7C, 0x05 },						//expected values
				{ 0xE9, HookControl::RELATIVE_DWORD, redirectAddress },	//redirect to our function
				NULL);
		}

		void __declspec(naked) extraRoomsPrivateRoomHRedirect5() {
			__asm {
				cmp edi, 0x80
				jg privateRoom
				cmp edi, 0x29
				jl publicRoom
				jmp extraRoomsPrivateReturn9

				privateRoom :
				jmp extraRoomsPrivateReturn8

					publicRoom :
				jmp extraRoomsPrivateSuccess5
			}
		}

		void extraRoomsPrivateRoomHInjection5() {
			//AA2Play.exe+15C147 - cmp edi,29
			//AA2Play.exe + 15C14A - jl AA2Play.exe + 15C156
			//AA2Play.exe+15C14C - cmp edi,3C

			const DWORD offset1[]{ 0x15C151 };
			extraRoomsPrivateReturn8 = (DWORD*)ExtVars::ApplyRule(offset1);
			const DWORD offset2[]{ 0x15C156 };
			extraRoomsPrivateSuccess5 = (DWORD*)ExtVars::ApplyRule(offset2);
			const DWORD offset3[]{ 0x15C14C };
			extraRoomsPrivateReturn9 = (DWORD*)ExtVars::ApplyRule(offset3);

			DWORD address = General::GameBase + 0x15C147;
			DWORD redirectAddress = (DWORD)(&extraRoomsPrivateRoomHRedirect5);
			Hook((BYTE*)address,
				{ 0x83, 0xFF, 0x29, 0x7C, 0x0A },						//expected values
				{ 0xE9, HookControl::RELATIVE_DWORD, redirectAddress },	//redirect to our function
				NULL);
		}

		void __declspec(naked) extraRoomsPrivateRoomHRedirect6() {
			__asm {
				cmp eax, 0x80
				mov edi, [edi + 0x8C]
				jg privateRoom
				cmp eax, 0x28
				jmp extraRoomsPrivateReturn11

				privateRoom :
				jmp extraRoomsPrivateReturn10
			}
		}

		void extraRoomsPrivateRoomHInjection6() {
			//AA2Play.exe + 15C254 - cmp eax, 28
			//AA2Play.exe + 15C257 - mov edi, [edi + 0000008C]
			//AA2Play.exe + 15C25D - jl AA2Play.exe + 15C268


			const DWORD offset1[]{ 0x15C420 };
			extraRoomsPrivateReturn10 = (DWORD*)ExtVars::ApplyRule(offset1);
			const DWORD offset3[]{ 0x15C25D };
			extraRoomsPrivateReturn11 = (DWORD*)ExtVars::ApplyRule(offset3);

			DWORD address = General::GameBase + 0x15C254;
			DWORD redirectAddress = (DWORD)(&extraRoomsPrivateRoomHRedirect6);
			Hook((BYTE*)address,
				{ 0x83, 0xFF, 0x29, 0x7C, 0x0A },						//expected values
				{ 0xE9, HookControl::RELATIVE_DWORD, redirectAddress },	//redirect to our function
				NULL);
		}


		void __declspec(naked) extraRoomsPrivateRoomHRedirect7() {
			__asm {
				cmp eax, 0x80
				jg privateRoom
				cmp eax, 0x2B
				jl publicRoom
				jmp extraRoomsPrivateReturn13

				publicRoom :
				jmp extraRoomsPrivateSuccess6

					privateRoom :
				jmp extraRoomsPrivateReturn12
			}
		}

		void extraRoomsPrivateRoomHInjection7() {
			//AA2Play.exe+3D48B - cmp eax,2B
			//AA2Play.exe + 3D48E - jl AA2Play.exe + 3D495
			//AA2Play.exe + 3D490 - cmp eax, 34

			const DWORD offset1[]{ 0x3D455 };
			extraRoomsPrivateReturn12 = (DWORD*)ExtVars::ApplyRule(offset1);
			const DWORD offset2[]{ 0x3D495 };
			extraRoomsPrivateSuccess6 = (DWORD*)ExtVars::ApplyRule(offset2);
			const DWORD offset3[]{ 0x3D490 };
			extraRoomsPrivateReturn13 = (DWORD*)ExtVars::ApplyRule(offset3);

			DWORD address = General::GameBase + 0x3D48B;
			DWORD redirectAddress = (DWORD)(&extraRoomsPrivateRoomHRedirect7);
			Hook((BYTE*)address,
				{ 0x83, 0xF8, 0x2B, 0x7C, 0x05 },						//expected values
				{ 0xE9, HookControl::RELATIVE_DWORD, redirectAddress },	//redirect to our function
				NULL);
		}



		void __declspec(naked) extraRoomsPrivateRoomHRedirect8() {
			__asm {
				cmp eax, 0x80
				jg privateRoom
				cmp eax, 0x2B
				jl publicRoom
				jmp extraRoomsPrivateReturn15

				publicRoom :
				jmp extraRoomsPrivateSuccess7

					privateRoom :
				jmp extraRoomsPrivateReturn14
			}
		}

		void extraRoomsPrivateRoomHInjection8() {
			//AA2Play.exe+3D43A - cmp eax,2B
			//AA2Play.exe + 3D43D - jl AA2Play.exe + 3D495
			//AA2Play.exe + 3D43F - cmp eax, 34

			const DWORD offset1[]{ 0x3D444 };
			extraRoomsPrivateReturn14 = (DWORD*)ExtVars::ApplyRule(offset1);
			const DWORD offset2[]{ 0x3D495 };
			extraRoomsPrivateSuccess7 = (DWORD*)ExtVars::ApplyRule(offset2);
			const DWORD offset3[]{ 0x3D43F };
			extraRoomsPrivateReturn15 = (DWORD*)ExtVars::ApplyRule(offset3);

			DWORD address = General::GameBase + 0x3D43A;
			DWORD redirectAddress = (DWORD)(&extraRoomsPrivateRoomHRedirect8);
			Hook((BYTE*)address,
				{ 0x83, 0xF8, 0x2B, 0x7C, 0x56 },						//expected values
				{ 0xE9, HookControl::RELATIVE_DWORD, redirectAddress },	//redirect to our function
				NULL);
		}

		void __declspec(naked) extraRoomsPrivateRoomHRedirect9() {
			__asm {
				add eax, -0x28
				cmp eax, 0x58
				jg privateRoom
				cmp eax, 0x14
				jmp extraRoomsPrivateReturn16

				privateRoom :
				jmp extraRoomsPrivateSuccess8
			}
		}

		void extraRoomsPrivateRoomHInjection9() {
			//AA2Play.exe+EB27D - add eax,-28
			//AA2Play.exe + EB280 - cmp eax, 14
			//AA2Play.exe+EB283 - ja AA2Play.exe+EB28F


			const DWORD offset2[]{ 0xEB285 };
			extraRoomsPrivateSuccess8 = (DWORD*)ExtVars::ApplyRule(offset2);
			const DWORD offset3[]{ 0xEB283 };
			extraRoomsPrivateReturn16 = (DWORD*)ExtVars::ApplyRule(offset3);

			DWORD address = General::GameBase + 0xEB27D;
			DWORD redirectAddress = (DWORD)(&extraRoomsPrivateRoomHRedirect9);
			Hook((BYTE*)address,
				{ 0x83, 0xC0, 0xD8, 0x83, 0xF8, 0x14 },						//expected values
				{ 0xE9, HookControl::RELATIVE_DWORD, redirectAddress, 0x90 },	//redirect to our function
				NULL);
		}

		void __declspec(naked) extraRoomsPrivateRoomHRedirect10() {
			__asm {
				lea ecx, [esi - 0x2D]
				cmp ecx, 0x53
				jg privateRoom
				cmp ecx, 0x07
				jmp extraRoomsPrivateReturn17

				privateRoom :
				jmp extraRoomsPrivateSuccess9
			}
		}

		void extraRoomsPrivateRoomHInjection10() {
			//AA2Play.exe+1AB9AF - lea ecx,[esi-2D]
			//AA2Play.exe + 1AB9B2 - cmp ecx, 07
			//AA2Play.exe + 1AB9B5 - ja AA2Play.exe + 1AB9C0

			const DWORD offset2[]{ 0x1AB9B7 };
			extraRoomsPrivateSuccess9 = (DWORD*)ExtVars::ApplyRule(offset2);
			const DWORD offset3[]{ 0x1AB9B5 };
			extraRoomsPrivateReturn17 = (DWORD*)ExtVars::ApplyRule(offset3);

			DWORD address = General::GameBase + 0x1AB9AF;
			DWORD redirectAddress = (DWORD)(&extraRoomsPrivateRoomHRedirect10);
			Hook((BYTE*)address,
				{ 0x8D, 0x4E, 0xD3, 0x83, 0xF9, 0x07 },						//expected values
				{ 0xE9, HookControl::RELATIVE_DWORD, redirectAddress, 0x90 },	//redirect to our function
				NULL);
		}

		void __declspec(naked) extraRoomsPrivateRoomHRedirect11() {
			__asm {
				cmp edx, 0x80
				jg privateRoom
				cmp edx, 0x28
				jl publicRoom
				jmp extraRoomsPrivateReturn19

				publicRoom :
				jmp extraRoomsPrivateSuccess10

					privateRoom :
				jmp extraRoomsPrivateReturn18
			}
		}

		void extraRoomsPrivateRoomHInjection11() {
			//AA2Play.exe+174C25 - cmp edx,28
			//AA2Play.exe + 174C28 - jl AA2Play.exe + 174C36
			//AA2Play.exe + 174C2A - cmp edx, 3C

			const DWORD offset1[]{ 0x174C2F };
			extraRoomsPrivateReturn18 = (DWORD*)ExtVars::ApplyRule(offset1);
			const DWORD offset2[]{ 0x174C36 };
			extraRoomsPrivateSuccess10 = (DWORD*)ExtVars::ApplyRule(offset2);
			const DWORD offset3[]{ 0x174C2A };
			extraRoomsPrivateReturn19 = (DWORD*)ExtVars::ApplyRule(offset3);

			DWORD address = General::GameBase + 0x174C25;
			DWORD redirectAddress = (DWORD)(&extraRoomsPrivateRoomHRedirect11);
			Hook((BYTE*)address,
				{ 0x83, 0xFA, 0x28, 0x7C, 0x0C },						//expected values
				{ 0xE9, HookControl::RELATIVE_DWORD, redirectAddress },	//redirect to our function
				NULL);
		}

	}
}
