cmake_minimum_required( VERSION 2.8 )
project( Flock )

set( CMAKE_INCLUDE_CURRENT_DIR ON )

find_package( ZeroMQ REQUIRED )
find_package( cppzmq REQUIRED )
include_directories( ${ZeroMQ_INCLUDE_DIR} ${cppzmq_INCLUDE_DIR} )

add_executable( Server server.cpp Console.cpp Flocking.cpp StopWatch.cpp )
target_link_libraries( Server ${cppzmq_LIBRARY} )

add_executable( Client client.cpp )
target_link_libraries( Client ${cppzmq_LIBRARY} )