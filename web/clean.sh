rm -rf build_web

cd ..
./build clean --exext ".html .js .wasm" --glfw-external --syslibs ""
cd web
