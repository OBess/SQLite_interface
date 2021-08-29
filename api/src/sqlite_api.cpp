// header
#include "sqlite_api.hpp"

// ---------------- Constructors
SQLiteAdapter::SQLiteAdapter(const std::string &path) noexcept
    : m_path(path)
{
    open(m_path);
}

SQLiteAdapter::SQLiteAdapter(const SQLiteAdapter &other) noexcept
    : SQLiteAdapter(other.m_path)
{
}

SQLiteAdapter::SQLiteAdapter(SQLiteAdapter &&other) noexcept
{
    SQLiteAdapter fake(other);
    swap(*this, fake);
}

// ---------------- Destructor
SQLiteAdapter::~SQLiteAdapter() noexcept
{
    close(m_path);
}

// ---------------- Assigne Operators
SQLiteAdapter &SQLiteAdapter::operator=(const SQLiteAdapter &other) noexcept
{
    SQLiteAdapter fake(other);
    swap(*this, fake);
    return *this;
}

SQLiteAdapter &SQLiteAdapter::operator=(SQLiteAdapter &&other) noexcept
{
    swap(*this, other);
    return *this;
}

// ---------------- Getters
int SQLiteAdapter::getCode() const
{
    return m_code;
}

const std::string &SQLiteAdapter::getMsg() const
{
    return m_errmsg;
}

// ---------------- Interface
void SQLiteAdapter::open(const std::string &path) noexcept
{
    close(m_path);

    m_path = path;
    m_code = sqlite3_open(m_path.c_str(), &m_db);

    if (m_code)
        m_errmsg = {sqlite3_errmsg(m_db)};
}

void SQLiteAdapter::close(const std::string &path) noexcept
{
    sqlite3_finalize(m_stmt);
    sqlite3_close(m_db);
}

int SQLiteAdapter::exec(const std::string &query, CALLBACK(func)) noexcept
{
    std::unique_lock<std::mutex> lock(m_mutex);
    char *error;

    m_code = sqlite3_exec(m_db, query.c_str(), func, 0, &error);

    if (error)
        m_errmsg = {error};

    sqlite3_free(error);
    return m_code;
}

int SQLiteAdapter::exec(const std::string &query, const mat_string &args) noexcept
{
    std::unique_lock<std::mutex> lock(m_mutex);

    if (args.empty())
        return SQLITE_RANGE;

    sqlite3_prepare_v2(m_db, query.c_str(), -1, &m_stmt, nullptr);
    const size_t size_params = static_cast<size_t>(sqlite3_bind_parameter_count(m_stmt));

    if (size_params != args[0].size())
        return SQLITE_RANGE;

    for (const auto &row : args)
    {
        for (size_t i = 0; i < row.size(); ++i)
            sqlite3_bind_text(m_stmt, i + 1, row[i].c_str(), -1, SQLITE_STATIC);
        sqlite3_step(m_stmt);
        sqlite3_reset(m_stmt);
    }
    
    return m_code;
}

mat_string SQLiteAdapter::select(const std::string &query) noexcept
{
    std::unique_lock<std::mutex> lock(m_mutex);

    sqlite3_prepare_v2(m_db, query.c_str(), -1, &m_stmt, nullptr);

    return getTableData();
}

mat_string SQLiteAdapter::getTableData() noexcept
{
    mat_string selected;
    int col_size = sqlite3_column_count(m_stmt);
    int row_size = 0;
    char buffer[512];

    while (sqlite3_step(m_stmt) == SQLITE_ROW)
    {
        selected.push_back({});
        for (int i = 0; i < col_size; ++i)
        {
            std::sprintf(buffer, "%s", sqlite3_column_text(m_stmt, i));
            selected[row_size].push_back({buffer});
        }
        ++row_size;
    }

    return selected;
}

// Auxiliary
void swap(SQLiteAdapter &m_db1, SQLiteAdapter &m_db2) noexcept
{
    std::swap(m_db1.m_db, m_db2.m_db);
    std::swap(m_db1.m_stmt, m_db2.m_stmt);
    std::swap(m_db1.m_errmsg, m_db2.m_errmsg);
    std::swap(m_db1.m_code, m_db2.m_code);
}