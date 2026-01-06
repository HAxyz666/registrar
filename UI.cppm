// Module
// File: UI.cppm   Version: 0.1.0   License: AGPLv3
// Created:licheng    id:2024051604016    2026-01-06 20:26:03
// Description:第一版ui界面
//

export module registrar:ui;

import std;
using std::numeric_limits;

using std::string;
using std::cin;
using std::endl;
using std::format;
using std::print;

export class UI
{
public:
    static UI& singleton();
    void run();
    void showMainMenu();
    void showStudentMenu();
    void showTeacherMenu();
    void showSecretaryMenu();
    
    // 设置当前用户信息
    void setCurrentUser(string userType, string userId);
    void clearCurrentUser();
    
    // 获取当前用户信息
    string getCurrentUserType() const { return currentUserType; }
    string getCurrentUserId() const { return currentUserId; }
    
    // 获取用户输入
    string getUserInput(const string& prompt);
    int getMenuChoice(const string& prompt, int maxChoice);
    
    // 显示消息
    void showMessage(const string& message);
    void showError(const string& error);
    
    // 工具函数
    void clearScreen();
    void pauseScreen();
    
private:
    UI() = default;
    string currentUserType;
    string currentUserId;
};

UI& UI::singleton()
{
    static UI instance;
    return instance;
}

void UI::run()
{
    clearCurrentUser();
}

void UI::clearScreen()
{
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

void UI::pauseScreen()
{
    print("\n按回车键继续...");
    
    // 尝试读取输入，但不阻塞
    cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
    
    // 如果有更多输入，读取一个字符
    if (cin.rdbuf()->in_avail() > 0) {
        cin.get();
    }
}

void UI::setCurrentUser(string userType, string userId)
{
    currentUserType = userType;
    currentUserId = userId;
}

void UI::clearCurrentUser()
{
    currentUserType = "";
    currentUserId = "";
}

string UI::getUserInput(const string& prompt)
{
    string input;
    print("{}", prompt);
    cin >> input;
    return input;
}

int UI::getMenuChoice(const string& prompt, int maxChoice)
{
    int choice;
    print("{}", prompt);
    
    if (!(cin >> choice)) {
        // 如果输入失败，返回0
        cin.clear();
        cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
        return 0;
    }
    
    while (choice < 0 || choice > maxChoice) {
        print("无效选择，请输入0-{}之间的数字: ", maxChoice);
        
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
            return 0;
        }
    }
    
    return choice;
}

void UI::showMessage(const string& message)
{
    print("{}\n", message);
}

void UI::showError(const string& error)
{
    print("错误: {}\n", error);
}

void UI::showMainMenu()
{
    clearScreen();
    print("========================================\n");
    print("        教务管理系统主菜单\n");
    print("========================================\n");
    print("1. 学生登录\n");
    print("2. 教师登录\n");
    print("3. 教学秘书登录\n");
    print("0. 退出系统\n");
    print("========================================\n");
}

void UI::showStudentMenu()
{
    clearScreen();
    print("========================================\n");
    print("        学生功能菜单\n");
    print("========================================\n");
    print("1. 查看课程列表\n");
    print("2. 查看个人课表\n");
    print("3. 查看成绩\n");
    print("4. 选修课程\n");
    print("5. 取消选课\n");
    print("0. 返回主菜单\n");
    print("========================================\n");
}

void UI::showTeacherMenu()
{
    clearScreen();
    print("========================================\n");
    print("        教师功能菜单\n");
    print("========================================\n");
    print("1. 查看授课课程列表\n");
    print("2. 查看个人课表\n");
    print("3. 查看课程学生名单\n");
    print("4. 给学生打分\n");
    print("0. 返回主菜单\n");
    print("========================================\n");
}

void UI::showSecretaryMenu()
{
    clearScreen();
    print("========================================\n");
    print("      教学秘书功能菜单\n");
    print("========================================\n");
    print("1. 创建课程\n");
    print("2. 分配教师到课程\n");
    print("3. 设置课程时间表\n");
    print("4. 查看已排课程\n");
    print("5. 取消排课\n");
    print("0. 返回主菜单\n");
    print("========================================\n");
}
