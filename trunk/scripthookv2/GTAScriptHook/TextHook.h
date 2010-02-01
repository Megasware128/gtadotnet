#pragma once

namespace GTA {
	const char* __stdcall GetGXTString(const char* string);

	public ref class TextHook {
	private:
		static List<Func<String^, String^>^>^ _accessFunctions;
		static List<Func<String^, String^>^>^ _filterFunctions;
	internal:
		static void Install();
		static const char* GetString(const char* string);
	public:
		static String^ Get(String^ label);

		static void RegisterCallback(Func<String^, String^>^ function) {
			_accessFunctions->Add(function);
		}

		static void RegisterFilter(Func<String^, String^>^ function) {
			_filterFunctions->Add(function);
		}
	};
}