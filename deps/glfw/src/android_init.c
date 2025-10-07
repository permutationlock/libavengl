//========================================================================
// GLFW 3.5 - www.glfw.org
//------------------------------------------------------------------------
// Copyright (c) 2024 kunitoki <kunitoki@gmail.com>
// Copyright (c) 2017 Curi0 <curi0minecraft@gmail.com>
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would
//    be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source
//    distribution.
//
//========================================================================

#include <assert.h>

#include <android_native_app_glue.h>
#include <android/keycodes.h>
#include <android/native_window.h>
#include <android/log.h>
#include <jni.h>

#include "internal.h"

struct android_app *_globalAndroidApp = NULL;

extern int main(void);

void handleAppCmd(struct android_app *app, int32_t cmd) {
    switch (cmd) {
        case APP_CMD_START:
            break;

        case APP_CMD_RESUME:
            break;

        case APP_CMD_PAUSE:
            break;

        case APP_CMD_STOP:
            break;

        case APP_CMD_DESTROY:
            break;

        case APP_CMD_INIT_WINDOW:
            if (!_glfw.gstate.suspended) {
                break;
            }
            _GLFWwndconfig wndconfig = _glfw.hints.window;
            _GLFWfbconfig fbconfig = _glfw.hints.framebuffer;
            _GLFWctxconfig ctxconfig = _glfw.hints.context;
            GLFWbool created = _glfwCreateWindowAndroid(
                _glfw.windowListHead,
                &wndconfig,
                &ctxconfig,
                &fbconfig
            );
            assert(created);
            _glfwInputWindowIconify(_glfw.windowListHead, GLFW_FALSE);
            _glfw.gstate.suspended = false;
            break;

        case APP_CMD_TERM_WINDOW:
            _glfw.gstate.suspended = true;
            _glfwInputWindowIconify(_glfw.windowListHead, GLFW_TRUE);
            _glfwDestroyWindowAndroid(_glfw.windowListHead);
            break;

        case APP_CMD_LOST_FOCUS:
            _glfwInputWindowFocus(_glfw.windowListHead, GLFW_FALSE);
            break;

        case APP_CMD_GAINED_FOCUS:
            _glfwInputWindowFocus(_glfw.windowListHead, GLFW_TRUE);
            break;

        case APP_CMD_WINDOW_RESIZED:
            _glfwInputWindowSize(
                _glfw.windowListHead,
                ANativeWindow_getWidth(app->window),
                ANativeWindow_getHeight(app->window)
            );
            break;

        case APP_CMD_WINDOW_REDRAW_NEEDED:
            _glfwInputWindowDamage(_glfw.windowListHead);
            break;

        case APP_CMD_CONTENT_RECT_CHANGED:
            _glfwInputFramebufferSize(
                _glfw.windowListHead,
                ANativeWindow_getWidth(app->window),
                ANativeWindow_getHeight(app->window)
            );
            break;

        case APP_CMD_CONFIG_CHANGED:
            break;

        case APP_CMD_INPUT_CHANGED:
            break;
    }
}

// Android Entry Point
void android_main(struct android_app *app) {
    app->onAppCmd = handleAppCmd;

    _globalAndroidApp = app;

    JNIEnv *envptr = NULL;
    JavaVM jnii = *app->activity->vm;
    jnii->AttachCurrentThread(app->activity->vm, &envptr, NULL);

    JNIEnv env = (*envptr);

    jclass activityClass = env->FindClass(envptr, "android/app/NativeActivity");
    jmethodID getWindow = env->GetMethodID(envptr, activityClass, "getWindow", "()Landroid/view/Window;");
    jobject jwindow = env->CallObjectMethod(envptr, app->activity->clazz, getWindow);
    jclass windowClass = env->FindClass(envptr, "android/view/Window");
    jmethodID getDecorView = env->GetMethodID(envptr, windowClass, "getDecorView", "()Landroid/view/View;");
    jobject decorView = env->CallObjectMethod(envptr, jwindow, getDecorView);

    jclass viewClass = env->FindClass(envptr, "android/view/View");
    const int flagLayoutHideNavigation = env->GetStaticIntField(envptr, viewClass, env->GetStaticFieldID(envptr, viewClass, "SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION", "I"));
    const int flagLayoutFullscreen = env->GetStaticIntField(envptr, viewClass, env->GetStaticFieldID(envptr, viewClass, "SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN", "I"));
    const int flagLowProfile = env->GetStaticIntField(envptr, viewClass, env->GetStaticFieldID(envptr, viewClass, "SYSTEM_UI_FLAG_LOW_PROFILE", "I"));
    const int flagHideNavigation = env->GetStaticIntField(envptr, viewClass, env->GetStaticFieldID(envptr, viewClass, "SYSTEM_UI_FLAG_HIDE_NAVIGATION", "I"));
    const int flagFullscreen = env->GetStaticIntField(envptr, viewClass, env->GetStaticFieldID(envptr, viewClass, "SYSTEM_UI_FLAG_FULLSCREEN", "I"));
    const int flagImmersiveSticky = env->GetStaticIntField(envptr, viewClass, env->GetStaticFieldID(envptr, viewClass, "SYSTEM_UI_FLAG_IMMERSIVE_STICKY", "I"));
    const int flagLayoutStable = env->GetStaticIntField(envptr, viewClass, env->GetStaticFieldID(envptr, viewClass, "SYSTEM_UI_FLAG_LAYOUT_STABLE", "I"));
    jmethodID setSystemUiVisibility = env->GetMethodID(envptr, viewClass, "setSystemUiVisibility", "(I)V");

    env->CallVoidMethod(envptr, decorView, setSystemUiVisibility,
            (flagLayoutHideNavigation | flagLayoutFullscreen | flagLowProfile | flagHideNavigation | flagFullscreen | flagImmersiveSticky | flagLayoutStable));

    jclass layoutManagerClass = env->FindClass(envptr, "android/view/WindowManager$LayoutParams");
    const int flag_WinMan_Fullscreen = env->GetStaticIntField(envptr, layoutManagerClass, (env->GetStaticFieldID(envptr, layoutManagerClass, "FLAG_FULLSCREEN", "I")));
    const int flag_WinMan_KeepScreenOn = env->GetStaticIntField(envptr, layoutManagerClass, (env->GetStaticFieldID(envptr, layoutManagerClass, "FLAG_KEEP_SCREEN_ON", "I")));
    const int flag_WinMan_hw_acc = env->GetStaticIntField(envptr, layoutManagerClass, (env->GetStaticFieldID(envptr, layoutManagerClass, "FLAG_HARDWARE_ACCELERATED", "I")));

    env->CallVoidMethod(envptr, jwindow, (env->GetMethodID(envptr, windowClass, "addFlags" , "(I)V")),
        (flag_WinMan_Fullscreen | flag_WinMan_KeepScreenOn | flag_WinMan_hw_acc));

    jnii->DetachCurrentThread(app->activity->vm);

    main();
}

//////////////////////////////////////////////////////////////////////////
//////                       GLFW platform API                      //////
//////////////////////////////////////////////////////////////////////////

GLFWbool _glfwConnectAndroid(int platformID, _GLFWplatform *platform) {
    const _GLFWplatform android = {
        .platformID = GLFW_PLATFORM_ANDROID,
        .init = _glfwInitAndroid,
        .terminate = _glfwTerminateAndroid,
        .getCursorPos = _glfwGetCursorPosAndroid,
        .setCursorPos = _glfwSetCursorPosAndroid,
        .setCursorMode = _glfwSetCursorModeAndroid,
        .setRawMouseMotion = _glfwSetRawMouseMotionAndroid,
        .rawMouseMotionSupported = _glfwRawMouseMotionSupportedAndroid,
        .createCursor = _glfwCreateCursorAndroid,
        .createStandardCursor = _glfwCreateStandardCursorAndroid,
        .destroyCursor = _glfwDestroyCursorAndroid,
        .setCursor = _glfwSetCursorAndroid,
        .getScancodeName = _glfwGetScancodeNameAndroid,
        .getKeyScancode = _glfwGetKeyScancodeAndroid,
        .setClipboardString = _glfwSetClipboardStringAndroid,
        .getClipboardString = _glfwGetClipboardStringAndroid,
        .initJoysticks = _glfwInitJoysticksAndroid,
        .terminateJoysticks = _glfwTerminateJoysticksAndroid,
        .pollJoystick = _glfwPollJoystickAndroid,
        .getMappingName = _glfwGetMappingNameAndroid,
        .updateGamepadGUID = _glfwUpdateGamepadGUIDAndroid,
        .freeMonitor = _glfwFreeMonitorAndroid,
        .getMonitorPos = _glfwGetMonitorPosAndroid,
        .getMonitorContentScale = _glfwGetMonitorContentScaleAndroid,
        .getMonitorWorkarea = _glfwGetMonitorWorkareaAndroid,
        .getVideoModes = _glfwGetVideoModesAndroid,
        .getVideoMode = _glfwGetVideoModeAndroid,
        .getGammaRamp = _glfwGetGammaRampAndroid,
        .setGammaRamp = _glfwSetGammaRampAndroid,
        .createWindow = _glfwCreateWindowAndroid,
        .destroyWindow = _glfwDestroyWindowAndroid,
        .setWindowTitle = _glfwSetWindowTitleAndroid,
        .setWindowIcon = _glfwSetWindowIconAndroid,
        .getWindowPos = _glfwGetWindowPosAndroid,
        .setWindowPos = _glfwSetWindowPosAndroid,
        .getWindowSize = _glfwGetWindowSizeAndroid,
        .setWindowSize = _glfwSetWindowSizeAndroid,
        .setWindowSizeLimits = _glfwSetWindowSizeLimitsAndroid,
        .setWindowAspectRatio = _glfwSetWindowAspectRatioAndroid,
        .getFramebufferSize = _glfwGetFramebufferSizeAndroid,
        .getWindowFrameSize = _glfwGetWindowFrameSizeAndroid,
        .getWindowContentScale = _glfwGetWindowContentScaleAndroid,
        .iconifyWindow = _glfwIconifyWindowAndroid,
        .restoreWindow = _glfwRestoreWindowAndroid,
        .maximizeWindow = _glfwMaximizeWindowAndroid,
        .showWindow = _glfwShowWindowAndroid,
        .hideWindow = _glfwHideWindowAndroid,
        .requestWindowAttention = _glfwRequestWindowAttentionAndroid,
        .focusWindow = _glfwFocusWindowAndroid,
        .setWindowMonitor = _glfwSetWindowMonitorAndroid,
        .windowFocused = _glfwWindowFocusedAndroid,
        .windowIconified = _glfwWindowIconifiedAndroid,
        .windowVisible = _glfwWindowVisibleAndroid,
        .windowMaximized = _glfwWindowMaximizedAndroid,
        .windowHovered = _glfwWindowHoveredAndroid,
        .framebufferTransparent = _glfwFramebufferTransparentAndroid,
        .getWindowOpacity = _glfwGetWindowOpacityAndroid,
        .setWindowResizable = _glfwSetWindowResizableAndroid,
        .setWindowDecorated = _glfwSetWindowDecoratedAndroid,
        .setWindowFloating = _glfwSetWindowFloatingAndroid,
        .setWindowOpacity = _glfwSetWindowOpacityAndroid,
        .setWindowMousePassthrough = _glfwSetWindowMousePassthroughAndroid,
        .pollEvents = _glfwPollEventsAndroid,
        .waitEvents = _glfwWaitEventsAndroid,
        .waitEventsTimeout = _glfwWaitEventsTimeoutAndroid,
        .postEmptyEvent = _glfwPostEmptyEventAndroid,
        .getEGLPlatform = _glfwGetEGLPlatformAndroid,
        .getEGLNativeDisplay = _glfwGetEGLNativeDisplayAndroid,
        .getEGLNativeWindow = _glfwGetEGLNativeWindowAndroid,
        .getRequiredInstanceExtensions =
            _glfwGetRequiredInstanceExtensionsAndroid,
        .getPhysicalDevicePresentationSupport =
            _glfwGetPhysicalDevicePresentationSupportAndroid,
        .createWindowSurface = _glfwCreateWindowSurfaceAndroid,
    };

    *platform = android;
    return GLFW_TRUE;
}

// Create key code translation tables
//
static void createAndroidKeyTables(void)
{
    memset(_glfw.gstate.keycodes, -1, sizeof(_glfw.gstate.keycodes));
    memset(_glfw.gstate.scancodes, -1, sizeof(_glfw.gstate.scancodes));

    _glfw.gstate.keycodes[AKEYCODE_GRAVE]      = GLFW_KEY_GRAVE_ACCENT;
    _glfw.gstate.keycodes[AKEYCODE_1]          = GLFW_KEY_1;
    _glfw.gstate.keycodes[AKEYCODE_2]          = GLFW_KEY_2;
    _glfw.gstate.keycodes[AKEYCODE_3]          = GLFW_KEY_3;
    _glfw.gstate.keycodes[AKEYCODE_4]          = GLFW_KEY_4;
    _glfw.gstate.keycodes[AKEYCODE_5]          = GLFW_KEY_5;
    _glfw.gstate.keycodes[AKEYCODE_6]          = GLFW_KEY_6;
    _glfw.gstate.keycodes[AKEYCODE_7]          = GLFW_KEY_7;
    _glfw.gstate.keycodes[AKEYCODE_8]          = GLFW_KEY_8;
    _glfw.gstate.keycodes[AKEYCODE_9]          = GLFW_KEY_9;
    _glfw.gstate.keycodes[AKEYCODE_0]          = GLFW_KEY_0;
    _glfw.gstate.keycodes[AKEYCODE_SPACE]      = GLFW_KEY_SPACE;
    _glfw.gstate.keycodes[AKEYCODE_MINUS]      = GLFW_KEY_MINUS;
    _glfw.gstate.keycodes[AKEYCODE_EQUALS]      = GLFW_KEY_EQUAL;
    _glfw.gstate.keycodes[AKEYCODE_Q]          = GLFW_KEY_Q;
    _glfw.gstate.keycodes[AKEYCODE_W]          = GLFW_KEY_W;
    _glfw.gstate.keycodes[AKEYCODE_E]          = GLFW_KEY_E;
    _glfw.gstate.keycodes[AKEYCODE_R]          = GLFW_KEY_R;
    _glfw.gstate.keycodes[AKEYCODE_T]          = GLFW_KEY_T;
    _glfw.gstate.keycodes[AKEYCODE_Y]          = GLFW_KEY_Y;
    _glfw.gstate.keycodes[AKEYCODE_U]          = GLFW_KEY_U;
    _glfw.gstate.keycodes[AKEYCODE_I]          = GLFW_KEY_I;
    _glfw.gstate.keycodes[AKEYCODE_O]          = GLFW_KEY_O;
    _glfw.gstate.keycodes[AKEYCODE_P]          = GLFW_KEY_P;
    _glfw.gstate.keycodes[AKEYCODE_LEFT_BRACKET]  = GLFW_KEY_LEFT_BRACKET;
    _glfw.gstate.keycodes[AKEYCODE_RIGHT_BRACKET] = GLFW_KEY_RIGHT_BRACKET;
    _glfw.gstate.keycodes[AKEYCODE_A]          = GLFW_KEY_A;
    _glfw.gstate.keycodes[AKEYCODE_S]          = GLFW_KEY_S;
    _glfw.gstate.keycodes[AKEYCODE_D]          = GLFW_KEY_D;
    _glfw.gstate.keycodes[AKEYCODE_F]          = GLFW_KEY_F;
    _glfw.gstate.keycodes[AKEYCODE_G]          = GLFW_KEY_G;
    _glfw.gstate.keycodes[AKEYCODE_H]          = GLFW_KEY_H;
    _glfw.gstate.keycodes[AKEYCODE_J]          = GLFW_KEY_J;
    _glfw.gstate.keycodes[AKEYCODE_K]          = GLFW_KEY_K;
    _glfw.gstate.keycodes[AKEYCODE_L]          = GLFW_KEY_L;
    _glfw.gstate.keycodes[AKEYCODE_SEMICOLON]  = GLFW_KEY_SEMICOLON;
    _glfw.gstate.keycodes[AKEYCODE_APOSTROPHE] = GLFW_KEY_APOSTROPHE;
    _glfw.gstate.keycodes[AKEYCODE_Z]          = GLFW_KEY_Z;
    _glfw.gstate.keycodes[AKEYCODE_X]          = GLFW_KEY_X;
    _glfw.gstate.keycodes[AKEYCODE_C]          = GLFW_KEY_C;
    _glfw.gstate.keycodes[AKEYCODE_V]          = GLFW_KEY_V;
    _glfw.gstate.keycodes[AKEYCODE_B]          = GLFW_KEY_B;
    _glfw.gstate.keycodes[AKEYCODE_N]          = GLFW_KEY_N;
    _glfw.gstate.keycodes[AKEYCODE_M]          = GLFW_KEY_M;
    _glfw.gstate.keycodes[AKEYCODE_COMMA]      = GLFW_KEY_COMMA;
    _glfw.gstate.keycodes[AKEYCODE_PERIOD]        = GLFW_KEY_PERIOD;
    _glfw.gstate.keycodes[AKEYCODE_SLASH]      = GLFW_KEY_SLASH;
    _glfw.gstate.keycodes[AKEYCODE_BACKSLASH]  = GLFW_KEY_BACKSLASH;
    _glfw.gstate.keycodes[AKEYCODE_ESCAPE]        = GLFW_KEY_ESCAPE;
    _glfw.gstate.keycodes[AKEYCODE_TAB]        = GLFW_KEY_TAB;
    _glfw.gstate.keycodes[AKEYCODE_SHIFT_LEFT]  = GLFW_KEY_LEFT_SHIFT;
    _glfw.gstate.keycodes[AKEYCODE_SHIFT_RIGHT] = GLFW_KEY_RIGHT_SHIFT;
    _glfw.gstate.keycodes[AKEYCODE_CTRL_LEFT]   = GLFW_KEY_LEFT_CONTROL;
    _glfw.gstate.keycodes[AKEYCODE_CTRL_RIGHT]  = GLFW_KEY_RIGHT_CONTROL;
    _glfw.gstate.keycodes[AKEYCODE_ALT_LEFT]    = GLFW_KEY_LEFT_ALT;
    _glfw.gstate.keycodes[AKEYCODE_ALT_RIGHT]   = GLFW_KEY_RIGHT_ALT;
    _glfw.gstate.keycodes[AKEYCODE_META_LEFT]   = GLFW_KEY_LEFT_SUPER;
    _glfw.gstate.keycodes[AKEYCODE_META_RIGHT]  = GLFW_KEY_RIGHT_SUPER;
    _glfw.gstate.keycodes[AKEYCODE_MENU]    = GLFW_KEY_MENU;
    _glfw.gstate.keycodes[AKEYCODE_NUM_LOCK]    = GLFW_KEY_NUM_LOCK;
    _glfw.gstate.keycodes[AKEYCODE_CAPS_LOCK]   = GLFW_KEY_CAPS_LOCK;
    _glfw.gstate.keycodes[AKEYCODE_SCROLL_LOCK] = GLFW_KEY_SCROLL_LOCK;
    _glfw.gstate.keycodes[AKEYCODE_FORWARD_DEL]     = GLFW_KEY_DELETE;
    _glfw.gstate.keycodes[AKEYCODE_DEL]  = GLFW_KEY_BACKSPACE;
    _glfw.gstate.keycodes[AKEYCODE_ENTER]      = GLFW_KEY_ENTER;
    _glfw.gstate.keycodes[AKEYCODE_MOVE_HOME]       = GLFW_KEY_HOME;
    _glfw.gstate.keycodes[AKEYCODE_MOVE_END]        = GLFW_KEY_END;
    _glfw.gstate.keycodes[AKEYCODE_PAGE_UP]     = GLFW_KEY_PAGE_UP;
    _glfw.gstate.keycodes[AKEYCODE_PAGE_DOWN]   = GLFW_KEY_PAGE_DOWN;
    _glfw.gstate.keycodes[AKEYCODE_INSERT]     = GLFW_KEY_INSERT;
    _glfw.gstate.keycodes[AKEYCODE_DPAD_LEFT]  = GLFW_KEY_LEFT;
    _glfw.gstate.keycodes[AKEYCODE_DPAD_RIGHT] = GLFW_KEY_RIGHT;
    _glfw.gstate.keycodes[AKEYCODE_DPAD_DOWN]  = GLFW_KEY_DOWN;
    _glfw.gstate.keycodes[AKEYCODE_DPAD_UP]    = GLFW_KEY_UP;
    _glfw.gstate.keycodes[AKEYCODE_F1]         = GLFW_KEY_F1;
    _glfw.gstate.keycodes[AKEYCODE_F2]         = GLFW_KEY_F2;
    _glfw.gstate.keycodes[AKEYCODE_F3]         = GLFW_KEY_F3;
    _glfw.gstate.keycodes[AKEYCODE_F4]         = GLFW_KEY_F4;
    _glfw.gstate.keycodes[AKEYCODE_F5]         = GLFW_KEY_F5;
    _glfw.gstate.keycodes[AKEYCODE_F6]         = GLFW_KEY_F6;
    _glfw.gstate.keycodes[AKEYCODE_F7]         = GLFW_KEY_F7;
    _glfw.gstate.keycodes[AKEYCODE_F8]         = GLFW_KEY_F8;
    _glfw.gstate.keycodes[AKEYCODE_F9]         = GLFW_KEY_F9;
    _glfw.gstate.keycodes[AKEYCODE_F10]        = GLFW_KEY_F10;
    _glfw.gstate.keycodes[AKEYCODE_F11]        = GLFW_KEY_F11;
    _glfw.gstate.keycodes[AKEYCODE_F12]        = GLFW_KEY_F12;
    _glfw.gstate.keycodes[AKEYCODE_NUMPAD_DIVIDE]    = GLFW_KEY_KP_DIVIDE;
    _glfw.gstate.keycodes[AKEYCODE_NUMPAD_MULTIPLY] = GLFW_KEY_KP_MULTIPLY;
    _glfw.gstate.keycodes[AKEYCODE_NUMPAD_SUBTRACT]    = GLFW_KEY_KP_SUBTRACT;
    _glfw.gstate.keycodes[AKEYCODE_NUMPAD_ADD]     = GLFW_KEY_KP_ADD;
    _glfw.gstate.keycodes[AKEYCODE_NUMPAD_0]        = GLFW_KEY_KP_0;
    _glfw.gstate.keycodes[AKEYCODE_NUMPAD_1]        = GLFW_KEY_KP_1;
    _glfw.gstate.keycodes[AKEYCODE_NUMPAD_2]        = GLFW_KEY_KP_2;
    _glfw.gstate.keycodes[AKEYCODE_NUMPAD_3]        = GLFW_KEY_KP_3;
    _glfw.gstate.keycodes[AKEYCODE_NUMPAD_4]        = GLFW_KEY_KP_4;
    _glfw.gstate.keycodes[AKEYCODE_NUMPAD_5]        = GLFW_KEY_KP_5;
    _glfw.gstate.keycodes[AKEYCODE_NUMPAD_6]        = GLFW_KEY_KP_6;
    _glfw.gstate.keycodes[AKEYCODE_NUMPAD_7]        = GLFW_KEY_KP_7;
    _glfw.gstate.keycodes[AKEYCODE_NUMPAD_8]        = GLFW_KEY_KP_8;
    _glfw.gstate.keycodes[AKEYCODE_NUMPAD_9]        = GLFW_KEY_KP_9;
    _glfw.gstate.keycodes[AKEYCODE_NUMPAD_DOT]      = GLFW_KEY_KP_DECIMAL;
    _glfw.gstate.keycodes[AKEYCODE_NUMPAD_EQUALS]    = GLFW_KEY_KP_EQUAL;
    _glfw.gstate.keycodes[AKEYCODE_NUMPAD_ENTER]    = GLFW_KEY_KP_ENTER;

    for (int scancode = 0;  scancode < 256;  scancode++)
    {
        if (_glfw.gstate.keycodes[scancode] > 0)
            _glfw.gstate.scancodes[_glfw.gstate.keycodes[scancode]] = scancode;
    }
}

int _glfwInitAndroid(void) {
    _glfw.gstate.app = _globalAndroidApp;

    createAndroidKeyTables();

    return GLFW_TRUE;
}

void _glfwTerminateAndroid(void) {
    ANativeActivity_finish(_glfw.gstate.app->activity);
    _glfw.gstate.app = NULL;
    _glfw.gstate.suspended = false;
}
