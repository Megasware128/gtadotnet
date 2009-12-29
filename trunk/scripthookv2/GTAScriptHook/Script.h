using namespace System::Threading;

namespace GTA {
	public ref class TickScript {
	private:
		int _interval;
	public:
		TickScript() { _interval = 0; }

		property int Interval {
			int get() {
				return _interval;
			}
			void set(int value) {
				_interval = value;
			}
		}

		event Action^ Tick;

		virtual void OnTick() {
			Tick();
		}

		void Run();
	};

	public ref class Script {
	public:
		void OnStart() { }
		virtual void Run();
	};

	private ref class ScriptContext {
	private:
		void HandleException(Exception^ e);
	internal:
		static ScriptContext^ current;

		bool _endNext;

		Thread^ _thread;
		EventWaitHandle^ _execute;
		EventWaitHandle^ _continue;
		Script^ _myScript;
		DWORD _wakeUpAt;

		String^ _identifier;

		ScriptContext(Script^ script);

		void WakeUp();
		void Run();
		void Clean();
	};
}