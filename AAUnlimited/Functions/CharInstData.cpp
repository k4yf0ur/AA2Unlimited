#include "StdAfx.h"
#include "Files\PersistentStorage.h"



CharInstData::CharInstData() : m_char(NULL) {
}


CharInstData::~CharInstData() {
}

static std::string styleName;

void CharInstData::SetCurrentStyle(int index)
{
	if (index > GetStyleCount()) return;
	m_cardData.SwitchActiveCardStyle(index, this->m_char->m_charData);

	auto storage = PersistentStorage::ClassStorage::getStorage(Shared::GameState::getCurrentClassSaveName());
	storage->storeCardInt(&AAPlay::g_characters[this->m_char->m_seat], L"m_currCardStyle", index);
}

void CharInstData::ApplyDecals(int bodyPart, int decalStrength)
{
	DWORD position = (DWORD)bodyPart;
	DWORD strength = (DWORD)decalStrength;
	const DWORD offset[]{ 0x151900 };
	DWORD* address = (DWORD*)ExtVars::ApplyRule(offset);
	if (this->IsValid()) {
		auto somepointer = *(DWORD*)((char*)(this->m_char->m_somePointer) + 0x13c);
		__asm
		{
			mov eax, position
			mov edi, somepointer
			mov ecx, strength
			push ebp
			push ecx
			call[address]
			pop ebp
		}
	}
}

void CharInstData::AddRelationshipPoints(ExtClass::CharacterStruct* towards, int love, int like, int dislike, int hate)
{
	if (this->IsValid() && General::IsAAPlay) {
		int arr[4] = { love, like, dislike, hate };
		const DWORD offset[]{ 0x1428E0 };
		DWORD* address = (DWORD*)ExtVars::ApplyRule(offset);
		DWORD* firstChar = (DWORD*)this->m_char->m_moreData;
		DWORD* arrayPointer = (DWORD*)arr;
		if (this->IsValid()) {
			auto somepointer = *(DWORD*)((char*)(this->m_char->m_somePointer) + 0x13c);
			__asm
			{
				mov eax, arrayPointer
				push eax
				mov eax, firstChar
				push eax
				mov eax, towards
				call[address]
			}
		}
	}
}

void CharInstData::ClearCache()
{
	const DWORD offset[]{ 0x150750 };
	DWORD* address = (DWORD*)ExtVars::ApplyRule(offset);
	if (this->IsValid()) {
		auto somepointer = *(DWORD*)((char*)(this->m_char->m_somePointer) + 0x13c);
		__asm
		{
			mov esi, somepointer
			push esi
			call[address]

		}
	}
}

void CharInstData::LowPolyUpdate(int state, int contex)
{
	if (this->IsValid() && General::IsAAPlay) {
		DWORD* address;
		ExtClass::CharacterStruct* characterStruct = this->m_char;
		if (characterStruct->m_charData->m_gender) {
			const DWORD offset[]{ 0x116760 };
			address = (DWORD*)ExtVars::ApplyRule(offset);
		}
		else {
			const DWORD offset[]{ 0x10DE50 };
			address = (DWORD*)ExtVars::ApplyRule(offset);
		}
		characterStruct->m_currClothes = contex;
		__asm
		{
			mov ecx, characterStruct
			push 0
			push state
			call[address]
		}
	}
}

void CharInstData::Teleport(DWORD location)
{
	if (this->IsValid()) {
		this->m_forceAction.conversationId = 0;
		this->m_forceAction.movementType = 0;
		this->m_char->m_characterStatus->m_npcStatus->m_status = 0;
		this->m_forceAction.roomTarget = -1;
		this->m_forceAction.target1 = NULL;
		this->m_forceAction.target2 = NULL;
		this->m_forceAction.unknown = -1;
		this->m_forceAction.unknown2 = -1;

		if (!this->IsPC()) {
			DWORD* address;
			ExtClass::CharacterStruct* chrStruct = this->m_char;
			uintptr_t base = reinterpret_cast<uintptr_t>(this->m_char->m_moreData2);
			uintptr_t ptr1 = *reinterpret_cast<uintptr_t*>(base + 0x14);
			uintptr_t CharStructEbx = *reinterpret_cast<uintptr_t*>(base + 0x18);
			const DWORD offset1[]{ 0x3761CC, 0x28, 0x2C };
			DWORD esiVar = *(DWORD*)ExtVars::ApplyRule(offset1);
			DWORD seatOrder;
			const DWORD offset2[]{ 0x3761CC, 0x1C, 0x98, 0x20, this->idxSave * 4, 0xE4 };
			DWORD chrRoomPTR = *(DWORD*)ExtVars::ApplyRule(offset2);
			const DWORD offset3[]{ 0x376164 };
			DWORD classPointer = *(DWORD*)ExtVars::ApplyRule(offset3);
			DWORD function1 = General::GameBase + 0x105880;
			DWORD function2 = General::GameBase + 0x1062B0;
			DWORD function3 = General::GameBase + 0xEC590;
			DWORD function4 = General::GameBase + 0x158420;
			DWORD function5 = General::GameBase + 0x1584C0;
			DWORD function6 = General::GameBase + 0x15F6F0;
			DWORD function7 = General::GameBase + 0x15E3E0;
			DWORD function8 = General::GameBase + 0xE1F20;
			DWORD function9 = General::GameBase + 0x102E60;

			if (ptr1) {
				uintptr_t ediVar = *reinterpret_cast<uintptr_t*>(ptr1 + 0x20);
				__asm
				{
					mov edi, location
					mov esi, edi
					mov ecx, ediVar //this appears to be the current room that the npc is in. It's available through charstruct in ediVar
					call function1
					mov esi, chrRoomPTR
					mov ecx, [esi]
					mov edx, [ecx + 0x08]
					mov eax, [edx + esi + 0x1C]
					push 0x00
					push edi
					call function2
					mov ecx, [esi]
					mov edx, [ecx + 0x04]
					mov[edx + esi + 0xDC], eax
					mov ecx, chrStruct
					mov esi, ecx
					mov edx, [esi + 0x3C]
					mov eax, esiVar
					mov eax, [eax + 0x14]
					push edx
					call function3
					movzx ecx, byte ptr[esi + 0x44]
					mov eax, esiVar
					mov eax, [eax + 0x18]
					push 0x00
					push edi
					call function2 //calculates the room pointer stuff and puts it in eax
					mov esi, eax
					//theres a suspicious mov [esp+24], esi here
					mov ecx, chrRoomPTR
					push ecx
					call function4
					mov edx, chrStruct
					mov eax, [edx + 0x3C]
					push eax
					push esi
					call function9
					mov esi, chrRoomPTR
					mov edi, eax //eax was calculated by the above function
					mov ecx, [edi + 0x08]
					push 0x01
					call function5
					mov eax, chrRoomPTR
					//here it hits the jump I killed

					//this is the model unloading
					mov eax, 0x4
					mov esi, esiVar
					push eax
					mov ebx, CharStructEbx
					mov eax, ebx
					mov edx, esi
					call function6
					mov eax, [edi + 0x08]
					push esi
					mov esi, ebx
					call function7

					//this is the ending sequence
					mov ebx, CharStructEbx
					mov eax, ebx
					call function8
				}
			}
		}
		else {
			DWORD* address;
			ExtClass::CharacterStruct* chrStruct = this->m_char;
			uintptr_t base = reinterpret_cast<uintptr_t>(this->m_char->m_moreData2);
			uintptr_t ptr1 = *reinterpret_cast<uintptr_t*>(base + 0x14);
			uintptr_t CharStructEbx = *reinterpret_cast<uintptr_t*>(base + 0x18);
			const DWORD offset1[]{ 0x3761CC, 0x28, 0x2C };
			DWORD esiVar = *(DWORD*)ExtVars::ApplyRule(offset1);
			DWORD seatOrder;
			const DWORD offset2[]{ 0x3761CC, 0x1C, 0x98, 0x20, this->idxSave * 4, 0xE4 };
			DWORD chrRoomPTR = *(DWORD*)ExtVars::ApplyRule(offset2);
			const DWORD offset3[]{ 0x376164 };
			auto roomno = *(((int*)this->m_char->m_npcData->roomPtr) + 5);
			DWORD classPointer = *(DWORD*)ExtVars::ApplyRule(offset3);
			DWORD function1 = General::GameBase + 0x105880;
			DWORD function2 = General::GameBase + 0x1062B0;
			DWORD function3 = General::GameBase + 0xEC590;
			DWORD function4 = General::GameBase + 0x158420;
			DWORD function5 = General::GameBase + 0x1584C0;
			DWORD function6 = General::GameBase + 0x15F6F0;
			DWORD function7 = General::GameBase + 0x15E3E0;
			DWORD function8 = General::GameBase + 0xE1F20;
			DWORD function9 = General::GameBase + 0x102E60;
			DWORD function10 = General::GameBase + 0x48FA0;
			DWORD function11 = General::GameBase + 0x8AC70;
			DWORD function12 = General::GameBase + 0x1570E0;
			DWORD function13 = General::GameBase + 0x1001C0;
			DWORD function14 = General::GameBase + 0x100D90;
			DWORD function15 = General::GameBase + 0x164DE0;
			DWORD function16 = General::GameBase + 0x15C0B0;
			DWORD function17 = General::GameBase + 0xF18F0;
			DWORD function18 = General::GameBase + 0x105930;
			DWORD function19 = General::GameBase + 0x103220;


			const DWORD offset4[]{ 0x3761CC, 0x28, 0x2C };
			DWORD weirdECX = *(DWORD*)ExtVars::ApplyRule(offset4);
			if (ptr1) {
				uintptr_t ediVar = *reinterpret_cast<uintptr_t*>(ptr1 + 0x20);
				__asm
				{
					// unloading current room and assets
					mov ecx, ediVar
					mov eax, roomno
					push ecx
					call function18

					mov edx, location
					mov esi, edx
					mov ecx, ediVar
					call function1

					mov edi, ediVar
					mov eax, [edi + 0x34]
					push eax
					call function19

					/*mov edi, chrStruct
					mov ecx, ediVar
					push ecx
					call function10*/
					mov ecx, [edi + 0x28]
					mov eax, [ecx + 0x6B8]
					add eax, [ecx + 0x6B0]
					add ecx, 0x6AC
					cdq
					sub eax, edx
					mov esi, eax
					sar esi, 1
					call function11
					mov esi, CharStructEbx
					mov ecx, [esi + 0xE4]
					mov eax, [ecx]
					mov eax, [eax + 0x08]
					add eax, ecx
					call function12
					//

					mov edi, location
					mov esi, edi
					mov ecx, ediVar //this appears to be the current room that the npc is in. It's available through charstruct in ediVar
					call function1
					mov esi, chrRoomPTR
					mov ecx, [esi]
					mov edx, [ecx + 0x08]
					mov eax, [edx + esi + 0x1C]
					push 0x00
					push edi
					call function2
					mov ecx, [esi]
					mov edx, [ecx + 0x04]
					mov[edx + esi + 0xDC], eax

					//for bg change
					mov ecx, esiVar
					mov esi, [ecx + 0x18]
					mov eax, [esi + 0x34]
					push 0x00
					mov edi, location
					mov esi, ediVar
					mov eax, esi
					push edi
					call function2
					push eax
					mov[esi + 0x34], eax
					call function13
					mov edx, [esi + 0x34]
					push edx
					call function14
					mov eax, esiVar
					mov eax, [eax + 0x2C]
					push eax
					mov edi, location
					mov ecx, edi
					call function15

					//

					mov ecx, chrStruct
					mov esi, ecx
					mov edx, [esi + 0x3C]
					mov eax, esiVar
					mov eax, [eax + 0x14]
					push edx
					call function3
					movzx ecx, byte ptr[esi + 0x44]
					mov eax, esiVar
					mov eax, [eax + 0x18]
					push 0x00
					push edi
					call function2 //calculates the room pointer stuff and puts it in eax
					mov esi, eax
					//theres a suspicious mov [esp+24], esi here
					mov ecx, chrRoomPTR
					push ecx
					call function4
					mov edx, chrStruct
					mov eax, [edx + 0x3C]
					push eax
					push esi
					call function9
					mov esi, chrRoomPTR
					mov edi, eax //eax was calculated by the above function
					mov ecx, [edi + 0x08]
					push 0x01
					call function5
					mov eax, chrRoomPTR
					//here it hits the jump I killed

					//this is the model unloading
					mov eax, 0x4
					mov esi, esiVar
					push eax
					mov ebx, CharStructEbx
					mov eax, ebx
					mov edx, esi
					call function6
					mov eax, [edi + 0x08]
					push esi
					mov esi, ebx
					call function7

					mov edi, weirdECX
					push 0x01
					push edi
					call function16

					mov eax, classPointer
					mov eax, [eax + 0x1C]
					push eax
					call function17

					//this is the ending sequence
					mov ebx, CharStructEbx
					mov eax, ebx
					call function8


				}
			}
		}
	}
}


void CharInstData::ArrangeDate(int targetSeat)
{
	if (this->IsValid() && General::IsAAPlay && AAPlay::g_characters[targetSeat].IsValid()) {
		DWORD* firstFunction;
		const DWORD offset[]{ 0x119900 };
		firstFunction = (DWORD*)ExtVars::ApplyRule(offset);
		auto fromCard = this->m_char->m_characterStatus;
		int fromTarget = targetSeat;

		DWORD* secondFunction;
		const DWORD offset2[]{ 0x119CE0 };
		secondFunction = (DWORD*)ExtVars::ApplyRule(offset2);
		auto towardsCard = AAPlay::g_characters[targetSeat].m_char->m_characterStatus;
		int towardsTarget = this->m_char->m_seat;

		__asm
		{
			mov ecx, fromTarget
			push ecx
			mov edx, fromCard
			call[firstFunction]
			mov ecx, towardsTarget
			push ecx
			mov edx, towardsCard
			call[secondFunction]
		}
	}
}

void CharInstData::PromiseLewd(int targetSeat)
{
	if (this->IsValid() && General::IsAAPlay && AAPlay::g_characters[targetSeat].IsValid()) {
		DWORD* firstFunction;
		const DWORD offset[]{ 0x119640 };
		firstFunction = (DWORD*)ExtVars::ApplyRule(offset);
		auto fromCard = this->m_char->m_characterStatus;
		int fromTarget = targetSeat;

		DWORD* secondFunction;
		const DWORD offset2[]{ 0x1194A0 };
		secondFunction = (DWORD*)ExtVars::ApplyRule(offset2);
		auto towardsCard = AAPlay::g_characters[targetSeat].m_char->m_characterStatus;
		int towardsTarget = this->m_char->m_seat;

		__asm
		{
			mov ecx, fromTarget
			push ecx
			mov edx, fromCard
			push edx
			call[firstFunction]
			mov ecx, towardsTarget
			push ecx
			mov edx, towardsCard
			push edx
			call[secondFunction]
		}
	}
}



void CharInstData::SetHeadTracking(int headtracking)
{
	//Set it to 1 to disable headtracking
	//2 enables headtracking
	//4 makes the girl avoid your gaze
	if (this->IsValid()) {
		if (this->m_char->m_xxSkeleton) {
			DWORD* somepointer = (DWORD*)((char*)(this->m_char->m_xxSkeleton->m_unknown13) + 0x88);
			const DWORD offset[]{ 0x1C9DD0 };
			DWORD* address = (DWORD*)ExtVars::ApplyRule(offset);
			__asm
			{
				mov eax, somepointer
				mov ecx, headtracking
				call[address]
			}
		}
	}
}


void CharInstData::AddShadows(DWORD* xxPTR)
{
	if (!g_Config.shadowMap) return;
	if (!xxPTR) return;
	if (this->IsValid()) {
		if (this->m_char->m_xxSkeleton) {
			//an IsValid() check would be nice to have, but that fucks with the display tab in maker 
			auto charPTR = this->m_char;
			DWORD* firstFunction;
			DWORD* secondFunction;
			if (General::IsAAPlay) {
				const DWORD offset[]{ 0x1AFD90 };
				firstFunction = (DWORD*)ExtVars::ApplyRule(offset);
				const DWORD offset1[]{ 0x21C9A0 };
				secondFunction = (DWORD*)ExtVars::ApplyRule(offset1);
			}
			else if (General::IsAAEdit) {
				const DWORD offset[]{ 0x192930 };
				firstFunction = (DWORD*)ExtVars::ApplyRule(offset);
				const DWORD offset1[]{ 0x1FEA70 };
				secondFunction = (DWORD*)ExtVars::ApplyRule(offset1);
			}
			__asm
			{
				mov esi, charPTR
				mov edi, xxPTR
				call[firstFunction]
				mov eax, xxPTR
				mov ecx, [eax + 0x218]
				mov esi, [ecx + 0x0C]
				xor edx, edx
				jmp JumpHere

			JumpHere:
				push 0x01
				push esi
				call[secondFunction]
				inc edx
				add esp, 0x08
				add esi, 0x000042F4
				cmp edx, [ecx + 0x08]
				jl JumpHere
			}
		}
	}
}

void CharInstData::CastShadows(DWORD* xxPTR)
{
	if (!g_Config.shadowMap) return;
	if (!xxPTR) return;
	if (this->IsValid() && General::IsAAPlay) {
		if (this->m_char->m_xxSkeleton) {
			DWORD* firstFunction;
			DWORD* somePTR;
			DWORD* nptr = nullptr;
			DWORD** aptr = &nptr;
			DWORD ** pointerToXXPTR = &xxPTR;

			const DWORD offset[]{ 0x1AFFD0 };
			firstFunction = (DWORD*)ExtVars::ApplyRule(offset);
			const DWORD offset2[]{ 0x3A6748, 0x30, 0x00, 0x18 };
			somePTR = (DWORD*)ExtVars::ApplyRule(offset2);
			__asm
			{
				mov eax, pointerToXXPTR
				push eax
				mov ecx, aptr
				push ecx
				mov eax, somePTR
				call[firstFunction]
			}
		}
	}
}


int CharInstData::GetStyleCount()
{
	return m_cardData.m_styles.size();
}

int CharInstData::GetCurrentStyle()
{
	return m_cardData.m_currCardStyle;
}

const char* CharInstData::GetStyleName(int index)
{
	if (index > GetStyleCount()) return "";
	styleName = General::CastToString(m_cardData.m_styles[index].m_name);
	return styleName.c_str();
}

ExtClass::Light* CharInstData::GetLightArray()
{
	if (this->IsValid() && this->m_char->m_xxSkeleton != NULL) {
		auto result = this->m_char->m_xxSkeleton->m_lightsArray;
		return result;
	}
	return nullptr;
}

int CharInstData::GetLightIndex(const char* name)
{
	if (this->IsValid() && this->m_char->m_xxSkeleton != NULL) {
		for (int i = 0; i < this->m_char->m_xxSkeleton->m_lightsCount; i++) {
			//if (strlen(name) != strlen(this->m_char->m_xxSkeleton->m_lightsArray[i].m_name)) continue;
			if (!strcmp(this->m_char->m_xxSkeleton->m_lightsArray[i].m_name, name)) {
				return i;
			}
		}
	}
	return -1;
}

void CharInstData::SetLightMaterialColor(int light, int materialSlot, float red, float green, float blue, float alpha)
{
	if (this->IsValid() && this->m_char->m_xxSkeleton != NULL) {
		if (light < this->m_char->m_xxSkeleton->m_lightsCount) {
			if (materialSlot < this->m_char->m_xxSkeleton->m_lightsArray[light].m_materialCount) {
				this->m_char->m_xxSkeleton->m_lightsArray[light].m_material[materialSlot].m_materialRed = red;
				this->m_char->m_xxSkeleton->m_lightsArray[light].m_material[materialSlot].m_materialGreen = green;
				this->m_char->m_xxSkeleton->m_lightsArray[light].m_material[materialSlot].m_materialBlue = blue;
				this->m_char->m_xxSkeleton->m_lightsArray[light].m_material[materialSlot].m_materialAlpha = alpha;
			}
		}
	}
}

void CharInstData::SetLightDirection(int light, float x, float y, float z, float w)
{
	if (this->IsValid() && this->m_char->m_xxSkeleton != NULL) {
		if (light < this->m_char->m_xxSkeleton->m_lightsCount) {
			this->m_char->m_xxSkeleton->m_lightsArray[light].m_lightMatrix[2][0] = x;
			this->m_char->m_xxSkeleton->m_lightsArray[light].m_lightMatrix[2][1] = y;
			this->m_char->m_xxSkeleton->m_lightsArray[light].m_lightMatrix[2][2] = z;
			this->m_char->m_xxSkeleton->m_lightsArray[light].m_lightMatrix[2][3] = w;
		}
	}
}

void CharInstData::SetAmbientColor(BYTE red, BYTE green, BYTE blue)
{
	if (this->IsValid()) {
		ExtClass::XXFile* xxlist[] = {
			this->m_char->m_xxFace, this->m_char->m_xxGlasses, this->m_char->m_xxFrontHair, this->m_char->m_xxSideHair,
			this->m_char->m_xxBackHair, this->m_char->m_xxHairExtension, this->m_char->m_xxTounge, this->m_char->m_xxSkeleton,
			this->m_char->m_xxBody, this->m_char->m_xxLegs, this->m_char->m_xxSkirt
		};
		if (this->m_char->m_charData->m_gender == 0) {
			xxlist[10] = NULL;
		}

		for (ExtClass::XXFile* file : xxlist) {
			if (file == NULL) continue;
			file->m_ambientLightBlue = blue;
			file->m_ambientLightGreen = green;
			file->m_ambientLightRed = red;
		}
	}
}

std::vector<float> CharInstData::GetLightMaterialColor(int light, int materialSlot)
{
	if (this->IsValid() && this->m_char->m_xxSkeleton != NULL) {
		if (light < this->m_char->m_xxSkeleton->m_lightsCount) {
			if (materialSlot < this->m_char->m_xxSkeleton->m_lightsArray[light].m_materialCount) {
				auto r = this->m_char->m_xxSkeleton->m_lightsArray[light].m_material[materialSlot].m_materialRed;
				auto g = this->m_char->m_xxSkeleton->m_lightsArray[light].m_material[materialSlot].m_materialGreen;
				auto b = this->m_char->m_xxSkeleton->m_lightsArray[light].m_material[materialSlot].m_materialBlue;
				auto a = this->m_char->m_xxSkeleton->m_lightsArray[light].m_material[materialSlot].m_materialAlpha;
				return std::vector<float>({ r, g, b, a });
			}
		}
	}
	return std::vector<float>();
}

std::vector<float> CharInstData::GetLightDirection(int light)
{
	if (this->IsValid() && this->m_char->m_xxSkeleton != NULL) {
		if (light < this->m_char->m_xxSkeleton->m_lightsCount) {
			auto x = this->m_char->m_xxSkeleton->m_lightsArray[light].m_lightMatrix[2][0];
			auto y = this->m_char->m_xxSkeleton->m_lightsArray[light].m_lightMatrix[2][1];
			auto z = this->m_char->m_xxSkeleton->m_lightsArray[light].m_lightMatrix[2][2];
			auto w = this->m_char->m_xxSkeleton->m_lightsArray[light].m_lightMatrix[2][3];
			return std::vector<float>({ x, y, z, w });
		}
	}
	return std::vector<float>();
}

std::vector<BYTE> CharInstData::GetAmbientColor()
{
	if (this->IsValid() && this->m_char->m_xxSkeleton != NULL) {
		auto r = this->m_char->m_xxSkeleton->m_ambientLightRed;
		auto g = this->m_char->m_xxSkeleton->m_ambientLightGreen;
		auto b = this->m_char->m_xxSkeleton->m_ambientLightBlue;
		return std::vector<BYTE>({ r, g, b });
	}
	return std::vector<BYTE>();
}

int CharInstData::GetLoveTowards(CharInstData* towards)
{
	if (!this->IsValid()) return 0;
	if (!towards->IsValid()) return 0;
	if (this == towards) return 0;

	auto* relations = this->m_char->GetRelations();
	auto* it = relations->m_start;
	for (it; it != relations->m_end; it++) {
		if (it->m_targetSeat == towards->m_char->m_seat) break;
	}
	if (it == relations->m_end) return 0;

	return it->m_lovePoints + it->m_loveCount * 30;
}

int CharInstData::GetLikeTowards(CharInstData* towards)
{
	if (!this->IsValid()) return 0;
	if (!towards->IsValid()) return 0;
	if (this == towards) return 0;

	auto* relations = this->m_char->GetRelations();
	auto* it = relations->m_start;
	for (it; it != relations->m_end; it++) {
		if (it->m_targetSeat == towards->m_char->m_seat) break;
	}
	if (it == relations->m_end) return 0;

	return it->m_likePoints + it->m_likeCount * 30;
}

int CharInstData::GetDislikeTowards(CharInstData* towards)
{
	if (!this->IsValid()) return 0;
	if (!towards->IsValid()) return 0;
	if (this == towards) return 0;

	auto* relations = this->m_char->GetRelations();
	auto* it = relations->m_start;
	for (it; it != relations->m_end; it++) {
		if (it->m_targetSeat == towards->m_char->m_seat) break;
	}
	if (it == relations->m_end) return 0;

	return it->m_dislikePoints + it->m_dislikeCount * 30;
}

int CharInstData::GetHateTowards(CharInstData* towards)
{
	if (!this->IsValid()) return 0;
	if (!towards->IsValid()) return 0;
	if (this == towards) return 0;

	auto* relations = this->m_char->GetRelations();
	auto* it = relations->m_start;
	for (it; it != relations->m_end; it++) {
		if (it->m_targetSeat == towards->m_char->m_seat) break;
	}
	if (it == relations->m_end) return 0;

	return it->m_hatePoints + it->m_hateCount * 30;
}

void CharInstData::SetPointsTowards(CharInstData* towards, float love, float like, float dislike, float hate, float spare)
{
	if (!this->IsValid()) return;
	if (!towards->IsValid()) return;
	if (this == towards) return;

	auto* ptrRel = this->m_char->GetRelations();
	auto* rel = ptrRel->m_start;
	if (ptrRel == NULL) return;
	for (rel; rel != ptrRel->m_end; rel++) {
		if (rel->m_targetSeat == towards->m_char->m_seat) {
			break;
		}
	}
	if (rel == ptrRel->m_end) return;	//if we didn't find the relationship data for the target we do nothing

	//normalize the points
	float ptsSum = love + like + dislike + hate + spare;
	float normalizer;
	if (ptsSum > 0)
	{
		normalizer = 900.0f / ptsSum;
	}
	else
	{
		normalizer = 0.0f;
	}
	love *= normalizer;
	like *= normalizer;
	dislike *= normalizer;
	hate *= normalizer;

	//nuke old relationship data
	rel->m_loveCount = 0;
	rel->m_likeCount = 0;
	rel->m_dislikeCount = 0;
	rel->m_hateCount = 0;

	rel->m_love = 2;
	rel->m_like = 2;
	rel->m_dislike = 2;
	rel->m_hate = 2;

	rel->m_lovePoints = 0;
	rel->m_likePoints = 0;
	rel->m_dislikePoints = 0;
	rel->m_hatePoints = 0;

	rel->m_poin = 0;

	rel->m_actionBacklog.m_end = rel->m_actionBacklog.m_start;

	//apply the points, discard the decimals
	rel->m_lovePoints = love;
	rel->m_likePoints = like;
	rel->m_dislikePoints = dislike;
	rel->m_hatePoints = hate;

	AAPlay::ApplyRelationshipPoints(this->m_char, rel);
}

int CharInstData::GetCurrentRoom()
{
	if (this->m_char->m_npcData != nullptr) {
		if (this->m_char->m_npcData->roomPtr != nullptr) {
			if ((((int*)this->m_char->m_npcData->roomPtr) + 5) != nullptr) {
				auto roomno = *(((int*)this->m_char->m_npcData->roomPtr) + 5);
				return roomno;
			}
			else return -1;
		}
		else return -1;
	}
	else return -1;
}

CharInstData* CharInstData::GetTargetInst()
{
	for (int character = 0; character < 25; character = character + 1) {
		CharInstData* inst2 = &AAPlay::g_characters[character];
		if (inst2->IsValid()) {
			if (inst2->m_char->m_npcData == this->m_char->m_npcData->m_target) {
				return inst2;
			}
		}
	}
	return nullptr;
}

bool CharInstData::IsValid()
{
	if (General::IsAAEdit) return (AAEdit::g_currChar.m_char != NULL || Editable());
	ExtClass::CharacterStruct** start = ExtVars::AAPlay::ClassMembersArray();
	ExtClass::CharacterStruct** end = ExtVars::AAPlay::ClassMembersArrayEnd();
	for (start; start != end; start++) {
		ExtClass::CharacterStruct* it = *start;
		if (it == m_char) {
			return m_char->m_seat + 1;
		}
	}
	return false;
}

bool CharInstData::IsPC() {
	if (!this->IsValid()) return false;
	return (Shared::GameState::getPlayerCharacter())->m_char->m_seat == this->m_char->m_seat;
}

void CharInstData::Respawn() {
	if (this->Editable()) {
		auto clip = this->m_char->m_xxSkeleton->m_poseNumber;
		this->m_char->Spawn(this->m_char->m_clothState, this->m_char->m_materialSlot, 0, 1);
		auto pp = L"data\\jg2e01_00_00.pp";
		wchar_t xa[255];
		auto strGender = this->m_char->m_charData->m_gender == 0 ? L"S" : L"A";
		int figure = this->m_char->m_charData->m_figure.height;
		if (this->m_char->m_charData->m_gender == 0) {	//male
			figure = this->m_char->m_charData->m_figure.height + this->m_char->m_charData->m_figure.figure - 1;
			//switch (g_currChar.m_char->m_charData->m_figure.height)
			//{
			//case 1:
			//	if (g_currChar.m_char->m_charData->m_figure.figure == 0) {
			//		//Delicate
			//		figure = 00;
			//	}
			//	if (g_currChar.m_char->m_charData->m_figure.figure == 1) {
			//		//Normal
			//		figure = 01;
			//	}
			//case 2:
			//	if (g_currChar.m_char->m_charData->m_figure.figure == 1) {
			//		//Tall
			//		figure = 02;
			//	}
			//	if (g_currChar.m_char->m_charData->m_figure.figure == 2) {
			//		//Fat
			//		figure = 03;
			//	}
			//}
		}
		swprintf(xa, L"data\\H%sE00_00_%02d_00.xa", strGender, figure);
		this->m_char->LoadXA(pp, xa, clip, 0, 0);
	}
}

void CharInstData::Reset() {
	m_char = NULL; //pointer pointing to the illusion data, now invalid
	m_cardData.RemoveAllHair();
	m_cardData.Reset();
	//for (int i = 0; i < 4; i++) m_hairs[i].clear(); //TODO: proper cleanup
}

void CharInstData::StoreInitialStats()
{
	auto storage = PersistentStorage::ClassStorage::getStorage(Shared::GameState::getCurrentClassSaveName());

	storage->storeCardAAUDataInt(this, L"sociability", this->m_char->m_charData->m_character.sociability);
	storage->storeCardAAUDataInt(this, L"virtue", this->m_char->m_charData->m_character.virtue);
	storage->storeCardAAUDataInt(this, L"strengthValue", this->m_char->m_charData->m_character.strengthValue);
	storage->storeCardAAUDataInt(this, L"intelligenceValue", this->m_char->m_charData->m_character.intelligenceValue);
	storage->storeCardAAUDataInt(this, L"clubValue", this->m_char->m_charData->m_character.clubValue);
}

