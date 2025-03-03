# Add the test executable
add_executable(test source/main.cpp)

# Link the executable to the library
target_link_libraries(test PRIVATE http_lib)

# Include the main include directory
target_include_directories(test PRIVATE 
    ${PROJECT_SOURCE_DIR}/include
)
