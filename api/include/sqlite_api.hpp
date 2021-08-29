#pragma once

// C++
#include <string>
#include <utility>
#include <mutex>
#include <vector>

// sqlite3
#include <cstdio>
#include "sqlite3.h"

// Usings
using mat_string = std::vector<std::vector<std::string>>;

// Interface of sqlite3
class SQLiteAdapter final
{
public:
   // m_code DataBase
   SQLiteAdapter(const std::string &path) noexcept;

   SQLiteAdapter(const SQLiteAdapter &other) noexcept;

   SQLiteAdapter(SQLiteAdapter &&other) noexcept;

   // Close DataBase and free all dinamyc data
   ~SQLiteAdapter() noexcept;

   SQLiteAdapter &operator=(const SQLiteAdapter &other) noexcept;

   SQLiteAdapter &operator=(SQLiteAdapter &&other) noexcept;

   // Status of DataBase
   int getCode() const;

   // Get msg from the work of database
   const std::string &getMsg() const;

   // Open DataBase
   void open(const std::string &path) noexcept;

   // Close DataBase
   void close(const std::string &path) noexcept;

   int exec(const std::string &query, int (*func)(void *, int, char **, char **) = nullptr) noexcept;

   mat_string select(const std::string &query) noexcept;

   friend void swap(SQLiteAdapter &m_db1, SQLiteAdapter &m_db2) noexcept;

private:
   sqlite3 *m_db = nullptr;
   sqlite3_stmt *m_stmt = nullptr;

   std::string m_errmsg{};

   int m_code = -1;
   std::string m_path{};

   std::mutex m_mutex;
};