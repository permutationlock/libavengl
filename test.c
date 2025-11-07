#if !defined(_WIN32) && !defined(_POSIX_C_SOURCE)
    #define _POSIX_C_SOURCE 200112L
#endif

#define AVEN_IMPLEMENTATION
#include <aven.h>
#include <aven/arena.h>
#include <aven/fs.h>
#include <aven/gl.h>
#include <aven/gl/shape.h>
#include <aven/gl/text.h>
#include <aven/gl/texture.h>
#include <aven/gl/ui.h>
#include <aven/gl/window.h>
#include <aven/gl/window/impl.h>
#include <aven/io.h>
#include <aven/math.h>
#include <aven/time.h>

#include <stdlib.h>

#include "font.h"

#define INIT_WIDTH 480
#define INIT_HEIGHT 480
#define ARENA_SIZE (4096 * 2048)
#define INTERVAL_NS (2 * AVEN_TIME_NSEC_PER_SEC)

typedef enum {
    TEST_AVEN_GL_NONE = 0,
    TEST_AVEN_GL_SHAPE,
    TEST_AVEN_GL_ROUNDED,
    TEST_AVEN_GL_TEXTURE,
    TEST_AVEN_GL_TEXT,
    TEST_AVEN_GL_JOIN,
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
        AvenGlShapeJoinCtx ctx;
        AvenGlShapeJoinGeometry geometry;
        AvenGlShapeJoinBuffer buffer;
    } join;
    struct {
        AvenGlTextureCtx ctx;
        AvenGlTextureGeometry geometry;
        AvenGlTextureBuffer buffer;
    } texture;
    struct {
        AvenGlTextFont font;
        AvenGlTextCtx ctx;
        AvenGlTextGeometry geometry;
        AvenGlTextBuffer buffer;
        AvenGlTextLine line;
    } text;
} TestAvenGlData;

typedef struct {
    AvenTimeInst start;
    TestAvenGlState state;
    TestAvenGlData data;
} TestAvenGl;

#define NVERTICES 10
#define NINDICES 12
#define TSIZE 2

typedef struct {
    Slice(uint32_t) texture;
    AvenArena arena;
    TestAvenGl app;
} TestCtx;

static void test_aven_gl_shape_init(AvenGlWindow *win) {
    TestCtx *ctx = win->ctx;
    AvenArena temp_arena = ctx->arena;
    ctx->app.state = TEST_AVEN_GL_SHAPE;
    ctx->app.data.shape.ctx = aven_gl_shape_ctx_init(&win->gl);
    ctx->app.data.shape.geometry = aven_gl_shape_geometry_init(
        NVERTICES,
        NINDICES,
        &temp_arena
    );
    ctx->app.data.shape.buffer = aven_gl_shape_buffer_init(
        &win->gl,
        &ctx->app.data.shape.ctx,
        &ctx->app.data.shape.geometry,
        AVEN_GL_BUFFER_USAGE_DYNAMIC
    );
}

static void test_aven_gl_shape_update(AvenGlWindow *win, float t) {
    (void)t;
    TestCtx *ctx = win->ctx;

    Aff2 trans;
    aff2_identity(trans);
    Aff2 camera;
    aff2_camera_position(camera, (Vec2){ 0.0f, 0.0f }, (Vec2){ 1.25f, 1.25f });

    aven_gl_shape_geometry_clear(&ctx->app.data.shape.geometry);
    aven_gl_shape_geometry_push_square(
        &ctx->app.data.shape.geometry,
        trans,
        (Vec4){ 1.0f, 0.0f, 0.0f, 1.0f }
    );
    aven_gl_shape_geometry_push_triangle_isoceles(
        &ctx->app.data.shape.geometry,
        trans,
        (Vec4){ 0.0f, 1.0f, 0.0f, 1.0f }
    );
    aven_gl_shape_geometry_push_triangle_right(
        &ctx->app.data.shape.geometry,
        trans,
        (Vec4){ 0.0f, 0.0f, 1.0f, 1.0f }
    );
    aven_gl_shape_buffer_update(
        &win->gl,
        &ctx->app.data.shape.buffer,
        &ctx->app.data.shape.geometry
    );

    int width;
    int height;
    glfwGetFramebufferSize(win->window, &width, &height);
    aven_gl_Viewport(&win->gl, 0, 0, width, height);
    aven_gl_ClearColor(&win->gl, 0.75f, 0.75f, 0.75f, 1.0f);
    aven_gl_Clear(&win->gl, GL_COLOR_BUFFER_BIT);
    aven_gl_shape_draw(
        &win->gl,
        &ctx->app.data.shape.ctx,
        &ctx->app.data.shape.buffer,
        camera
    );
}

static void test_aven_gl_shape_deinit(AvenGlWindow *win) {
    TestCtx *ctx = win->ctx;
    aven_gl_shape_buffer_deinit(&win->gl, &ctx->app.data.shape.buffer);
    aven_gl_shape_geometry_deinit(&ctx->app.data.shape.geometry);
    aven_gl_shape_ctx_deinit(&win->gl, &ctx->app.data.shape.ctx);
}

static void test_aven_gl_shape_rounded_init(AvenGlWindow *win) {
    TestCtx *ctx = win->ctx;
    AvenArena temp_arena = ctx->arena;
    ctx->app.state = TEST_AVEN_GL_ROUNDED;
    ctx->app.data.rounded.ctx = aven_gl_shape_rounded_ctx_init(&win->gl);
    ctx->app.data.rounded.geometry = aven_gl_shape_rounded_geometry_init(
        NVERTICES,
        NINDICES,
        &temp_arena
    );
    ctx->app.data.rounded.buffer = aven_gl_shape_rounded_buffer_init(
        &win->gl,
        &ctx->app.data.rounded.ctx,
        &ctx->app.data.rounded.geometry,
        AVEN_GL_BUFFER_USAGE_DYNAMIC
    );
}

static void test_aven_gl_shape_rounded_update(AvenGlWindow *win, float t) {
    TestCtx *ctx = win->ctx;

    Aff2 trans;
    aff2_identity(trans);
    Aff2 camera;
    aff2_camera_position(camera, (Vec2){ 0.0f, 0.0f }, (Vec2){ 1.25f, 1.25f });

    float rounded = min(1.0f, 1.33f * t);
    aven_gl_shape_rounded_geometry_clear(&ctx->app.data.rounded.geometry);
    aven_gl_shape_rounded_geometry_push_square(
        &ctx->app.data.rounded.geometry,
        trans,
        rounded,
        (Vec4){ 1.0f, 0.0f, 0.0f, 1.0f }
    );
    aven_gl_shape_rounded_geometry_push_triangle_isoceles(
        &ctx->app.data.rounded.geometry,
        trans,
        rounded,
        (Vec4){ 0.0f, 1.0f, 0.0f, 1.0f }
    );
    aven_gl_shape_rounded_geometry_push_triangle_right(
        &ctx->app.data.rounded.geometry,
        trans,
        rounded,
        (Vec4){ 0.0f, 0.0f, 1.0f, 1.0f }
    );
    aven_gl_shape_rounded_buffer_update(
        &win->gl,
        &ctx->app.data.rounded.buffer,
        &ctx->app.data.rounded.geometry
    );

    int width;
    int height;
    glfwGetFramebufferSize(win->window, &width, &height);
    aven_gl_Viewport(&win->gl, 0, 0, width, height);
    aven_gl_ClearColor(&win->gl, 0.75f, 0.75f, 0.75f, 1.0f);
    aven_gl_Clear(&win->gl, GL_COLOR_BUFFER_BIT);
    aven_gl_shape_rounded_draw(
        &win->gl,
        &ctx->app.data.rounded.ctx,
        &ctx->app.data.rounded.buffer,
        2.0f / (float)height,
        camera
    );
}

static void test_aven_gl_shape_rounded_deinit(AvenGlWindow *win) {
    TestCtx *ctx = win->ctx;
    aven_gl_shape_rounded_buffer_deinit(&win->gl, &ctx->app.data.rounded.buffer);
    aven_gl_shape_rounded_geometry_deinit(&ctx->app.data.rounded.geometry);
    aven_gl_shape_rounded_ctx_deinit(&win->gl, &ctx->app.data.rounded.ctx);
}

static void test_aven_gl_shape_join_init(AvenGlWindow *win) {
    TestCtx *ctx = win->ctx;
    AvenArena temp_arena = ctx->arena;
    ctx->app.state = TEST_AVEN_GL_JOIN;
    ctx->app.data.join.ctx = aven_gl_shape_join_ctx_init(&win->gl);
    ctx->app.data.join.geometry = aven_gl_shape_join_geometry_init(
        2,
        &temp_arena
    );
    ctx->app.data.join.buffer = aven_gl_shape_join_buffer_init(
        &win->gl,
        &ctx->app.data.join.ctx,
        &ctx->app.data.join.geometry,
        AVEN_GL_BUFFER_USAGE_DYNAMIC
    );
}

static void test_aven_gl_shape_join_update(AvenGlWindow *win, float t) {
    TestCtx *ctx = win->ctx;

    Aff2 trans;
    aff2_identity(trans);
    aff2_stretch(trans, (Vec2){ 0.3f + 0.5f * t, 0.5f }, trans);
    Aff2 camera;
    aff2_camera_position(camera, (Vec2){ 0.0f, 0.0f }, (Vec2){ 1.0f, 1.0f });

    aven_gl_shape_join_geometry_clear(&ctx->app.data.join.geometry);
    aven_gl_shape_join_geometry_push_square(
        &ctx->app.data.join.geometry,
        trans,
        (Vec2){ 1.0f, 0.5f },
        (Vec2){ 0.5f - 2.0f * t, 0.5f - 2.0f * t },
        (Vec4){ 1.0f, 0.0f, 0.0f, 1.0f }
    );
    aven_gl_shape_join_buffer_update(
        &win->gl,
        &ctx->app.data.join.buffer,
        &ctx->app.data.join.geometry
    );

    int width;
    int height;
    glfwGetFramebufferSize(win->window, &width, &height);
    aven_gl_Viewport(&win->gl, 0, 0, width, height);
    aven_gl_ClearColor(&win->gl, 0.75f, 0.75f, 0.75f, 1.0f);
    aven_gl_Clear(&win->gl, GL_COLOR_BUFFER_BIT);
    aven_gl_shape_join_draw(
        &win->gl,
        &ctx->app.data.join.ctx,
        &ctx->app.data.join.buffer,
        2.0f / (float)height,
        camera
    );
}

static void test_aven_gl_shape_join_deinit(AvenGlWindow *win) {
    TestCtx *ctx = win->ctx;
    aven_gl_shape_join_buffer_deinit(&win->gl, &ctx->app.data.join.buffer);
    aven_gl_shape_join_geometry_deinit(&ctx->app.data.join.geometry);
    aven_gl_shape_join_ctx_deinit(&win->gl, &ctx->app.data.join.ctx);
}

static void test_aven_gl_texture_init(AvenGlWindow *win) {
    TestCtx *ctx = win->ctx;
    AvenArena temp_arena = ctx->arena;
    ctx->app.state = TEST_AVEN_GL_TEXTURE;
    ctx->app.data.texture.ctx = aven_gl_texture_ctx_init(
        &win->gl,
        TSIZE,
        TSIZE,
        (AvenGlTextureBytesOptional){
            .valid = true,
            .value = slice_as_bytes(ctx->texture),
        }
    );
    ctx->app.data.texture.geometry = aven_gl_texture_geometry_init(
        1,
        &temp_arena
    );
    ctx->app.data.texture.buffer = aven_gl_texture_buffer_init(
        &win->gl,
        &ctx->app.data.texture.ctx,
        &ctx->app.data.texture.geometry,
        AVEN_GL_BUFFER_USAGE_DYNAMIC
    );
}

static void test_aven_gl_texture_update(AvenGlWindow *win, float t) {
    (void)t;
    TestCtx *ctx = win->ctx;

    Aff2 trans;
    aff2_identity(trans);
    Aff2 camera;
    aff2_camera_position(camera, (Vec2){ 0.0f, 0.0f }, (Vec2){ 1.25f, 1.25f });

    aven_gl_texture_geometry_clear(&ctx->app.data.texture.geometry);
    aven_gl_texture_geometry_push_square(
        &ctx->app.data.texture.geometry,
        trans,
        trans
    );
    aven_gl_texture_buffer_update(
        &win->gl,
        &ctx->app.data.texture.buffer,
        &ctx->app.data.texture.geometry
    );

    int width;
    int height;
    glfwGetFramebufferSize(win->window, &width, &height);
    aven_gl_Viewport(&win->gl, 0, 0, width, height);
    aven_gl_ClearColor(&win->gl, 0.75f, 0.75f, 0.75f, 1.0f);
    aven_gl_Clear(&win->gl, GL_COLOR_BUFFER_BIT);
    aven_gl_texture_draw(
        &win->gl,
        &ctx->app.data.texture.ctx,
        &ctx->app.data.texture.buffer,
        camera
    );
}

static void test_aven_gl_texture_deinit(AvenGlWindow *win) {
    TestCtx *ctx = win->ctx;
    aven_gl_texture_buffer_deinit(&win->gl, &ctx->app.data.texture.buffer);
    aven_gl_texture_geometry_deinit(&ctx->app.data.texture.geometry);
    aven_gl_texture_ctx_deinit(&win->gl, &ctx->app.data.texture.ctx);
}

static void test_aven_gl_text_init(AvenGlWindow *win) {
    TestCtx *ctx = win->ctx;
    AvenArena temp_arena = ctx->arena;

    AvenStr msg = aven_str("Hello, OpenGL!");
    ctx->app.state = TEST_AVEN_GL_TEXT;
    ByteSlice font_bytes = {
        .ptr = (unsigned char *)game_font_opensans_ttf,
        .len = sizeof(game_font_opensans_ttf),
    };
    ctx->app.data.text.font = aven_gl_text_font_init(
        &win->gl,
        font_bytes,
        48,
        ctx->arena
    );
    ctx->app.data.text.ctx = aven_gl_text_ctx_init(&win->gl);
    ctx->app.data.text.geometry = aven_gl_text_geometry_init(
        msg.len,
        &temp_arena
    );
    ctx->app.data.text.buffer = aven_gl_text_buffer_init(
        &win->gl,
        &ctx->app.data.text.ctx,
        &ctx->app.data.text.geometry,
        AVEN_GL_BUFFER_USAGE_DYNAMIC
    );
    ctx->app.data.text.line = aven_gl_text_line(
        &ctx->app.data.text.font,
        msg,
        &temp_arena
    );
}

static void test_aven_gl_text_update(AvenGlWindow *win, float t) {
    (void)t;
    TestCtx *ctx = win->ctx;

    Aff2 trans;
    aff2_identity(trans);

    int width;
    int height;
    glfwGetFramebufferSize(win->window, &width, &height);

    float screen_ratio = (float)width / (float)height;
    float norm_height = 1.0f;
    float norm_width = screen_ratio;
    float pixel_size = 2.0f / (float)height;

    if (screen_ratio < 1.0f) {
        norm_height = 1.0f / screen_ratio;
        norm_width = 1.0f;
        screen_ratio = 1.0f / screen_ratio;
        pixel_size = 2.0f / (float)width;
    }
    Aff2 camera;
    aff2_camera_position(
        camera,
        (Vec2){ 0.0f, 0.0f },
        (Vec2){ norm_width, norm_height }
    );

    aven_gl_text_geometry_clear(&ctx->app.data.text.geometry);
    aven_gl_text_geometry_push_line(
        &ctx->app.data.text.geometry,
        &ctx->app.data.text.line,
        trans,
        pixel_size,
        (Vec4){ 0.25f, 0.25f, 0.25f, 1.0f }
    );
    aven_gl_text_buffer_update(
        &win->gl,
        &ctx->app.data.text.buffer,
        &ctx->app.data.text.geometry
    );

    aven_gl_Viewport(&win->gl, 0, 0, width, height);
    aven_gl_ClearColor(&win->gl, 0.75f, 0.75f, 0.75f, 1.0f);
    aven_gl_Clear(&win->gl, GL_COLOR_BUFFER_BIT);
    aven_gl_text_draw(
        &win->gl,
        &ctx->app.data.text.ctx,
        &ctx->app.data.text.buffer,
        &ctx->app.data.text.font,
        camera
    );
}

static void test_aven_gl_text_deinit(AvenGlWindow *win) {
    TestCtx *ctx = win->ctx;
    aven_gl_text_buffer_deinit(&win->gl, &ctx->app.data.text.buffer);
    aven_gl_text_geometry_deinit(&ctx->app.data.text.geometry);
    aven_gl_text_ctx_deinit(&win->gl, &ctx->app.data.text.ctx);
}

static AvenGlWindowAction update(AvenGlWindow *win) {
    TestCtx *ctx = win->ctx;
    int64_t elapsed = aven_time_since(win->now, ctx->app.start);
    if (elapsed >= INTERVAL_NS) {
        switch (ctx->app.state) {
            case TEST_AVEN_GL_NONE:
                return AVEN_GL_WINDOW_ACTION_CLOSE;
                break;
            case TEST_AVEN_GL_SHAPE:
                test_aven_gl_shape_deinit(win);
                test_aven_gl_shape_rounded_init(win);
                break;
            case TEST_AVEN_GL_ROUNDED:
                test_aven_gl_shape_rounded_deinit(win);
                test_aven_gl_shape_join_init(win);
                break;
            case TEST_AVEN_GL_JOIN:
                test_aven_gl_shape_join_deinit(win);
                test_aven_gl_texture_init(win);
                break;
            case TEST_AVEN_GL_TEXTURE:
                test_aven_gl_texture_deinit(win);
                test_aven_gl_text_init(win);
                break;
            case TEST_AVEN_GL_TEXT:
                test_aven_gl_text_deinit(win);
                test_aven_gl_shape_init(win);
                break;
        }
        ctx->app.start = win->now;
    } else {
        float t = (float)elapsed / (float)INTERVAL_NS;
        switch (ctx->app.state) {
            case TEST_AVEN_GL_NONE:
                return AVEN_GL_WINDOW_ACTION_CLOSE;
                break;
            case TEST_AVEN_GL_SHAPE:
                test_aven_gl_shape_update(win, t);
                break;
            case TEST_AVEN_GL_ROUNDED:
                test_aven_gl_shape_rounded_update(win, t);
                break;
            case TEST_AVEN_GL_JOIN:
                test_aven_gl_shape_join_update(win, t);
                break;
            case TEST_AVEN_GL_TEXTURE:
                test_aven_gl_texture_update(win, t);
                break;
            case TEST_AVEN_GL_TEXT:
                test_aven_gl_text_update(win, t);
                break;
        }
    }

    return AVEN_GL_WINDOW_ACTION_SWAP;
}

static void load(AvenGlWindow *win) {
    TestCtx *ctx = win->ctx;
    bool fail = false;

    if (win->gl.ActiveTexture == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glActiveTexture\n");
    }
    if (win->gl.AttachShader == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glAttachShader\n");
    }
    if (win->gl.BindAttribLocation == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glBindAttribLocation\n");
    }
    if (win->gl.BindBuffer == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glBindBuffer\n");
    }
    if (win->gl.BindFramebuffer == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glBindFramebuffer\n");
    }
    if (win->gl.BindRenderbuffer == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glBindRenderbuffer\n");
    }
    if (win->gl.BindTexture == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glBindTexture\n");
    }
    if (win->gl.BlendColor == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glBlendColor\n");
    }
    if (win->gl.BlendEquation == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glBlendEquation\n");
    }
    if (win->gl.BlendEquationSeparate == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glBlendEquationSeparate\n");
    }
    if (win->gl.BlendFunc == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glBlendFunc\n");
    }
    if (win->gl.BlendFuncSeparate == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glBlendFuncSeparate\n");
    }
    if (win->gl.BufferData == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glBufferData\n");
    }
    if (win->gl.BufferSubData == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glBufferSubData\n");
    }
    if (win->gl.CheckFramebufferStatus == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glCheckFramebufferStatus\n");
    }
    if (win->gl.Clear == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glClear\n");
    }
    if (win->gl.ClearColor == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glClearColor\n");
    }
    if (win->gl.ClearDepthf == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glClearDepth\n");
    }
    if (win->gl.ClearStencil == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glClearStencil\n");
    }
    if (win->gl.ColorMask == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glColorMask\n");
    }
    if (win->gl.CompileShader == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glCompileShader\n");
    }
    if (win->gl.CompressedTexImage2D == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glCompressedTexImage2D\n");
    }
    if (win->gl.CompressedTexSubImage2D == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glCompressedTexSubImage2D\n");
    }
    if (win->gl.CopyTexImage2D == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glCopyTexImage2D\n");
    }
    if (win->gl.CopyTexSubImage2D == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glCopyTexSubImage2D\n");
    }
    if (win->gl.CreateProgram == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glCreateProgram\n");
    }
    if (win->gl.CreateShader == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glCreateShader\n");
    }
    if (win->gl.CullFace == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glCullFace\n");
    }
    if (win->gl.DeleteBuffers == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glDeleteBuffers\n");
    }
    if (win->gl.DeleteFramebuffers == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glDeleteFramebuffers\n");
    }
    if (win->gl.DeleteProgram == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glDeleteProgram\n");
    }
    if (win->gl.DeleteRenderbuffers == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glDeleteRenderbuffers\n");
    }
    if (win->gl.DeleteShader == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glDeleteShader\n");
    }
    if (win->gl.DeleteTextures == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glDeleteTextures\n");
    }
    if (win->gl.DepthFunc == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glDepthFunc\n");
    }
    if (win->gl.DepthMask == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glDepthMask\n");
    }
    if (win->gl.DepthRangef == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glDepthRangef\n");
    }
    if (win->gl.DetachShader == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glDetachShader\n");
    }
    if (win->gl.Disable == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glDisable\n");
    }
    if (win->gl.DisableVertexAttribArray == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glDisableVertexAttribArray\n");
    }
    if (win->gl.DrawArrays == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glDrawArrays\n");
    }
    if (win->gl.DrawElements == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glDrawElements\n");
    }
    if (win->gl.Enable == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glEnable\n");
    }
    if (win->gl.EnableVertexAttribArray == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glEnableVertexAttribArray\n");
    }
    if (win->gl.Finish == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glFinish\n");
    }
    if (win->gl.Flush == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glFlush\n");
    }
    if (win->gl.FramebufferRenderbuffer == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glFramebufferRenderbuffer\n");
    }
    if (win->gl.FramebufferTexture2D == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glFramebufferTexture2D\n");
    }
    if (win->gl.FrontFace == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glFrontFace\n");
    }
    if (win->gl.GenBuffers == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glGenBuffers\n");
    }
    if (win->gl.GenerateMipmap == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glGenerateMipmap\n");
    }
    if (win->gl.GenFramebuffers == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glGenFramebuffers\n");
    }
    if (win->gl.GenRenderbuffers == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glGenRenderbuffers\n");
    }
    if (win->gl.GenTextures == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glGenTextures\n");
    }
    if (win->gl.GetActiveAttrib == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glGetActiveAttrib\n");
    }
    if (win->gl.GetActiveUniform == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glGetActiveUniform\n");
    }
    if (win->gl.GetAttachedShaders == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glGetAttachedShaders\n");
    }
    if (win->gl.GetAttribLocation == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glGetAttribLocation\n");
    }
    if (win->gl.GetBooleanv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glGetBooleanv\n");
    }
    if (win->gl.GetBufferParameteriv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glGetBufferParameteriv\n");
    }
    if (win->gl.GetError == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glGetError\n");
    }
    if (win->gl.GetFloatv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glGetFloatv\n");
    }
    if (win->gl.GetFramebufferAttachmentParameteriv == NULL) {
        fail = true;
        aven_io_print(
            "test failed: aven_gl_load glGetFramebufferAttachmentParameteriv\n"
        );
    }
    if (win->gl.GetIntegerv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glGetIntegerv\n");
    }
    if (win->gl.GetProgramiv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glGetProgramiv\n");
    }
    if (win->gl.GetProgramInfoLog == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glGetProgramInfoLog\n");
    }
    if (win->gl.GetRenderbufferParameteriv == NULL) {
        fail = true;
        aven_io_print(
            "test failed: aven_gl_load glGetRenderbufferParameteriv\n"
        );
    }
    if (win->gl.GetShaderiv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glGetShaderiv\n");
    }
    if (win->gl.GetShaderInfoLog == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glGetShaderInfoLog\n");
    }
    if (win->gl.GetShaderPrecisionFormat == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glGetShaderPrecisionFormat\n");
    }
    if (win->gl.GetShaderSource == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glGetShaderSource\n");
    }
    if (win->gl.GetString == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glGetString\n");
    }
    if (win->gl.GetTexParameterfv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glGetTexParameterfv\n");
    }
    if (win->gl.GetTexParameteriv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glGetTexParameteriv\n");
    }
    if (win->gl.GetUniformfv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glGetUniformfv\n");
    }
    if (win->gl.GetUniformiv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glGetUniformiv\n");
    }
    if (win->gl.GetUniformLocation == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glGetUniformLocation\n");
    }
    if (win->gl.GetVertexAttribfv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glGetVertexAttribfv\n");
    }
    if (win->gl.GetVertexAttribiv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glGetVertexAttribiv\n");
    }
    if (win->gl.GetVertexAttribPointerv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glGetVertexAttribPointerv\n");
    }
    if (win->gl.Hint == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glHint\n");
    }
    if (win->gl.IsBuffer == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glIsBuffer\n");
    }
    if (win->gl.IsEnabled == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glIsEnabled\n");
    }
    if (win->gl.IsFramebuffer == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glIsFramebuffer\n");
    }
    if (win->gl.IsProgram == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glIsProgram\n");
    }
    if (win->gl.IsRenderbuffer == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glIsRenderbuffer\n");
    }
    if (win->gl.IsShader == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glIsShader\n");
    }
    if (win->gl.IsTexture == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glIsTexture\n");
    }
    if (win->gl.LineWidth == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glLineWidth\n");
    }
    if (win->gl.LinkProgram == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glLinkProgram\n");
    }
    if (win->gl.PixelStorei == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glPixelStorei\n");
    }
    if (win->gl.PolygonOffset == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glPolygonOffset\n");
    }
    if (win->gl.ReadPixels == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glReadPixels\n");
    }
    if (win->gl.ReleaseShaderCompiler == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glReleaseShaderCompiler\n");
    }
    if (win->gl.RenderbufferStorage == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glRenderbufferStorage\n");
    }
    if (win->gl.SampleCoverage == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glSampleCoverage\n");
    }
    if (win->gl.Scissor == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glScissor\n");
    }
    if (win->gl.ShaderBinary == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glShaderBinary\n");
    }
    if (win->gl.ShaderSource == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glShaderSource\n");
    }
    if (win->gl.StencilFunc == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glStencilFunc\n");
    }
    if (win->gl.StencilFuncSeparate == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glStencilFuncSeparate\n");
    }
    if (win->gl.StencilMask == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glStencilMask\n");
    }
    if (win->gl.StencilMaskSeparate == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glStencilMaskSeparate\n");
    }
    if (win->gl.StencilOp == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glStencilOp\n");
    }
    if (win->gl.StencilOpSeparate == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glStencilOpSeparate\n");
    }
    if (win->gl.TexImage2D == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glTexImage2D\n");
    }
    if (win->gl.TexParameterf == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glTexParameterf\n");
    }
    if (win->gl.TexParameterfv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glTexParameterfv\n");
    }
    if (win->gl.TexParameteri == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glTexParameteri\n");
    }
    if (win->gl.TexParameteriv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glTexParameteriv\n");
    }
    if (win->gl.TexSubImage2D == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glTexSubImage2D\n");
    }
    if (win->gl.Uniform1f == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glUniform1f\n");
    }
    if (win->gl.Uniform1fv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glUniform1fv\n");
    }
    if (win->gl.Uniform1i == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glUniform1i\n");
    }
    if (win->gl.Uniform1iv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glUniform1iv\n");
    }
    if (win->gl.Uniform2f == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glUniform2f\n");
    }
    if (win->gl.Uniform2fv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glUniform2fv\n");
    }
    if (win->gl.Uniform2i == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glUniform2i\n");
    }
    if (win->gl.Uniform2iv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glUniform2iv\n");
    }
    if (win->gl.Uniform3f == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glUniform3f\n");
    }
    if (win->gl.Uniform3fv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glUniform3fv\n");
    }
    if (win->gl.Uniform3i == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glUniform3i\n");
    }
    if (win->gl.Uniform3iv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glUniform3iv\n");
    }
    if (win->gl.Uniform4f == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glUniform4f\n");
    }
    if (win->gl.Uniform4fv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glUniform4fv\n");
    }
    if (win->gl.Uniform4i == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glUniform4i\n");
    }
    if (win->gl.Uniform4iv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glUniform4iv\n");
    }
    if (win->gl.UniformMatrix2fv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glUniformMatrix2fv\n");
    }
    if (win->gl.UniformMatrix3fv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glUniformMatrix3fv\n");
    }
    if (win->gl.UniformMatrix4fv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glUniformMatrix4fv\n");
    }
    if (win->gl.UseProgram == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glUseProgram\n");
    }
    if (win->gl.ValidateProgram == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glValidateProgram\n");
    }
    if (win->gl.VertexAttrib1f == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glVertexAttrib1f\n");
    }
    if (win->gl.VertexAttrib1fv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glVertexAttrib1fv\n");
    }
    if (win->gl.VertexAttrib2f == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glVertexAttrib2f\n");
    }
    if (win->gl.VertexAttrib2fv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glVertexAttrib2fv\n");
    }
    if (win->gl.VertexAttrib3f == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glVertexAttrib3f\n");
    }
    if (win->gl.VertexAttrib3fv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glVertexAttrib3fv\n");
    }
    if (win->gl.VertexAttrib4f == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glVertexAttrib4f\n");
    }
    if (win->gl.VertexAttrib4fv == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glVertexAttrib4fv\n");
    }
    if (win->gl.VertexAttribPointer == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glVertexAttribPointer\n");
    }
    if (win->gl.Viewport == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glViewport\n");
    }
    if (win->gl.BindVertexArray == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glBindVertexArray\n");
    }
    if (win->gl.GenVertexArrays == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glGenVertexArrays\n");
    }
    if (win->gl.DeleteVertexArrays == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glDeleteVertexArrays\n");
    }
    if (win->gl.IsVertexArray == NULL) {
        fail = true;
        aven_io_print("test failed: aven_gl_load glIsVertexArray\n");
    }

    if (!fail) {
        test_aven_gl_text_init(win);
    }
    ctx->app.start = aven_time_now();
}

static void init(AvenGlWindow *win) {
    TestCtx *ctx = win->ctx;
    switch (ctx->app.state) {
        case TEST_AVEN_GL_NONE:
            load(win);
            break;
        case TEST_AVEN_GL_SHAPE:
            test_aven_gl_shape_init(win);
            break;
        case TEST_AVEN_GL_ROUNDED:
            test_aven_gl_shape_rounded_init(win);
            break;
        case TEST_AVEN_GL_JOIN:
            test_aven_gl_shape_join_init(win);
            break;
        case TEST_AVEN_GL_TEXTURE:
            test_aven_gl_texture_init(win);
            break;
        case TEST_AVEN_GL_TEXT:
            test_aven_gl_text_init(win);
            break;
    }
}

static void deinit(AvenGlWindow *win) {
    TestCtx *ctx = win->ctx;
    switch (ctx->app.state) {
        case TEST_AVEN_GL_NONE:
            break;
        case TEST_AVEN_GL_SHAPE:
            test_aven_gl_shape_deinit(win);
            break;
        case TEST_AVEN_GL_ROUNDED:
            test_aven_gl_shape_rounded_deinit(win);
            break;
        case TEST_AVEN_GL_JOIN:
            test_aven_gl_shape_join_deinit(win);
            break;
        case TEST_AVEN_GL_TEXTURE:
            test_aven_gl_texture_deinit(win);
            break;
        case TEST_AVEN_GL_TEXT:
            test_aven_gl_text_deinit(win);
            break;
    }
}

void damage(AvenGlWindow *win) {
    aven_io_printf(
        "damage: ({}, {})\n",
        aven_fmt_int(win->width),
        aven_fmt_int(win->height)
    );
}

void key(AvenGlWindow *win, int key, int scancode, int action, int modes) {
    (void)win;
    aven_io_printf(
        "key: ({}, {}, {}, {})\n",
        aven_fmt_int(key),
        aven_fmt_int(scancode),
        aven_fmt_int(action),
        aven_fmt_int(modes)
    );
}

void mouse_click(AvenGlWindow *win, Vec2 pos, int button, int action, int modes) {
    (void)win;
    aven_io_printf(
        "click: (({}, {}), {}, {}, {})\n",
        aven_fmt_int((int)pos[0]),
        aven_fmt_int((int)pos[1]),
        aven_fmt_int(button),
        aven_fmt_int(action),
        aven_fmt_int(modes)
    );
}

void mouse_move(AvenGlWindow *win, Vec2 pos) {
    (void)win;
    aven_io_printf(
        "mouse: ({}, {})\n",
        aven_fmt_int((int)pos[0]),
        aven_fmt_int((int)pos[1])
    );
}

void mouse_enter(AvenGlWindow *win, bool entered) {
    (void)win;
    if (entered) {
        aven_io_print("mouse entered\n");
    } else {
        aven_io_print("mouse left\n");
    }
}

static uint32_t texture[TSIZE * TSIZE] = {
    0xffffffff,
    0xff0000ff,
    0xff0000ff,
    0xffffffff,
};
static AvenArena arena;
static TestCtx ctx;

int main(void) {
    void *mem = malloc(ARENA_SIZE);
    if (mem == NULL) {
        aven_panic("malloc failed");
    }

    arena = aven_arena_init(mem, ARENA_SIZE);
    ctx = (TestCtx){ .texture = slice_array(texture), .arena = arena };

    AvenGlWindowCode rcode = aven_gl_window_impl(
        INIT_WIDTH,
        INIT_HEIGHT,
        "AvenGL Test",
        (AvenGlWindowVtable){
            .init = init,
            .deinit = deinit,
            .update = update,
            .damage = { .value = damage },
            .key = { .value = key },
            .mouse_click = { .value = mouse_click },
            .mouse_move = { .value = mouse_move },
            .mouse_enter = { .value = mouse_enter },
        },
        &ctx
    );

    return (int)rcode;
}

#ifdef _MSC_VER
    int WinMain(void) {
        main();
    }
#endif
