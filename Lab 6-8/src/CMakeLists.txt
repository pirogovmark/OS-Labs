add_executable(client ./node.h ./client.cpp)
target_include_directories(client PRIVATE include)
target_link_libraries(client ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(client zmq)

add_executable(server ./comp.cpp)
target_link_libraries(server zmq)