#include "MenuBar.h"

void MenuBar::UpdateDrawable()
{
    OnGui();
}

void MenuBar::OnGui()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New", "CTRL+N")) {}
            if (ImGui::MenuItem("Open...", "CTRL+O")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Save", "CTRL+S")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Quit", "ALT+F4")) {}

            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}
