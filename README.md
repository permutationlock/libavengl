# libavengl: a simple cross-platform 2D graphics library

The `libavengl` library provides a dynamic loader for the OpenGL ES 3.1
API and an API for drawing 2D shapes, textures, text, and an immediate mode
UI. It vendors the GLFW and STB TrueType libraries, along with the
headers for OpenGL ES 3.1, Wayland, X11, and xkbcommon.

The provided drawing APIs all use a common subset of desktop OpenGL 4.5
and OpenGL ES 3.1, and the provided GLFW wrapper will automatically
load OpenGL ES 3.1 as a runtime backup if OpenGL 4.5 support fails to load.
A helper macro is provided to runtime select compatible shader preludes
(GLSL version, etc.) for the loaded API.

## Platform support

I've tested the project on `x86_64` machines running Windows, Linux
(gnu or musl libc, X11 or Wayland display manager),
and FreeBSD (X11 or Wayland). Theoretically, it should run on
other architectures as well. Apple operating systems are not
supported at the moment as I don't have an Apple device to test on.
Web platforms are supported via Emscripten.

## Testing the loader

To build and test the loader, run:
```Shell
make
./build test
```
You should see some colored shapes, colored rounded shapes, and a checkered
square.

## Cleaning up

To clean test build artifacts, run:
```Shell
./build clean
make clean
```

## Example

A full example application using `libavengl` is the algorithm
visualization in [`libavengraph`][1].

[1]: https://github.com/permutationlock/libavengraph

