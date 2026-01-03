// Module
// File: main.cpp   Version: 0.1.0   License: AGPLv3
// Created: DaiSen Zhou   3563248115@qq.com   2026-01-02 13:39:09
// Description:
//      start of registrar system
import registrar;
import std;

int main()
{
    // alias for static function Registrar::singleton
    auto system = &Registrar::singleton;

    system().initialize();

    // Simulate some students erolls in some courses
    system().studentEnrollsInCourse("S001", "CS101");
    system().studentEnrollsInCourse("S001", "CS201");
    system().studentEnrollsInCourse("S001", "MATH101");

    system().studentEnrollsInCourse("S002", "CS101");
    system().studentEnrollsInCourse("S002", "MATH101");

    system().studentEnrollsInCourse("S003", "CS201");
    system().studentEnrollsInCourse("S003", "MATH101");


    system().studentEnrollsInCourse("S004", "CS101");
    system().studentEnrollsInCourse("S004", "CS201");

    system().studentEnrollsInCourse("S005", "CS201");

    std::println();

    // Print course rosters
    system().courseRoster("CS101");
    system().courseRoster("CS201");
    system().courseRoster("MATH101");

    std::println();

    // Print student schedules
    system().studentCoursesList("S001");
    system().studentCoursesList("S003");

    std::println();

    // Assign teachers to courses
    // 测试：一名教师教授多门课程
    system().assignTeacherToCourse("T001", "CS101");
    system().assignTeacherToCourse("T001", "MATH101");  // T001教授两门课程
    system().assignTeacherToCourse("T002", "CS201");
    system().assignTeacherToCourse("T003", "MATH101");

    std::println();

    // Print teacher teaching courses
    system().teacherTeachingCourses("T001");
    system().teacherTeachingCourses("T002");
    system().teacherTeachingCourses("T003");

    std::println();

    // Teachers grade students
    // 测试：T001教授两门课程，为两门课程打分
    system().teacherGradeStudent("T001", "S001", "CS101", 95.5);
    system().teacherGradeStudent("T001", "S002", "CS101", 88.0);
    system().teacherGradeStudent("T001", "S001", "MATH101", 90.0);
    system().teacherGradeStudent("T001", "S003", "MATH101", 78.5);
    
    // T002为CS201打分
    system().teacherGradeStudent("T002", "S001", "CS201", 92.0);
    system().teacherGradeStudent("T002", "S003", "CS201", 85.5);
    
    // T003为MATH101打分
    system().teacherGradeStudent("T003", "S001", "MATH101", 89.0);
    
    // 测试：无权限打分 - T002试图为CS101打分（T002没有教授CS101）
    std::println("测试无权限打分：");
    system().teacherGradeStudent("T002", "S001", "CS101", 85.0);

    std::println();

    // Print student grades
    system().studentGrades("S001");
    system().studentGrades("S002");
    system().studentGrades("S003");

    return 0;
}
