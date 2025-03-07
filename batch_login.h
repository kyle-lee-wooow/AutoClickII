#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <windows.h>
#include <string>
#include <thread>
#include <shellapi.h>  // ���δ���� ShellExecuteW ����


namespace batchGameLogin{
// �˺Žṹ��
    struct Account {
        std::string username;
        std::string password;
    };

    // �ַ���ת����std::string -> std::wstring
    std::wstring stringToWstring(const std::string& str) {
        return std::wstring(str.begin(), str.end());
    }

    // ��ȡ�˺���Ϣ
    std::vector<Account> readAccounts(const std::string& filePath) {
        std::vector<Account> accounts;
        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "�޷��� accounts.txt��\n";
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

    // ������Ϸ
    bool launchGame(const std::wstring& gamePath) {
        HINSTANCE result = ShellExecuteW(NULL, L"open", gamePath.c_str(), NULL, NULL, SW_SHOWNORMAL);
        return ((int)result > 32);  // ShellExecute ���ش��� 32 ��ֵ��ʾ�ɹ�
    }

    // ģ���������
    void sendKey(const std::string& text) {
        for (char ch : text) {
            SHORT vk = VkKeyScanA(ch);
            if (vk != -1) {
                INPUT input = { 0 };
                input.type = INPUT_KEYBOARD;
                input.ki.wVk = vk;
                SendInput(1, &input, sizeof(INPUT));

                // �ͷŰ���
                input.ki.dwFlags = KEYEVENTF_KEYUP;
                SendInput(1, &input, sizeof(INPUT));
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(50));  // �����ӳ�
        }
    }

    // ���͵�������
    void sendSingleKey(WORD key) {
        INPUT input = { 0 };
        input.type = INPUT_KEYBOARD;
        input.ki.wVk = key;
        SendInput(1, &input, sizeof(INPUT));

        input.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1, &input, sizeof(INPUT));
    }

    // ������Ϸ����
    bool activateGameWindow(const std::wstring& windowTitle) {
        HWND hwnd = FindWindowW(NULL, windowTitle.c_str());
        if (!hwnd) return false;
        SetForegroundWindow(hwnd);
        return true;
    }

    int logins_file(std::string gamePath, std::wstring windowTitle, std::string accountsFile) {
        //std::string gamePath = "WoW.exe";  // ���滻Ϊ��Ϸ·��
        std::wstring wGamePath = stringToWstring(gamePath);
        //std::wstring windowTitle = L"ħ������"; // ���ڱ��⣬�����ʵ���������
        //std::string accountsFile = "accounts.txt";

        std::vector<Account> accounts = readAccounts(accountsFile);
        if (accounts.empty()) {
            std::cerr << "û���ҵ���Ч���˺���Ϣ��\n";
            return 1;
        }

        for (const auto& acc : accounts) {
            std::cout << "���ڵ�¼�˺�: " << acc.username << std::endl;

            // ������Ϸ
            if (!launchGame(wGamePath)) {
                std::cerr << "�޷�������Ϸ��\n";
                continue;
            }

            // �ȴ���Ϸ���ڼ���
 /*           std::this_thread::sleep_for(std::chrono::seconds(10));
            if (!activateGameWindow(windowTitle)) {
                std::cerr << "δ���ҵ���Ϸ���ڣ�\n";
                continue;
            }*/

            // �����˺�
            sendKey(acc.username);
            std::this_thread::sleep_for(std::chrono::milliseconds(900));

            // �л��������Tab��
            sendSingleKey(VK_TAB);
            std::this_thread::sleep_for(std::chrono::milliseconds(900));

            // ��������
            sendKey(acc.password);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));

            // ���»س�����¼
            sendSingleKey(VK_RETURN);
            std::this_thread::sleep_for(std::chrono::seconds(3));

            // ���� Ctrl + Alt + A
            keybd_event(VK_CONTROL, 0, 0, 0);
            keybd_event(VK_MENU, 0, 0, 0);
            keybd_event('A', 0, 0, 0);
            keybd_event('A', 0, KEYEVENTF_KEYUP, 0);
            keybd_event(VK_MENU, 0, KEYEVENTF_KEYUP, 0);
            keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);

            // �ȴ��˳������������������ֶ��˳����߼���
            std::this_thread::sleep_for(std::chrono::seconds(2));

            // ������Լ����˳��˺ŵĴ��룬���緢�� ESC -> ѡ���˳�
        }

        std::cout << "�����˺��ѳ��Ե�¼��\n";
        return 0;
    }
}
