// Module
// File: main.cpp   Version: 0.1.0   License: AGPLv3
// Created:licheng  2026-01-06
// Description:
//      教务管理系统主程序

import registrar;
import std;

int main()
{
    // 初始化教务系统
    Registrar::singleton().initialize();
    
    // 启动UI界面

    
    return Registrar::singleton().exec();
}
