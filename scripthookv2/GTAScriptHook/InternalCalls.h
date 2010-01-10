#pragma once

#include "VarPointer.h"

namespace GTA {
	namespace Internal {
#ifdef GTA_SCM
		public value class Parameter {
		internal:
			bool _isPointer;
			cli::array<unsigned char>^ _internalArray;
			Object^ _preVal;

			int ApplyPointer(unsigned short id);
		public:
			Parameter(int value);

			static operator Parameter(int value) {
				return Parameter(value);
			}

			Parameter(float value);

			static operator Parameter(float value) {
				return Parameter(value);
			}

			Parameter(String^ value);

			static operator Parameter(String^ value) {
				return Parameter(value);
			}

			Parameter(VarPointer^ value);

			static operator Parameter(VarPointer^ value) {
				return Parameter(value);
			}
		};

		public ref class Function {
		internal:
			static Object^ Call(unsigned int identifier, Type^ returnType, ... cli::array<Parameter>^ parameters);
		public:
			static bool Call(unsigned int identifier, ... cli::array<Parameter>^ parameters);
			generic <typename TReturn>
			static TReturn Call(unsigned int identifier, ... cli::array<Parameter>^ parameters);
		private:
			static bool CallRaw(unsigned int identifier, ... cli::array<Parameter>^ parameters);
		};
#endif
	}
}

struct GTASA_SCRIPT_THREAD	// 0xE0 bytes total
{
	void* pNext;			// 0x00
	void* pPrev;			// 0x04
	char strName[8];		// 0x08
	DWORD dwBaseIP;			// 0x10
	DWORD dwScriptIP;		// 0x14
	DWORD dwReturnStack[8];	// 0x18
	WORD dwStackPointer;	// 0x38
	DWORD dwLocalVar[34];	// 0x3C
	BYTE bStartNewScript;	// 0xC4
	BYTE bJumpFlag;			// 0xC5
	BYTE bIsMissionThread;	// 0xC6
	BYTE bIsExternalScript;	// 0xC7
	BYTE bInMenu;			// 0xC8
	BYTE bUnknown;			// 0xC9
	DWORD dwWakeTime;		// 0xCC
	WORD wIfParam;			// 0xD0
	BYTE bNotFlag;			// 0xD2
	BYTE bWastedBustedCheck;// 0xD3
	BYTE bWastedBustedFlag;	// 0xD4
	DWORD dwSceneSkipIP;	// 0xD8
	BYTE bMissionThread;	// 0xDC
};