#include "Stdafx.h"
#include "Script.h"
#include "InternalCalls.h"
#include "ScriptLoader.h"
#include "ScriptProcessor.h"
#include "Log.h"

namespace GTA {
	ref class TestScript1 : Script {
	public:
		virtual void Run() override {
			while (true) {
				Internal::Function::Call(0x0394, 1);
				Wait(15000);
			}
		}
	};

	void ScriptLoader::LoadScripts() {
		LoadScript(gcnew TestScript1());
	}

	void ScriptLoader::LoadAssemblies(String^ filter) {

	}

	void ScriptLoader::LoadAssembly(Assembly^ assembly) {

	}

	void ScriptLoader::LoadScript(Script^ script) {
		Log::Info("Loading script " + script->GetType()->Name);

		ScriptProcessor::Instance->AddScript(script);
	}
}