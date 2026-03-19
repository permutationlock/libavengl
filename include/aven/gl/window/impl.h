#ifndef AVEN_GL_WINDOW_IMPL_H
    #define AVEN_GL_WINDOW_IMPL_H

    #include <aven.h>
    #include <aven/math.h>
    #include <aven/str.h>
    #include <aven/time.h>

    #include "../../gl.h"
    #include "../window.h"

    #ifndef GLFW_INCLUDE_NONE
        #define GLFW_INCLUDE_NONE
        #include <GLFW/glfw3.h>
        #undef GLFW_INCLUDE_NONE
    #else
        #include <GLFW/glfw3.h>
    #endif

    #if defined(AVEN_GL_NDEBUG) and !defined(AVEN_GL_WINDOW_NDEBUG)
        #define AVEN_GL_WINDOW_NDEBUG
    #endif

    #ifdef __EMSCRIPTEN__
        #include <emscripten.h>
        #ifndef AVEN_GL_WINDOW_NDEBUG
            #define AVEN_GL_WINDOW_NDEBUG
        #endif
    #endif

    AvenGlWindowKeyInfo aven_gl_window_get_key_info(AvenGlWindowKey key) {
        int scancode = glfwGetKeyScancode(key);

        char *res = (char *)glfwGetKeyName(
            AVEN_GL_WINDOW_KEY_NONE,
            (int)scancode
        );
        if (res == NULL) {
            res = "";
        }

        AvenStr name = aven_str_cstr(res);

        return (AvenGlWindowKeyInfo){
            .key = key,
            .scancode = (uint32_t)scancode,
            .name = name,
        };
    }

    AvenGlWindowKeyInfo aven_gl_window_get_key_unknown_info(uint32_t scancode) {
        char *res = (char *)glfwGetKeyName(
            AVEN_GL_WINDOW_KEY_NONE,
            (int)scancode
        );
        if (res == NULL) {
            res = "";
        }

        AvenStr name = aven_str_cstr(res);

        return (AvenGlWindowKeyInfo){
            .key = AVEN_GL_WINDOW_KEY_NONE,
            .scancode = (uint32_t)scancode,
            .name = name,
        };
    }

    static AvenGlWindow aven_gl_window_impl_ctx;

    static void aven_gl_window_impl_damage_internal(GLFWwindow *window) {
        (void)window;
        AvenGlWindow *win = &aven_gl_window_impl_ctx;
        glfwGetFramebufferSize(win->window, &win->width, &win->height);
        unwrap(win->vtable.damage)(win);
    }

    static void aven_gl_window_impl_key_internal(
        GLFWwindow *window,
        int key,
        int scancode,
        int action,
        int mods
    ) {
        (void)window;
        AvenGlWindow *win = &aven_gl_window_impl_ctx;
        unwrap(win->vtable.key)(
            win,
            (AvenGlWindowKey)key,
            (uint32_t)scancode,
            (AvenGlWindowPress)action,
            (uint32_t)mods
        );
    }

    static void aven_gl_window_impl_mouse_click_internal(
        GLFWwindow *window,
        int button,
        int action,
        int mods
    ) {
        (void)window;
        AvenGlWindow *win = &aven_gl_window_impl_ctx;
        double x;
        double y;
        glfwGetCursorPos(win->window, &x, &y);
        unwrap(win->vtable.mouse_click)(
            win,
            (Vec2){ (float)x, (float)y },
            (AvenGlWindowMouse)button,
            (AvenGlWindowPress)action,
            (uint32_t)mods
        );
    }

    static void aven_gl_window_impl_mouse_move_internal(
        GLFWwindow *window,
        double x,
        double y
    ) {
        (void)window;
        AvenGlWindow *win = &aven_gl_window_impl_ctx;
        unwrap(win->vtable.mouse_move)(win, (Vec2){ (float)x, (float)y });
    }

    static void aven_gl_window_impl_mouse_enter_internal(
        GLFWwindow *window,
        int entered
    ) {
        (void)window;
        AvenGlWindow *win = &aven_gl_window_impl_ctx;
        unwrap(win->vtable.mouse_enter)(win, entered);
    }

    #ifdef __ANDROID__
        static void aven_gl_window_impl_iconify_android_internal(
            GLFWwindow *window,
            int iconified
        ) {
            (void)window;
            AvenGlWindow *win = &aven_gl_window_impl_ctx;
            if (iconified) {
                win->vtable.deinit(win);
                win->minimized = true;
            } else {
                win->vtable.init(win);
                win->minimized = false;
            }
        }
    #endif

    static bool aven_gl_window_impl_update(AvenGlWindow *win) {
        glfwPollEvents();
    #ifdef __ANDROID__
        while (win->minimized) {
            glfwWaitEvents();
        }
    #endif
        if (glfwWindowShouldClose(win->window)) {
            return false;
        }
        glfwGetFramebufferSize(win->window, &win->width, &win->height);
        win->last = win->now;
        win->now = aven_time_now();
        switch (win->vtable.update(win)) {
            case AVEN_GL_WINDOW_ACTION_NONE:
                aven_time_sleep_ms(AVEN_TIME_MSEC_PER_SEC / 30);
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
        void aven_gl_window_impl_emscripten_resize(int width, int height) {
            AvenGlWindow *win = &aven_gl_window_impl_ctx;
            glfwSetWindowSize(win->window, width, height);
            win->width = width;
            win->height = height;
            if (win->vtable.damage.valid) {
                unwrap(win->vtable.damage)(win);
            }
        }

        static void aven_gl_window_impl_emscripten_main_loop(void) {
            AvenGlWindow *win = &aven_gl_window_impl_ctx;
            if (!aven_gl_window_impl_update(win)) {
                win->vtable.deinit(win);
                glfwDestroyWindow(win->window);
                glfwTerminate();
                emscripten_cancel_main_loop();
            }
        }
    #endif

    #ifdef _WIN32
        void *glfwGetWin32Window(void *window);
        typedef void TimerCallbackFn(
            void *p1,
            unsigned int p2,
            unsigned int p3,
            uint32_t p4
        );
        AVEN_WIN32_FN(int) SetTimer(
            void *hwnd,
            unsigned int id,
            unsigned int tstep,
            TimerCallbackFn *callback
        );

        static void aven_gl_window_impl_win32_timestep(
            void *p1,
            unsigned int p2,
            unsigned int p3,
            uint32_t p4
        ) {
            (void)p1;
            (void)p2;
            (void)p3;
            (void)p4;

            AvenGlWindow *win = &aven_gl_window_impl_ctx;
            if (glfwWindowShouldClose(win->window)) {
                return;
            }
            glfwGetFramebufferSize(win->window, &win->width, &win->height);
            win->last = win->now;
            win->now = aven_time_now();
            switch (win->vtable.update(win)) {
                case AVEN_GL_WINDOW_ACTION_NONE:
                    break;
                case AVEN_GL_WINDOW_ACTION_SWAP:
                    glfwSwapBuffers(win->window);
                    break;
                case AVEN_GL_WINDOW_ACTION_CLOSE:
                    win->vtable.deinit(win);
                    glfwSetWindowShouldClose(win->window, GLFW_TRUE);
                    break;
            }
        }
    #endif

    #ifndef AVEN_GL_WINDOW_NDEBUG
        static void APIENTRY aven_gl_window_impl_debug(
            GLenum source,
            GLenum type,
            GLuint id,
            GLenum severity,
            GLsizei length,
            const GLchar *message,
            const void *userParam
        ) {
            (void)userParam;
            AvenStr src_str;
            switch (source) {
                case GL_DEBUG_SOURCE_API:
                    src_str = aven_str("API");
                    break;
                case GL_DEBUG_SOURCE_APPLICATION:
                    src_str = aven_str("APPLICATION");
                    break;
                case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
                    src_str = aven_str("WINDOW_SYSTEM");
                    break;
                case GL_DEBUG_SOURCE_SHADER_COMPILER:
                    src_str = aven_str("SHADER_COMPILER");
                    break;
                case GL_DEBUG_SOURCE_THIRD_PARTY:
                    src_str = aven_str("THIRD_PARTY");
                    break;
                default:
                    src_str = aven_str("OTHER");
                    break;
            }

            AvenStr type_str;
            switch (type) {
                case GL_DEBUG_TYPE_ERROR:
                    type_str = aven_str("ERROR");
                    break;
                case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
                    type_str = aven_str("DEPRECATED_BEHAVIOR");
                    break;
                case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
                    type_str = aven_str("UNDEFINED_BEHAVIOR");
                    break;
                case GL_DEBUG_TYPE_PORTABILITY:
                    type_str = aven_str("PORTABILITY");
                    break;
                case GL_DEBUG_TYPE_PERFORMANCE:
                    type_str = aven_str("PERFORMANCE");
                    break;
                case GL_DEBUG_TYPE_MARKER:
                    type_str = aven_str("MARKER");
                    break;
                case GL_DEBUG_TYPE_PUSH_GROUP:
                    type_str = aven_str("PUSH_GROUP");
                    break;
                case GL_DEBUG_TYPE_POP_GROUP:
                    type_str = aven_str("POP_GROUP");
                    break;
                default:
                    type_str = aven_str("OTHER");
                    break;
            }

            AvenStr sev_str;
            switch (severity) {
                case GL_DEBUG_SEVERITY_LOW:
                    sev_str = aven_str("LOW");
                    break;
                case GL_DEBUG_SEVERITY_MEDIUM:
                    sev_str = aven_str("MEDIUM");
                    break;
                case GL_DEBUG_SEVERITY_HIGH:
                    sev_str = aven_str("HIGH");
                    break;
                case GL_DEBUG_SEVERITY_NOTIFICATION:
                    sev_str = aven_str("NOTIFICATION");
                    break;
                default:
                    sev_str = aven_str("UNKNOWN");
                    break;
            }

            AvenStr msg_str = { .ptr = (char *)message, .len = (size_t)length };

            aven_io_perrf(
                "GL {} {} {} ({}): {}\n",
                aven_fmt_str(src_str),
                aven_fmt_str(type_str),
                aven_fmt_uint(id),
                aven_fmt_str(sev_str),
                aven_fmt_str(msg_str)
            );
        }
    #endif

    typedef enum {
        AVEN_GL_WINDOW_CODE_NONE = 0,
        AVEN_GL_WINDOW_CODE_FAILED,
    } AvenGlWindowCode;

    static inline AvenGlWindowCode aven_gl_window_impl(
        int width,
        int height,
        const char *title,
        AvenGlWindowVtable vtable,
        void *ctx
    ) {
        aven_gl_window_impl_ctx = (AvenGlWindow){
            .ctx = ctx,
            .width = width,
            .height = height,
            .vtable = vtable,
        };
        AvenGlWindow *win = &aven_gl_window_impl_ctx;

        glfwInit();
        glfwWindowHint(GLFW_SAMPLES, 16);
    #ifdef AVEN_GL_WINDOW_NDEBUG
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_FALSE);
    #else
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    #endif

        bool es = true;
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
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
        emscripten_set_main_loop(aven_gl_window_impl_emscripten_main_loop, 0, 0);
    #endif

        if (win->vtable.damage.valid) {
            glfwSetWindowRefreshCallback(
                win->window,
                aven_gl_window_impl_damage_internal
            );
        }
        if (win->vtable.key.valid) {
            glfwSetKeyCallback(win->window, aven_gl_window_impl_key_internal);
        }
        if (win->vtable.mouse_click.valid) {
            glfwSetMouseButtonCallback(
                win->window,
                aven_gl_window_impl_mouse_click_internal
            );
        }
        if (win->vtable.mouse_move.valid) {
            glfwSetCursorPosCallback(
                win->window,
                aven_gl_window_impl_mouse_move_internal
            );
        }
        if (win->vtable.mouse_enter.valid) {
            glfwSetCursorEnterCallback(
                win->window,
                aven_gl_window_impl_mouse_enter_internal
            );
        }
    #ifdef __ANDROID__
        glfwSetWindowIconifyCallback(
            win->window,
            aven_gl_window_impl_iconify_android_internal
        );
    #endif

        glfwMakeContextCurrent(win->window);
        glfwSwapInterval(1);

        win->gl = aven_gl_load(glfwGetProcAddress, es);
    #ifndef AVEN_GL_WINDOW_NDEBUG
        win->gl.DebugMessageCallback(aven_gl_window_impl_debug, NULL);
    #endif

        win->vtable.init(win);

    #ifdef _WIN32
        int success = SetTimer(
            glfwGetWin32Window(win->window),
            1,
            AVEN_TIME_MSEC_PER_SEC / 60,
            aven_gl_window_impl_win32_timestep
        );
        if (!success) {
            aven_panic("failed to set window timer");
        }
    #endif

    #ifndef __EMSCRIPTEN__
        while (aven_gl_window_impl_update(win)) {}

        glfwDestroyWindow(win->window);
        glfwTerminate();
        *win = (AvenGlWindow){ 0 };
    #endif

        return AVEN_GL_WINDOW_CODE_NONE;
    }
#endif // AVEN_GL_WINDOW_IMPL_H 
