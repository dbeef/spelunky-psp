project(MainDude)

add_library(MainDude STATIC
        src/main-dude/MainDudeHelpers.cpp
        src/main-dude/MainDudeComponent.cpp
        src/main-dude/MainDudeObservers.cpp
        src/main-dude/states/MainDudePushingState.cpp
        src/main-dude/states/MainDudeLookingUpState.cpp
        src/main-dude/states/MainDudeCrawlingState.cpp
        src/main-dude/states/MainDudeThrowingState.cpp
        src/main-dude/states/MainDudeRunningLookingUpState.cpp
        src/main-dude/states/MainDudeClimbingState.cpp
        src/main-dude/states/MainDudeFallingState.cpp
        src/main-dude/states/MainDudeJumpingState.cpp
        src/main-dude/states/MainDudeRunningState.cpp
        src/main-dude/states/MainDudeExitingState.cpp
        src/main-dude/states/MainDudeDuckingState.cpp
        src/main-dude/states/MainDudeStunnedState.cpp
        src/main-dude/states/MainDudeLevelSummaryState.cpp
        src/main-dude/states/MainDudeStandingState.cpp
        src/main-dude/states/MainDudeDeadState.cpp
        src/main-dude/states/MainDudeCliffHangingState.cpp
        interface/main-dude/MainDudeEvent.hpp
        interface/main-dude/states/MainDudeRunningState.hpp
        interface/main-dude/states/MainDudeDuckingState.hpp
        interface/main-dude/states/MainDudeStandingState.hpp
        interface/main-dude/states/MainDudeExitingState.hpp
        interface/main-dude/states/MainDudeStunnedState.hpp
        interface/main-dude/states/MainDudeLookingUpState.hpp
        interface/main-dude/states/MainDudeLevelSummaryState.hpp
        interface/main-dude/states/MainDudeCliffHangingState.hpp
        interface/main-dude/states/MainDudeThrowingState.hpp
        interface/main-dude/states/MainDudeCrawlingState.hpp
        interface/main-dude/states/MainDudePushingState.hpp
        interface/main-dude/states/MainDudeClimbingState.hpp
        interface/main-dude/states/MainDudeDeadState.hpp
        interface/main-dude/states/MainDudeFallingState.hpp
        interface/main-dude/states/MainDudeJumpingState.hpp
        interface/main-dude/states/MainDudeBaseState.hpp
        interface/main-dude/states/MainDudeRunningLookingUpState.hpp
        interface/main-dude/MainDudeHelpers.hpp
        interface/main-dude/MainDudeObservers.hpp
)

target_include_directories(MainDude
        PRIVATE interface interface
        INTERFACE interface
)

set_target_properties(MainDude PROPERTIES
        CXX_STANDARD
        17
)

target_link_libraries(MainDude
        PUBLIC
        Viewport
        Patterns
        EntityRegistry
        RenderingTypes
        Level
        Components
        Input
        Other
        PRIVATE
        Logger
        Collisions
        Audio
)

if (SPELUNKY_PSP_PLATFORM_LINUX)
    target_link_libraries(MainDude PRIVATE dl)
    target_compile_definitions(MainDude PRIVATE SPELUNKY_PSP_PLATFORM_LINUX)
elseif (SPELUNKY_PSP_PLATFORM_PSP)
    target_compile_definitions(MainDude PRIVATE SPELUNKY_PSP_PLATFORM_PSP)
endif ()
