// =======================================
// Module
// File: secretar.cppm   Version: 0.1.0   License: AGPLv3
// Created: lichao2024051604024  3042525170@qq.com   2026-01-04 20:24:19
// Description:assignCourse,setcoursr
//

//  [v1.0] chao li (3042525170@qq.com)   2026-01-04
//         * added:setcourse,assigncouse;
export module registrar:secretary;

import std;
import :course;
import :teacher;

using std::string; using std::vector;

export class Secretary
{
public:
    Secretary(string id, string name);

    string info();
    bool hasId(string id);
    void createCourse(class Course* course);
    void assignTeacherToCourse(class Teacher* teacher, class Course* course);
    void setCourseSchedule(class Course* course, string timeSlot, string classroom);
    string getScheduledCourses();

private:
    string m_name;
    string m_id;
    static int sm_totalCount;  // static data member

    vector<class Course*> _createdCourses;
};

// ----- Partial implementation of class Secretary -----

int Secretary::sm_totalCount = 0; // initialize static data memeber

Secretary::Secretary(string id, string name)
    : m_name(name)
    , m_id(id)
{
    sm_totalCount++;
}

string Secretary::info()
{
    return format("{}   {}", m_id, m_name);
}

bool Secretary::hasId(string id)
{
    return id == m_id;
}

void Secretary::createCourse(Course* course)
{
    _createdCourses.push_back(course);
    print("教学秘书 {} 创建了课程: {}\n", m_name, course->info());
}

void Secretary::assignTeacherToCourse(Teacher* teacher, Course* course)
{
    teacher->assignCourse(course);
    print("教学秘书 {} 安排教师 {} 教授课程: {}\n", m_name, teacher->info(), course->info());
}

void Secretary::setCourseSchedule(Course* course, string timeSlot, string classroom)
{
    course->m_roomandtime.emplace_back(timeSlot, classroom);
    print("教学秘书 {} 为课程 {} 安排时间: {}, 教室: {}\n", m_name, course->info(), timeSlot, classroom);
}

string Secretary::getScheduledCourses()
{
    auto s = format("{} 安排的课程:\n", m_name);
    for (auto& course : _createdCourses) {
        s += course->info();
        s += "\n";
    }
    return s;
}
