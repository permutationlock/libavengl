#ifndef AVEN_GL_SHADOW_H
    #define AVEN_GL_SHADOW_H

    #include <aven.h>
    #include <aven/arena.h>
    #include <aven/math.h>

    #include "../gl.h"
    #include "texture.h"

    typedef struct {
        Vec4 pos;
    } AvenGlShadowHardVertex;

    typedef struct {
        List(AvenGlShadowHardVertex) vertices;
        List(GLushort) indices;
    } AvenGlShadowHardGeometry;

    typedef struct {
        GLuint vertex_shader;
        GLuint fragment_shader;
        GLuint program;
        GLuint utrans_location;
        GLuint upos_location;
        GLuint vpos_location;
        GLuint vcolor_location;
    } AvenGlShadowHardCtx;

    typedef struct {
        size_t vertex_cap;
        size_t index_cap;
        size_t index_len;
        GLuint vertex;
        GLuint index;
        GLuint vao;
        AvenGlBufferUsage usage;
    } AvenGlShadowHardBuffer;

    static inline AvenGlShadowHardGeometry aven_gl_shadow_hard_geometry_init(
        size_t max_vertices,
        size_t max_indices,
        AvenArena *arena
    ) {
        assert(max_indices >= max_vertices);

        AvenGlShadowHardGeometry geometry = {
            .vertices = { .cap = max_vertices },
            .indices = { .cap = max_indices },
        };
        geometry.vertices.ptr = aven_arena_create_array(
            AvenGlShadowHardVertex,
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

    static inline void aven_gl_shadow_hard_geometry_clear(
        AvenGlShadowHardGeometry *geometry
    ) {
        geometry->vertices.len = 0;
        geometry->indices.len = 0;
    }

    static inline void aven_gl_shadow_hard_geometry_deinit(
        AvenGlShadowHardGeometry *geometry
    ) {
        *geometry = (AvenGlShadowHardGeometry){ 0 };
    }

    static inline AvenGlShadowHardCtx aven_gl_shadow_hard_ctx_init(AvenGl *gl) {
        AvenGlShadowHardCtx ctx = { 0 };

        const char *vertex_shader_text = aven_gl_shader(
            gl,
            "precision mediump float;\n"
            "in vec4 vPos;\n"
            "uniform mat2 uTrans;\n"
            "uniform vec2 uPos;\n"
            "void main() {\n"
            "    gl_Position = vec4((uTrans * vPos.xy) + uPos, 0.0, vPos.w);\n"
            "}\n"
        );

        const char *fragment_shader_text = aven_gl_shader(
            gl,
            "precision mediump float;\n"
            "out vec4 FragColor;\n"
            "void main() {\n"
            "    FragColor = vec4(0.0, 0.0, 0.0, 1.0);\n"
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

    static inline void aven_gl_shadow_hard_ctx_deinit(
        AvenGl *gl,
        AvenGlShadowHardCtx *ctx
    ) {
        aven_gl_DeleteProgram(gl, ctx->program);
        aven_gl_DeleteShader(gl, ctx->fragment_shader);
        aven_gl_DeleteShader(gl, ctx->vertex_shader);
        *ctx = (AvenGlShadowHardCtx){ 0 };
    }

    static inline AvenGlShadowHardBuffer aven_gl_shadow_hard_buffer_init(
        AvenGl *gl,
        AvenGlShadowHardCtx *ctx,
        AvenGlShadowHardGeometry *geometry,
        AvenGlBufferUsage buffer_usage
    ) {
        AvenGlShadowHardBuffer buffer = { .usage = buffer_usage };

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
            4,
            GL_FLOAT,
            GL_FALSE,
            sizeof(AvenGlShadowHardVertex),
            (void *)offsetof(AvenGlShadowHardVertex, pos)
        );
        aven_gl_EnableVertexAttribArray(gl, ctx->vpos_location);

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

    static inline void aven_gl_shadow_hard_buffer_deinit(
        AvenGl *gl,
        AvenGlShadowHardBuffer *buffer
    ) {
        aven_gl_DeleteBuffers(gl, 1, &buffer->index);
        aven_gl_DeleteVertexArrays(gl, 1, &buffer->vao);
        aven_gl_DeleteBuffers(gl, 1, &buffer->vertex);
        *buffer = (AvenGlShadowHardBuffer){ 0 };
    }

    static inline void aven_gl_shadow_hard_buffer_update(
        AvenGl *gl,
        AvenGlShadowHardBuffer *buffer,
        AvenGlShadowHardGeometry *geometry
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

    static inline void aven_gl_shadow_hard_draw(
        AvenGl *gl,
        AvenGlShadowHardCtx *ctx,
        AvenGlShadowHardBuffer *buffer,
        Aff2 cam_trans
    ) {
        aven_gl_UseProgram(gl, ctx->program);

        aven_gl_BindVertexArray(gl, buffer->vao);
        aven_gl_BindBuffer(gl, GL_ARRAY_BUFFER, buffer->vertex);
        aven_gl_BindBuffer(gl, GL_ELEMENT_ARRAY_BUFFER, buffer->index);

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

        aven_gl_BindBuffer(gl, GL_ELEMENT_ARRAY_BUFFER, 0);
        aven_gl_BindBuffer(gl, GL_ARRAY_BUFFER, 0);
        aven_gl_BindVertexArray(gl, 0);
    }

    static inline void aven_gl_shadow_hard_geometry_shade(
        AvenGlShadowHardGeometry *geometry,
        Vec2 src,
        Vec2 p1,
        Vec2 p2
    ) {
        Vec2 vp1;
        vec2_sub(vp1, p1, src);

        Vec2 p1p2;
        vec2_sub(p1p2, p2, p1);

        Vec2 n = { p1p2[1], -p1p2[0] };

        if (vec2_dot(vp1, n) < 0.0f) {
            return;
        }

        Vec2 vp2;
        vec2_sub(vp2, p2, src);

        size_t start_index = geometry->vertices.len;

        list_push(geometry->vertices) = (AvenGlShadowHardVertex){
            .pos = { p1[0], p1[1], 0.0f, 1.0f },
        };
        list_push(geometry->vertices) = (AvenGlShadowHardVertex){
            .pos = { p2[0], p2[1], 0.0f, 1.0f },
        };
        list_push(geometry->vertices) = (AvenGlShadowHardVertex){
            .pos = { vp2[0], vp2[1], 0.0f, 0.0f },
        };
        list_push(geometry->vertices) = (AvenGlShadowHardVertex){
            .pos = { vp1[0], vp1[1], 0.0f, 0.0f },
        };

        list_push(geometry->indices) = (GLushort)start_index + 0;
        list_push(geometry->indices) = (GLushort)start_index + 1;
        list_push(geometry->indices) = (GLushort)start_index + 2;
        list_push(geometry->indices) = (GLushort)start_index + 0;
        list_push(geometry->indices) = (GLushort)start_index + 2;
        list_push(geometry->indices) = (GLushort)start_index + 3;
    }

    static inline void aven_gl_shadow_hard_geometry_push_square(
        AvenGlShadowHardGeometry *geometry,
        Vec2 src,
        Aff2 trans
    ) {
        Vec2 pts[4] = {
            { -1.0f, -1.0f },
            { 1.0f, -1.0f },
            { 1.0f, 1.0f },
            { -1.0f, 1.0f },
        };
        Vec2 norms[4] = {
            { 0.0f, -1.0f },
            { 1.0f, 0.0f },
            { 0.0f, 1.0f },
            { -1.0f, 0.0f },
        };

        for (size_t i = 0; i < countof(pts); i += 1) {
            aff2_transform(pts[i], trans, pts[i]);
            mat2_mul_vec2(norms[i], trans, norms[i]);
        }

        for (size_t i = 0; i < countof(pts); i += 1) {
            size_t j = (i + 1) % countof(pts);

            Vec2 dir;
            vec2_sub(dir, pts[i], src);

            if (vec2_dot(dir, norms[i]) >= 0.0f) {
                aven_gl_shadow_hard_geometry_shade(
                    geometry,
                    src,
                    pts[i],
                    pts[j]
                );
            }
        }
    }

    static inline void aven_gl_shadow_hard_geometry_push_triangle(
        AvenGlShadowHardGeometry *geometry,
        Vec2 src,
        Aff2 trans,
        Vec2 p1,
        Vec2 p2,
        Vec2 p3
    ) {
        Vec2 pts[3] = { { p1[0], p1[1] }, { p2[0], p2[1] }, { p3[0], p3[1] } };
        for (size_t i = 0; i < countof(pts); i += 1) {
            aff2_transform(pts[i], trans, pts[i]);
        }

        for (size_t i = 0; i < countof(pts); i += 1) {
            size_t j = (i + 1) % countof(pts);

            Vec2 dir;
            vec2_sub(dir, src, pts[i]);

            Vec2 pipj;
            vec2_sub(pipj, pts[j], pts[i]);

            Vec2 norm = { pipj[1], -pipj[0] };

            if (vec2_dot(dir, norm) >= 0.0f) {
                aven_gl_shadow_hard_geometry_shade(
                    geometry,
                    src,
                    pts[j],
                    pts[i]
                );
            }
        }
    }

    static inline void aven_gl_shadow_hard_geometry_push_triangle_isoceles(
        AvenGlShadowHardGeometry *geometry,
        Vec2 src,
        Aff2 trans
    ) {
        Vec2 p1 = { 0.0f, 1.0f };
        Vec2 p2 = { 1.0f, -1.0f };
        Vec2 p3 = { -1.0f, -1.0f };

        aven_gl_shadow_hard_geometry_push_triangle(
            geometry,
            src,
            trans,
            p1,
            p2,
            p3
        );
    }

    static inline void aven_gl_shadow_hard_geometry_push_triangle_right(
        AvenGlShadowHardGeometry *geometry,
        Vec2 src,
        Aff2 trans
    ) {
        Vec2 p1 = { -1.0f, 1.0f };
        Vec2 p2 = { 1.0f, -1.0f };
        Vec2 p3 = { -1.0f, -1.0f };

        aven_gl_shadow_hard_geometry_push_triangle(
            geometry,
            src,
            trans,
            p1,
            p2,
            p3
        );
    }
#endif // AVEN_GL_SHADOW_H
