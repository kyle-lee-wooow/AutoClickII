#pragma once
 
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

#include "framework.h"
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
#include <uxtheme.h>

#include "resource.h"
#include "batch_login.h"
#undef max
#undef min
#include "with_mqtt.h"
#include "MsgParse.h"
#include <codecvt>

#include "cfg.h" //本地配置
#pragma comment(lib, "UxTheme.lib")
#pragma comment(lib, "comctl32.lib")

HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];
HWND hListBox;
HWND TaskStartBtn;
HWND TaskStopBtn;
HWND hCheckBoxes[L_TASK_COUNTS];
HWND hEditB[L_TASK_COUNTS];
HWND hEditC[L_TASK_COUNTS];
HWND mainHWnd;

HWND hStatusLabel; // 状态标签

//本地文件配置
std::unique_ptr<CFG> l_config;  // 使用 unique_ptr


// 用来存储每一行的下次执行时间
std::chrono::steady_clock::time_point nextExecutionTimes[L_TASK_COUNTS];

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//单个按键
void sendKeyToListBoxs(int key);
void sendKeyToListBoxsDown(int key);
void sendKeyToListBoxsUp(int key);

//窗口重新排序，及清除无效进程
void gameResiceAndClearInva();

void sendMessageToListBoxs(const std::wstring& text);

HBRUSH hGreenBrush, hRedBrush; // 按钮背景色画刷

bool isTaskRunning = false;  // 任务是否正在执行
std::thread taskThread;      // 任务线程

bool remoteCtrlisTaskRunning = false;  // 远控任务是否正在执行

//处理远程控制事件
void withRemoteCtrlHandler(const std::wstring& message);

//强制关闭游戏
void closeListboxProcesses();

//多个案件
void sendMultipleKeysToListBoxs(const std::wstring& keyCombo);
std::vector<BYTE> ParseKeyCombo(const std::wstring& keyCombo);

//多按键发送
std::vector<std::vector<BYTE>> ParseMultipleKeyCombos(const std::wstring& keyCombos);

//点击说明
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

//批量登录 
INT_PTR CALLBACK BATCH_LOGINS(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

//选择本地文件
void SelectFile(HWND hEdit);

//隐藏/显示所有窗口
void SetWindowVisibilityEx(bool show);
 
//鼠标点击
void sendClickToListBoxs(std::wstring key, bool leftClick);

//纯数字字符串检查
bool is_all_digits(const std::wstring& s);