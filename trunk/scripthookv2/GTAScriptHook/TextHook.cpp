#include "Stdafx.h"
#include "TextHook.h"
#include "GameVersion.h"
#include "Hooking.h"
#include "GTAUtils.h"

StompHook readGXTHook;

namespace GTA {
	static bool inTextHook = false;

	const char* __stdcall GetGXTString(const char* string) {
		return TextHook::GetString(string);
	}

#pragma unmanaged
	static void __declspec(naked) gxtHookReturn()
	{
		__asm {
			sub esp, 32
			push esi
			push edi
			jmp eax
		}
	}

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
			jmp gxtHookReturn
		}
	}

#pragma managed

	void TextHook::Install() {
		if (_accessFunctions == nullptr) {
				_accessFunctions = gcnew List<Func<String^, String^>^>();
		}

		if (_filterFunctions == nullptr) {
				_filterFunctions = gcnew List<Func<String^, String^>^>();
		}

		_keyCache = gcnew Dictionary<String^, IntPtr>();
		_textMemory = 0;

		// patch the memory
		PBYTE location = (PBYTE)0x6A0050;

		if (GameVersion::VersionNumber == 101 || GameVersion::VersionNumber == 200) {
			return;
		}

		BYTE originalCode[5];
		memset(&originalCode, 0, 5);
		memcpy(&originalCode, location, 5);

		GTAUtils::RebaseNearOpcode(originalCode, location, (PBYTE)&gxtHookReturn);

		DWORD oldProtect;
		VirtualProtect(&gxtHookReturn, 5, PAGE_EXECUTE_READWRITE, &oldProtect);
		memcpy(&gxtHookReturn, &originalCode, 5);
		VirtualProtect(&gxtHookReturn, 5, oldProtect, &oldProtect);

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

	void TextHook::RemoveCache(String^ name) {
		if (_keyCache->ContainsKey(name)) {
			//int len = strlen((char*)_keyCache[name].ToPointer());

			Runtime::InteropServices::Marshal::FreeHGlobal(_keyCache[name]);
			_keyCache->Remove(name);

			//_textMemory -= len + 1;
		}
	}

	const char* TextHook::GetString(const char* string) {
		if (inTextHook) {
			return NULL;
		}

		inTextHook = true;

		String^ name = gcnew String(string);
		name = name->Trim();

		String^ retval = "";

		if (name == "_TMEMO") {
			retval = "Text memory: " + (_textMemory / 1024.0f).ToString("F4") + "K";
		}

		if (!_keyCache->ContainsKey(name)) {
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

			IntPtr rvalue = Runtime::InteropServices::Marshal::StringToHGlobalAnsi(retval);
			_keyCache->Add(name, rvalue);

			//_textMemory += retval->Length + 1;

			return (char*)rvalue.ToPointer();
		}

		IntPtr value = _keyCache[name];

		inTextHook = false;

		if (value == IntPtr::Zero) {
			return "";
		}

		return (char*)value.ToPointer();
	}
}