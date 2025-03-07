#pragma once

#include "resource.h"
#include "batch_login.h"


std::string WcharToString(const wchar_t* wstr)
{
    // ��ȡת������ַ���
    int len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);

    // ����һ���㹻��� buffer �����ת������ַ���
    char* str = new char[len];

    // ִ��ת��
    WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);

    // ����ת����� std::string
    std::string result(str);

    delete[] str;  // �ǵ��ͷ��ڴ�
    return result;
}


// ��ȡ�ؼ��ı��ĺ���
std::wstring GetEditText(HWND hDlg, int controlID) {
    wchar_t szText[256];  // ������
    GetWindowText(GetDlgItem(hDlg, controlID), szText, sizeof(szText) / sizeof(wchar_t));
    return std::wstring(szText);
}