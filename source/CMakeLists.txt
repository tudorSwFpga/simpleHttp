find_package(spdlog QUIET)

if (NOT spdlog_FOUND)
    message(STATUS "spdlog not found, fetching from github")
    FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.15.0 
    )
    FetchContent_MakeAvailable(spdlog)
endif()

# Set the name of your library
add_library(http_lib STATIC 
    http.cpp
    tcp.cpp
)
target_include_directories(http_lib PUBLIC ${spdlog_SOURCE_DIR}/include)
target_link_libraries(http_lib PRIVATE spdlog::spdlog)


# Specify the include directory for the library (this can also be in your main CMakeLists.txt)
# target_include_directories(htpp_lib PUBLIC 
#     ${PROJECT_SOURCE_DIR}/include
# )
