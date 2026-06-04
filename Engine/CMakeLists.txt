add_library(Engine STATIC)

target_sources(Engine
    PRIVATE
        Core/Application.cpp
        Core/Layer.cpp
)

target_include_directories(Engine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/Core
)