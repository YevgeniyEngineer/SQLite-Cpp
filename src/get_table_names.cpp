#include "SQLiteCpp/SQLiteCpp.h"
#include <cstring>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

extern "C"
{
#include "sqlite3.h"
}

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

    // Open a database file
    sqlite3 *db = nullptr;
    int rc = sqlite3_open(database_path.c_str(), &db);
    if (rc)
    {
        std::cerr << "sqlite3_open error: " << sqlite3_errstr(rc) << std::endl;
        EXIT_FAILURE;
    }

    // Lambda callback for sqlite exec
    static std::vector<std::string> tables;
    auto callback = [](void *dest, int num_cols, char **data, char **columns) -> int {
        for (int idx = 0; idx < num_cols; ++idx)
        {
            // std::cout << "The data in column " << columns[idx] << " is " << data[idx] << std::endl;
            tables.push_back({data[idx]});
        }
        return SQLITE_OK;
    };

    const char *query =
        "SELECT name FROM sqlite_schema WHERE type IN ('table','view') AND name NOT LIKE 'sqlite_%' ORDER BY 1;";

    rc = sqlite3_exec(db, query, callback, nullptr, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "sqlite3_exec error: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return EXIT_FAILURE;
    }
    else
    {
        for (const auto &table_name : tables)
        {
            std::cout << "Table: " << table_name << std::endl;
        }
    }
    sqlite3_close(db);

    return EXIT_SUCCESS;
}