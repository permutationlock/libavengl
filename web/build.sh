# required environment:
#     WEB_EMCC: path to emcc
#     APP_NAME: the name of your app
#     CFLAGS: common flags to be passed to C compiler
#     LDFLAGS: common flags to be passed to the linker

# clean old build artifiacts
./clean.sh

mkdir build_web
envsubst '$$APP_NAME' < template/shell.html > build_web/shell.html
cd ..
./build test --cc "$WEB_EMCC" \
    --ccflags "$CFLAGS" \
    --glfw-ccflags "$CFLAGS" \
    --stb-ccflags "$CFLAGS" \
    --ldflags "$LDFLAGS \
        -s ASYNCIFY \
        -s USE_GLFW=3 \
        -s EXPORTED_RUNTIME_METHDOS=cwrap \
        -s EXPORTED_FUNCTIONS=_main, _on_resize \
        --shell-file web/build_web/shell.html" \
    --exext ".html .js .wasm" \
    --glfw-external \
    --syslibs ""
cp build_out/test.html web/build_web/index.html
cp build_out/test.js web/build_web/index.js
cp build_out/test.wasm web/build_web/index.wasm
cd web
