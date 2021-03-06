#pragma once

#include "Script.h"

using namespace System::Reflection;

namespace GTA {
	public ref class ScriptInitializer abstract {
	public:
		virtual void OnGameStart() { }
		//virtual void OnScriptStart() { }
	};

	public ref class ScriptPostInitializer abstract {
	public:
		virtual void OnGameStart() { }
		//virtual void OnScriptStart() { }
	};

	public ref class ScriptLoader : public MarshalByRefObject {
	public:
		virtual Object^ InitializeLifetimeService() override {
			return nullptr;
		}
	internal:
		List<String^>^ _loadedAssemblies;
		List<Assembly^>^ loadedAssemblies;
		List<Assembly^>^ referenceAssemblies;
		String^ _rootDirectory;

		bool _gameInitialized;

		void LoadScripts();
		void LoadReferences();
		void UnloadScripts();
		//void UnloadScripts();
		void LoadAssemblies(String^ folder, String^ filter);
		void LoadScriptFiles(String^ folder, String^ filter);

		void Initialize();
		void ProxyTick(DWORD timerDelta);
#if GTA_SA
		const char* ProxyGetString(const char* string);
#else
		const wchar_t* ProxyGetString(const char* string);
#endif

		static Assembly^ ResolveAssembly(Object^ sender, ResolveEventArgs^ args);
	public:
		property bool GameInitialized {
			bool get();
			void set(bool value);
		}

		void LoadAssembly(Assembly^ assembly);
		static void LoadScript(BaseScript^ script);
	};
}