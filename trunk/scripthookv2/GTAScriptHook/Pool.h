#pragma once

namespace GTA {
	private struct CPool {
		DWORD_PTR objects;
		signed char* flags;
		DWORD size;
		DWORD top;
	};

	public ref class Pool {
	private:
		DWORD _elSize;

		CPool** _pool;
		Pool(IntPtr start, DWORD elSize);

		static Pool^ _pedPool;
		static Pool^ _vehiclePool;
		static Pool^ _objectPool;
	internal:
		static void InitializeDefault();

	public:
		List<IntPtr>^ GetAllPointers();
		List<int>^ GetAllHandles();

		IntPtr HandleToPtr(int handle);
		int PtrToHandle(IntPtr pointer);

		static property Pool^ Ped {
			Pool^ get() {
				return _pedPool;
			}
		}

		static property Pool^ Vehicle {
			Pool^ get() {
				return _vehiclePool;
			}
		}

		static property Pool^ Object {
			Pool^ get() {
				return _objectPool;
			}
		}
	};
}