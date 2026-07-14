#include "WowHelper.h"
 


// 封装构建管道名称的函数
std::string BuildPipeName(const std::string& baseName, DWORD pid) {
    std::ostringstream oss;
    oss << R"(\\.\pipe\)" << baseName << "_PID_" << pid;
    return oss.str();
}



std::string WoWGetPlayerName(DWORD pid) {
 
    // 创建PipeClient实例，并尝试读取玩家GUID
    PipeClient pipeClient(BuildPipeName(pipeName_KylePlayerNamePipe,pid));
    std::string names;
    if (pipeClient.readDataU8(names)) {
        // 如果成功读取GUID，则尝试获取玩家名称
         
		return names;
    }
    else {
        std::cerr << "读取玩家Name失败\n";
        return "";
    }
}

uint64_t WoWGetPlayerGuid(DWORD pid) {

    // 创建PipeClient实例，并尝试读取玩家GUID
    PipeClient pipeClient(BuildPipeName(pipeName_KylePlayerGuidPipe,pid));
    uint64_t guid = 0;
    if (pipeClient.ReadData(guid)) {
        
        return guid;
    }
    else {
        std::cerr << "读取玩家ID失败\n";
        return 0;
    }
}