cmake_minimum_required(VERSION 3.7)
project(DDoSDetector)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp types.h headers.h DDoSDetector.cpp DDoSDetector.h misc.cpp CurlHelper.h CurlHelper.cpp RandomQueue.h)
add_executable(DDoSDetector ${SOURCE_FILES})
target_link_libraries(DDoSDetector pcap pthread curl)
