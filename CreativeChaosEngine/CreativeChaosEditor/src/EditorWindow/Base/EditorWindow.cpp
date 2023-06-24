#include "EditorWindow.h"
#include <CCEngine.h>

// TODO: Check if the update can be called collectively by using an event or smth

/// <summary>
/// Updates the window every frame.
/// </summary>
void EditorWindow::UpdateWindow()
{
	if (initialized)
	{
		isOpen = ImGui::Begin(windowName.Value());
		OnGui();
		ImGui::End();
	}
}

/// <summary>
/// Initializes the GUI. This function should only be called once.
/// </summary>
void EditorWindow::InitializeGUI() const
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	io.Fonts->AddFontFromFileTTF("resources/fonts/Inter-Light.ttf", 14);
	io.Fonts->AddFontFromFileTTF("resources/fonts/Lexend-Light.ttf", 14);

	ImGui::StyleColorsCCE();

	// Hook the editors input calls to the engines input
	InputManager::Instance->inputCallback = &ImGui_ImplWin32_WndProcHandler;

	DASSERT(ImGui_ImplDX11_Init(Graphics::RenderPipeline::Instance->GetDevicePtr(),
		Graphics::RenderPipeline::Instance->GetDeviceContextPtr()), "Failed initializing GUI with D3D11.");

	// TODO: Change GetActiveWindow for multi window support.
	DASSERT(ImGui_ImplWin32_Init(GetActiveWindow()),
		"Failed initializing GUI with Editor Window.");
}

void EditorWindow::UnInitializeGUI() const
{
	if (initialized)
	{
		initialized = false;
		
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();

		editorWindows.clear();
		editorWindows.~vector();
	}
}

/// <summary>
/// Set new GUI frame.
/// </summary>
void EditorWindow::PreGUIUpdate()
{
	if (initialized)
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}	
}

/// <summary>
/// Render the GUI.
/// </summary>
void EditorWindow::PostGUIUpdate()
{
	if (initialized)
	{
		ImGui::Render();
		// TODO: allocate dynamically
		p_drawData = ImGui::GetDrawData();
		ImGui_ImplDX11_RenderDrawData(p_drawData);
	}
}

/// <summary>
/// A list of all the editor windows that have been created.
/// </summary>
/// <returns>Editor* list</returns>
std::vector<EditorWindow*> EditorWindow::GetEditorWindowPtrs()
{
	return editorWindows;
}

/// <summary>
/// The static flag which indicates if ImGui was already initialized.
/// </summary>
std::atomic<bool> EditorWindow::initialized = false;

std::vector<EditorWindow*> EditorWindow::editorWindows;

ImDrawData* EditorWindow::p_drawData = nullptr;
