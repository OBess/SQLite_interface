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
   if (!database.getCode())
      std::cout << "DataBase [ " << file << " ] is open!" << std::endl;

   SQL = "CREATE TABLE Students(Id INTEGER PRIMARY KEY, Surname TEXT, Name TEXT, Arrange REAL);"
         "INSERT INTO Students(Surname, Name, Arrange) VALUES ('Surn1', 'Tom', 2.3);"
         "INSERT INTO Students(Surname, Name, Arrange) VALUES ('Surn2', 'Endy', 4.3);"
         "INSERT INTO Students(Surname, Name, Arrange) VALUES ('Surn3', 'Oliver', 5.0);";
   database.SQLexec(SQL);
   SQL = "SELECT * FROM Students";
   database.SQLexec(SQL, callback);

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