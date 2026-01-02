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
    system().studentSchedule("S001");
    system().studentSchedule("S003");

    std::println();

    // Assign teachers to courses
    system().assignTeacherToCourse("T001", "CS101");
    system().assignTeacherToCourse("T002", "CS201");
    system().assignTeacherToCourse("T003", "MATH101");

    std::println();

    // Print teacher teaching courses
    system().teacherTeachingCourses("T001");
    system().teacherTeachingCourses("T002");
    system().teacherTeachingCourses("T003");

    std::println();

    // Teachers grade students
    system().teacherGradeStudent("T001", "S001", "CS101", 95.5);
    system().teacherGradeStudent("T001", "S002", "CS101", 88.0);
    system().teacherGradeStudent("T002", "S001", "CS201", 92.0);
    system().teacherGradeStudent("T002", "S003", "CS201", 85.5);
    system().teacherGradeStudent("T003", "S001", "MATH101", 90.0);
    system().teacherGradeStudent("T003", "S003", "MATH101", 78.5);

    std::println();

    // Print student grades
    system().studentGrades("S001");
    system().studentGrades("S002");
    system().studentGrades("S003");

    return 0;
}
