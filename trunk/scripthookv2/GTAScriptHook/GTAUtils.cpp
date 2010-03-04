#include "Stdafx.h"
#include "GTAUtils.h"
#include "Script.h"
#include "InternalCalls.h"

#ifdef GTA_SA
#pragma unmanaged
void PlayerConfigureWeaponSlot(DWORD dwThis, BYTE weaponSlot) {
    DWORD dwFunc = 0x60D000;
    _asm
    {
        mov     ecx, dwThis
        push    weaponSlot
        call    dwFunc
    }
}
#pragma managed
#endif

namespace GTA {
	void GTAUtils::Wait(int time) {
		ScriptContext^ context = ScriptContext::current;

		//Internal::Function::Call(0x0001, 0);

		context->_wakeUpAt = GTAUtils::GetGameTimer() + time;
		context->_continue->Set();
		context->_execute->WaitOne();
	}

#ifdef GTA_SA
	void GTAUtils::SetPlayerWeaponSlot(int slotID) {
		BYTE bslotID = (BYTE)slotID; // to help alignment

		// 0xB7CD98 (first CPlayerInfo) + 4 (CPlayerPedData) + 28 (m_nChosenWeapon)
		*(BYTE *)0xB7CDB8 = bslotID;

		PlayerConfigureWeaponSlot(*(DWORD *)0xB7CD98, slotID);
	}
#endif

	void GTAUtils::RebaseNearOpcode(BYTE* command, BYTE* originalLocation, BYTE* newLocation) {
		if (command[0] == 0xE9 || command[0] == 0xE8) { // jump near and call near
			int originalPointer;
			memcpy(&originalPointer, &command[1], sizeof(int)); // easiest to understand :p

			originalPointer += (DWORD)originalLocation;
			originalPointer -= (DWORD)newLocation;

			memcpy(&command[1], &originalPointer, sizeof(int));
		}
	}
} 