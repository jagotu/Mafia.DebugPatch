/*
2016.10.13
created by JaGoTu

An asi patch that makes Mafia print all debugging messages in game.
Currently only supports v1.2
*/

#include "CPatch.h"

void Init();
void script_command_hook();




BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
	if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
		Init();
	}
	return TRUE;
}

void Init()
{
	CPatch::SetUChar(0x4637F0, 0x10); //debug_text is a function with one string parameter, not garbage
	CPatch::SetUChar(0x462CF9, 0x7F); //String length is now max 127, not 48
	CPatch::RedirectCall(0x46D3F9, *script_command_hook); //Redirect command processing

}


void __declspec(naked)script_command_hook() {
	static void* sub_47BE10 = (void*)0x47BE10;
	static void* PrintMafiaString = (void*)0x5F9D50;
	static int unk_6BF980 = 0x6BF980;
	_asm {
		mov eax, [ecx + 4];
		mov ecx, [esp + 4];
		lea eax, [eax + ecx * 8];

		PUSHAD;
		PUSHFD;
		mov eax, [eax]
		cmp eax, 0x38;
		jnz NotDebug;

		mov     edx, [ebp + 1Ch];
		mov     ecx, esi;
		push    edx;
		call    sub_47BE10;
		mov     eax, [eax + 4];
		mov		esi, unk_6BF980;
		mov		ecx, esi;

		push 0xFF0000; //DEBUG TEXT COLOR
		push eax;
		call PrintMafiaString;

	NotDebug:
		POPFD;
		POPAD;
		retn 4;
	}
}
