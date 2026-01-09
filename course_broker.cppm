// =======================================
// Module
// File: course_broker.cppm   Version: 0.1.0   License: AGPLv3
// Created: lichao2024051604024  3042525170@qq.com   2026-01-09 22:29:59
// Description:
//     课程代理类：处理课程相关的业务逻辑

export module registrar:course_broker;

import std;
import :broker_factory;
import :course;

using std::string;
using std::vector;

export class CourseBroker : public IBroker {
public:
    CourseBroker() = default;
    ~CourseBroker() = default;
    void initialize() override;
    Course* findCourseById(const string& id);
    Course* createCourse(const string& cid, const string& cname);
    string courseRoster(const string& cid);

private:
    vector<Course*> _courses;

};

void CourseBroker::initialize()
{
    _courses.push_back(new Course("CS101", "C Programming"));
    _courses.push_back(new Course("CS201", "Data structure"));
    _courses.push_back(new Course("MATH101", "Advanced Math"));
}

Course* CourseBroker::findCourseById(const string& id)
{
    for (auto* course : _courses) {
        if (course->hasId(id)) {
            return course;
        }
    }
    return nullptr;
}

Course* CourseBroker::createCourse(const string& cid, const string& cname)
{
    Course* course = findCourseById(cid);
    if (course) {
        std::print("警告: 课程 {} 已存在！\n", cid);
        return course;
    }
    course = new Course(cid, cname);
    _courses.push_back(course);
    return course;
}

string CourseBroker::courseRoster(const string& cid)
{
    Course* course = findCourseById(cid);
    if (course) {
        return course->roster();
    }
    return "课程不存在！\n";
}
