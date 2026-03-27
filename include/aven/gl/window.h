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

    AvenStr aven_gl_window_key_names_data[] = {
        [AVEN_GL_WINDOW_KEY_SPACE] = aven_str_init("SPACE"),
        [AVEN_GL_WINDOW_KEY_APOSTROPHE] = aven_str_init("APOSTROPHE"),
        [AVEN_GL_WINDOW_KEY_COMMA] = aven_str_init("COMMA"),
        [AVEN_GL_WINDOW_KEY_MINUS] = aven_str_init("MINUS"),
        [AVEN_GL_WINDOW_KEY_PERIOD] = aven_str_init("PERIOD"),
        [AVEN_GL_WINDOW_KEY_SLASH] = aven_str_init("SLASH"),
        [AVEN_GL_WINDOW_KEY_0] = aven_str_init("0"),
        [AVEN_GL_WINDOW_KEY_1] = aven_str_init("1"),
        [AVEN_GL_WINDOW_KEY_2] = aven_str_init("2"),
        [AVEN_GL_WINDOW_KEY_3] = aven_str_init("3"),
        [AVEN_GL_WINDOW_KEY_4] = aven_str_init("4"),
        [AVEN_GL_WINDOW_KEY_5] = aven_str_init("5"),
        [AVEN_GL_WINDOW_KEY_6] = aven_str_init("6"),
        [AVEN_GL_WINDOW_KEY_7] = aven_str_init("7"),
        [AVEN_GL_WINDOW_KEY_8] = aven_str_init("8"),
        [AVEN_GL_WINDOW_KEY_9] = aven_str_init("9"),
        [AVEN_GL_WINDOW_KEY_SEMICOLON] = aven_str_init("SEMICOLON"),
        [AVEN_GL_WINDOW_KEY_EQUAL] = aven_str_init("EQUAL"),
        [AVEN_GL_WINDOW_KEY_A] = aven_str_init("A"),
        [AVEN_GL_WINDOW_KEY_B] = aven_str_init("B"),
        [AVEN_GL_WINDOW_KEY_C] = aven_str_init("C"),
        [AVEN_GL_WINDOW_KEY_D] = aven_str_init("D"),
        [AVEN_GL_WINDOW_KEY_E] = aven_str_init("E"),
        [AVEN_GL_WINDOW_KEY_F] = aven_str_init("F"),
        [AVEN_GL_WINDOW_KEY_G] = aven_str_init("G"),
        [AVEN_GL_WINDOW_KEY_H] = aven_str_init("H"),
        [AVEN_GL_WINDOW_KEY_I] = aven_str_init("I"),
        [AVEN_GL_WINDOW_KEY_J] = aven_str_init("J"),
        [AVEN_GL_WINDOW_KEY_K] = aven_str_init("K"),
        [AVEN_GL_WINDOW_KEY_L] = aven_str_init("L"),
        [AVEN_GL_WINDOW_KEY_M] = aven_str_init("M"),
        [AVEN_GL_WINDOW_KEY_N] = aven_str_init("N"),
        [AVEN_GL_WINDOW_KEY_O] = aven_str_init("O"),
        [AVEN_GL_WINDOW_KEY_P] = aven_str_init("P"),
        [AVEN_GL_WINDOW_KEY_Q] = aven_str_init("Q"),
        [AVEN_GL_WINDOW_KEY_R] = aven_str_init("R"),
        [AVEN_GL_WINDOW_KEY_S] = aven_str_init("S"),
        [AVEN_GL_WINDOW_KEY_T] = aven_str_init("T"),
        [AVEN_GL_WINDOW_KEY_U] = aven_str_init("U"),
        [AVEN_GL_WINDOW_KEY_V] = aven_str_init("V"),
        [AVEN_GL_WINDOW_KEY_W] = aven_str_init("W"),
        [AVEN_GL_WINDOW_KEY_X] = aven_str_init("X"),
        [AVEN_GL_WINDOW_KEY_Y] = aven_str_init("Y"),
        [AVEN_GL_WINDOW_KEY_Z] = aven_str_init("Z"),
        [AVEN_GL_WINDOW_KEY_LEFT_BRACKET] = aven_str_init("LEFT_BRACKET"),
        [AVEN_GL_WINDOW_KEY_BACKSLASH] = aven_str_init("BACKSLASH"),
        [AVEN_GL_WINDOW_KEY_RIGHT_BRACKET] = aven_str_init("RIGHT_BRACKET"),
        [AVEN_GL_WINDOW_KEY_GRAVE_ACCENT] = aven_str_init("GRAVE_ACCENT"),
        [AVEN_GL_WINDOW_KEY_WORLD_1] = aven_str_init("WORLD_1"),
        [AVEN_GL_WINDOW_KEY_WORLD_2] = aven_str_init("WORLD_2"),
        [AVEN_GL_WINDOW_KEY_ESCAPE] = aven_str_init("ESCAPE"),
        [AVEN_GL_WINDOW_KEY_ENTER] = aven_str_init("ENTER"),
        [AVEN_GL_WINDOW_KEY_TAB] = aven_str_init("TAB"),
        [AVEN_GL_WINDOW_KEY_BACKSPACE] = aven_str_init("BACKSPACE"),
        [AVEN_GL_WINDOW_KEY_INSERT] = aven_str_init("INSERT"),
        [AVEN_GL_WINDOW_KEY_DELETE] = aven_str_init("DELETE"),
        [AVEN_GL_WINDOW_KEY_RIGHT] = aven_str_init("RIGHT"),
        [AVEN_GL_WINDOW_KEY_LEFT] = aven_str_init("LEFT"),
        [AVEN_GL_WINDOW_KEY_DOWN] = aven_str_init("DOWN"),
        [AVEN_GL_WINDOW_KEY_UP] = aven_str_init("UP"),
        [AVEN_GL_WINDOW_KEY_PAGE_UP] = aven_str_init("PAGE_UP"),
        [AVEN_GL_WINDOW_KEY_PAGE_DOWN] = aven_str_init("PAGE_DOWN"),
        [AVEN_GL_WINDOW_KEY_HOME] = aven_str_init("HOME"),
        [AVEN_GL_WINDOW_KEY_END] = aven_str_init("END"),
        [AVEN_GL_WINDOW_KEY_CAPS_LOCK] = aven_str_init("CAPS_LOCK"),
        [AVEN_GL_WINDOW_KEY_SCROLL_LOCK] = aven_str_init("SCROLL_LOCK"),
        [AVEN_GL_WINDOW_KEY_NUM_LOCK] = aven_str_init("NUM_LOCK"),
        [AVEN_GL_WINDOW_KEY_PRINT_SCREEN] = aven_str_init("PRINT_SCREEN"),
        [AVEN_GL_WINDOW_KEY_PAUSE] = aven_str_init("PAUSE"),
        [AVEN_GL_WINDOW_KEY_F1] = aven_str_init("F1"),
        [AVEN_GL_WINDOW_KEY_F2] = aven_str_init("F2"),
        [AVEN_GL_WINDOW_KEY_F3] = aven_str_init("F3"),
        [AVEN_GL_WINDOW_KEY_F4] = aven_str_init("F4"),
        [AVEN_GL_WINDOW_KEY_F5] = aven_str_init("F5"),
        [AVEN_GL_WINDOW_KEY_F6] = aven_str_init("F6"),
        [AVEN_GL_WINDOW_KEY_F7] = aven_str_init("F7"),
        [AVEN_GL_WINDOW_KEY_F8] = aven_str_init("F8"),
        [AVEN_GL_WINDOW_KEY_F9] = aven_str_init("F9"),
        [AVEN_GL_WINDOW_KEY_F10] = aven_str_init("F10"),
        [AVEN_GL_WINDOW_KEY_F11] = aven_str_init("F11"),
        [AVEN_GL_WINDOW_KEY_F12] = aven_str_init("F12"),
        [AVEN_GL_WINDOW_KEY_F13] = aven_str_init("F13"),
        [AVEN_GL_WINDOW_KEY_F14] = aven_str_init("F14"),
        [AVEN_GL_WINDOW_KEY_F15] = aven_str_init("F15"),
        [AVEN_GL_WINDOW_KEY_F16] = aven_str_init("F16"),
        [AVEN_GL_WINDOW_KEY_F17] = aven_str_init("F17"),
        [AVEN_GL_WINDOW_KEY_F18] = aven_str_init("F18"),
        [AVEN_GL_WINDOW_KEY_F19] = aven_str_init("F19"),
        [AVEN_GL_WINDOW_KEY_F20] = aven_str_init("F20"),
        [AVEN_GL_WINDOW_KEY_F21] = aven_str_init("F21"),
        [AVEN_GL_WINDOW_KEY_F22] = aven_str_init("F22"),
        [AVEN_GL_WINDOW_KEY_F23] = aven_str_init("F23"),
        [AVEN_GL_WINDOW_KEY_F24] = aven_str_init("F24"),
        [AVEN_GL_WINDOW_KEY_F25] = aven_str_init("F25"),
        [AVEN_GL_WINDOW_KEY_KP_0] = aven_str_init("KP_0"),
        [AVEN_GL_WINDOW_KEY_KP_1] = aven_str_init("KP_1"),
        [AVEN_GL_WINDOW_KEY_KP_2] = aven_str_init("KP_2"),
        [AVEN_GL_WINDOW_KEY_KP_3] = aven_str_init("KP_3"),
        [AVEN_GL_WINDOW_KEY_KP_4] = aven_str_init("KP_4"),
        [AVEN_GL_WINDOW_KEY_KP_5] = aven_str_init("KP_5"),
        [AVEN_GL_WINDOW_KEY_KP_6] = aven_str_init("KP_6"),
        [AVEN_GL_WINDOW_KEY_KP_7] = aven_str_init("KP_7"),
        [AVEN_GL_WINDOW_KEY_KP_8] = aven_str_init("KP_8"),
        [AVEN_GL_WINDOW_KEY_KP_9] = aven_str_init("KP_9"),
        [AVEN_GL_WINDOW_KEY_KP_DECIMAL] = aven_str_init("KP_DECIMAL"),
        [AVEN_GL_WINDOW_KEY_KP_DIVIDE] = aven_str_init("KP_DIVIDE"),
        [AVEN_GL_WINDOW_KEY_KP_MULTIPLY] = aven_str_init("KP_MULTIPLY"),
        [AVEN_GL_WINDOW_KEY_KP_SUBTRACT] = aven_str_init("KP_SUBTRACT"),
        [AVEN_GL_WINDOW_KEY_KP_ADD] = aven_str_init("KP_ADD"),
        [AVEN_GL_WINDOW_KEY_KP_ENTER] = aven_str_init("KP_ENTER"),
        [AVEN_GL_WINDOW_KEY_KP_EQUAL] = aven_str_init("KP_EQUAL"),
        [AVEN_GL_WINDOW_KEY_LEFT_SHIFT] = aven_str_init("LEFT_SHIFT"),
        [AVEN_GL_WINDOW_KEY_LEFT_CONTROL] = aven_str_init("LEFT_CONTROL"),
        [AVEN_GL_WINDOW_KEY_LEFT_ALT] = aven_str_init("LEFT_ALT"),
        [AVEN_GL_WINDOW_KEY_LEFT_SUPER] = aven_str_init("LEFT_SUPER"),
        [AVEN_GL_WINDOW_KEY_RIGHT_SHIFT] = aven_str_init("RIGHT_SHIFT"),
        [AVEN_GL_WINDOW_KEY_RIGHT_CONTROL] = aven_str_init("RIGHT_CONTROL"),
        [AVEN_GL_WINDOW_KEY_RIGHT_ALT] = aven_str_init("RIGHT_ALT"),
        [AVEN_GL_WINDOW_KEY_RIGHT_SUPER] = aven_str_init("RIGHT_SUPER"),
        [AVEN_GL_WINDOW_KEY_MENU] = aven_str_init("MENU"),
    };

    AvenStrSlice aven_gl_window_key_names = {
        .ptr = (AvenStr *)aven_gl_window_key_names_data,
        .len = countof(aven_gl_window_key_names_data),
    };

    static inline AvenStr aven_gl_window_key_name(AvenGlWindowKey key) {
        if (key < 0) {
            return (AvenStr){ 0 };
        }
        return get(aven_gl_window_key_names, (size_t)key);
    }

    typedef struct {
        AvenGlWindowKey key;
        uint32_t scancode;
        AvenStr name;
    } AvenGlWindowKeyInfo;

    AvenGlWindowKeyInfo aven_gl_window_get_key_info(AvenGlWindowKey key);
    AvenGlWindowKeyInfo aven_gl_window_get_key_unknown_info(uint32_t scancode);

    typedef enum {
        AVEN_GL_WINDOW_PRESS_UP = 0,
        AVEN_GL_WINDOW_PRESS_DOWN,
        AVEN_GL_WINDOW_PRESS_REPEAT,
    } AvenGlWindowPress;

    AvenStr aven_gl_window_press_names_data[] = {
        [AVEN_GL_WINDOW_PRESS_UP] = aven_str_init("UP"),
        [AVEN_GL_WINDOW_PRESS_DOWN] = aven_str_init("DOWN"),
        [AVEN_GL_WINDOW_PRESS_REPEAT] = aven_str_init("REPEAT"),
    };

    AvenStrSlice aven_gl_window_press_names = {
        .ptr = (AvenStr *)aven_gl_window_press_names_data,
        .len = countof(aven_gl_window_press_names_data),
    };

    static inline AvenStr aven_gl_window_press_name(AvenGlWindowPress press) {
        return get(aven_gl_window_press_names, press);
    }

    typedef enum {
        AVEN_GL_WINDOW_MOD_NONE = 0x0,
        AVEN_GL_WINDOW_MOD_SHFT = 0x1,
        AVEN_GL_WINDOW_MOD_CTRL = 0x2,
        AVEN_GL_WINDOW_MOD_ALT = 0x4,
        AVEN_GL_WINDOW_MOD_SUP = 0x8,
        AVEN_GL_WINDOW_MOD_CAP = 0x10,
        AVEN_GL_WINDOW_MOD_NUM = 0x20,
    } AvenGlWindowModBit;

    AvenStr aven_gl_window_mod_names_data[] = {
        [AVEN_GL_WINDOW_MOD_NONE] = aven_str_init(""),
        [AVEN_GL_WINDOW_MOD_SHFT] = aven_str_init("<SHFT>"),
        [AVEN_GL_WINDOW_MOD_CTRL] = aven_str_init("<CTRL>"),
        [AVEN_GL_WINDOW_MOD_ALT] = aven_str_init("<ALT>"),
        [AVEN_GL_WINDOW_MOD_SUP] = aven_str_init("<SUP>"),
        [AVEN_GL_WINDOW_MOD_CAP] = aven_str_init("<CAP>"),
        [AVEN_GL_WINDOW_MOD_NUM] = aven_str_init("<NUM>"),
    };

    AvenStrSlice aven_gl_window_mod_names = {
        .ptr = (AvenStr *)aven_gl_window_mod_names_data,
        .len = countof(aven_gl_window_mod_names_data),
    };

    AvenStr aven_gl_window_mod_name(AvenGlWindowModBit mod) {
        return get(aven_gl_window_mod_names, mod);
    }

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
