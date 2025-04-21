cmake_minimum_required(VERSION 3.30)
project(renderer LANGUAGES C)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_C_STANDARD_REQUIRED True)

add_executable(run
	source/arena.c
	source/color.c
	source/entity.c
	source/main.c
	source/sprite.c
	source/vector.c
	source/window.c
)
target_include_directories(run PUBLIC include)
add_subdirectory(dependencies/minifb)
target_link_libraries(run m minifb)
