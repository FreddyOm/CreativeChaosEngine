#include "IGUIDrawable.h"
#include "CCEngine.h"

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
std::vector<IGUIDrawable*>* IGUIDrawable::GetGUIDrawablePtrs()
{
	return &guiDrawables;
}

void IGUIDrawable::ToggleWindow()
{
	isOpen = !isOpen;
}

bool IGUIDrawable::IsOpen()
{
	return isOpen;
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

// @TODO: Fix runtime error in release config where font atlas is nullptr

/// <summary>
/// Initializes the gui context.
/// </summary>
void IGUIDrawable::InitializeGUI()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	//lexend_light = io.Fonts->AddFontFromFileTTF("./resources/fonts/Lexend-Light.ttf", 14);
	inter_light = io.Fonts->AddFontFromFileTTF("./resources/fonts/Inter-Light.ttf", 14);

	ImFontConfig config;
	config.MergeMode = true;
	config.GlyphMinAdvanceX = 14.0f; // Use if you want to make the icon monospaced
	static const ImWchar icon_ranges[] = { 0xf000, 0xf372, 0 };
	io.Fonts->AddFontFromFileTTF("./resources/fonts/IconFont.ttf", 14.0f, &config, icon_ranges);

	config.MergeMode = false;
	inter_bold = io.Fonts->AddFontFromFileTTF("./resources/fonts/Inter-Bold.ttf", 11);

	config.MergeMode = true;
	io.Fonts->AddFontFromFileTTF("./resources/fonts/IconFont.ttf", 14.0f, &config, icon_ranges);

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
	Input::InputCallback = &ImGui_ImplWin32_WndProcHandler;

	bool ImguiInitD3D11 = ImGui_ImplDX11_Init(Graphics::g_pDevice.Get(),
		Graphics::g_pContext.Get());

	DASSERT(ImguiInitD3D11, "Failed initializing GUI with D3D11.");

	bool ImguiInitWin32 = ImGui_ImplWin32_Init(GetActiveWindow());

	// @TODO: Change GetActiveWindow for multi window support.
	DASSERT(ImguiInitWin32, "Failed initializing GUI with Editor Window.");
}

/// <summary>
/// A flag that indicates wether or not the gui is initialized.
/// </summary>
std::atomic<bool> IGUIDrawable::initialized = false;

ImFont* IGUIDrawable::inter_light = nullptr;
ImFont* IGUIDrawable::inter_bold = nullptr;

/// <summary>
/// A list of registered gui drawables.
/// </summary>
std::vector<IGUIDrawable*> IGUIDrawable::guiDrawables = {};

/// <summary>
/// ImGuis draw data. 
/// </summary>
ImDrawData* IGUIDrawable::p_drawData = nullptr;