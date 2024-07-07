# CMakeList.txt : CMake project for UniversityProgressionCalculator, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

include(FetchContent)

project ("UniversityProgressionCalculator")

# Prefer static libraries
#set(BUILD_SHARED_LIBS OFF)

#find_package(SFML REQUIRED COMPONENTS graphics system window)
find_package(imgui CONFIG REQUIRED)
#find_package(ImGui-SFML CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

# Get imnodes for node editor
FetchContent_Declare( imnodes 
    GIT_REPOSITORY https://github.com/Nelarius/imnodes.git
    GIT_TAG origin/master
    )
FetchContent_MakeAvailable(imnodes)

# Get ctre for faster regex
FetchContent_Declare( ctre 
    GIT_REPOSITORY https://github.com/hanickadot/compile-time-regular-expressions
    GIT_TAG origin/main
    )
FetchContent_MakeAvailable(ctre)

# Add source to this project's executable.
add_executable (UniversityProgressionCalculator "UniversityProgressionCalculator.cpp" "UniversityProgressionCalculator.h" "module.cpp" "module.h" "term.cpp" "term.h" "year.h" "year.cpp" "saveLoadJson.h"  "progressionTree.h" "progressionTree.cpp" "imguiStyle.h")

#set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# Set MSVC to use static runtime libraries
#set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

target_link_libraries(${PROJECT_NAME} PRIVATE 
 #                   sfml-graphics sfml-system sfml-window
                    imgui::imgui
  #                  ImGui-SFML::ImGui-SFML
                    nlohmann_json::nlohmann_json
                    imnodes
                    ctre::ctre
                    glad::glad
                    )

# Set the output directory path
set(OUTPUT_DIRECTORY_DATA ${CMAKE_CURRENT_BINARY_DIR}/userData)
set(OUTPUT_DIRECTORY_PROGRESSION ${CMAKE_CURRENT_BINARY_DIR}/progressionData)

# Create the directory using the file command
file(MAKE_DIRECTORY ${OUTPUT_DIRECTORY_DATA})
file(MAKE_DIRECTORY ${OUTPUT_DIRECTORY_PROGRESSION})

# Create a custom command to copy the directory
add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/userData ${OUTPUT_DIRECTORY_DATA}
    COMMENT "Copying data directory from ${CMAKE_SOURCE_DIR}/userData to ${OUTPUT_DIRECTORY_DATA}"
)

add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/progressionData ${OUTPUT_DIRECTORY_PROGRESSION}
    COMMENT "Copying data directory from ${CMAKE_SOURCE_DIR}/progressionData to ${OUTPUT_DIRECTORY_PROGRESSION}"
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET UniversityProgressionCalculator PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
