# CJSON
set(BUILD_SHARED_LIBS OFF)
set(BUILD_SHARED_AND_STATIC_LIBS OFF)
set(ENABLE_TARGET_EXPORT OFF)
set(ENABLE_CJSON_TEST OFF)
set(ENABLE_CJSON_UTILS ON)

add_subdirectory(logger)
add_subdirectory(stb)
add_subdirectory(glm)
add_subdirectory(glad)
add_subdirectory(cjson)
add_subdirectory(entt)

if (SPELUNKY_PSP_WITH_IMGUI)
    add_subdirectory(imgui)
endif()
