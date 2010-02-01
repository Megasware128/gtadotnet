#pragma once
#include "Script.h"
using namespace System::Threading;

namespace GTA {
	public ref class ScriptProcessor {
	private:
		ScriptProcessor();

		static ScriptProcessor^ _instance;
		static List<ScriptContext^>^ _scripts;

		bool _inLock;

		void WaitForAddScript(Object^ script);
	internal:
		cli::array<Byte>^ _keys;

		void Tick(DWORD timerDelta);
		void AddScript(BaseScript^ script);
		void CheckKeys();
	public:
		event Action<DWORD>^ RawTick;

		static void Initialize() {
			if (_instance != nullptr) {
				return;
			}

			_instance = gcnew ScriptProcessor();
			_scripts = gcnew List<ScriptContext^>();
			_instance->_inLock = false;
			_instance->_keys = gcnew cli::array<Byte>(256);
		}

		property cli::array<Byte>^ KeyBuffer {
			cli::array<Byte>^ get() {
				return _keys;
			}
		}

		static property ScriptProcessor^ Instance {
			ScriptProcessor^ get() {
				Initialize();
				return _instance;
			}
		}
	};
}