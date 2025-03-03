find_package(spdlog REQUIRED)

# Set the name of your library
add_library(http_lib STATIC 
    http.cpp
    tcp.cpp
)

target_link_libraries(http_lib PRIVATE spdlog::spdlog)

# Specify the include directory for the library (this can also be in your main CMakeLists.txt)
# target_include_directories(htpp_lib PUBLIC 
#     ${PROJECT_SOURCE_DIR}/include
# )
