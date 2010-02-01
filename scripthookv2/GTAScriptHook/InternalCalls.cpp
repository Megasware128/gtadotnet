#include "stdafx.h"
#include "internalcalls.h"
#include "GTAUtils.h"
#include "Log.h"
#include "GameVersion.h"

BYTE ScriptBuf[384];

#ifdef GTA_SA
#pragma unmanaged
int ExecuteBuffer(BYTE *sbuf, DWORD *lvars, DWORD buflen, int gameVersion)
{
	LPVOID ProcessOneCommand = (LPVOID)0x469F00;

	if (gameVersion == 101 || gameVersion == 200) {
		ProcessOneCommand = (LPVOID)0x469F80;
	}

	GTASA_SCRIPT_THREAD gst, *pGst;

	//logbuffer(sbuf, buflen);

	ZeroMemory(&gst, sizeof(gst));
	gst.bUnknown = 0xFF;
	gst.bStartNewScript = 1;
	gst.dwBaseIP = (DWORD) sbuf;
	gst.dwScriptIP = (DWORD) sbuf;
	gst.strName[0] = 'n';
	gst.strName[1] = 'e';
	gst.strName[2] = 't';
	gst.strName[3] = '\0';
	memcpy(gst.dwLocalVar, lvars, 32 * sizeof(DWORD));
	pGst = &gst;
	__asm {
		push ecx
		mov ecx, dword ptr pGst
		call ProcessOneCommand
		pop ecx
	}
	memcpy(lvars, gst.dwLocalVar, 32 * sizeof(DWORD));
	return gst.bJumpFlag;
}
#pragma managed
#endif

namespace GTA {
	namespace Internal {
		static Function::Function() {
			_typeHandlers = gcnew Dictionary<Type^, Func<Object^, Object^>^>();
		}

#ifdef GTA_SCM
		bool Function::Call(unsigned int identifier, ... cli::array<GTA::Internal::Parameter,1> ^parameters) {
			return CallRaw(identifier, parameters);
		}

		generic <typename TReturn>
		TReturn Function::Call(unsigned int identifier, ... cli::array<GTA::Internal::Parameter,1> ^parameters) {
			Object^ retval = Call(identifier, TReturn::typeid, parameters);

			// ask the type handler
			if (_typeHandlers->ContainsKey(TReturn::typeid)) {
				return (TReturn)_typeHandlers[TReturn::typeid](retval);
			}

			return (TReturn)retval;
		}

		Object^ Function::Call(unsigned int identifier, Type^ returnType, ... cli::array<Parameter>^ parameters) {
			//int ptrCount = 1;
			/*
			if (returnType == Vector3::typeid) {

			}
			*/

			int pi = parameters->Length;
			Array::Resize(parameters, parameters->Length + 1);

			parameters[pi] = gcnew VarPointer();
			CallRaw(identifier, parameters);

			//if (ptrCount == 1) {
			return ((VarPointer^)parameters[pi]._preVal)->Value;
			/*} else {
			pi = parameters->Length;
			
			for (; (pi - parameters->Length) < ptrCount; pi++) {
				parameters[pi]->Value;
			}
			}*/

			return nullptr;
		}

		void Function::RegisterType(Type^ type, Func<Object^, Object^>^ handler) {
			_typeHandlers->Add(type, handler);
		}

		bool Function::CallRaw(unsigned int identifier, ... cli::array<GTA::Internal::Parameter,1> ^parameters) {
#ifdef GTA_SCM
			unsigned short scmid = (unsigned short)identifier;

			memset(&ScriptBuf, 0, sizeof(ScriptBuf));
			memcpy(&ScriptBuf, &scmid, 2);
			
			int buf_pos = 2;
			int var_id = 0;
			DWORD LocalVars[32];
			DWORD wait0 = 0x40001U;	// don't know why, but the example code used this.
									// might be to make the game immediately execute the code?
			memset(&LocalVars, 0, sizeof(DWORD) * 32);

			for each (Parameter parameter in parameters) {
				if (parameter._isPointer) {
					LocalVars[var_id] = parameter.ApplyPointer(var_id++);
				}

				size_t length = parameter._internalArray->Length;
				pin_ptr<unsigned char> bytes = &parameter._internalArray[0];
				memcpy(&ScriptBuf[buf_pos], (void*)bytes, length);
				buf_pos += length;

				/*
				// safe way, but slower
				for each (unsigned char byte in parameter._internalArray) {
					ScriptBuf[buf_pos] = byte;
					buf_pos++;
				}
				*/
			}

			memcpy(&ScriptBuf[buf_pos], &wait0, 4); // coincidence: it will end up with 00 01 04 00
			buf_pos += 4;

			/*
			String^ output = "Script buffer: ";

            for (int i = 0; i < buf_pos; i++) {
                    output += ScriptBuf[i].ToString("X2") + " ";
            }

            Log::Debug(output);
			*/

			int result = ExecuteBuffer(ScriptBuf, LocalVars, buf_pos, GameVersion::VersionNumber);

			if (var_id == 0) {
				return (result == 1);
			}

			var_id = 0;

			for each (Parameter parameter in parameters) {
				if (parameter._isPointer) {
					VarPointer^ pointer = (VarPointer^)parameter._preVal;

					pointer->Value = LocalVars[var_id];
					var_id++;
				}
			}

			return (result == 1);
#endif
		}

		Parameter::Parameter(int value) {
			_preVal = value;
			_isPointer = false;

			Byte b0 = (Byte)value;
			Byte b1 = (Byte)(value >> 8);
			Byte b2 = (Byte)(value >> 16);
			Byte b3 = (Byte)(value >> 24);

			_internalArray = gcnew cli::array<unsigned char>(5) { 0x01, b0, b1, b2, b3 }; // 0x01 -> type 32-bit int
		}

		Parameter::Parameter(cli::array<Byte>^ value) {
			_preVal = value;
			_isPointer = false;

			_internalArray = value;
		}

		Parameter::Parameter(float value) {
			_preVal = value;
			_isPointer = false;

			// this is not an int, this is just to be able to access it as an int
			int valueC = GTAUtils::ConvertFloatAsInt(value);

			Byte b0 = (Byte)valueC;
			Byte b1 = (Byte)(valueC >> 8);
			Byte b2 = (Byte)(valueC >> 16);
			Byte b3 = (Byte)(valueC >> 24);

			_internalArray = gcnew cli::array<unsigned char>(5) { 0x06, b0, b1, b2, b3 }; // 0x06 -> type float
		}

		Parameter::Parameter(VarPointer^ value) {
			_isPointer = true;
			_preVal = value;
			_internalArray = gcnew cli::array<unsigned char>(3) { 0x03, 0, 0 };
		}

		int Parameter::ApplyPointer(unsigned short id) {
			if (!_isPointer) {
				return 0;
			}

			Byte b0 = (Byte)id;
			Byte b1 = (Byte)(id >> 8);

			_internalArray = gcnew cli::array<unsigned char>(3) { 0x03, b0, b1 };

			return ((VarPointer^)_preVal)->Value;
		}

		Parameter::Parameter(GlobalVariable^ value) {
			_isPointer = false;
			_preVal = value;

			unsigned short id = (value->ID * 4);

			Byte b0 = (Byte)id;
			Byte b1 = (Byte)(id >> 8);

			_internalArray = gcnew cli::array<unsigned char>(3) { 0x02, b0, b1 };
		}

		Parameter::Parameter(String^ value) {
			_isPointer = false;
			_preVal = value;
#ifdef GTA_VC
			_internalArray = Text::Encoding::ASCII->GetBytes(value->PadRight(8, '\0'));
#else
			_internalArray = gcnew cli::array<unsigned char>(value->Length + 2) { 0x0E };
			_internalArray[1] = value->Length;
			cli::array<unsigned char>^ text = Text::Encoding::ASCII->GetBytes(value);
			Array::Copy(text, 0, _internalArray, 2, value->Length);
#endif
		}

		DWORD* GlobalVariable::GetAddress() {
			DWORD* baseAddress = (DWORD*)0xA49960;

			if (GameVersion::VersionNumber == 101 || GameVersion::VersionNumber == 200) {
				baseAddress = (DWORD*)0xA4BFE0;
			}

			baseAddress += ID;

			return baseAddress;
		}

		int GlobalVariable::Value::get() {
			return *(GetAddress());
		}

		void GlobalVariable::Value::set(int value) {
			DWORD* ad = GetAddress();

			*ad = value;
		}
#endif
	}
}