#include "Stdafx.h"
#include "Script.h"
#include "InternalCalls.h"
#include "ScriptLoader.h"
#include "ScriptProcessor.h"
#include "Log.h"

using namespace System::IO;

namespace GTA {
	Assembly^ ScriptLoader::ResolveAssembly(Object^ sender, ResolveEventArgs^ args) {
		for each (Assembly^ assembly in AppDomain::CurrentDomain->GetAssemblies()) {
			if (assembly->FullName == args->Name) {
				return assembly;
			}
		}

		for each (String^ file in Directory::GetFiles("scripts")) {
			try {
				String^ name = FileInfo(file).FullName;

				if (AssemblyName::GetAssemblyName(name)->FullName == args->Name) {
					return Assembly::LoadFile(name);
				}
			} catch (Exception^) {
				// nothing
			}
		}

		return nullptr;
	}

	void ScriptLoader::LoadScripts() {
		AppDomain::CurrentDomain->AssemblyResolve += gcnew ResolveEventHandler(&ScriptLoader::ResolveAssembly);

		LoadAssemblies("scripts", "*.dll");
		LoadAssemblies(".", "*.net"); // don't think this is going to be used anyway
		LoadAssemblies(".", "GTAScriptAPI.dll");
	}

	void ScriptLoader::LoadAssembly(Assembly^ assembly) {
		try {
			cli::array<Type^>^ types = assembly->GetTypes();

			for each (Type^ type in types) {
				if (type->IsPublic && !type->IsAbstract) {
					try {
						if (type->IsSubclassOf(BaseScript::typeid)) {
							BaseScript^ script = (BaseScript^)Activator::CreateInstance(type);

							LoadScript(script);
						}

						if (type->IsSubclassOf(ScriptInitializer::typeid)) {
							ScriptInitializer^ script = (ScriptInitializer^)Activator::CreateInstance(type);
							script->OnGameStart();
						}
					} catch (Exception^ e) {
						Log::Error("An exception occurred during initialization of the script " + type->Name + ".");
						Log::Error(e);
					}
				}
			}
		} catch (ReflectionTypeLoadException^ e) {
			// likely a non-v2hook script
			Log::Warn("Assembly " + assembly->GetName() + " could not be loaded because of a loader exception (perhaps this is made for an older GTAScriptHook?)");
			Log::Warn("Exception: " + e->LoaderExceptions[0]->ToString()->Split('\n')[0]->Trim());
		}
	}

	void ScriptLoader::LoadAssemblies(String^ folder, String^ filter) {
		cli::array<String^>^ files = Directory::GetFiles(folder, filter);

		for each (String^ file in files) {
			Assembly^ assembly = Assembly::LoadFile(FileInfo(file).FullName);
			
			LoadAssembly(assembly);
		}
	}

	void ScriptLoader::LoadScript(BaseScript^ script) {
		Log::Info("Loading script " + script->GetType()->Name);

		ScriptProcessor::Instance->AddScript(script);
	}
}