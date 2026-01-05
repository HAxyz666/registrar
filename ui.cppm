// Module
// File: ui.cppm   Version: 0.1.0   License: AGPLv3
// Created:licheng    id:2024051604016    2026-01-05 21:15:12
// Description:
//
export module ui;
import std;
using string = std::string;

// 在UI模块中定义服务接口
export class IRegistrarService {
public:
    virtual ~IRegistrarService() = default;
    virtual void initialize() = 0;
    virtual void studentEnrollsInCourse(string sid, string cid) = 0;
    virtual void studentDropsFromCourse(string sid, string cid) = 0;
    virtual void studentSchedule(const string& sid) = 0;
    virtual void studentGrades(const string& sid) = 0;
    virtual void studentCoursesList(const string& sid) = 0;
    virtual void teacherTeachingCourses(string tid) = 0;
    virtual void teacherGradeStudent(string tid, string sid, string cid, double grade) = 0;
    virtual void teacherSchedule(const string& tid) = 0;
    virtual void secretaryAssignTeacher(string secid, string tid, string cid) = 0;
    virtual void secretarySetSchedule(string secid, string cid, string timeSlot, string classroom) = 0;
    virtual void secretaryScheduledCourses(string secid) = 0;
    virtual void secretaryCancelSchedule(string secid, string cid, string timeSlot) = 0;
}; // 导入registrar模块以使用回调函数类型


export class UI {
public:
    void setRegistrarService(IRegistrarService* service);
    void exec();
private:
    IRegistrarService* _service = nullptr;
    void showMainMenu();
    void showStudentMenu();
    void showTeacherMenu();
    void showSecretaryMenu();
    int getChoice(const std::string& prompt, int min, int max);
    std::string getUserInput(const std::string& prompt);
};
void UI::exec() {
    std::println("欢迎使用教务管理系统！");
    if (_service) {
        _service->initialize();  // 通过接口初始化系统数据
    }

    while (true) {
        showMainMenu();
        int choice = getChoice("请选择: ", 0, 3);

        switch (choice) {
            case 1:
                showStudentMenu();
                break;
            case 2:
                showTeacherMenu();
                break;
            case 3:
                showSecretaryMenu();
                break;
            case 0:
                std::println("感谢使用教务管理系统，再见！");
                return;
            default:
                std::println("无效选择，请重新输入。");
        }
    }
}

void UI::showMainMenu() {
    std::println("\n教务管理系统");
    std::println("=================");
    std::println("1. 学生功能");
    std::println("2. 教师功能");
    std::println("3. 教学秘书功能");
    std::println("0. 退出系统");
}

void UI::showStudentMenu() {
    while (true) {
        std::println("\n学生功能");
        std::println("=================");
        std::println("1. 选课");
        std::println("2. 查看个人课表");
        std::println("3. 查看个人成绩");
        std::println("4. 查看已选课程");
        std::println("5. 退课");
        std::println("0. 返回主菜单");

        int choice = getChoice("请选择: ", 0, 5);

        switch (choice) {
            case 1: {
                std::string sid = getUserInput("请输入学生ID: ");
                std::string cid = getUserInput("请输入课程ID: ");
                if (_service) {
                    _service->studentEnrollsInCourse(sid, cid);
                }
                break;
            }
            case 2: {
                std::string sid = getUserInput("请输入学生ID: ");
                if (_service) {
                    _service->studentSchedule(sid);
                }
                break;
            }
            case 3: {
                std::string sid = getUserInput("请输入学生ID: ");
                if (_service) {
                    _service->studentGrades(sid);
                }
                break;
            }
            case 4: {
                std::string sid = getUserInput("请输入学生ID: ");
                if (_service) {
                    _service->studentCoursesList(sid);
                }
                break;
            }
            case 5: {
                std::string sid = getUserInput("请输入学生ID: ");
                std::string cid = getUserInput("请输入课程ID: ");
                if (_service) {
                    _service->studentDropsFromCourse(sid, cid);
                }
                break;
            }
            case 0:
                return;
            default:
                std::println("无效选择，请重新输入。");
        }
    }
}

void UI::showTeacherMenu() {
    while (true) {
        std::println("\n教师功能");
        std::println("=================");
        std::println("1. 查看教学任务");
        std::println("2. 评定学生成绩");
        std::println("3. 查看个人课表");
        std::println("0. 返回主菜单");

        int choice = getChoice("请选择: ", 0, 3);

        switch (choice) {
            case 1: {
                std::string tid = getUserInput("请输入教师ID: ");
                if (_service) {
                    _service->teacherTeachingCourses(tid);
                }
                break;
            }
            case 2: {
                std::string tid = getUserInput("请输入教师ID: ");
                std::string sid = getUserInput("请输入学生ID: ");
                std::string cid = getUserInput("请输入课程ID: ");
                std::string gradeStr = getUserInput("请输入成绩: ");
                try {
                    double grade = std::stod(gradeStr);
                    if (grade >= 0 && grade <= 100) {
                        if (_service) {
                            _service->teacherGradeStudent(tid, sid, cid, grade);
                        }
                    } else {
                        std::println("成绩应在0-100之间。");
                    }
                } catch (const std::exception&) {
                    std::println("请输入有效的成绩数字。");
                }
                break;
            }
            case 3: {
                std::string tid = getUserInput("请输入教师ID: ");
                if (_service) {
                    _service->teacherSchedule(tid);
                }
                break;
            }
            case 0:
                return;
            default:
                std::println("无效选择，请重新输入。");
        }
    }
}

void UI::showSecretaryMenu() {
    while (true) {
        std::println("\n教学秘书功能");
        std::println("=================");
        std::println("1. 创建课程");
        std::println("2. 分配教师到课程");
        std::println("3. 设置课程时间安排");
        std::println("4. 查看已安排课程");
        std::println("5. 取消课程时间安排");
        std::println("0. 返回主菜单");

        int choice = getChoice("请选择: ", 0, 5);

        switch (choice) {
            case 1: {
                std::string secid = getUserInput("请输入教学秘书ID: ");
                std::string cid = getUserInput("请输入课程ID: ");
                std::string cname = getUserInput("请输入课程名称: ");
                // 由于现有的 secretaryCreateCourse 需要已存在的课程对象，
                // 我们需要先创建课程然后添加到系统中
                std::println("课程创建需通过系统初始化，当前功能仅作演示。");
                break;
            }
            case 2: {
                std::string secid = getUserInput("请输入教学秘书ID: ");
                std::string tid = getUserInput("请输入教师ID: ");
                std::string cid = getUserInput("请输入课程ID: ");
                if (_service) {
                    _service->secretaryAssignTeacher(secid, tid, cid);
                }
                break;
            }
            case 3: {
                std::string secid = getUserInput("请输入教学秘书ID: ");
                std::string cid = getUserInput("请输入课程ID: ");
                std::string timeSlot = getUserInput("请输入时间安排: ");
                std::string classroom = getUserInput("请输入教室: ");
                if (_service) {
                    _service->secretarySetSchedule(secid, cid, timeSlot, classroom);
                }
                break;
            }
            case 4: {
                std::string secid = getUserInput("请输入教学秘书ID: ");
                if (_service) {
                    _service->secretaryScheduledCourses(secid);
                }
                break;
            }
            case 5: {
                std::string secid = getUserInput("请输入教学秘书ID: ");
                std::string cid = getUserInput("请输入课程ID: ");
                std::string timeSlot = getUserInput("请输入要取消的时间段: ");
                if (_service) {
                    _service->secretaryCancelSchedule(secid, cid, timeSlot);
                }
                break;
            }
            case 0:
                return;
            default:
                std::println("无效选择，请重新输入。");
        }
    }
}

std::string UI::getUserInput(const std::string& prompt) {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    return input;
}
int UI::getChoice(const std::string& prompt, int min, int max) {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);

    try {
        int choice = std::stoi(input);
        if (choice >= min && choice <= max) {
            return choice;
        } else {
            std::println("输入超出范围，请输入 {} 到 {} 之间的数字。", min, max);
            return -1;  // 返回无效值，调用者需要重新获取输入
        }
    } catch (const std::exception&) {
        std::println("请输入有效的数字。");
        return -1;  // 返回无效值，调用者需要重新获取输入
    }
}

void UI::setRegistrarService(IRegistrarService* service) {
    _service = service;
}
