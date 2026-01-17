// =======================================
// Module
// File: course_broker.cppm   Version: 0.1.0   License: AGPLv3
// Created: lichao2024051604024  3042525170@qq.com   2026-01-09 22:29:59
// Description:
//     课程代理类：处理课程相关的业务逻辑

export module registrar:course_broker;

import std;
import :broker_factory;
import :course;
import :database_manager;

using std::string;
using std::vector;

export class CourseBroker : public IBroker {
public:
    CourseBroker() = default;
    ~CourseBroker() = default;
    void initialize() override;
    Course* findCourseById(const string& id);
    Course* createCourse(const string& cid, const string& cname);
    string courseRoster(const string& cid);

private:
    vector<Course*> _courses;
    void loadFromDatabase();
    void saveCourseToDatabase(const string& cid, const string& cname);

};

void CourseBroker::initialize()
{
    // 尝试从数据库加载数据
    loadFromDatabase();
    
    // 如果数据库中没有数据或连接失败，使用默认数据
    if (_courses.empty()) {
        _courses.push_back(new Course("CS101", "C Programming"));
        _courses.push_back(new Course("CS201", "Data structure"));
        _courses.push_back(new Course("MATH101", "Advanced Math"));
    }
}

void CourseBroker::loadFromDatabase()
{
    auto& db = DatabaseManager::singleton();
    if (!db.isConnected()) {
        return;
    }
    
    string query = "SELECT id, name, credit FROM courses ORDER BY id;";
    auto results = db.executeSelect(query);
    
    for (const auto& row : results) {
        if (row.size() >= 2) {
            _courses.push_back(new Course(row[0], row[1]));
        }
    }
    
    if (!_courses.empty()) {
        std::print("从数据库加载了 {} 个课程记录\n", _courses.size());
    }
}

Course* CourseBroker::findCourseById(const string& id)
{
    for (auto* course : _courses) {
        if (course->hasId(id)) {
            return course;
        }
    }
    return nullptr;
}

Course* CourseBroker::createCourse(const string& cid, const string& cname)
{
    Course* course = findCourseById(cid);
    if (course) {
        std::print("警告: 课程 {} 已存在！\n", cid);
        return course;
    }
    course = new Course(cid, cname);
    _courses.push_back(course);
    saveCourseToDatabase(cid, cname);
    return course;
}

string CourseBroker::courseRoster(const string& cid)
{
    Course* course = findCourseById(cid);
    if (course) {
        return course->roster();
    }
    return "课程不存在！\n";
}

void CourseBroker::saveCourseToDatabase(const string& cid, const string& cname)
{
    auto& db = DatabaseManager::singleton();
    if (!db.isConnected()) {
        return;
    }
    
    string escapedCid = db.escapeString(cid);
    string escapedCname = db.escapeString(cname);
    
    string query = "INSERT INTO courses (id, name, credit) VALUES (" 
                   + escapedCid + ", " + escapedCname + ", 0);";
    
    if (!db.executeQuery(query)) {
        std::print("错误: 保存课程到数据库失败\n");
    }
}
