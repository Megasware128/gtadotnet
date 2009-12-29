#include "stdafx.h"
#include "Log.h"
#include "GameVersion.h"
#include "ScriptProcessor.h"

using namespace System::Reflection;

void GoManaged() {
	// we're really managed now :)

	GTA::Log::Initialize("GTAScriptHook.log", GTA::LogLevel::Debug | GTA::LogLevel::Info | GTA::LogLevel::Warning | GTA::LogLevel::Error);
	GTA::Log::Info("DBNetwork GTA .NET Scripting API (v" + Assembly::GetCallingAssembly()->GetName()->Version->ToString() + ")");
#ifdef GTA_SA
	GTA::Log::Info("compiled for GTA: San Andreas");
#endif

	GTA::GameVersion::Detect();
	GTA::Log::Info("Game version: " + GTA::GameVersion::VersionName);

	GTA::ScriptProcessor::Initialize();
}

#pragma unmanaged

// function to help with some stuff that comes with CLR init
DWORD GoUnmanaged() {
	GoManaged();
	return 0;
}

// DllMain, initializes the managed thread
BOOL APIENTRY DllMain (HINSTANCE hInstance, DWORD reason, LPVOID reserved) {
	switch (reason) {
		case DLL_PROCESS_ATTACH:
			DWORD threadID = 0;
			HANDLE threadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)GoUnmanaged, 0, 0, &threadID);

			if (threadHandle == 0 || threadHandle == INVALID_HANDLE_VALUE) {
				return false;
			}

			CloseHandle(threadHandle);
		break;
	}

	return true;
}