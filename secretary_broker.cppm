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
import :course_broker;
import :teacher_broker;
import :database_manager;

using std::string;
using std::vector;

export class SecretaryBroker : public IBroker {

public:
    SecretaryBroker() = default;
    ~SecretaryBroker() = default;
    void initialize() override;
    void setBrokers(class CourseBroker* courseBroker, class TeacherBroker* teacherBroker);
    Secretary* findSecretaryById(const string& id);
    void secretaryCreateCourse(string secid, string cid, string cname,Secretary* secretary, Course* course);
    void secretaryAssignTeacher(string secid, string tid, string cid,Secretary* secretary, Teacher* teacher, Course* course);
    void secretarySetSchedule(string secid, string cid, string tid, string timeSlot,string classroom,Secretary* secretary, Course* course, Teacher* teacher);
    string secretaryScheduledCourses(const string& secid);
    void secretaryCancelSchedule(string secid, string cid, string timeSlot,Secretary* secretary, Course* course);


private:
    vector<Secretary*> _secretaries;
    CourseBroker* _courseBroker = nullptr;
    TeacherBroker* _teacherBroker = nullptr;
    void loadFromDatabase();
    void loadSchedulesFromDatabase();
    void saveScheduleToDatabase(const string& cid, const string& tid, const string& timeSlot, const string& classroom);
    void removeScheduleFromDatabase(const string& cid, const string& timeSlot);
    void saveCourseToDatabase(const string& cid, const string& cname);
    void saveTeachingToDatabase(const string& tid, const string& cid);

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

    // 注意：loadSchedulesFromDatabase 将在 setBrokers 之后调用
}

void SecretaryBroker::setBrokers(CourseBroker* courseBroker, TeacherBroker* teacherBroker)
{
    _courseBroker = courseBroker;
    _teacherBroker = teacherBroker;

    // 在设置 Broker 之后加载排课记录
    loadSchedulesFromDatabase();
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
        saveCourseToDatabase(cid, cname);
        print("课程 {} 创建成功！\n", cid);
    } else {
        print("错误: 教学秘书 {} 不存在！\n", secid);
    }
}

void SecretaryBroker::secretaryAssignTeacher(string secid, string tid, string cid,Secretary* secretary, Teacher* teacher, Course* course)
{
if (secretary && teacher && course) {
        secretary->assignTeacherToCourse(teacher, course);
        saveTeachingToDatabase(tid, cid);
    } else {
        std::print("错误: 教学秘书、教师或课程不存在！\n");
    }
}

void SecretaryBroker::secretarySetSchedule(string secid, string cid, string tid, string timeSlot,string classroom,Secretary* secretary, Course* course, Teacher* teacher)
{
if (secretary && course && teacher) {
    secretary->setCourseSchedule(course, teacher, timeSlot, classroom);
    saveScheduleToDatabase(cid, tid, timeSlot, classroom);
    } else {
        std::print("错误: 教学秘书、课程或教师不存在！\n");
    }
}

string SecretaryBroker::secretaryScheduledCourses(const string& secid)
{
    Secretary* secretary = findSecretaryById(secid);
    if (!secretary) {
        return "教学秘书不存在！\n";
    }

    auto& db = DatabaseManager::singleton();
    if (!db.isConnected()) {
        return secretary->getScheduledCourses();
    }

    string query = "SELECT c.id, c.name, s.time_slot, s.classroom "
                   "FROM courses c "
                   "INNER JOIN schedules s ON c.id = s.course_id "
                   "ORDER BY c.id, s.time_slot;";
    auto results = db.executeSelect(query);

    if (results.empty()) {
        return "暂无已排课的课程\n";
    }

    auto s = format("{} 安排的课程:\n", secretary->info());
    string currentCourseId = "";
    string currentCourseName = "";
    vector<string> schedules;

    for (const auto& row : results) {
        if (row.size() >= 4) {
            if (row[0] != currentCourseId) {
                // 输出上一个课程的信息
                if (!currentCourseId.empty()) {
                    s += format("{}   {} - 时间安排: ", currentCourseId, currentCourseName);
                    for (std::size_t i = 0; i < schedules.size(); ++i) {
                        if (i > 0) s += "; ";
                        s += schedules[i];
                    }
                    s += "\n";
                }
                // 开始新课程
                currentCourseId = row[0];
                currentCourseName = row[1];
                schedules.clear();
            }
            schedules.push_back(format("{} {}", row[2], row[3]));
        }
    }

    // 输出最后一个课程的信息
    if (!currentCourseId.empty()) {
        s += format("{}   {} - 时间安排: ", currentCourseId, currentCourseName);
        for (std::size_t i = 0; i < schedules.size(); ++i) {
            if (i > 0) s += "; ";
            s += schedules[i];
        }
        s += "\n";
    }

    return s;
}

void SecretaryBroker::secretaryCancelSchedule(string secid, string cid, string timeSlot,Secretary* secretary, Course* course)
{
    if (secretary && course) {
        secretary->cancelCourseSchedule(course, timeSlot);
        removeScheduleFromDatabase(cid, timeSlot);
    } else {
        std::print("错误: 教学秘书或课程不存在！\n");
    }
}

void SecretaryBroker::saveScheduleToDatabase(const string& cid, const string& tid, const string& timeSlot, const string& classroom)
{
    auto& db = DatabaseManager::singleton();
    if (!db.isConnected()) {
        return;
    }
    
    string escapedCid = db.escapeString(cid);
    string escapedTid = db.escapeString(tid);
    string escapedTimeSlot = db.escapeString(timeSlot);
    string escapedClassroom = db.escapeString(classroom);
    
    string query = "INSERT INTO schedules (course_id, teacher_id, time_slot, classroom) VALUES (" 
                   + escapedCid + ", " + escapedTid + ", " + escapedTimeSlot + ", " + escapedClassroom + ");";
    
    if (!db.executeQuery(query)) {
        std::print("错误: 保存排课记录到数据库失败\n");
    }
}

void SecretaryBroker::removeScheduleFromDatabase(const string& cid, const string& timeSlot)
{
    auto& db = DatabaseManager::singleton();
    if (!db.isConnected()) {
        return;
    }
    
    string escapedCid = db.escapeString(cid);
    string escapedTimeSlot = db.escapeString(timeSlot);
    
    string query = "DELETE FROM schedules WHERE course_id = " 
                   + escapedCid + " AND time_slot = " + escapedTimeSlot + ";";
    
    if (!db.executeQuery(query)) {
        std::print("错误: 从数据库删除排课记录失败\n");
    }
}

void SecretaryBroker::saveCourseToDatabase(const string& cid, const string& cname)
{
    auto& db = DatabaseManager::singleton();
    if (!db.isConnected()) {
        return;
    }
    
    string escapedCid = db.escapeString(cid);
    string escapedCname = db.escapeString(cname);
    
    string query = "INSERT INTO courses (id, name, credit) VALUES (" 
                   + escapedCid + ", " + escapedCname + ", 0) "
                   "ON CONFLICT (id) DO NOTHING;";
    
    if (!db.executeQuery(query)) {
        std::print("错误: 保存课程到数据库失败\n");
    }
}

void SecretaryBroker::saveTeachingToDatabase(const string& tid, const string& cid)
{
    auto& db = DatabaseManager::singleton();
    if (!db.isConnected()) {
        return;
    }
    
    string escapedTid = db.escapeString(tid);
    string escapedCid = db.escapeString(cid);
    
    string query = "INSERT INTO teaching (teacher_id, course_id) VALUES (" 
                   + escapedTid + ", " + escapedCid + ") "
                   "ON CONFLICT (teacher_id, course_id) DO NOTHING;";
    
    if (!db.executeQuery(query)) {
        std::print("错误: 保存授课记录到数据库失败\n");
    }
}

void SecretaryBroker::loadSchedulesFromDatabase()
{
    auto& db = DatabaseManager::singleton();
    if (!db.isConnected()) {
        return;
    }

    string query = "SELECT course_id, teacher_id, time_slot, classroom FROM schedules ORDER BY course_id, time_slot;";
    auto results = db.executeSelect(query);

    int count = 0;
    for (const auto& row : results) {
        if (row.size() >= 4) {
            if (_courseBroker && _teacherBroker) {
                Course* course = _courseBroker->findCourseById(row[0]);
                Teacher* teacher = _teacherBroker->findTeacherById(row[1]);
                if (course && teacher) {
                    course->m_roomandtime.emplace_back(row[2], row[3]);
                    count++;
                }
            }
        }
    }

    if (count > 0) {
        std::print("从数据库加载了 {} 个排课记录\n", count);
    }
}
