#include "Stdafx.h"
#include "VarPointer.h"
#include "GTAUtils.h"

namespace GTA {
	VarPointer::operator float(VarPointer^ source) {
		return GTAUtils::ConvertIntAsFloat(source->Value);
	}

	VarPointer::operator int(VarPointer^ source) {
		return source->Value;
	}

	VarPointer::VarPointer(int value) {
		varStorage[_curID] = value;
		_varID = _curID;
		_curID++;

		if (_curID >= 64) {
			_curID = 0;
		}
	}
}