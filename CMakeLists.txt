cmake_minimum_required(VERSION 3.12.0)
PROJECT(simd)
set(Callgrind OFF)

set(CMAKE_CXX_STANDARD 23)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  # using Clang or GCC
  if(Callgrind STREQUAL "ON")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic -Werror -O2")
  else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic -Werror -fsanitize=address -fsanitize=undefined -fno-sanitize-recover=all -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fno-sanitize=null -fno-sanitize=alignment -O2")
  endif()
endif()

# use pthreads if unix or unix-alike (linux)
if(UNIX)
  set(THREADS_PREFER_PTHREAD_FLAG ON)
  find_package(Threads REQUIRED)
endif()
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

FILE(GLOB MAIN "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")
FILE(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
FILE(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")
FILE(GLOB CPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
list(REMOVE_ITEM CPP_FILES ${MAIN})

ADD_EXECUTABLE(${PROJECT_NAME}
  ${MAIN}
  ${CPP_FILES}
  ${HEADER_FILES}
)
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX _d)

target_link_libraries(${PROJECT_NAME} ${BOOST_LIBRARIES})

if(UNIX)
  target_link_libraries(${PROJECT_NAME} Threads::Threads)
endif()

INSTALL(TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

#INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt")
