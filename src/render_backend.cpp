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

#include <SpecialK/render_backend.h>

SK_RenderBackend __SK_RBkEnd;

SK_RenderBackend
__stdcall
SK_GetCurrentRenderBackend (void)
{
  return __SK_RBkEnd;
}

#include <SpecialK/log.h>

extern void WINAPI SK_HookGL     (void);
extern void WINAPI SK_HookVulkan (void);
extern void WINAPI SK_HookD3D9   (void);
extern void WINAPI SK_HookDXGI   (void);

void
SK_BootD3D9 (void)
{
  static volatile ULONG __booted = FALSE;

  if (InterlockedCompareExchange (&__booted, TRUE, FALSE))
    return;

  dll_log.Log (L"[API Detect]  <!> [ Bootstrapping Direct3D 9 (d3d9.dll) ] <!>");

  SK_HookD3D9 ();
}

void
SK_BootDXGI (void)
{
  static volatile ULONG __booted = FALSE;

  if (InterlockedCompareExchange (&__booted, TRUE, FALSE))
    return;

  dll_log.Log (L"[API Detect]  <!> [    Bootstrapping DXGI (dxgi.dll)    ] <!>");

  SK_HookDXGI ();
}

void
SK_BootOpenGL (void)
{
  static volatile ULONG __booted = FALSE;

  if (InterlockedCompareExchange (&__booted, TRUE, FALSE))
    return;

  dll_log.Log (L"[API Detect]  <!> [ Bootstrapping OpenGL (OpenGL32.dll) ] <!>");

  SK_HookGL ();
}

void
SK_BootVulkan (void)
{
  static volatile ULONG __booted = FALSE;

  if (InterlockedCompareExchange (&__booted, TRUE, FALSE))
    return;

  dll_log.Log (L"[API Detect]  <!> [ Bootstrapping Vulkan 1.x (vulkan-1.dll) ] <!>");

  SK_HookVulkan ();
}