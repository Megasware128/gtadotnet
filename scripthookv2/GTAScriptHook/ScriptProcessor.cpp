#include "stdafx.h"
#include "gameversion.h"
#include "ScriptProcessor.h"
#include "Log.h"
#include "Hooking.h"

#ifndef GTA_SCO
CallHook scriptTickHook;
#endif

namespace GTA {
#ifdef GTA_SA
	void __stdcall ScriptTickHandler(DWORD timerDelta) {
		ScriptProcessor::Instance->Tick(timerDelta);
	}

	#pragma unmanaged
	static void __declspec(naked) scriptHandlerStub()
	{
		__asm {
			push dword ptr [esp + 20]
			push scriptTickHook.pOriginal
			jmp ScriptTickHandler
		}
	}
	#pragma managed
#endif

	void ScriptProcessor::AddScript(BaseScript^ script) {
		if (!_inLock) {
			_scripts->Add(gcnew ScriptContext(script));
		} else {
			ThreadPool::QueueUserWorkItem(gcnew WaitCallback(this, &ScriptProcessor::WaitForAddScript), script);
		}
	}

	void ScriptProcessor::WaitForAddScript(Object^ script) {
		while (_inLock) {
			Thread::Sleep(0);
		}

		AddScript((BaseScript^)script);
	}

	ScriptProcessor::ScriptProcessor() {
#ifdef GTA_SA
		BYTE* patchAddress = (BYTE*)0x46A22E;

		if (GameVersion::VersionNumber == 101 || GameVersion::VersionNumber == 200) {
			patchAddress = (BYTE*)0x46A2AE;
		}

		scriptTickHook.initialize("\xE8\xCD\x6A\x12", patchAddress);
		scriptTickHook.installHook(scriptHandlerStub, true);
#endif
	}

	void ScriptProcessor::CheckKeys() {
		for (int x = 0; x < 256; x++) {
			_keys[x] = (char) (GetAsyncKeyState(x) >> 8);
		}
	}

	void ScriptProcessor::Tick(DWORD timerDelta) {
		CheckKeys();
		List<ScriptContext^>^ toRemove = gcnew List<ScriptContext^>();

		_inLock = true;
		for each (ScriptContext^ script in _scripts) {
			script->WakeUp();

			if (script->_endNext) {
				toRemove->Add(script);
			}
		}
		_inLock = false;

		for each (ScriptContext^ script in toRemove) {
			script->Clean();
			_scripts->Remove(script);
		}

		RawTick(timerDelta);
	}
}