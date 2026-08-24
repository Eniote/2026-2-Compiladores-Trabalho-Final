cmake_minimum_required(VERSION 3.20)
project(FlexWorkspace)

set(CMAKE_CXX_STANDARD 26)

# Retrieve dependencies once for the entire workspace
include(FetchContent)
FetchContent_Declare(
        win_flex_bison
        URL https://github.com/lexxmark/winflexbison/releases/download/v2.5.25/win_flex_bison-2.5.25.zip
)
FetchContent_MakeAvailable(win_flex_bison)

# Assign the executable path. Subdirectories will inherit this variable.
set(FLEX_EXECUTABLE "${win_flex_bison_SOURCE_DIR}/win_flex.exe")
find_package(FLEX REQUIRED)

# Initialize the shared library first
add_subdirectory(common_lib)

# Initialize subprojects
add_subdirectory(env_test)
add_subdirectory(no_lexer)
add_subdirectory(with_lexer)
add_subdirectory(diagrams)