#include <iostream>

#include <d3d9.h>

#include "kiero.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx9.h"

#include "hook_hero_selected.hpp"
#include "hook_d3d9.hpp"

typedef long(__stdcall* Reset)(LPDIRECT3DDEVICE9, D3DPRESENT_PARAMETERS*);

static Reset oReset = NULL;

typedef long(__stdcall* EndScene)(LPDIRECT3DDEVICE9);

static EndScene oEndScene = NULL;

static bool init = false;

long __stdcall hkReset(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    long result = oReset(pDevice, pPresentationParameters);
    ImGui_ImplDX9_CreateDeviceObjects();
    return result;
}

LPDIRECT3DDEVICE9 initDevice = nullptr;

long __stdcall hkEndScene(LPDIRECT3DDEVICE9 pDevice)
{
    if (!init)
    {
        init = true;
        D3DDEVICE_CREATION_PARAMETERS params;
        pDevice->GetCreationParameters(&params);

        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();
        ImGui::GetStyle().WindowBorderSize = 0;
        ImGui_ImplWin32_Init(params.hFocusWindow);
    }
    if (initDevice != pDevice) {
        initDevice = pDevice;
        ImGui_ImplDX9_InvalidateDeviceObjects();
        if (ImGui::GetIO().BackendRendererName != nullptr)
        {
            ImGui_ImplDX9_Shutdown();
        }
        ImGui_ImplDX9_Init(pDevice);
        ImGui_ImplDX9_CreateDeviceObjects();
    }
    if (get_current_moves() == -1)
    {
        return oEndScene(pDevice);
    }
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGuiIO& io = ImGui::GetIO();
    auto offset = min(io.DisplaySize.x * 0.2, io.DisplaySize.y * 0.28);
    ImVec2 window_pos = ImVec2(offset * 1.08, io.DisplaySize.y - offset * 0.86);
    ImVec2 window_pos_pivot = ImVec2(0.0f, 1.0f);
    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
    ImGui::SetNextWindowBgAlpha(0.7F); // Transparent background
    if (ImGui::Begin("Example: Simple overlay",
        nullptr,
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize
            | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
    {
        if (get_path_length() > 0)
        {
            ImGui::Text("%d / %d", get_path_length(), get_current_moves());
        }
        else
        {
            ImGui::Text("%d", get_current_moves());
        }
    }

    ImGui::EndFrame();
    ImGui::Render();

    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
    return oEndScene(pDevice);
}

bool setup_d3d9_hook()
{
    if (kiero::init(kiero::RenderType::D3D9) != kiero::Status::Success)
    {
        std::cout << "Failed to init kiero";
        return false;
    }
    else
    {
        kiero::Status::Enum resetBind = kiero::bind(16, (void**)&oReset, hkReset);
        std::cout << "reset bind: " << resetBind << std::endl;
        kiero::Status::Enum endSceneBind = kiero::bind(42, (void**)&oEndScene, hkEndScene);
        std::cout << "end scene bind: " << endSceneBind << std::endl;
    }
    return true;
}
