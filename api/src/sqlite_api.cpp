// header
#include "sqlite_api.hpp"

// ---------------- Constructors
SQLiteAdapter::SQLiteAdapter(const std::string &path) noexcept
    : m_path(path)
{
    m_code = sqlite3_open(m_path.c_str(), &m_db);
    if (m_code)
        sqlite3_errmsg(m_db);
}

SQLiteAdapter::SQLiteAdapter(const SQLiteAdapter &other) noexcept
    : SQLiteAdapter(other.m_path)
{
}

SQLiteAdapter::SQLiteAdapter(SQLiteAdapter &&other) noexcept
    : SQLiteAdapter(other.m_path)
{
}

// ---------------- Destructor
SQLiteAdapter::~SQLiteAdapter() noexcept
{
    sqlite3_free(m_msg);
    sqlite3_close(m_db);
}

// ---------------- Assigne Operators
SQLiteAdapter &SQLiteAdapter::operator==(const SQLiteAdapter &other) noexcept
{
    SQLiteAdapter fake(other);
    swap(*this, fake);
    return *this;
}

SQLiteAdapter &SQLiteAdapter::operator==(SQLiteAdapter &&other) noexcept
{
    swap(*this, other);
    return *this;
}

// ---------------- Getters
int SQLiteAdapter::getCode() const
{
    return m_code;
}

std::string SQLiteAdapter::getMsg() const
{
    return m_msg;
}

// ---------------- Interface
void SQLiteAdapter::open(const std::string &path) noexcept
{
    sqlite3_free(m_msg);
    sqlite3_close(m_db);
    m_code = false;
}

// Close DataBase
void SQLiteAdapter::close(const std::string &path) noexcept
{
    SQLiteAdapter fake(path);
    swap(*this, fake);
}

int SQLiteAdapter::SQLexec(const std::string &code) noexcept
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_code = sqlite3_exec(m_db, code.c_str(), 0, 0, &m_msg);
    return m_code;
}

int SQLiteAdapter::SQLexec(const std::string &code, int (*func)(void *, int, char **, char **)) noexcept
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_code = sqlite3_exec(m_db, code.c_str(), func, 0, &m_msg);
    return m_code;
}

// Auxiliary
void swap(SQLiteAdapter &m_db1, SQLiteAdapter &m_db2) noexcept
{
    std::swap(m_db1.m_db, m_db2.m_db);
    std::swap(m_db1.m_msg, m_db2.m_msg);
    std::swap(m_db1.m_code, m_db2.m_code);
}