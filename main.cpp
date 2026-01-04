// Module
// File: main.cpp   Version: 0.1.0   License: AGPLv3
// Created: DaiSen Zhou   3563248115@qq.com   2026-01-02 13:39:09
// Description:
//      start of registrar system


//  [v1.0] chao li (3042525170@qq.com)   2026-01-04
//         * added:教学秘书排课功能测试
import registrar;
import std;

int main()
{
    // alias for static function Registrar::singleton
    auto system = &Registrar::singleton;

    system().initialize();

    // Secretary operations
    std::println("=== 教学秘书排课功能测试 ===");
    std::println();

    // Secretary creates courses
    system().secretaryCreateCourse("SEC001", "CS101", "C Programming");
    system().secretaryCreateCourse("SEC001", "CS201", "Data structure");
    system().secretaryCreateCourse("SEC002", "MATH101", "Advanced Math");

    std::println();

    // Secretary assigns teachers to courses
    system().secretaryAssignTeacher("SEC001", "T001", "CS101");
    system().secretaryAssignTeacher("SEC001", "T001", "MATH101");
    system().secretaryAssignTeacher("SEC002", "T002", "CS201");
    system().secretaryAssignTeacher("SEC002", "T003", "MATH101");

    std::println();

    // Secretary sets course schedules
    system().secretarySetSchedule("SEC001", "CS101", "周一 08:00-10:00", "A101");
    system().secretarySetSchedule("SEC001", "MATH101", "周三 14:00-16:00", "B203");
    system().secretarySetSchedule("SEC002", "CS201", "周二 10:00-12:00", "A102");
    system().secretarySetSchedule("SEC002", "MATH101", "周四 08:00-10:00", "B201");

    std::println();

    // Print secretary scheduled courses
    system().secretaryScheduledCourses("SEC001");
    system().secretaryScheduledCourses("SEC002");

    std::println();
    std::println("=== 学生选课功能测试 ===");
    std::println();

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

    std::println();
    std::println("=== 课表输出功能测试 ===");
    std::println();

    // Print student schedules
    system().studentSchedule("S001");
    system().studentSchedule("S002");
    system().studentSchedule("S003");

    std::println();

    // Print teacher schedules
    system().teacherSchedule("T001");
    system().teacherSchedule("T002");
    system().teacherSchedule("T003");

    return 0;
}
