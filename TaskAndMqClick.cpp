// TaskAndMqClick.cpp : 定义应用程序的入口点。

#include "framework.h"
#include "TaskAndMqClick.h"
#include <windows.h>
#include <string>
#include <commctrl.h>
#include <thread>
#include <vector>
#include <string> 
#include <commdlg.h>  // 用于文件选择对话框
#include <sstream>
#include <iostream>
#include <locale>  // 必须包含这个头文件

#pragma comment(lib, "comctl32.lib")

#define MAX_LOADSTRING 100
#define ID_LISTBOX 101
#define HOTKEY_ID 102  

#define CHECKBOX_BASE_ID 200  
#define EDIT_B_BASE_ID 300    
#define EDIT_C_BASE_ID 400    



#define TIMER_ID 1
#define INTERVAL_MS 500  // 定时器触发间隔时间

// 现在可以同时执行多少个按键
#define L_TASK_COUNTS 10

HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];
HWND hListBox;
HWND TaskStartBtn;
HWND TaskStopBtn;
HWND hCheckBoxes[L_TASK_COUNTS];
HWND hEditB[L_TASK_COUNTS];
HWND hEditC[L_TASK_COUNTS];


// 用来存储每一行的下次执行时间
std::chrono::steady_clock::time_point nextExecutionTimes[L_TASK_COUNTS];

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//单个案件
void sendKeyToListBoxs(int key);
void sendKeyToListBoxsDown(int key);
void sendKeyToListBoxsUp(int key);

HBRUSH hGreenBrush, hRedBrush; // 按钮背景色画刷


//多个案件
void sendMultipleKeysToListBoxs(const std::wstring& keyCombo);
std::vector<BYTE> ParseKeyCombo(const std::wstring& keyCombo);


//多按键发送
std::vector<std::vector<BYTE>> ParseMultipleKeyCombos(const std::wstring& keyCombos);

INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

//批量登录 
INT_PTR CALLBACK BATCH_LOGINS(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);


//选择本地文件
void SelectFile(HWND hEdit);

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
    if(isTaskRunning == false){
       /* ShowWindow(TaskStopBtn, SW_SHOW);
        ShowWindow(TaskStartBtn, SW_HIDE);*/
        isTaskRunning = true;

        // 强制重绘窗口，触发WM_PAINT消息
        InvalidateRect(hWnd, NULL, TRUE);
        UpdateWindow(hWnd);
    }
}

// 停止任务A的函数
void StopTaskA(HWND hWnd) {
    SetFocus(hWnd);
    if (isTaskRunning == true) {
        isTaskRunning = false;
        /*ShowWindow(TaskStopBtn, SW_HIDE);
        ShowWindow(TaskStartBtn, SW_SHOW);*/
        std::fill(std::begin(nextExecutionTimes), std::end(nextExecutionTimes), std::chrono::steady_clock::time_point{});

        // 强制重绘窗口，触发WM_PAINT消息
        InvalidateRect(hWnd, NULL, TRUE);
        UpdateWindow(hWnd);
    }
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

        for (int i = 0; i < L_TASK_COUNTS; i++)
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
        SetWindowTextW(hEditB[7], L"30");
        // 设置开箱子
        SetWindowTextW(hEditC[7], L"Shift+8");

        // 设置自动打怪
        SetWindowTextW(hEditB[0], L"1");
        // 设置自动打怪
        SetWindowTextW(hEditC[0], L"0");


        // 设置自动摧毁
        SetWindowTextW(hEditB[6], L"600");
        // 设置自动摧毁
        SetWindowTextW(hEditC[6], L"7");



        // 创建启动按钮
        TaskStartBtn = CreateWindowW(L"BUTTON", L"启动", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            400, 320, 80, 30, hWnd, (HMENU)1001, hInst, NULL);

        // 创建停止按钮
        TaskStopBtn = CreateWindowW(L"BUTTON", L"停止", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            500, 320, 80, 30, hWnd, (HMENU)1002, hInst, NULL);

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

    case WM_LBUTTONDOWN:  // 处理鼠标左键点击事件
    {
        //int x = LOWORD(lParam);  // 获取鼠标 X 坐标
        //int y = HIWORD(lParam);  // 获取鼠标 Y 坐标
        //wchar_t msg[50];
        //swprintf_s(msg, L"鼠标点击坐标: %d, %d", x, y);
        //MessageBox(hWnd, msg, L"鼠标点击", MB_OK);
        SetFocus(hWnd);
        break;
    }


    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        if (wmId >= CHECKBOX_BASE_ID && wmId < CHECKBOX_BASE_ID + L_TASK_COUNTS)
        {
            LRESULT state = SendMessage((HWND)lParam, BM_GETCHECK, 0, 0);
            SendMessage((HWND)lParam, BM_SETCHECK, state == BST_CHECKED ? BST_UNCHECKED : BST_CHECKED, 0);
        }



        switch (wmId)
        {
        case 1001: // 启动按钮点击
            StartTaskA(hWnd);
            break;

        case 1002: // 停止按钮点击
            StopTaskA(hWnd);
            break;

        case ID_LISTBOX:
            if(HIWORD(wParam) == LBN_DBLCLK)
            {
                // 获取选中的索引
                int index = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
                if (index != LB_ERR)
                {
                    // 删除选中的项目
                    SendMessage(hListBox, LB_DELETESTRING, (WPARAM)index, 0);
                }
            }
            break;
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_LOGINS:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_LOGINSBOX), hWnd, BATCH_LOGINS);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;

    case WM_TIMER:
        if (wParam == TIMER_ID && isTaskRunning)
        {
            auto currentTime = std::chrono::steady_clock::now();

            for (int i = 0; i < L_TASK_COUNTS; i++)
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

    case WM_PAINT:
       
        HDC hdc;
        PAINTSTRUCT ps;
        RECT rect;

        // 获取设备上下文
        hdc = BeginPaint(hWnd, &ps);

        // 获取窗口的大小
        GetClientRect(hWnd, &rect);


        // 根据isrun的值绘制圆圈
        if (isTaskRunning)
        {
            // 绘制绿色圆圈
            HBRUSH greenBrush = CreateSolidBrush(RGB(0, 255, 0)); // 绿色
            SelectObject(hdc, greenBrush);
            Ellipse(hdc, 340, 325, 360, 345); // 绘制圆形
            DeleteObject(greenBrush);
        }
        else
        {
            // 绘制红色圆圈
            HBRUSH redBrush = CreateSolidBrush(RGB(255, 0, 0)); // 红色
            SelectObject(hdc, redBrush);
            Ellipse(hdc, 340, 325, 360, 345); // 绘制圆形
            DeleteObject(redBrush);
        }

        // 结束绘制
        EndPaint(hWnd, &ps);
        return 0;

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
        else if (_wcsicmp(key.c_str(), L"-") == 0) keys.push_back(VK_OEM_MINUS);
        else if (_wcsicmp(key.c_str(), L"+") == 0) keys.push_back(VK_OEM_PLUS);
        else if (_wcsicmp(key.c_str(), L"F1") == 0) keys.push_back(VK_F1);
        else if (_wcsicmp(key.c_str(), L"F2") == 0) keys.push_back(VK_F2);
        else if (_wcsicmp(key.c_str(), L"F3") == 0) keys.push_back(VK_F3);
        else if (_wcsicmp(key.c_str(), L"F4") == 0) keys.push_back(VK_F4);
        else if (_wcsicmp(key.c_str(), L"F5") == 0) keys.push_back(VK_F5);
        else if (_wcsicmp(key.c_str(), L"F6") == 0) keys.push_back(VK_F6);
        else if (_wcsicmp(key.c_str(), L"F7") == 0) keys.push_back(VK_F7);
        else if (_wcsicmp(key.c_str(), L"F8") == 0) keys.push_back(VK_F8);
        else if (_wcsicmp(key.c_str(), L"F9") == 0) keys.push_back(VK_F9);
        else if (_wcsicmp(key.c_str(), L"F10") == 0) keys.push_back(VK_F10);
        else if (_wcsicmp(key.c_str(), L"F11") == 0) keys.push_back(VK_F11);
        else if (_wcsicmp(key.c_str(), L"F12") == 0) keys.push_back(VK_F12);
        else if (key.length() == 1) keys.push_back(VkKeyScan(key[0])); // 普通字符
    }

    return keys;
}


void sendKeyToListBoxsDown(int key)
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
                }
            }
        }
    }
}


void sendKeyToListBoxsUp(int key)
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
                   
                    PostMessage(targetWindow, WM_KEYUP, (WPARAM)key, 0);
                }
            }
        }
    }
}

void sendKeyToListBoxs(int key)
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


INT_PTR CALLBACK BATCH_LOGINS(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    //batchGameLogin::logins_file("WoW.exe", L"魔兽世界", "accounts.txt");

    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:

        switch (LOWORD(wParam)) {
            case IDCANCEL:
                EndDialog(hDlg, LOWORD(wParam));
                return (INT_PTR)TRUE;

            case IDC_LOGIN_EXE_EDIT1:
            case IDC_LOGIN_ACCOUNTS_EDIT2: {
                // 当输入框内容更新时，检查是否包含空格，若包含则弹出文件选择器
                if (HIWORD(wParam) == EN_UPDATE) {
                    std::wstring text = GetEditText(hDlg, LOWORD(wParam));
                    if (wcschr(text.c_str(), L' ')) {  // wcschr 查找第一个空格
                        SelectFile(GetDlgItem(hDlg, LOWORD(wParam)));
                    }
                }
                break;
            }

            case ID_LOGINS_OK: {
                // 获取输入框内容
                std::wstring exeText = GetEditText(hDlg, IDC_LOGIN_EXE_EDIT1);
                std::wstring acctText = GetEditText(hDlg, IDC_LOGIN_ACCOUNTS_EDIT2);

                // 转换为 std::string
                std::string exeAddr = WcharToString(exeText.c_str());
                std::string acctAddr = WcharToString(acctText.c_str());

                if (exeAddr.empty()) {
                    exeAddr = "WoW.exe";
                }


                if (acctAddr.empty()) {
                    acctAddr = "accounts.txt";
                }


                if (!exeAddr.empty() && !acctAddr.empty()) {
                    //batchGameLogin::logins_file(exeAddr, L"", acctAddr);
                    std::vector<batchGameLogin::ProcessInfo> processList= batchGameLogin::logins_file_and_splite_screen(exeAddr, L"", acctAddr);

                    for (auto& proc : processList) {

                        WCHAR buffer[100];
                        swprintf_s(buffer, L"Window ID: 0x%08X, PID: %u", (UINT)proc.hwnd, proc.pi.dwProcessId);
                        SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)buffer);
                    }


                    processList.clear();
                }
                EndDialog(hDlg, LOWORD(wParam));
                return (INT_PTR)TRUE;
            }

            default:
                break;
        }
    }
    return (INT_PTR)FALSE;
}

// 选择文件并填入输入框
void SelectFile(HWND hEdit)
{
    OPENFILENAME ofn;
    wchar_t szFile[MAX_PATH] = { 0 };

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hEdit;
    ofn.lpstrFilter = L"All Files\0*.*\0Text Files\0*.TXT\0";
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    ofn.lpstrTitle = L"选择文件";

    if (GetOpenFileName(&ofn))
    {
        SetWindowText(hEdit, szFile);  // 将文件路径填充到输入框
    }
}