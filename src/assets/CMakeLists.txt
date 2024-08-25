project(Assets)

add_library(Assets STATIC
        $<IF:$<BOOL:${SPELUNKY_PSP_USE_VIRTUAL_FILESYSTEM}>,src/AssetsVirtualFilesystem.cpp,src/AssetsRealFilesystem.cpp>
        $<IF:$<BOOL:${SPELUNKY_PSP_USE_VIRTUAL_FILESYSTEM}>,generated/VirtualFileIndex.cpp,>
        $<IF:$<BOOL:${SPELUNKY_PSP_USE_VIRTUAL_FILESYSTEM}>,generated/VirtualFileIndex.hpp,>
        src/Paths.cpp
        src/Assets.cpp
        interface/assets/Assets.hpp
        interface/assets/Paths.hpp
)

target_include_directories(Assets
        PRIVATE generated interface
        INTERFACE interface
)

set_target_properties(Assets PROPERTIES
        CXX_STANDARD 17
)

target_link_libraries(Assets
        PUBLIC
        Patterns
        PRIVATE
        Logger
)
