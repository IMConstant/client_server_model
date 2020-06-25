set(SERVER_SOURCE_FILES ./src/main.cpp
        ./src/Server.cpp
        ../dependencies/socket/socket/Socket.cpp)

set(SERVER_HEADER_FILES ./src/Server.h
        ../dependencies/socket/socket/Socket.h)

add_executable(server ${SERVER_SOURCE_FILES} ${SERVER_HEADER_FILES})
target_include_directories(server PUBLIC ../dependencies/socket)
target_link_libraries(server PUBLIC -lwsock32)