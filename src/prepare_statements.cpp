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
    std::string database_name = "scratch.db";

    // Find location of the database file with respect to relative path
    std::string file_path = __FILE__;
    std::string dir_path = file_path.substr(0, file_path.rfind("\\"));
    auto database_path = std::filesystem::path(dir_path).remove_filename().parent_path().parent_path();
    database_path.append("Ex_Files_Using_SQL_with_Cplusplus/ExerciseFiles/db/" + database_name);
    std::cout << "Reading/Writing database file " << database_path << std::endl;

    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt = nullptr;

    int rc = sqlite3_open(database_path.c_str(), &db);
    if (rc != SQLITE_OK)
    {
        std::cerr << "sqlite3_open error: " << sqlite3_errstr(rc) << std::endl;
        return EXIT_FAILURE;
    }

    // Create a table
    constexpr const char *sql_create = "DROP TABLE IF EXISTS temp;"
                                       "BEGIN;"
                                       "CREATE TABLE IF NOT EXISTS temp ( a TEXT, b TEXT, c TEXT );"
                                       "INSERT INTO temp VALUES ('one', 'two', 'three');"
                                       "INSERT INTO temp VALUES ('four', 'five', 'six');"
                                       "INSERT INTO temp VALUES ('seven', 'eight', 'nine');"
                                       "COMMIT;";

    rc = sqlite3_exec(db, sql_create, nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cout << "sqlite3_exec error: " << sqlite3_errmsg(db) << std::endl;
        return EXIT_FAILURE;
    }

    // Fetch all rows
    sqlite3_prepare_v2(db, "SELECT * FROM temp", -1, &stmt, nullptr);
    int col_num = sqlite3_column_count(stmt);
    int row_num = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        for (int i = 0; i < col_num; ++i)
        {
            std::cout << sqlite3_column_text(stmt, i) << std::endl;
        }
        ++row_num;
        std::cout << std::endl;
    }
    sqlite3_finalize(stmt);

    // Find a row
    const char *sql_prepare = "SELECT * FROM temp WHERE a = ?";
    const char *param_1 = "four";
    sqlite3_prepare_v2(db, sql_prepare, -1, &stmt, nullptr);
    std::cout << "The statement has " << sqlite3_bind_parameter_count(stmt) << " parameter(s)." << std::endl;

    int param_position = 1;
    // -1 for strlen
    sqlite3_bind_text(stmt, param_position, param_1, -1, SQLITE_STATIC);
    col_num = sqlite3_column_count(stmt);
    row_num = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        for (int i = 0; i < col_num; ++i)
        {
            std::cout << sqlite3_column_text(stmt, i) << std::endl;
        }
        ++row_num;
        std::cout << std::endl;
    }
    sqlite3_finalize(stmt);

    // Drop table
    constexpr const char *drop_table = "DROP TABLE IF EXISTS temp";
    rc = sqlite3_exec(db, drop_table, nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "sqlite3_exec error: " << sqlite3_errmsg(db) << std::endl;
        return EXIT_FAILURE;
    }

    sqlite3_close(db);

    return EXIT_SUCCESS;
}