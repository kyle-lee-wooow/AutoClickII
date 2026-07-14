#pragma once
#include <chrono>
#include <condition_variable>
#include <functional>  // 引入 std::function
#include <iostream>
#include <mutex>
#include <string>
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif
#include <mqtt/async_client.h>

const std::string TOPIC = "wow/key_sync";                    // MQTT 主题

namespace RemoteCtrl{
    
    class Callback : public virtual mqtt::callback
    {
    private:
        mqtt::async_client& client;
        std::function<void(const std::wstring&)> messageHandler;  // 自定义回调函数

        void subscribeTopic() {
            try {
                client.subscribe(TOPIC, 1);
                std::cout << "已发起订阅主题：" << TOPIC << std::endl;
            }
            catch (const mqtt::exception& exc) {
                std::cerr << "订阅主题失败: " << exc.what() << std::endl;
            }
        }
        
    public:
        explicit Callback(mqtt::async_client& cli)
            : client(cli) {

        }

        void setCallFunc(std::function<void(const std::wstring&)> func) {
            messageHandler = func;
        }

        // 自动重连成功后会进入这里，需要重新订阅主题。
        void connected(const std::string& cause) override {
            std::cout << "MQTT连接成功，原因: " << cause << std::endl;
            subscribeTopic();
        }

        // 断线后由 Paho automatic_reconnect 负责后台重连，不要在回调线程里阻塞循环。
        void connection_lost(const std::string& cause) override {
            std::cerr << "\nMQTT连接已断开，原因: " << cause << std::endl;
            std::cout << "等待自动重连..." << std::endl;
        }

        std::wstring Utf8ToWstring(const std::string& utf8Str) {
            int sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, nullptr, 0);
            if (sizeNeeded <= 0) {
                return L"";
            }

            std::wstring wstr(sizeNeeded, 0);
            MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, &wstr[0], sizeNeeded);
            if (!wstr.empty() && wstr.back() == L'\0') {
                wstr.pop_back();
            }
            return wstr;
        }

        // 消息回调
        void message_arrived(mqtt::const_message_ptr msg) override {
            // 执行传入的处理函数
            if (messageHandler) {
                std::string payload = msg->get_payload();
                std::wstring wstr = Utf8ToWstring(payload);
                messageHandler(wstr);
            }
        }
    };

    class Conntection
    {

    public:
        std::string SERVER_ADDRESS ; // MQTT 服务器地址
        std::string MQ_NAME;                    // MQTT 主题
        std::string MQ_PWD;

        // 提供全局访问点
        static Conntection& getInstance() {
            static Conntection instance;  // C++11 局部静态变量，线程安全
            
            return instance;
        }

        void setMQTTParam(const std::string& add, const std::string& na, const std::string& pwd) {
            SERVER_ADDRESS = add;
            MQ_NAME = na;
            MQ_PWD = pwd;
        }

        std::string GenerateTimeBasedID() {
            // 获取当前时间的时间点
            auto now = std::chrono::system_clock::now();

            // 获取自 epoch（1970-01-01 00:00:00 UTC）以来的毫秒数
            auto timestamp_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

            std::cout << "Current timestamp (in milliseconds): " << timestamp_ms << std::endl;
            return  std::to_string(timestamp_ms);
        }

        int loading(std::function<void(const std::wstring&)> func) {
            {
                std::lock_guard<std::mutex> lock(mtx);
                if (lisTaskRunning) { return 0; }
                lisTaskRunning = true;
                stopRequested = false;
            }
                
            // 设置连接参数
            auto connOpts = mqtt::connect_options_builder()
                .user_name(MQ_NAME)
                .password(MQ_PWD)
                .clean_session()
                .automatic_reconnect(std::chrono::seconds(3), std::chrono::seconds(30)) // 自动重连机制
                .finalize();
     
            if (!client) {
                std::string clientID = GenerateTimeBasedID();
                client = std::make_unique<mqtt::async_client>(SERVER_ADDRESS, clientID);

                callback = std::make_unique<Callback>(*client);
                client->set_callback(*callback);
            }

            callback->setCallFunc(func);

            try {
                std::cout << "正在连接至 MQTT 服务器..." << std::endl;
                if (!client->is_connected()) {
                    client->connect(connOpts)->wait();
                }

                // 首次连接后立即订阅；断线重连后会由 connected() 回调重新订阅。
                client->subscribe(TOPIC, 1)->wait();
                std::cout << "已成功订阅主题：" << TOPIC << std::endl;
            }
            catch (const mqtt::exception& exc) {
                std::lock_guard<std::mutex> lock(mtx);
                lisTaskRunning = false;
                std::cerr << "错误: " << exc.what() << std::endl;
                return 1;
            }
         
            // 等待停止信号来阻止远控线程退出。
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [this] { return stopRequested; });
            lisTaskRunning = false;
            return 0;
        }

        // 添加信号触发来停止客户端
        void stopMqtt() {
            {
                std::lock_guard<std::mutex> lock(mtx);
                stopRequested = true;
            }

            if (client) {
                try {
                    client->disconnect()->wait();
                }
                catch (const mqtt::exception& exc) {
                    std::cerr << "MQTT断开失败: " << exc.what() << std::endl;
                }
            }

            cv.notify_all();
        }
    private:
        bool lisTaskRunning = false;  // 任务是否正在执行
        bool stopRequested = false;
 
        Conntection() {};
        ~Conntection() {};
        std::condition_variable cv;
        std::mutex mtx;
        std::unique_ptr<mqtt::async_client> client;
        std::unique_ptr<Callback> callback;

    };

   



}



