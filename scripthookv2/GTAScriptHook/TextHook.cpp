#include "Stdafx.h"
#include "TextHook.h"
#include "GameVersion.h"
#include "Hooking.h"

StompHook readGXTHook;

namespace GTA {
	static bool inTextHook = false;

	const char* __stdcall GetGXTString(const char* string) {
		return TextHook::GetString(string);
	}

#pragma unmanaged
	static void __declspec(naked) gxtHookStub()
	{
		__asm {
			mov eax, dword ptr [esp + 8]
			test eax, eax
			jz short l1
			push ecx
			push eax
			call GetGXTString
			pop ecx
			test eax, eax
			jz short l1
			add esp, 4
			ret 4
	l1:
			pop eax
			sub esp, 32
			push esi
			push edi
			jmp eax
		}
	}
#pragma managed

	void TextHook::Install() {
		_accessFunctions = gcnew List<Func<String^, String^>^>();
		_filterFunctions = gcnew List<Func<String^, String^>^>();

		// patch the memory
		PBYTE location = (PBYTE)0x6A0050;

		if (GameVersion::VersionNumber == 101 || GameVersion::VersionNumber == 200) {
			return;
		}

		readGXTHook.initialize("aaaaa", 5, location);
		readGXTHook.installHook(gxtHookStub, false, true);
	}

#pragma unmanaged
	const char* CallOriginalGXT(const char* key, int _VersionNumber) {
		char* retval;

		DWORD function = 0x6A0050;
		DWORD classn = 0xC1B340;

		if (_VersionNumber == 101 || _VersionNumber == 200) {
			return NULL;
		}

		_asm {
			push dword ptr key
			mov ecx, classn
			call dword ptr function
			mov retval, eax
		}

		return retval;
	}
#pragma managed

	String^ TextHook::Get(String^ label) {
		return gcnew String(CallOriginalGXT((char*)Runtime::InteropServices::Marshal::StringToHGlobalAnsi(label).ToPointer(), GameVersion::VersionNumber));
	}

	const char* TextHook::GetString(const char* string) {
		if (inTextHook) {
			return NULL;
		}

		inTextHook = true;

		String^ name = gcnew String(string);
		name = name->Trim();

		String^ retval = "";

		// check who knows this string
		for each (Func<String^, String^>^ function in _accessFunctions) {
			String^ val = function(name);

			if (val != nullptr) {
				//return (char*)Runtime::InteropServices::Marshal::StringToHGlobalAnsi(retval).ToPointer();
				retval = val;
				break;
			}
		}

		if (retval == "") {
			const char* val = CallOriginalGXT(string, GameVersion::VersionNumber);

			if (val == NULL) { inTextHook = false; return NULL; }

			retval = gcnew String(val);
		}

		for each (Func<String^, String^>^ function in _filterFunctions) {
			retval = function(retval);
		}

		inTextHook = false;

		return (char*)Runtime::InteropServices::Marshal::StringToHGlobalAnsi(retval).ToPointer();
	}
}