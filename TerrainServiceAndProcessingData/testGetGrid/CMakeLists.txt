
cmake_minimum_required(VERSION 3.14)

# Set the project name
project (testGetGrid)

file(GLOB_RECURSE CURRENT_HEADERS  *.h *.hpp)
source_group("Header Files" FILES ${CURRENT_HEADERS}) 


set(Boost_INCLUDE_DIR ${THIRDPART}/boost_1_58_0/)
set(Boost_LIBRARY_DIR ${THIRDPART}/boost_1_58_0/stage/lib)
#find_package(Boost COMPONENTS serialization REQUIRED)
#include_directories(${Boost_INCLUDE_DIR})
#link_directories(${Boost_LIBRARY_DIR})
SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11")
find_package(Boost REQUIRED COMPONENTS
# regex
serialization   # 我的工程中只使用了 boost 的 serialization 功能,因此这里只有一个组件
)
if(NOT Boost_FOUND)
    message("Not found Boost")
endif()

include_directories(${Boost_INCLUDE_DIR})
message("${Boost_INCLUDE_DIR}")
message("${Boost_LIBRARY_DIR}")

# Create the executable
add_executable(${PROJECT_NAME} testGetGrid.cpp ${CURRENT_HEADERS})

target_link_libraries(${PROJECT_NAME}
    TransformationDll
    ${Boost_LIBRARY_DIR}
)
