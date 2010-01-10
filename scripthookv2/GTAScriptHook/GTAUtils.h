#pragma once

namespace GTA {
	public ref class GTAUtils {
	private:
		GTAUtils() { }
	public:
		static float ConvertIntAsFloat(int integer) {
			float retval;
			memcpy(&retval, &integer, sizeof(float));

			return retval;
		}

		static int ConvertFloatAsInt(float floatval) {
			int retval;
			memcpy(&retval, &floatval, sizeof(int));

			return retval;
		}
	};
}