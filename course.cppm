// Module
// File: course.cppm   Version: 0.1.0   License: AGPLv3
// Created: DaiSen Zhou   3563248115@qq.com   2026-01-02 13:40:54
// Description:
//     The interface and implementation of Course class are logically separated.

export module registrar:course;
import std;

using std::string; using std::vector;

// 课程时间安排结构体
export struct RoomAndTime
{
    string timeSlot;   // 时间段
    string classroom;  // 教室

    RoomAndTime(string ts, string cr) : timeSlot(ts), classroom(cr) {}
};

export class Course
{
public:
    Course(string id, string name);

    bool acceptEnrollment(class Student* student);
    void removeStudent(Student *student);
    string roster();
    string info();
    bool hasId(string id);
    string getScheduleInfo() const;

    friend class Secretary;
private:
    string m_name;
    string m_id;
    short m_credit;
    static int cm_totalCount;  // static data member

    vector<class Student*> _students;
    vector<RoomAndTime> m_roomandtime;  // 课程时间安排
    
    friend class DatabaseManager;
    friend class Registrar;
};

// ----- Partial implementation of class Course -----

int Course::cm_totalCount = 0; // initialize static data memeber

Course::Course(string id, string name)
    : m_name(name)
    , m_id(id)
    , m_roomandtime()
{
    cm_totalCount++;
}

void Course::removeStudent(Student *student)
{
    auto it = std::find(_students.begin(), _students.end(), student);
    if (it != _students.end()) {
        _students.erase(it);
        print("\"{}\" 退课成功！目前选择该课程的人数: {}\n",
              m_name, _students.size());
    } else {
        std::print("错误: 学生未选择该课程！\n");
    }
}
bool Course::acceptEnrollment(Student *student){
    if(_students.size() < 80){  // 假定某个课程的最大人数为80
        _students.push_back(student);
        print("\"{}\" 选课成功！目前选择该课程的人数: {}\n",
              m_name, _students.size());
        return true;
    }
    return false;
}

string Course::info(){
    return format("{}   {}", m_id, m_name);
}


bool Course::hasId(string id){
    return id == m_id;
}

string Course::getScheduleInfo() const
{
    if (m_roomandtime.empty()) {
        return "未排课";
    }

    string scheduleInfo = "";
    for (std::size_t i = 0; i < m_roomandtime.size(); ++i) {
        if (i > 0) {
            scheduleInfo += "; ";
        }
        scheduleInfo += format("{} {}", m_roomandtime[i].timeSlot, m_roomandtime[i].classroom);
    }
    return scheduleInfo;
}
