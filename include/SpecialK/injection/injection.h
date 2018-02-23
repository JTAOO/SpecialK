/**
 * This file is part of Special K.
 *
 * Special K is free software : you can redistribute it
 * and/or modify it under the terms of the GNU General Public License
 * as published by The Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * Special K is distributed in the hope that it will be useful,
 *
 * But WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Special K.
 *
 *   If not, see <http://www.gnu.org/licenses/>.
 *
**/

#ifndef __SK__INJECTION_H__
#define __SK__INJECTION_H__

#include <SpecialK/window.h>
#include <SpecialK/core.h>
#include <SpecialK/render/backend.h>

LRESULT
CALLBACK
CBTProc (int nCode, WPARAM wParam, LPARAM lParam);

void __stdcall SKX_InstallCBTHook (void);
void __stdcall SKX_RemoveCBTHook  (void);
bool __stdcall SKX_IsHookingCBT   (void);

size_t __stdcall SKX_GetInjectedPIDs (DWORD* pdwList, size_t capacity);

bool
SK_Inject_SwitchToGlobalInjector (void);

bool
SK_Inject_SwitchToGlobalInjectorEx (DLL_ROLE role);

bool
SK_Inject_SwitchToRenderWrapper (void);

bool
SK_Inject_SwitchToRenderWrapperEx (DLL_ROLE role);

// Are we capable of injecting into admin-elevated applications?
bool
SK_Inject_IsAdminSupported (void);


bool
SK_Inject_TestWhitelists (const wchar_t* wszExecutable);


// Internal use only
//
void
SK_Inject_ReleaseProcess (void);

void
SK_Inject_AcquireProcess (void);


#define MAX_INJECTED_PROCS        16
#define MAX_INJECTED_PROC_HISTORY 64

extern "C"
{
struct SK_InjectionRecord_s
{
  struct {
    wchar_t    name [MAX_PATH] =  { 0 };
    DWORD      id              =    0;
    __time64_t inject          = 0ULL;
    __time64_t eject           = 0ULL;
  } process;

  struct {
    SK_RenderAPI api    = SK_RenderAPI::Reserved;
    ULONG64      frames = 0ULL;
  } render;

  // Use a bitmask instead of this stupidness
  struct {
    bool xinput       = false;
    bool raw_input    = false;
    bool direct_input = false;
    bool hid          = false;
    bool steam        = false;
  } input;

  static __declspec (dllexport) volatile LONG count;
  static __declspec (dllexport) volatile LONG rollovers;
};
  extern __declspec (dllexport) wchar_t g_LastBouncedModule0 [MAX_PATH + 1];
  extern __declspec (dllexport) wchar_t g_LastBouncedModule1 [MAX_PATH + 1];
  extern __declspec (dllexport) wchar_t g_LastBouncedModule2 [MAX_PATH + 1];
  extern __declspec (dllexport) wchar_t g_LastBouncedModule3 [MAX_PATH + 1];
  extern __declspec (dllexport) wchar_t g_LastBouncedModule4 [MAX_PATH + 1];
  extern __declspec (dllexport) wchar_t g_LastBouncedModule5 [MAX_PATH + 1];
  extern __declspec (dllexport) wchar_t g_LastBouncedModule6 [MAX_PATH + 1];
  extern __declspec (dllexport) wchar_t g_LastBouncedModule7 [MAX_PATH + 1];
  extern __declspec (dllexport) int     g_LastBounceIdx;
};

SK_InjectionRecord_s*
SK_Inject_GetRecord (int idx);


// Part of the DLL Shared Data Segment
//
struct SK_InjectionBase_s
{
           HANDLE hShutdownEvent = INVALID_HANDLE_VALUE; // Event to signal unloading injected DLL instances
           DWORD  dwHookPID      =                  0UL; // Process that owns the CBT hook
  volatile HHOOK  hHookCBT       =              nullptr; // CBT hook
           BOOL   bAdmin         =                FALSE; // Is SKIM64 able to inject into admin apps?
};



#endif /* __SK__INJECTION_H__ */