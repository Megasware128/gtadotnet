// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#include <windows.h>

#if GTA_VC | GTA_SA
#define GTA_SCM 1
#else
#define GTA_SCO 1
#endif

using namespace System;
using namespace System::Collections::Generic;

#ifdef GTA_IV
#include "../ScriptHook/ScriptThread.h"
#include "../ScriptHook/Game.h"
#include "../ScriptHook/GameTypes.h"
#include "../ScriptHook/ScriptingHelpers.h"
#endif