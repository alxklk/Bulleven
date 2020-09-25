export LIBS="-ld3d11 -ld3dcompiler"
export OPTS="-static-libstdc++ -static-libgcc"
export SRCS="Main.cpp  Model.cpp  Renderer.cpp  RTTexture.cpp  Scene.cpp"
x86_64-w64-mingw32-g++ -oDX11Base $SRCS $OPTS $LIBS
cp DX11Base.exe bin/