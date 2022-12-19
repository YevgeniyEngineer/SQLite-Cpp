cmake_minimum_required(VERSION 3.1)

project(sqlite_examples)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

find_package(SQLiteCpp)

# set executables
set(EXECUTABLES 
    read_columns 
    create_table_insert_values
    get_table_names
    prepare_statements
)

# generate executables
foreach(EXECUTABLE ${EXECUTABLES})

    add_executable(${EXECUTABLE} src/${EXECUTABLE}.cpp)
    target_include_directories(${EXECUTABLE} PRIVATE src)
    target_link_libraries(${EXECUTABLE} SQLiteCpp)

endforeach()
