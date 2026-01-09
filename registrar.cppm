// Module
// File: registrar.cppm   Version: 0.1.0   License: AGPLv3
// Created: DaiSen Zhou   3563248115@qq.com   2026-01-02 13:41:55
// Description:
//     The interface and implementation of Register class are logically separated.


//  [v1.0] chao li (3042525170@qq.com)   2026-01-04
//         * added:secretary;
//  [v2.0] chao li (3042525170@qq.com)   2026-01-09
//         * added:用Broker代管者模式;

export module registrar;

import std;

// 导出所有模块分区
export import :broker_factory;
export import :student_broker;
export import :course_broker;
export import :teacher_broker;
export import :secretary_broker;
export import :student;
export import :course;
export import :teacher;
export import :secretary;
export import :ui;

using std::string;
using std::vector;
using std::shared_ptr;
using std::make_shared;

export class Registrar
{
public:
    static Registrar& singleton();

    // 学生相关方法（委托给StudentBroker）
    void studentDropsFromCourse(string sid, string cid);
    void studentEnrollsInCourse(string sid, string cid);
    void studentSchedule(const string& sid);
    void courseRoster(string cid);
    void studentCoursesList(const string& sid);
    void studentGrades(const string& sid);

    // 教师相关方法（委托给TeacherBroker）
    void assignTeacherToCourse(string tid, string cid);
    void teacherGradeStudent(string tid, string sid, string cid, double grade);
    void teacherTeachingCourses(string tid);
    void teacherSchedule(const string& tid);

    // 教学秘书相关方法（委托给SecretaryBroker）
    void secretaryCreateCourse(string secid, string cid, string cname);
    void secretaryAssignTeacher(string secid, string tid, string cid);
    void secretarySetSchedule(string secid, string cid, string tid, string timeSlot, string classroom);
    void secretaryScheduledCourses(string secid);
    void secretaryCancelSchedule(string secid, string cid, string timeSlot);

    // 系统初始化
    void initialize();

    // UI交互方法
    int exec();

private:
    Registrar();

    // Broker成员变量（直接创建）
    shared_ptr<StudentBroker> _studentBroker;
    shared_ptr<CourseBroker> _courseBroker;
    shared_ptr<TeacherBroker> _teacherBroker;
    shared_ptr<SecretaryBroker> _secretaryBroker;

    // UI交互处理方法
    void handleStudentLogin(class UI& ui);
    void handleTeacherLogin(class UI& ui);
    void handleSecretaryLogin(class UI& ui);
    void handleStudentFunctions(class UI& ui);
    void handleTeacherFunctions(class UI& ui);
    void handleSecretaryFunctions(class UI& ui);
};

// ----- The implementation of class Registrar -----

Registrar &Registrar::singleton(){
    static Registrar instance;
    return instance;
}

void Registrar::initialize(){
    // 直接创建Broker实例（简化版工厂模式）
    _studentBroker = make_shared<StudentBroker>();
    _courseBroker = make_shared<CourseBroker>();
    _teacherBroker = make_shared<TeacherBroker>();
    _secretaryBroker = make_shared<SecretaryBroker>();

    // 初始化各个Broker
    _studentBroker->initialize();
    _courseBroker->initialize();
    _teacherBroker->initialize();
    _secretaryBroker->initialize();
}

Registrar::Registrar(){}

// ----- 学生相关方法（委托给StudentBroker） -----

void Registrar::studentDropsFromCourse(string sid, string cid){
    Student* student = _studentBroker->findStudentById(sid);
    Course* course = _courseBroker->findCourseById(cid);
    _studentBroker->studentDropsFromCourse(sid, cid, course);
}

void Registrar::studentEnrollsInCourse(string sid, string cid){
    Student* student = _studentBroker->findStudentById(sid);
    Course* course = _courseBroker->findCourseById(cid);
    _studentBroker->studentEnrollsInCourse(sid, cid, course);
}

void Registrar::studentSchedule(const string& sid){
    print("{}", _studentBroker->studentSchedule(sid));
}

void Registrar::studentCoursesList(const string& sid){
    print("{}", _studentBroker->studentCoursesList(sid));
}

void Registrar::courseRoster(string cid){
    print("{}", _courseBroker->courseRoster(cid));
}

void Registrar::studentGrades(const string& sid){
    print("{}", _studentBroker->studentGrades(sid));
}

// ----- Course::roster 的实现 -----
string Course::roster(){
    auto rst = format("{} selected by the students:\n", m_name);
    for (auto s : _students) {
        rst += s->info();
        rst += "\n";
    }
    return rst;
}

// ----- 教师相关方法（委托给TeacherBroker） -----

void Registrar::assignTeacherToCourse(string tid, string cid){
    Teacher* teacher = _teacherBroker->findTeacherById(tid);
    Course* course = _courseBroker->findCourseById(cid);
    _teacherBroker->assignTeacherToCourse(tid, cid, teacher, course);
}

void Registrar::teacherGradeStudent(string tid, string sid, string cid, double grade){
    Teacher* teacher = _teacherBroker->findTeacherById(tid);
    Student* student = _studentBroker->findStudentById(sid);
    Course* course = _courseBroker->findCourseById(cid);
    _teacherBroker->teacherGradeStudent(tid, sid, cid, teacher, student, course, grade);
}

void Registrar::teacherTeachingCourses(string tid){
    print("{}", _teacherBroker->teacherTeachingCourses(tid));
}

void Registrar::teacherSchedule(const string& tid){
    print("{}", _teacherBroker->teacherSchedule(tid));
}

// ----- 教学秘书相关方法（委托给SecretaryBroker） -----

void Registrar::secretaryCreateCourse(string secid, string cid, string cname){
    Secretary* secretary = _secretaryBroker->findSecretaryById(secid);
    Course* course = _courseBroker->createCourse(cid, cname);
    _secretaryBroker->secretaryCreateCourse(secid, cid, cname, secretary, course);
}

void Registrar::secretaryAssignTeacher(string secid, string tid, string cid){
    Secretary* secretary = _secretaryBroker->findSecretaryById(secid);
    Teacher* teacher = _teacherBroker->findTeacherById(tid);
    Course* course = _courseBroker->findCourseById(cid);
    _secretaryBroker->secretaryAssignTeacher(secid, tid, cid, secretary, teacher, course);
}

void Registrar::secretarySetSchedule(string secid, string cid, string tid, string timeSlot, string classroom){
    Secretary* secretary = _secretaryBroker->findSecretaryById(secid);
    Course* course = _courseBroker->findCourseById(cid);
    Teacher* teacher = _teacherBroker->findTeacherById(tid);
    _secretaryBroker->secretarySetSchedule(secid, cid, tid, timeSlot, classroom, secretary, course, teacher);
}

void Registrar::secretaryScheduledCourses(string secid){
    print("{}", _secretaryBroker->secretaryScheduledCourses(secid));
}

void Registrar::secretaryCancelSchedule(string secid, string cid, string timeSlot){
    Secretary* secretary = _secretaryBroker->findSecretaryById(secid);
    Course* course = _courseBroker->findCourseById(cid);
    _secretaryBroker->secretaryCancelSchedule(secid, cid, timeSlot, secretary, course);
}


// ----- UI交互实现 -----

int Registrar::exec()
{
    auto& ui = UI::singleton();

    while (true) {
        ui.showMainMenu();

        int choice = ui.getMenuChoice("请选择登录类型 (0-3): ", 3);

        switch (choice) {
            case 1:
                handleStudentLogin(ui);
                break;
            case 2:
                handleTeacherLogin(ui);
                break;
            case 3:
                handleSecretaryLogin(ui);
                break;
            case 0:
                print("感谢使用教务管理系统，再见！\n");
                return 0;
            default:
                ui.showError("无效选择，请重新输入！");
                ui.pauseScreen();
                break;
        }
    }
    return 0;
}

void Registrar::handleStudentLogin(UI& ui)
{
    ui.clearScreen();
    print("========================================\n");
    print("          学生登录\n");
    print("========================================\n");

    string studentId = ui.getUserInput("请输入学生ID (输入0返回主菜单): ");

    if (studentId == "0") {
        return;
    }

    if (_studentBroker->findStudentById(studentId)) {
        ui.setCurrentUser("student", studentId);
        ui.showMessage("登录成功！欢迎，学生 " + studentId);
        ui.pauseScreen();
        handleStudentFunctions(ui);
    } else {
        ui.showError("学生ID不存在或密码错误。");
        ui.pauseScreen();
    }
}

void Registrar::handleTeacherLogin(UI& ui)
{
    ui.clearScreen();
    print("========================================\n");
    print("          教师登录\n");
    print("========================================\n");

    string teacherId = ui.getUserInput("请输入教师ID (输入0返回主菜单): ");

    if (teacherId == "0") {
        return;
    }

    if (_teacherBroker->findTeacherById(teacherId)) {
        ui.setCurrentUser("teacher", teacherId);
        ui.showMessage("登录成功！欢迎，教师 " + teacherId);
        ui.pauseScreen();
        handleTeacherFunctions(ui);
    } else {
        ui.showError("教师ID不存在或密码错误。");
        ui.pauseScreen();
    }
}

void Registrar::handleSecretaryLogin(UI& ui)
{
    ui.clearScreen();
    print("========================================\n");
    print("        教学秘书登录\n");
    print("========================================\n");

    string secretaryId = ui.getUserInput("请输入教学秘书ID (输入0返回主菜单): ");

    if (secretaryId == "0") {
        return;
    }

    if (_secretaryBroker->findSecretaryById(secretaryId)) {
        ui.setCurrentUser("secretary", secretaryId);
        ui.showMessage("登录成功！欢迎，教学秘书 " + secretaryId);
        ui.pauseScreen();
        handleSecretaryFunctions(ui);
    } else {
        ui.showError("教学秘书ID不存在或密码错误。");
        ui.pauseScreen();
    }
}

void Registrar::handleStudentFunctions(UI& ui)
{
    while (ui.getCurrentUserType() == "student") {
        ui.showStudentMenu();

        int choice = ui.getMenuChoice("请选择功能 (0-5): ", 5);

        switch (choice) {
            case 1:
                ui.clearScreen();
                print("========================================\n");
                print("        课程列表\n");
                print("========================================\n");
                studentCoursesList(ui.getCurrentUserId());
                ui.pauseScreen();
                break;

            case 2:
                ui.clearScreen();
                print("========================================\n");
                print("        个人课表\n");
                print("========================================\n");
                studentSchedule(ui.getCurrentUserId());
                ui.pauseScreen();
                break;

            case 3:
                ui.clearScreen();
                print("========================================\n");
                print("        成绩查询\n");
                print("========================================\n");
                studentGrades(ui.getCurrentUserId());
                ui.pauseScreen();
                break;

            case 4:
                {
                    ui.clearScreen();
                    print("========================================\n");
                    print("        选修课程\n");
                    print("========================================\n");
                    string courseId = ui.getUserInput("请输入要选修的课程ID: ");
                    studentEnrollsInCourse(ui.getCurrentUserId(), courseId);
                    ui.showMessage("选课操作完成。");
                    ui.pauseScreen();
                }
                break;

            case 5:
                {
                    ui.clearScreen();
                    print("========================================\n");
                    print("        取消选课\n");
                    print("========================================\n");
                    string courseId = ui.getUserInput("请输入要取消的课程ID: ");
                    studentDropsFromCourse(ui.getCurrentUserId(), courseId);
                    ui.showMessage("取消选课操作完成。");
                    ui.pauseScreen();
                }
                break;

            case 0:
                ui.clearCurrentUser();
                return;
        }
    }
}

void Registrar::handleTeacherFunctions(UI& ui)
{
    while (ui.getCurrentUserType() == "teacher") {
        ui.showTeacherMenu();

        int choice = ui.getMenuChoice("请选择功能 (0-4): ", 4);

        switch (choice) {
            case 1:
                ui.clearScreen();
                print("========================================\n");
                print("        授课课程列表\n");
                print("========================================\n");
                teacherTeachingCourses(ui.getCurrentUserId());
                ui.pauseScreen();
                break;

            case 2:
                ui.clearScreen();
                print("========================================\n");
                print("        个人课表\n");
                print("========================================\n");
                teacherSchedule(ui.getCurrentUserId());
                ui.pauseScreen();
                break;

            case 3:
                {
                    ui.clearScreen();
                    print("========================================\n");
                    print("        查看课程学生名单\n");
                    print("========================================\n");
                    string courseId = ui.getUserInput("请输入课程ID: ");
                    courseRoster(courseId);
                    ui.pauseScreen();
                }
                break;

            case 4:
                {
                    ui.clearScreen();
                    print("========================================\n");
                    print("        给学生打分\n");
                    print("========================================\n");
                    string studentId = ui.getUserInput("请输入学生ID: ");
                    string courseId = ui.getUserInput("请输入课程ID: ");
                    print("请输入成绩: ");
                    double grade;
                    std::cin >> grade;
                    teacherGradeStudent(ui.getCurrentUserId(), studentId, courseId, grade);
                    ui.showMessage("打分操作完成。");
                    ui.pauseScreen();
                }
                break;

            case 0:
                ui.clearCurrentUser();
                return;
        }
    }
}

void Registrar::handleSecretaryFunctions(UI& ui)
{
    while (ui.getCurrentUserType() == "secretary") {
        ui.showSecretaryMenu();

        int choice = ui.getMenuChoice("请选择功能 (0-5): ", 5);

        switch (choice) {
            case 1:
                {
                    ui.clearScreen();
                    print("========================================\n");
                    print("        创建课程\n");
                    print("========================================\n");
                    string courseId = ui.getUserInput("请输入课程ID: ");
                    string courseName = ui.getUserInput("请输入课程名称: ");
                    secretaryCreateCourse(ui.getCurrentUserId(), courseId, courseName);
                    ui.showMessage("课程创建操作完成。");
                    ui.pauseScreen();
                }
                break;

            case 2:
                {
                    ui.clearScreen();
                    print("========================================\n");
                    print("        分配教师到课程\n");
                    print("========================================\n");
                    string teacherId = ui.getUserInput("请输入教师ID: ");
                    string courseId = ui.getUserInput("请输入课程ID: ");
                    secretaryAssignTeacher(ui.getCurrentUserId(), teacherId, courseId);
                    ui.showMessage("教师分配操作完成。");
                    ui.pauseScreen();
                }
                break;

            case 3:
                {
                    ui.clearScreen();
                    print("========================================\n");
                    print("        设置课程时间表\n");
                    print("========================================\n");
                    string courseId = ui.getUserInput("请输入课程ID: ");
                    string teacherId = ui.getUserInput("请输入教师ID: ");
                    print("请输入时间段: ");
                    string timeSlot;
                    std::cin.ignore();
                    std::getline(std::cin, timeSlot);
                    string classroom = ui.getUserInput("请输入教室: ");
                    secretarySetSchedule(ui.getCurrentUserId(), courseId, teacherId, timeSlot, classroom);
                    ui.showMessage("时间表设置操作完成。");
                    ui.pauseScreen();
                }
                break;

            case 4:
                ui.clearScreen();
                print("========================================\n");
                print("        已排课程列表\n");
                print("========================================\n");
                secretaryScheduledCourses(ui.getCurrentUserId());
                ui.pauseScreen();
                break;

            case 5:
                {
                    ui.clearScreen();
                    print("========================================\n");
                    print("        取消排课\n");
                    print("========================================\n");
                    string courseId = ui.getUserInput("请输入要取消排课的课程ID: ");
                    print("请输入要取消的时间段: ");
                    string timeSlot;
                    std::cin.ignore();
                    std::getline(std::cin, timeSlot);
                    secretaryCancelSchedule(ui.getCurrentUserId(), courseId, timeSlot);
                    ui.showMessage("取消排课操作完成。");
                    ui.pauseScreen();
                }
                break;

            case 0:
                ui.clearCurrentUser();
                return;
        }
    }
}
