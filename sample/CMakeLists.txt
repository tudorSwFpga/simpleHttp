# Add sample executable
add_executable(sample source/main.cpp)

# Link the executable to the library
target_link_libraries(sample PRIVATE http_lib)

# Include the main include directory
target_include_directories(sample PRIVATE 
    ${PROJECT_SOURCE_DIR}/include
)
