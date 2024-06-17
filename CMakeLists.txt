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

find_package(SFML REQUIRED COMPONENTS graphics system window)
find_package(imgui CONFIG REQUIRED)
find_package(ImGui-SFML CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

# Get imnodes for node editor
FetchContent_Declare( imnodes 
    GIT_REPOSITORY https://github.com/Nelarius/imnodes.git
    GIT_TAG origin/master
    )
FetchContent_MakeAvailable(imnodes)

# Add source to this project's executable.
add_executable (UniversityProgressionCalculator "UniversityProgressionCalculator.cpp" "UniversityProgressionCalculator.h" "module.cpp" "module.h" "term.cpp" "term.h" "year.h" "year.cpp" "saveLoadJson.h"  "progressionTree.h" "progressionTree.cpp")

target_link_libraries(${PROJECT_NAME} PRIVATE 
                    sfml-graphics sfml-system sfml-window
                    imgui::imgui
                    ImGui-SFML::ImGui-SFML
                    nlohmann_json::nlohmann_json
                    imnodes
                    )

# Create a custom command to copy the directory
add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/data ${CMAKE_BINARY_DIR}
    COMMENT "Copying data directory from ${CMAKE_SOURCE_DIR}/data to ${CMAKE_BINARY_DIR}"
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET UniversityProgressionCalculator PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
