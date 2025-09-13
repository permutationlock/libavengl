#ifndef AVEN_GL_TEXTURE_H
    #define AVEN_GL_TEXTURE_H

    #include <aven.h>
    #include <aven/arena.h>
    #include <aven/str.h>
    #include <aven/math.h>

    #include "../gl.h"

    typedef struct {
        GLuint texture_id;
        GLuint vertex_shader;
        GLuint fragment_shader;
        GLuint program;
        GLuint utrans_location;
        GLuint upos_location;
        GLuint vpos_location;
    } AvenGlTextureCtx;

    typedef struct {
        Vec4 pos;
    } AvenGlTextureVertex;

    typedef struct {
        size_t vertex_cap;
        size_t index_cap;
        size_t index_len;
        GLuint vertex;
        GLuint index;
        GLuint vao;
        AvenGlBufferUsage usage;
    } AvenGlTextureBuffer;

    typedef struct {
        List(AvenGlTextureVertex) vertices;
        List(GLushort) indices;
    } AvenGlTextureGeometry;

    typedef Optional(ByteSlice) AvenGlTextureBytesOptional;

    static inline AvenGlTextureCtx aven_gl_texture_ctx_init(
        AvenGl *gl,
        size_t width,
        size_t height,
        AvenGlTextureBytesOptional maybe_bytes
    ) {
        if (maybe_bytes.valid) {
            assert((width * height) == (maybe_bytes.value.len / 4));
        }

        AvenGlTextureCtx ctx = { 0 };

        const char *vertex_shader_text = aven_gl_shader(
            gl,
            "precision mediump float;\n"
            "uniform mat2 uTrans;\n"
            "uniform vec2 uPos;\n"
            "in vec4 vPos;\n"
            "out vec2 tCoord;\n"
            "void main() {\n"
            "    gl_Position = vec4((uTrans * vPos.xy) + uPos, 0.0, 1.0);\n"
            "    tCoord = vPos.zw;\n"
            "}\n"
        );

        const char *fragment_shader_text = aven_gl_shader(
            gl,
            "precision mediump float;\n"
            "uniform sampler2D texSampler;\n"
            "in vec2 tCoord;\n"
            "out vec4 FragColor;\n"
            "void main() {\n"
            "    FragColor = texture(texSampler, tCoord);\n"
            "}\n"
        );

        ctx.vertex_shader = gl->CreateShader(GL_VERTEX_SHADER);
        aven_gl_check_error(gl);
        gl->ShaderSource(ctx.vertex_shader, 1, &vertex_shader_text, NULL);
        aven_gl_check_error(gl);
        gl->CompileShader(ctx.vertex_shader);
        aven_gl_check_error(gl);
        aven_gl_shader_validate(gl, ctx.vertex_shader, vertex_shader_text);

        ctx.fragment_shader = gl->CreateShader(GL_FRAGMENT_SHADER);
        aven_gl_check_error(gl);
        gl->ShaderSource(ctx.fragment_shader, 1, &fragment_shader_text, NULL);
        aven_gl_check_error(gl);
        gl->CompileShader(ctx.fragment_shader);
        aven_gl_check_error(gl);
        aven_gl_shader_validate(gl, ctx.fragment_shader, fragment_shader_text);

        ctx.program = gl->CreateProgram();
        aven_gl_check_error(gl);
        gl->AttachShader(ctx.program, ctx.vertex_shader);
        aven_gl_check_error(gl);
        gl->AttachShader(ctx.program, ctx.fragment_shader);
        aven_gl_check_error(gl);
        gl->LinkProgram(ctx.program);
        aven_gl_program_validate(gl, ctx.program);

        ctx.utrans_location = (GLuint)gl->GetUniformLocation(
            ctx.program,
            "uTrans"
        );
        aven_gl_check_error(gl);
        ctx.upos_location = (GLuint)gl->GetUniformLocation(ctx.program, "uPos");
        aven_gl_check_error(gl);

        ctx.vpos_location = (GLuint)gl->GetAttribLocation(ctx.program, "vPos");
        aven_gl_check_error(gl);

        gl->GenTextures(1, &ctx.texture_id);
        aven_gl_check_error(gl);
        gl->BindTexture(GL_TEXTURE_2D, ctx.texture_id);
        aven_gl_check_error(gl);
        gl->TexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            (GLsizei)width,
            (GLsizei)height,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            maybe_bytes.valid ? maybe_bytes.value.ptr : NULL
        );
        aven_gl_check_error(gl);
        gl->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        aven_gl_check_error(gl);
        gl->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        aven_gl_check_error(gl);
        gl->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        aven_gl_check_error(gl);
        gl->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        aven_gl_check_error(gl);

        gl->BindTexture(GL_TEXTURE_2D, 0);
        aven_gl_check_error(gl);

        return ctx;
    }

    static inline void aven_gl_texture_ctx_update(
        AvenGl *gl,
        AvenGlTextureCtx *ctx,
        size_t width,
        size_t height,
        AvenGlTextureBytesOptional maybe_bytes
    ) {
        gl->BindTexture(GL_TEXTURE_2D, ctx->texture_id);
        aven_gl_check_error(gl);
        gl->TexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            (GLsizei)width,
            (GLsizei)height,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            maybe_bytes.valid ? maybe_bytes.value.ptr : NULL
        );
        aven_gl_check_error(gl);
        gl->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        aven_gl_check_error(gl);
        gl->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        aven_gl_check_error(gl);
        gl->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        aven_gl_check_error(gl);
        gl->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        aven_gl_check_error(gl);

        gl->BindTexture(GL_TEXTURE_2D, 0);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_texture_ctx_update_framebuffer(
        AvenGl *gl,
        AvenGlTextureCtx *ctx,
        size_t x,
        size_t y,
        size_t width,
        size_t height
    ) {
        gl->BindTexture(GL_TEXTURE_2D, ctx->texture_id);
        aven_gl_check_error(gl);
        gl->CopyTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            (GLsizei)x,
            (GLsizei)y,
            (GLsizei)width,
            (GLsizei)height,
            0
        );
        aven_gl_check_error(gl);
        gl->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        aven_gl_check_error(gl);
        gl->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        aven_gl_check_error(gl);
        gl->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        aven_gl_check_error(gl);
        gl->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        aven_gl_check_error(gl);

        gl->BindTexture(GL_TEXTURE_2D, 0);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_texture_ctx_deinit(
        AvenGl *gl,
        AvenGlTextureCtx *ctx
    ) {
        gl->DeleteProgram(ctx->program);
        gl->DeleteShader(ctx->fragment_shader);
        gl->DeleteShader(ctx->vertex_shader);
    }

    static inline AvenGlTextureGeometry aven_gl_texture_geometry_init(
        size_t max_quads,
        AvenArena *arena
    ) {
        AvenGlTextureGeometry geometry = {
            .vertices = { .cap = max_quads * 4 },
            .indices = { .cap = max_quads * 6 },
        };

        geometry.vertices.ptr = aven_arena_create_array(
            AvenGlTextureVertex,
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

    static inline void aven_gl_texture_geometry_deinit(
        AvenGlTextureGeometry *geometry
    ) {
        *geometry = (AvenGlTextureGeometry){ 0 };
    }

    static inline void aven_gl_texture_geometry_push_square(
        AvenGlTextureGeometry *geometry,
        Aff2 trans,
        Aff2 texture_trans
    ) {
        Vec2 p1 = { -1.0f, -1.0f };
        Vec2 p2 = { 1.0f, -1.0f };
        Vec2 p3 = { 1.0f, 1.0f };
        Vec2 p4 = { -1.0f, 1.0f };

        aff2_transform(p1, trans, p1);
        aff2_transform(p2, trans, p2);
        aff2_transform(p3, trans, p3);
        aff2_transform(p4, trans, p4);

        Vec2 t1 = { 0.0f, 0.0f };
        Vec2 t2 = { 1.0f, 0.0f };
        Vec2 t3 = { 1.0f, 1.0f };
        Vec2 t4 = { 0.0f, 1.0f };

        aff2_transform(t1, texture_trans, t1);
        aff2_transform(t2, texture_trans, t2);
        aff2_transform(t3, texture_trans, t3);
        aff2_transform(t4, texture_trans, t4);

        size_t start_index = geometry->vertices.len;

        list_push(geometry->vertices) = (AvenGlTextureVertex){
            .pos = { p1[0], p1[1], t1[0], t1[1] },
        };
        list_push(geometry->vertices) = (AvenGlTextureVertex){
            .pos = { p2[0], p2[1], t2[0], t2[1] },
        };
        list_push(geometry->vertices) = (AvenGlTextureVertex){
            .pos = { p3[0], p3[1], t3[0], t3[1] },
        };
        list_push(geometry->vertices) = (AvenGlTextureVertex){
            .pos = { p4[0], p4[1], t4[0], t4[1] },
        };

        list_push(geometry->indices) = (GLushort)start_index + 0;
        list_push(geometry->indices) = (GLushort)start_index + 1;
        list_push(geometry->indices) = (GLushort)start_index + 2;
        list_push(geometry->indices) = (GLushort)start_index + 0;
        list_push(geometry->indices) = (GLushort)start_index + 2;
        list_push(geometry->indices) = (GLushort)start_index + 3;
    }

    static inline void aven_gl_texture_geometry_clear(
        AvenGlTextureGeometry *geometry
    ) {
        geometry->vertices.len = 0;
        geometry->indices.len = 0;
    }

    static inline AvenGlTextureBuffer aven_gl_texture_buffer_init(
        AvenGl *gl,
        AvenGlTextureCtx *ctx,
        AvenGlTextureGeometry *geometry,
        AvenGlBufferUsage buffer_usage
    ) {
        AvenGlTextureBuffer buffer = { .usage = buffer_usage };

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

        gl->GenVertexArrays(1, &buffer.vao);
        aven_gl_check_error(gl);
        gl->BindVertexArray(buffer.vao);
        aven_gl_check_error(gl);

        gl->GenBuffers(1, &buffer.vertex);
        aven_gl_check_error(gl);
        gl->BindBuffer(GL_ARRAY_BUFFER, buffer.vertex);
        aven_gl_check_error(gl);

        gl->BufferData(
            GL_ARRAY_BUFFER,
            (GLsizeiptr)(buffer.vertex_cap * sizeof(*geometry->vertices.ptr)),
            geometry->vertices.ptr,
            (GLenum)buffer_usage
        );
        aven_gl_check_error(gl);

        gl->VertexAttribPointer(
            ctx->vpos_location,
            4,
            GL_FLOAT,
            GL_FALSE,
            sizeof(AvenGlTextureVertex),
            (void *)offsetof(AvenGlTextureVertex, pos)
        );
        aven_gl_check_error(gl);
        gl->EnableVertexAttribArray(ctx->vpos_location);
        aven_gl_check_error(gl);

        gl->BindBuffer(GL_ARRAY_BUFFER, 0);
        aven_gl_check_error(gl);

        gl->BindVertexArray(0);
        aven_gl_check_error(gl);

        gl->GenBuffers(1, &buffer.index);
        aven_gl_check_error(gl);
        gl->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.index);
        aven_gl_check_error(gl);
        gl->BufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            (GLsizeiptr)(buffer.index_cap * sizeof(*geometry->indices.ptr)),
            geometry->indices.ptr,
            (GLenum)buffer_usage
        );
        aven_gl_check_error(gl);

        gl->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        aven_gl_check_error(gl);

        return buffer;
    }

    static inline void aven_gl_texture_buffer_deinit(
        AvenGl *gl,
        AvenGlTextureBuffer *buffer
    ) {
        gl->DeleteBuffers(1, &buffer->index);
        gl->DeleteVertexArrays(1, &buffer->vao);
        gl->DeleteBuffers(1, &buffer->vertex);
        *buffer = (AvenGlTextureBuffer){ 0 };
    }

    static inline void aven_gl_texture_buffer_update(
        AvenGl *gl,
        AvenGlTextureBuffer *buffer,
        AvenGlTextureGeometry *geometry
    ) {
        assert(buffer->usage == AVEN_GL_BUFFER_USAGE_DYNAMIC);
        assert(geometry->vertices.len <= buffer->vertex_cap);
        assert(geometry->indices.len <= buffer->index_cap);

        gl->BindBuffer(GL_ARRAY_BUFFER, buffer->vertex);
        aven_gl_check_error(gl);

        gl->BufferSubData(
            GL_ARRAY_BUFFER,
            0,
            (GLsizeiptr)(
                geometry->vertices.len * sizeof(*geometry->vertices.ptr)
            ),
            geometry->vertices.ptr
        );
        aven_gl_check_error(gl);

        gl->BindBuffer(GL_ARRAY_BUFFER, 0);
        aven_gl_check_error(gl);

        gl->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->index);
        aven_gl_check_error(gl);
        gl->BufferSubData(
            GL_ELEMENT_ARRAY_BUFFER,
            0,
            (GLsizeiptr)(geometry->indices.len * sizeof(*geometry->indices.ptr)),
            geometry->indices.ptr
        );
        aven_gl_check_error(gl);

        buffer->index_len = geometry->indices.len;

        gl->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_texture_draw_range(
        AvenGl *gl,
        AvenGlTextureCtx *ctx,
        AvenGlTextureBuffer *buffer,
        Aff2 cam_trans,
        size_t start,
        size_t end
    ) {
        assert(start < end);
        assert(end <= buffer->index_len);

        gl->UseProgram(ctx->program);
        aven_gl_check_error(gl);

        gl->BindVertexArray(buffer->vao);
        aven_gl_check_error(gl);
        gl->BindBuffer(GL_ARRAY_BUFFER, buffer->vertex);
        aven_gl_check_error(gl);
        gl->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->index);
        aven_gl_check_error(gl);

        gl->Enable(GL_BLEND);
        aven_gl_check_error(gl);
        gl->BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        aven_gl_check_error(gl);
        gl->BindTexture(GL_TEXTURE_2D, ctx->texture_id);
        aven_gl_check_error(gl);

        gl->UniformMatrix2fv(
            (GLint)ctx->utrans_location,
            1,
            GL_FALSE,
            (GLfloat *)cam_trans
        );
        aven_gl_check_error(gl);
        gl->Uniform2fv((GLint)ctx->upos_location, 1, (GLfloat *)cam_trans[2]);
        aven_gl_check_error(gl);

        gl->DrawElements(
            GL_TRIANGLES,
            (GLsizei)(end - start),
            GL_UNSIGNED_SHORT,
            (void *)start
        );
        aven_gl_check_error(gl);

        gl->BindTexture(GL_TEXTURE_2D, 0);
        aven_gl_check_error(gl);

        gl->Disable(GL_BLEND);
        aven_gl_check_error(gl);

        gl->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        aven_gl_check_error(gl);
        gl->BindBuffer(GL_ARRAY_BUFFER, 0);
        aven_gl_check_error(gl);
        gl->BindVertexArray(0);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_texture_draw(
        AvenGl *gl,
        AvenGlTextureCtx *ctx,
        AvenGlTextureBuffer *buffer,
        Aff2 cam_trans
    ) {
        aven_gl_texture_draw_range(
            gl,
            ctx,
            buffer,
            cam_trans,
            0,
            buffer->index_len
        );
    }
#endif // AVEN_GL_TEXTURE_H
