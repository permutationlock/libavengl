#ifndef AVEN_GL_WINDOW_H
    #define AVEN_GL_WINDOW_H

    #include <aven.h>
    #include <aven/math.h>
    #include <aven/time.h>

    #include "../gl.h"

    #include <GLFW/glfw3.h>

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
        Vec2 pos,
        int button,
        int action,
        int modes
    );
    typedef void AvenGlWindowMouseMoveFn(AvenGlWindow *win, Vec2 pos);
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
        bool minimized;
    };
#endif // AVEN_GL_WINDOW_H 
