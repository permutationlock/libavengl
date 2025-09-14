#ifndef AVEN_GL_SHAPE_H
    #define AVEN_GL_SHAPE_H

    #include <aven.h>
    #include <aven/arena.h>
    #include <aven/math.h>

    #include "../gl.h"

    typedef struct {
        Vec4 color;
        Vec2 pos;
    } AvenGlShapeVertex;

    typedef struct {
        IVec2 pos;
        IVec2 dim;
        size_t start;
        size_t end;
    } AvenGlShapeScissor;

    typedef struct {
        List(AvenGlShapeVertex) vertices;
        List(GLushort) indices;
    } AvenGlShapeGeometry;

    typedef struct {
        GLuint vertex_shader;
        GLuint fragment_shader;
        GLuint program;
        GLuint utrans_location;
        GLuint upos_location;
        GLuint vpos_location;
        GLuint vcolor_location;
    } AvenGlShapeCtx;

    typedef struct {
        size_t vertex_cap;
        size_t index_cap;
        size_t index_len;
        GLuint vertex;
        GLuint index;
        GLuint vao;
        AvenGlBufferUsage usage;
    } AvenGlShapeBuffer;

    static inline AvenGlShapeGeometry aven_gl_shape_geometry_init(
        size_t max_vertices,
        size_t max_indices,
        AvenArena *arena
    ) {
        assert(max_indices >= max_vertices);

        AvenGlShapeGeometry geometry = {
            .vertices = { .cap = max_vertices },
            .indices = { .cap = max_indices },
        };
        geometry.vertices.ptr = aven_arena_create_array(
            AvenGlShapeVertex,
            arena,
            geometry.vertices.cap
        );
        geometry.indices.ptr = aven_arena_create_array(
            GLushort,
            arena,
            geometry.indices.cap
        );

        return geometry;
    }

    static inline void aven_gl_shape_geometry_clear(
        AvenGlShapeGeometry *geometry
    ) {
        geometry->vertices.len = 0;
        geometry->indices.len = 0;
    }

    static inline void aven_gl_shape_geometry_deinit(
        AvenGlShapeGeometry *geometry
    ) {
        *geometry = (AvenGlShapeGeometry){ 0 };
    }

    static inline AvenGlShapeCtx aven_gl_shape_ctx_init(AvenGl *gl) {
        AvenGlShapeCtx ctx = { 0 };

        const char *vertex_shader_text = aven_gl_shader(
            gl,
            "precision mediump float;\n"
            "in vec2 vPos;\n"
            "in vec4 vColor;\n"
            "uniform mat2 uTrans;\n"
            "uniform vec2 uPos;\n"
            "out vec4 fColor;\n"
            "void main() {\n"
            "    gl_Position = vec4((uTrans * vPos.xy) + uPos, 0.0, 1.0);\n"
            "    fColor = vColor;\n"
            "}\n"
        );

        const char *fragment_shader_text = aven_gl_shader(
            gl,
            "precision mediump float;\n"
            "in vec4 fColor;\n"
            "out vec4 FragColor;\n"
            "void main() {\n"
            "    FragColor = fColor;\n"
            "}\n"
        );

        ctx.vertex_shader = aven_gl_CreateShader(gl, GL_VERTEX_SHADER);
        aven_gl_ShaderSource(
            gl,
            ctx.vertex_shader,
            1,
            &vertex_shader_text,
            NULL
        );
        aven_gl_CompileShader(gl, ctx.vertex_shader);

        ctx.fragment_shader = aven_gl_CreateShader(gl, GL_FRAGMENT_SHADER);
        aven_gl_ShaderSource(
            gl,
            ctx.fragment_shader,
            1,
            &fragment_shader_text,
            NULL
        );
        aven_gl_CompileShader(gl, ctx.fragment_shader);

        ctx.program = aven_gl_CreateProgram(gl);
        aven_gl_AttachShader(gl, ctx.program, ctx.vertex_shader);
        aven_gl_AttachShader(gl, ctx.program, ctx.fragment_shader);
        aven_gl_LinkProgram(gl, ctx.program);

        ctx.utrans_location = (GLuint)aven_gl_GetUniformLocation(
            gl,
            ctx.program,
            "uTrans"
        );
        ctx.upos_location = (GLuint)aven_gl_GetUniformLocation(
            gl,
            ctx.program,
            "uPos"
        );

        ctx.vpos_location = (GLuint)aven_gl_GetAttribLocation(
            gl,
            ctx.program,
            "vPos"
        );
        ctx.vcolor_location = (GLuint)aven_gl_GetAttribLocation(
            gl,
            ctx.program,
            "vColor"
        );

        return ctx;
    }

    static inline void aven_gl_shape_ctx_deinit(AvenGl *gl, AvenGlShapeCtx *ctx) {
        aven_gl_DeleteProgram(gl, ctx->program);
        aven_gl_DeleteShader(gl, ctx->fragment_shader);
        aven_gl_DeleteShader(gl, ctx->vertex_shader);
        *ctx = (AvenGlShapeCtx){ 0 };
    }

    static inline AvenGlShapeBuffer aven_gl_shape_buffer_init(
        AvenGl *gl,
        AvenGlShapeCtx *ctx,
        AvenGlShapeGeometry *geometry,
        AvenGlBufferUsage buffer_usage
    ) {
        AvenGlShapeBuffer buffer = { .usage = buffer_usage };

        switch (buffer_usage) {
            case AVEN_GL_BUFFER_USAGE_DYNAMIC:
                buffer.vertex_cap = geometry->vertices.cap;
                buffer.index_cap = geometry->indices.cap;
                break;
            case AVEN_GL_BUFFER_USAGE_STATIC:
            case AVEN_GL_BUFFER_USAGE_STREAM:
                buffer.vertex_cap = geometry->vertices.len;
                buffer.index_cap = geometry->indices.len;
                buffer.index_len = geometry->indices.len;
                break;
            default:
                assert(false);
        }

        aven_gl_GenVertexArrays(gl, 1, &buffer.vao);
        aven_gl_BindVertexArray(gl, buffer.vao);

        aven_gl_GenBuffers(gl, 1, &buffer.vertex);
        aven_gl_BindBuffer(gl, GL_ARRAY_BUFFER, buffer.vertex);
        aven_gl_BufferData(
            gl,
            GL_ARRAY_BUFFER,
            (GLsizeiptr)(buffer.vertex_cap * sizeof(*geometry->vertices.ptr)),
            geometry->vertices.ptr,
            (GLenum)buffer_usage
        );
        aven_gl_VertexAttribPointer(
            gl,
            ctx->vpos_location,
            2,
            GL_FLOAT,
            GL_FALSE,
            sizeof(AvenGlShapeVertex),
            (void *)offsetof(AvenGlShapeVertex, pos)
        );
        aven_gl_EnableVertexAttribArray(gl, ctx->vpos_location);
        aven_gl_VertexAttribPointer(
            gl,
            ctx->vcolor_location,
            4,
            GL_FLOAT,
            GL_FALSE,
            sizeof(AvenGlShapeVertex),
            (void *)offsetof(AvenGlShapeVertex, color)
        );
        aven_gl_EnableVertexAttribArray(gl, ctx->vcolor_location);

        aven_gl_BindBuffer(gl, GL_ARRAY_BUFFER, 0);

        aven_gl_BindVertexArray(gl, 0);

        aven_gl_GenBuffers(gl, 1, &buffer.index);
        aven_gl_BindBuffer(gl, GL_ELEMENT_ARRAY_BUFFER, buffer.index);
        aven_gl_BufferData(
            gl,
            GL_ELEMENT_ARRAY_BUFFER,
            (GLsizeiptr)(buffer.index_cap * sizeof(*geometry->indices.ptr)),
            geometry->indices.ptr,
            (GLenum)buffer_usage
        );

        aven_gl_BindBuffer(gl, GL_ELEMENT_ARRAY_BUFFER, 0);

        return buffer;
    }

    static inline void aven_gl_shape_buffer_deinit(
        AvenGl *gl,
        AvenGlShapeBuffer *buffer
    ) {
        aven_gl_DeleteBuffers(gl, 1, &buffer->index);
        aven_gl_DeleteVertexArrays(gl, 1, &buffer->vao);
        aven_gl_DeleteBuffers(gl, 1, &buffer->vertex);
        *buffer = (AvenGlShapeBuffer){ 0 };
    }

    static inline void aven_gl_shape_buffer_update(
        AvenGl *gl,
        AvenGlShapeBuffer *buffer,
        AvenGlShapeGeometry *geometry
    ) {
        assert(buffer->usage == AVEN_GL_BUFFER_USAGE_DYNAMIC);
        assert(geometry->vertices.len <= buffer->vertex_cap);
        assert(geometry->indices.len <= buffer->index_cap);

        aven_gl_BindBuffer(gl, GL_ARRAY_BUFFER, buffer->vertex);

        aven_gl_BufferSubData(
            gl,
            GL_ARRAY_BUFFER,
            0,
            (GLsizeiptr)(
                sizeof(*geometry->vertices.ptr) * geometry->vertices.len
            ),
            geometry->vertices.ptr
        );

        aven_gl_BindBuffer(gl, GL_ARRAY_BUFFER, 0);

        aven_gl_BindBuffer(gl, GL_ELEMENT_ARRAY_BUFFER, buffer->index);

        aven_gl_BufferSubData(
            gl,
            GL_ELEMENT_ARRAY_BUFFER,
            0,
            (GLsizeiptr)(geometry->indices.len * sizeof(*geometry->indices.ptr)),
            geometry->indices.ptr
        );

        buffer->index_len = geometry->indices.len;

        aven_gl_BindBuffer(gl, GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    static inline void aven_gl_shape_draw(
        AvenGl *gl,
        AvenGlShapeCtx *ctx,
        AvenGlShapeBuffer *buffer,
        Aff2 cam_trans
    ) {
        aven_gl_UseProgram(gl, ctx->program);

        aven_gl_BindVertexArray(gl, buffer->vao);
        aven_gl_BindBuffer(gl, GL_ARRAY_BUFFER, buffer->vertex);
        aven_gl_BindBuffer(gl, GL_ELEMENT_ARRAY_BUFFER, buffer->index);

        aven_gl_Enable(gl, GL_BLEND);
        aven_gl_BlendFunc(gl, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        aven_gl_UniformMatrix2fv(
            gl,
            (GLint)ctx->utrans_location,
            1,
            GL_FALSE,
            (GLfloat *)cam_trans
        );
        aven_gl_Uniform2fv(
            gl,
            (GLint)ctx->upos_location,
            1,
            (GLfloat *)cam_trans[2]
        );

        aven_gl_DrawElements(
            gl,
            GL_TRIANGLES,
            (GLsizei)buffer->index_len,
            GL_UNSIGNED_SHORT,
            0
        );

        aven_gl_Disable(gl, GL_BLEND);

        aven_gl_BindBuffer(gl, GL_ELEMENT_ARRAY_BUFFER, 0);
        aven_gl_BindBuffer(gl, GL_ARRAY_BUFFER, 0);
        aven_gl_BindVertexArray(gl, 0);
    }

    static inline void aven_gl_shape_geometry_push_triangle(
        AvenGlShapeGeometry *geometry,
        Aff2 trans,
        Vec2 p1,
        Vec2 p2,
        Vec2 p3,
        Vec4 color
    ) {
        aff2_transform(p1, trans, p1);
        aff2_transform(p2, trans, p2);
        aff2_transform(p3, trans, p3);

        size_t start_index = geometry->vertices.len;

        list_push(geometry->vertices) = (AvenGlShapeVertex){
            .pos = { p1[0], p1[1] },
            .color = { color[0], color[1], color[2], color[3] },
        };
        list_push(geometry->vertices) = (AvenGlShapeVertex){
            .pos = { p2[0], p2[1] },
            .color = { color[0], color[1], color[2], color[3] },
        };
        list_push(geometry->vertices) = (AvenGlShapeVertex){
            .pos = { p3[0], p3[1] },
            .color = { color[0], color[1], color[2], color[3] },
        };

        list_push(geometry->indices) = (GLushort)start_index + 0;
        list_push(geometry->indices) = (GLushort)start_index + 1;
        list_push(geometry->indices) = (GLushort)start_index + 2;
    }

    static inline void aven_gl_shape_geometry_push_triangle_isoceles(
        AvenGlShapeGeometry *geometry,
        Aff2 trans,
        Vec4 color
    ) {
        Vec2 p1 = { 0.0f, 1.0f };
        Vec2 p2 = { 1.0f, -1.0f };
        Vec2 p3 = { -1.0f, -1.0f };

        aven_gl_shape_geometry_push_triangle(geometry, trans, p1, p2, p3, color);
    }

    static inline void aven_gl_shape_geometry_push_triangle_right(
        AvenGlShapeGeometry *geometry,
        Aff2 trans,
        Vec4 color
    ) {
        Vec2 p1 = { -1.0f, 1.0f };
        Vec2 p2 = { 1.0f, -1.0f };
        Vec2 p3 = { -1.0f, -1.0f };

        aven_gl_shape_geometry_push_triangle(geometry, trans, p1, p2, p3, color);
    }

    static inline void aven_gl_shape_geometry_push_square(
        AvenGlShapeGeometry *geometry,
        Aff2 trans,
        Vec4 color
    ) {
        Vec2 p1 = { -1.0f, -1.0f };
        Vec2 p2 = { 1.0f, -1.0f };
        Vec2 p3 = { 1.0f, 1.0f };
        Vec2 p4 = { -1.0f, 1.0f };

        aff2_transform(p1, trans, p1);
        aff2_transform(p2, trans, p2);
        aff2_transform(p3, trans, p3);
        aff2_transform(p4, trans, p4);

        size_t start_index = geometry->vertices.len;

        list_push(geometry->vertices) = (AvenGlShapeVertex){
            .pos = { p1[0], p1[1] },
            .color = { color[0], color[1], color[2], color[3] },
        };
        list_push(geometry->vertices) = (AvenGlShapeVertex){
            .pos = { p2[0], p2[1] },
            .color = { color[0], color[1], color[2], color[3] },
        };
        list_push(geometry->vertices) = (AvenGlShapeVertex){
            .pos = { p3[0], p3[1] },
            .color = { color[0], color[1], color[2], color[3] },
        };
        list_push(geometry->vertices) = (AvenGlShapeVertex){
            .pos = { p4[0], p4[1] },
            .color = { color[0], color[1], color[2], color[3] },
        };

        list_push(geometry->indices) = (GLushort)start_index + 0;
        list_push(geometry->indices) = (GLushort)start_index + 1;
        list_push(geometry->indices) = (GLushort)start_index + 2;
        list_push(geometry->indices) = (GLushort)start_index + 0;
        list_push(geometry->indices) = (GLushort)start_index + 2;
        list_push(geometry->indices) = (GLushort)start_index + 3;
    }

    // Rounded

    typedef struct {
        Vec4 color;
        Vec4 info;
        Vec2 pos;
    } AvenGlShapeRoundedVertex;

    typedef struct {
        List(AvenGlShapeRoundedVertex) vertices;
        List(GLushort) indices;
    } AvenGlShapeRoundedGeometry;

    typedef struct {
        size_t vertex_cap;
        size_t index_cap;
        size_t index_len;
        GLuint vertex;
        GLuint index;
        GLuint vao;
        AvenGlBufferUsage usage;
    } AvenGlShapeRoundedBuffer;

    typedef struct {
        GLuint vertex_shader;
        GLuint fragment_shader;
        GLuint program;
        GLuint utrans_location;
        GLuint upos_location;
        GLuint upx_location;
        GLuint vpos_location;
        GLuint vinfo_location;
        GLuint vcolor_location;
    } AvenGlShapeRoundedCtx;

    static inline AvenGlShapeRoundedGeometry aven_gl_shape_rounded_geometry_init(
        size_t max_vertices,
        size_t max_indices,
        AvenArena *arena
    ) {
        assert(max_indices >= max_vertices);

        AvenGlShapeRoundedGeometry geometry = {
            .vertices = { .cap = max_vertices },
            .indices = { .cap = max_indices },
        };
        geometry.vertices.ptr = aven_arena_create_array(
            AvenGlShapeRoundedVertex,
            arena,
            geometry.vertices.cap
        );
        geometry.indices.ptr = aven_arena_create_array(
            GLushort,
            arena,
            geometry.indices.cap
        );

        return geometry;
    }

    static inline void aven_gl_shape_rounded_geometry_clear(
        AvenGlShapeRoundedGeometry *geometry
    ) {
        geometry->vertices.len = 0;
        geometry->indices.len = 0;
    }

    static inline void aven_gl_shape_rounded_geometry_deinit(
        AvenGlShapeRoundedGeometry *geometry
    ) {
        *geometry = (AvenGlShapeRoundedGeometry){ 0 };
    }

    static inline AvenGlShapeRoundedCtx aven_gl_shape_rounded_ctx_init(
        AvenGl *gl
    ) {
        AvenGlShapeRoundedCtx ctx = { 0 };

        const char *vertex_shader_text = aven_gl_shader(
            gl,
            "precision mediump float;\n"
            "uniform mat2 uTrans;\n"
            "uniform vec2 uPos;\n"
            "uniform float uPx;\n"
            "in vec2 vPos;\n"
            "in vec4 vInfo;\n"
            "in vec4 vColor;\n"
            "out vec2 tPos;\n"
            "out vec2 tOffset;\n"
            "out vec4 fColor;\n"
            "void main() {\n"
            "    gl_Position = vec4((uTrans * vPos.xy) + uPos, 0.0, 1.0);\n"
            "    tPos = vInfo.xy;\n"
            "    tOffset = vec2(uPx * vInfo.z, uPx * vInfo.w);\n"
            "    fColor = vColor;\n"
            "}\n"
        );

        const char *fragment_shader_text = aven_gl_shader(
            gl,
            "precision mediump float;\n"
            "in vec2 tPos;\n"
            "in vec2 tOffset;\n"
            "in vec4 fColor;\n"
            "out vec4 FragColor;\n"
            "float msaa(vec2 p) {\n"
            "    if (dot(p, p) > 1.0) {\n"
            "        return 0.0;\n"
            "    }\n"
            "    return 1.0;\n"
            "}\n"
            "void main() {\n"
            "    float magnitude = 0.0;\n"
            "    magnitude += msaa(tPos);\n"
            "    magnitude += msaa(tPos + vec2(0, -tOffset.y));\n"
            "    magnitude += msaa(tPos + vec2(tOffset.x, 0));\n"
            "    magnitude += msaa(tPos + vec2(0, tOffset.y));\n"
            "    magnitude += msaa(tPos + vec2(-tOffset.x, 0));\n"
            "    magnitude += msaa(tPos + vec2(-tOffset.x, tOffset.y));\n"
            "    magnitude += msaa(tPos + vec2(-tOffset.x, -tOffset.y));\n"
            "    magnitude += msaa(tPos + vec2(tOffset.x, -tOffset.y));\n"
            "    magnitude += msaa(tPos + vec2(tOffset.x, tOffset.y));\n"
            "    magnitude /= 9.0;\n"
            "    FragColor = vec4(fColor.xyz, fColor.w * magnitude);\n"
            "}\n"
        );

        ctx.vertex_shader = aven_gl_CreateShader(gl, GL_VERTEX_SHADER);
        aven_gl_ShaderSource(
            gl,
            ctx.vertex_shader,
            1,
            &vertex_shader_text,
            NULL
        );
        aven_gl_CompileShader(gl, ctx.vertex_shader);

        ctx.fragment_shader = aven_gl_CreateShader(gl, GL_FRAGMENT_SHADER);
        aven_gl_ShaderSource(
            gl,
            ctx.fragment_shader,
            1,
            &fragment_shader_text,
            NULL
        );
        aven_gl_CompileShader(gl, ctx.fragment_shader);

        ctx.program = aven_gl_CreateProgram(gl);
        aven_gl_AttachShader(gl, ctx.program, ctx.vertex_shader);
        aven_gl_AttachShader(gl, ctx.program, ctx.fragment_shader);
        aven_gl_LinkProgram(gl, ctx.program);

        ctx.utrans_location = (GLuint)aven_gl_GetUniformLocation(
            gl,
            ctx.program,
            "uTrans"
        );
        ctx.upos_location = (GLuint)aven_gl_GetUniformLocation(
            gl,
            ctx.program,
            "uPos"
        );
        ctx.upx_location = (GLuint)aven_gl_GetUniformLocation(
            gl,
            ctx.program,
            "uPx"
        );

        ctx.vpos_location = (GLuint)aven_gl_GetAttribLocation(
            gl,
            ctx.program,
            "vPos"
        );
        ctx.vinfo_location = (GLuint)aven_gl_GetAttribLocation(
            gl,
            ctx.program,
            "vInfo"
        );
        ctx.vcolor_location = (GLuint)aven_gl_GetAttribLocation(
            gl,
            ctx.program,
            "vColor"
        );

        return ctx;
    }

    static inline void aven_gl_shape_rounded_ctx_deinit(
        AvenGl *gl,
        AvenGlShapeRoundedCtx *ctx
    ) {
        aven_gl_DeleteProgram(gl, ctx->program);
        aven_gl_DeleteShader(gl, ctx->fragment_shader);
        aven_gl_DeleteShader(gl, ctx->vertex_shader);
        *ctx = (AvenGlShapeRoundedCtx){ 0 };
    }

    static inline AvenGlShapeRoundedBuffer aven_gl_shape_rounded_buffer_init(
        AvenGl *gl,
        AvenGlShapeRoundedCtx *ctx,
        AvenGlShapeRoundedGeometry *geometry,
        AvenGlBufferUsage buffer_usage
    ) {
        AvenGlShapeRoundedBuffer buffer = { .usage = buffer_usage };

        switch (buffer_usage) {
            case AVEN_GL_BUFFER_USAGE_DYNAMIC:
                buffer.vertex_cap = geometry->vertices.cap;
                buffer.index_cap = geometry->indices.cap;
                break;
            case AVEN_GL_BUFFER_USAGE_STATIC:
            case AVEN_GL_BUFFER_USAGE_STREAM:
                buffer.vertex_cap = geometry->vertices.len;
                buffer.index_cap = geometry->indices.len;
                buffer.index_len = buffer.index_len;
                break;
            default:
                assert(false);
        }

        aven_gl_GenVertexArrays(gl, 1, &buffer.vao);
        aven_gl_BindVertexArray(gl, buffer.vao);

        aven_gl_GenBuffers(gl, 1, &buffer.vertex);
        aven_gl_BindBuffer(gl, GL_ARRAY_BUFFER, buffer.vertex);
        aven_gl_BufferData(
            gl,
            GL_ARRAY_BUFFER,
            (GLsizeiptr)(buffer.vertex_cap * sizeof(*geometry->vertices.ptr)),
            geometry->vertices.ptr,
            (GLenum)buffer_usage
        );

        aven_gl_VertexAttribPointer(
            gl,
            ctx->vpos_location,
            2,
            GL_FLOAT,
            GL_FALSE,
            sizeof(AvenGlShapeRoundedVertex),
            (void *)offsetof(AvenGlShapeRoundedVertex, pos)
        );
        aven_gl_EnableVertexAttribArray(gl, ctx->vpos_location);
        aven_gl_VertexAttribPointer(
            gl,
            ctx->vinfo_location,
            4,
            GL_FLOAT,
            GL_FALSE,
            sizeof(AvenGlShapeRoundedVertex),
            (void *)offsetof(AvenGlShapeRoundedVertex, info)
        );
        aven_gl_EnableVertexAttribArray(gl, ctx->vinfo_location);
        aven_gl_VertexAttribPointer(
            gl,
            ctx->vcolor_location,
            4,
            GL_FLOAT,
            GL_FALSE,
            sizeof(AvenGlShapeRoundedVertex),
            (void *)offsetof(AvenGlShapeRoundedVertex, color)
        );
        aven_gl_EnableVertexAttribArray(gl, ctx->vcolor_location);

        aven_gl_BindBuffer(gl, GL_ARRAY_BUFFER, 0);

        aven_gl_BindVertexArray(gl, 0);

        aven_gl_GenBuffers(gl, 1, &buffer.index);
        aven_gl_BindBuffer(gl, GL_ELEMENT_ARRAY_BUFFER, buffer.index);
        aven_gl_BufferData(
            gl,
            GL_ELEMENT_ARRAY_BUFFER,
            (GLsizeiptr)(buffer.index_cap * sizeof(*geometry->indices.ptr)),
            geometry->indices.ptr,
            (GLenum)buffer_usage
        );

        aven_gl_BindBuffer(gl, GL_ELEMENT_ARRAY_BUFFER, 0);

        return buffer;
    }

    static inline void aven_gl_shape_rounded_buffer_deinit(
        AvenGl *gl,
        AvenGlShapeRoundedBuffer *buffer
    ) {
        aven_gl_DeleteBuffers(gl, 1, &buffer->index);
        aven_gl_DeleteVertexArrays(gl, 1, &buffer->vao);
        aven_gl_DeleteBuffers(gl, 1, &buffer->vertex);
        *buffer = (AvenGlShapeRoundedBuffer){ 0 };
    }

    static inline void aven_gl_shape_rounded_buffer_update(
        AvenGl *gl,
        AvenGlShapeRoundedBuffer *buffer,
        AvenGlShapeRoundedGeometry *geometry
    ) {
        assert(buffer->usage == AVEN_GL_BUFFER_USAGE_DYNAMIC);
        assert(geometry->vertices.len <= buffer->vertex_cap);
        assert(geometry->indices.len <= buffer->index_cap);

        aven_gl_BindBuffer(gl, GL_ARRAY_BUFFER, buffer->vertex);

        aven_gl_BufferSubData(
            gl,
            GL_ARRAY_BUFFER,
            0,
            (GLsizeiptr)(
                geometry->vertices.len * sizeof(*geometry->vertices.ptr)
            ),
            geometry->vertices.ptr
        );

        aven_gl_BindBuffer(gl, GL_ARRAY_BUFFER, 0);

        aven_gl_BindBuffer(gl, GL_ELEMENT_ARRAY_BUFFER, buffer->index);

        aven_gl_BufferSubData(
            gl,
            GL_ELEMENT_ARRAY_BUFFER,
            0,
            (GLsizeiptr)(geometry->indices.len * sizeof(*geometry->indices.ptr)),
            geometry->indices.ptr
        );

        buffer->index_len = geometry->indices.len;

        aven_gl_BindBuffer(gl, GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    static inline void aven_gl_shape_rounded_draw(
        AvenGl *gl,
        AvenGlShapeRoundedCtx *ctx,
        AvenGlShapeRoundedBuffer *buffer,
        float pixel_size,
        Aff2 cam_trans
    ) {
        aven_gl_UseProgram(gl, ctx->program);

        aven_gl_BindVertexArray(gl, buffer->vao);
        aven_gl_BindBuffer(gl, GL_ARRAY_BUFFER, buffer->vertex);
        aven_gl_BindBuffer(gl, GL_ELEMENT_ARRAY_BUFFER, buffer->index);

        aven_gl_Enable(gl, GL_BLEND);
        aven_gl_BlendFunc(gl, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        aven_gl_UniformMatrix2fv(
            gl,
            (GLint)ctx->utrans_location,
            1,
            GL_FALSE,
            (GLfloat *)cam_trans
        );
        aven_gl_Uniform2fv(
            gl,
            (GLint)ctx->upos_location,
            1,
            (GLfloat *)cam_trans[2]
        );
        aven_gl_Uniform1fv(
            gl,
            (GLint)ctx->upx_location,
            1,
            (GLfloat *)&pixel_size
        );

        aven_gl_DrawElements(
            gl,
            GL_TRIANGLES,
            (GLsizei)buffer->index_len,
            GL_UNSIGNED_SHORT,
            0
        );

        aven_gl_Disable(gl, GL_BLEND);

        aven_gl_BindBuffer(gl, GL_ELEMENT_ARRAY_BUFFER, 0);
        aven_gl_BindBuffer(gl, GL_ARRAY_BUFFER, 0);
        aven_gl_BindVertexArray(gl, 0);
    }

    static inline void aven_gl_shape_rounded_geometry_push_sector(
        AvenGlShapeRoundedGeometry *geometry,
        Aff2 trans,
        float start_angle,
        float stop_angle,
        Vec4 color
    ) {
        assert((stop_angle - start_angle) < AVEN_MATH_PI_F);

        float angle = stop_angle - start_angle;

        Vec2 tex_points[3];
        vec2_copy(tex_points[0], (Vec2){ 0.0f, 0.0f });
        vec2_copy(tex_points[1], (Vec2){ 1.0f, 0.0f });
        vec2_copy(tex_points[2], (Vec2){ cosf(angle), sinf(angle) });

        Vec2 height_vec;
        mat2_mul_vec2(height_vec, trans, (Vec2){ 0.0f, tex_points[2][1] });

        Vec2 width_vec;
        mat2_mul_vec2(width_vec, trans, (Vec2){ 1.0f, 0.0f });

        float wscale = 1.0f / (2.0f * vec2_mag(width_vec));
        float hscale = 1.0f / (2.0f * vec2_mag(height_vec));

        Vec2 midpoint;
        vec2_add(midpoint, tex_points[1], tex_points[2]);
        vec2_scale(midpoint, 0.5f, midpoint);
        float scale = vec2_mag(midpoint);

        vec2_scale(tex_points[1], 1.0f / scale, tex_points[1]);
        vec2_scale(tex_points[2], 1.0f / scale, tex_points[2]);
        vec2_scale(midpoint, scale, midpoint);

        Vec2 points[3];

        Mat2 rot;
        mat2_identity(rot);
        mat2_rotate(rot, rot, start_angle);

        mat2_mul_vec2(points[0], rot, tex_points[0]);
        mat2_mul_vec2(points[1], rot, tex_points[1]);
        mat2_mul_vec2(points[2], rot, tex_points[2]);

        aff2_transform(points[0], trans, points[0]);
        aff2_transform(points[1], trans, points[1]);
        aff2_transform(points[2], trans, points[2]);

        size_t start_index = geometry->vertices.len;

        for (size_t i = 0; i < countof(points); i += 1) {
            list_push(geometry->vertices) = (AvenGlShapeRoundedVertex){
                .pos = { points[i][0], points[i][1] },
                .info = { tex_points[i][0], tex_points[i][1], wscale, hscale },
                .color = { color[0], color[1], color[2], color[3] },
            };
        }

        list_push(geometry->indices) = (GLushort)start_index + 0;
        list_push(geometry->indices) = (GLushort)start_index + 1;
        list_push(geometry->indices) = (GLushort)start_index + 2;
    }

    static inline void aven_gl_shape_rounded_geometry_push_triangle(
        AvenGlShapeRoundedGeometry *geometry,
        Aff2 trans,
        Vec2 p1,
        Vec2 p2,
        Vec2 p3,
        float roundness,
        Vec4 color
    ) {
        aff2_transform(p1, trans, p1);
        aff2_transform(p2, trans, p2);
        aff2_transform(p3, trans, p3);

        Vec2 mid;
        vec2_midpoint(mid, p2, p3);

        Vec2 up;
        vec2_sub(up, p1, mid);

        Vec2 base;
        vec2_sub(base, p2, p3);

        float wscale = 1.0f / vec2_mag(up);
        float hscale = 1.0f / vec2_mag(base);

        // Roundness calculation done for equilateral triangle
        float rs = 1.0f + roundness;
        float sx = AVEN_MATH_SQRT3_F / 2.0f;

        size_t start_index = geometry->vertices.len;

        list_push(geometry->vertices) = (AvenGlShapeRoundedVertex){
            .pos = { p1[0], p1[1] },
            .info = { 0.0f, rs * 1.0f, hscale, wscale },
            .color = { color[0], color[1], color[2], color[3] },
        };
        list_push(geometry->vertices) = (AvenGlShapeRoundedVertex){
            .pos = { p2[0], p2[1] },
            .info = { rs * -sx, rs * -0.5f, hscale, wscale },
            .color = { color[0], color[1], color[2], color[3] },
        };
        list_push(geometry->vertices) = (AvenGlShapeRoundedVertex){
            .pos = { p3[0], p3[1] },
            .info = { rs * sx, rs * -0.5f, hscale, wscale },
            .color = { color[0], color[1], color[2], color[3] },
        };

        list_push(geometry->indices) = (GLushort)start_index + 0;
        list_push(geometry->indices) = (GLushort)start_index + 1;
        list_push(geometry->indices) = (GLushort)start_index + 2;
    }

    static inline void aven_gl_shape_rounded_geometry_push_triangle_isoceles(
        AvenGlShapeRoundedGeometry *geometry,
        Aff2 trans,
        float roundness,
        Vec4 color
    ) {
        Vec2 p1 = { 0.0f, 1.0f };
        Vec2 p2 = { 1.0f, -1.0f };
        Vec2 p3 = { -1.0f, -1.0f };

        aven_gl_shape_rounded_geometry_push_triangle(
            geometry,
            trans,
            p1,
            p2,
            p3,
            roundness,
            color
        );
    }

    static inline void aven_gl_shape_rounded_geometry_push_triangle_right(
        AvenGlShapeRoundedGeometry *geometry,
        Aff2 trans,
        float roundness,
        Vec4 color
    ) {
        Vec2 p1 = { -1.0f, 1.0f };
        Vec2 p2 = { 1.0f, -1.0f };
        Vec2 p3 = { -1.0f, -1.0f };

        aven_gl_shape_rounded_geometry_push_triangle(
            geometry,
            trans,
            p1,
            p2,
            p3,
            roundness,
            color
        );
    }

    static inline void aven_gl_shape_rounded_geometry_push_square(
        AvenGlShapeRoundedGeometry *geometry,
        Aff2 trans,
        float roundness,
        Vec4 color
    ) {
        Vec2 p1 = { -1.0f, -1.0f };
        Vec2 p2 = { 1.0f, -1.0f };
        Vec2 p3 = { 1.0f, 1.0f };
        Vec2 p4 = { -1.0f, 1.0f };

        aff2_transform(p1, trans, p1);
        aff2_transform(p2, trans, p2);
        aff2_transform(p3, trans, p3);
        aff2_transform(p4, trans, p4);

        float rs = (1.0f / AVEN_MATH_SQRT2_F) +
            roundness * (1.0f - (1.0f / AVEN_MATH_SQRT2_F));

        Vec2 p1p2;
        vec2_sub(p1p2, p2, p1);

        Vec2 p1p4;
        vec2_sub(p1p4, p4, p1);

        float wscale = 1.0f / vec2_mag(p1p2);
        float hscale = 1.0f / vec2_mag(p1p4);

        size_t start_index = geometry->vertices.len;

        list_push(geometry->vertices) = (AvenGlShapeRoundedVertex){
            .pos = { p1[0], p1[1] },
            .info = { -1.0f * rs, -1.0f * rs, wscale, hscale },
            .color = { color[0], color[1], color[2], color[3] },
        };
        list_push(geometry->vertices) = (AvenGlShapeRoundedVertex){
            .pos = { p2[0], p2[1] },
            .info = { 1.0f * rs, -1.0f * rs, wscale, hscale },
            .color = { color[0], color[1], color[2], color[3] },
        };
        list_push(geometry->vertices) = (AvenGlShapeRoundedVertex){
            .pos = { p3[0], p3[1] },
            .info = { 1.0f * rs, 1.0f * rs, wscale, hscale },
            .color = { color[0], color[1], color[2], color[3] },
        };
        list_push(geometry->vertices) = (AvenGlShapeRoundedVertex){
            .pos = { p4[0], p4[1] },
            .info = { -1.0f * rs, 1.0f * rs, wscale, hscale },
            .color = { color[0], color[1], color[2], color[3] },
        };

        list_push(geometry->indices) = (GLushort)start_index + 0;
        list_push(geometry->indices) = (GLushort)start_index + 1;
        list_push(geometry->indices) = (GLushort)start_index + 2;
        list_push(geometry->indices) = (GLushort)start_index + 0;
        list_push(geometry->indices) = (GLushort)start_index + 2;
        list_push(geometry->indices) = (GLushort)start_index + 3;
    }

    static inline void aven_gl_shape_rounded_geometry_push_square_half(
        AvenGlShapeRoundedGeometry *geometry,
        Aff2 trans,
        float roundness,
        Vec4 color
    ) {
        Vec2 p1 = { -1.0f, -1.0f };
        Vec2 p2 = { 1.0f, -1.0f };
        Vec2 p3 = { 1.0f, 1.0f };
        Vec2 p4 = { -1.0f, 1.0f };

        aff2_transform(p1, trans, p1);
        aff2_transform(p2, trans, p2);
        aff2_transform(p3, trans, p3);
        aff2_transform(p4, trans, p4);

        Vec2 p1p2;
        vec2_sub(p1p2, p2, p1);

        Vec2 p1p4;
        vec2_sub(p1p4, p4, p1);

        float wscale = 1.0f / vec2_mag(p1p2);
        float hscale = 1.0f / (2.0f * vec2_mag(p1p4));

        float rs = (1.0f / AVEN_MATH_SQRT2_F) +
            roundness * (1.0f - (1.0f / AVEN_MATH_SQRT2_F));

        size_t start_index = geometry->vertices.len;

        list_push(geometry->vertices) = (AvenGlShapeRoundedVertex){
            .pos = { p1[0], p1[1] },
            .info = { -1.0f * rs, 0.0f, wscale, hscale },
            .color = { color[0], color[1], color[2], color[3] },
        };
        list_push(geometry->vertices) = (AvenGlShapeRoundedVertex){
            .pos = { p2[0], p2[1] },
            .info = { 1.0f * rs, 0.0f, wscale, hscale },
            .color = { color[0], color[1], color[2], color[3] },
        };
        list_push(geometry->vertices) = (AvenGlShapeRoundedVertex){
            .pos = { p3[0], p3[1] },
            .info = { 1.0f * rs, 1.0f * rs, wscale, hscale },
            .color = { color[0], color[1], color[2], color[3] },
        };
        list_push(geometry->vertices) = (AvenGlShapeRoundedVertex){
            .pos = { p4[0], p4[1] },
            .info = { -1.0f * rs, 1.0f * rs, wscale, hscale },
            .color = { color[0], color[1], color[2], color[3] },
        };

        list_push(geometry->indices) = (GLushort)start_index + 0;
        list_push(geometry->indices) = (GLushort)start_index + 1;
        list_push(geometry->indices) = (GLushort)start_index + 2;
        list_push(geometry->indices) = (GLushort)start_index + 0;
        list_push(geometry->indices) = (GLushort)start_index + 2;
        list_push(geometry->indices) = (GLushort)start_index + 3;
    }

    // Join

    typedef struct {
        Vec4 color;
        Vec4 texture;
        Vec4 info;
        Vec2 pos;
    } AvenGlShapeJoinVertex;

    typedef struct {
        List(AvenGlShapeJoinVertex) vertices;
        List(GLushort) indices;
    } AvenGlShapeJoinGeometry;

    typedef struct {
        size_t vertex_cap;
        size_t index_cap;
        size_t index_len;
        GLuint vertex;
        GLuint index;
        GLuint vao;
        AvenGlBufferUsage usage;
    } AvenGlShapeJoinBuffer;

    typedef struct {
        GLuint vertex_shader;
        GLuint fragment_shader;
        GLuint program;
        GLuint utrans_location;
        GLuint upos_location;
        GLuint upx_location;
        GLuint vpos_location;
        GLuint vtex_location;
        GLuint vinfo_location;
        GLuint vcolor_location;
    } AvenGlShapeJoinCtx;

    static inline AvenGlShapeJoinGeometry aven_gl_shape_join_geometry_init(
        size_t max_lines,
        AvenArena *arena
    ) {
        AvenGlShapeJoinGeometry geometry = {
            .vertices = { .cap = 4 * max_lines },
            .indices = { .cap = 6 * max_lines },
        };
        geometry.vertices.ptr = aven_arena_create_array(
            AvenGlShapeJoinVertex,
            arena,
            geometry.vertices.cap
        );
        geometry.indices.ptr = aven_arena_create_array(
            GLushort,
            arena,
            geometry.indices.cap
        );

        return geometry;
    }

    static inline void aven_gl_shape_join_geometry_clear(
        AvenGlShapeJoinGeometry *geometry
    ) {
        geometry->vertices.len = 0;
        geometry->indices.len = 0;
    }

    static inline void aven_gl_shape_join_geometry_deinit(
        AvenGlShapeJoinGeometry *geometry
    ) {
        *geometry = (AvenGlShapeJoinGeometry){ 0 };
    }

    static inline AvenGlShapeJoinCtx aven_gl_shape_join_ctx_init(AvenGl *gl) {
        AvenGlShapeJoinCtx ctx = { 0 };

        const char *vertex_shader_text = aven_gl_shader(
            gl,
            "precision mediump float;\n"
            "uniform mat2 uTrans;\n"
            "uniform vec2 uPos;\n"
            "uniform float uPx;\n"
            "in vec4 vTex;\n"
            "in vec4 vInfo;\n"
            "in vec4 vColor;\n"
            "in vec2 vPos;\n"
            "out vec4 tPos;\n"
            "out vec2 tOffset;\n"
            "out vec2 tFocus;\n"
            "out vec4 fColor;\n"
            "void main() {\n"
            "    gl_Position = vec4((uTrans * vPos) + uPos, 0.0, 1.0);\n"
            "    tPos = vTex;\n"
            "    tOffset = uPx * vInfo.zw;\n"
            "    fColor = vColor;\n"
            "    tFocus = vInfo.xy;\n"
            "}\n"
        );

        const char *fragment_shader_text = aven_gl_shader(
            gl,
            "precision mediump float;\n"
            "in vec4 tPos;\n"
            "in vec2 tOffset;\n"
            "in vec2 tFocus;\n"
            "in vec4 fColor;\n"
            "out vec4 FragColor;\n"
            "float msaa(vec2 p) {\n"
            "    if (p.y >= 0.0) {\n"
            "        float h = tFocus.x * p.x * p.x + 0.5 - tFocus.x;\n"
            "        if (p.y > h) {\n"
            "            return 0.0;\n"
            "        }\n"
            "    } else {\n"
            "        float h = tFocus.y * p.x * p.x - 0.5 - tFocus.y;\n"
            "        if (p.y < h) {\n"
            "            return 0.0;\n"
            "        }\n"
            "    }\n"
            "    return 1.0;\n"
            "}\n"
            "void main() {\n"
            "    vec2 pos = vec2(tPos.x / tPos.w, tPos.y / tPos.w);\n"
            "    float magnitude = 0.0;\n"
            "    magnitude += msaa(pos);\n"
            "    magnitude += msaa(pos + vec2(0, -tOffset.y));\n"
            "    magnitude += msaa(pos + vec2(tOffset.x, 0));\n"
            "    magnitude += msaa(pos + vec2(0, tOffset.y));\n"
            "    magnitude += msaa(pos + vec2(-tOffset.x, 0));\n"
            "    magnitude += msaa(pos + vec2(-tOffset.x, tOffset.y));\n"
            "    magnitude += msaa(pos + vec2(-tOffset.x, -tOffset.y));\n"
            "    magnitude += msaa(pos + vec2(tOffset.x, -tOffset.y));\n"
            "    magnitude += msaa(pos + vec2(tOffset.x, tOffset.y));\n"
            "    magnitude /= 9.0;\n"
            "    FragColor = vec4(fColor.xyz, fColor.w * magnitude);\n"
            "}\n"
        );

        ctx.vertex_shader = aven_gl_CreateShader(gl, GL_VERTEX_SHADER);
        aven_gl_ShaderSource(
            gl,
            ctx.vertex_shader,
            1,
            &vertex_shader_text,
            NULL
        );
        aven_gl_CompileShader(gl, ctx.vertex_shader);

        ctx.fragment_shader = aven_gl_CreateShader(gl, GL_FRAGMENT_SHADER);
        aven_gl_ShaderSource(
            gl,
            ctx.fragment_shader,
            1,
            &fragment_shader_text,
            NULL
        );
        aven_gl_CompileShader(gl, ctx.fragment_shader);

        ctx.program = aven_gl_CreateProgram(gl);
        aven_gl_AttachShader(gl, ctx.program, ctx.vertex_shader);
        aven_gl_AttachShader(gl, ctx.program, ctx.fragment_shader);
        aven_gl_LinkProgram(gl, ctx.program);

        ctx.utrans_location = (GLuint)aven_gl_GetUniformLocation(
            gl,
            ctx.program,
            "uTrans"
        );
        ctx.upos_location = (GLuint)aven_gl_GetUniformLocation(
            gl,
            ctx.program,
            "uPos"
        );
        ctx.upx_location = (GLuint)aven_gl_GetUniformLocation(
            gl,
            ctx.program,
            "uPx"
        );

        ctx.vpos_location = (GLuint)aven_gl_GetAttribLocation(
            gl,
            ctx.program,
            "vPos"
        );
        ctx.vtex_location = (GLuint)aven_gl_GetAttribLocation(
            gl,
            ctx.program,
            "vTex"
        );
        ctx.vinfo_location = (GLuint)aven_gl_GetAttribLocation(
            gl,
            ctx.program,
            "vInfo"
        );
        ctx.vcolor_location = (GLuint)aven_gl_GetAttribLocation(
            gl,
            ctx.program,
            "vColor"
        );

        return ctx;
    }

    static inline void aven_gl_shape_join_ctx_deinit(
        AvenGl *gl,
        AvenGlShapeJoinCtx *ctx
    ) {
        aven_gl_DeleteProgram(gl, ctx->program);
        aven_gl_DeleteShader(gl, ctx->fragment_shader);
        aven_gl_DeleteShader(gl, ctx->vertex_shader);
        *ctx = (AvenGlShapeJoinCtx){ 0 };
    }

    static inline AvenGlShapeJoinBuffer aven_gl_shape_join_buffer_init(
        AvenGl *gl,
        AvenGlShapeJoinCtx *ctx,
        AvenGlShapeJoinGeometry *geometry,
        AvenGlBufferUsage buffer_usage
    ) {
        AvenGlShapeJoinBuffer buffer = { .usage = buffer_usage };

        switch (buffer_usage) {
            case AVEN_GL_BUFFER_USAGE_DYNAMIC:
                buffer.vertex_cap = geometry->vertices.cap;
                buffer.index_cap = geometry->indices.cap;
                break;
            case AVEN_GL_BUFFER_USAGE_STATIC:
            case AVEN_GL_BUFFER_USAGE_STREAM:
                buffer.vertex_cap = geometry->vertices.len;
                buffer.index_cap = geometry->indices.len;
                buffer.index_len = buffer.index_len;
                break;
            default:
                assert(false);
        }

        aven_gl_GenVertexArrays(gl, 1, &buffer.vao);
        aven_gl_BindVertexArray(gl, buffer.vao);

        aven_gl_GenBuffers(gl, 1, &buffer.vertex);
        aven_gl_BindBuffer(gl, GL_ARRAY_BUFFER, buffer.vertex);
        aven_gl_BufferData(
            gl,
            GL_ARRAY_BUFFER,
            (GLsizeiptr)(buffer.vertex_cap * sizeof(*geometry->vertices.ptr)),
            geometry->vertices.ptr,
            (GLenum)buffer_usage
        );

        aven_gl_VertexAttribPointer(
            gl,
            ctx->vpos_location,
            2,
            GL_FLOAT,
            GL_FALSE,
            sizeof(AvenGlShapeJoinVertex),
            (void *)offsetof(AvenGlShapeJoinVertex, pos)
        );
        aven_gl_EnableVertexAttribArray(gl, ctx->vpos_location);
        aven_gl_VertexAttribPointer(
            gl,
            ctx->vtex_location,
            4,
            GL_FLOAT,
            GL_FALSE,
            sizeof(AvenGlShapeJoinVertex),
            (void *)offsetof(AvenGlShapeJoinVertex, texture)
        );
        aven_gl_EnableVertexAttribArray(gl, ctx->vtex_location);
        aven_gl_VertexAttribPointer(
            gl,
            ctx->vinfo_location,
            4,
            GL_FLOAT,
            GL_FALSE,
            sizeof(AvenGlShapeJoinVertex),
            (void *)offsetof(AvenGlShapeJoinVertex, info)
        );
        aven_gl_EnableVertexAttribArray(gl, ctx->vinfo_location);
        aven_gl_VertexAttribPointer(
            gl,
            ctx->vcolor_location,
            4,
            GL_FLOAT,
            GL_FALSE,
            sizeof(AvenGlShapeJoinVertex),
            (void *)offsetof(AvenGlShapeJoinVertex, color)
        );
        aven_gl_EnableVertexAttribArray(gl, ctx->vcolor_location);

        aven_gl_BindBuffer(gl, GL_ARRAY_BUFFER, 0);

        aven_gl_BindVertexArray(gl, 0);

        aven_gl_GenBuffers(gl, 1, &buffer.index);
        aven_gl_BindBuffer(gl, GL_ELEMENT_ARRAY_BUFFER, buffer.index);
        aven_gl_BufferData(
            gl,
            GL_ELEMENT_ARRAY_BUFFER,
            (GLsizeiptr)(buffer.index_cap * sizeof(*geometry->indices.ptr)),
            geometry->indices.ptr,
            (GLenum)buffer_usage
        );

        aven_gl_BindBuffer(gl, GL_ELEMENT_ARRAY_BUFFER, 0);

        return buffer;
    }

    static inline void aven_gl_shape_join_buffer_deinit(
        AvenGl *gl,
        AvenGlShapeJoinBuffer *buffer
    ) {
        aven_gl_DeleteBuffers(gl, 1, &buffer->index);
        aven_gl_DeleteVertexArrays(gl, 1, &buffer->vao);
        aven_gl_DeleteBuffers(gl, 1, &buffer->vertex);
        *buffer = (AvenGlShapeJoinBuffer){ 0 };
    }

    static inline void aven_gl_shape_join_buffer_update(
        AvenGl *gl,
        AvenGlShapeJoinBuffer *buffer,
        AvenGlShapeJoinGeometry *geometry
    ) {
        assert(buffer->usage == AVEN_GL_BUFFER_USAGE_DYNAMIC);
        assert(geometry->vertices.len <= buffer->vertex_cap);
        assert(geometry->indices.len <= buffer->index_cap);

        aven_gl_BindBuffer(gl, GL_ARRAY_BUFFER, buffer->vertex);

        aven_gl_BufferSubData(
            gl,
            GL_ARRAY_BUFFER,
            0,
            (GLsizeiptr)(
                geometry->vertices.len * sizeof(*geometry->vertices.ptr)
            ),
            geometry->vertices.ptr
        );

        aven_gl_BindBuffer(gl, GL_ARRAY_BUFFER, 0);

        aven_gl_BindBuffer(gl, GL_ELEMENT_ARRAY_BUFFER, buffer->index);

        aven_gl_BufferSubData(
            gl,
            GL_ELEMENT_ARRAY_BUFFER,
            0,
            (GLsizeiptr)(geometry->indices.len * sizeof(*geometry->indices.ptr)),
            geometry->indices.ptr
        );

        buffer->index_len = geometry->indices.len;

        aven_gl_BindBuffer(gl, GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    static inline void aven_gl_shape_join_draw(
        AvenGl *gl,
        AvenGlShapeJoinCtx *ctx,
        AvenGlShapeJoinBuffer *buffer,
        float pixel_size,
        Aff2 cam_trans
    ) {
        aven_gl_UseProgram(gl, ctx->program);

        aven_gl_BindVertexArray(gl, buffer->vao);
        aven_gl_BindBuffer(gl, GL_ARRAY_BUFFER, buffer->vertex);
        aven_gl_BindBuffer(gl, GL_ELEMENT_ARRAY_BUFFER, buffer->index);

        aven_gl_Enable(gl, GL_BLEND);
        aven_gl_BlendFunc(gl, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        aven_gl_UniformMatrix2fv(
            gl,
            (GLint)ctx->utrans_location,
            1,
            GL_FALSE,
            (GLfloat *)cam_trans
        );
        aven_gl_Uniform2fv(
            gl,
            (GLint)ctx->upos_location,
            1,
            (GLfloat *)cam_trans[2]
        );
        aven_gl_Uniform1fv(
            gl,
            (GLint)ctx->upx_location,
            1,
            (GLfloat *)&pixel_size
        );

        aven_gl_DrawElements(
            gl,
            GL_TRIANGLES,
            (GLsizei)buffer->index_len,
            GL_UNSIGNED_SHORT,
            0
        );

        aven_gl_Disable(gl, GL_BLEND);

        aven_gl_BindBuffer(gl, GL_ELEMENT_ARRAY_BUFFER, 0);
        aven_gl_BindBuffer(gl, GL_ARRAY_BUFFER, 0);
        aven_gl_BindVertexArray(gl, 0);
    }

    static inline void aven_gl_shape_join_geometry_push_square(
        AvenGlShapeJoinGeometry *geometry,
        Aff2 trans,
        Vec2 end_scale,
        Vec2 roundness,
        Vec4 color
    ) {
        Vec2 p1 = { -1.0f, -2.0f * end_scale[0] };
        Vec2 p2 = { 1.0f, -2.0f * end_scale[1] };
        Vec2 p3 = { 1.0f, 2.0f * end_scale[1] };
        Vec2 p4 = { -1.0f, 2.0f * end_scale[0] };

        aff2_transform(p1, trans, p1);
        aff2_transform(p2, trans, p2);
        aff2_transform(p3, trans, p3);
        aff2_transform(p4, trans, p4);

        Vec2 p1p2;
        vec2_sub(p1p2, p2, p1);

        Vec2 p1p4;
        vec2_sub(p1p4, p4, p1);

        Vec2 p2p3;
        vec2_sub(p2p3, p3, p2);

        float wscale = 1.0f / vec2_mag(p1p2);
        float hscale1 = 1.0f / vec2_mag(p1p4);
        float hscale2 = 1.0f / vec2_mag(p2p3);

        Vec2 focus = { -0.5f * roundness[0], 0.5f * roundness[1] };

        size_t start_index = geometry->vertices.len;

        list_push(geometry->vertices) = (AvenGlShapeJoinVertex){
            .pos = { p1[0], p1[1] },
            .info = { focus[0], focus[1], wscale, hscale1 },
            .texture = { -end_scale[0], -end_scale[0], 0.0f, end_scale[0] },
            .color = { color[0], color[1], color[2], color[3] },
        };
        list_push(geometry->vertices) = (AvenGlShapeJoinVertex){
            .pos = { p2[0], p2[1] },
            .info = { focus[0], focus[1], wscale, hscale2 },
            .texture = { end_scale[1], -end_scale[1], 0.0f, end_scale[1] },
            .color = { color[0], color[1], color[2], color[3] },
        };
        list_push(geometry->vertices) = (AvenGlShapeJoinVertex){
            .pos = { p3[0], p3[1] },
            .info = { focus[0], focus[1], wscale, hscale2 },
            .texture = { end_scale[1], end_scale[1], 0.0f, end_scale[1] },
            .color = { color[0], color[1], color[2], color[3] },
        };
        list_push(geometry->vertices) = (AvenGlShapeJoinVertex){
            .pos = { p4[0], p4[1] },
            .info = { focus[0], focus[1], wscale, hscale1 },
            .texture = { -end_scale[0], end_scale[0], 0.0f, end_scale[0] },
            .color = { color[0], color[1], color[2], color[3] },
        };

        list_push(geometry->indices) = (GLushort)start_index + 0;
        list_push(geometry->indices) = (GLushort)start_index + 1;
        list_push(geometry->indices) = (GLushort)start_index + 2;
        list_push(geometry->indices) = (GLushort)start_index + 0;
        list_push(geometry->indices) = (GLushort)start_index + 2;
        list_push(geometry->indices) = (GLushort)start_index + 3;
    }
#endif // AVEN_GL_SHAPE_H
