// Module
// File: student.cppm   Version: 0.1.0   License: AGPLv3
// Created: DaiSen Zhou   3563248115@qq.com   2026-01-02 13:42:36
// Description:
//     The interface and implementation of Student class are logically separated.

export module registrar:student;
import std;

using std::string; using std::vector;

export class Student
{
public:
    Student(string id, string name);

    void enrollsIn(class Course* course);
    string schedule();
    string info();
    bool hasId(string id);

private:
    string m_name;
    string m_id;

    vector<class Course*> _courses;
};

// ----- Partial implementation of class Student -----

Student::Student(string id, string name)
    : m_name(name)
    , m_id(id)
{}

string Student::info()
{
    return format("{}   {}\n", m_id, m_name);
}

bool Student::hasId(string id)
{
    return id == m_id;
}
