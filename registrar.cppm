// Module
// File: registrar.cppm   Version: 0.1.0   License: AGPLv3
// Created: DaiSen Zhou   3563248115@qq.com   2026-01-02 13:41:55
// Description:
//     The interface and implementation of Register class are logically separated.

export module registrar;
export import :student;
export import :course;
export import :teacher;

import std;
using std::string; using std::vector;

export class Registrar
{
public:
    static Registrar& singleton();  //static function member
    void studentEnrollsInCourse(string sid, string cid);
    void studentSchedule(const string& sid);
    void courseRoster(string cid);
    void initialize();
    void assignTeacherToCourse(string tid, string cid);
    void teacherGradeStudent(string tid, string sid, string cid, double grade);
    void teacherTeachingCourses(string tid);
    void studentGrades(const string& sid);
    void studentCoursesList(const string& sid);

private:
    Registrar();  // Prohibit creating objects directly
    class Student* findStudentById(const string& id);
    class Course* findCourseById(const string& id);
    class Teacher* findTeacherById(const string& id);

    vector<class Course*> _courses;
    vector<class Student*> _students;
    vector<class Teacher*> _teachers;
};

// ----- The implementaion of class Registrar -----
Registrar &Registrar::singleton(){
    static Registrar instance;
    return instance;
}

void Registrar::studentEnrollsInCourse(string sid, string cid){
    Student* student = findStudentById(sid);
    Course* course = findCourseById(cid);

    if (student && course) {
        student->enrollsIn(course);
    }
}

// void Registrar::studentSchedule(const string &sid)
// {
//     auto s = findStudentById(sid);
//     print("{}\n",s->schedule());
// }

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

void Registrar::studentGrades(const string& sid){
    auto s = findStudentById(sid);
    if (s) {
        print("{}\n", s->getGrades());
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
