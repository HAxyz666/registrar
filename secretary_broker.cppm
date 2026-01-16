// =======================================
// Module
// File: secretary_broker.cppm   Version: 0.1.0   License: AGPLv3
// Created: lichao2024051604024  3042525170@qq.com   2026-01-09 22:30:07
// Description:
//     教学秘书代理类：处理教学秘书相关的业务逻辑

export module registrar:secretary_broker;

import std;
import :broker_factory;
import :secretary;
import :course;
import :teacher;
import :database_manager;

using std::string;
using std::vector;

export class SecretaryBroker : public IBroker {

public:
    SecretaryBroker() = default;
    ~SecretaryBroker() = default;
    void initialize() override;
    Secretary* findSecretaryById(const string& id);
    void secretaryCreateCourse(string secid, string cid, string cname,Secretary* secretary, Course* course);
    void secretaryAssignTeacher(string secid, string tid, string cid,Secretary* secretary, Teacher* teacher, Course* course);
    void secretarySetSchedule(string secid, string cid, string tid, string timeSlot,string classroom,Secretary* secretary, Course* course, Teacher* teacher);
    string secretaryScheduledCourses(const string& secid);
    void secretaryCancelSchedule(string secid, string cid, string timeSlot,Secretary* secretary, Course* course);


private:
    vector<Secretary*> _secretaries;
    void loadFromDatabase();

};

void SecretaryBroker::initialize()
{
    // 尝试从数据库加载数据
    loadFromDatabase();
    
    // 如果数据库中没有数据或连接失败，使用默认数据
    if (_secretaries.empty()) {
        _secretaries.push_back(new Secretary("SEC001", "Ms. Wang"));
        _secretaries.push_back(new Secretary("SEC002", "Mr. Li"));
    }
}

void SecretaryBroker::loadFromDatabase()
{
    auto& db = DatabaseManager::singleton();
    if (!db.isConnected()) {
        return;
    }
    
    string query = "SELECT id, name FROM secretaries ORDER BY id;";
    auto results = db.executeSelect(query);
    
    for (const auto& row : results) {
        if (row.size() >= 2) {
            _secretaries.push_back(new Secretary(row[0], row[1]));
        }
    }
    
    if (!_secretaries.empty()) {
        std::print("从数据库加载了 {} 个教学秘书记录\n", _secretaries.size());
    }
}

Secretary* SecretaryBroker::findSecretaryById(const string& id) {
    for (auto* secretary : _secretaries) {
        if (secretary->hasId(id)) {
            return secretary;
        }
    }
    return nullptr;
}

void SecretaryBroker::secretaryCreateCourse(string secid, string cid, string cname,Secretary* secretary, Course* course)
{
    if (secretary && course) {
        secretary->createCourse(course);
        print("课程 {} 创建成功！\n", cid);
    } else {
        print("错误: 教学秘书 {} 不存在！\n", secid);
    }
}

void SecretaryBroker::secretaryAssignTeacher(string secid, string tid, string cid,Secretary* secretary, Teacher* teacher, Course* course)
{
if (secretary && teacher && course) {
        secretary->assignTeacherToCourse(teacher, course);
    } else {
        std::print("错误: 教学秘书、教师或课程不存在！\n");
    }
}

void SecretaryBroker::secretarySetSchedule(string secid, string cid, string tid, string timeSlot,string classroom,Secretary* secretary, Course* course, Teacher* teacher)
{
if (secretary && course && teacher) {
    secretary->setCourseSchedule(course, teacher, timeSlot, classroom);
    } else {
        std::print("错误: 教学秘书、课程或教师不存在！\n");
    }
}

string SecretaryBroker::secretaryScheduledCourses(const string& secid)
{
    Secretary* secretary = findSecretaryById(secid);
    if (secretary) {
        return secretary->getScheduledCourses();
    }
    return "教学秘书不存在！\n";
}

void SecretaryBroker::secretaryCancelSchedule(string secid, string cid, string timeSlot,Secretary* secretary, Course* course)
{
    if (secretary && course) {
        secretary->cancelCourseSchedule(course, timeSlot);
    } else {
        std::print("错误: 教学秘书或课程不存在！\n");
    }
}
