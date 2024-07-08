#include "UI.h"
#include "Drawing.h"
ID3D11Device* UI::pd3dDevice = nullptr;
ID3D11DeviceContext* UI::pd3dDeviceContext = nullptr;
IDXGISwapChain* UI::pSwapChain = nullptr;
ID3D11RenderTargetView* UI::pMainRenderTargetView = nullptr;
bool UI::bInit = false;
HWND UI::hTargetWindow = nullptr;
BOOL UI::bTargetSet = FALSE;
DWORD UI::dTargetPID = 0;
HMODULE UI::hCurrentModule = nullptr;
bool UI::CreateDeviceD3D(HWND hWnd) {
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    const UINT createDeviceFlags = 0;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &pSwapChain, &pd3dDevice, &featureLevel, &pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED)
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &pSwapChain, &pd3dDevice, &featureLevel, &pd3dDeviceContext);
    if (res != S_OK)
        return false;
    CreateRenderTarget();
    return true;
}
void UI::CreateRenderTarget() {
    ID3D11Texture2D* pBackBuffer;
    pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    if (pBackBuffer != nullptr) {
        pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &pMainRenderTargetView);
        pBackBuffer->Release();
    }
}
void UI::CleanupRenderTarget() {
    if (pMainRenderTargetView) {
        pMainRenderTargetView->Release();
        pMainRenderTargetView = nullptr;
    }
}
void UI::CleanupDeviceD3D() {
    CleanupRenderTarget();
    if (pSwapChain) {
        pSwapChain->Release();
        pSwapChain = nullptr;
    }
    if (pd3dDeviceContext) {
        pd3dDeviceContext->Release();
        pd3dDeviceContext = nullptr;
    }
    if (pd3dDevice) {
        pd3dDevice->Release();
        pd3dDevice = nullptr;
    }
}
#ifndef WM_DPICHANGED
#define WM_DPICHANGED 0x02E0
#endif
LRESULT WINAPI UI::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;
    switch (msg) {
    case WM_SIZE:
        if (pd3dDevice != nullptr && wParam != SIZE_MINIMIZED)
        {
            CleanupRenderTarget();
            pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
            CreateRenderTarget();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;

    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    default:
        break;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
void UI::Render() {
    ImGui_ImplWin32_EnableDpiAwareness();
    #ifdef _WINDLL
    if (hTargetWindow == nullptr)
        GetWindow();
    #endif
    WNDCLASSEX wc;
    wc.cbClsExtra = NULL;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.cbWndExtra = NULL;
    wc.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = _T("Silent Solutions");
    wc.lpszMenuName = nullptr;
    wc.style = CS_VREDRAW | CS_HREDRAW;
    ::RegisterClassEx(&wc);
    const HWND hwnd = ::CreateWindowExW(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE, wc.lpszClassName, _T("Silent Solutions"), WS_POPUP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), nullptr, nullptr, wc.hInstance, nullptr);
    SetLayeredWindowAttributes(hwnd, 0, 255, LWA_ALPHA);
    const MARGINS margin = {-1, 0, 0, 0};
    DwmExtendFrameIntoClientArea(hwnd, &margin);
    if (!CreateDeviceD3D(hwnd)) {
        CleanupDeviceD3D();
        ::UnregisterClass(wc.lpszClassName, wc.hInstance);
        return;
    }
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui::StyleColorsDark();
    const HMONITOR monitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
    MONITORINFO info = {};
    info.cbSize = sizeof(MONITORINFO);
    GetMonitorInfo(monitor, &info);
    const int monitor_height = info.rcMonitor.bottom - info.rcMonitor.top;
    if (monitor_height > 1080) {
        const float fScale = 2.0f;
        ImFontConfig cfg;
        cfg.SizePixels = 13 * fScale;
        ImGui::GetIO().Fonts->AddFontDefault(&cfg);
    }
    ImGui::GetIO().IniFilename = nullptr;
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(pd3dDevice, pd3dDeviceContext);
    const ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    bInit = true;
    bool bDone = false;
    while (!bDone) {
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                bDone = true;
        }
        if (GetAsyncKeyState(VK_END) & 1)
            bDone = true;
        if (!IsWindowAlive() && bTargetSet)
            bDone = true;
        if (bDone)
            break;
        #ifdef _WINDLL 
        if (hTargetWindow != nullptr)
            MoveWindow(hwnd);
        else
            continue;
        #else
        if (hTargetWindow != nullptr && bTargetSet)
            MoveWindow(hwnd);
        #endif
        if (!IsWindowFocus(hwnd) && bTargetSet) {
            const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
            pd3dDeviceContext->OMSetRenderTargets(1, &pMainRenderTargetView, nullptr);
            pd3dDeviceContext->ClearRenderTargetView(pMainRenderTargetView, clear_color_with_alpha);
            pSwapChain->Present(1, 0);
            continue;
        }
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame(); {
            Drawing::Draw();
        }
        ImGui::EndFrame();
        if (Drawing::isActive())
            SetWindowLong(hwnd, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TOOLWINDOW);
        else
            SetWindowLong(hwnd, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOOLWINDOW);
        ImGui::Render();
        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        pd3dDeviceContext->OMSetRenderTargets(1, &pMainRenderTargetView, nullptr);
        pd3dDeviceContext->ClearRenderTargetView(pMainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        pSwapChain->Present(1, 0);
    }
    bInit = false;
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClass(wc.lpszClassName, wc.hInstance);

#ifdef _WINDLL
    CreateThread(nullptr, NULL, (LPTHREAD_START_ROUTINE)FreeLibrary, hCurrentModule, NULL, nullptr);
#else
    TerminateProcess(GetModuleHandleA(nullptr), 0);
#endif
}
void UI::CloseProcess() {
    TerminateProcess(GetModuleHandleA(nullptr), 0);
    std::exit(0);
}
void UI::GetWindow() {
    EnumWindows(EnumWind, NULL);
}
BOOL CALLBACK UI::EnumWind(const HWND hWindow, const LPARAM lPrams) {
    DWORD procID;
    GetWindowThreadProcessId(hWindow, &procID);
    if (GetCurrentProcessId() != procID)
        return TRUE;
    if (!IsWindowValid(hWindow))
        return TRUE;
    SetTargetWindow(hWindow);
    return FALSE;
}
void UI::MoveWindow(const HWND hCurrentProcessWindow) {
    RECT rect;
    if (hTargetWindow == nullptr)
        return;
    GetWindowRect(hTargetWindow, &rect);
    int lWindowWidth = rect.right - rect.left;
    int lWindowHeight = rect.bottom - rect.top;
    lWindowWidth -= 5;
    lWindowHeight -= 29;
    SetWindowPos(hCurrentProcessWindow, nullptr, rect.left, rect.top, lWindowWidth, lWindowHeight, SWP_SHOWWINDOW);
}
BOOL UI::IsWindowFocus(const HWND hCurrentProcessWindow) {
    char lpCurrentWindowUsedClass[125];
    char lpCurrentWindowClass[125];
    char lpOverlayWindowClass[125];
    const HWND hCurrentWindowUsed = GetForegroundWindow();
    if (GetClassNameA(hCurrentWindowUsed, lpCurrentWindowUsedClass, sizeof(lpCurrentWindowUsedClass)) == 0)
        return FALSE;
    if (GetClassNameA(hTargetWindow, lpCurrentWindowClass, sizeof(lpCurrentWindowClass)) == 0)
        return FALSE;
    if (GetClassNameA(hCurrentProcessWindow, lpOverlayWindowClass, sizeof(lpOverlayWindowClass)) == 0)
        return FALSE;
    if (strcmp(lpCurrentWindowUsedClass, lpCurrentWindowClass) != 0 && strcmp(lpCurrentWindowUsedClass, lpOverlayWindowClass) != 0) {
        SetWindowLong(hCurrentProcessWindow, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOOLWINDOW);
        return FALSE;
    }
    return TRUE;
}
BOOL UI::IsWindowTargeted() {
    return bTargetSet;
}
void UI::GetAllWindow(std::vector<WindowItem>* vWindowList) {
    vWindowList->clear();
    EnumWindows(EnumAllWind, (LPARAM)vWindowList);
}
BOOL CALLBACK UI::EnumAllWind(const HWND hWindow, const LPARAM lPrams) {
    if (!IsWindowValid(hWindow))
        return TRUE;
    WindowItem CurrentWindowItem = { hWindow, 0, 0 };
    DWORD procID;
    GetWindowTextA(hWindow, CurrentWindowItem.CurrentWindowTitle, sizeof(CurrentWindowItem.CurrentWindowTitle));
    if (strlen(CurrentWindowItem.CurrentWindowTitle) == 0)
        return TRUE;
    GetWindowThreadProcessId(hWindow, &procID);
    GetProcessName(CurrentWindowItem.CurrentProcessName, procID);
    const auto vWindowList = (std::vector<WindowItem>*)lPrams;
    vWindowList->push_back(CurrentWindowItem);
    return TRUE;
}
void UI::GetProcessName(const LPSTR lpProcessName, const DWORD dPID) {
    char lpCurrentProcessName[125];
    PROCESSENTRY32 ProcList{};
    ProcList.dwSize = sizeof(ProcList);
    const HANDLE hProcList = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcList == INVALID_HANDLE_VALUE)
        return;
    if (!Process32First(hProcList, &ProcList))
        return;
    if (ProcList.th32ProcessID == dPID) {
        wcstombs_s(nullptr, lpCurrentProcessName, ProcList.szExeFile, sizeof(lpCurrentProcessName));
        strncpy_s(lpProcessName, sizeof(lpCurrentProcessName), lpCurrentProcessName, sizeof(lpCurrentProcessName));
        return;
    }
    while (Process32Next(hProcList, &ProcList)) {
        if (ProcList.th32ProcessID == dPID) {
            wcstombs_s(nullptr, lpCurrentProcessName, ProcList.szExeFile, sizeof(lpCurrentProcessName));
            strncpy_s(lpProcessName, sizeof(lpCurrentProcessName), lpCurrentProcessName, sizeof(lpCurrentProcessName));
            return;
        }
    }
}
BOOL UI::IsWindowValid(const HWND hCurrentWindow) {
    DWORD styles, ex_styles;
    RECT rect;
    if (!IsWindowVisible(hCurrentWindow) ||
        (IsIconic(hCurrentWindow) || IsWindowCloaked(hCurrentWindow)))
        return FALSE;
    GetClientRect(hCurrentWindow, &rect);
    styles = (DWORD)GetWindowLongPtr(hCurrentWindow, GWL_STYLE);
    ex_styles = (DWORD)GetWindowLongPtr(hCurrentWindow, GWL_EXSTYLE);
    if (ex_styles & WS_EX_TOOLWINDOW)
        return FALSE;
    if (styles & WS_CHILD)
        return FALSE;
    if (rect.bottom == 0 || rect.right == 0)
        return FALSE;
    return TRUE;
}
BOOL UI::IsWindowCloaked(const HWND hCurrentWindow) {
    DWORD cloaked;
    const HRESULT hr = DwmGetWindowAttribute(hCurrentWindow, DWMWA_CLOAKED, &cloaked,
        sizeof(cloaked));
    return SUCCEEDED(hr) && cloaked;
}
void UI::SetTargetWindow(const HWND hWindow) {
    hTargetWindow = hWindow;
    SetForegroundWindow(hTargetWindow);
    GetWindowThreadProcessId(hTargetWindow, &dTargetPID);
    bTargetSet = TRUE;
}
BOOL UI::IsWindowAlive() {
    DWORD dCurrentPID;
    if (hTargetWindow == nullptr)
        return FALSE;
    if (!IsWindow(hTargetWindow))
        return FALSE;
    GetWindowThreadProcessId(hTargetWindow, &dCurrentPID);
    if (dCurrentPID != dTargetPID)
        return FALSE;
    return TRUE;
}