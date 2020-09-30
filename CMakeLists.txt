cmake_minimum_required(VERSION 3.10)

set(PRJNAME Bulleven)

project(${PRJNAME})

include_directories(../03rd/boost)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PRJNAME}
	Main.cpp
	Floor.cpp
	Renderer.cpp
	Texture.cpp
	Scene.cpp
	Overlay.cpp
	Walls.cpp
	Bullets.cpp
	Camera.cpp
)

target_link_libraries(${PRJNAME}
	d3d11
	d3dcompiler
	kernel32
	user32
	gdi32
)
