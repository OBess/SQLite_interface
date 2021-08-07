// C++
#include <iostream>

// Custom
#include "include/interface/database.hpp"

int main()
{
   const std::string path = "first.db";

   IDataBase database(path);
   if (database.isOpen())
      std::cout << "DataBase [ " << path << " ] is open!" << std::endl;

   std::string SQL{"CREATE TABLE IF NOT EXISTS foo(a,b,c); INSERT INTO FOO VALUES(1,2,3); INSERT INTO FOO SELECT * FROM FOO;"};
   database.SQLexec(SQL);

   return EXIT_SUCCESS;
}