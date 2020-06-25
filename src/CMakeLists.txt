add_library(MyStaticLib STATIC
            ./test/test.h
            ./test/test.cpp
        )

target_include_directories(MyStaticLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/test)
target_compile_definitions(MyStaticLib PUBLIC SAY_HELLO_VERSION=5)