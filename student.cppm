// Module
// File: student.cppm   Version: 0.1.0   License: AGPLv3
// Created: DaiSen Zhou   3563248115@qq.com   2026-01-02 13:42:36
// Description:
//     The interface and implementation of Student class are logically separated.

export module registrar:student;
import std;
import :course;

using std::string; using std::vector; using std::pair; using std::format;

export class Student
{
public:
    Student(string id, string name);

    void enrollsIn(class Course* course);
    string coursesList();
    string info();
    bool hasId(string id);
    void addGrade(class Course* course, double grade);
    string getGrades();
    double getGradeForCourse(class Course* course);
    string schedule();

private:
    string m_name;
    string m_id;

    vector<class Course*> _courses;
    vector<pair<class Course*, double>> _grades; // 保存课程和成绩的映射
};

// ----- Partial implementation of class Student -----

Student::Student(string id, string name)
    : m_name(name)
    , m_id(id)
{}

string Student::info()
{
    return format("{}   {}", m_id, m_name);
}

bool Student::hasId(string id)
{
    return id == m_id;
}

void Student::addGrade(Course* course, double grade)
{
    // 检查是否已存在该课程的成绩，如果存在则更新
    for (auto& gradeEntry : _grades) {
        if (gradeEntry.first == course) {
            gradeEntry.second = grade;
            return;
        }
    }
    // 如果不存在则添加新的成绩记录
    _grades.push_back({course, grade});
}

string Student::getGrades()
{
    auto s = format("{} 的成绩:\n", m_name);
    for (auto& gradeEntry : _grades) {
        s += format("{} 成绩: {}\n", gradeEntry.first->info(), gradeEntry.second);
    }
    return s;
}

double Student::getGradeForCourse(Course* course)
{
    for (auto& gradeEntry : _grades) {
        if (gradeEntry.first == course) {
            return gradeEntry.second;
        }
    }
    return -1.0; // 表示未评定成绩
}

string Student::schedule()
{
    auto s = format("{} 的课表:\n", m_name);
    for (auto& course : _courses) {
        s += format("{}   {}\n", course->info(), course->getScheduleInfo());
    }
    return s;
}
