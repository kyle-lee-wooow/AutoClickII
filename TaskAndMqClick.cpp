// TaskAndMqClick.cpp : 定义应用程序的入口点。

#include "framework.h"
#include "TaskAndMqClick.h"
#include <windows.h>
#include <string>
#include <commctrl.h>
#include <thread>
#include <vector>
#include <string> 

#include <sstream>
#include <iostream>

#pragma comment(lib, "comctl32.lib")

#define MAX_LOADSTRING 100
#define ID_LISTBOX 101
#define HOTKEY_ID 102  

#define CHECKBOX_BASE_ID 200  
#define EDIT_B_BASE_ID 300    
#define EDIT_C_BASE_ID 400    



#define TIMER_ID 1
#define INTERVAL_MS 500  // 定时器触发间隔时间

HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];
HWND hListBox;
HWND hCheckBoxes[10];
HWND hEditB[10];
HWND hEditC[10];


// 用来存储每一行的下次执行时间
std::chrono::steady_clock::time_point nextExecutionTimes[10];

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//单个案件
void sendKeyToListBoxs(char key);
void sendKeyToListBoxsDown(char key);
void sendKeyToListBoxsUp(char key);


//多个案件
void sendMultipleKeysToListBoxs(const std::wstring& keyCombo);
std::vector<BYTE> ParseKeyCombo(const std::wstring& keyCombo);


//多按键发送
std::vector<std::vector<BYTE>> ParseMultipleKeyCombos(const std::wstring& keyCombos);

INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TASKANDMQCLICK, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TASKANDMQCLICK));
    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }


    return (int)msg.wParam;
}

bool isTaskRunning = false;  // 任务是否正在执行
std::thread taskThread;      // 任务线程

// 启动任务A的函数
void StartTaskA(HWND hWnd) {
    SetFocus(hWnd);
    isTaskRunning = true;
}

// 停止任务A的函数
void StopTaskA(HWND hWnd) {
    SetFocus(hWnd);
    isTaskRunning = false;

}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TASKANDMQCLICK));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_TASKANDMQCLICK);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;

    //HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        //CW_USEDEFAULT, 0, 600, 400, nullptr, nullptr, hInstance, nullptr);

    HWND hWnd = CreateWindowW(szWindowClass, szTitle,
        WS_OVERLAPPEDWINDOW | WS_TABSTOP,  // 确保支持键盘输入
        CW_USEDEFAULT, CW_USEDEFAULT, 700, 500,
        nullptr, nullptr, hInstance, nullptr);


    if (!hWnd)
    {
        return FALSE;
    }

    hListBox = CreateWindowW(L"LISTBOX", NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY | WS_BORDER,
        10, 10, 200, 350, hWnd, (HMENU)ID_LISTBOX, hInstance, NULL);



    // 设置定时器，定时触发WM_TIMER消息
    SetTimer(hWnd, TIMER_ID, INTERVAL_MS, NULL);


    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);



    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        // 在窗口右侧增加10行，每行包含：
     // - 勾选框 A
     // - 文字 "每隔"
     // - 输入框 B（只能输入数字，单位：秒）
     // - 文字 "秒执行按键"
     // - 输入框 C（输入键盘按键）

    case WM_CREATE:
    {
        RegisterHotKey(hWnd, HOTKEY_ID, MOD_CONTROL | MOD_ALT, 'A');

        int startX = 250;
        int startY = 10;
        int rowHeight = 30;
        int checkBoxWidth = 20;
        int textWidth = 50;
        int editWidth = 80;
        int gap = 10;

        for (int i = 0; i < 10; i++)
        {
            hCheckBoxes[i] = CreateWindowW(L"BUTTON", L"", WS_VISIBLE | WS_CHILD | BS_CHECKBOX,
                startX, startY + i * rowHeight, checkBoxWidth, 20, hWnd, (HMENU)(CHECKBOX_BASE_ID + i), hInst, NULL);

            CreateWindowW(L"STATIC", L"每隔", WS_VISIBLE | WS_CHILD,
                startX + checkBoxWidth + gap, startY + i * rowHeight, textWidth, 20, hWnd, NULL, hInst, NULL);

            hEditB[i] = CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER,
                startX + checkBoxWidth + gap + textWidth, startY + i * rowHeight, editWidth, 20, hWnd, (HMENU)(EDIT_B_BASE_ID + i), hInst, NULL);

            CreateWindowW(L"STATIC", L"秒执行", WS_VISIBLE | WS_CHILD,
                startX + checkBoxWidth + gap + textWidth + editWidth + gap, startY + i * rowHeight, textWidth * 2, 20, hWnd, NULL, hInst, NULL);

            hEditC[i] = CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
                startX + checkBoxWidth + gap + textWidth + editWidth + gap + textWidth * 2, startY + i * rowHeight, editWidth, 20, hWnd, (HMENU)(EDIT_C_BASE_ID + i), hInst, NULL);
        }

        // 设置开箱子
        SetWindowTextW(hEditB[7], L"60");
        // 设置开箱子
        SetWindowTextW(hEditC[7], L"Shift+8");
        // 设置自动打怪
        SetWindowTextW(hEditB[0], L"1");
        // 设置自动打怪
        SetWindowTextW(hEditC[0], L"0");



        // 创建启动按钮
        CreateWindowW(L"BUTTON", L"Start", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            500, 320, 80, 30, hWnd, (HMENU)1001, hInst, NULL);

        // 创建停止按钮
        CreateWindowW(L"BUTTON", L"Stop", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            500, 360, 80, 30, hWnd, (HMENU)1002, hInst, NULL);

        RegisterHotKey(hWnd, HOTKEY_ID, MOD_CONTROL | MOD_ALT, 'A');

    }
    break;



    case WM_KEYDOWN:
    {
        sendKeyToListBoxs(wParam);
    }
    break;

    case WM_HOTKEY:
        if (wParam == HOTKEY_ID)
        {
            HWND hForegroundWnd = GetForegroundWindow();
            if (hForegroundWnd)
            {
                DWORD processId;
                GetWindowThreadProcessId(hForegroundWnd, &processId);

                WCHAR buffer[100];
                swprintf_s(buffer, L"Window ID: 0x%08X, PID: %u", (UINT)hForegroundWnd, processId);
                SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)buffer);
            }
        }
        break;

    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        if (wmId >= CHECKBOX_BASE_ID && wmId < CHECKBOX_BASE_ID + 10)
        {
            LRESULT state = SendMessage((HWND)lParam, BM_GETCHECK, 0, 0);
            SendMessage((HWND)lParam, BM_SETCHECK, state == BST_CHECKED ? BST_UNCHECKED : BST_CHECKED, 0);
        }

        //if (wmId >= EDIT_C_BASE_ID && wmId < EDIT_C_BASE_ID + 10)
        //{
        //    WCHAR keyText[100]; // 增加缓冲区支持更长的按键输入
        //    SendMessage((HWND)lParam, WM_GETTEXT, 100, (LPARAM)keyText);

        //    std::wstring keyCombo(keyText); // 获取输入框中的按键组合
        //    sendKeyToListBoxsWs(keyCombo); // 发送多按键组合
        //}
       



        switch (wmId)
        {
        case 1001: // 启动按钮点击
            StartTaskA(hWnd);
            break;

        case 1002: // 停止按钮点击
            StopTaskA(hWnd);
            break;


        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;

    //case WM_TIMER:
    //    if (wParam == TIMER_ID && isTaskRunning)
    //    {
    //        SYSTEMTIME systemTime;
    //        GetSystemTime(&systemTime);
    //        auto currentTime = std::chrono::steady_clock::now();

    //        // 检查每一行控件是否应该执行任务
    //        for (int i = 0; i < 10; i++)
    //        {
    //            // 检查当前时间是否已经超过该行的下次执行时间
    //            if (currentTime >= nextExecutionTimes[i])
    //            {
    //                // 获取勾选框状态
    //                LRESULT state = SendMessage(hCheckBoxes[i], BM_GETCHECK, 0, 0);

    //                if (state == BST_CHECKED)
    //                {
    //                    // 获取间隔时间（秒）
    //                    WCHAR intervalText[10];
    //                    SendMessage(hEditB[i], WM_GETTEXT, 10, (LPARAM)intervalText);
    //                    int interval = _wtoi(intervalText);  // 获取秒数

    //                    // 获取按键字符
    //                    WCHAR keyText[10];
    //                    SendMessage(hEditC[i], WM_GETTEXT, 10, (LPARAM)keyText);
    //                    char key = keyText[0];

    //                    std::wstring keyStr(keyText);

    //                    // 如果是空格键，使用 VK_SPACE
    //                    if (keyStr == L"Space")
    //                    {
    //                        key = VK_SPACE;
    //                    }
    //                    // 如果是回车键，使用 VK_RETURN
    //                    else if (keyStr == L"Enter")
    //                    {
    //                        key = VK_RETURN;
    //                    }
    //                    // 如果是 Tab 键，使用 VK_TAB
    //                    else if (keyStr == L"Tab")
    //                    {
    //                        key = VK_TAB;
    //                    }
    //                    // 如果是其他键，处理为字符
    //                    else
    //                    {
    //                        key = keyStr[0];
    //                    }


    //                    sendKeyToListBoxs(key);

    //                    // 更新下次执行时间
    //                    nextExecutionTimes[i] = currentTime + std::chrono::seconds(interval);
    //                }
    //            }
    //        }
    //    }
    //    break;


    case WM_TIMER:
        if (wParam == TIMER_ID && isTaskRunning)
        {
            auto currentTime = std::chrono::steady_clock::now();

            for (int i = 0; i < 10; i++)
            {
                if (currentTime >= nextExecutionTimes[i])
                {
                    LRESULT state = SendMessage(hCheckBoxes[i], BM_GETCHECK, 0, 0);
                    if (state == BST_CHECKED)
                    {
                        WCHAR intervalText[10];
                        SendMessage(hEditB[i], WM_GETTEXT, 10, (LPARAM)intervalText);
                        int interval = _wtoi(intervalText);

                        WCHAR keyText[100];
                        SendMessage(hEditC[i], WM_GETTEXT, 100, (LPARAM)keyText);
                        std::wstring keyStr(keyText);

                        sendMultipleKeysToListBoxs(keyStr); // 发送多按键

                        nextExecutionTimes[i] = currentTime + std::chrono::seconds(interval);
                    }
                }
            }
        }
        break;


    case WM_DESTROY:
        UnregisterHotKey(hWnd, HOTKEY_ID);
        PostQuitMessage(0);
        break;


    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void sendMultipleKeysToListBoxs(const std::wstring& keyCombos)
{
    std::vector<std::vector<BYTE>> allKeyCombos = ParseMultipleKeyCombos(keyCombos);

    for (const auto& keyCombo : allKeyCombos) // 逐个执行每个按键组合
    {
        if (!keyCombo.empty())
        {
            //std::vector<INPUT> inputs;


            //  // 按下所有按键
            //for (BYTE key : keyCombo) {
            //    INPUT input = { 0 };
            //    input.type = INPUT_KEYBOARD;
            //    input.ki.wVk = key;
            //    inputs.push_back(input);
            //   
            //}
            // SendInput(static_cast<UINT>(inputs.size()), inputs.data(), sizeof(INPUT));

            //// 清空 inputs 以存储松开按键的事件
            //inputs.clear();


            //// **逆序** 松开所有按键（后按下的先释放）
            //for (auto it = keyCombo.rbegin(); it != keyCombo.rend(); ++it) {
            //    INPUT input = { 0 };
            //    input.type = INPUT_KEYBOARD;
            //    input.ki.wVk = *it;
            //    input.ki.dwFlags = KEYEVENTF_KEYUP;
            //    inputs.push_back(input);
            //}
            //SendInput(static_cast<UINT>(inputs.size()), inputs.data(), sizeof(INPUT));





               // 按下所有按键
            for (BYTE key : keyCombo) {
                sendKeyToListBoxsDown(key);

            } 

    

            // **逆序** 松开所有按键（后按下的先释放）
            for (auto it = keyCombo.rbegin(); it != keyCombo.rend(); ++it) {
                sendKeyToListBoxsUp(*it);
            }
            



            std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 短暂延迟防止冲突
        }
    }
}

// 解析多个按键组合
std::vector<std::vector<BYTE>> ParseMultipleKeyCombos(const std::wstring& keyCombos)
{
    std::vector<std::vector<BYTE>> allKeyCombos;
    size_t startPos = 0, endPos = 0;

    while ((endPos = keyCombos.find(L",", startPos)) != std::wstring::npos)
    {
        std::wstring singleCombo = keyCombos.substr(startPos, endPos - startPos);
        allKeyCombos.push_back(ParseKeyCombo(singleCombo)); // 解析每个组合
        startPos = endPos + 1;
    }

    allKeyCombos.push_back(ParseKeyCombo(keyCombos.substr(startPos))); // 解析最后一个
    return allKeyCombos;
}



std::vector<BYTE> ParseKeyCombo(const std::wstring& keyCombo)
{
    std::vector<BYTE> keys;
    std::wistringstream stream(keyCombo);
    std::wstring key;

    while (std::getline(stream, key, L'+')) // 以 '+' 拆分组合键
    {
        if (_wcsicmp(key.c_str(), L"Ctrl") == 0) keys.push_back(VK_CONTROL);
        else if (_wcsicmp(key.c_str(), L"Alt") == 0) keys.push_back(VK_MENU);
        else if (_wcsicmp(key.c_str(), L"Space") == 0) keys.push_back(VK_SPACE);
        else if (_wcsicmp(key.c_str(), L"Shift") == 0) keys.push_back(VK_SHIFT);
        else if (_wcsicmp(key.c_str(), L"Tab") == 0) keys.push_back(VK_TAB);
        else if (_wcsicmp(key.c_str(), L"Enter") == 0) keys.push_back(VK_RETURN);
        else if (_wcsicmp(key.c_str(), L"Esc") == 0) keys.push_back(VK_ESCAPE);
        else if (key.length() == 1) keys.push_back(VkKeyScan(key[0])); // 普通字符
    }

    return keys;
}


void sendKeyToListBoxsDown(char key)
{
    // 执行按键发送
    if (key != '\0')
    {
        int count = SendMessage(hListBox, LB_GETCOUNT, 0, 0);
        for (int j = 0; j < count; j++)
        {
            WCHAR buffer[100];
            SendMessage(hListBox, LB_GETTEXT, j, (LPARAM)buffer);

            UINT windowID;
            if (swscanf_s(buffer, L"Window ID: 0x%08X", &windowID) == 1)
            {
                HWND targetWindow = (HWND)windowID;
                if (IsWindow(targetWindow))
                {
                    PostMessage(targetWindow, WM_KEYDOWN, (WPARAM)key, 0);
                    //PostMessage(targetWindow, WM_KEYUP, (WPARAM)key, 0);
                }
            }
        }
    }
}


void sendKeyToListBoxsUp(char key)
{
    // 执行按键发送
    if (key != '\0')
    {
        int count = SendMessage(hListBox, LB_GETCOUNT, 0, 0);
        for (int j = 0; j < count; j++)
        {
            WCHAR buffer[100];
            SendMessage(hListBox, LB_GETTEXT, j, (LPARAM)buffer);

            UINT windowID;
            if (swscanf_s(buffer, L"Window ID: 0x%08X", &windowID) == 1)
            {
                HWND targetWindow = (HWND)windowID;
                if (IsWindow(targetWindow))
                {
                    //PostMessage(targetWindow, WM_KEYDOWN, (WPARAM)key, 0);
                    PostMessage(targetWindow, WM_KEYUP, (WPARAM)key, 0);
                }
            }
        }
    }
}

void sendKeyToListBoxs(char key)
{
    // 执行按键发送
    if (key != '\0')
    {
        int count = SendMessage(hListBox, LB_GETCOUNT, 0, 0);
        for (int j = 0; j < count; j++)
        {
            WCHAR buffer[100];
            SendMessage(hListBox, LB_GETTEXT, j, (LPARAM)buffer);

            UINT windowID;
            if (swscanf_s(buffer, L"Window ID: 0x%08X", &windowID) == 1)
            {
                HWND targetWindow = (HWND)windowID;
                if (IsWindow(targetWindow))
                {
                    PostMessage(targetWindow, WM_KEYDOWN, (WPARAM)key, 0);
                    PostMessage(targetWindow, WM_KEYUP, (WPARAM)key, 0);
                }
            }
        }
    }
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
