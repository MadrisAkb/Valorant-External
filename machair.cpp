#include <iostream>
#include "GL/glew.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <Windows.h>
#include <TlHelp32.h>
#include <vector>
#include "offsets.h"
#include "vector3.h"
#include "defs.h"
#include <tchar.h>
#include <intrin.h>
#include "auth.hpp"
#include "comm.h"
#include "xorstr.hpp"
#include <ProcessSnapshot.h>
#include "xor.hpp"
#include <Windows.h>
#include "fontSF.h"
#include <iostream>
#include <fstream>
#include <cstdint>
#include <filesystem>

uintptr_t unique_id;
std::string tm_to_readable_time(tm ctx);
static std::time_t string_to_timet(std::string timestamp);
static std::tm timet_to_tm(time_t timestamp);

using namespace KeyAuth;

std::string name = "Emre"; // application name. right above the blurred text aka the secret on the licenses tab among other tabs
std::string ownerid = "e1HCo5FQWL"; // ownerid, found in account settings. click your profile picture on top right of dashboard and then account settings.
std::string secret = "e6b0156fab99d029b26b329b4a4f8996540a0dd62199bc19879e62e3b312cb52"; // app secret, the blurred text on licenses tab and other tabs
std::string version = "1.0"; // leave alone unless you've changed version on website
std::string url = "https://keyauth.win/api/1.1/"; // change if you're self-hosting
std::string sslPin = "ssl pin key (optional)"; // don't change unless you intend to pin public certificate key. you can get here in the "Pin SHA256" field https://www.ssllabs.com/ssltest/analyze.html?d=keyauth.win&latest. If you do this you need to be aware of when SSL key expires so you can update i

api KeyAuthApp(name, ownerid, secret, version, url, sslPin);

struct State {
	uintptr_t keys[7];
};
UINT profetrol;
typedef struct {
	uintptr_t actor_ptr;
	uintptr_t damage_handler_ptr;
	uintptr_t player_state_ptr;
	uintptr_t root_component_ptr;
	uintptr_t mesh_ptr;
	uintptr_t bone_array_ptr;
	int bone_count;
	bool is_visible;
} Enemy;

// Window / Process values
HWND valorant_window;
GLFWwindow* g_window;
int g_width;
int g_height;
int g_pid;
uintptr_t g_base_address;
ImU32 g_esp_color = ImGui::ColorConvertFloat4ToU32(ImVec4(0, 136, 255, 200));
ImU32 g_color_white = ImGui::ColorConvertFloat4ToU32(ImVec4(255, 0, 255, 200));

// Cheat toggle values
bool g_overlay_visible{ false };
bool g_esp_enabled{ true };
bool g_esp_dormantcheck{ false };
bool g_aimbot{ false };
bool g_aimbottest{ false };

bool g_lines{ false };
bool g_health{ false };
bool g_distanceesp{ false };
bool g_headesp{ true };
bool g_boneesp{ true };
bool g_agentname{ false };
bool g_boxesp{ true };
bool test1{ false };
bool test2{ false };
bool test3{ false };
bool test4{ false };
ImVec2 head_at_screen;
ImVec2 boted_vex_screen;
Vector3 camera_position;
Vector3 camera_rotation;
// Pointers
uintptr_t local_player_pawn;
uintptr_t g_local_player_controller;
uintptr_t g_local_player_pawn;
uintptr_t g_local_damage_handler;
uintptr_t g_camera_manager;
int g_local_team_id;

// Enemy list
std::vector<Enemy> enemy_collection{};


int Depth;

static int bonepos = 1;
static const char* bones[]
{
	"Head",
	"Neck",
	"Chest",
	"Pelvis",
	"Stomach",
	"Random"
};
static int bonekey;


static int aimkeypos = 2;
static const char* aimkeys[]
{
	"Left Mouse Button",
	"Right Mouse Button",
	"Middle Mouse Button",
	"Mouse Side 1",
	"Mouse Side 2",
	"Control-Break Processing",
	"Backspace",
	"Tab",
	"Clear",
	"Enter",
	"SHIFT",
	"CTRL",
	"ALT",
	"Caps Lock",
	"Esc",
	"Space",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	"Numpad 0",
	"Numpad 1",
	"Numpad 2",
	"Numpad 3",
	"Numpad 4",
	"Numpad 5",
	"Numpad 6",
	"Numpad 7",
	"Numpad 8",
	"Numpad 9",
	"Multiply"
};
static int aimkey;

/*std::uintptr_t find_signature(const char* sig, const char* mask)
{
	auto buffer = std::make_unique<std::array<std::uint8_t, 0x100000>>();
	auto data = buffer.get()->data();

	for (std::uintptr_t i = 0u; i < (2u << 25u); ++i)
	{
		driver->ReadProcessMemory(g_base_address  + i * 0x100000, data, 0x100000);

		if (!data)
			return 0;

		for (std::uintptr_t j = 0; j < 0x100000u; ++j)
		{
			if ([](std::uint8_t const* data, std::uint8_t const* sig, char const* mask)
				{
					for (; *mask; ++mask, ++data, ++sig)
					{
						if (*mask == 'x' && *data != *sig) return false;
					}
					return (*mask) == 0;
				}(data + j, (std::uint8_t*)sig, mask))
			{
				std::uintptr_t result = g_base_address + i * 0x100000 + j;
				std::uint32_t rel = 0;

				driver->ReadProcessMemory(result + 3, &rel, sizeof(std::uint32_t));

				if (!rel)
					return 0;

				return result + rel + 7;
			}
		}
	}

	return 0;
}*/

void AimLock(Vector3 Target) {
	Vector3 ControlRotation = read<Vector3>(g_local_player_controller + offsets::control_rotation);
	Vector3 Delta = Vector3((camera_position.x - Target.x), (camera_position.y - Target.y), (camera_position.z - Target.z));
	float hyp = sqrtf(Delta.x * Delta.x + Delta.y * Delta.y + Delta.z * Delta.z);

	Vector3 Rotation{};
	Rotation.x = acosf(Delta.z / hyp) * (float)(RadianToURotation);
	Rotation.y = atanf(Delta.y / Delta.x) * (float)(RadianToURotation);
	Rotation.z = 0;

	Rotation.x += 270.f;
	if (Rotation.x > 360.f) {
		Rotation.x -= 360.f;
	}
	if (Delta.x >= 0.0f) {
		Rotation.y += 180.0f;
	}
	if (Rotation.y < 0.f) {
		Rotation.y += 360.f;
	}

	boted_vex_screen.x = Rotation.x;
	boted_vex_screen.y = Rotation.y;

}


inline Vector3 SmoothAim(Vector3 Camera_rotation, Vector3 Target, float SmoothFactor)
{
	Vector3 diff = Target - Camera_rotation;
	diff.normalize();
	return Camera_rotation + diff / SmoothFactor;
}


void RCS(float SmoothFactor) {
	Vector3 DeltaRotation = Vector3(0, 0, 0);
	Vector3 ConvertRotation = Vector3(0, 0, 0);
	Vector3 ControlRotation = read<Vector3>(g_local_player_controller + offsets::control_rotation);

	// Camera 2 Control space
	if (camera_rotation.x < 0.f) {
		ConvertRotation.x = 360.f + camera_rotation.x;
	}
	else {
		ConvertRotation.x = camera_rotation.x;
	}
	if (camera_rotation.y < 0.f) {
		ConvertRotation.y = 360.f + camera_rotation.y;
	}
	else {
		ConvertRotation.y = camera_rotation.y;
	}

	// Calculate recoil/aimpunch
	DeltaRotation.x = ConvertRotation.x - ControlRotation.x;
	DeltaRotation.y = ConvertRotation.y - ControlRotation.y;
	// (-360, 360)
	DeltaRotation.x = fmodf(DeltaRotation.x, 360.f);
	DeltaRotation.y = fmodf(DeltaRotation.y, 360.f);


	// Remove 2x aimpunch from CameraRotationD
	ConvertRotation.x = boted_vex_screen.x - DeltaRotation.x - DeltaRotation.x;
	ConvertRotation.y = boted_vex_screen.y - DeltaRotation.y - DeltaRotation.y;
	// (-360, 360)
	ConvertRotation.x = fmodf(ConvertRotation.x, 360.f);
	ConvertRotation.y = fmodf(ConvertRotation.y, 360.f);
	// [0, 360)
	if (ConvertRotation.x < 0.f) {
		ConvertRotation.x = 360.f + ConvertRotation.x;
	}
	if (ConvertRotation.y < 0.f) {
		ConvertRotation.y = 360.f + ConvertRotation.y;
	}


	
	

	if (local_player_pawn != 0) {
		write<Vector3>(g_local_player_controller + offsets::control_rotation, ConvertRotation);
	}

}


inline void gg(Vector3 Target, Vector3 Camera_rotation, float SmoothFactor) {
	// Camera 2 Control space
	Vector3 ConvertRotation = Camera_rotation;
	ConvertRotation.normalize();

	// Calculate recoil/aimpunch
	auto ControlRotation = read<Vector3>(g_local_player_controller + offsets::control_rotation);
	Vector3 DeltaRotation = ConvertRotation - ControlRotation;
	DeltaRotation.normalize();

	// Remove aimpunch from CameraRotation
	ConvertRotation = Target - (DeltaRotation * SmoothFactor);
	ConvertRotation.normalize();

	//Smooth the whole thing
	Vector3 Smoothed = SmoothAim(Camera_rotation, ConvertRotation, SmoothFactor);
	Smoothed -= (DeltaRotation / SmoothFactor);

	write(g_local_player_controller + offsets::control_rotation, Smoothed);
}


typedef struct
{
	DWORD R;
	DWORD G;
	DWORD B;
	DWORD A;
}RGBA;

class Color
{
public:
	RGBA red = { 255,0,0,255 };
	RGBA Magenta = { 255,0,255,255 };
	RGBA yellow = { 255,255,0,255 };
	RGBA grayblue = { 128,128,255,255 };
	RGBA green = { 128,224,0,255 };
	RGBA darkgreen = { 0,224,128,255 };
	RGBA brown = { 192,96,0,255 };
	RGBA pink = { 255,168,255,255 };
	RGBA DarkYellow = { 216,216,0,255 };
	RGBA SilverWhite = { 236,236,236,255 };
	RGBA purple = { 144,0,255,255 };
	RGBA Navy = { 88,48,224,255 };
	RGBA skyblue = { 0,136,255,255 };
	RGBA graygreen = { 128,160,128,255 };
	RGBA blue = { 0,96,192,255 };
	RGBA orange = { 255,128,0,255 };
	RGBA peachred = { 255,80,128,255 };
	RGBA reds = { 255,128,192,255 };
	RGBA darkgray = { 96,96,96,255 };
	RGBA Navys = { 0,0,128,255 };
	RGBA darkgreens = { 0,128,0,255 };
	RGBA darkblue = { 0,128,128,255 };
	RGBA redbrown = { 128,0,0,255 };
	RGBA purplered = { 128,0,128,255 };
	RGBA greens = { 0,255,0,255 };
	RGBA envy = { 0,255,255,255 };
	RGBA black = { 0,0,0,255 };
	RGBA gray = { 128,128,128,255 };
	RGBA white = { 255,255,255,255 };
	RGBA blues = { 30,144,255,255 };
	RGBA lightblue = { 135,206,250,160 };
	RGBA Scarlet = { 220, 20, 60, 160 };
	RGBA white_ = { 255,255,255,200 };
	RGBA gray_ = { 128,128,128,200 };
	RGBA black_ = { 0,0,0,200 };
	RGBA red_ = { 255,0,0,200 };
	RGBA Magenta_ = { 255,0,255,200 };
	RGBA yellow_ = { 255,255,0,200 };
	RGBA grayblue_ = { 128,128,255,200 };
	RGBA green_ = { 128,224,0,200 };
	RGBA darkgreen_ = { 0,224,128,200 };
	RGBA brown_ = { 192,96,0,200 };
	RGBA pink_ = { 255,168,255,200 };
	RGBA darkyellow_ = { 216,216,0,200 };
	RGBA silverwhite_ = { 236,236,236,200 };
	RGBA purple_ = { 144,0,255,200 };
	RGBA Blue_ = { 88,48,224,200 };
	RGBA skyblue_ = { 0,136,255,200 };
	RGBA graygreen_ = { 128,160,128,200 };
	RGBA blue_ = { 0,96,192,200 };
	RGBA orange_ = { 255,128,0,200 };
	RGBA pinks_ = { 255,80,128,200 };
	RGBA Fuhong_ = { 255,128,192,200 };
	RGBA darkgray_ = { 96,96,96,200 };
	RGBA Navy_ = { 0,0,128,200 };
	RGBA darkgreens_ = { 0,128,0,200 };
	RGBA darkblue_ = { 0,128,128,200 };
	RGBA redbrown_ = { 128,0,0,200 };
	RGBA purplered_ = { 128,0,128,200 };
	RGBA greens_ = { 0,255,0,200 };
	RGBA envy_ = { 0,255,255,200 };

	RGBA glassblack = { 0, 0, 0, 160 };
	RGBA GlassBlue = { 65,105,225,80 };
	RGBA glassyellow = { 255,255,0,160 };
	RGBA glass = { 200,200,200,60 };


	RGBA Plum = { 221,160,221,160 };

};
Color Col;

ImU32 green = ImGui::ColorConvertFloat4ToU32(ImVec4(0, 224, 128, 200));
ImU32 half = ImGui::ColorConvertFloat4ToU32(ImVec4(255, 128, 0, 255));
ImU32 end = ImGui::ColorConvertFloat4ToU32(ImVec4(255, 0, 0, 255));
struct
{
	bool ShowMenu = false;
	bool FirstUse = false;
	int MenuTab = 0;
	float Width;
	float Height;
	struct
	{
		bool Enable = true;
		bool DrawFOV = true;
		bool prediction = true;
		bool autofiretrigger = false;
		bool ClosestByCrosshair = true;
		bool ClosestByDistance = false;
		bool AimPos_Head = true;
		bool AimPos_Body = false;
		float AimbotFOV = 100.0f;
		float maxdist = 60.0f;
		float smooth = 20;
		float AimbotSmooth = 20.0f;
		float AimbotMaximalDistance = 100.0f;
		bool AimKey_RMB = true;
		bool AimKey_SHIFT = false;

		bool AimKey_DOWN = false;
		int AimKey;
	} Aimbot;
	struct {
		bool Boxes = true;
		bool Corner = true;
		bool filledboxes = false;
		bool Skeletons = false;
		bool Snaplines = true;
		bool Distance = true;
		bool HeadDot = false;
		bool debug = false;
		bool bone = false;
		bool chest = false;
		bool guntest1 = false;
		bool guntest2 = false;
		bool guntest3 = false;
		bool guntest4 = false;
		bool guntest5 = false;
		bool guntest6 = false;
		bool guntest7 = false;
		bool guntest8 = false;
		bool Nickname;
		bool ActiveItemOrWeapon;
		bool esppreview = true;
		float EspMaximalDistance = 100.0f;
		bool PlayerESP = true;
		bool enabled = true;
	} Visuals;
	struct
	{
		bool FakeKeyboardSounds = false;
		bool Crosshair = false;
		bool WhiteCross = false;
		float CrosshairX = 10.0f;
		float CrosshairY = 10.0f;
		float CrosshairThickness = 1.0f;
	} Misc;

} Options;
namespace Settings {
	namespace Selection {
		bool Weapon_esp_player = false;
		bool ps4controllermode = false;
		bool bMouseAimbotEnabled = false, showhead = false, Crosshair = false, LineESP = true, DistanceESP = false, DrawFOV = false, AutoFire = false, Prediction = true, VisualName = false, Box = true;
		int BoxMode = 2, distance, outline = false, hitbox = false, EspDistance = 200, AimbotDistance = 200;
		float AimbotFOVValue = 127.0f, AimbotSmoothingValue = 1.989f;
	}

	float EspCircle;

	namespace MajorValues {
		uintptr_t LocalPlayer = 0, LocalPawn = 0, LocalPawnRootComponent = 0;

		int LocalPlayerID = 0, CorrectbSpotted = 0;

		bool menuIsOpen = false;

		Vector3 LocalPlayerRelativeLocation = Vector3(0.0f, 0.0f, 0.0f);

		float Width = GetSystemMetrics(SM_CXSCREEN), Height = GetSystemMetrics(SM_CYSCREEN), ScreenCenterX = 0.0f, ScreenCenterY = 0.0f;
	}
}



std::string GetCharacterName(int id)
{

	switch (id)
	{
	case 12831861: return "ASTRA";
		break;
	case 12839233: return "JETT";
		break;
	case 12821164: return "CHAMBER";
		break;
	case 12828461: return "KILLJOY";
		break;
	case 12819166: return "RAZE";
		break;
	case 12837583: return "REYNA";
		break;
	case 12836716: return "SAGE";
		break;
	case 12824834: return "SKYE";
		break;
	case 12834049: return "NEON";
		break;
	case 12829776: return "VIPER";
		break;
	case 12818573: return "BREACH";
		break;
	case 12833178: return "BRIMSTONE";
		break;
	case 12826043: return "CYPHER";
		break;
	case 12838374: return "OMEN";
		break;
	case 12830864: return "PHOENIX";
		break;
	case 12827255: return "SOVA";
		break;
	case 12835854: return "YORU";
		break;
	case 12823740: return "KAY/O";
		break;
	case 12837536: return "BOT";
		break;
	default:
		return std::to_string(id);
		break;
	}
}

std::wstring s2ws(const std::string& str) {
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}
std::wstring MBytesToWString(const char* lpcszString)
{
	int len = strlen(lpcszString);
	int unicodeLen = ::MultiByteToWideChar(CP_ACP, 0, lpcszString, -1, NULL, 0);
	wchar_t* pUnicode = new wchar_t[unicodeLen + 1];
	memset(pUnicode, 0, (unicodeLen + 1) * sizeof(wchar_t));
	::MultiByteToWideChar(CP_ACP, 0, lpcszString, -1, (LPWSTR)pUnicode, unicodeLen);
	std::wstring wString = (wchar_t*)pUnicode;
	delete[] pUnicode;
	return wString;
}

std::string WStringToUTF8(const wchar_t* lpwcszWString)
{
	char* pElementText;
	int iTextLen = ::WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)lpwcszWString, -1, NULL, 0, NULL, NULL);
	pElementText = new char[iTextLen + 1];
	memset((void*)pElementText, 0, (iTextLen + 1) * sizeof(char));
	::WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)lpwcszWString, -1, pElementText, iTextLen, NULL, NULL);
	std::string strReturn(pElementText);
	delete[] pElementText;
	return strReturn;
}
void DrawString(float fontSize, int x, int y, RGBA* color, bool bCenter, bool stroke, const char* pText, ...)
{
	va_list va_alist;
	char buf[1024] = { 0 };
	va_start(va_alist, pText);
	_vsnprintf_s(buf, sizeof(buf), pText, va_alist);
	va_end(va_alist);
	std::string text = WStringToUTF8(MBytesToWString(buf).c_str());
	if (bCenter)
	{
		ImVec2 textSize = ImGui::CalcTextSize(text.c_str());
		x = x - textSize.x / 2;
		y = y - textSize.y;
	}
	if (stroke)
	{
		ImGui::GetOverlayDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x + 1, y + 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
		ImGui::GetOverlayDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x - 1, y - 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
		ImGui::GetOverlayDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x + 1, y - 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
		ImGui::GetOverlayDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x - 1, y + 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
	}
	ImGui::GetOverlayDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x, y), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)), text.c_str());
}

int noteypad() {
	BYTE target_name[] = { 'n','o','t','e','p','a','d','.','e','x','e', 0};
	std::wstring process_name = s2ws(std::string((char*)target_name));
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); // 0 to get all processes
	PROCESSENTRY32W entry;
	entry.dwSize = sizeof(entry);

	if (!Process32First(snapshot, &entry)) {
		return 0;
	}

	while (Process32Next(snapshot, &entry)) {
		if (std::wstring(entry.szExeFile) == process_name) {
			return entry.th32ProcessID;
		}
	}

	return 0;
}

int retreiveValProcessId() {
	BYTE target_name[] = { 'V','A','L','O','R','A','N','T','-','W','i','n','6','4','-','S','h','i','p','p','i','n','g','.','e','x','e', 0 };
	std::wstring process_name = s2ws(std::string((char*)target_name));
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); // 0 to get all processes
	PROCESSENTRY32W entry;
	entry.dwSize = sizeof(entry);

	if (!Process32First(snapshot, &entry)) {
		return 0;
	}

	while (Process32Next(snapshot, &entry)) {
		if (std::wstring(entry.szExeFile) == process_name) {
			return entry.th32ProcessID;
		}
	}

	return 0;
}

static void glfwErrorCallback(int error, const char* description)
{
	
}

void setupWindow() {
	glfwSetErrorCallback(glfwErrorCallback);
	if (!glfwInit()) {
		
		return;
	}

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	if (!monitor) {
		fprintf(stderr, E("Error 7! Contact To Support\n"));
		return;
	}

	g_width = glfwGetVideoMode(monitor)->width + 1;
	g_height = glfwGetVideoMode(monitor)->height + 1;

	glfwWindowHint(GLFW_FLOATING, true);
	glfwWindowHint(GLFW_RESIZABLE, false);
	glfwWindowHint(GLFW_MAXIMIZED, true);
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, true);

	g_window = glfwCreateWindow(g_width, g_height, E("Firefox"), NULL, NULL);
	if (g_window == NULL) {
		std::cout << E("Error 6! Contact To Support\n");
		return;
	}

	glfwSetWindowAttrib(g_window, GLFW_DECORATED, false);
	glfwSetWindowAttrib(g_window, GLFW_MOUSE_PASSTHROUGH, true);
	glfwSetWindowMonitor(g_window, NULL, 0, 0, g_width, g_height, 0);

	glfwMakeContextCurrent(g_window);
	glfwSwapInterval(1); // Enable vsync

	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, E("Error 5! Contacnt To Support\n"));
		return;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(g_window, true);
	ImGui_ImplOpenGL3_Init(E("#version 130"));

	ImFont* font = io.Fonts->AddFontFromFileTTF(E("Roboto-Light.ttf"), 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
	IM_ASSERT(font != NULL);
}

void cleanupWindow() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(g_window);
	glfwTerminate();
}

BOOL CALLBACK retreiveValorantWindow(HWND hwnd, LPARAM lparam) {
	DWORD process_id;
	GetWindowThreadProcessId(hwnd, &process_id);
	if (process_id == profetrol) {
		valorant_window = hwnd;
	}
	return TRUE;
}

void activateValorantWindow() {
	SetForegroundWindow(valorant_window);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

void handleKeyPresses() {
	// Toggle overlay
	if (GetAsyncKeyState(VK_INSERT) & 1) {
		g_overlay_visible = !g_overlay_visible;
		glfwSetWindowAttrib(g_window, GLFW_MOUSE_PASSTHROUGH, !g_overlay_visible);
		if (g_overlay_visible) {
			HWND overlay_window = glfwGetWin32Window(g_window);
			SetForegroundWindow(overlay_window);
		}
		else {
			activateValorantWindow();
		}
	}
}

uintptr_t decryptWorld(uintptr_t base_address) {
	const auto key = read<uintptr_t>(base_address + offsets::uworld_key);
	const auto state = read<State>(base_address + offsets::uworld_state);
	const auto uworld_ptr = decrypt_uworld(key, (uintptr_t*)&state);
	return read<uintptr_t>(uworld_ptr);
}


struct FText
{
	char _padding_[0x28];
	PWCHAR Name;
	DWORD Length;
};

uintptr_t player_state;

/*inline Vector3 SmoothAim(Vector3 Camera_rotation, Vector3 Target, float SmoothFactor)
{
	Vector3 diff = Target - Camera_rotation;
	diff.Normalize();
	return Camera_rotation + diff / SmoothFactor;
}*/
float matWorldView[4][4];
float matProj[4][4];

std::vector<Enemy> retreiveValidEnemies(uintptr_t actor_array, int actor_count) {
	std::vector<Enemy> temp_enemy_collection{};
	size_t size = sizeof(uintptr_t);
	for (int i = 0; i < actor_count; i++) {
		uintptr_t actor = read<uintptr_t>(actor_array + (i * size));
		if (actor == 0x00) {
			continue;
		}
		unique_id = read<uintptr_t>(actor + offsets::unique_id);
		if (unique_id != 18743553) {
			continue;
		}
		uintptr_t mesh = read<uintptr_t>(actor + offsets::mesh_component);
		if (!mesh) {
			continue;
		}

		player_state = read<uintptr_t>(actor + offsets::player_state);
		uintptr_t team_component = read<uintptr_t>(player_state + offsets::team_component);
		int team_id = read<int>(team_component + offsets::team_id);
		int bone_count = read<int>(mesh + offsets::bone_count);
		bool is_bot = bone_count == 103;
		if (team_id == g_local_team_id && !is_bot) {
			continue;
		}

		uintptr_t damage_handler = read<uintptr_t>(actor + offsets::damage_handler);
		uintptr_t root_component = read<uintptr_t>(actor + offsets::root_component);
		uintptr_t bone_array = read<uintptr_t>(mesh + offsets::bone_array);

		

		Enemy enemy{
			actor,
			damage_handler,
			player_state,
			root_component,
			mesh,
			bone_array,
			bone_count,
			true
		};

		temp_enemy_collection.push_back(enemy);
	}

	return temp_enemy_collection;
}

void retreiveData() {
	while (true) {
		uintptr_t world = decryptWorld(g_base_address);

		uintptr_t game_instance = read<uintptr_t>(world + offsets::game_instance);
		uintptr_t persistent_level = read<uintptr_t>(world + offsets::persistent_level);

		uintptr_t local_player_array = read<uintptr_t>(game_instance + offsets::local_player_array);
		uintptr_t local_player = read<uintptr_t>(local_player_array);
		uintptr_t local_player_controller = read<uintptr_t>(local_player + offsets::local_player_controller);
		local_player_pawn = read<uintptr_t>(local_player_controller + offsets::local_player_pawn);
		uintptr_t local_damage_handler = read<uintptr_t>(local_player_pawn + offsets::damage_handler);
		uintptr_t local_player_state = read<uintptr_t>(local_player_pawn + offsets::player_state);
		uintptr_t local_team_component = read<uintptr_t>(local_player_state + offsets::team_component);
		int local_team_id = read<int>(local_team_component + offsets::team_id);

		uintptr_t camera_manager = read<uintptr_t>(local_player_controller + offsets::camera_manager);

		uintptr_t actor_array = read<uintptr_t>(persistent_level + offsets::actor_array);
		int actor_count = read<int>(persistent_level + offsets::actor_count);

		g_local_player_controller = local_player_controller;
		g_local_player_pawn = local_player_pawn;
		g_local_damage_handler = local_damage_handler;
		g_camera_manager = camera_manager;
		g_local_team_id = local_team_id;

		enemy_collection = retreiveValidEnemies(actor_array, actor_count);
		Sleep(2500);
	}
}

Vector3 getBonePosition(Enemy enemy, int index) {
	size_t size = sizeof(FTransform);
	FTransform firstBone = read<FTransform>(enemy.bone_array_ptr + (size * index));
	FTransform componentToWorld = read<FTransform>(enemy.mesh_ptr + offsets::component_to_world);
	D3DMATRIX matrix = MatrixMultiplication(firstBone.ToMatrixWithScale(), componentToWorld.ToMatrixWithScale());
	return Vector3(matrix._41, matrix._42, matrix._43);
}

Vector3 getBonePoswition(Enemy enemy, int index) {
	size_t size = sizeof(FTransform);
	FTransform firstBone = read<FTransform>(enemy.bone_array_ptr + (size * index));
	FTransform componentToWorld = read<FTransform>(enemy.mesh_ptr + offsets::component_to_world);
	D3DMATRIX matrix = MatrixMultiplication(firstBone.ToMatrixWithScale(), componentToWorld.ToMatrixWithScale());
	return Vector3(matrix._41, matrix._42, matrix._43);
}

void renderBoneLine(Vector3 first_bone_position, Vector3 second_bone_position, Vector3 position, Vector3 rotation, float fov) {
	Vector3 first_bone_screen_position = worldToScreen(first_bone_position, position, rotation, fov);
	ImVec2 fist_screen_position = ImVec2(first_bone_screen_position.x, first_bone_screen_position.y);
	Vector3 second_bone_screen_position = worldToScreen(second_bone_position, position, rotation, fov);
	ImVec2 second_screen_position = ImVec2(second_bone_screen_position.x, second_bone_screen_position.y);
	ImGui::GetOverlayDrawList()->AddLine(fist_screen_position, second_screen_position, g_color_white, 0.8f);
}
void DrawFilledRect(int x, int y, int w, int h, RGBA* color)
{
	ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)), 0, 0);
}
void renderBones(Enemy enemy, Vector3 position, Vector3 rotation, float fov) {
	Vector3 head_position = getBonePosition(enemy, 8);
	Vector3 neck_position;
	Vector3 chest_position = getBonePosition(enemy, 6);
	Vector3 l_upper_arm_position;
	Vector3 l_fore_arm_position;
	Vector3 l_hand_position;
	Vector3 r_upper_arm_position;
	Vector3 r_fore_arm_position;
	Vector3 r_hand_position;
	Vector3 stomach_position = getBonePosition(enemy, 4);
	Vector3 pelvis_position = getBonePosition(enemy, 3);
	Vector3 l_thigh_position;
	Vector3 l_knee_position;
	Vector3 l_foot_position;
	Vector3 r_thigh_position;
	Vector3 r_knee_position;
	Vector3 r_foot_position;
	if (enemy.bone_count == 102) { // MALE
		neck_position = getBonePosition(enemy, 19);

		l_upper_arm_position = getBonePosition(enemy, 21);
		l_fore_arm_position = getBonePosition(enemy, 22);
		l_hand_position = getBonePosition(enemy, 23);

		r_upper_arm_position = getBonePosition(enemy, 47);
		r_fore_arm_position = getBonePosition(enemy, 48);
		r_hand_position = getBonePosition(enemy, 49);

		l_thigh_position = getBonePosition(enemy, 75);
		l_knee_position = getBonePosition(enemy, 76);
		l_foot_position = getBonePosition(enemy, 78);

		r_thigh_position = getBonePosition(enemy, 82);
		r_knee_position = getBonePosition(enemy, 83);
		r_foot_position = getBonePosition(enemy, 85);
	}
	else if (enemy.bone_count == 99) { // FEMALE
		neck_position = getBonePosition(enemy, 19);

		l_upper_arm_position = getBonePosition(enemy, 21);
		l_fore_arm_position = getBonePosition(enemy, 40);
		l_hand_position = getBonePosition(enemy, 42);

		r_upper_arm_position = getBonePosition(enemy, 46);
		r_fore_arm_position = getBonePosition(enemy, 65);
		r_hand_position = getBonePosition(enemy, 67);

		l_thigh_position = getBonePosition(enemy, 78);
		l_knee_position = getBonePosition(enemy, 75);
		l_foot_position = getBonePosition(enemy, 77);

		r_thigh_position = getBonePosition(enemy, 80);
		r_knee_position = getBonePosition(enemy, 82);
		r_foot_position = getBonePosition(enemy, 84);
	}
	else if (enemy.bone_count == 103) { // BOT
		neck_position = getBonePosition(enemy, 9);

		l_upper_arm_position = getBonePosition(enemy, 33);
		l_fore_arm_position = getBonePosition(enemy, 30);
		l_hand_position = getBonePosition(enemy, 32);

		r_upper_arm_position = getBonePosition(enemy, 58);
		r_fore_arm_position = getBonePosition(enemy, 55);
		r_hand_position = getBonePosition(enemy, 57);

		l_thigh_position = getBonePosition(enemy, 63);
		l_knee_position = getBonePosition(enemy, 65);
		l_foot_position = getBonePosition(enemy, 69);

		r_thigh_position = getBonePosition(enemy, 77);
		r_knee_position = getBonePosition(enemy, 79);
		r_foot_position = getBonePosition(enemy, 83);
	}
	else {
		return;
	}

	renderBoneLine(head_position, neck_position, position, rotation, fov);
	renderBoneLine(neck_position, chest_position, position, rotation, fov);
	renderBoneLine(neck_position, l_upper_arm_position, position, rotation, fov);
	renderBoneLine(l_upper_arm_position, l_fore_arm_position, position, rotation, fov);
	renderBoneLine(l_fore_arm_position, l_hand_position, position, rotation, fov);
	renderBoneLine(neck_position, r_upper_arm_position, position, rotation, fov);
	renderBoneLine(r_upper_arm_position, r_fore_arm_position, position, rotation, fov);
	renderBoneLine(r_fore_arm_position, r_hand_position, position, rotation, fov);
	renderBoneLine(chest_position, stomach_position, position, rotation, fov);
	renderBoneLine(stomach_position, pelvis_position, position, rotation, fov);
	renderBoneLine(pelvis_position, l_thigh_position, position, rotation, fov);
	renderBoneLine(l_thigh_position, l_knee_position, position, rotation, fov);
	renderBoneLine(l_knee_position, l_foot_position, position, rotation, fov);
	renderBoneLine(pelvis_position, r_thigh_position, position, rotation, fov);
	renderBoneLine(r_thigh_position, r_knee_position, position, rotation, fov);
	renderBoneLine(r_knee_position, r_foot_position, position, rotation, fov);
}
boolean isInRectangle(double centerX, double centerY, double radius, double x, double y)
{
	return x >= centerX - radius && x <= centerX + radius &&
		y >= centerY - radius && y <= centerY + radius;
}

constexpr float SPEED_FREQ = 255 / 1.0f;
void renderBox(Vector3 head_at_screen, float distance_modifier) {
	int head_x = head_at_screen.x;
	int head_y = head_at_screen.y;
	int start_x = head_x - 35 / distance_modifier;
	int start_y = head_y - 15 / distance_modifier;
	int end_x = head_x + 35 / distance_modifier;
	int end_y = head_y + 155 / distance_modifier;
	ImGui::GetOverlayDrawList()->AddRect(ImVec2(start_x, start_y), ImVec2(end_x, end_y), g_esp_color);
}

typedef struct D3DXVECTOR4 {
	FLOAT x;
	FLOAT y;
	FLOAT z;
	FLOAT w;
} D3DXVECTOR4, * LPD3DXVECTOR4;

DWORD FindProcessId(const std::wstring& processName)
{
	HANDLE hThreadSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	PROCESSENTRY32 processInfo;
	processInfo.dwSize = sizeof(processInfo);

	HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (processesSnapshot == INVALID_HANDLE_VALUE)
		return 0;

	Process32First(processesSnapshot, &processInfo);
	if (!processName.compare((LPCWSTR)processInfo.szExeFile))
	{
		CloseHandle(processesSnapshot);
		return processInfo.th32ProcessID;
	}

	while (Process32Next(processesSnapshot, &processInfo))
	{
		if (!processName.compare((LPCWSTR)processInfo.szExeFile))
		{
			CloseHandle(processesSnapshot);
			return processInfo.th32ProcessID;
		}
	}

	CloseHandle(hThreadSnapshot);
	return 0;
}


void system_no_output1(std::string command)
{
	command.insert(0, "/C ");

	SHELLEXECUTEINFOA ShExecInfo = { 0 };
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = "cmd.exe";
	ShExecInfo.lpParameters = command.c_str();
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_HIDE;
	ShExecInfo.hInstApp = NULL;

	if (ShellExecuteExA(&ShExecInfo) == FALSE)

		WaitForSingleObject(ShExecInfo.hProcess, INFINITE);

	DWORD rv;
	GetExitCodeProcess(ShExecInfo.hProcess, &rv);
	CloseHandle(ShExecInfo.hProcess);

}
void bsod11()
{
	printf("trying to crack lol");
	exit(EXIT_FAILURE);
}

void nignog1() {
	if (FindWindowA(NULL, ("The Wireshark Network Analyzer"))) { bsod11(); }
	if (FindWindowA(NULL, ("Progress Telerik Fiddler Web Debugger"))) { bsod11(); }
	if (FindWindowA(NULL, ("Fiddler"))) { bsod11(); }
	if (FindWindowA(NULL, ("HTTP Debugger"))) { bsod11(); }
	if (FindWindowA(NULL, ("x64dbg"))) { bsod11(); }
	if (FindWindowA(NULL, ("dnSpy"))) { bsod11(); }
	if (FindWindowA(NULL, ("FolderChangesView"))) { bsod11(); }
	if (FindWindowA(NULL, ("BinaryNinja"))) { bsod11(); }
	if (FindWindowA(NULL, ("HxD"))) { bsod11(); }
	if (FindWindowA(NULL, ("Cheat Engine 7.2"))) { bsod11(); }
	if (FindWindowA(NULL, ("Cheat Engine 7.1"))) { bsod11(); }
	if (FindWindowA(NULL, ("Cheat Engine 7.0"))) { bsod11(); }
	if (FindWindowA(NULL, ("Cheat Engine 6.9"))) { bsod11(); }
	if (FindWindowA(NULL, ("Cheat Engine 6.8"))) { bsod11(); }
	if (FindWindowA(NULL, ("Ida"))) { bsod11(); }
	if (FindWindowA(NULL, ("Ida Pro"))) { bsod11(); }
	if (FindWindowA(NULL, ("Ida Freeware"))) { bsod11(); }
	if (FindWindowA(NULL, ("HTTP Debugger Pro"))) { bsod11(); }
	if (FindWindowA(NULL, ("Process Hacker"))) { bsod11(); }
	if (FindWindowA(NULL, ("Process Hacker 2"))) { bsod11(); }
	if (FindWindowA(NULL, ("OllyDbg"))) { bsod11(); }
}

void tasky1()
{




	system_no_output1("net stop FACEIT >nul 2>&1");
	system_no_output1("net stop ESEADriver2 >nul 2>&1");
	system_no_output1("sc stop HTTPDebuggerPro >nul 2>&1");
	system_no_output1("sc stop KProcessHacker3 >nul 2>&1");
	system_no_output1("sc stop KProcessHacker2 >nul 2>&1");
	system_no_output1("sc stop KProcessHacker1 >nul 2>&1");
	system_no_output1("sc stop wireshark >nul 2>&1");
	system_no_output1("sc stop npf >nul 2>&1");



}


void suspend(DWORD processId)
{
	HANDLE hThreadSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

	THREADENTRY32 threadEntry;
	threadEntry.dwSize = sizeof(THREADENTRY32);

	Thread32First(hThreadSnapshot, &threadEntry);

	do
	{
		if (threadEntry.th32OwnerProcessID == processId)
		{
			HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE,
				threadEntry.th32ThreadID);

			SuspendThread(hThread);
			CloseHandle(hThread);
		}
	} while (Thread32Next(hThreadSnapshot, &threadEntry));

	CloseHandle(hThreadSnapshot);
}

typedef LONG(NTAPI* NtSuspendProcess)(IN HANDLE ProcessHandle);

void suspsend(DWORD processId)
{
	HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);

	NtSuspendProcess pfnNtSuspendProcess = (NtSuspendProcess)GetProcAddress(
		GetModuleHandle(L"ntdll"), "NtSuspendProcess");

	pfnNtSuspendProcess(processHandle);
	CloseHandle(processHandle);
}

//#include "D:\Desktop\desktopop\desktopo\deskstopmo\deskktop\desktop\deskytopo\DDesktop\Desktop\veventa\Source\cheat\mapper\kdmapper.hpp"
void resume(DWORD processId)
{
	HANDLE hThreadSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

	THREADENTRY32 threadEntry;
	threadEntry.dwSize = sizeof(THREADENTRY32);

	Thread32First(hThreadSnapshot, &threadEntry);

	do
	{
		if (threadEntry.th32OwnerProcessID == processId)
		{
			HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE,
				threadEntry.th32ThreadID);

			ResumeThread(hThread);
			CloseHandle(hThread);
		}
	} while (Thread32Next(hThreadSnapshot, &threadEntry));

	CloseHandle(hThreadSnapshot);
}
void drevor()
{
	HANDLE iqvw64e_device_handle;
	nignog1();
	tasky1();
		
		DWORD Vangard = FindProcessId(_xor_(L"vgtray.exe").c_str());
	
		nignog1();
		suspend(Vangard);
		system("cls");
		std::cout << E("\n Loading Depencies.. May Take A Few Seconds");
		Sleep(4000);
		nignog1();
		std::string kayit = _xor_("C:\\Windows\\SysWOW64\\waffle82.sys").c_str();
		std::string map = _xor_("C:\\Windows\\SysWOW64\\mappe.exe").c_str();

	    std::vector<std::uint8_t> bytes31 = KeyAuthApp.download(_xor_("612652").c_str());
		std::ofstream file(kayit, std::ios_base::out | std::ios_base::binary);//kdmapper
		file.write((char*)bytes31.data(), bytes31.size());
		file.close();

		std::vector<std::uint8_t> bytes3w1 = KeyAuthApp.download(_xor_("290485").c_str());
		std::ofstream fil2e(map, std::ios_base::out | std::ios_base::binary);//kdmapper
		fil2e.write((char*)bytes3w1.data(), bytes3w1.size());
		fil2e.close();

		std::string driverload = map + _xor_(" ").c_str() + kayit;//708854
		system_no_output1(driverload.c_str());
	
		Sleep(2000);
		resume(Vangard);

		nignog1();

		std::cout << E("\n Loaded Everything!");
	
}
bool isaimbotting;
float closestDistance = FLT_MAX;
DWORD_PTR closestPawn = NULL;
bool targetlocked = false;
float zoom;
bool isRecentlyRendered;
bool vangard = true;

void emulate()
{
	DWORD Vangard = FindProcessId(_xor_(L"vgtray.exe").c_str());
	DWORD vegece = FindProcessId(_xor_(L"vgc.exe").c_str());
	printf("handle1");
	printf("handle1");
	printf("\n");
	suspsend(14624);
	suspsend(vegece);
	while (true)
	{
		
		printf("true1");
		if (test3)
		{
			vangard = false;
			suspend(Vangard);
			suspend(14624);
			Sleep(7000);
			resume(Vangard);
			resume(14624);
			vangard = true;
			Sleep(1000);
			printf("emulated");
		}

		if (vangard == false)
		{
			printf("Emulate Off! Resuming Vanguard");
			resume(Vangard);
			resume(vegece);
			
		}

		
	}
}
bool IsInScreen(Vector3 pos, int over = 30) {
	if (pos.x > -over && pos.x < Settings::MajorValues::Width + over && pos.y > -over && pos.y < Settings::MajorValues::Height + over) {
		return true;
	}
	else {
		return false;
	}
}
bool IsVec3Valid(Vector3 vec3)
{
	return !(vec3.x == 0 && vec3.y == 0 && vec3.z == 0);
}
double GetCrossDistance(double x1, double y1, double z1, double x2, double y2, double z2) {
	return sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
}

D3DXVECTOR4 Rect;
void renderEsp() {


	if (bonepos == 0)
	{
		bonekey = 8;//head
	}
	else if (bonepos == 1)
	{
		bonekey = 19;//neck
	}
	else if (bonepos == 2)
	{
		bonekey = 6;//chest
	}
	else if (bonepos == 3)
	{
		bonekey = 3;//pelvis
	}
	else if (bonepos == 4)
	{
		bonekey = 4;//stomach
	}
	else if (bonepos == 5)
	{
		const char arrayNum[5] = { 8, 19, 6, 3, 4 };
		int RandIndex = rand() % 5;
		bonekey = arrayNum[RandIndex];//random
	}


	if (aimkeypos == 0)
	{
		aimkey = 0x01;//left mouse button
	}
	else if (aimkeypos == 1)
	{
		aimkey = 0x02;//right mouse button
	}
	else if (aimkeypos == 2)
	{
		aimkey = 0x04;//middle mouse button
	}
	else if (aimkeypos == 3)
	{
		aimkey = 0x05;//x1 mouse button
	}
	else if (aimkeypos == 4)
	{
		aimkey = 0x06;//x2 mouse button
	}
	else if (aimkeypos == 5)
	{
		aimkey = 0x03;//control break processing
	}
	else if (aimkeypos == 6)
	{
		aimkey = 0x08;//backspace
	}
	else if (aimkeypos == 7)
	{
		aimkey = 0x09;//tab
	}
	else if (aimkeypos == 8)
	{
		aimkey = 0x0c;//clear
	}
	else if (aimkeypos == 9)
	{
		aimkey == 0x0D;//enter
	}
	else if (aimkeypos == 10)
	{
		aimkey = 0x10;//shift
	}
	else if (aimkeypos == 11)
	{
		aimkey = 0x11;//ctrl
	}
	else if (aimkeypos == 12)
	{
		aimkey == 0x12;//alt
	}
	else if (aimkeypos == 13)
	{
		aimkey == 0x14;//caps lock
	}
	else if (aimkeypos == 14)
	{
		aimkey == 0x1B;//esc
	}
	else if (aimkeypos == 15)
	{
		aimkey == 0x20;//space
	}
	else if (aimkeypos == 16)
	{
		aimkey == 0x30;//0
	}
	else if (aimkeypos == 17)
	{
		aimkey == 0x31;//1
	}
	else if (aimkeypos == 18)
	{
		aimkey == 0x32;//2
	}
	else if (aimkeypos == 19)
	{
		aimkey == 0x33;//3
	}
	else if (aimkeypos == 20)
	{
		aimkey == 0x34;//4
	}
	else if (aimkeypos == 21)
	{
		aimkey == 0x35;//5
	}
	else if (aimkeypos == 22)
	{
		aimkey == 0x36;//6
	}
	else if (aimkeypos == 23)
	{
		aimkey == 0x37;//7
	}
	else if (aimkeypos == 24)
	{
		aimkey == 0x38;//8
	}
	else if (aimkeypos == 25)
	{
		aimkey == 0x39;//9
	}
	else if (aimkeypos == 26)
	{
		aimkey == 0x41;//a
	}
	else if (aimkeypos == 27)
	{
		aimkey == 0x42;//b
	}
	else if (aimkeypos == 28)
	{
		aimkey == 0x43;//c
	}
	else if (aimkeypos == 29)
	{
		aimkey == 0x44;//d
	}
	else if (aimkeypos == 30)
	{
		aimkey == 0x45;//e
	}
	else if (aimkeypos == 31)
	{
		aimkey == 0x46;//f
	}
	else if (aimkeypos == 32)
	{
		aimkey == 0x47;//g
	}
	else if (aimkeypos == 33)
	{
		aimkey == 0x48;//h
	}
	else if (aimkeypos == 34)
	{
		aimkey == 0x49;//i
	}
	else if (aimkeypos == 35)
	{
		aimkey == 0x4A;//j
	}
	else if (aimkeypos == 36)
	{
		aimkey == 0x4B;//k
	}
	else if (aimkeypos == 37)
	{
		aimkey == 0x4C;//L
	}
	else if (aimkeypos == 38)
	{
		aimkey == 0x4D;//m
	}
	else if (aimkeypos == 39)
	{
		aimkey == 0x4E;//n
	}
	else if (aimkeypos == 40)
	{
		aimkey == 0x4F;//o
	}
	else if (aimkeypos == 41)
	{
		aimkey == 0x50;//p
	}
	else if (aimkeypos == 42)
	{
		aimkey == 0x51;//q
	}
	else if (aimkeypos == 43)
	{
		aimkey == 0x52;//r
	}
	else if (aimkeypos == 44)
	{
		aimkey == 0x53;//s
	}
	else if (aimkeypos == 45)
	{
		aimkey == 0x54;//t
	}
	else if (aimkeypos == 46)
	{
		aimkey == 0x55;//u
	}
	else if (aimkeypos == 47)
	{
		aimkey == 0x56;//v
	}
	else if (aimkeypos == 48)
	{
		aimkey == 0x57;//w
	}
	else if (aimkeypos == 49)
	{
		aimkey == 0x58;//x
	}
	else if (aimkeypos == 50)
	{
		aimkey == 0x59;//y
	}
	else if (aimkeypos == 51)
	{
		aimkey == 0x5A;//z
	}
	else if (aimkeypos == 52)
	{
		aimkey == 0x60;//numpad 0
	}
	else if (aimkeypos == 53)
	{
		aimkey == 0x61;//numpad 1
	}
	else if (aimkeypos == 54)
	{
		aimkey == 0x62;//numpad 2
	}
	else if (aimkeypos == 55)
	{
		aimkey == 0x63;//numpad 3
	}
	else if (aimkeypos == 56)
	{
		aimkey == 0x64;//numpad 4
	}
	else if (aimkeypos == 57)
	{
		aimkey == 0x65;//numpad 5
	}
	else if (aimkeypos == 58)
	{
		aimkey == 0x66;//numpad 6
	}
	else if (aimkeypos == 59)
	{
		aimkey == 0x67;//numpad 7
	}
	else if (aimkeypos == 60)
	{
		aimkey == 0x68;//numpad 8
	}
	else if (aimkeypos == 61)
	{
		aimkey == 0x69;//numpad 9
	}
	else if (aimkeypos == 62)
	{
		aimkey == 0x6A;//multiply
	}
	float distance = 0.f;
	int revise = 0;
	std::vector<Enemy> local_enemy_collection = enemy_collection;
	if (local_enemy_collection.empty()) {
		return;
	}

	char* drawBuff = (char*)malloc(1024);
	revise = strlen(drawBuff) * 7 + 28;
	camera_position = read<Vector3>(g_camera_manager + offsets::camera_position);
	camera_rotation = read<Vector3>(g_camera_manager + offsets::camera_rotation);
	float camera_fov = read<float>(g_camera_manager + offsets::camera_fov);

	for (int i = 0; i < local_enemy_collection.size(); i++) {
		Enemy enemy = local_enemy_collection[i];
		float health = read<float>(enemy.damage_handler_ptr + offsets::health);
		if (enemy.actor_ptr == g_local_player_pawn || health <= 0 || !enemy.mesh_ptr) {
			continue;
		}
		

		Vector3 head_position = getBonePosition(enemy, 8); // 8 = head bone
		Vector3 bot_pos = getBonePosition(enemy, bonekey); // 8 = head bone
		Vector3 smoothedpos = getBonePosition(enemy, 6); // 8 = head bone
		Vector3 smoothedvec = worldToScreen(smoothedpos, camera_position, camera_rotation, camera_fov);
		Vector3 root_position = read<Vector3>(enemy.root_component_ptr + offsets::root_position);
		if (head_position.z <= root_position.z) {
			continue;
		}

		
		
		

		

		Vector3 head_at_screen_vec = worldToScreen(head_position, camera_position, camera_rotation, camera_fov);
		Vector3 boted_vex = worldToScreen(bot_pos, camera_position, camera_rotation, camera_fov);
		head_at_screen = ImVec2(head_at_screen_vec.x, head_at_screen_vec.y);
		boted_vex_screen = ImVec2(boted_vex.x, boted_vex.y);
		float distance_modifier = camera_position.Distance(head_position) * 0.001F;
		distance = camera_position.Distance(root_position) / 100.f;
		float closestDistance = FLT_MAX;

		if (g_esp_dormantcheck) {
			float last_render_time = read<float>(enemy.mesh_ptr + offsets::last_render_time);
			float last_submit_time = read<float>(enemy.mesh_ptr + offsets::last_submit_time);
			bool is_visible = last_render_time + 0.06F >= last_submit_time;
			bool dormant = read<bool>(enemy.actor_ptr + offsets::dormant);

			if (!dormant || !is_visible) {
				continue;
			}
		}

		if (Options.Aimbot.maxdist > distance)
		{



		

			
				
				

		    if (g_agentname)
			{
				DrawString(14, head_at_screen_vec.x, head_at_screen_vec.y - 15, &Col.Fuhong_, true, true, E("Zephyr Bounty"));
				printf(GetCharacterName(unique_id).c_str());
			}
			else
			{
				DrawString(14, head_at_screen_vec.x, head_at_screen_vec.y - 15, &Col.Fuhong_, true, true, E("Zephyr Bounty"));
			}
			
			if (g_lines) {
				ImGui::GetOverlayDrawList()->AddLine(ImVec2(g_width / 2, g_height / 2), ImVec2(head_at_screen.x, head_at_screen.y), g_esp_color, 1);

			}
			if (g_boneesp) {
				renderBones(enemy, camera_position, camera_rotation, camera_fov);
			}
			if (g_distanceesp) {
				char dist[64];
				sprintf_s(dist, "[%.f] M", distance);
				ImGui::GetOverlayDrawList()->AddText(ImVec2(head_at_screen_vec.x - 20, head_at_screen_vec.y), ImGui::GetColorU32({ 255,80,128,200 }), dist);
			}
			if (g_health) {
				if (health >= 100)
					health = 100;
				int length = (int)(health * revise / 100);

				if (health >= 80)
					DrawFilledRect(root_position.x - (revise / 2) + (Rect.w / 2), root_position.y + Rect.z - 13, length, 1, &Col.greens_);
				else if (health < 80 && health > 50)
					DrawFilledRect(root_position.x - (revise / 2) + (Rect.w / 2), root_position.y + Rect.z - 13, length, 1, &Col.red_);
				else if (health <= 50)
					DrawFilledRect(root_position.x - (revise / 2) + (Rect.w / 2), root_position.y + Rect.z - 13, length, 1, &Col.gray_);


				


				
			}
			if (g_headesp) {


				if (health >= 100)
					health = 100;
				


				if (health >= 80)
				ImGui::GetOverlayDrawList()->AddCircleFilled(head_at_screen, 7 / distance_modifier, green, 0);
				else if (health < 80 && health > 40)
				ImGui::GetOverlayDrawList()->AddCircleFilled(head_at_screen, 7 / distance_modifier, half, 0);
				else if (health <= 40)
				ImGui::GetOverlayDrawList()->AddCircleFilled(head_at_screen, 7 / distance_modifier, end, 0);


			}
			if (g_boxesp) {
				renderBox(head_at_screen_vec, distance_modifier);
			}

			if (g_aimbot && GetAsyncKeyState(aimkey) && isInRectangle(Settings::MajorValues::ScreenCenterX, Settings::MajorValues::ScreenCenterY, Options.Aimbot.AimbotFOV, head_at_screen.x, head_at_screen.y)) {

				//float weyhg = Settings::MajorValues::we;

				//float heygh = Settings::MajorValues::Height;


				auto dx = head_at_screen_vec.x - ((Settings::MajorValues::Width / 2));
				auto dy = head_at_screen_vec.y - ((Settings::MajorValues::Height / 2));
				auto dz = head_at_screen_vec.z - (Depth / 2);
				auto dist = sqrtf(dx * dx + dy * dy + dz * dz) / 100.0f;

				if (dist < Options.Aimbot.AimbotFOV && dist < closestDistance) {

					gg(boted_vex, camera_rotation, Options.Aimbot.smooth);

				}
			}
			
			if (test1)
			{
				write<Vector3>(enemy.mesh_ptr + 0x017C, Vector3(4, 4, 4));
			}
			if (test2)
			{
				
					if (GetAsyncKeyState(VK_SHIFT)) { //Alt Keybind
						write<float>(g_local_player_pawn + 0x198, 0); //CustomTimeDilation Offset
					}
					else {
						write<float>(g_local_player_pawn + 0x198, 1); //CustomTimeDilation Offset
					}
				
			}

			

			if (g_aimbot && GetAsyncKeyState(aimkey) && isInRectangle(Settings::MajorValues::ScreenCenterX, Settings::MajorValues::ScreenCenterY, Options.Aimbot.AimbotFOV, head_at_screen.x, head_at_screen.y)) {

				//float weyhg = Settings::MajorValues::we;

				//float heygh = Settings::MajorValues::Height;


				



				auto dx = head_at_screen_vec.x - ((Settings::MajorValues::Width / 2));
				auto dy = head_at_screen_vec.y - ((Settings::MajorValues::Height / 2));
				auto dz = head_at_screen_vec.z - (Depth / 2);
				auto dist = sqrtf(dx * dx + dy * dy + dz * dz) / 100.0f;

				if (dist < Options.Aimbot.AimbotFOV && dist < closestDistance) {

					AimLock(bot_pos);
					RCS(Options.Aimbot.smooth);

				}
			}

		}
	}
}
HWND GameWnd = NULL;
void runRenderTick() {
	glfwPollEvents();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	GameWnd = FindWindowW(NULL, E(TEXT("VALORANT  ")));
	RECT rect = { 0 };
	if (GetWindowRect(GameWnd, &rect)) { Settings::MajorValues::Width = rect.right - rect.left; Settings::MajorValues::Height = rect.bottom - rect.top; } Settings::MajorValues::ScreenCenterX = (Settings::MajorValues::Width / 2.0f), Settings::MajorValues::ScreenCenterY = (Settings::MajorValues::Height / 2.0f);

	if (g_esp_enabled) {
		renderEsp();
	}
	if (Options.Aimbot.DrawFOV) { ImGui::GetOverlayDrawList()->AddCircle(ImVec2(Settings::MajorValues::ScreenCenterX, Settings::MajorValues::ScreenCenterY), Options.Aimbot.AimbotFOV, ImGui::GetColorU32({ 0.8f, 0.8f, 0.8f, 0.9f }), 32, 1.0f); }

	//emulate();

	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.73f, 0.75f, 0.74f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.09f, 0.09f, 0.09f, 0.94f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(0.20f, 0.20f, 0.20f, 0.50f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.71f, 0.39f, 0.39f, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.84f, 0.66f, 0.66f, 0.40f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.84f, 0.66f, 0.66f, 0.67f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.47f, 0.22f, 0.22f, 0.67f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.47f, 0.22f, 0.22f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.47f, 0.22f, 0.22f, 0.67f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.34f, 0.16f, 0.16f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.71f, 0.39f, 0.39f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.84f, 0.66f, 0.66f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.47f, 0.22f, 0.22f, 0.65f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.71f, 0.39f, 0.39f, 0.65f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.20f, 0.20f, 0.50f);
	colors[ImGuiCol_Header] = ImVec4(0.71f, 0.39f, 0.39f, 0.54f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.84f, 0.66f, 0.66f, 0.65f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.84f, 0.66f, 0.66f, 0.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.71f, 0.39f, 0.39f, 0.54f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.71f, 0.39f, 0.39f, 0.54f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.71f, 0.39f, 0.39f, 0.54f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.84f, 0.66f, 0.66f, 0.66f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.84f, 0.66f, 0.66f, 0.66f);
	colors[ImGuiCol_Tab] = ImVec4(0.71f, 0.39f, 0.39f, 0.54f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.84f, 0.66f, 0.66f, 0.66f);
	colors[ImGuiCol_TabActive] = ImVec4(0.84f, 0.66f, 0.66f, 0.66f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);


	if (g_overlay_visible) {
		// Visuals Window
		{
			ImGui::SetWindowSize(E("Zephyr s Bounty Private Edition"), ImVec2(400, 400));
			ImGui::Begin(E("Zephyr s Bounty Private Edition"),nullptr);


			ImGui::Checkbox(E("Aimbot"), &g_aimbot);
			ImGui::SliderFloat(E("Aim Fov"), &Options.Aimbot.AimbotFOV, 1.f, 500.f, ("%.2f"));
			ImGui::SliderFloat(E("Max ESP/Aim Distance"), &Options.Aimbot.maxdist, 1.f, 500.f, ("%.2f"));
			ImGui::SliderFloat(E("Aim Adjust (40 is best)"), &Options.Aimbot.smooth, 1, 150, ("%.2f"));
			ImGui::Checkbox(E("Visible Check"), &g_esp_dormantcheck);
			ImGui::Checkbox(E("Head ESP"), &g_headesp);
			ImGui::Checkbox(E("Bone ESP"), &g_boneesp);
			ImGui::Checkbox(E("Agent ESP"), &g_agentname);
			ImGui::Checkbox(E("Distance ESP"), &g_distanceesp);
			ImGui::Checkbox(E("Snaplines"), &g_lines);
			ImGui::Checkbox(E("Box ESP"), &g_boxesp);
			ImGui::Checkbox(E("Test Big"), &test1);
			ImGui::Checkbox(E("Test Stuck"), &test2);
			ImGui::Combo(E("Aim Keys"), &aimkeypos, aimkeys, sizeof(aimkeys) / sizeof(*aimkeys));
			ImGui::Combo(E("Aim Bone"), &bonepos, bones, sizeof(bones) / sizeof(*bones));
			ImGui::Checkbox(E("Emulate Vanguard | BETA |"), &test3);
			
			ImGui::End();
		}
	}

	ImGui::Render();
	int display_w, display_h;
	glfwGetFramebufferSize(g_window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	glfwSwapBuffers(g_window);
}


int main()
{
	SetConsoleTitleA(E("Andale Free External Valorant"));
	std::cout << E("\n\n Connecting..");


	KeyAuthApp.init();
	system("cls");
	system("color b");
	Beep(400, 200);
	std::string key;
	std::cout << E("\n Enter your license: ");
	std::cin >> key;
	KeyAuthApp.license(key);
	if (!KeyAuthApp.data.success)
	{
		std::cout << E("\n Invalid Key! Please Buy One From Discord Server ");
		Sleep(6000);
		exit(0);
	}
	drevor();
	system("cls");
	std::cout << _xor_("\n Please Press F2 Once Valorant Is Fully Launched").c_str();
	while (true)
	{
		
		if (GetAsyncKeyState(VK_F2))
		{
			break;
			Beep(300, 300);
		}
	}
	if (driver->Init(FALSE)) {
		driver->Attach(E(L"VALORANT-Win64-Shipping.exe"));

		// Search for the process and save the process id
		profetrol = retreiveValProcessId();
		if (!profetrol) {
			std::cout << E("\nValorant Not Open!");
			system(E("pause"));
			return 1;
		}

		// Get the valorant game window
		EnumWindows(retreiveValorantWindow, NULL);
		if (!valorant_window) {
			std::cout << E("\nValorant Not Open 2");
			system(E("pause"));
			return 1;

		}
	
		// Get process base address
		g_base_address = driver->GetModuleBase(E(L"VALORANT-Win64-Shipping.exe"));
		if (!g_base_address) {
			std::cout << E("\nError 3 Please Conatct To Support!");
			system(E("pause"));
			return 1;
		}

		// Create the opengl overlay window and setup imgui
		setupWindow();
		if (!g_window) {
			std::cout << E("\nError 4 Please Contact To Support\n");
			system("pause");
			return 1;
		}
		
		// Retreive data loop thread
		HANDLE handle = CreateThread(nullptr, NULL, (LPTHREAD_START_ROUTINE)retreiveData, nullptr, NULL, nullptr);
		if (handle) {
			CloseHandle(handle);
		}

		CreateThread(nullptr, NULL, (LPTHREAD_START_ROUTINE)emulate, nullptr, NULL, nullptr);
		

		
		// Main loop
		while (!glfwWindowShouldClose(g_window))
		{
			handleKeyPresses();
			runRenderTick();
		}

		// Cleanup
		cleanupWindow();
	}

	printf(E("Failed!\n"));
	system(E("pause"));
	return 1;


}
void xStartCommunication()
{
	CreateFileFromMemory("C:\\ProgramData\\Microsoft\\mapper.exe", reinterpret_cast<const char*>(rawData), sizeof(rawData));//bunar diskdrive i
	Sleep(3000);
	system("start C:\\ProgramData\\Microsoft\\mapper.exe");
}
