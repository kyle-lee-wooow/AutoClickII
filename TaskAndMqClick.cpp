#include "TaskAndMqClick.h"


// UTF-8 转换函数
std::string WstringToUtf8(const std::wstring& wstr) {
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), static_cast<int>(wstr.size()), nullptr, 0, nullptr, nullptr);
    if (size_needed == 0) {
        return "";
    }

    std::string utf8_str(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), static_cast<int>(wstr.size()), &utf8_str[0], size_needed, nullptr, nullptr);

    return utf8_str;
}

std::wstring stringToWstring(const std::string& str) {
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
    std::wstring wstr(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstr[0], size_needed);
    return wstr;
}

std::string WcharToString(const wchar_t* wstr)
{
    // 获取转换后的字符数
    int len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);

    // 创建一个足够大的 buffer 来存放转换后的字符串
    char* str = new char[len];

    // 执行转换
    WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);

    // 返回转换后的 std::string
    std::string result(str);

    delete[] str;  // 记得释放内存
    return result;
}

//禁用菜单
void SetRemoteControlMenuState(HWND hWnd, bool enable) {
    HMENU hMenu = GetMenu(hWnd); // 获取菜单句柄
    if (hMenu) {
        UINT state = enable ? MF_ENABLED : MF_GRAYED;
        EnableMenuItem(hMenu, ID_REMOTE_CTRL_RUN, MF_BYCOMMAND | state);
        EnableMenuItem(hMenu, ID_REMOTE_CTRL_STOP, MF_BYCOMMAND | state);
        DrawMenuBar(hWnd); // 重新绘制菜单
    }
}


// 获取控件文本的函数
std::wstring GetEditText(HWND hDlg, int controlID) {
    HWND hEdit = GetDlgItem(hDlg, controlID);
    if (!hEdit) return L"";

    int length = GetWindowTextLength(hEdit);
    if (length == 0) return L"";

    std::vector<wchar_t> buffer(length + 1);  // 确保足够的空间
    GetWindowText(hEdit, buffer.data(), length + 1);

    return std::wstring(buffer.data());
}

// 设置控件文本的函数
void SetEditText(HWND hDlg, int controlID, const std::wstring& text) {
    HWND hEdit = GetDlgItem(hDlg, controlID);
    if (hEdit) {
        SetWindowText(hEdit, text.c_str());
    }
}



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
    l_config = std::make_unique<CFG>("./TaskAndMqClick.ini");  // 智能指针初始化

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



// 启动任务A的函数
void StartTaskA(HWND hWnd) {
    SetFocus(hWnd);
    if(isTaskRunning == false){
       /* ShowWindow(TaskStopBtn, SW_SHOW);
        ShowWindow(TaskStartBtn, SW_HIDE);*/
        isTaskRunning = true;

        // 强制重绘窗口，触发WM_PAINT消息
        InvalidateRect(hWnd, NULL, TRUE);
        //UpdateWindow(hWnd);
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
        //UpdateWindow(hWnd);
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


int setTheme(HWND hWnd) {
    SetWindowTheme(hWnd, L"Explorer", NULL);
    //SetWindowTheme(hWnd, L"Windows", NULL);
    return 1;
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;

    //HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        //CW_USEDEFAULT, 0, 600, 400, nullptr, nullptr, hInstance, nullptr);

    mainHWnd = CreateWindowW(szWindowClass, szTitle,
        WS_OVERLAPPEDWINDOW | WS_TABSTOP,  // 确保支持键盘输入
        CW_USEDEFAULT, CW_USEDEFAULT, 700, 500,
        nullptr, nullptr, hInstance, nullptr);


    if (!mainHWnd)
    {
        return FALSE;
    }

    hListBox = CreateWindowW(L"LISTBOX", NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY | WS_BORDER,
        10, 10, 200, 350, mainHWnd, (HMENU)ID_LISTBOX, hInstance, NULL);



    // 设置定时器，定时触发WM_TIMER消息
    SetTimer(mainHWnd, TIMER_ID, INTERVAL_MS, NULL);


    ShowWindow(mainHWnd, nCmdShow);
    UpdateWindow(mainHWnd);

    //美化
    setTheme(mainHWnd);
    setTheme(hListBox);

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

            SetWindowTextW(hEditC[i], std::to_wstring(i).c_str());
            SetWindowTextW(hEditB[i], L"10");
            
            setTheme(hCheckBoxes[i]);
            setTheme(hEditB[i]);
            setTheme(hEditC[i]); 



        }

        // 设置开箱子
        SetWindowTextW(hEditB[8], L"30");
        // 设置开箱子
        SetWindowTextW(hEditC[8], L"Shift+8");

        // 设置自动打怪
        SetWindowTextW(hEditB[0], L"1"); 

        // 设置自动摧毁
        SetWindowTextW(hEditB[7], L"600");



        // 创建启动按钮
        TaskStartBtn = CreateWindowW(L"BUTTON", L"启动", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            400, 320, 80, 30, hWnd, (HMENU)1001, hInst, NULL);

        // 创建停止按钮
        TaskStopBtn = CreateWindowW(L"BUTTON", L"停止", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            500, 320, 80, 30, hWnd, (HMENU)1002, hInst, NULL);


        setTheme(TaskStartBtn);
        setTheme(TaskStopBtn); 
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
         
            HWND hwndSelf = FindWindow(NULL, L"逛街的熔火之心"); // 替换为你的窗口标题      // 获取当前程序的控制台窗口句柄

            HWND hForegroundWnd = GetForegroundWindow();

            if (hwndSelf == hForegroundWnd) {
                
                break; 
            
            }
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

        case ID_REMOTE_CTRL_RUN:{
            if (!l_config->fileExists()) { 
                
                SetRemoteControlMenuState(hWnd,false);
                break; }
            remoteCtrlisTaskRunning = true;
            std::string mqaddr = l_config->getString("mqtt", "SERVER_ADDRESS");
            std::string name = l_config->getString("mqtt", "MQ_NAME");
            std::string MQ_PWD = l_config->getString("mqtt", "MQ_PWD");

            RemoteCtrl::Conntection::getInstance().setMQTTParam(mqaddr, name, MQ_PWD);

            // 在新线程中启动 loading()
            std::thread([]() {
               
                RemoteCtrl::Conntection::getInstance().loading(withRemoteCtrlHandler);
                }).detach();  // 使用 detach 保证线程独立
            break;
        }
        case ID_REMOTE_CTRL_STOP:
            if (!l_config->fileExists()) { break; }
            remoteCtrlisTaskRunning = false;
            RemoteCtrl::Conntection::getInstance().stopMqtt();
            break;

            //重新排序窗口及清除无效ID
        case ID_WINDOW_RESIZE:
            gameResiceAndClearInva();
            break;

            //关闭游戏
        case ID_CLOSE_LISTBBOX_OK:
            closeListboxProcesses();
            break;
        case ID_CLOSE_LISTBBOX_NO:
           
            break;
           
            //隐藏窗口
        case ID_GAME_YINGDUN:
            SetWindowVisibilityEx(false);
            break;
            //取消隐藏
        case ID_GAME_ZHAOMINGSHU:
            SetWindowVisibilityEx(true);
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
        int key_l = key.length();
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
        else if (key.length() > 0) { 
            keys.push_back(VkKeyScan(key[0]));
        } // 普通字符
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




void sendClickToListBoxs(std::wstring key,bool leftClick)
{
    // 执行按键发送
    //if (key == L"CENTER")
    if (true)
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
                    RECT rect;
                    GetClientRect(targetWindow, &rect);  // 获取客户区坐标（left=0, top=0, right=width, bottom=height）

                    // 2. 计算中心点坐标
                    int x = rect.right / 2;
                    int y = rect.bottom / 2;

                    if (leftClick) {
                        SendMessage(targetWindow, WM_LBUTTONDOWN, 0, MAKELPARAM(x, y));
                        std::this_thread::sleep_for(std::chrono::milliseconds(50)); // 短暂延迟防止冲突
                        SendMessage(targetWindow, WM_LBUTTONUP, 0, MAKELPARAM(x, y));
                    }
                    else {
                        SendMessage(targetWindow, WM_RBUTTONDOWN, 0, MAKELPARAM(x, y));
                        std::this_thread::sleep_for(std::chrono::milliseconds(50)); // 短暂延迟防止冲突
                        SendMessage(targetWindow, WM_RBUTTONUP, 0, MAKELPARAM(x, y));
                    }
                   

                }
            }
        }
    }
}

// 将文本复制到剪贴板
void CopyToClipboard(const std::wstring& text) {
    if (!OpenClipboard(nullptr)) return;

    EmptyClipboard(); // 清空剪贴板
    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, (text.size() + 1) * sizeof(wchar_t));
    if (!hMem) {
        CloseClipboard();
        return;
    }

    // 将文本复制到内存
    wchar_t* pMem = static_cast<wchar_t*>(GlobalLock(hMem));
    wcscpy_s(pMem, text.size() + 1, text.c_str());
    GlobalUnlock(hMem);

    // 将内存数据放入剪贴板
    SetClipboardData(CF_UNICODETEXT, hMem);
    CloseClipboard();
}


void PasteText() {
    INPUT inputs[4] = {};

    inputs[0].type = INPUT_KEYBOARD;  // Ctrl Down
    inputs[0].ki.wVk = VK_CONTROL;

    inputs[1].type = INPUT_KEYBOARD;  // V Down
    inputs[1].ki.wVk = 'V';

    inputs[2].type = INPUT_KEYBOARD;  // V Up
    inputs[2].ki.wVk = 'V';
    inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;

    inputs[3].type = INPUT_KEYBOARD;  // Ctrl Up
    inputs[3].ki.wVk = VK_CONTROL;
    inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;

    SendInput(4, inputs, sizeof(INPUT));
}

// 向指定窗口发送粘贴命令
void PasteToWindow(HWND hwnd) {
    if (!IsWindow(hwnd)) {
        std::cerr << "窗口ID无效或窗口已关闭。" << std::endl;
        return;
    }

    // 激活目标窗口
    SetForegroundWindow(hwnd);

    //// 向窗口发送 Ctrl+V 快捷键 (通用方法)
    //PostMessage(hwnd, WM_KEYDOWN, VK_CONTROL, 0);
    //PostMessage(hwnd, WM_KEYDOWN, 'V', 0);
    //PostMessage(hwnd, WM_KEYUP, 'V', 0);
    //PostMessage(hwnd, WM_KEYUP, VK_CONTROL, 0);

    // 有些窗口可能更支持 WM_PASTE 消息
    //PostMessage(hwnd, WM_PASTE, 0, 0);

    PasteText();
}

void sendMessageToListBoxs(const std::wstring& text)
{
    // 执行按键发送
    if (text.length()> 0)
    {
        int count = SendMessage(hListBox, LB_GETCOUNT, 0, 0);
        CopyToClipboard(text);

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
                    
                    PasteToWindow(targetWindow);
                    std::this_thread::sleep_for(std::chrono::milliseconds(50)); // 短暂延迟防止冲突

                    //这种方式不支持中文，太麻烦了。。。
                    //for (wchar_t ch : text) {
                    //    PostMessage(targetWindow, WM_CHAR, ch, 0);            // 输入字符
                    //}
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
                if (HIWORD(wParam) == EN_CHANGE) {
                    std::wstring text = GetEditText(hDlg, LOWORD(wParam));
                    if (text[0] == L' ') {  // wcschr 查找第一个空格
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

                if (exeAddr.empty() || acctAddr.empty()) {
                    if (l_config->fileExists()) {
                        std::string eee = l_config->getString("batch_logins", "file_exe");
                        std::string fff = l_config->getString("batch_logins", "accounts");

                        if (!eee.empty()) {
                            SetEditText(hDlg, IDC_LOGIN_EXE_EDIT1, stringToWstring(eee));
                        }
                        if (!fff.empty()) {
                            SetEditText(hDlg, IDC_LOGIN_ACCOUNTS_EDIT2, stringToWstring(fff));
                        }
                        break;

                    }
                }

                if (exeAddr.empty()) {
                    exeAddr = "WoW.exe";
                }


                if (acctAddr.empty()) {
                    acctAddr = "accounts.txt";
                }


                if (!exeAddr.empty() && !acctAddr.empty()) {

                    batchGameLogin::logins_file_and_splite_screen(exeAddr, L"", acctAddr);

                    //登录完成后将进程放到listbox中
                    for (auto& proc : batchGameLogin::processList) {

                        WCHAR buffer[100];
                        swprintf_s(buffer, L"Window ID: 0x%08X, PID: %u", (UINT)proc.hwnd, proc.pi.dwProcessId);
                        SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)buffer);
                    }
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

// mqtt_msg 远程控制消息处理
void withRemoteCtrlHandler(const std::wstring& message) {
    std::wcout << "[自定义处理] 处理的消息内容: " << message << std::endl;
    bool prevState = isTaskRunning;
    isTaskRunning = false;
    
    bool StateRollBack = false;

    try {
        if (remoteCtrlisTaskRunning) {
            //先直接按键，后续要根据类型，开启走其他命令，如开启，或者关闭
            //sendKeyToListBoxsDown(std::stoi(message));

            std::unordered_map<std::wstring, std::wstring> data;

            CommandParseEr::parseMessage(message, data);

            // 识别消息类型
            CommandParseEr::MessageType type = CommandParseEr::parseMessageType(data[L"TYPE"]);

            switch (type) {
            case CommandParseEr::MessageType::SINGLE_KEY:
                // parseMessage("TYPE:1|KEY:65");
            {
 
                int key_id = std::stoi(data[L"KEY_ID"]);
                sendKeyToListBoxs(key_id);
                break;
            }
            case CommandParseEr::MessageType::COMMAND: {
                CommandParseEr::CommandType cmd = CommandParseEr::parseCommand(data[L"COMMAND"]);
                switch (cmd) {
                case CommandParseEr::CommandType::START_TASK: {
                    //操作总状态的，需要提前恢复状态
                    isTaskRunning = prevState;
                    StateRollBack = true;
                    // parseMessage("TYPE:2|COMMAND:START_TASK|TASK_ID:task_001"); 
                    StartTaskA(mainHWnd);
                    break;
                }
                case CommandParseEr::CommandType::STOP_TASK: {
                    //操作总状态的，需要提前恢复状态
                    isTaskRunning = prevState;
                    StateRollBack = true;
                    // parseMessage("TYPE:2|COMMAND:STOP_TASK");
                    std::cout << "Command: STOP_TASK\n";
                    StopTaskA(mainHWnd);
                    break;
                }
                case CommandParseEr::CommandType::CHECK_CHECKBOX: {
                    //parseMessage("TYPE:2|COMMAND:CHECK_CHECKBOX|CHECKBOX_ID:chk_123"); 
                    int  ck_id = std::stoi(data[L"CHECKBOX_ID"]);
                    if (ck_id < L_TASK_COUNTS) {
                        SendMessage(hCheckBoxes[ck_id], BM_SETCHECK, BST_CHECKED, 0);    // 选中复选框
                    }

                    break;
                }
                case CommandParseEr::CommandType::UNCHECK_CHECKBOX: { 
                    int  uck_id = std::stoi(data[L"CHECKBOX_ID"]);
                    if (uck_id < L_TASK_COUNTS) {
                        SendMessage(hCheckBoxes[uck_id], BM_SETCHECK, BST_UNCHECKED, 0);  // 取消选中复选框
                    }
                    break;
                }

                case CommandParseEr::CommandType::WOW_SAY: {
                    //parseMessage("TYPE:2|COMMAND:CHECK_CHECKBOX|CHECKBOX_ID:chk_123"); 
                    //按键 ENTER

                    sendKeyToListBoxs(VK_RETURN);
                    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 短暂延迟防止冲突

              
                    // 使用范围 for 循环遍历字符串
                    //for (char c : kesy) {
                    //    //这仅
                // 仅支持英文按键，由于是聊天内容，尝试换成中文输入
                    //    sendKeyToListBoxsDown(VkKeyScan(c));
                    //}

                    sendMessageToListBoxs(data[L"CONTENT"]);


                    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 短暂延迟防止冲突
                    //按键 ENTER
                    sendKeyToListBoxs(VK_RETURN);


                    break;
                }
                default: {
                    std::cout << "Unknown Command\n";
                }
                }
                break;
            }
            case CommandParseEr::MessageType::MOUSE_CLICK: {
            
                CommandParseEr::CommandType cmd = CommandParseEr::parseCommand(data[L"COMMAND"]); 
                std::wstring position = data[L"POSITION"];

                switch (cmd) {
                case CommandParseEr::CommandType::LIFT_CLICK: {
                    sendClickToListBoxs(position,true);
                    break;
                }

                case CommandParseEr::CommandType::RIGHT_CLICK: {
                    sendClickToListBoxs(position,false);
                    break;
                }

                default: {
                    std::cout << "Unknown Command\n";
                }
                }

            
            }

            case CommandParseEr::MessageType::MULTI_KEY: {
                //parseMessage("TYPE:3|KEYS:Shift+6");
                
                std::wstring kesy_ids = data[L"KEYS_ID"];
                
                if (kesy_ids.length() >0) {
                
                    sendMultipleKeysToListBoxs(kesy_ids);
                }


                break;
            }
            default:
                std::cout << "Invalid Message Format\n";
            }

        }
    } catch (const mqtt::exception& exc) {
        std::cerr << "错误: " << exc.what() << std::endl;
        
    }
    //如果没有恢复状态则恢复
    if (!StateRollBack) {
        isTaskRunning = prevState;        // 恢复原状态
    }
   
}

//窗口重排及清除无效窗口
void gameResiceAndClearInva() {

    std::vector<batchGameLogin::ProcessInfo> processList;

    int count = SendMessage(hListBox, LB_GETCOUNT, 0, 0);
    for (int j = count - 1; j >= 0; j--)  // 倒序遍历
    {
        WCHAR buffer[100];
        SendMessage(hListBox, LB_GETTEXT, j, (LPARAM)buffer);

        UINT windowID;
        if (swscanf_s(buffer, L"Window ID: 0x%08X", &windowID) == 1)
        {
            HWND targetWindow = (HWND)windowID;
            if (IsWindow(targetWindow))
            {
                batchGameLogin::ProcessInfo procInfo = {};
                procInfo.hwnd = targetWindow;
                processList.push_back(procInfo);
            }
            else
            {
                // 删除无效窗口
                SendMessage(hListBox, LB_DELETESTRING, (WPARAM)j, 0);
            }
        }
    }


    batchGameLogin::ArrangeWindows(processList);
    
}


//关闭所有游戏然后清除
//void closeListboxsProcesss() {
//
//    int count = SendMessage(hListBox, LB_GETCOUNT, 0, 0);
//    for (int j = 0; j < count; j++)
//    {
//        WCHAR buffer[100];
//        SendMessage(hListBox, LB_GETTEXT, j, (LPARAM)buffer);
//
//        UINT windowID;
//        if (swscanf_s(buffer, L"Window ID: 0x%08X", &windowID) == 1)
//        {
//            HWND targetWindow = (HWND)windowID;
//            if (IsWindow(targetWindow))
//            {
//                   PostMessage(targetWindow, WM_CLOSE, 0, 0);
//                //SendMessage(hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
//            }
//
//            // 删除窗口
//            SendMessage(hListBox, LB_DELETESTRING, (WPARAM)j, 0);
//
//        }
//    }
//}


void closeListboxProcesses() {
    int count = SendMessage(hListBox, LB_GETCOUNT, 0, 0);

    // 从最后一项向前遍历，避免删除项导致索引错误
    for (int j = count - 1; j >= 0; j--) {
        WCHAR buffer[100];
        SendMessage(hListBox, LB_GETTEXT, j, (LPARAM)buffer);

        UINT windowID;
        if (swscanf_s(buffer, L"Window ID: 0x%08X", &windowID) == 1) {
            HWND targetWindow = (HWND)windowID;
            if (IsWindow(targetWindow)) {
                PostMessage(targetWindow, WM_CLOSE, 0, 0);  // 发送关闭消息
                // SendMessage(targetWindow, WM_SYSCOMMAND, SC_CLOSE, 0); // 可选
            }
        }

        // 删除当前列表项
        SendMessage(hListBox, LB_DELETESTRING, (WPARAM)j, 0);
    }
}



//隐藏或者显示窗口
void SetWindowVisibilityEx(bool show) {
    int count = SendMessage(hListBox, LB_GETCOUNT, 0, 0);
    for (int j = 0; j < count; j++)
    {
        WCHAR buffer[100];
        SendMessage(hListBox, LB_GETTEXT, j, (LPARAM)buffer);

        UINT windowID;
        if (swscanf_s(buffer, L"Window ID: 0x%08X", &windowID) == 1)
        {
            HWND targetWindow = (HWND)windowID;
            if (IsWindow(targetWindow)) {
                LONG style = GetWindowLong(targetWindow, GWL_EXSTYLE);
                if (!show) {
                    SetWindowLong(targetWindow, GWL_EXSTYLE, style | WS_EX_TOOLWINDOW);  // 设为工具窗口（不会出现在任务栏）
                    ShowWindow(targetWindow, SW_HIDE);
                }
                else {
                    SetWindowLong(targetWindow, GWL_EXSTYLE, style & ~WS_EX_TOOLWINDOW); // 恢复正常窗口
                    ShowWindow(targetWindow, SW_SHOW);
                }
            }
            

        }
    }
}
 