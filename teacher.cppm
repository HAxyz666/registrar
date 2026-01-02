// Module
// File: teacher.cppm   Version: 0.1.0   License: AGPLv3
// Created: DaiSen Zhou   3563248115@qq.com   2026-01-02 14:01:10
// Description:
//     The interface and implementation of Teacher class are logically separated.

export module registrar:teacher;

import std;
import :course;
import :student;

using std::string; using std::vector;

export class Teacher
{
public:
    Teacher(string id, string name);
    
    string info();
    bool hasId(string id);
    void assignCourse(class Course* course);
    void gradeStudent(class Student* student, class Course* course, double grade);
    string getTeachingCourses();
    
private:
    string m_name;
    string m_id;
    static int tm_totalCount;  // static data member
    
    vector<class Course*> _assignedCourses;
};

// ----- Partial implementation of class Teacher -----

int Teacher::tm_totalCount = 0; // initialize static data memeber

Teacher::Teacher(string id, string name)
    : m_name(name)
    , m_id(id)
{
    tm_totalCount++;
}

string Teacher::info()
{
    return format("{}   {}", m_id, m_name);
}

bool Teacher::hasId(string id)
{
    return id == m_id;
}

void Teacher::assignCourse(Course* course)
{
    _assignedCourses.push_back(course);
    print("教师 {} 已被分配课程: {}", m_name, course->info());
}

void Teacher::gradeStudent(Student* student, Course* course, double grade)
{
    student->addGrade(course, grade);
    print("教师 {} 为学生 {} 的课程 {} 评定成绩: {}", m_name, student->info(), course->info(), grade);
}

string Teacher::getTeachingCourses()
{
    auto s = format("{} 的教学任务:", m_name);
    for (auto& course : _assignedCourses) {
        s += course->info();
    }
    return s;
}
