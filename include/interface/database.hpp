#pragma once

// C++
#include <iostream>
#include <string>
#include <utility>
#include <thread>
#include <mutex>

// sqlite3
#include <cstdio>
#include "../sqlite/sqlite3.h"

#define _GLIBCXX_HAS_GTHREADS 1

// Interface of sqlite3
class IDataBase
{
public:
   // m_code DataBase
   IDataBase(const std::string &path) noexcept
       : m_path(path)
   {
      m_code = sqlite3_open(m_path.c_str(), &m_db);
      if (m_code)
         sqlite3_errmsg(m_db);
   }

   IDataBase(const IDataBase &other) noexcept
       : IDataBase(other.m_path)
   {
   }

   IDataBase(IDataBase &&other) noexcept
       : IDataBase(other.m_path)
   {
   }

   // Close DataBase and free all dinamyc data
   ~IDataBase() noexcept
   {
      sqlite3_free(m_msg);
      sqlite3_close(m_db);
   }

   IDataBase &operator==(const IDataBase &other) noexcept
   {
      IDataBase fake(other);
      swap(*this, fake);
      return *this;
   }

   IDataBase &operator==(IDataBase &&other) noexcept
   {
      swap(*this, other);
      return *this;
   }

   // Open DataBase
   void open(const std::string &path) noexcept
   {
      sqlite3_free(m_msg);
      sqlite3_close(m_db);
      m_code = false;
   }

   // Close DataBase
   void close(const std::string &path) noexcept
   {
      IDataBase fake(path);
      swap(*this, fake);
   }

   bool SQLexec(const std::string &code) noexcept
   {
      std::unique_lock<std::mutex> lock(m_mutex);
      return sqlite3_exec(m_db, code.c_str(), 0, 0, &m_msg);
   }

   // Status of DataBase
   bool getCode() const
   {
      return m_code;
   }

   // Get msg from the work of database
   std::string getMsg() const
   {
      return m_msg;
   }

   friend void swap(IDataBase &m_db1, IDataBase &m_db2) noexcept
   {
      std::swap(m_db1.m_db, m_db2.m_db);
      std::swap(m_db1.m_msg, m_db2.m_msg);
      std::swap(m_db1.m_code, m_db2.m_code);
   }

private:
   sqlite3 *m_db = nullptr;
   char *m_msg = nullptr;

   int m_code = false;
   std::string m_path{};

   std::mutex m_mutex;
};