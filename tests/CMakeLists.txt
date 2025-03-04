# Add the test executable
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0  # Use the latest stable version
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(test source/main.cpp)
target_link_libraries( test GTest::gtest_main http_lib )

# Include the main include directory
target_include_directories(test PRIVATE 
    ${PROJECT_SOURCE_DIR}/include
)
