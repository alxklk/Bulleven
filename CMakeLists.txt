cmake_minimum_required(VERSION 3.10)

set(PRJNAME Bullet)

project(${PRJNAME})

set(CMAKE_CXX_STANDARD 17)

add_executable(${PRJNAME}
	Main.cpp Model.cpp Renderer.cpp Texture.cpp Scene.cpp Overlay.cpp
)

target_link_libraries(${PRJNAME}
	d3d11
	d3dcompiler
	kernel32
	user32
	gdi32
)
