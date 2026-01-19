// =======================================
// Module
// File: student_broker.cppm   Version: 0.1.0   License: AGPLv3
// Created: lichao2024051604024  3042525170@qq.com   2026-01-09 22:30:28
// Description:
//     学生代理类：处理学生相关的业务逻辑

export module registrar:student_broker;

import std;
import :broker_factory;
import :student;
import :course;
import :course_broker;
import :database_manager;

using std::string;
using std::vector;
using std::shared_ptr;
using std::make_shared;

export class StudentBroker : public IBroker {

public:
    StudentBroker() = default;
    ~StudentBroker() = default;
    void initialize() override;
    void setCourseBroker(class CourseBroker* courseBroker);
    Student* findStudentById(const string& id);
    void studentEnrollsInCourse(string sid, string cid, Course* course);
    void studentDropsFromCourse(string sid, string cid, Course* course);
    string studentSchedule(const string& sid);
    string studentCoursesList(const string& sid);
    string studentGrades(const string& sid);

private:
    vector<Student*> _students;
    CourseBroker* _courseBroker = nullptr;
    void loadFromDatabase();
    void loadGradesFromDatabase();
    void saveEnrollmentToDatabase(const string& sid, const string& cid);
    void removeEnrollmentFromDatabase(const string& sid, const string& cid);
    void removeGradeFromDatabase(const string& sid, const string& cid);
    void loadEnrollmentsFromDatabase();

};

void StudentBroker::initialize()
{
    // 尝试从数据库加载数据
    loadFromDatabase();

    // 如果数据库中没有数据或连接失败，使用默认数据
    if (_students.empty()) {
        _students.push_back(new Student("S001", "Thomas"));
        _students.push_back(new Student("S002", "Jerry"));
        _students.push_back(new Student("S003", "Baker"));
        _students.push_back(new Student("S004", "Tom"));
        _students.push_back(new Student("S005", "Musk"));
    }

    // 注意：loadEnrollmentsFromDatabase 和 loadGradesFromDatabase 将在 setCourseBroker 之后调用
}

void StudentBroker::setCourseBroker(CourseBroker* courseBroker)
{
    _courseBroker = courseBroker;

    // 在设置 CourseBroker 之后加载选课和成绩记录
    loadEnrollmentsFromDatabase();
    loadGradesFromDatabase();
}

void StudentBroker::loadFromDatabase()
{
    auto& db = DatabaseManager::singleton();
    if (!db.isConnected()) {
        return;
    }
    
    string query = "SELECT id, name FROM students ORDER BY id;";
    auto results = db.executeSelect(query);
    
    for (const auto& row : results) {
        if (row.size() >= 2) {
            _students.push_back(new Student(row[0], row[1]));
        }
    }
    
    if (!_students.empty()) {
        std::print("从数据库加载了 {} 个学生记录\n", _students.size());
    }
}

Student* StudentBroker::findStudentById(const string& id)
{
    for (auto* student : _students) {
       if (student->hasId(id)) {
              return student;
           }
       }
       return nullptr;
}

void StudentBroker::studentEnrollsInCourse(string sid, string cid, Course* course)
{
    Student* student = findStudentById(sid);
    if (student && course) {
        student->enrollsIn(course);
        saveEnrollmentToDatabase(sid, cid);
    } else {
        std::print("错误: 学生或课程不存在！\n");
    }
}

void StudentBroker::studentDropsFromCourse(string sid, string cid, Course* course)
{
    Student* student = findStudentById(sid);
    if (student && course) {
        student->dropsFrom(course);
        removeEnrollmentFromDatabase(sid, cid);
        removeGradeFromDatabase(sid, cid);
    } else {
        std::print("错误: 学生或课程不存在！\n");
    }
}

string StudentBroker::studentSchedule(const string& sid)
{
    Student* student = findStudentById(sid);
    if (student) {
        return student->schedule();
    }
    return "学生不存在！\n";
}

string StudentBroker::studentCoursesList(const string& sid)
{
    Student* student = findStudentById(sid);
    if (student) {
        return student->coursesList();
    }
    return "学生不存在！\n";
}

string StudentBroker::studentGrades(const string& sid)
{
    Student* student = findStudentById(sid);
    if (student) {
        return student->getGrades();
    }
    return "学生不存在！\n";
}

void StudentBroker::saveEnrollmentToDatabase(const string& sid, const string& cid)
{
    auto& db = DatabaseManager::singleton();
    if (!db.isConnected()) {
        return;
    }
    
    string escapedSid = db.escapeString(sid);
    string escapedCid = db.escapeString(cid);
    
    string query = "INSERT INTO enrollments (student_id, course_id) VALUES (" 
                   + escapedSid + ", " + escapedCid + ") "
                   "ON CONFLICT (student_id, course_id) DO NOTHING;";
    
    if (!db.executeQuery(query)) {
        std::print("错误: 保存选课记录到数据库失败\n");
    }
}

void StudentBroker::removeEnrollmentFromDatabase(const string& sid, const string& cid)
{
    auto& db = DatabaseManager::singleton();
    if (!db.isConnected()) {
        return;
    }
    
    string escapedSid = db.escapeString(sid);
    string escapedCid = db.escapeString(cid);
    
    string query = "DELETE FROM enrollments WHERE student_id = " 
                   + escapedSid + " AND course_id = " + escapedCid + ";";
    
    if (!db.executeQuery(query)) {
        std::print("错误: 从数据库删除选课记录失败\n");
    }
}

void StudentBroker::loadEnrollmentsFromDatabase()
{
    auto& db = DatabaseManager::singleton();
    if (!db.isConnected()) {
        return;
    }

    string query = "SELECT student_id, course_id FROM enrollments ORDER BY student_id, course_id;";
    auto results = db.executeSelect(query);

    int count = 0;
    for (const auto& row : results) {
        if (row.size() >= 2) {
            if (_courseBroker) {
                Student* student = findStudentById(row[0]);
                Course* course = _courseBroker->findCourseById(row[1]);
                if (student && course) {
                    student->_courses.push_back(course);
                    course->_students.push_back(student);
                    count++;
                }
            }
        }
    }

    if (count > 0) {
        std::print("从数据库加载了 {} 个选课记录\n", count);
    }
}

void StudentBroker::loadGradesFromDatabase()
{
    auto& db = DatabaseManager::singleton();
    if (!db.isConnected()) {
        return;
    }

    string query = "SELECT student_id, course_id, grade FROM grades ORDER BY student_id, course_id;";
    auto results = db.executeSelect(query);

    int count = 0;
    for (const auto& row : results) {
        if (row.size() >= 3) {
            if (_courseBroker) {
                Student* student = findStudentById(row[0]);
                Course* course = _courseBroker->findCourseById(row[1]);
                if (student && course) {
                    try {
                        double grade = std::stod(row[2]);
                        student->addGrade(course, grade);
                        count++;
                    } catch (...) {
                        // 忽略无效的成绩数据
                    }
                }
            }
        }
    }

    if (count > 0) {
        std::print("从数据库加载了 {} 个成绩记录\n", count);
    }
}

void StudentBroker::removeGradeFromDatabase(const string& sid, const string& cid)
{
    auto& db = DatabaseManager::singleton();
    if (!db.isConnected()) {
        return;
    }
    
    string escapedSid = db.escapeString(sid);
    string escapedCid = db.escapeString(cid);
    
    string query = "DELETE FROM grades WHERE student_id = " 
                   + escapedSid + " AND course_id = " + escapedCid + ";";
    
    if (!db.executeQuery(query)) {
        std::print("错误: 从数据库删除成绩记录失败\n");
    }
}
