#include "stdafx.h"
#include "Pool.h"
#include "Log.h"
#include "GTAUtils.h"

namespace GTA {
	Pool::Pool(IntPtr start, DWORD elSize) {
		_pool = (CPool**)start.ToPointer();
		_elSize = elSize;
	}

	int Pool::PtrToHandle(IntPtr pointer) {
		DWORD number = pointer.ToInt32();

		DWORD amount = number - ((DWORD)(*_pool)->objects);
		amount = amount / _elSize;

		signed char unique = (*_pool)->flags[amount];
		return (amount << 8) + unique;
	}

	IntPtr Pool::HandleToPtr(int handle) {
		int index = (handle >> 8);
		int uniqu = (handle & 0xFF);

		if ((*_pool)->flags[index] == uniqu) {
			DWORD_PTR ptr = (*_pool)->objects;
			ptr += (index * _elSize);
			return IntPtr((int)ptr);
		}

		return IntPtr::Zero;
	}

	List<int>^ Pool::GetAllHandles() {
		List<IntPtr>^ pointers = GetAllPointers();
		List<int>^ handles = gcnew List<int>();

		for each (IntPtr pointer in pointers) {
			handles->Add(PtrToHandle(pointer));
		}

		return handles;
	}

	List<IntPtr>^ Pool::GetAllPointers() {
		List<IntPtr>^ pointers = gcnew List<IntPtr>();

		for (int i = (*_pool)->size; i > 0; i--) {
			if ((*_pool)->flags[i] >= 0) {
				//DWORD ptr = *(DWORD*)((*_pool)->objects);
				DWORD_PTR ptr = (*_pool)->objects;
				ptr += (i * _elSize);
				pointers->Add(IntPtr((int)ptr));
			}
		}

		return pointers;
	}

	void Pool::InitializeDefault() {
#ifdef GTA_SA
		_pedPool = gcnew Pool(IntPtr(0xB74490), 1988);
		_vehiclePool = gcnew Pool(IntPtr(0xB74494), 2584);
		_objectPool = gcnew Pool(IntPtr(0xB7449C), 412);
#endif
	}
}