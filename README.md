# libavengl: a simple cross-platform 2D graphics library

The `libavengl` library provides a dynamic loader for the OpenGL ES 2.0
API and provides an API for drawing 2D shapes, text, and an immediate mode
UI. It vendors GLFW and the STB TrueType libraries, along with the
headers for OpenGL ES 2.0, Wayland, X11, and xkbcommon.

## Platform support

I've tested the project on `x86_64` machines running Windows, Linux
(gnu or musl libc, X11 or Wayland display manager),
and FreeBSD (X11 or Wayland). Theoretically, it should run on
other architectures as well. Apple operating systems are not
supported at the moment as I don't have an Apple device to test on.

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

