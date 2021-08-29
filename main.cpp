// C++
#include <iostream>
#include <filesystem>

// Custom
#include "sqlite_api.hpp"

// Usings
namespace fs = std::filesystem;

int callback(void *NotUsed, int argc, char **argv, char **azColName);

int main()
{
   static const std::string path{"log"};
   fs::create_directory(path);

   static const std::string file{path + "/Students.db"};
   std::string SQL{};

   SQLiteAdapter database(file);
   if (database.getCode() == SQLITE_OK)
      std::cout << "DataBase [ " << file << " ] is open!" << std::endl;

   SQL = "CREATE TABLE Students(Id INTEGER PRIMARY KEY, Surname TEXT, Name TEXT, Arrange REAL);";
   database.exec(SQL);
   SQL = "INSERT INTO Students(Surname, Name, Arrange) VALUES (?, ?, ?);";
   database.exec(SQL, {{"Sush", "Dima", "23"}, {"Sush1", "Dima1", "213"}});
   SQL = "SELECT * FROM Students";
   database.exec(SQL, callback);

   return EXIT_SUCCESS;
}

// For SELECT
int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
   NotUsed = 0;
   for (int i = 0; i < argc; ++i)
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   return 0;
}