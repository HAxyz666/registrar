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

    system().courseRoster("CS101");
    system().courseRoster("CS201");
    system().courseRoster("MATH101");

    system().studentSchedule("S001");

    system().studentSchedule("S003");

    return 0;
}
