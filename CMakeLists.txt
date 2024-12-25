cmake_minimum_required(VERSION 3.15)

project(ZObject VERSION 1.0 DESCRIPTION "ZObject OOP simulation in C")

set(TARGET zobject)

# Set C standard
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Define compiler warnings in one place
set(C_WARNINGS "-Wall -Wextra -Wshadow -Wformat=2 -Wunused")

# Set flags for Debug and Release configurations
set(CMAKE_C_FLAGS_DEBUG     "${CMAKE_C_FLAGS_DEBUG} -ggdb3 -O0 -fno-omit-frame-pointer -fsanitize=address")
set(CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS_RELEASE} -O3")

# Set version script
set(VERSION_SCRIPT ${CMAKE_SOURCE_DIR}/zobject.map.txt)
if(EXISTS ${VERSION_SCRIPT})
  set(VERSION_SCRIPT_FLAGS "-Wl,--version-script,${VERSION_SCRIPT}" "-Wl,--no-undefined-version")
else()
  set(VERSION_SCRIPT_FLAGS "")
endif()

# Include source and header files
include(sources.cmake)
include(headers.cmake)

# Create INTERFACE target for shared flags
add_library(zobject_c_flags INTERFACE)
target_compile_features(zobject_c_flags INTERFACE c_std_17)
target_compile_options(
  zobject_c_flags
  INTERFACE
    "$<BUILD_INTERFACE:${C_WARNINGS}>"
)
target_link_options(
  zobject_c_flags
  INTERFACE
    ${VERSION_SCRIPT_FLAGS}
)

# Main library setup
add_library(${TARGET} SHARED)
target_sources(${TARGET} PRIVATE ${SOURCE_FILES})
target_link_options(${TARGET} PRIVATE ${VERSION_SCRIPT_FLAGS})
set_target_properties(${TARGET} PROPERTIES LINK_DEPENDS ${VERSION_SCRIPT})

# Tests section
# Option to enable or disable tests
option(BUILD_TESTS "Build tests" ON)

if(BUILD_TESTS)
  include(include_google_test.cmake)
  add_executable(zobject_test tests/zobjecttest.cpp tests/zobjectsubclass.cpp)
  target_link_libraries(zobject_test PRIVATE ${TARGET} gtest gtest_main)
  enable_testing()

  include(GoogleTest)
  gtest_discover_tests(zobject_test)
endif()

# Install targets
install(TARGETS ${TARGET} DESTINATION bin)
install(FILES ${HEADER_FILES} DESTINATION include)