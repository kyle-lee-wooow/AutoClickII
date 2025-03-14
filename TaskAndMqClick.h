#pragma once
 
#include "resource.h"
#include "batch_login.h"
#undef max
#undef min
#include "with_mqtt.h"
#include "MsgParse.h"

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


// 获取控件文本的函数
std::wstring GetEditText(HWND hDlg, int controlID) {
    wchar_t szText[256];  // 缓冲区
    GetWindowText(GetDlgItem(hDlg, controlID), szText, sizeof(szText) / sizeof(wchar_t));
    return std::wstring(szText);
}