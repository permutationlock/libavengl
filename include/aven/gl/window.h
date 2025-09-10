#ifndef AVEN_GL_WINDOW_H
    #define AVEN_GL_WINDOW_H

    #include <aven.h>
    #include <aven/time.h>

    #include "../gl.h"

    #include <GLFW/glfw3.h>

    #ifdef __EMSCRIPTEN__
        #include <emscripten.h>
    #endif

    typedef enum {
        AVEN_GL_WINDOW_ACTION_NONE = 0,
        AVEN_GL_WINDOW_ACTION_SWAP,
        AVEN_GL_WINDOW_ACTION_CLOSE,
    } AvenGlWindowAction;

    typedef struct AvenGlWindow AvenGlWindow;
    typedef void AvenGlWindowInitFn(AvenGlWindow *win);
    typedef void AvenGlWindowDeinitFn(AvenGlWindow *win);
    typedef AvenGlWindowAction AvenGlWindowUpdateFn(AvenGlWindow *win);
    typedef void AvenGlWindowDamageFn(AvenGlWindow *win);
    typedef void AvenGlWindowMouseClickFn(
        AvenGlWindow *win,
        int button,
        int action,
        int modes
    );
    typedef void AvenGlWindowMouseMoveFn(AvenGlWindow *win, float x, float y);
    typedef void AvenGlWindowMouseEnterFn(AvenGlWindow *win, bool entered);
    typedef void AvenGlWindowKeyFn(
        AvenGlWindow *win,
        int key,
        int scancode,
        int action,
        int modes
    );

    typedef struct {
        AvenGlWindowInitFn *init;
        AvenGlWindowDeinitFn *deinit;
        AvenGlWindowUpdateFn *update;
        OptPtr(AvenGlWindowDamageFn) damage;
        OptPtr(AvenGlWindowMouseClickFn) mouse_click;
        OptPtr(AvenGlWindowMouseMoveFn) mouse_move;
        OptPtr(AvenGlWindowMouseEnterFn) mouse_enter;
        OptPtr(AvenGlWindowKeyFn) key;
    } AvenGlWindowVtable;

    struct AvenGlWindow {
        AvenGl gl;
        GLFWwindow *window;
        AvenGlWindowVtable vtable;
        AvenTimeInst last;
        AvenTimeInst now;
        int width;
        int height;
    #ifdef __ANDROID__
        bool minimized;
    #endif
    };

    static AvenGlWindow aven_gl_window_ctx;

    static void aven_gl_window_damage_internal(GLFWwindow *window) {
        (void)window;
        AvenGlWindow *win = &aven_gl_window_ctx;
        glfwGetFramebufferSize(win->window, &win->width, &win->height);
        unwrap(win->vtable.damage)(win);
    }

    static void aven_gl_window_key_internal(
        GLFWwindow *window,
        int key,
        int scancode,
        int action,
        int modes
    ) {
        (void)window;
        AvenGlWindow *win = &aven_gl_window_ctx;
        unwrap(win->vtable.key)(win, key, scancode, action, modes);
    }

    static void aven_gl_window_mouse_click_internal(
        GLFWwindow *window,
        int button,
        int action,
        int modes
    ) {
        (void)window;
        AvenGlWindow *win = &aven_gl_window_ctx;
        unwrap(win->vtable.mouse_click)(win, button, action, modes);
    }

    static void aven_gl_window_mouse_move_internal(
        GLFWwindow *window,
        double x,
        double y
    ) {
        (void)window;
        AvenGlWindow *win = &aven_gl_window_ctx;
        unwrap(win->vtable.mouse_move)(win, (float)x, (float)y);
    }

    static void aven_gl_window_mouse_enter_internal(
        GLFWwindow *window,
        int entered
    ) {
        (void)window;
        AvenGlWindow *win = &aven_gl_window_ctx;
        unwrap(win->vtable.mouse_enter)(win, entered);
    }

    #ifdef __ANDROID__
        static void aven_gl_window_iconify_android_internal(
            GLFWwindow *window,
            int inconfied
        ) {
            (void)window;
            AvenGlWindow *win = &aven_gl_window_ctx;
            if (iconified) {
                win->vtable.deinit(win);
                win->minimized = true;
            } else {
                win->vtable.init(win);
                win->minimized = false;
            }
        }
    #endif

    static bool aven_gl_window_update(AvenGlWindow *win) {
        if (glfwWindowShouldClose(win->window)) {
            return false;
        }
        glfwPollEvents();
    #ifdef __ANDROID__
        while (win->minimized) {
            glfwWaitEvents();
        }
    #endif
        glfwGetFramebufferSize(win->window, &win->width, &win->height);
        win->last = win->now;
        win->now = aven_time_now();
        switch (win->vtable.update(win)) {
            case AVEN_GL_WINDOW_ACTION_NONE:
                aven_time_sleep_ms(AVEN_TIME_NSEC_PER_SEC / 30);
                break;
            case AVEN_GL_WINDOW_ACTION_SWAP:
                glfwSwapBuffers(win->window);
                break;
            case AVEN_GL_WINDOW_ACTION_CLOSE:
                win->vtable.deinit(win);
                glfwSetWindowShouldClose(win->window, GLFW_TRUE);
                break;
        }
        return true;
    }

    #ifdef __EMSCRIPTEN__
        static void aven_gl_window_emscripten_main_loop(void) {
            AvenGlWindow *win = &aven_gl_window_ctx;
            if (!aven_gl_window_update(win)) {
                win->deinit(win);
                glfwDestroyWindow(win->window);
                glfwTerminate();
                emscripten_cancel_main_loop();
            }
        }
    #endif

    typedef enum {
        AVEN_GL_WINDOW_CODE_NONE = 0,
        AVEN_GL_WINDOW_CODE_FAILED,
    } AvenGlWindowCode;

    static inline AvenGlWindowCode aven_gl_window(
        int width,
        int height,
        const char *title,
        AvenGlWindowVtable vtable
    ) {
        aven_gl_window_ctx = (AvenGlWindow){
            .width = width,
            .height = height,
            .vtable = vtable,
        };
        AvenGlWindow *win = &aven_gl_window_ctx;

        glfwInit();
        glfwWindowHint(GLFW_SAMPLES, 16);

        bool es = true;
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
        win->window = glfwCreateWindow(
            (int)width,
            (int)height,
            title,
            NULL,
            NULL
        );
        if (win->window == NULL) {
            glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);
            win->window = glfwCreateWindow(
                (int)width,
                (int)height,
                title,
                NULL,
                NULL
            );
        }
        if (win->window == NULL) {
            es = false;
            glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
            win->window = glfwCreateWindow(
                (int)width,
                (int)height,
                title,
                NULL,
                NULL
            );
            if (win->window == NULL) {
                glfwWindowHint(
                    GLFW_CONTEXT_CREATION_API,
                    GLFW_NATIVE_CONTEXT_API
                );
                win->window = glfwCreateWindow(
                    (int)width,
                    (int)height,
                    title,
                    NULL,
                    NULL
                );
            }
        }
        if (win->window == NULL) {
            return AVEN_GL_WINDOW_CODE_FAILED;
        }

    #ifdef __EMSCRIPTEN__
        emscripten_set_main_loop(aven_gl_window_update, 0, 0);
    #endif

        if (win->vtable.damage.valid) {
            glfwSetWindowRefreshCallback(
                win->window,
                aven_gl_window_damage_internal
            );
        }
        if (win->vtable.key.valid) {
            glfwSetKeyCallback(win->window, aven_gl_window_key_internal);
        }
        if (win->vtable.mouse_click.valid) {
            glfwSetMouseButtonCallback(
                win->window,
                aven_gl_window_mouse_click_internal
            );
        }
        if (win->vtable.mouse_move.valid) {
            glfwSetCursorPosCallback(
                win->window,
                aven_gl_window_mouse_move_internal
            );
        }
        if (win->vtable.mouse_enter.valid) {
            glfwSetCursorEnterCallback(
                win->window,
                aven_gl_window_mouse_enter_internal
            );
        }
    #ifdef __ANDROID__
        glfwSetWindowIconifyCallback(
            win->window,
            aven_gl_window_iconify_android_internal
        );
    #endif

        glfwMakeContextCurrent(win->window);
        glfwSwapInterval(1);

        win->gl = aven_gl_load(glfwGetProcAddress, es);

        win->vtable.init(win);
        while (aven_gl_window_update(win)) {}

        glfwDestroyWindow(win->window);
        glfwTerminate();
        *win = (AvenGlWindow){ 0 };

        return AVEN_GL_WINDOW_CODE_NONE;
    }
#endif // AVEN_GL_WINDOW_H 
