#define _POSIX_C_SOURCE 200112L
#define AVEN_IMPLEMENTATION

#include <aven.h>
#include <aven/arena.h>
#include <aven/fs.h>
#include <aven/gl.h>

#include <stdio.h>
#include <stdlib.h>

#include <GLFW/glfw3.h>

#define ARENA_SIZE (4096 * 16)

AvenArena test_arena;

int main(void) {
    aven_fs_utf8_mode();
    void *mem = malloc(ARENA_SIZE);
    test_arena = aven_arena_init(mem, ARENA_SIZE);

    int width = 480;
    int height = 480;

    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);

    GLFWwindow *window = glfwCreateWindow(
        (int)width,
        (int)height,
        "AvenGL Test",
        NULL,
        NULL
    );
    if (window == NULL) {
        glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);
        window = glfwCreateWindow(
            (int)width,
            (int)height,
            "AvenGL Test",
            NULL,
            NULL
        );
        if (window == NULL) {
            printf("test failed: glfwCreateWindow\n");
            return 1;
        }
    }

    glfwMakeContextCurrent(window);

    AvenGL gl = aven_gl_load(glfwGetProcAddress);
    bool fail = false;

    if (gl.ActiveTexture == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glActiveTexture\n");
    }
    if (gl.AttachShader == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glAttachShader\n");
    }
    if (gl.BindAttribLocation == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glBindAttribLocation\n");
    }
    if (gl.BindBuffer == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glBindBuffer\n");
    }
    if (gl.BindFramebuffer == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glBindFramebuffer\n");
    }
    if (gl.BindRenderbuffer == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glBindRenderbuffer\n");
    }
    if (gl.BindTexture == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glBindTexture\n");
    }
    if (gl.BlendColor == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glBlendColor\n");
    }
    if (gl.BlendEquation == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glBlendEquation\n");
    }
    if (gl.BlendEquationSeparate == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glBlendEquationSeparate\n");
    }
    if (gl.BlendFunc == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glBlendFunc\n");
    }
    if (gl.BlendFuncSeparate == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glBlendFuncSeparate\n");
    }
    if (gl.BufferData == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glBufferData\n");
    }
    if (gl.BufferSubData == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glBufferSubData\n");
    }
    if (gl.CheckFramebufferStatus == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glCheckFramebufferStatus\n");
    }
    if (gl.Clear == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glClear\n");
    }
    if (gl.ClearColor == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glClearColor\n");
    }
    if (gl.ClearDepth == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glClearDepth\n");
    }
    if (gl.ClearStencil == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glClearStencil\n");
    }
    if (gl.ColorMask == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glColorMask\n");
    }
    if (gl.CompileShader == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glCompileShader\n");
    }
    if (gl.CompressedTexImage2D == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glCompressedTexImage2D\n");
    }
    if (gl.CompressedTexSubImage2D == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glCompressedTexSubImage2D\n");
    }
    if (gl.CopyTexImage2D == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glCopyTexImage2D\n");
    }
    if (gl.CopyTexSubImage2D == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glCopyTexSubImage2D\n");
    }
    if (gl.CreateProgram == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glCreateProgram\n");
    }
    if (gl.CreateShader == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glCreateShader\n");
    }
    if (gl.CullFace == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glCullFace\n");
    }
    if (gl.DeleteBuffers == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glDeleteBuffers\n");
    }
    if (gl.DeleteFramebuffers == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glDeleteFramebuffers\n");
    }
    if (gl.DeleteProgram == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glDeleteProgram\n");
    }
    if (gl.DeleteRenderbuffers == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glDeleteRenderbuffers\n");
    }
    if (gl.DeleteShader == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glDeleteShader\n");
    }
    if (gl.DeleteTextures == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glDeleteTextures\n");
    }
    if (gl.DepthFunc == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glDepthFunc\n");
    }
    if (gl.DepthMask == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glDepthMask\n");
    }
    if (gl.DepthRangef == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glDepthRangef\n");
    }
    if (gl.DetachShader == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glDetachShader\n");
    }
    if (gl.Disable == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glDisable\n");
    }
    if (gl.DisableVertexAttribArray == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glDisableVertexAttribArray\n");
    }
    if (gl.DrawArrays == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glDrawArrays\n");
    }
    if (gl.DrawElements == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glDrawElements\n");
    }
    if (gl.Enable == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glEnable\n");
    }
    if (gl.EnableVertexAttribArray == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glEnableVertexAttribArray\n");
    }
    if (gl.Finish == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glFinish\n");
    }
    if (gl.Flush == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glFlush\n");
    }
    if (gl.FramebufferRenderbuffer == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glFramebufferRenderbuffer\n");
    }
    if (gl.FramebufferTexture2D == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glFramebufferTexture2D\n");
    }
    if (gl.FrontFace == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glFrontFace\n");
    }
    if (gl.GenBuffers == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glGenBuffers\n");
    }
    if (gl.GenerateMipmap == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glGenerateMipmap\n");
    }
    if (gl.GenFramebuffers == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glGenFramebuffers\n");
    }
    if (gl.GenRenderbuffers == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glGenRenderbuffers\n");
    }
    if (gl.GenTextures == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glGenTextures\n");
    }
    if (gl.GetActiveAttrib == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glGetActiveAttrib\n");
    }
    if (gl.GetActiveUniform == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glGetActiveUniform\n");
    }
    if (gl.GetAttachedShaders == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glGetAttachedShaders\n");
    }
    if (gl.GetAttribLocation == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glGetAttribLocation\n");
    }
    if (gl.GetBooleanv == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glGetBooleanv\n");
    }
    if (gl.GetBufferParameteriv == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glGetBufferParameteriv\n");
    }
    if (gl.GetError == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glGetError\n");
    }
    if (gl.GetFloatv == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glGetFloatv\n");
    }
    if (gl.GetFramebufferAttachmentParameteriv == NULL) {
        fail = true;
        printf(
            "test failed: aven_gl_load glGetFramebufferAttachmentParameteriv\n"
        );
    }
    if (gl.GetIntegerv == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glGetIntegerv\n");
    }
    if (gl.GetProgramiv == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glGetProgramiv\n");
    }
    if (gl.GetProgramInfoLog == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glGetProgramInfoLog\n");
    }
    if (gl.GetRenderbufferParameteriv == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glGetRenderbufferParameteriv\n");
    }
    if (gl.GetShaderiv == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glGetShaderiv\n");
    }
    if (gl.GetShaderInfoLog == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glGetShaderInfoLog\n");
    }
    if (gl.GetShaderPrecisionFormat == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glGetShaderPrecisionFormat\n");
    }
    if (gl.GetShaderSource == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glGetShaderSource\n");
    }
    if (gl.GetString == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glGetString\n");
    }
    if (gl.GetTexParameterfv == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glGetTexParameterfv\n");
    }
    if (gl.GetTexParameteriv == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glGetTexParameteriv\n");
    }
    if (gl.GetUniformfv == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glGetUniformfv\n");
    }
    if (gl.GetUniformiv == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glGetUniformiv\n");
    }
    if (gl.GetUniformLocation == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glGetUniformLocation\n");
    }
    if (gl.GetVertexAttribfv == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glGetVertexAttribfv\n");
    }
    if (gl.GetVertexAttribiv == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glGetVertexAttribiv\n");
    }
    if (gl.GetVertexAttribPointerv == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glGetVertexAttribPointerv\n");
    }
    if (gl.Hint == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glHint\n");
    }
    if (gl.IsBuffer == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glIsBuffer\n");
    }
    if (gl.IsEnabled == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glIsEnabled\n");
    }
    if (gl.IsFramebuffer == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glIsFramebuffer\n");
    }
    if (gl.IsProgram == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glIsProgram\n");
    }
    if (gl.IsRenderbuffer == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glIsRenderbuffer\n");
    }
    if (gl.IsShader == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glIsShader\n");
    }
    if (gl.IsTexture == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glIsTexture\n");
    }
    if (gl.LineWidth == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glLineWidth\n");
    }
    if (gl.LinkProgram == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glLinkProgram\n");
    }
    if (gl.PixelStorei == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glPixelStorei\n");
    }
    if (gl.PolygonOffset == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glPolygonOffset\n");
    }
    if (gl.ReadPixels == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glReadPixels\n");
    }
    if (gl.ReleaseShaderCompiler == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glReleaseShaderCompiler\n");
    }
    if (gl.RenderbufferStorage == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glRenderbufferStorage\n");
    }
    if (gl.SampleCoverage == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glSampleCoverage\n");
    }
    if (gl.Scissor == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glScissor\n");
    }
    if (gl.ShaderBinary == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glShaderBinary\n");
    }
    if (gl.ShaderSource == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glShaderSource\n");
    }
    if (gl.StencilFunc == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glStencilFunc\n");
    }
    if (gl.StencilFuncSeparate == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glStencilFuncSeparate\n");
    }
    if (gl.StencilMask == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glStencilMask\n");
    }
    if (gl.StencilMaskSeparate == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glStencilMaskSeparate\n");
    }
    if (gl.StencilOp == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glStencilOp\n");
    }
    if (gl.StencilOpSeparate == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glStencilOpSeparate\n");
    }
    if (gl.TexImage2D == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glTexImage2D\n");
    }
    if (gl.TexParameterf == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glTexParameterf\n");
    }
    if (gl.TexParameterfv == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glTexParameterfv\n");
    }
    if (gl.TexParameteri == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glTexParameteri\n");
    }
    if (gl.TexParameteriv == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glTexParameteriv\n");
    }
    if (gl.TexSubImage2D == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glTexSubImage2D\n");
    }
    if (gl.Uniform1f == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glUniform1f\n");
    }
    if (gl.Uniform1fv == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glUniform1fv\n");
    }
    if (gl.Uniform1i == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glUniform1i\n");
    }
    if (gl.Uniform1iv == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glUniform1iv\n");
    }
    if (gl.Uniform2f == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glUniform2f\n");
    }
    if (gl.Uniform2fv == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glUniform2fv\n");
    }
    if (gl.Uniform2i == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glUniform2i\n");
    }
    if (gl.Uniform2iv == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glUniform2iv\n");
    }
    if (gl.Uniform3f == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glUniform3f\n");
    }
    if (gl.Uniform3fv == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glUniform3fv\n");
    }
    if (gl.Uniform3i == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glUniform3i\n");
    }
    if (gl.Uniform3iv == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glUniform3iv\n");
    }
    if (gl.Uniform4f == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glUniform4f\n");
    }
    if (gl.Uniform4fv == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glUniform4fv\n");
    }
    if (gl.Uniform4i == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glUniform4i\n");
    }
    if (gl.Uniform4iv == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glUniform4iv\n");
    }
    if (gl.UniformMatrix2fv == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glUniformMatrix2fv\n");
    }
    if (gl.UniformMatrix3fv == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glUniformMatrix3fv\n");
    }
    if (gl.UniformMatrix4fv == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glUniformMatrix4fv\n");
    }
    if (gl.UseProgram == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glUseProgram\n");
    }
    if (gl.ValidateProgram == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glValidateProgram\n");
    }
    if (gl.VertexAttrib1f == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glVertexAttrib1f\n");
    }
    if (gl.VertexAttrib1fv == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glVertexAttrib1fv\n");
    }
    if (gl.VertexAttrib2f == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glVertexAttrib2f\n");
    }
    if (gl.VertexAttrib2fv == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glVertexAttrib2fv\n");
    }
    if (gl.VertexAttrib3f == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glVertexAttrib3f\n");
    }
    if (gl.VertexAttrib3fv == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glVertexAttrib3fv\n");
    }
    if (gl.VertexAttrib4f == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glVertexAttrib4f\n");
    }
    if (gl.VertexAttrib4fv == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glVertexAttrib4fv\n");
    }
    if (gl.VertexAttribPointer == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glVertexAttribPointer\n");
    }
    if (gl.Viewport == NULL) {
        fail = true;
        printf("test failed: aven_gl_load glViewport\n");
    }

    if (!fail) {
        printf("all tests passed\n");
        return 0;
    }

    return 1;
}
