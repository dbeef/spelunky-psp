project(TextureBank)

add_library(TextureBank STATIC
        src/TextureBank.cpp
        interface/TextureBank.hpp
        interface/TextureType.hpp
        )

target_include_directories(TextureBank
        PRIVATE include interface
        INTERFACE interface
        )

set_target_properties(TextureBank PROPERTIES
        CXX_STANDARD
        14
        )

target_link_libraries(TextureBank
        PRIVATE
            Logger
            stb
            Camera
            Video
            glm::glm
            GraphicsUtils
            cjson
        PUBLIC
            glad
        )

if (SPELUNKY_PSP_PLATFORM_LINUX)
    target_link_libraries(TextureBank PRIVATE dl)
    target_compile_definitions(TextureBank PRIVATE SPELUNKY_PSP_PLATFORM_LINUX)
elseif(SPELUNKY_PSP_PLATFORM_PSP)
    target_compile_definitions(TextureBank PRIVATE SPELUNKY_PSP_PLATFORM_PSP)
endif()
