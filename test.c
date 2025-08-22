#define _POSIX_C_SOURCE 200112L
#define AVEN_IMPLEMENTATION

#include <aven.h>
#include <aven/arena.h>
#include <aven/fs.h>
#include <aven/gl.h>
#include <aven/gl/shape.h>
#include <aven/gl/texture.h>
#include <aven/gl/ui.h>
#include <aven/gl/window.h>
#include <aven/io.h>
#include <aven/time.h>

#include <stdlib.h>

#include <GLFW/glfw3.h>

#if defined(__EMSCRIPTEN__)
    #include <emscripten.h>

    #ifdef HOT_RELOAD
        #error "hot reloading dll incompatible with emcc"
    #endif

    void on_resize(int width, int height) {
        glfwSetWindowSize(win.window, width, height);
        on_damage(win.window);
    }
#endif

#define INIT_WIDTH 480
#define INIT_HEIGHT 480
#define ARENA_SIZE (4096 * 16)

static AvenArena test_arena;

typedef enum {
    TEST_AVEN_GL_SHAPE = 0,
    TEST_AVEN_GL_ROUNDED,
    TEST_AVEN_GL_TEXTURE,
} TestAvenGlState;

typedef union {
    struct {
        AvenGlShapeCtx ctx;
        AvenGlShapeGeometry geometry;
        AvenGlShapeBuffer buffer;
    } shape;
    struct {
        AvenGlShapeRoundedCtx ctx;
        AvenGlShapeRoundedGeometry geometry;
        AvenGlShapeRoundedBuffer buffer;
    } rounded;
    struct {
        AvenGlTextureCtx ctx;
        AvenGlTextureGeometry geometry;
        AvenGlTextureBuffer buffer;
    } texture;
} TestAvenGlData;

typedef struct {
    TestAvenGlState state;
    TestAvenGlData data;
} TestAvenGl;

static AvenGlWindow win;
static TestAvenGl app;

#define NVERTICES 16
#define NINDICES 64
#define TSIZE 2

static uint32_t texture_data[TSIZE * TSIZE] = {
    0xffffffff,
    0xff0000ff,
    0xff0000ff,
    0xffffffff,
};
static Slice(uint32_t) texture = slice_array(texture_data);

#ifdef __ANDROID__
    #include <android/log.h>
    static bool minimized;

    void on_android_pause_resume(GLFWwindow *window, int iconified) {
        if (iconified) {
            switch (app.state) {
                case TEST_AVEN_GL_SHAPE:
                    aven_gl_shape_buffer_deinit(&win.gl, &app.data.shape.buffer);
                    aven_gl_shape_ctx_deinit(&win.gl, &app.data.shape.ctx);
                    break;
                case TEST_AVEN_GL_ROUNDED:
                    aven_gl_shape_rounded_buffer_deinit(
                        &win.gl,
                        &app.data.rounded.buffer
                    );
                    aven_gl_shape_rounded_ctx_deinit(
                        &win.gl,
                        &app.data.rounded.ctx
                    );
                    break;
                case TEST_AVEN_GL_TEXTURE:
                    aven_gl_texture_buffer_deinit(
                        &win.gl,
                        &app.data.texture.buffer
                    );
                    aven_gl_texture_ctx_deinit(&win.gl, &app.data.texture.ctx);
                    break;
            }
            // win = (AvenGlWindow){ 0 };
            minimized = true;
        } else {
            win.window = window;
            win.gl = aven_gl_load(glfwGetProcAddress, win.gl.es);
            switch (app.state) {
                case TEST_AVEN_GL_SHAPE:
                    app.data.shape.ctx = aven_gl_shape_ctx_init(&win.gl);
                    app.data.shape.buffer = aven_gl_shape_buffer_init(
                        &win.gl,
                        &app.data.shape.geometry,
                        AVEN_GL_BUFFER_USAGE_DYNAMIC
                    );
                    break;
                case TEST_AVEN_GL_ROUNDED:
                    app.data.rounded.ctx = aven_gl_shape_rounded_ctx_init(
                        &win.gl
                    );
                    app.data.rounded.buffer = aven_gl_shape_rounded_buffer_init(
                        &win.gl,
                        &app.data.rounded.geometry,
                        AVEN_GL_BUFFER_USAGE_DYNAMIC
                    );
                    break;
                case TEST_AVEN_GL_TEXTURE:
                    app.data.texture.ctx = aven_gl_texture_ctx_init(
                        &win.gl,
                        TSIZE,
                        TSIZE,
                        (AvenGlTextureBytesOptional){
                            .valid = true,
                            .value = slice_as_bytes(texture),
                        }
                    );
                    app.data.texture.buffer = aven_gl_texture_buffer_init(
                        &win.gl,
                        &app.data.texture.geometry,
                        AVEN_GL_BUFFER_USAGE_DYNAMIC
                    );
                    break;
            }
            minimized = false;
        }
    }
#endif

void test_aven_gl_shape(void) {
    AvenArena temp_arena = test_arena;
    app.state = TEST_AVEN_GL_SHAPE;
    app.data.shape.ctx = aven_gl_shape_ctx_init(&win.gl);
    app.data.shape.geometry = aven_gl_shape_geometry_init(
        NVERTICES,
        NINDICES,
        &temp_arena
    );
    app.data.shape.buffer = aven_gl_shape_buffer_init(
        &win.gl,
        &app.data.shape.geometry,
        AVEN_GL_BUFFER_USAGE_DYNAMIC
    );
    Aff2 trans;
    aff2_identity(trans);
    Aff2 camera;
    aff2_camera_position(camera, (Vec2){ 0.0f, 0.0f }, (Vec2){ 1.25f, 1.25f });
    AvenTimeInst start = aven_time_now();
    while (aven_time_since(aven_time_now(), start) / 2 < AVEN_TIME_NSEC_PER_SEC) {
        if (glfwWindowShouldClose(win.window)) {
            break;
        }
        aven_gl_shape_geometry_clear(&app.data.shape.geometry);
        aven_gl_shape_geometry_push_square(
            &app.data.shape.geometry,
            trans,
            (Vec4){ 1.0f, 0.0f, 0.0f, 1.0f }
        );
        aven_gl_shape_geometry_push_triangle_isoceles(
            &app.data.shape.geometry,
            trans,
            (Vec4){ 0.0f, 1.0f, 0.0f, 1.0f }
        );
        aven_gl_shape_geometry_push_triangle_right(
            &app.data.shape.geometry,
            trans,
            (Vec4){ 0.0f, 0.0f, 1.0f, 1.0f }
        );
        aven_gl_shape_buffer_update(
            &win.gl,
            &app.data.shape.buffer,
            &app.data.shape.geometry
        );

        int width;
        int height;
        glfwGetFramebufferSize(win.window, &width, &height);
        win.gl.Viewport(0, 0, width, height);
        assert(win.gl.GetError() == 0);

        win.gl.ClearColor(0.75f, 0.75f, 0.75f, 1.0f);
        assert(win.gl.GetError() == 0);
        win.gl.Clear(GL_COLOR_BUFFER_BIT);
        assert(win.gl.GetError() == 0);
        aven_gl_shape_draw(
            &win.gl,
            &app.data.shape.ctx,
            &app.data.shape.buffer,
            camera
        );

        glfwSwapBuffers(win.window);
        glfwPollEvents();
#ifdef __ANDROID__
        while (minimized) {
            glfwWaitEvents();
        }
#endif
    }
    aven_gl_shape_buffer_deinit(&win.gl, &app.data.shape.buffer);
    aven_gl_shape_geometry_deinit(&app.data.shape.geometry);
    aven_gl_shape_ctx_deinit(&win.gl, &app.data.shape.ctx);
}

void test_aven_gl_shape_rounded(void) {
    AvenArena temp_arena = test_arena;
    app.state = TEST_AVEN_GL_ROUNDED;
    app.data.rounded.ctx = aven_gl_shape_rounded_ctx_init(&win.gl);
    app.data.rounded.geometry = aven_gl_shape_rounded_geometry_init(
        NVERTICES,
        NINDICES,
        &temp_arena
    );
    app.data.rounded.buffer = aven_gl_shape_rounded_buffer_init(
        &win.gl,
        &app.data.rounded.geometry,
        AVEN_GL_BUFFER_USAGE_DYNAMIC
    );
    Aff2 trans;
    aff2_identity(trans);
    Aff2 camera;
    aff2_camera_position(camera, (Vec2){ 0.0f, 0.0f }, (Vec2){ 1.25f, 1.25f });
    AvenTimeInst start = aven_time_now();
    while (aven_time_since(aven_time_now(), start) / 2 < AVEN_TIME_NSEC_PER_SEC) {
        if (glfwWindowShouldClose(win.window)) {
            break;
        }
        aven_gl_shape_rounded_geometry_clear(&app.data.rounded.geometry);
        aven_gl_shape_rounded_geometry_push_square(
            &app.data.rounded.geometry,
            trans,
            0.25f,
            (Vec4){ 1.0f, 0.0f, 0.0f, 1.0f }
        );
        aven_gl_shape_rounded_geometry_push_triangle_isoceles(
            &app.data.rounded.geometry,
            trans,
            0.25f,
            (Vec4){ 0.0f, 1.0f, 0.0f, 1.0f }
        );
        aven_gl_shape_rounded_geometry_push_triangle_right(
            &app.data.rounded.geometry,
            trans,
            0.25f,
            (Vec4){ 0.0f, 0.0f, 1.0f, 1.0f }
        );
        aven_gl_shape_rounded_buffer_update(
            &win.gl,
            &app.data.rounded.buffer,
            &app.data.rounded.geometry
        );

        int width;
        int height;
        glfwGetFramebufferSize(win.window, &width, &height);
        win.gl.Viewport(0, 0, width, height);
        assert(win.gl.GetError() == 0);

        win.gl.ClearColor(0.75f, 0.75f, 0.75f, 1.0f);
        assert(win.gl.GetError() == 0);
        win.gl.Clear(GL_COLOR_BUFFER_BIT);
        assert(win.gl.GetError() == 0);
        aven_gl_shape_rounded_draw(
            &win.gl,
            &app.data.rounded.ctx,
            &app.data.rounded.buffer,
            2.0f / (float)height,
            camera
        );

        glfwSwapBuffers(win.window);
        glfwPollEvents();
#ifdef __ANDROID__
        while (minimized) {
            glfwWaitEvents();
        }
#endif
    }
    aven_gl_shape_rounded_buffer_deinit(&win.gl, &app.data.rounded.buffer);
    aven_gl_shape_rounded_geometry_deinit(&app.data.rounded.geometry);
    aven_gl_shape_rounded_ctx_deinit(&win.gl, &app.data.rounded.ctx);
}

void test_aven_gl_texture(void) {
    AvenArena temp_arena = test_arena;
    app.state = TEST_AVEN_GL_TEXTURE;
    app.data.texture.ctx = aven_gl_texture_ctx_init(
        &win.gl,
        TSIZE,
        TSIZE,
        (AvenGlTextureBytesOptional){
            .valid = true,
            .value = slice_as_bytes(texture),
        }
    );
    app.data.texture.geometry = aven_gl_texture_geometry_init(1, &temp_arena);
    app.data.texture.buffer = aven_gl_texture_buffer_init(
        &win.gl,
        &app.data.texture.geometry,
        AVEN_GL_BUFFER_USAGE_DYNAMIC
    );
    Aff2 trans;
    aff2_identity(trans);
    Aff2 camera;
    aff2_camera_position(camera, (Vec2){ 0.0f, 0.0f }, (Vec2){ 1.25f, 1.25f });
    AvenTimeInst start = aven_time_now();
    while (aven_time_since(aven_time_now(), start) / 2 < AVEN_TIME_NSEC_PER_SEC) {
        if (glfwWindowShouldClose(win.window)) {
            break;
        }
        aven_gl_texture_geometry_clear(&app.data.texture.geometry);
        aven_gl_texture_geometry_push_square(
            &app.data.texture.geometry,
            trans,
            trans
        );
        aven_gl_texture_buffer_update(
            &win.gl,
            &app.data.texture.buffer,
            &app.data.texture.geometry
        );

        int width;
        int height;
        glfwGetFramebufferSize(win.window, &width, &height);
        win.gl.Viewport(0, 0, width, height);
        assert(win.gl.GetError() == 0);

        win.gl.ClearColor(0.75f, 0.75f, 0.75f, 1.0f);
        assert(win.gl.GetError() == 0);
        win.gl.Clear(GL_COLOR_BUFFER_BIT);
        assert(win.gl.GetError() == 0);
        aven_gl_texture_draw(
            &win.gl,
            &app.data.texture.ctx,
            &app.data.texture.buffer,
            camera
        );

        glfwSwapBuffers(win.window);
        glfwPollEvents();
#ifdef __ANDROID__
        while (minimized) {
            glfwWaitEvents();
        }
#endif
    }
    aven_gl_texture_buffer_deinit(&win.gl, &app.data.texture.buffer);
    aven_gl_texture_geometry_deinit(&app.data.texture.geometry);
    aven_gl_texture_ctx_deinit(&win.gl, &app.data.texture.ctx);
}

int main(void) {
    aven_fs_utf8_mode();
    void *mem = malloc(ARENA_SIZE);
    test_arena = aven_arena_init(mem, ARENA_SIZE);

    win = aven_gl_window(INIT_WIDTH, INIT_HEIGHT, "AvenGL Test");

#ifdef __ANDROID__
    glfwSetWindowIconifyCallback(win.window, on_android_pause_resume);
#endif

    bool fail = false;

    if (win.gl.ActiveTexture == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glActiveTexture\n");
    }
    if (win.gl.AttachShader == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glAttachShader\n");
    }
    if (win.gl.BindAttribLocation == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glBindAttribLocation\n");
    }
    if (win.gl.BindBuffer == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glBindBuffer\n");
    }
    if (win.gl.BindFramebuffer == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glBindFramebuffer\n");
    }
    if (win.gl.BindRenderbuffer == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glBindRenderbuffer\n");
    }
    if (win.gl.BindTexture == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glBindTexture\n");
    }
    if (win.gl.BlendColor == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glBlendColor\n");
    }
    if (win.gl.BlendEquation == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glBlendEquation\n");
    }
    if (win.gl.BlendEquationSeparate == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glBlendEquationSeparate\n");
    }
    if (win.gl.BlendFunc == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glBlendFunc\n");
    }
    if (win.gl.BlendFuncSeparate == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glBlendFuncSeparate\n");
    }
    if (win.gl.BufferData == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glBufferData\n");
    }
    if (win.gl.BufferSubData == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glBufferSubData\n");
    }
    if (win.gl.CheckFramebufferStatus == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glCheckFramebufferStatus\n");
    }
    if (win.gl.Clear == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glClear\n");
    }
    if (win.gl.ClearColor == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glClearColor\n");
    }
    if (win.gl.ClearDepth == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glClearDepth\n");
    }
    if (win.gl.ClearStencil == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glClearStencil\n");
    }
    if (win.gl.ColorMask == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glColorMask\n");
    }
    if (win.gl.CompileShader == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glCompileShader\n");
    }
    if (win.gl.CompressedTexImage2D == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glCompressedTexImage2D\n");
    }
    if (win.gl.CompressedTexSubImage2D == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glCompressedTexSubImage2D\n");
    }
    if (win.gl.CopyTexImage2D == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glCopyTexImage2D\n");
    }
    if (win.gl.CopyTexSubImage2D == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glCopyTexSubImage2D\n");
    }
    if (win.gl.CreateProgram == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glCreateProgram\n");
    }
    if (win.gl.CreateShader == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glCreateShader\n");
    }
    if (win.gl.CullFace == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glCullFace\n");
    }
    if (win.gl.DeleteBuffers == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glDeleteBuffers\n");
    }
    if (win.gl.DeleteFramebuffers == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glDeleteFramebuffers\n");
    }
    if (win.gl.DeleteProgram == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glDeleteProgram\n");
    }
    if (win.gl.DeleteRenderbuffers == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glDeleteRenderbuffers\n");
    }
    if (win.gl.DeleteShader == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glDeleteShader\n");
    }
    if (win.gl.DeleteTextures == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glDeleteTextures\n");
    }
    if (win.gl.DepthFunc == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glDepthFunc\n");
    }
    if (win.gl.DepthMask == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glDepthMask\n");
    }
    if (win.gl.DepthRangef == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glDepthRangef\n");
    }
    if (win.gl.DetachShader == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glDetachShader\n");
    }
    if (win.gl.Disable == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glDisable\n");
    }
    if (win.gl.DisableVertexAttribArray == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glDisableVertexAttribArray\n");
    }
    if (win.gl.DrawArrays == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glDrawArrays\n");
    }
    if (win.gl.DrawElements == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glDrawElements\n");
    }
    if (win.gl.Enable == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glEnable\n");
    }
    if (win.gl.EnableVertexAttribArray == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glEnableVertexAttribArray\n");
    }
    if (win.gl.Finish == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glFinish\n");
    }
    if (win.gl.Flush == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glFlush\n");
    }
    if (win.gl.FramebufferRenderbuffer == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glFramebufferRenderbuffer\n");
    }
    if (win.gl.FramebufferTexture2D == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glFramebufferTexture2D\n");
    }
    if (win.gl.FrontFace == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glFrontFace\n");
    }
    if (win.gl.GenBuffers == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glGenBuffers\n");
    }
    if (win.gl.GenerateMipmap == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glGenerateMipmap\n");
    }
    if (win.gl.GenFramebuffers == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glGenFramebuffers\n");
    }
    if (win.gl.GenRenderbuffers == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glGenRenderbuffers\n");
    }
    if (win.gl.GenTextures == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glGenTextures\n");
    }
    if (win.gl.GetActiveAttrib == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glGetActiveAttrib\n");
    }
    if (win.gl.GetActiveUniform == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glGetActiveUniform\n");
    }
    if (win.gl.GetAttachedShaders == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glGetAttachedShaders\n");
    }
    if (win.gl.GetAttribLocation == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glGetAttribLocation\n");
    }
    if (win.gl.GetBooleanv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glGetBooleanv\n");
    }
    if (win.gl.GetBufferParameteriv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glGetBufferParameteriv\n");
    }
    if (win.gl.GetError == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glGetError\n");
    }
    if (win.gl.GetFloatv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glGetFloatv\n");
    }
    if (win.gl.GetFramebufferAttachmentParameteriv == NULL) {
        fail = true;
        aven_io_print(
            "test failed: aven_gl_load glGetFramebufferAttachmentParameteriv\n"
        );
    }
    if (win.gl.GetIntegerv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glGetIntegerv\n");
    }
    if (win.gl.GetProgramiv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glGetProgramiv\n");
    }
    if (win.gl.GetProgramInfoLog == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glGetProgramInfoLog\n");
    }
    if (win.gl.GetRenderbufferParameteriv == NULL) {
        fail = true;
        aven_io_print(
            "test failed: aven_gl_load glGetRenderbufferParameteriv\n"
        );
    }
    if (win.gl.GetShaderiv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glGetShaderiv\n");
    }
    if (win.gl.GetShaderInfoLog == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glGetShaderInfoLog\n");
    }
    if (win.gl.GetShaderPrecisionFormat == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glGetShaderPrecisionFormat\n");
    }
    if (win.gl.GetShaderSource == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glGetShaderSource\n");
    }
    if (win.gl.GetString == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glGetString\n");
    }
    if (win.gl.GetTexParameterfv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glGetTexParameterfv\n");
    }
    if (win.gl.GetTexParameteriv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glGetTexParameteriv\n");
    }
    if (win.gl.GetUniformfv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glGetUniformfv\n");
    }
    if (win.gl.GetUniformiv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glGetUniformiv\n");
    }
    if (win.gl.GetUniformLocation == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glGetUniformLocation\n");
    }
    if (win.gl.GetVertexAttribfv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glGetVertexAttribfv\n");
    }
    if (win.gl.GetVertexAttribiv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glGetVertexAttribiv\n");
    }
    if (win.gl.GetVertexAttribPointerv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glGetVertexAttribPointerv\n");
    }
    if (win.gl.Hint == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glHint\n");
    }
    if (win.gl.IsBuffer == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glIsBuffer\n");
    }
    if (win.gl.IsEnabled == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glIsEnabled\n");
    }
    if (win.gl.IsFramebuffer == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glIsFramebuffer\n");
    }
    if (win.gl.IsProgram == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glIsProgram\n");
    }
    if (win.gl.IsRenderbuffer == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glIsRenderbuffer\n");
    }
    if (win.gl.IsShader == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glIsShader\n");
    }
    if (win.gl.IsTexture == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glIsTexture\n");
    }
    if (win.gl.LineWidth == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glLineWidth\n");
    }
    if (win.gl.LinkProgram == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glLinkProgram\n");
    }
    if (win.gl.PixelStorei == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glPixelStorei\n");
    }
    if (win.gl.PolygonOffset == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glPolygonOffset\n");
    }
    if (win.gl.ReadPixels == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glReadPixels\n");
    }
    if (win.gl.ReleaseShaderCompiler == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glReleaseShaderCompiler\n");
    }
    if (win.gl.RenderbufferStorage == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glRenderbufferStorage\n");
    }
    if (win.gl.SampleCoverage == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glSampleCoverage\n");
    }
    if (win.gl.Scissor == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glScissor\n");
    }
    if (win.gl.ShaderBinary == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glShaderBinary\n");
    }
    if (win.gl.ShaderSource == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glShaderSource\n");
    }
    if (win.gl.StencilFunc == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glStencilFunc\n");
    }
    if (win.gl.StencilFuncSeparate == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glStencilFuncSeparate\n");
    }
    if (win.gl.StencilMask == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glStencilMask\n");
    }
    if (win.gl.StencilMaskSeparate == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glStencilMaskSeparate\n");
    }
    if (win.gl.StencilOp == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glStencilOp\n");
    }
    if (win.gl.StencilOpSeparate == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glStencilOpSeparate\n");
    }
    if (win.gl.TexImage2D == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glTexImage2D\n");
    }
    if (win.gl.TexParameterf == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glTexParameterf\n");
    }
    if (win.gl.TexParameterfv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glTexParameterfv\n");
    }
    if (win.gl.TexParameteri == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glTexParameteri\n");
    }
    if (win.gl.TexParameteriv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glTexParameteriv\n");
    }
    if (win.gl.TexSubImage2D == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glTexSubImage2D\n");
    }
    if (win.gl.Uniform1f == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glUniform1f\n");
    }
    if (win.gl.Uniform1fv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glUniform1fv\n");
    }
    if (win.gl.Uniform1i == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glUniform1i\n");
    }
    if (win.gl.Uniform1iv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glUniform1iv\n");
    }
    if (win.gl.Uniform2f == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glUniform2f\n");
    }
    if (win.gl.Uniform2fv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glUniform2fv\n");
    }
    if (win.gl.Uniform2i == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glUniform2i\n");
    }
    if (win.gl.Uniform2iv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glUniform2iv\n");
    }
    if (win.gl.Uniform3f == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glUniform3f\n");
    }
    if (win.gl.Uniform3fv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glUniform3fv\n");
    }
    if (win.gl.Uniform3i == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glUniform3i\n");
    }
    if (win.gl.Uniform3iv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glUniform3iv\n");
    }
    if (win.gl.Uniform4f == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glUniform4f\n");
    }
    if (win.gl.Uniform4fv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glUniform4fv\n");
    }
    if (win.gl.Uniform4i == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glUniform4i\n");
    }
    if (win.gl.Uniform4iv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glUniform4iv\n");
    }
    if (win.gl.UniformMatrix2fv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glUniformMatrix2fv\n");
    }
    if (win.gl.UniformMatrix3fv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glUniformMatrix3fv\n");
    }
    if (win.gl.UniformMatrix4fv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glUniformMatrix4fv\n");
    }
    if (win.gl.UseProgram == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glUseProgram\n");
    }
    if (win.gl.ValidateProgram == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glValidateProgram\n");
    }
    if (win.gl.VertexAttrib1f == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glVertexAttrib1f\n");
    }
    if (win.gl.VertexAttrib1fv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glVertexAttrib1fv\n");
    }
    if (win.gl.VertexAttrib2f == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glVertexAttrib2f\n");
    }
    if (win.gl.VertexAttrib2fv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glVertexAttrib2fv\n");
    }
    if (win.gl.VertexAttrib3f == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glVertexAttrib3f\n");
    }
    if (win.gl.VertexAttrib3fv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glVertexAttrib3fv\n");
    }
    if (win.gl.VertexAttrib4f == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glVertexAttrib4f\n");
    }
    if (win.gl.VertexAttrib4fv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glVertexAttrib4fv\n");
    }
    if (win.gl.VertexAttribPointer == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glVertexAttribPointer\n");
    }
    if (win.gl.Viewport == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glViewport\n");
    }

    if (!fail) {
        aven_io_print("all gl functions loaded\n");
    }

#if defined(__ANDROID__) or defined(__EMSCRIPTEN__)
    while (1) {
        test_aven_gl_shape();
        test_aven_gl_shape_rounded();
        test_aven_gl_texture();
    }
#else
    test_aven_gl_shape();
    test_aven_gl_shape_rounded();
    test_aven_gl_texture();
#endif

    glfwDestroyWindow(win.window);
    glfwTerminate();

    return (int)fail;
}
