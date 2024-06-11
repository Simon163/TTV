CMAKE_MINIMUM_REQUIRED(VERSION 3.8.2)
project(TTV)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_FLAGS_DEBUG "-g -ggdb -std=c++11  -Wall -w -O0 -fPIC -pthread -fsigned-char -save-temps")
set(CMAKE_CXX_FLAGS_RELEASE "-std=c++11  -Wall -w -O3 -fPIC -pthread -fsigned-char")
set(CMAKE_C_FLAGS_DEBUG "-g -ggdb -std=c11 -Wall -w -O0 -fPIC -pthread -fsigned-char -save-temps")
set(CMAKE_C_FLAGS_RELEASE "-std=c11 -Wall -w -O3 -fPIC -pthread -fsigned-char")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB TTV_SRCS ${CMAKE_CURRENT_LIST_DIR}/source/Ttv.cpp ${CMAKE_CURRENT_LIST_DIR}/source/TtvBox.cpp ${CMAKE_CURRENT_LIST_DIR}/source/TtvBuffer.cpp)
message("TTV_SRCS: " ${TTV_SRCS})
add_library(TTV SHARED ${TTV_SRCS})

set(TTV_DEPS "")
list(APPEND TTV_DEPS TTV)

if(CMAKE_SYSTEM_NAME MATCHES "^Linux")
set(TTV_DEPS -Wl,--whole-archive ${TTV_DEPS} -lpthread -Wl,--no-whole-archive)
else()
set(TTV_DEPS -Wl,--whole-archive ${TTV_DEPS} -Wl,--no-whole-archive)
endif()

add_executable(testTtvBox.out ${CMAKE_CURRENT_LIST_DIR}/test/testTtvBox.cpp)
target_link_libraries(testTtvBox.out ${TTV_DEPS})

add_executable(testTtvBuffer.out ${CMAKE_CURRENT_LIST_DIR}/test/testTtvBuffer.cpp)
target_link_libraries(testTtvBuffer.out ${TTV_DEPS})

add_executable(testModelPreCfgDemo.out ${CMAKE_CURRENT_LIST_DIR}/demo/testModelPreCfgDemo.cpp)
target_link_libraries(testModelPreCfgDemo.out ${TTV_DEPS})

