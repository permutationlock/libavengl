#ifndef AVEN_GL_WINDOW_H
    #define AVEN_GL_WINDOW_H

    #include "../gl.h"

    #include <aven.h>
    #include <GLFW/glfw3.h>

    typedef struct {
        AvenGl gl;
        GLFWwindow *window;
    } AvenGlWindow;

    static inline AvenGlWindow aven_gl_window(
        int width,
        int height,
        const char *title
    ) {
        AvenGlWindow win = { 0 };
        bool es = true;
        glfwInit();
        glfwWindowHint(GLFW_SAMPLES, 16);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
        win.window = glfwCreateWindow(
            (int)width,
            (int)height,
            title,
            NULL,
            NULL
        );
        if (win.window == NULL) {
            glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);
            win.window = glfwCreateWindow(
                (int)width,
                (int)height,
                title,
                NULL,
                NULL
            );
        }
        if (win.window == NULL) {
            es = false;
            glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
            win.window = glfwCreateWindow(
                (int)width,
                (int)height,
                title,
                NULL,
                NULL
            );
            if (win.window == NULL) {
                glfwWindowHint(
                    GLFW_CONTEXT_CREATION_API,
                    GLFW_NATIVE_CONTEXT_API
                );
                win.window = glfwCreateWindow(
                    (int)width,
                    (int)height,
                    title,
                    NULL,
                    NULL
                );
            }
        }
        if (win.window == NULL) {
            aven_panic("failed to open window");
        }
        glfwMakeContextCurrent(win.window);
        win.gl = aven_gl_load(glfwGetProcAddress, es);
        return win;
    }
#endif // AVEN_GL_WINDOW_H 
