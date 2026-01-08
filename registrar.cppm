// Module
// File: registrar.cppm   Version: 0.1.0   License: AGPLv3
// Created: DaiSen Zhou   3563248115@qq.com   2026-01-02 13:41:55
// Description:
//     The interface and implementation of Register class are logically separated.


//  [v1.0] chao li (3042525170@qq.com)   2026-01-04
//         * added:secretary;

export module registrar;
export import :student;
export import :course;
export import :teacher;
export import :secretary;
export import :ui;

import std;
using std::string; using std::vector;

export class Registrar
{
public:
    static Registrar& singleton();  //static function member
    void studentDropsFromCourse(string sid, string cid);
    void studentEnrollsInCourse(string sid, string cid);
    void studentSchedule(const string& sid);
    void courseRoster(string cid);
    void initialize();
    void assignTeacherToCourse(string tid, string cid);
    void teacherGradeStudent(string tid, string sid, string cid, double grade);
    void teacherTeachingCourses(string tid);
    void teacherSchedule(const string& tid);
    void studentGrades(const string& sid);
    void studentCoursesList(const string& sid);
    //secretary function
    void secretaryCreateCourse(string secid, string cid, string cname);
    void secretaryAssignTeacher(string secid, string tid, string cid);
    void secretarySetSchedule(string secid, string cid, string tid, string timeSlot, string classroom);
    void secretaryScheduledCourses(string secid);
    void secretaryCancelSchedule(string secid, string cid, string timeSlot);
    
    // UI交互方法
    int exec();

private:
    Registrar();  // Prohibit creating objects directly
    class Student* findStudentById(const string& id);
    class Course* findCourseById(const string& id);
    class Teacher* findTeacherById(const string& id);
    class Secretary* findSecretaryById(const string& id);

    vector<class Course*> _courses;
    vector<class Student*> _students;
    vector<class Teacher*> _teachers;
    vector<class Secretary*> _secretaries;
    
    // UI交互处理方法
    void handleStudentLogin(class UI& ui);
    void handleTeacherLogin(class UI& ui);
    void handleSecretaryLogin(class UI& ui);
    void handleStudentFunctions(class UI& ui);
    void handleTeacherFunctions(class UI& ui);
    void handleSecretaryFunctions(class UI& ui);
};

// ----- The implementaion of class Registrar -----
Registrar &Registrar::singleton(){
    static Registrar instance;
    return instance;
}

void Registrar::studentDropsFromCourse(string sid, string cid){
    Student* student = findStudentById(sid);
    Course* course = findCourseById(cid);

    if (student && course) {
        student->dropsFrom(course);
    } else {
        std::print("错误: 学生或课程不存在！\n");
    }
}

void Registrar::studentEnrollsInCourse(string sid, string cid){
    Student* student = findStudentById(sid);
    Course* course = findCourseById(cid);

    if (student && course) {
        student->enrollsIn(course);
    }
}

void Registrar::studentSchedule(const string &sid)
{
    auto s = findStudentById(sid);
    if (s) {
        print("{}\n", s->schedule());
    }
}

void Registrar::studentCoursesList(const string &sid)
{
    auto s = findStudentById(sid);
    print("{}\n",s->coursesList());
}

void Registrar::courseRoster(string cid){
    auto c = findCourseById(cid);
    print("{}\n", c->roster());
}

void Registrar::initialize(){
    _students.push_back(new Student("S001", "Thomas"));
    _students.push_back(new Student("S002", "Jerry"));
    _students.push_back(new Student("S003", "Baker"));
    _students.push_back(new Student("S004", "Tom"));
    _students.push_back(new Student("S005", "Musk"));

    _courses.push_back(new Course("CS101", "C Programming"));
    _courses.push_back(new Course("CS201", "Data structure"));
    _courses.push_back(new Course("MATH101", "Advanced Math"));

    _teachers.push_back(new Teacher("T001", "Dr. Smith"));
    _teachers.push_back(new Teacher("T002", "Dr. Johnson"));
    _teachers.push_back(new Teacher("T003", "Dr. Brown"));

    _secretaries.push_back(new Secretary("SEC001", "Ms. Wang"));
    _secretaries.push_back(new Secretary("SEC002", "Mr. Li"));
}

Registrar::Registrar(){}

Student *Registrar::findStudentById(const string &id){
    for (auto& student :_students) {
        if (student->hasId(id))
            return student;
    }
    return nullptr;
}

Course *Registrar::findCourseById(const string &id){
    for (auto& course : _courses) {
        if (course->hasId(id) )
            return course;
    }
    return nullptr;
}

Teacher *Registrar::findTeacherById(const string& id){
    for (auto& teacher : _teachers) {
        if (teacher->hasId(id))
            return teacher;
    }
    return nullptr;
}

Secretary *Registrar::findSecretaryById(const string& id){
    for (auto& secretary : _secretaries) {
        if (secretary->hasId(id))
            return secretary;
    }
    return nullptr;
}

void Registrar::assignTeacherToCourse(string tid, string cid){
    Teacher* teacher = findTeacherById(tid);
    Course* course = findCourseById(cid);

    if (teacher && course) {
        teacher->assignCourse(course);
    }
}

void Registrar::teacherGradeStudent(string tid, string sid, string cid, double grade){
    Teacher* teacher = findTeacherById(tid);
    Student* student = findStudentById(sid);
    Course* course = findCourseById(cid);

    if (teacher && student && course) {
        teacher->gradeStudent(student, course, grade);
    }
}

void Registrar::teacherTeachingCourses(string tid){
    Teacher* teacher = findTeacherById(tid);
    if (teacher) {
        print("{}\n", teacher->getTeachingCourses());
    }
}

void Registrar::teacherSchedule(const string& tid){
    Teacher* teacher = findTeacherById(tid);
    if (teacher) {
        print("{}\n", teacher->schedule());
    }
}

void Registrar::studentGrades(const string& sid){
    auto s = findStudentById(sid);
    if (s) {
        print("{}\n", s->getGrades());
    }
}
//..........add seretary function...............

void Registrar::secretaryCreateCourse(string secid, string cid, string cname){
    Secretary* secretary = findSecretaryById(secid);
    Course* course = findCourseById(cid);

    if (secretary) {
        if (course) {
            print("警告: 课程 {} 已存在！\n", cid);
            secretary->createCourse(course);
        } else {
            // 创建新课程
            course = new Course(cid, cname);
            _courses.push_back(course);
            secretary->createCourse(course);
            print("课程 {} 创建成功！\n", cid);
        }
    } else {
        print("错误: 教学秘书 {} 不存在！\n", secid);
    }
}

void Registrar::secretaryAssignTeacher(string secid, string tid, string cid){
    Secretary* secretary = findSecretaryById(secid);
    Teacher* teacher = findTeacherById(tid);
    Course* course = findCourseById(cid);

    if (secretary && teacher && course) {
        secretary->assignTeacherToCourse(teacher, course);
    } else {
        std::print("错误: 教学秘书、教师或课程不存在！\n");
    }
}

void Registrar::secretarySetSchedule(string secid, string cid, string tid, string timeSlot, string classroom){
    Secretary* secretary = findSecretaryById(secid);
    Course* course = findCourseById(cid);
    Teacher* teacher = findTeacherById(tid);

    if (secretary && course && teacher) {
        secretary->setCourseSchedule(course, teacher, timeSlot, classroom);
    } else {
        std::print("错误: 教学秘书、课程或教师不存在！\n");
    }
}

void Registrar::secretaryScheduledCourses(string secid){
    Secretary* secretary = findSecretaryById(secid);
    if (secretary) {
        print("{}\n", secretary->getScheduledCourses());
    }
}

void Registrar::secretaryCancelSchedule(string secid, string cid, string timeSlot){
    Secretary* secretary = findSecretaryById(secid);
    Course* course = findCourseById(cid);

    if (secretary && course) {
        secretary->cancelCourseSchedule(course, timeSlot);
    } else {
        std::print("错误: 教学秘书或课程不存在！\n");
    }
}

// ----- Implementation of Student-Course object interactions -----

void Student::enrollsIn(Course *course){
    if(course->acceptEnrollment(this))
        _courses.push_back(course);
}

string Course::roster(){
    auto rst = format("{} selected by the students:\n", m_name);
    for (auto s : _students) {
        rst += s->info(); // 课程对象委托学生对象自己输出相关信息
        rst += "\n";
    }
    return rst;
}

string Student::coursesList()
{
    auto s = format("{}'s courses:\n", m_name);
    for(auto &c: _courses){
        s += c->info();
        s += "\n";
    }
    return s;
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
    
    if (findStudentById(studentId)) {
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
    
    if (findTeacherById(teacherId)) {
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
    
    if (findSecretaryById(secretaryId)) {
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
