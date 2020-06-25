set(CLIENT_SOURCE_FILES ./src/main.cpp
        ./src/Application.cpp
        ./src/Event.cpp
        ./src/Input.cpp
        ./src/ParticleSystem.cpp
        ./src/Renderer.cpp
        ./src/WindowEventHandler.cpp
        ../dependencies/socket/socket/Socket.cpp
        ../core/objects/Entity.cpp
        ../core/objects/components/Component.cpp
        ../core/managers/EntityManager.cpp
        ../renderer/View.cpp
        ../renderer/Renderer.cpp
        ../renderer/Shader.cpp
        ../renderer/Texture.cpp
        ../scripts/UserControlComponent.cpp)

set(CLIENT_HEADER_FILES ./src/Application.h
        ./src/Event.h
        ./src/Input.h
        ./src/ParticleSystem.h
        ./src/Renderer.h
        ./src/WindowEventHandler.h
        ../dependencies/socket/socket/Socket.h
        src/KeyboardButtons.h
        ../core/objects/Entity.h
        ../core/objects/components/Component.h
        ../core/managers/EntityManager.h
        ../core/TypePrimitives.h ../core/math/math.h ../renderer/View.h ../renderer/Renderer.h ../renderer/Shader.h ../renderer/Texture.h ../scripts/UserControlComponent.h ../core/Vertex.h ../core/objects/components/ShapeComponent.cpp ../core/objects/components/ShapeComponent.h ../core/objects/components/DirectionComponent.cpp ../core/objects/components/DirectionComponent.h ../core/objects/components/components.h)

add_executable(client ${CLIENT_SOURCE_FILES} ${CLIENT_HEADER_FILES})
target_include_directories(client PUBLIC ../dependencies/glew/include
        ../dependencies/glfw/include ../dependencies/glm
        ../dependencies/imgui/imgui-master
        ../dependencies/socket)
target_link_libraries(client PUBLIC glew_s glfw imgui -lwsock32)