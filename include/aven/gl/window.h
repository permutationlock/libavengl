#ifndef AVEN_GL_WINDOW_H
    #define AVEN_GL_WINDOW_H

    #include <aven.h>
    #include <aven/math.h>
    #include <aven/time.h>

    #include "../gl.h"

    typedef enum {
        AVEN_GL_WINDOW_ACTION_NONE = 0,
        AVEN_GL_WINDOW_ACTION_SWAP,
        AVEN_GL_WINDOW_ACTION_CLOSE,
    } AvenGlWindowAction;

    typedef enum {
        AVEN_GL_WINDOW_KEY_NONE = -1,
        AVEN_GL_WINDOW_KEY_SPACE = 32,
        AVEN_GL_WINDOW_KEY_APOSTROPHE = 39,
        AVEN_GL_WINDOW_KEY_COMMA = 44,
        AVEN_GL_WINDOW_KEY_MINUS = 45,
        AVEN_GL_WINDOW_KEY_PERIOD = 46,
        AVEN_GL_WINDOW_KEY_SLASH = 47,
        AVEN_GL_WINDOW_KEY_0 = 48,
        AVEN_GL_WINDOW_KEY_1 = 49,
        AVEN_GL_WINDOW_KEY_2 = 50,
        AVEN_GL_WINDOW_KEY_3 = 51,
        AVEN_GL_WINDOW_KEY_4 = 52,
        AVEN_GL_WINDOW_KEY_5 = 53,
        AVEN_GL_WINDOW_KEY_6 = 54,
        AVEN_GL_WINDOW_KEY_7 = 55,
        AVEN_GL_WINDOW_KEY_8 = 56,
        AVEN_GL_WINDOW_KEY_9 = 57,
        AVEN_GL_WINDOW_KEY_SEMICOLON = 59,
        AVEN_GL_WINDOW_KEY_EQUAL = 61,
        AVEN_GL_WINDOW_KEY_A = 65,
        AVEN_GL_WINDOW_KEY_B = 66,
        AVEN_GL_WINDOW_KEY_C = 67,
        AVEN_GL_WINDOW_KEY_D = 68,
        AVEN_GL_WINDOW_KEY_E = 69,
        AVEN_GL_WINDOW_KEY_F = 70,
        AVEN_GL_WINDOW_KEY_G = 71,
        AVEN_GL_WINDOW_KEY_H = 72,
        AVEN_GL_WINDOW_KEY_I = 73,
        AVEN_GL_WINDOW_KEY_J = 74,
        AVEN_GL_WINDOW_KEY_K = 75,
        AVEN_GL_WINDOW_KEY_L = 76,
        AVEN_GL_WINDOW_KEY_M = 77,
        AVEN_GL_WINDOW_KEY_N = 78,
        AVEN_GL_WINDOW_KEY_O = 79,
        AVEN_GL_WINDOW_KEY_P = 80,
        AVEN_GL_WINDOW_KEY_Q = 81,
        AVEN_GL_WINDOW_KEY_R = 82,
        AVEN_GL_WINDOW_KEY_S = 83,
        AVEN_GL_WINDOW_KEY_T = 84,
        AVEN_GL_WINDOW_KEY_U = 85,
        AVEN_GL_WINDOW_KEY_V = 86,
        AVEN_GL_WINDOW_KEY_W = 87,
        AVEN_GL_WINDOW_KEY_X = 88,
        AVEN_GL_WINDOW_KEY_Y = 89,
        AVEN_GL_WINDOW_KEY_Z = 90,
        AVEN_GL_WINDOW_KEY_LEFT_BRACKET = 91,
        AVEN_GL_WINDOW_KEY_BACKSLASH = 92,
        AVEN_GL_WINDOW_KEY_RIGHT_BRACKET = 93,
        AVEN_GL_WINDOW_KEY_GRAVE_ACCENT = 96,
        AVEN_GL_WINDOW_KEY_WORLD_1 = 161,
        AVEN_GL_WINDOW_KEY_WORLD_2 = 162,
        AVEN_GL_WINDOW_KEY_ESCAPE = 256,
        AVEN_GL_WINDOW_KEY_ENTER = 257,
        AVEN_GL_WINDOW_KEY_TAB = 258,
        AVEN_GL_WINDOW_KEY_BACKSPACE = 259,
        AVEN_GL_WINDOW_KEY_INSERT = 260,
        AVEN_GL_WINDOW_KEY_DELETE = 261,
        AVEN_GL_WINDOW_KEY_RIGHT = 262,
        AVEN_GL_WINDOW_KEY_LEFT = 263,
        AVEN_GL_WINDOW_KEY_DOWN = 264,
        AVEN_GL_WINDOW_KEY_UP = 265,
        AVEN_GL_WINDOW_KEY_PAGE_UP = 266,
        AVEN_GL_WINDOW_KEY_PAGE_DOWN = 267,
        AVEN_GL_WINDOW_KEY_HOME = 268,
        AVEN_GL_WINDOW_KEY_END = 269,
        AVEN_GL_WINDOW_KEY_CAPS_LOCK = 280,
        AVEN_GL_WINDOW_KEY_SCROLL_LOCK = 281,
        AVEN_GL_WINDOW_KEY_NUM_LOCK = 282,
        AVEN_GL_WINDOW_KEY_PRINT_SCREEN = 283,
        AVEN_GL_WINDOW_KEY_PAUSE = 284,
        AVEN_GL_WINDOW_KEY_F1 = 290,
        AVEN_GL_WINDOW_KEY_F2 = 291,
        AVEN_GL_WINDOW_KEY_F3 = 292,
        AVEN_GL_WINDOW_KEY_F4 = 293,
        AVEN_GL_WINDOW_KEY_F5 = 294,
        AVEN_GL_WINDOW_KEY_F6 = 295,
        AVEN_GL_WINDOW_KEY_F7 = 296,
        AVEN_GL_WINDOW_KEY_F8 = 297,
        AVEN_GL_WINDOW_KEY_F9 = 298,
        AVEN_GL_WINDOW_KEY_F10 = 299,
        AVEN_GL_WINDOW_KEY_F11 = 300,
        AVEN_GL_WINDOW_KEY_F12 = 301,
        AVEN_GL_WINDOW_KEY_F13 = 302,
        AVEN_GL_WINDOW_KEY_F14 = 303,
        AVEN_GL_WINDOW_KEY_F15 = 304,
        AVEN_GL_WINDOW_KEY_F16 = 305,
        AVEN_GL_WINDOW_KEY_F17 = 306,
        AVEN_GL_WINDOW_KEY_F18 = 307,
        AVEN_GL_WINDOW_KEY_F19 = 308,
        AVEN_GL_WINDOW_KEY_F20 = 309,
        AVEN_GL_WINDOW_KEY_F21 = 310,
        AVEN_GL_WINDOW_KEY_F22 = 311,
        AVEN_GL_WINDOW_KEY_F23 = 312,
        AVEN_GL_WINDOW_KEY_F24 = 313,
        AVEN_GL_WINDOW_KEY_F25 = 314,
        AVEN_GL_WINDOW_KEY_KP_0 = 320,
        AVEN_GL_WINDOW_KEY_KP_1 = 321,
        AVEN_GL_WINDOW_KEY_KP_2 = 322,
        AVEN_GL_WINDOW_KEY_KP_3 = 323,
        AVEN_GL_WINDOW_KEY_KP_4 = 324,
        AVEN_GL_WINDOW_KEY_KP_5 = 325,
        AVEN_GL_WINDOW_KEY_KP_6 = 326,
        AVEN_GL_WINDOW_KEY_KP_7 = 327,
        AVEN_GL_WINDOW_KEY_KP_8 = 328,
        AVEN_GL_WINDOW_KEY_KP_9 = 329,
        AVEN_GL_WINDOW_KEY_KP_DECIMAL = 330,
        AVEN_GL_WINDOW_KEY_KP_DIVIDE = 331,
        AVEN_GL_WINDOW_KEY_KP_MULTIPLY = 332,
        AVEN_GL_WINDOW_KEY_KP_SUBTRACT = 333,
        AVEN_GL_WINDOW_KEY_KP_ADD = 334,
        AVEN_GL_WINDOW_KEY_KP_ENTER = 335,
        AVEN_GL_WINDOW_KEY_KP_EQUAL = 336,
        AVEN_GL_WINDOW_KEY_LEFT_SHIFT = 340,
        AVEN_GL_WINDOW_KEY_LEFT_CONTROL = 341,
        AVEN_GL_WINDOW_KEY_LEFT_ALT = 342,
        AVEN_GL_WINDOW_KEY_LEFT_SUPER = 343,
        AVEN_GL_WINDOW_KEY_RIGHT_SHIFT = 344,
        AVEN_GL_WINDOW_KEY_RIGHT_CONTROL = 345,
        AVEN_GL_WINDOW_KEY_RIGHT_ALT = 346,
        AVEN_GL_WINDOW_KEY_RIGHT_SUPER = 347,
        AVEN_GL_WINDOW_KEY_MENU = 348,
        AVEN_GL_WINDOW_KEY_MAX,
    } AvenGlWindowKey;

    typedef enum {
        AVEN_GL_WINDOW_PRESS_UP = 0,
        AVEN_GL_WINDOW_PRESS_DOWN,
        AVEN_GL_WINDOW_PRESS_REPEAT,
    } AvenGlWindowPress;

    typedef enum {
        AVEN_GL_WINDOW_MOD_SHFT = 0x1,
        AVEN_GL_WINDOW_MOD_CTRL = 0x2,
        AVEN_GL_WINDOW_MOD_ALT = 0x4,
        AVEN_GL_WINDOW_MOD_SUP = 0x8,
        AVEN_GL_WINDOW_MOD_CAP = 0x10,
        AVEN_GL_WINDOW_MOD_NUM = 0x20,
    } AvenGlWindowModBit;

    typedef enum {
        AVEN_GL_WINDOW_MOUSE_LEFT = 0,
        AVEN_GL_WINDOW_MOUSE_RIGHT,
        AVEN_GL_WINDOW_MOUSE_MIDDLE,
        AVEN_GL_WINDOW_MOUSE_EXTRA1,
        AVEN_GL_WINDOW_MOUSE_EXTRA2,
        AVEN_GL_WINDOW_MOUSE_EXTRA3,
        AVEN_GL_WINDOW_MOUSE_EXTRA4,
        AVEN_GL_WINDOW_MOUSE_EXTRA5,
    } AvenGlWindowMouse;

    typedef struct AvenGlWindow AvenGlWindow;
    typedef void AvenGlWindowVoidFn(AvenGlWindow *win);
    typedef AvenGlWindowAction AvenGlWindowUpdateFn(AvenGlWindow *win);
    typedef void AvenGlWindowMouseClickFn(
        AvenGlWindow *win,
        Vec2 pos,
        AvenGlWindowMouse button,
        AvenGlWindowPress action,
        uint32_t mods
    );
    typedef void AvenGlWindowMouseMoveFn(AvenGlWindow *win, Vec2 pos);
    typedef void AvenGlWindowMouseEnterFn(AvenGlWindow *win, bool entered);
    typedef void AvenGlWindowKeyFn(
        AvenGlWindow *win,
        AvenGlWindowKey key,
        uint32_t scancode,
        AvenGlWindowPress action,
        uint32_t mods
    );

    typedef struct {
        AvenGlWindowVoidFn *init;
        AvenGlWindowVoidFn *deinit;
        AvenGlWindowUpdateFn *update;
        OptPtr(AvenGlWindowVoidFn) damage;
        OptPtr(AvenGlWindowMouseClickFn) mouse_click;
        OptPtr(AvenGlWindowMouseMoveFn) mouse_move;
        OptPtr(AvenGlWindowMouseEnterFn) mouse_enter;
        OptPtr(AvenGlWindowKeyFn) key;
    } AvenGlWindowVtable;

    struct AvenGlWindow {
        AvenGl gl;
        void *window;
        void *ctx;
        AvenGlWindowVtable vtable;
        AvenTimeInst last;
        AvenTimeInst now;
        int width;
        int height;
        bool minimized;
    };
#endif // AVEN_GL_WINDOW_H
