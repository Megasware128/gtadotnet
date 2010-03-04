#pragma once

#include "VarPointer.h"

namespace GTA {
	namespace Internal {
#ifdef GTA_SCO
		public ref class Function {
		internal:
			//static Object^ Call(unsigned int identifier, Type^ returnType, ... cli::array<Parameter>^ parameters);
		public:
			static Function();

			static bool Call(unsigned int identifier, ... cli::array<Parameter>^ parameters);
			//generic <typename TReturn>
			//static TReturn Call(unsigned int identifier, ... cli::array<Parameter>^ parameters);

			//static void RegisterType(Type^ type, Func<Object^, Object^>^ handler);
		private:
			static bool CallRaw(unsigned int identifier, ... cli::array<Parameter>^ parameters);

			//static Dictionary<Type^, Func<Object^, Object^>^>^ _typeHandlers;
		};
#endif
#ifdef GTA_SCM
		ref class GlobalVariable;

		public value class Parameter {
		internal:
			bool _isPointer;
			cli::array<unsigned char>^ _internalArray;

			int ApplyPointer(unsigned short id);
		public:
			Object^ _preVal;
			int _dataType;

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

			Parameter(GlobalVariable^ value);

			static operator Parameter(GlobalVariable^ value) {
				return Parameter(value);
			}

			static operator Parameter(bool value) {
				return Parameter(value ? 1 : 0);
			}

			Parameter(cli::array<Byte>^ value);

			static operator Parameter(cli::array<Byte>^ value) {
				return Parameter(value);
			}
		};

		public ref class Function {
		internal:
			static Object^ Call(unsigned int identifier, Type^ returnType, ... cli::array<Parameter>^ parameters);
		public:
			static Function();

			static bool Call(unsigned int identifier, ... cli::array<Parameter>^ parameters);
			generic <typename TReturn>
			static TReturn Call(unsigned int identifier, ... cli::array<Parameter>^ parameters);

			static void RegisterType(Type^ type, Func<Object^, Object^>^ handler);
		private:
			static bool CallRaw(unsigned int identifier, ... cli::array<Parameter>^ parameters);

			static Dictionary<Type^, Func<Object^, Object^>^>^ _typeHandlers;
		};

		public ref class GlobalVariable {
		private:
			unsigned short _varID;

			DWORD* GetAddress();
		public:
			GlobalVariable(unsigned short varID) {
				_varID = varID;
			}

			static GlobalVariable^ Get(unsigned short varID) {
				return gcnew GlobalVariable(varID);
			}

			property unsigned short ID {
				unsigned short get() {
					return _varID;
				}
			}

			property int Value {
				int get();
				void set(int value);
			}
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