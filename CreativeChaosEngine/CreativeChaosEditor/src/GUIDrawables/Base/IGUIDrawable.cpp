#include "IGUIDrawable.h"

/// <summary>
/// Set new GUI frame.
/// </summary>
void IGUIDrawable::PreGUIUpdate()
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
void IGUIDrawable::PostGUIUpdate()
{
	if (initialized)
	{
		ImGui::Render();
		p_drawData = ImGui::GetDrawData();
		ImGui_ImplDX11_RenderDrawData(p_drawData);
	}
}

/// <summary>
/// Returns a list of pointers to drawable gui elements.
/// </summary>
/// <returns>A vector of gui drawables.</returns>
std::vector<IGUIDrawable*> IGUIDrawable::GetGUIDrawablePtrs()
{
	return guiDrawables;
}

/// <summary>
/// Uninitializes the gui context.
/// </summary>
void IGUIDrawable::UnInitializeGUI() const
{
	if (initialized)
	{
		initialized = false;

		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();

		guiDrawables.clear();
		guiDrawables.~vector();
	}
}

// TODO: Fix runtime error in release config where font atlas is nullptr

/// <summary>
/// Initializes the gui context.
/// </summary>
void IGUIDrawable::InitializeGUI() const
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	io.Fonts->AddFontFromFileTTF("./resources/fonts/Inter-Light.ttf", 14);
	io.Fonts->AddFontFromFileTTF("./resources/fonts/Lexend-Light.ttf", 14);
	io.Fonts->Build();

	// Use CCE Colors
	ImGui::StyleColorsCCE();

	// Set Window specs1
	ImGui::GetStyle().WindowRounding = 4.0f;
	ImGui::GetStyle().ChildRounding = 4.0f;
	ImGui::GetStyle().FrameRounding = 1.0f;
	ImGui::GetStyle().GrabRounding = 0.0f;
	ImGui::GetStyle().PopupRounding = 4.0f;
	ImGui::GetStyle().ScrollbarRounding = 4.0f;

	// Hook the editors input calls to the engines input
	InputManager::Instance->inputCallback = &ImGui_ImplWin32_WndProcHandler;

	DASSERT(ImGui_ImplDX11_Init(Graphics::RenderPipeline::Instance->GetDevicePtr(),
		Graphics::RenderPipeline::Instance->GetDeviceContextPtr()), "Failed initializing GUI with D3D11.");

	// TODO: Change GetActiveWindow for multi window support.
	DASSERT(ImGui_ImplWin32_Init(GetActiveWindow()),
		"Failed initializing GUI with Editor Window.");
}

/// <summary>
/// A flag that indicates wether or not the gui is initialized.
/// </summary>
std::atomic<bool> IGUIDrawable::initialized = false;

/// <summary>
/// A list of registered gui drawables.
/// </summary>
std::vector<IGUIDrawable*> IGUIDrawable::guiDrawables;

/// <summary>
/// ImGuis draw data. 
/// </summary>
ImDrawData* IGUIDrawable::p_drawData = { 0 };