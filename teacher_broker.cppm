// =======================================
// Module
// File: teacher_broker.cppm   Version: 0.1.0   License: AGPLv3
// Created: lichao2024051604024  3042525170@qq.com   2026-01-09 22:30:41
// Description:
//     教师代理类：处理教师相关的业务逻辑

export module registrar:teacher_broker;

import std;
import :broker_factory;
import :teacher;
import :course;
import :student;
import :database_manager;

using std::string;
using std::vector;

export class TeacherBroker : public IBroker {
public:
    TeacherBroker() = default;
    ~TeacherBroker() = default;
    void initialize() override;
    Teacher* findTeacherById(const string& id);
    void assignTeacherToCourse(string tid, string cid, Teacher* teacher, Course* course);
    void teacherGradeStudent(string tid, string sid, string cid, Teacher* teacher, Student* student, Course* course, double grade);
    string teacherTeachingCourses(const string& tid);
    string teacherSchedule(const string& tid);

private:
    vector<Teacher*> _teachers;
    void loadFromDatabase();
};

void TeacherBroker::initialize()
{
    // 尝试从数据库加载数据
    loadFromDatabase();
    
    // 如果数据库中没有数据或连接失败，使用默认数据
    if (_teachers.empty()) {
        _teachers.push_back(new Teacher("T001", "Dr. Smith"));
        _teachers.push_back(new Teacher("T002", "Dr. Johnson"));
        _teachers.push_back(new Teacher("T003", "Dr. Brown"));
    }
}

void TeacherBroker::loadFromDatabase()
{
    auto& db = DatabaseManager::singleton();
    if (!db.isConnected()) {
        return;
    }
    
    string query = "SELECT id, name FROM teachers ORDER BY id;";
    auto results = db.executeSelect(query);
    
    for (const auto& row : results) {
        if (row.size() >= 2) {
            _teachers.push_back(new Teacher(row[0], row[1]));
        }
    }
    
    if (!_teachers.empty()) {
        std::print("从数据库加载了 {} 个教师记录\n", _teachers.size());
    }
}

Teacher* TeacherBroker::findTeacherById(const string& id)
{
    for (auto* teacher : _teachers) {
        if (teacher->hasId(id)) {
            return teacher;
        }
    }
    return nullptr;
}

void TeacherBroker::assignTeacherToCourse(string tid, string cid, Teacher* teacher, Course* course)
{
    if (teacher && course) {
        teacher->assignCourse(course);
    } else {
        std::print("错误: 教师或课程不存在！\n");
    }
}

void TeacherBroker::teacherGradeStudent(string tid, string sid, string cid, Teacher* teacher, Student* student, Course* course, double grade)
{
    if (teacher && student && course) {
        teacher->gradeStudent(student, course, grade);
    } else {
         std::print("错误: 教师、学生或课程不存在！\n");
    }
}

string TeacherBroker::teacherTeachingCourses(const string& tid) {
    Teacher* teacher = findTeacherById(tid);
    if (teacher) {
        return teacher->getTeachingCourses();
    }
    return "教师不存在！\n";
}

string TeacherBroker::teacherSchedule(const string& tid) {
    Teacher* teacher = findTeacherById(tid);
    if (teacher) {
        return teacher->schedule();
    }
    return "教师不存在！\n";
}
