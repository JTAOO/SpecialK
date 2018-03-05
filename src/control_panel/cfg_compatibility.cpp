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

#include <imgui/imgui.h>

#include <SpecialK/control_panel.h>
#include <SpecialK/control_panel/compatibility.h>

#include <SpecialK/diagnostics/debug_utils.h>
#include <SpecialK/diagnostics/compatibility.h>

#include <SpecialK/core.h>
#include <SpecialK/config.h>
#include <SpecialK/window.h>
#include <SpecialK/utility.h>

using namespace SK::ControlPanel;

bool
SK::ControlPanel::Compatibility::Draw (void)
{
  if ( ImGui::CollapsingHeader ("Compatibility Settings") )
  {
    SK_RenderBackend& rb =
      SK_GetCurrentRenderBackend ();

    ImGui::PushStyleColor (ImGuiCol_Header,        ImVec4 (0.02f, 0.68f, 0.90f, 0.45f));
    ImGui::PushStyleColor (ImGuiCol_HeaderHovered, ImVec4 (0.07f, 0.72f, 0.90f, 0.80f));
    ImGui::PushStyleColor (ImGuiCol_HeaderActive,  ImVec4 (0.14f, 0.78f, 0.87f, 0.80f));
    ImGui::TreePush       ("");

    if (ImGui::CollapsingHeader ("Third-Party Software"))
    {
      ImGui::TreePush ("");
      ImGui::Checkbox     ("Disable GeForce Experience and NVIDIA Shield", &config.compatibility.disable_nv_bloat);
      if (ImGui::IsItemHovered ())
        ImGui::SetTooltip ("May improve software compatibility, but disables ShadowPlay, couch co-op and various Shield-related functionality.");
      ImGui::TreePop  ();
    }

    if ( ImGui::CollapsingHeader ("Render Backends",
           SK_IsInjected () ? ImGuiTreeNodeFlags_DefaultOpen :
                              0 ) )
    {
      ImGui::TreePush ("");

      auto EnableActiveAPI =
      [ ](SK_RenderAPI api)
      {
        switch (api)
        {
          case SK_RenderAPI::D3D9Ex:
            config.apis.d3d9ex.hook     = true;
          case SK_RenderAPI::D3D9:
            config.apis.d3d9.hook       = true;
            break;

#ifdef _WIN64
          case SK_RenderAPI::D3D12:
            config.apis.dxgi.d3d12.hook = true;
#endif
          case SK_RenderAPI::D3D11:
            config.apis.dxgi.d3d11.hook = true;
            break;

#ifndef _WIN64
          case SK_RenderAPI::DDrawOn11:
            config.apis.ddraw.hook       = true;
            config.apis.dxgi.d3d11.hook  = true;
            break;

          case SK_RenderAPI::D3D8On11:
            config.apis.d3d8.hook        = true;
            config.apis.dxgi.d3d11.hook  = true;
            break;
#endif

          case SK_RenderAPI::OpenGL:
            config.apis.OpenGL.hook     = true;
            break;

#ifdef _WIN64
          case SK_RenderAPI::Vulkan:
            config.apis.Vulkan.hook     = true;
            break;
#endif
        }
      };

      using Tooltip_pfn = void (*)(void);

      auto ImGui_CheckboxEx =
      [ ]( const char* szName, bool* pVar,
                               bool  enabled = true,
           Tooltip_pfn tooltip_disabled      = nullptr )
      {
        if (enabled)
        {
          ImGui::Checkbox (szName, pVar);
        }

        else
        {
          ImGui::TreePush     ("");
          ImGui::TextDisabled (szName);

          if (tooltip_disabled != nullptr)
            tooltip_disabled ();

          ImGui::TreePop      (  );

          *pVar = false;
        }
      };

#ifdef _WIN64
      const int num_lines = 4; // Basic set of APIs
#else
      const int num_lines = 5; // + DirectDraw / Direct3D 8
#endif

      ImGui::PushStyleVar                                                                          (ImGuiStyleVar_ChildWindowRounding, 10.0f);
      ImGui::BeginChild ("", ImVec2 (font.size * 39, font.size_multiline * num_lines * 1.1f), true, ImGuiWindowFlags_NavFlattened);

      ImGui::Columns    ( 2 );

      ImGui_CheckboxEx ("Direct3D 9",   &config.apis.d3d9.hook);
      ImGui_CheckboxEx ("Direct3D 9Ex", &config.apis.d3d9ex.hook, config.apis.d3d9.hook);

      ImGui::NextColumn (   );

      ImGui_CheckboxEx ("Direct3D 11",  &config.apis.dxgi.d3d11.hook);

#ifdef _WIN64
      ImGui_CheckboxEx ("Direct3D 12",  &config.apis.dxgi.d3d12.hook, config.apis.dxgi.d3d11.hook);
#endif

      ImGui::Columns    ( 1 );
      ImGui::Separator  (   );

#ifndef _WIN64
      ImGui::Columns    ( 2 );

      static bool has_dgvoodoo2 =
        GetFileAttributesA (
          SK_FormatString ( R"(%ws\PlugIns\ThirdParty\dgVoodoo\d3dimm.dll)",
                              std::wstring ( SK_GetDocumentsDir () + L"\\My Mods\\SpecialK" ).c_str ()
                          ).c_str ()
        ) != INVALID_FILE_ATTRIBUTES;

      // Leaks memory, but who cares? :P
      static const char* dgvoodoo2_install_path =
        _strdup (
          SK_FormatString ( R"(%ws\PlugIns\ThirdParty\dgVoodoo)",
                  std::wstring ( SK_GetDocumentsDir () + L"\\My Mods\\SpecialK" ).c_str ()
              ).c_str ()
        );

      auto Tooltip_dgVoodoo2 = []
      {
        if (ImGui::IsItemHovered ())
        {
          ImGui::BeginTooltip ();
            ImGui::TextColored (ImColor (235, 235, 235), "Requires Third-Party Plug-In:");
            ImGui::SameLine    ();
            ImGui::TextColored (ImColor (255, 255, 0),   "dgVoodoo2");
            ImGui::Separator   ();
            ImGui::BulletText  ("Please install this to: '%s'", dgvoodoo2_install_path);
          ImGui::EndTooltip   ();
        }
      };

      ImGui_CheckboxEx ("Direct3D 8", &config.apis.d3d8.hook,   has_dgvoodoo2, Tooltip_dgVoodoo2);

      ImGui::NextColumn (  );

      ImGui_CheckboxEx ("Direct Draw", &config.apis.ddraw.hook, has_dgvoodoo2, Tooltip_dgVoodoo2);

      ImGui::Columns    ( 1 );
      ImGui::Separator  (   );
#endif

      ImGui::Columns    ( 2 );
                        
      ImGui::Checkbox   ("OpenGL ", &config.apis.OpenGL.hook); ImGui::SameLine ();
#ifdef _WIN64
      ImGui::Checkbox   ("Vulkan ", &config.apis.Vulkan.hook);
#endif

      ImGui::NextColumn (  );

      if (ImGui::Button (" Disable All But the Active API "))
      {
        config.apis.d3d9ex.hook     = false; config.apis.d3d9.hook       = false;
        config.apis.dxgi.d3d11.hook = false;
        config.apis.OpenGL.hook     = false;
#ifdef _WIN64
        config.apis.dxgi.d3d12.hook = false; config.apis.Vulkan.hook     = false;
#else
        config.apis.d3d8.hook       = false; config.apis.ddraw.hook      = false;
#endif
      }

      if (ImGui::IsItemHovered ())
        ImGui::SetTooltip ("Application start time and negative interactions with third-party software can be reduced by turning off APIs that are not needed...");

      ImGui::Columns    ( 1 );

      ImGui::EndChild   (  );
      ImGui::PopStyleVar ( );

      EnableActiveAPI   (render_api);
      ImGui::TreePop    ();
    }

    if (ImGui::CollapsingHeader ("Hardware Monitoring"))
    {
      ImGui::TreePush ("");
      ImGui::Checkbox ("NvAPI  ", &config.apis.NvAPI.enable);
      if (ImGui::IsItemHovered ())
        ImGui::SetTooltip ("NVIDIA's hardware monitoring API, needed for the GPU stats on the OSD. Turn off only if your driver is buggy.");

      ImGui::SameLine ();
      ImGui::Checkbox ("ADL   ",   &config.apis.ADL.enable);
      if (ImGui::IsItemHovered ())
        ImGui::SetTooltip ("AMD's hardware monitoring API, needed for the GPU stats on the OSD. Turn off only if your driver is buggy.");
      ImGui::TreePop  ();
    }

    ImGui::PushStyleColor (ImGuiCol_Header,        ImVec4 (0.90f, 0.40f, 0.40f, 0.45f));
    ImGui::PushStyleColor (ImGuiCol_HeaderHovered, ImVec4 (0.90f, 0.45f, 0.45f, 0.80f));
    ImGui::PushStyleColor (ImGuiCol_HeaderActive,  ImVec4 (0.87f, 0.53f, 0.53f, 0.80f));

    if (ImGui::CollapsingHeader ("Debugging"))
    {
      ImGui::TreePush   ("");
      ImGui::BeginGroup (  );
      ImGui::Checkbox   ("Enable Crash Handler",          &config.system.handle_crashes);

      if (ImGui::IsItemHovered ())
        ImGui::SetTooltip ("Play Metal Gear Solid Alert Sound and Log Crashes in logs/crash.log");

      ImGui::Checkbox  ("ReHook LoadLibrary",             &config.compatibility.rehook_loadlibrary);

      if (ImGui::IsItemHovered ())
      {
        ImGui::BeginTooltip ();
        ImGui::Text         ("Keep LoadLibrary Hook at Front of Hook Chain");
        ImGui::Separator    ();
        ImGui::BulletText   ("Improves Debug Log Accuracy");
        ImGui::BulletText   ("Third-Party Software May Deadlock Game at Startup if Enabled");
        ImGui::EndTooltip   ();
      }

      ImGui::SliderInt ("Log Level",                      &config.system.log_level, 0, 5);

      if (ImGui::IsItemHovered ())
        ImGui::SetTooltip ("Controls Debug Log Verbosity; Higher = Bigger/Slower Logs");

      ImGui::Checkbox  ("Log Game Output",                &config.system.game_output);

      if (ImGui::IsItemHovered ())
        ImGui::SetTooltip ("Log any Game Text Output to logs/game_output.log");

      if (ImGui::Checkbox  ("Print Debug Output to Console",  &config.system.display_debug_out))
      {
        if (config.system.display_debug_out)
        {
          if (! SK::Diagnostics::Debugger::CloseConsole ()) config.system.display_debug_out = true;
        }

        else
        {
          SK::Diagnostics::Debugger::SpawnConsole ();
        }
      }

      if (ImGui::IsItemHovered ())
        ImGui::SetTooltip ("Spawns Debug Console at Startup for Debug Text from Third-Party Software");

      ImGui::Checkbox  ("Trace LoadLibrary",              &config.system.trace_load_library);

      if (ImGui::IsItemHovered ())
      {
        ImGui::BeginTooltip ();
        ImGui::Text         ("Monitor DLL Load Activity");
        ImGui::Separator    ();
        ImGui::BulletText   ("Required for Render API Auto-Detection in Global Injector");
        ImGui::EndTooltip   ();
      }

      ImGui::Checkbox  ("Strict DLL Loader Compliance",   &config.system.strict_compliance);

      if (ImGui::IsItemHovered ())
      {
        ImGui::BeginTooltip    (  );
        ImGui::TextUnformatted ("Prevent Loading DLLs Simultaneously Across Multiple Threads");
        ImGui::Separator       (  );
        ImGui::BulletText      ("Eliminates Race Conditions During DLL Startup");
        ImGui::BulletText      ("Unsafe for a LOT of Improperly Designed Third-Party Software\n");
        ImGui::TreePush        ("");
        ImGui::TextUnformatted ("");
        ImGui::BeginGroup      (  );
        ImGui::TextUnformatted ("PROPER DLL DESIGN:  ");
        ImGui::EndGroup        (  );
        ImGui::SameLine        (  );
        ImGui::BeginGroup      (  );
        ImGui::TextUnformatted ("Never Call LoadLibrary (...) from DllMain (...)'s Thread !!!");
        ImGui::TextUnformatted ("Never Wait on a Synchronization Object from DllMain (...) !!");
        ImGui::EndGroup        (  );
        ImGui::TreePop         (  );
        ImGui::EndTooltip      (  );
      }

      ImGui::EndGroup    ( );

      ImGui::SameLine    ( );
      ImGui::BeginGroup  ( );

      static int window_pane = 1;

      RECT window = { };
      RECT client = { };

      ImGui::TextColored (ImColor (1.f,1.f,1.f), "Window Management:  "); ImGui::SameLine ();
      ImGui::RadioButton ("Dimensions", &window_pane, 0);                 ImGui::SameLine ();
      ImGui::RadioButton ("Details",    &window_pane, 1);
      ImGui::Separator   (                             );

        auto DescribeRect = [](LPRECT rect, const char* szType, const char* szName)
        {
          ImGui::Text (szType);
          ImGui::NextColumn ();
          ImGui::Text (szName);
          ImGui::NextColumn ();
          ImGui::Text ( "| (%4li,%4li) / %4lix%li |  ",
                            rect->left, rect->top,
                              rect->right-rect->left, rect->bottom - rect->top );
          ImGui::NextColumn ();
        };

#define __SK_ImGui_FixedWidthFont ImGui::GetIO ().Fonts->Fonts [1]

      ImGui::PushFont    (__SK_ImGui_FixedWidthFont);

      switch (window_pane)
      {
      case 0:
        ImGui::Columns   (3);

        DescribeRect (&game_window.actual.window, "Window", "Actual" );
        DescribeRect (&game_window.actual.client, "Client", "Actual" );

        ImGui::Columns   (1);
        ImGui::Separator ( );
        ImGui::Columns   (3);

        DescribeRect (&game_window.game.window,   "Window", "Game"   );
        DescribeRect (&game_window.game.client,   "Client", "Game"   );

        ImGui::Columns   (1);
        ImGui::Separator ( );
        ImGui::Columns   (3);

        GetClientRect (game_window.hWnd, &client);
        GetWindowRect (game_window.hWnd, &window);

        DescribeRect  (&window,   "Window", "GetWindowRect"   );
        DescribeRect  (&client,   "Client", "GetClientRect"   );
                           
        ImGui::Columns   (1);
        break;

     case 1:
        ImGui::Text      ( "App_Active   : %s", game_window.active ? "Yes" : "No" );
        ImGui::Text      ( "Active HWND  : %x", GetActiveWindow     () );
        ImGui::Text      ( "Foreground   : %x", GetForegroundWindow () );
        ImGui::Text      ( "Input Focus  : %x", GetFocus            () );
        ImGui::Separator (    );
        ImGui::Text      ( "HWND         : Focus: %06x, Device: %06x",
                                           rb.windows.focus.hwnd, rb.windows.device.hwnd );
        ImGui::Text      ( "Window Class : %32ws :: %32ws", rb.windows.focus.class_name,
                                                            rb.windows.device.class_name );
        ImGui::Text      ( "Window Title : %32ws :: %32ws", rb.windows.focus.title,
                                                            rb.windows.device.title );
        ImGui::Text      ( "Owner PID    : %8lu, %8lu",     rb.windows.focus.owner.pid,
                                                            rb.windows.device.owner.pid );
        ImGui::Text      ( "Owner TID    : %8lu, %8lu",     rb.windows.focus.owner.tid,
                                                            rb.windows.device.owner.tid );
        ImGui::Text      ( "Init. Frame  : %8lu, %8lu",
                                                            rb.windows.focus.last_changed,
                                                            rb.windows.device.last_changed );
        ImGui::Text      ( "Unicode      : %8s, %8s",       rb.windows.focus.unicode  ? "Yes" : "No",
                                                            rb.windows.device.unicode ? "Yes" : "No" );
        ImGui::Text      ( "Top          : %08x, %08x",
                             GetTopWindow (rb.windows.focus.hwnd), 
                             GetTopWindow (rb.windows.device.hwnd)     );
        ImGui::Text      ( "Parent       : %08x, %08x",
                                          (rb.windows.focus.parent),
                                          (rb.windows.device.parent)   );
        break;
      }

      ImGui::PopFont     ( );
      ImGui::Separator   ( );

      ImGui::Text        ( "ImGui Cursor State: %lu (%lu,%lu) { %lu, %lu }",
                             SK_ImGui_Cursor.visible, SK_ImGui_Cursor.pos.x,
                                                      SK_ImGui_Cursor.pos.y,
                               SK_ImGui_Cursor.orig_pos.x, SK_ImGui_Cursor.orig_pos.y );
      ImGui::SameLine    ( );
      ImGui::Text        (" {%s :: Last Update: %lu}",
                            SK_ImGui_Cursor.idle ? "Idle" :
                                                   "Not Idle",
                              SK_ImGui_Cursor.last_move);
      ImGui::EndGroup    ( );
      ImGui::TreePop     ( );
    }

    ImGui::PopStyleColor  (3);

    ImGui::TreePop        ( );
    ImGui::PopStyleColor  (3);

    return true;
  }

  return false;
}