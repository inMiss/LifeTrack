#ifndef LIFETRACK_CUSTOMMESSAGE_H
#define LIFETRACK_CUSTOMMESSAGE_H

#pragma once
#include <functional>
#include <map>
#include <vector>
#include <QString>
#include <QMutex>

// 消息类型枚举
enum EventID {
    EVT_OPER_BUTTON_CLICKED = 1001,  // 按钮点击
};

// 通用消息结构
struct Message {
    int nEventID;      // 消息类型
    int nSenderID;     // 发送者ID
    int nIntParam;     // 整型参数
    QString qsParam;   // 字符串参数
    void* pParam;      // 指针参数

    Message(int eventID = 0, int senderID = 0, int intParam = 0,
        const QString& strParam = "", void* ptrParam = nullptr)
        : nEventID(eventID)
        , nSenderID(senderID)
        , nIntParam(intParam)
        , qsParam(strParam)
        , pParam(ptrParam)
    {
    }
};

class MessageHandler
{
public:
    using Callback = std::function<bool(const Message&)>;

    // 饿汉式单例：程序启动时就创建，线程安全
    static MessageHandler& instance()
    {
        static MessageHandler handler;
        return handler;
    }

    // 注册消息监听器
    void registerListener(int eventID, Callback callback)
    {
        QMutexLocker locker(&m_mutex);
        m_listeners[eventID].push_back(callback);
    }

    // 发送消息，返回是否已被处理
    bool sendMessage(const Message& msg)
    {
        QMutexLocker locker(&m_mutex);
        auto it = m_listeners.find(msg.nEventID);
        if (it != m_listeners.end()) {
            for (const auto& callback : it->second) {
                if (callback(msg)) {
                    return true;
                }
            }
        }
        return false;
    }

    // 发送消息（简化版）
    bool sendMessage(int eventID, int senderID = 0, int intParam = 0,
        const QString& strParam = "", void* ptrParam = nullptr)
    {
        Message msg(eventID, senderID, intParam, strParam, ptrParam);
        return sendMessage(msg);
    }

private:
    // 私有构造函数，确保单例
    MessageHandler() = default;
    ~MessageHandler() = default;

    // 禁止拷贝和赋值
    MessageHandler(const MessageHandler&) = delete;
    MessageHandler& operator=(const MessageHandler&) = delete;

    std::map<int, std::vector<Callback>> m_listeners;
    QMutex m_mutex;
};


#endif