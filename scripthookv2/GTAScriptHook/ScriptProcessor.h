#include "Script.h"
using namespace System::Threading;

namespace GTA {
	public ref class ScriptProcessor {
	private:
		ScriptProcessor();

		static ScriptProcessor^ _instance;
		static List<ScriptContext^>^ _scripts;
	internal:
		void Tick(DWORD timerDelta);
	public:
		static void Initialize() {
			if (_instance != nullptr) {
				return;
			}

			_instance = gcnew ScriptProcessor();
			_scripts = gcnew List<ScriptContext^>();
		}

		static property ScriptProcessor^ Instance {
			ScriptProcessor^ get() {
				return _instance;
			}
		}
	};
}