# libavengl: a simple cross-platform 2D graphics library

The `libavengl` library provides a dynamic loader for the OpenGL ES 2.0
API and provides an API for drawing 2D shapes, text, and an immediate mode
UI. It vendors GLFW and the STB TrueType libraries, along with the
headers for OpenGL ES 2.0, Wayland, X11, and xkbcommon.

## Testing the loader

To build and test the loader, run:
```Shell
make
./build test
```

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

