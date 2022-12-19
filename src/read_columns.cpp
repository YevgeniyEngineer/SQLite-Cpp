#include "SQLiteCpp/SQLiteCpp.h"
#include <filesystem>
#include <iostream>
#include <string>

int main()
{
    // Database file to be read
    std::string database_name = "album.db";

    // Find location of the database file with respect to relative path
    std::string file_path = __FILE__;
    std::string dir_path = file_path.substr(0, file_path.rfind("\\"));
    auto database_path = std::filesystem::path(dir_path).remove_filename().parent_path().parent_path();
    database_path.append("Ex_Files_Using_SQL_with_Cplusplus/ExerciseFiles/db/" + database_name);
    std::cout << "Reading/Writing database file " << database_path << std::endl;

    try
    {
        // Open a database file
        SQLite::Database db(database_path.c_str());

        // Compile Query
        SQLite::Statement query(db, "SELECT * from album");

        // Fetch Results From Database
        while (query.executeStep())
        {
            int id = query.getColumn(0);
            const char *title = query.getColumn(1);
            const char *artist = query.getColumn(2);
            const char *label = query.getColumn(3);
            const char *text = query.getColumn(4);

            std::cout << "ID: " << id << std::endl;
            std::cout << "Artist: " << artist << std::endl;
            std::cout << "Label: " << label << std::endl;
            std::cout << "Text: " << text << std::endl;
            std::cout << std::endl;
        }
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Exception: " << ex.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "Unknown exception!" << std::endl;
    }

    return EXIT_SUCCESS;
}