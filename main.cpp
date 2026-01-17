// Module
// File: main.cpp   Version: 0.1.1   License: AGPLv3
//  Created:licheng 2024051604016   1月8日
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
