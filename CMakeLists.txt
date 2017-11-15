cmake_minimum_required(VERSION 3.8)
project(TECMFS)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp VideoHandler.cpp VideoHandler.h DataBase.cpp DataBase.h DataStructures/Node.h DataStructures/LinkedList.h Singleton.cpp Singleton.h Controller.cpp Controller.h FaultTolerance.h)
add_executable(TECMFS ${SOURCE_FILES})

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")

find_package(OpenCV REQUIRED)
if(OpenCV_FOUND)
    target_link_libraries(TECMFS ${OpenCV_LIBS})
    target_link_libraries(TECMFS ${opencv_core})
    target_link_libraries(TECMFS ${opencv_videoio})
    target_link_libraries(TECMFS ${opencv_highgui})
endif()

find_package(SFML REQUIRED system window graphics network audio)
if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(TECMFS ${SFML_LIBRARIES})
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")