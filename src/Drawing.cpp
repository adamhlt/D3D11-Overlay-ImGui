#include "Drawing.h"
#include "WindowTracker.h"

#include <Windows.h>
#include <vector>

#include "imgui.h"

namespace
{
    constexpr const char* kWindowName = "D3D11 Overlay ImGui";
    const ImVec2 kWindowSize = { 350, 75 };
    constexpr ImGuiWindowFlags kWindowFlags = 0;

    bool bDraw = true;
    WindowTracker::WindowItem selectedWindow;

    /**
        @brief : Draw the window picker.
                 Should be drawn only when the overlay is build as an EXE.
    **/
    void DrawWindowPicker()
    {
        std::vector<WindowTracker::WindowItem> windowList;
        WindowTracker::GetAllWindows(windowList);

        if (windowList.empty())
            return;

        ImGui::SetNextWindowSize({ 400, 100 }, ImGuiCond_Once);
        ImGui::SetNextWindowBgAlpha(1.0f);

        ImGui::Begin("Overlay Target Chooser", &bDraw, kWindowFlags);
        {
            if (ImGui::BeginCombo("##combo", selectedWindow.sTitle.c_str()))
            {
                for (const auto& item : windowList)
                {
                    const bool bSelected = selectedWindow.sTitle == item.sTitle;
                    if (ImGui::Selectable(item.sTitle.c_str(), bSelected))
                        selectedWindow = item;
                    if (bSelected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
            ImGui::NewLine();
            if (ImGui::Button("Start Overlay") && selectedWindow.hWindow != nullptr)
                WindowTracker::SetTarget(selectedWindow.hWindow);
        }
        ImGui::End();
    }

    /**
        @brief : Draw your own menu here.
    **/
    void DrawMenu()
    {
        ImGui::SetNextWindowSize(kWindowSize, ImGuiCond_Once);
        ImGui::SetNextWindowBgAlpha(1.0f);
        ImGui::Begin(kWindowName, &bDraw, kWindowFlags);
        {
            ImGui::Text("Create your own menu.");
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }
        ImGui::End();
    }
}

bool Drawing::IsActive()
{
    return bDraw;
}

void Drawing::Draw()
{
    if (IsActive())
    {
        if (!WindowTracker::IsTargeted())
            DrawWindowPicker();
        else
            DrawMenu();
    }

    if (GetAsyncKeyState(VK_INSERT) & 1)
        bDraw = !bDraw;
}
