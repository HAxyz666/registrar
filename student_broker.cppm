// =======================================
// Module
// File: student_broker.cppm   Version: 0.1.0   License: AGPLv3
// Created: lichao2024051604024  3042525170@qq.com   2026-01-09 22:30:28
// Description:
//     学生代理类：处理学生相关的业务逻辑

export module registrar:student_broker;

import std;
import :broker_factory;
import :student;
import :course;

using std::string;
using std::vector;
using std::shared_ptr;
using std::make_shared;

export class StudentBroker : public IBroker {

public:
    StudentBroker() = default;
    ~StudentBroker() = default;
    void initialize() override;
    Student* findStudentById(const string& id);
    void studentEnrollsInCourse(string sid, string cid, Course* course);
    void studentDropsFromCourse(string sid, string cid, Course* course);
    string studentSchedule(const string& sid);
    string studentCoursesList(const string& sid);
    string studentGrades(const string& sid);

private:
    vector<Student*> _students;

};

void StudentBroker::initialize()
{
    _students.push_back(new Student("S001", "Thomas"));
    _students.push_back(new Student("S002", "Jerry"));
    _students.push_back(new Student("S003", "Baker"));
    _students.push_back(new Student("S004", "Tom"));
    _students.push_back(new Student("S005", "Musk"));
}

Student* StudentBroker::findStudentById(const string& id)
{
    for (auto* student : _students) {
       if (student->hasId(id)) {
              return student;
           }
       }
       return nullptr;
}

void StudentBroker::studentEnrollsInCourse(string sid, string cid, Course* course)
{
    Student* student = findStudentById(sid);
    if (student && course) {
        student->enrollsIn(course);
    } else {
        std::print("错误: 学生或课程不存在！\n");
    }
}

void StudentBroker::studentDropsFromCourse(string sid, string cid, Course* course)
{
    Student* student = findStudentById(sid);
    if (student && course) {
        student->dropsFrom(course);
    } else {
        std::print("错误: 学生或课程不存在！\n");
    }
}

string StudentBroker::studentSchedule(const string& sid)
{
    Student* student = findStudentById(sid);
    if (student) {
        return student->schedule();
    }
    return "学生不存在！\n";
}

string StudentBroker::studentCoursesList(const string& sid)
{
    Student* student = findStudentById(sid);
    if (student) {
        return student->coursesList();
    }
    return "学生不存在！\n";
}

string StudentBroker::studentGrades(const string& sid)
{
    Student* student = findStudentById(sid);
    if (student) {
        return student->getGrades();
    }
    return "学生不存在！\n";
}
