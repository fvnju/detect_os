cmake_minimum_required(VERSION 3.10)
project(detected VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source files
set(SOURCES
    main.cpp
    hw_info.cpp
    cpu_count.cpp
)

# Create executable
add_executable(detected ${SOURCES})

# Add Windows-specific libraries
if(WIN32)
    target_link_libraries(detected PRIVATE
        ole32
        oleaut32
        wbemuuid
    )
endif()

# Add include directories
target_include_directories(detected PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}) 