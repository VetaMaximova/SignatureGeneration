cmake_minimum_required(VERSION 3.10)
set (CMAKE_CXX_STANDARD 11)
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

find_package(Boost COMPONENTS system filesystem thread REQUIRED)

add_executable(signature_generation main.cpp SignatureGenerator.cpp FileReader.cpp FileWriter.cpp HashWriter.cpp LoadBalancer.cpp)
target_link_libraries( signature_generation ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} Boost::thread -lpthread -lrt)
