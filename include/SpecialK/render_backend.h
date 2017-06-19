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

#ifndef __SK__RENDER_BACKEND_H__
#define __SK__RENDER_BACKEND_H__

#include <Windows.h>
#include <nvapi.h>

extern HGLRC WINAPI SK_GetCurrentGLContext (void);

enum class SK_RenderAPI {
  Reserved  = 0x0001,

  // Native API Implementations
  OpenGL    = 0x0002,
  Vulkan    = 0x0004,
  D3D9      = 0x0008,
  D3D9Ex    = 0x0018,
  D3D10     = 0x0020, // Don't care
  D3D11     = 0x0040,
  D3D12     = 0x0080,

  // These aren't native, but we need the bitmask anyway
  D3D8      = 0x2000,
  DDraw     = 0x4000,
  Glide     = 0x8000,

  // Wrapped APIs (D3D12 Flavor)
  D3D11On12 = 0x00C0,

  // Wrapped APIs (D3D11 Flavor)
  D3D8On11  = 0x2040,
  DDrawOn11 = 0x4040,
  GlideOn11 = 0x8040,
};

struct SK_RenderBackend_V1 {
  enum class SK_RenderAPI api       = SK_RenderAPI::Reserved;
             wchar_t      name [16] = { L'\0' };
};

enum {
  SK_FRAMEBUFFER_FLAG_SRGB  = 0x1,
  SK_FRAMEBUFFER_FLAG_FLOAT = 0x2
};

enum reset_stage_e {
  Initiate = 0x0, // Fake device loss
  Respond  = 0x1, // Fake device not reset
  Clear    = 0x2  // Return status to normal
} extern trigger_reset;

enum mode_change_request_e {
  Windowed   = 0,    // Switch from Fullscreen to Windowed
  Fullscreen = 1,    // Switch from Windowed to Fullscreen
  None       = 0xFF
} extern request_mode_change;

struct SK_RenderBackend_V2 : SK_RenderBackend_V1 {
  IUnknown*               device               = nullptr;
  IUnknown*               swapchain            = nullptr;
  NVDX_ObjectHandle       surface              = 0;
  bool                    fullscreen_exclusive = false;
  uint64_t                framebuffer_flags    = 0x00;

  struct gsync_s {
    void update (void);

    BOOL                  capable      = FALSE;
    BOOL                  active       = FALSE;
    DWORD                 last_checked = 0;
  } gsync_state;

  DWORD                   thread       = 0;

  bool canEnterFullscreen  (void);

  void requestFullscreenMode (bool override = false);
  void requestWindowedMode   (bool override = false);
};

typedef SK_RenderBackend_V2 SK_RenderBackend;

SK_RenderBackend&
__stdcall
SK_GetCurrentRenderBackend (void);

void
__stdcall
SK_InitRenderBackends (void);

void SK_BootD3D8   (void);
void SK_BootDDraw  (void);
void SK_BootGlide  (void);

void SK_BootD3D9   (void);
void SK_BootDXGI   (void);
void SK_BootOpenGL (void);
void SK_BootVulkan (void);

#endif /* __SK__RENDER_BACKEND__H__ */