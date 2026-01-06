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
    void setCourseSchedule(class Course* course, class Teacher* teacher, string timeSlot, string classroom);
    string getScheduledCourses();
    bool checkTimeConflict(class Teacher* teacher, string timeSlot);
    bool checkClassroomConflict(string timeSlot, string classroom);
    void cancelCourseSchedule(Course* course, string timeSlot);

private:
    string m_name;
    string m_id;
    static int sm_totalCount;  // static data member

    vector<class Course*> _createdCourses;
    
    friend class DatabaseManager;
    friend class Registrar;
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

void Secretary::setCourseSchedule(Course* course, Teacher* teacher, string timeSlot, string classroom)
{
    // 检查教室冲突
    if (checkClassroomConflict(timeSlot, classroom)) {
        print("错误: 教室 {} 在时间段 {} 已被其他课程占用！\n", classroom, timeSlot);
        return;
    }
    
    // 检查教师时间冲突
    if (checkTimeConflict(teacher, timeSlot)) {
        print("错误: 教师 {} 在时间段 {} 已有其他课程安排！\n", teacher->info(), timeSlot);
        return;
    }
    
    // 如果没有冲突，则安排课程
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

bool Secretary::checkTimeConflict(Teacher* teacher, string timeSlot)
{
    // 检查教师是否在同一时间段有其他课程
    for (auto& assignedCourse : teacher->_assignedCourses) {
        for (auto& schedule : assignedCourse->m_roomandtime) {
            if (schedule.timeSlot == timeSlot) {
                return true; // 发现时间冲突
            }
        }
    }
    return false; // 没有时间冲突
}

bool Secretary::checkClassroomConflict(string timeSlot, string classroom)
{
    // 检查同一教室在同一时间段是否有其他课程
    for (auto& course : _createdCourses) {
        for (auto& schedule : course->m_roomandtime) {
            if (schedule.timeSlot == timeSlot && schedule.classroom == classroom) {
                return true; // 发现教室冲突
            }
        }
    }
    return false; // 没有教室冲突
}
void Secretary::cancelCourseSchedule(Course* course, string timeSlot)
{
    // 在课程的时间安排中查找并删除指定时间段的安排
    auto it = std::remove_if(course->m_roomandtime.begin(),
                             course->m_roomandtime.end(),
        [&](const RoomAndTime& schedule) {
            return schedule.timeSlot == timeSlot;
        });

    if (it != course->m_roomandtime.end()) {
        course->m_roomandtime.erase(it, course->m_roomandtime.end());
        print("成功取消课程 {} 在时间段 {} 的安排\n",
              course->info(), timeSlot);
    } else {
        print("错误: 课程 {} 在时间段 {} 没有安排\n",
              course->info(), timeSlot);
    }
}
