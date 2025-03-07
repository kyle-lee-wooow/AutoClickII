#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <windows.h>
#include <string>
#include <thread>
#include <shellapi.h>  // 解决未定义 ShellExecuteW 问题


namespace batchGameLogin{
// 账号结构体
    struct Account {
        std::string username;
        std::string password;
    };

    // 字符串转换：std::string -> std::wstring
    std::wstring stringToWstring(const std::string& str) {
        return std::wstring(str.begin(), str.end());
    }

    // 读取账号信息
    std::vector<Account> readAccounts(const std::string& filePath) {
        std::vector<Account> accounts;
        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "无法打开 accounts.txt！\n";
            return accounts;
        }

        std::string line;
        while (std::getline(file, line)) {
            size_t commaPos = line.find(',');
            if (commaPos != std::string::npos) {
                accounts.push_back({ line.substr(0, commaPos), line.substr(commaPos + 1) });
            }
        }
        file.close();
        return accounts;
    }

    // 运行游戏
    bool launchGame(const std::wstring& gamePath) {
        HINSTANCE result = ShellExecuteW(NULL, L"open", gamePath.c_str(), NULL, NULL, SW_SHOWNORMAL);
        return ((int)result > 32);  // ShellExecute 返回大于 32 的值表示成功
    }

    // 模拟键盘输入
    void sendKey(const std::string& text) {
        for (char ch : text) {
            SHORT vk = VkKeyScanA(ch);
            if (vk != -1) {
                INPUT input = { 0 };
                input.type = INPUT_KEYBOARD;
                input.ki.wVk = vk;
                SendInput(1, &input, sizeof(INPUT));

                // 释放按键
                input.ki.dwFlags = KEYEVENTF_KEYUP;
                SendInput(1, &input, sizeof(INPUT));
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(50));  // 短暂延迟
        }
    }

    // 发送单个按键
    void sendSingleKey(WORD key) {
        INPUT input = { 0 };
        input.type = INPUT_KEYBOARD;
        input.ki.wVk = key;
        SendInput(1, &input, sizeof(INPUT));

        input.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1, &input, sizeof(INPUT));
    }

    // 激活游戏窗口
    bool activateGameWindow(const std::wstring& windowTitle) {
        HWND hwnd = FindWindowW(NULL, windowTitle.c_str());
        if (!hwnd) return false;
        SetForegroundWindow(hwnd);
        return true;
    }

    int logins_file(std::string gamePath, std::wstring windowTitle, std::string accountsFile) {
        //std::string gamePath = "WoW.exe";  // 请替换为游戏路径
        std::wstring wGamePath = stringToWstring(gamePath);
        //std::wstring windowTitle = L"魔兽世界"; // 窗口标题，需根据实际情况调整
        //std::string accountsFile = "accounts.txt";

        std::vector<Account> accounts = readAccounts(accountsFile);
        if (accounts.empty()) {
            std::cerr << "没有找到有效的账号信息！\n";
            return 1;
        }

        for (const auto& acc : accounts) {
            std::cout << "正在登录账号: " << acc.username << std::endl;

            // 启动游戏
            if (!launchGame(wGamePath)) {
                std::cerr << "无法启动游戏！\n";
                continue;
            }

            // 等待游戏窗口激活
 /*           std::this_thread::sleep_for(std::chrono::seconds(10));
            if (!activateGameWindow(windowTitle)) {
                std::cerr << "未能找到游戏窗口！\n";
                continue;
            }*/

            // 输入账号
            sendKey(acc.username);
            std::this_thread::sleep_for(std::chrono::milliseconds(900));

            // 切换到密码框（Tab）
            sendSingleKey(VK_TAB);
            std::this_thread::sleep_for(std::chrono::milliseconds(900));

            // 输入密码
            sendKey(acc.password);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));

            // 按下回车键登录
            sendSingleKey(VK_RETURN);
            std::this_thread::sleep_for(std::chrono::seconds(3));

            // 发送 Ctrl + Alt + A
            keybd_event(VK_CONTROL, 0, 0, 0);
            keybd_event(VK_MENU, 0, 0, 0);
            keybd_event('A', 0, 0, 0);
            keybd_event('A', 0, KEYEVENTF_KEYUP, 0);
            keybd_event(VK_MENU, 0, KEYEVENTF_KEYUP, 0);
            keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);

            // 等待退出操作（这里可以添加手动退出的逻辑）
            std::this_thread::sleep_for(std::chrono::seconds(2));

            // 这里可以加入退出账号的代码，例如发送 ESC -> 选择退出
        }

        std::cout << "所有账号已尝试登录！\n";
        return 0;
    }
}
