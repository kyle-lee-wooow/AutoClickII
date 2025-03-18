#pragma once


#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>


namespace CommandParseEr{

    
        // 命令类型枚举
        enum class CommandType { START_TASK, STOP_TASK, CHECK_CHECKBOX, UNCHECK_CHECKBOX, UNKNOWN,WOW_SAY };

        // 消息类型枚举
        enum class MessageType { SINGLE_KEY, COMMAND, MULTI_KEY, INVALID };

        // 解析 Command 类型
       inline CommandType parseCommand(const std::wstring& command) {
            if (command == L"START_TASK") return CommandType::START_TASK;
            if (command == L"STOP_TASK") return CommandType::STOP_TASK;
            if (command == L"CHECK_CHECKBOX") return CommandType::CHECK_CHECKBOX;
            if (command == L"UNCHECK_CHECKBOX") return CommandType::UNCHECK_CHECKBOX;
            if (command == L"WOW_SAY") return CommandType::WOW_SAY;
            return CommandType::UNKNOWN;
        }

        // 解析消息类型
       inline MessageType parseMessageType(const std::wstring& type) {
            if (type == L"1") return MessageType::SINGLE_KEY;
            if (type == L"2") return MessageType::COMMAND;
            if (type == L"3") return MessageType::MULTI_KEY;
            return MessageType::INVALID;
        }

        // 消息解析函数
       inline void parseMessage(const std::wstring& message, std::unordered_map<std::wstring, std::wstring>& data) {
           std::wstringstream stream(message); // 使用 wstringstream 处理 wstring
           std::wstring segment;

           while (std::getline(stream, segment, L'|')) {  // 分隔符改为宽字符 L'|'
               size_t pos = segment.find(L':');           // 查找分隔符 L':'
               if (pos != std::wstring::npos) {
                   std::wstring key = segment.substr(0, pos);
                   std::wstring value = segment.substr(pos + 1);
                   data[key] = value;
               }
           }
       }

       inline int test() {
            /*
         

            //按键65
            TYPE:1|KEY:65

            //启动定时任务
            TYPE:2|COMMAND:START_TASK|TASK_ID:0

            //关闭定时任务
            TYPE:2|COMMAND:STOP_TASK|TASK_ID:0

            //选中选择任务框0
            TYPE:2|COMMAND:CHECK_CHECKBOX|CHECKBOX_ID:0

            //取消选中框0
            TYPE:2|COMMAND:UNCHECK_CHECKBOX|CHECKBOX_ID:0

             //wow喊话信息
            TYPE:2|COMMAND:WOW_SAY|CONTENT:/dance

            //发送批量按键
            TYPE:3|KEYS:Shift+6
            
            */

            return 0;
        }
}