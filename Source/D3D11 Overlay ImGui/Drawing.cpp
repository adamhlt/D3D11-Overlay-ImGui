#include "Drawing.h"

LPCSTR Drawing::lpWindowName = "D3D11 Overlay ImGui";
ImVec2 Drawing::vWindowSize = { 350, 75 };
ImGuiWindowFlags Drawing::WindowFlags = 0;
bool Drawing::bDraw = true;
UI::WindowItem Drawing::lpSelectedWindow = { nullptr, "", "" };

bool Drawing::isActive()
{
	return bDraw == true;
}

void Drawing::Draw()
{
	if (isActive())
	{
		// Draw the window picker.
		// Should be drawed only when the overlay is build as an EXE.
		if (!UI::IsWindowTargeted())
		{
			std::vector<UI::WindowItem> WindowList;
			UI::GetAllWindow(&WindowList);

			if (WindowList.empty())
				return;

			ImGui::SetNextWindowSize({ 400, 100 }, ImGuiCond_Once);
			ImGui::SetNextWindowBgAlpha(1.0f);

			ImGui::Begin("Overlay Target Chooser", &bDraw, WindowFlags);
			{
				if (ImGui::BeginCombo("##combo", lpSelectedWindow.CurrentWindowTitle))
				{
					for (const auto& item : WindowList)
					{
						const bool is_selected = (strcmp(lpSelectedWindow.CurrentWindowTitle, item.CurrentWindowTitle) == 0);
						if (ImGui::Selectable(item.CurrentWindowTitle, is_selected))
							lpSelectedWindow = item;
						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}
				ImGui::NewLine();
				if (ImGui::Button("Start Overlay"))
				{
					UI::SetTargetWindow(lpSelectedWindow.CurrentWindow);
				}
			}
			ImGui::End();

			return;
		}

		ImGui::SetNextWindowSize(vWindowSize, ImGuiCond_Once);
		ImGui::SetNextWindowBgAlpha(1.0f);
		ImGui::Begin(lpWindowName, &bDraw, WindowFlags);
		{

			ImGui::Text("Create your own menu.");

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		}
		ImGui::End();
	}

	if (GetAsyncKeyState(VK_INSERT) & 1)
		bDraw = !bDraw;
}