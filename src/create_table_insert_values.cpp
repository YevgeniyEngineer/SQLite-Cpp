#include "SQLiteCpp/SQLiteCpp.h"
#include <filesystem>
#include <iostream>
#include <string>

int main()
{
    // Database file to be read
    std::string database_name = "scratch.db";

    // Find location of the database file with respect to relative path
    std::string file_path = __FILE__;
    std::string dir_path = file_path.substr(0, file_path.rfind("\\"));
    auto database_path = std::filesystem::path(dir_path).remove_filename().parent_path().parent_path();
    database_path.append("Ex_Files_Using_SQL_with_Cplusplus/ExerciseFiles/db/" + database_name);
    std::cout << "Reading/Writing database file " << database_path << std::endl;

    try
    {
        // Open a database file
        SQLite::Database db(database_path.c_str(), SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

        {
            // Begin transaction
            SQLite::Transaction transaction(db);

            db.exec("DROP TABLE IF EXISTS temp;");
            db.exec("CREATE TABLE IF NOT EXISTS temp ( a TEXT, b TEXT, c TEXT );");
            db.exec("INSERT INTO temp VALUES ('one', 'two', 'three');");
            db.exec("INSERT INTO temp VALUES ('four', 'five', 'six');");
            db.exec("INSERT INTO temp VALUES ('seven', 'eight', 'nine');");

            // Commit transaction
            transaction.commit();
        }

        {
            // Read Columns
            constexpr const char *sql_inspect = "SELECT * FROM temp;";

            SQLite::Statement query(db, sql_inspect);

            // Iterate over rows
            while (query.executeStep())
            {
                const char *a = query.getColumn(0);
                const char *b = query.getColumn(1);
                const char *c = query.getColumn(2);

                std::cout << "a: " << a << std::endl;
                std::cout << "b: " << b << std::endl;
                std::cout << "c: " << c << std::endl;
                std::cout << std::endl;
            }
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