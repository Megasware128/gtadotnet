#pragma once

#include "Script.h"

using namespace System::Reflection;

namespace GTA {
	public ref class ScriptInitializer abstract {
	public:
		virtual void OnGameStart() { }
		//virtual void OnScriptStart() { }
	};

	public ref class ScriptLoader {
	internal:
		static void LoadScripts();
		static void LoadAssemblies(String^ folder, String^ filter);

		static Assembly^ ResolveAssembly(Object^ sender, ResolveEventArgs^ args);
	public:
		static void LoadAssembly(Assembly^ assembly);
		static void LoadScript(BaseScript^ script);
	};
}