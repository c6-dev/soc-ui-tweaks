#pragma once
#include "../ModUtils/Patterns.h"
#include "calls.h"

HMODULE __stdcall writePatches(HMODULE module) {
	if (module) {
		auto invertScrollwheelZoom = hook::make_module_pattern(module, "B0 01 84 C0 F3 0F 10 47 74 0F 28 D8").get_first(26);
		SafeWrite8((UInt32)invertScrollwheelZoom, 0x7);
		SafeWrite8((UInt32)invertScrollwheelZoom + 5, 0x8);
		SafeWrite8((UInt32)invertScrollwheelZoom + 21, 0x7);

		auto fasterMapZoom = hook::make_module_pattern(module, "F3 0F 10 80 78 01 00 00 F3 0F 5C 80 70 01 00 00 F3 0F 5C CA F3 0F 5E C8").get_first(94);
		UInt32 fasterMapZoomTgt = (UInt32)fasterMapZoom + 48;
		WriteRelJump((UInt32)fasterMapZoom, fasterMapZoomTgt);

	}
	return module;
}

__declspec(naked) void LoadLibraryHook() {
	static const UInt32 retnAddr = 0x4BAAB5;
	static const UInt32 jumpAddr = 0x4BAAEB;
	__asm {
		test eax, eax
		mov [esi], eax
		jne WRITE_PATCHES
		jmp retnAddr
		WRITE_PATCHES:
			push eax
			call writePatches
			jmp jumpAddr
	}
}



