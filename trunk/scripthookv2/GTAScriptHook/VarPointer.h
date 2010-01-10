#pragma once

namespace GTA {
	public ref class VarPointer {
	private:
		static cli::array<int>^ varStorage;
		static int _curID;

		int _varID;
	public:
		static VarPointer() {
			varStorage = gcnew cli::array<int>(64); // 32 * 2
			_curID = 0;
		}

		VarPointer() {
			VarPointer(0);
		}

		VarPointer(int value);

		property int Value {
			int get() {
				return varStorage[_varID];
			}
			void set(int value) {
				varStorage[_varID] = value;
			}
		}

		static explicit operator int (VarPointer^ source);
		static explicit operator float (VarPointer^ source);
	};
}