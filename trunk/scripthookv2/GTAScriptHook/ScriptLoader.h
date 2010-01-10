#pragma once

#include "Script.h"

using namespace System::Reflection;

namespace GTA {
	public ref class ScriptLoader {
	internal:
		static void LoadScripts();
		static void LoadAssemblies(String^ filter);
	public:
		static void LoadAssembly(Assembly^ assembly);
		static void LoadScript(Script^ script);
	};
}