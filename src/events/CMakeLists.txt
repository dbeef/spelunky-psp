project(Events)

add_library(Events STATIC
        interface/events/ShopkeeperAssaultEvent.hpp
        interface/events/ThieveryEvent.hpp
)

target_include_directories(Events
        PRIVATE interface interface
        INTERFACE interface
)

set_target_properties(Events PROPERTIES
        CXX_STANDARD
        17
)

target_link_libraries(Events
        PUBLIC
        Patterns
        EntityRegistry
        Components
        PRIVATE
        Logger
        Audio
        cjson
)

if (SPELUNKY_PSP_PLATFORM_LINUX)
    target_link_libraries(Events PRIVATE dl)
    target_compile_definitions(Events PRIVATE SPELUNKY_PSP_PLATFORM_LINUX)
elseif (SPELUNKY_PSP_PLATFORM_PSP)
    target_compile_definitions(Events PRIVATE SPELUNKY_PSP_PLATFORM_PSP)
endif ()
