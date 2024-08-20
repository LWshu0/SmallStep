project(Game)

find_package(SDL2 REQUIRED)
if(SDL2_FOUND)
    message(STATUS "Find SDL2")
    foreach(VALUE IN LISTS SDL2_INCLUDE_DIRS)
        message(STATUS "SDL_INCLUDE PATH: ${VALUE}")
    endforeach()
endif()

find_package(SDL2_image REQUIRED)
if(SDL2_image_FOUND)
    message(STATUS "Find SDL2_image")
    foreach(VALUE IN LISTS SDL2_image_INCLUDE_DIRS)
        message(STATUS "SDL2_image_INCLUDE PATH: ${VALUE}")
    endforeach()
endif()

find_package(SDL2_ttf REQUIRED)
if(SDL2_ttf_FOUND)
    message(STATUS "Find SDL2_ttf")
    foreach(VALUE IN LISTS SDL2_ttf_INCLUDE_DIRS)
        message(STATUS "SDL2_ttf_INCLUDE PATH: ${VALUE}")
    endforeach()
endif()

find_package(glm REQUIRED)
if(glm_FOUND)
    message(STATUS "Find glm")
    foreach(VALUE IN LISTS GLM_INCLUDE_DIRS)
        message(STATUS "GLM_INCLUDE_DIRS PATH: ${VALUE}")
    endforeach()
endif()

# # 配置 IMGUI 库
# aux_source_directory(${3RD_DIR}/imgui IMGUI_SRC)
# add_library(imgui STATIC ${IMGUI_SRC})
# target_link_libraries(imgui PUBLIC mingw32 SDL2::SDL2main SDL2::SDL2)

# 指明所有头文件路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR}
                    ${SDL2_INCLUDE_DIRS}
                    ${SDL2_image_INCLUDE_DIRS}
                    ${SDL2_ttf_INCLUDE_DIRS}
                    ${GLM_INCLUDE_DIRS}
                    )

add_subdirectory(Core)
add_subdirectory(Timer)
add_subdirectory(Tests)