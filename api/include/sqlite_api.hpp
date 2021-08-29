#pragma once

// C++
#include <string>
#include <utility>
#include <mutex>

// sqlite3
#include <cstdio>
#include "sqlite3.h"

// Interface of sqlite3
class SQLiteAdapter
{
public:
   // m_code DataBase
   SQLiteAdapter(const std::string &path) noexcept;

   SQLiteAdapter(const SQLiteAdapter &other) noexcept;

   SQLiteAdapter(SQLiteAdapter &&other) noexcept;

   // Close DataBase and free all dinamyc data
   ~SQLiteAdapter() noexcept;

   SQLiteAdapter &operator==(const SQLiteAdapter &other) noexcept;

   SQLiteAdapter &operator==(SQLiteAdapter &&other) noexcept;

   // Status of DataBase
   int getCode() const;

   // Get msg from the work of database
   std::string getMsg() const;

   // Open DataBase
   void open(const std::string &path) noexcept;

   // Close DataBase
   void close(const std::string &path) noexcept;

   int SQLexec(const std::string &code) noexcept;

   int SQLexec(const std::string &code, int (*func)(void *, int, char **, char **)) noexcept;

   friend void swap(SQLiteAdapter &m_db1, SQLiteAdapter &m_db2) noexcept;

private:
   sqlite3 *m_db = nullptr;
   char *m_msg = nullptr;

   int m_code = false;
   std::string m_path{};

   std::mutex m_mutex;
};