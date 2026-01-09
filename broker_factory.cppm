// =======================================
// Module
// File: broker_factory.cppm   Version: 0.1.0   License: AGPLv3
// Created: lichao2024051604024  3042525170@qq.com   2026-01-09 22:29:46
// Description:
//     简单工厂模式：Broker工厂类，用于创建不同类型的Broker实例

export module registrar:broker_factory;

import std;

using std::shared_ptr;

// Broker基类接口
export class IBroker {
public:
    virtual ~IBroker() = default;
    virtual void initialize() = 0;
};

// 前向声明Broker类
export class StudentBroker;
export class CourseBroker;
export class TeacherBroker;
export class SecretaryBroker;

// 简单工厂类
export class BrokerFactory {
public:
    enum class BrokerType {
        Student,
        Course,
        Teacher,
        Secretary
    };

    // 创建函数 - 在registrar模块中通过dynamic_pointer_cast使用
    static shared_ptr<IBroker> createBroker(BrokerType type) {
        // 由于模块限制，这里返回nullptr，实际创建在各个broker模块中进行
        // Registrar通过直接使用make_shared创建各个broker实例
        return nullptr;
    }
};
