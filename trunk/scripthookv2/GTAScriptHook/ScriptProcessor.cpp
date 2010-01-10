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

	void ScriptProcessor::AddScript(Script^ script) {
		_scripts->Add(gcnew ScriptContext(script));
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

	void ScriptProcessor::Tick(DWORD timerDelta) {
		//Controls::CheckKeys();
		List<ScriptContext^>^ toRemove = gcnew List<ScriptContext^>();

		for each (ScriptContext^ script in _scripts) {
			script->WakeUp();

			if (script->_endNext) {
				toRemove->Add(script);
			}
		}

		for each (ScriptContext^ script in toRemove) {
			script->Clean();
			_scripts->Remove(script);
		}
	}
}