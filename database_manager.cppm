// Module
// File: database_manager.cppm   Version: 0.1.0   License: AGPLv3
// Created: DaiSen Zhou   3563248115@qq.com   2026-01-16 22:54:06
// Description:
//     数据库管理模块：负责PostgreSQL数据库连接和基本操作

module;

#include <libpq-fe.h>

export module registrar:database_manager;

import std;

using std::string;
using std::vector;
using std::shared_ptr;
using std::make_shared;

export class DatabaseManager {
public:
    static DatabaseManager& singleton();
    
    // 数据库连接管理
    bool connect(const string& host, const string& port, const string& dbname, 
                 const string& user, const string& password);
    void disconnect();
    bool isConnected() const;
    
    // 表初始化
    bool initializeTables();
    
    // 通用查询方法
    bool executeQuery(const string& query);
    vector<vector<string>> executeSelect(const string& query);
    string escapeString(const string& input);
    
    // 获取最后插入的ID
    int getLastInsertId(const string& tableName, const string& idColumn);
    
private:
    DatabaseManager() = default;
    ~DatabaseManager();
    
    PGconn* _connection = nullptr;
    
    // 禁用拷贝
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;
    
    // 连接重试机制
    inline static const int MAX_RETRY_ATTEMPTS = 3;
    bool connectWithRetry(const string& conninfo);
};

// ----- 单例实现 -----

DatabaseManager& DatabaseManager::singleton()
{
    static DatabaseManager instance;
    return instance;
}

DatabaseManager::~DatabaseManager()
{
    disconnect();
}

// ----- 连接管理 -----

bool DatabaseManager::connectWithRetry(const string& conninfo)
{
    int status = std::system("pg_isready > /dev/null 2>&1");
    if(status != 0){
        std::system("sudo systemctl start postgresql");
    }
    for (int attempt = 1; attempt <= MAX_RETRY_ATTEMPTS; attempt++) {
        _connection = PQconnectdb(conninfo.c_str());
        
        if (PQstatus(_connection) == CONNECTION_OK) {
            std::print("成功连接到PostgreSQL数据库\n");
            return true;
        }
        
        std::print("数据库连接失败 (尝试 {}/{}): {}\n", 
                   attempt, MAX_RETRY_ATTEMPTS, PQerrorMessage(_connection));
        
        PQfinish(_connection);
        _connection = nullptr;
        
        if (attempt < MAX_RETRY_ATTEMPTS) {
            std::print("等待 {} 秒后重试...\n", attempt * 2);
            std::this_thread::sleep_for(std::chrono::seconds(attempt * 2));
        }
    }
    
    return false;
}

bool DatabaseManager::connect(const string& host, const string& port, const string& dbname,
                              const string& user, const string& password)
{
    if (_connection) {
        disconnect();
    }
    
    string conninfo = "host=" + host + 
                      " port=" + port + 
                      " dbname=" + dbname + 
                      " user=" + user + 
                      " password=" + password;
    
    return connectWithRetry(conninfo);
}

void DatabaseManager::disconnect()
{
    if (_connection) {
        PQfinish(_connection);
        _connection = nullptr;
        std::print("数据库连接已关闭\n");
    }
}

bool DatabaseManager::isConnected() const
{
    return _connection != nullptr && PQstatus(_connection) == CONNECTION_OK;
}

// ----- 表初始化 -----

bool DatabaseManager::initializeTables()
{
    if (!isConnected()) {
        std::print("错误: 数据库未连接\n");
        return false;
    }
    
    // 创建学生表
    string createStudentsTable = R"(
        CREATE TABLE IF NOT EXISTS students (
            id VARCHAR(20) PRIMARY KEY,
            name VARCHAR(100) NOT NULL
        );
    )";
    
    // 创建教师表
    string createTeachersTable = R"(
        CREATE TABLE IF NOT EXISTS teachers (
            id VARCHAR(20) PRIMARY KEY,
            name VARCHAR(100) NOT NULL
        );
    )";
    
    // 创建教学秘书表
    string createSecretariesTable = R"(
        CREATE TABLE IF NOT EXISTS secretaries (
            id VARCHAR(20) PRIMARY KEY,
            name VARCHAR(100) NOT NULL
        );
    )";
    
    // 创建课程表
    string createCoursesTable = R"(
        CREATE TABLE IF NOT EXISTS courses (
            id VARCHAR(20) PRIMARY KEY,
            name VARCHAR(200) NOT NULL,
            credit SMALLINT DEFAULT 0
        );
    )";
    
    // 创建选课表
    string createEnrollmentsTable = R"(
        CREATE TABLE IF NOT EXISTS enrollments (
            student_id VARCHAR(20) REFERENCES students(id) ON DELETE CASCADE,
            course_id VARCHAR(20) REFERENCES courses(id) ON DELETE CASCADE,
            PRIMARY KEY (student_id, course_id)
        );
    )";
    
    // 创建成绩表
    string createGradesTable = R"(
        CREATE TABLE IF NOT EXISTS grades (
            student_id VARCHAR(20) REFERENCES students(id) ON DELETE CASCADE,
            course_id VARCHAR(20) REFERENCES courses(id) ON DELETE CASCADE,
            grade DOUBLE PRECISION,
            PRIMARY KEY (student_id, course_id)
        );
    )";
    
    // 创建教师授课表
    string createTeachingTable = R"(
        CREATE TABLE IF NOT EXISTS teaching (
            teacher_id VARCHAR(20) REFERENCES teachers(id) ON DELETE CASCADE,
            course_id VARCHAR(20) REFERENCES courses(id) ON DELETE CASCADE,
            PRIMARY KEY (teacher_id, course_id)
        );
    )";
    
    // 创建课程时间表
    string createSchedulesTable = R"(
        CREATE TABLE IF NOT EXISTS schedules (
            course_id VARCHAR(20) REFERENCES courses(id) ON DELETE CASCADE,
            teacher_id VARCHAR(20) REFERENCES teachers(id) ON DELETE CASCADE,
            time_slot VARCHAR(50) NOT NULL,
            classroom VARCHAR(50) NOT NULL,
            PRIMARY KEY (course_id, time_slot)
        );
    )";
    
    if (!executeQuery(createStudentsTable) ||
        !executeQuery(createTeachersTable) ||
        !executeQuery(createSecretariesTable) ||
        !executeQuery(createCoursesTable) ||
        !executeQuery(createEnrollmentsTable) ||
        !executeQuery(createGradesTable) ||
        !executeQuery(createTeachingTable) ||
        !executeQuery(createSchedulesTable)) {
        std::print("错误: 初始化数据库表失败\n");
        return false;
    }
    
    std::print("数据库表初始化成功\n");
    return true;
}

// ----- 通用查询方法 -----

bool DatabaseManager::executeQuery(const string& query)
{
    if (!isConnected()) {
        std::print("错误: 数据库未连接\n");
        return false;
    }
    
    PGresult* res = PQexec(_connection, query.c_str());
    
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::print("查询执行失败: {}\n", PQerrorMessage(_connection));
        PQclear(res);
        return false;
    }
    
    PQclear(res);
    return true;
}

vector<vector<string>> DatabaseManager::executeSelect(const string& query)
{
    vector<vector<string>> results;
    
    if (!isConnected()) {
        std::print("错误: 数据库未连接\n");
        return results;
    }
    
    PGresult* res = PQexec(_connection, query.c_str());
    
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::print("查询执行失败: {}\n", PQerrorMessage(_connection));
        PQclear(res);
        return results;
    }
    
    int rows = PQntuples(res);
    int cols = PQnfields(res);
    
    for (int i = 0; i < rows; i++) {
        vector<string> row;
        for (int j = 0; j < cols; j++) {
            row.push_back(PQgetvalue(res, i, j));
        }
        results.push_back(row);
    }
    
    PQclear(res);
    return results;
}

string DatabaseManager::escapeString(const string& input)
{
    if (!isConnected()) {
        std::print("错误: 数据库未连接\n");
        return "";
    }
    
    char* escaped = PQescapeLiteral(_connection, input.c_str(), input.size());
    
    if (!escaped) {
        std::print("错误: 字符串转义失败: {}\n", PQerrorMessage(_connection));
        return "";
    }
    
    string result(escaped);
    PQfreemem(escaped);
    
    return result;
}

int DatabaseManager::getLastInsertId(const string& tableName, const string& idColumn)
{
    string query = "SELECT MAX(" + idColumn + ") FROM " + tableName + ";";
    auto results = executeSelect(query);
    
    if (!results.empty() && !results[0].empty() && !results[0][0].empty()) {
        return std::stoi(results[0][0]);
    }
    
    return 0;
}
