#ifndef AVEN_GL_H
    #define AVEN_GL_H

    #include <aven.h>
    #include <aven/io.h>
    #include <aven/str.h>

    #define GL_GLES_PROTOTYPES 0
    #include <GLES3/gl32.h>

    typedef struct {
        PFNGLACTIVETEXTUREPROC ActiveTexture;
        PFNGLATTACHSHADERPROC AttachShader;
        PFNGLBINDATTRIBLOCATIONPROC BindAttribLocation;
        PFNGLBINDBUFFERPROC BindBuffer;
        PFNGLBINDFRAMEBUFFERPROC BindFramebuffer;
        PFNGLBINDRENDERBUFFERPROC BindRenderbuffer;
        PFNGLBINDTEXTUREPROC BindTexture;
        PFNGLBLENDCOLORPROC BlendColor;
        PFNGLBLENDEQUATIONPROC BlendEquation;
        PFNGLBLENDEQUATIONSEPARATEPROC BlendEquationSeparate;
        PFNGLBLENDFUNCPROC BlendFunc;
        PFNGLBLENDFUNCSEPARATEPROC BlendFuncSeparate;
        PFNGLBUFFERDATAPROC BufferData;
        PFNGLBUFFERSUBDATAPROC BufferSubData;
        PFNGLCHECKFRAMEBUFFERSTATUSPROC CheckFramebufferStatus;
        PFNGLCLEARPROC Clear;
        PFNGLCLEARCOLORPROC ClearColor;
        PFNGLCLEARDEPTHFPROC ClearDepthf;
        PFNGLCLEARSTENCILPROC ClearStencil;
        PFNGLCOLORMASKPROC ColorMask;
        PFNGLCOMPILESHADERPROC CompileShader;
        PFNGLCOMPRESSEDTEXIMAGE2DPROC CompressedTexImage2D;
        PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC CompressedTexSubImage2D;
        PFNGLCOPYTEXIMAGE2DPROC CopyTexImage2D;
        PFNGLCOPYTEXSUBIMAGE2DPROC CopyTexSubImage2D;
        PFNGLCREATEPROGRAMPROC CreateProgram;
        PFNGLCREATESHADERPROC CreateShader;
        PFNGLCULLFACEPROC CullFace;
        PFNGLDELETEBUFFERSPROC DeleteBuffers;
        PFNGLDELETEFRAMEBUFFERSPROC DeleteFramebuffers;
        PFNGLDELETEPROGRAMPROC DeleteProgram;
        PFNGLDELETERENDERBUFFERSPROC DeleteRenderbuffers;
        PFNGLDELETESHADERPROC DeleteShader;
        PFNGLDELETETEXTURESPROC DeleteTextures;
        PFNGLDEPTHFUNCPROC DepthFunc;
        PFNGLDEPTHMASKPROC DepthMask;
        PFNGLDEPTHRANGEFPROC DepthRangef;
        PFNGLDETACHSHADERPROC DetachShader;
        PFNGLDISABLEPROC Disable;
        PFNGLDISABLEVERTEXATTRIBARRAYPROC DisableVertexAttribArray;
        PFNGLDRAWARRAYSPROC DrawArrays;
        PFNGLDRAWELEMENTSPROC DrawElements;
        PFNGLENABLEPROC Enable;
        PFNGLENABLEVERTEXATTRIBARRAYPROC EnableVertexAttribArray;
        PFNGLFINISHPROC Finish;
        PFNGLFLUSHPROC Flush;
        PFNGLFRAMEBUFFERRENDERBUFFERPROC FramebufferRenderbuffer;
        PFNGLFRAMEBUFFERTEXTURE2DPROC FramebufferTexture2D;
        PFNGLFRONTFACEPROC FrontFace;
        PFNGLGENBUFFERSPROC GenBuffers;
        PFNGLGENERATEMIPMAPPROC GenerateMipmap;
        PFNGLGENFRAMEBUFFERSPROC GenFramebuffers;
        PFNGLGENRENDERBUFFERSPROC GenRenderbuffers;
        PFNGLGENTEXTURESPROC GenTextures;
        PFNGLGETACTIVEATTRIBPROC GetActiveAttrib;
        PFNGLGETACTIVEUNIFORMPROC GetActiveUniform;
        PFNGLGETATTACHEDSHADERSPROC GetAttachedShaders;
        PFNGLGETATTRIBLOCATIONPROC GetAttribLocation;
        PFNGLGETBOOLEANVPROC GetBooleanv;
        PFNGLGETBUFFERPARAMETERIVPROC GetBufferParameteriv;
        PFNGLGETERRORPROC GetError;
        PFNGLGETFLOATVPROC GetFloatv;
        PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC
            GetFramebufferAttachmentParameteriv;
        PFNGLGETINTEGERVPROC GetIntegerv;
        PFNGLGETPROGRAMIVPROC GetProgramiv;
        PFNGLGETPROGRAMINFOLOGPROC GetProgramInfoLog;
        PFNGLGETRENDERBUFFERPARAMETERIVPROC GetRenderbufferParameteriv;
        PFNGLGETSHADERIVPROC GetShaderiv;
        PFNGLGETSHADERINFOLOGPROC GetShaderInfoLog;
        PFNGLGETSHADERPRECISIONFORMATPROC GetShaderPrecisionFormat;
        PFNGLGETSHADERSOURCEPROC GetShaderSource;
        PFNGLGETSTRINGPROC GetString;
        PFNGLGETTEXPARAMETERFVPROC GetTexParameterfv;
        PFNGLGETTEXPARAMETERIVPROC GetTexParameteriv;
        PFNGLGETUNIFORMFVPROC GetUniformfv;
        PFNGLGETUNIFORMIVPROC GetUniformiv;
        PFNGLGETUNIFORMLOCATIONPROC GetUniformLocation;
        PFNGLGETVERTEXATTRIBFVPROC GetVertexAttribfv;
        PFNGLGETVERTEXATTRIBIVPROC GetVertexAttribiv;
        PFNGLGETVERTEXATTRIBPOINTERVPROC GetVertexAttribPointerv;
        PFNGLHINTPROC Hint;
        PFNGLISBUFFERPROC IsBuffer;
        PFNGLISENABLEDPROC IsEnabled;
        PFNGLISFRAMEBUFFERPROC IsFramebuffer;
        PFNGLISPROGRAMPROC IsProgram;
        PFNGLISRENDERBUFFERPROC IsRenderbuffer;
        PFNGLISSHADERPROC IsShader;
        PFNGLISTEXTUREPROC IsTexture;
        PFNGLLINEWIDTHPROC LineWidth;
        PFNGLLINKPROGRAMPROC LinkProgram;
        PFNGLPIXELSTOREIPROC PixelStorei;
        PFNGLPOLYGONOFFSETPROC PolygonOffset;
        PFNGLREADPIXELSPROC ReadPixels;
        PFNGLRELEASESHADERCOMPILERPROC ReleaseShaderCompiler;
        PFNGLRENDERBUFFERSTORAGEPROC RenderbufferStorage;
        PFNGLSAMPLECOVERAGEPROC SampleCoverage;
        PFNGLSCISSORPROC Scissor;
        PFNGLSHADERBINARYPROC ShaderBinary;
        PFNGLSHADERSOURCEPROC ShaderSource;
        PFNGLSTENCILFUNCPROC StencilFunc;
        PFNGLSTENCILFUNCSEPARATEPROC StencilFuncSeparate;
        PFNGLSTENCILMASKPROC StencilMask;
        PFNGLSTENCILMASKSEPARATEPROC StencilMaskSeparate;
        PFNGLSTENCILOPPROC StencilOp;
        PFNGLSTENCILOPSEPARATEPROC StencilOpSeparate;
        PFNGLTEXIMAGE2DPROC TexImage2D;
        PFNGLTEXPARAMETERFPROC TexParameterf;
        PFNGLTEXPARAMETERFVPROC TexParameterfv;
        PFNGLTEXPARAMETERIPROC TexParameteri;
        PFNGLTEXPARAMETERIVPROC TexParameteriv;
        PFNGLTEXSUBIMAGE2DPROC TexSubImage2D;
        PFNGLUNIFORM1FPROC Uniform1f;
        PFNGLUNIFORM1FVPROC Uniform1fv;
        PFNGLUNIFORM1IPROC Uniform1i;
        PFNGLUNIFORM1IVPROC Uniform1iv;
        PFNGLUNIFORM2FPROC Uniform2f;
        PFNGLUNIFORM2FVPROC Uniform2fv;
        PFNGLUNIFORM2IPROC Uniform2i;
        PFNGLUNIFORM2IVPROC Uniform2iv;
        PFNGLUNIFORM3FPROC Uniform3f;
        PFNGLUNIFORM3FVPROC Uniform3fv;
        PFNGLUNIFORM3IPROC Uniform3i;
        PFNGLUNIFORM3IVPROC Uniform3iv;
        PFNGLUNIFORM4FPROC Uniform4f;
        PFNGLUNIFORM4FVPROC Uniform4fv;
        PFNGLUNIFORM4IPROC Uniform4i;
        PFNGLUNIFORM4IVPROC Uniform4iv;
        PFNGLUNIFORMMATRIX2FVPROC UniformMatrix2fv;
        PFNGLUNIFORMMATRIX2FVPROC UniformMatrix3fv;
        PFNGLUNIFORMMATRIX2FVPROC UniformMatrix4fv;
        PFNGLUSEPROGRAMPROC UseProgram;
        PFNGLVALIDATEPROGRAMPROC ValidateProgram;
        PFNGLVERTEXATTRIB1FPROC VertexAttrib1f;
        PFNGLVERTEXATTRIB1FVPROC VertexAttrib1fv;
        PFNGLVERTEXATTRIB2FPROC VertexAttrib2f;
        PFNGLVERTEXATTRIB2FVPROC VertexAttrib2fv;
        PFNGLVERTEXATTRIB3FPROC VertexAttrib3f;
        PFNGLVERTEXATTRIB3FVPROC VertexAttrib3fv;
        PFNGLVERTEXATTRIB4FPROC VertexAttrib4f;
        PFNGLVERTEXATTRIB4FVPROC VertexAttrib4fv;
        PFNGLVERTEXATTRIBPOINTERPROC VertexAttribPointer;
        PFNGLVIEWPORTPROC Viewport;
        PFNGLBINDVERTEXARRAYPROC BindVertexArray;
        PFNGLGENVERTEXARRAYSPROC GenVertexArrays;
        PFNGLDELETEVERTEXARRAYSPROC DeleteVertexArrays;
        PFNGLISVERTEXARRAYPROC IsVertexArray;
        PFNGLDEBUGMESSAGECALLBACKPROC DebugMessageCallback;
        bool es;
    } AvenGl;

    typedef void (*AvenGlProcFn)(void);
    typedef AvenGlProcFn (*AvenGlLoadProcFn)(const char *);

    static inline AvenGl aven_gl_load(AvenGlLoadProcFn load, bool es) {
        AvenGl gl = { .es = es };

        gl.ActiveTexture = (PFNGLACTIVETEXTUREPROC)load("glActiveTexture");
        gl.AttachShader = (PFNGLATTACHSHADERPROC)load("glAttachShader");
        gl.BindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)load(
            "glBindAttribLocation"
        );
        gl.BindBuffer = (PFNGLBINDBUFFERPROC)load("glBindBuffer");
        gl.BindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)load("glBindFramebuffer");
        gl.BindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC)load(
            "glBindRenderbuffer"
        );
        gl.BindTexture = (PFNGLBINDTEXTUREPROC)load("glBindTexture");
        gl.BlendColor = (PFNGLBLENDCOLORPROC)load("glBlendColor");
        gl.BlendEquation = (PFNGLBLENDEQUATIONPROC)load("glBlendEquation");
        gl.BlendEquationSeparate = (PFNGLBLENDEQUATIONSEPARATEPROC)load(
            "glBlendEquationSeparate"
        );
        gl.BlendFunc = (PFNGLBLENDFUNCPROC)load("glBlendFunc");
        gl.BlendFuncSeparate = (PFNGLBLENDFUNCSEPARATEPROC)load(
            "glBlendFuncSeparate"
        );
        gl.BufferData = (PFNGLBUFFERDATAPROC)load("glBufferData");
        gl.BufferSubData = (PFNGLBUFFERSUBDATAPROC)load("glBufferSubData");
        gl.CheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)load(
            "glCheckFramebufferStatus"
        );
        gl.Clear = (PFNGLCLEARPROC)load("glClear");
        gl.ClearColor = (PFNGLCLEARCOLORPROC)load("glClearColor");
        gl.ClearDepthf = (PFNGLCLEARDEPTHFPROC)load("glClearDepthf");
        gl.ClearStencil = (PFNGLCLEARSTENCILPROC)load("glClearStencil");
        gl.ColorMask = (PFNGLCOLORMASKPROC)load("glColorMask");
        gl.CompileShader = (PFNGLCOMPILESHADERPROC)load("glCompileShader");
        gl.CompressedTexImage2D = (PFNGLCOMPRESSEDTEXIMAGE2DPROC)load(
            "glCompressedTexImage2D"
        );
        gl.CompressedTexSubImage2D = (PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC)load(
            "glCompressedTexSubImage2D"
        );
        gl.CopyTexImage2D = (PFNGLCOPYTEXIMAGE2DPROC)load("glCopyTexImage2D");
        gl.CopyTexSubImage2D = (PFNGLCOPYTEXSUBIMAGE2DPROC)load(
            "glCopyTexSubImage2D"
        );
        gl.CreateProgram = (PFNGLCREATEPROGRAMPROC)load("glCreateProgram");
        gl.CreateShader = (PFNGLCREATESHADERPROC)load("glCreateShader");
        gl.CullFace = (PFNGLCULLFACEPROC)load("glCullFace");
        gl.DeleteBuffers = (PFNGLDELETEBUFFERSPROC)load("glDeleteBuffers");
        gl.DeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC)load(
            "glDeleteFramebuffers"
        );
        gl.DeleteProgram = (PFNGLDELETEPROGRAMPROC)load("glDeleteProgram");
        gl.DeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC)load(
            "glDeleteRenderbuffers"
        );
        gl.DeleteShader = (PFNGLDELETESHADERPROC)load("glDeleteShader");
        gl.DeleteTextures = (PFNGLDELETETEXTURESPROC)load("glDeleteTextures");
        gl.DepthFunc = (PFNGLDEPTHFUNCPROC)load("glDepthFunc");
        gl.DepthMask = (PFNGLDEPTHMASKPROC)load("glDepthMask");
        gl.DepthRangef = (PFNGLDEPTHRANGEFPROC)load("glDepthRangef");
        gl.DetachShader = (PFNGLDETACHSHADERPROC)load("glDetachShader");
        gl.Disable = (PFNGLDISABLEPROC)load("glDisable");
        gl.DisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)load(
            "glDisableVertexAttribArray"
        );
        gl.DrawArrays = (PFNGLDRAWARRAYSPROC)load("glDrawArrays");
        gl.DrawElements = (PFNGLDRAWELEMENTSPROC)load("glDrawElements");
        gl.Enable = (PFNGLENABLEPROC)load("glEnable");
        gl.EnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)load(
            "glEnableVertexAttribArray"
        );
        gl.Finish = (PFNGLFINISHPROC)load("glFinish");
        gl.Flush = (PFNGLFLUSHPROC)load("glFlush");
        gl.FramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)load(
            "glFramebufferRenderbuffer"
        );
        gl.FramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)load(
            "glFramebufferTexture2D"
        );
        gl.FrontFace = (PFNGLFRONTFACEPROC)load("glFrontFace");
        gl.GenBuffers = (PFNGLGENBUFFERSPROC)load("glGenBuffers");
        gl.GenerateMipmap = (PFNGLGENERATEMIPMAPPROC)load("glGenerateMipmap");
        gl.GenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)load("glGenFramebuffers");
        gl.GenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC)load(
            "glGenRenderbuffers"
        );
        gl.GenTextures = (PFNGLGENTEXTURESPROC)load("glGenTextures");
        gl.GetActiveAttrib = (PFNGLGETACTIVEATTRIBPROC)load("glGetActiveAttrib");
        gl.GetActiveUniform = (PFNGLGETACTIVEUNIFORMPROC)load(
            "glGetActiveUniform"
        );
        gl.GetAttachedShaders = (PFNGLGETATTACHEDSHADERSPROC)load(
            "glGetAttachedShaders"
        );
        gl.GetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)load(
            "glGetAttribLocation"
        );
        gl.GetBooleanv = (PFNGLGETBOOLEANVPROC)load("glGetBooleanv");
        gl.GetBufferParameteriv = (PFNGLGETBUFFERPARAMETERIVPROC)load(
            "glGetBufferParameteriv"
        );
        gl.GetError = (PFNGLGETERRORPROC)load("glGetError");
        gl.GetFloatv = (PFNGLGETFLOATVPROC)load("glGetFloatv");
        gl.GetFramebufferAttachmentParameteriv = (
            PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC
        )load("glGetFramebufferAttachmentParameteriv");
        gl.GetIntegerv = (PFNGLGETINTEGERVPROC)load("glGetIntegerv");
        gl.GetProgramiv = (PFNGLGETPROGRAMIVPROC)load("glGetProgramiv");
        gl.GetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)load(
            "glGetProgramInfoLog"
        );
        gl.GetRenderbufferParameteriv = (PFNGLGETRENDERBUFFERPARAMETERIVPROC)
            load("glGetRenderbufferParameteriv");
        gl.GetShaderiv = (PFNGLGETSHADERIVPROC)load("glGetShaderiv");
        gl.GetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)load(
            "glGetShaderInfoLog"
        );
        gl.GetShaderPrecisionFormat = (PFNGLGETSHADERPRECISIONFORMATPROC)load(
            "glGetShaderPrecisionFormat"
        );
        gl.GetShaderSource = (PFNGLGETSHADERSOURCEPROC)load("glGetShaderSource");
        gl.GetString = (PFNGLGETSTRINGPROC)load("glGetString");
        gl.GetTexParameterfv = (PFNGLGETTEXPARAMETERFVPROC)load(
            "glGetTexParameterfv"
        );
        gl.GetTexParameteriv = (PFNGLGETTEXPARAMETERIVPROC)load(
            "glGetTexParameteriv"
        );
        gl.GetUniformfv = (PFNGLGETUNIFORMFVPROC)load("glGetUniformfv");
        gl.GetUniformiv = (PFNGLGETUNIFORMIVPROC)load("glGetUniformiv");
        gl.GetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)load(
            "glGetUniformLocation"
        );
        gl.GetVertexAttribfv = (PFNGLGETVERTEXATTRIBFVPROC)load(
            "glGetVertexAttribfv"
        );
        gl.GetVertexAttribiv = (PFNGLGETVERTEXATTRIBIVPROC)load(
            "glGetVertexAttribiv"
        );
        gl.GetVertexAttribPointerv = (PFNGLGETVERTEXATTRIBPOINTERVPROC)load(
            "glGetVertexAttribPointerv"
        );
        gl.Hint = (PFNGLHINTPROC)load("glHint");
        gl.IsBuffer = (PFNGLISBUFFERPROC)load("glIsBuffer");
        gl.IsEnabled = (PFNGLISENABLEDPROC)load("glIsEnabled");
        gl.IsFramebuffer = (PFNGLISFRAMEBUFFERPROC)load("glIsFramebuffer");
        gl.IsProgram = (PFNGLISPROGRAMPROC)load("glIsProgram");
        gl.IsRenderbuffer = (PFNGLISRENDERBUFFERPROC)load("glIsRenderbuffer");
        gl.IsShader = (PFNGLISSHADERPROC)load("glIsShader");
        gl.IsTexture = (PFNGLISTEXTUREPROC)load("glIsTexture");
        gl.LineWidth = (PFNGLLINEWIDTHPROC)load("glLineWidth");
        gl.LinkProgram = (PFNGLLINKPROGRAMPROC)load("glLinkProgram");
        gl.PixelStorei = (PFNGLPIXELSTOREIPROC)load("glPixelStorei");
        gl.PolygonOffset = (PFNGLPOLYGONOFFSETPROC)load("glPolygonOffset");
        gl.ReadPixels = (PFNGLREADPIXELSPROC)load("glReadPixels");
        gl.ReleaseShaderCompiler = (PFNGLRELEASESHADERCOMPILERPROC)load(
            "glReleaseShaderCompiler"
        );
        gl.RenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC)load(
            "glRenderbufferStorage"
        );
        gl.SampleCoverage = (PFNGLSAMPLECOVERAGEPROC)load("glSampleCoverage");
        gl.Scissor = (PFNGLSCISSORPROC)load("glScissor");
        gl.ShaderBinary = (PFNGLSHADERBINARYPROC)load("glShaderBinary");
        gl.ShaderSource = (PFNGLSHADERSOURCEPROC)load("glShaderSource");
        gl.StencilFunc = (PFNGLSTENCILFUNCPROC)load("glStencilFunc");
        gl.StencilFuncSeparate = (PFNGLSTENCILFUNCSEPARATEPROC)load(
            "glStencilFuncSeparate"
        );
        gl.StencilMask = (PFNGLSTENCILMASKPROC)load("glStencilMask");
        gl.StencilMaskSeparate = (PFNGLSTENCILMASKSEPARATEPROC)load(
            "glStencilMaskSeparate"
        );
        gl.StencilOp = (PFNGLSTENCILOPPROC)load("glStencilOp");
        gl.StencilOpSeparate = (PFNGLSTENCILOPSEPARATEPROC)load(
            "glStencilOpSeparate"
        );
        gl.TexImage2D = (PFNGLTEXIMAGE2DPROC)load("glTexImage2D");
        gl.TexParameterf = (PFNGLTEXPARAMETERFPROC)load("glTexParameterf");
        gl.TexParameterfv = (PFNGLTEXPARAMETERFVPROC)load("glTexParameterfv");
        gl.TexParameteri = (PFNGLTEXPARAMETERIPROC)load("glTexParameteri");
        gl.TexParameteriv = (PFNGLTEXPARAMETERIVPROC)load("glTexParameteriv");
        gl.TexSubImage2D = (PFNGLTEXSUBIMAGE2DPROC)load("glTexSubImage2D");
        gl.Uniform1f = (PFNGLUNIFORM1FPROC)load("glUniform1f");
        gl.Uniform1fv = (PFNGLUNIFORM1FVPROC)load("glUniform1fv");
        gl.Uniform1i = (PFNGLUNIFORM1IPROC)load("glUniform1i");
        gl.Uniform1iv = (PFNGLUNIFORM1IVPROC)load("glUniform1iv");
        gl.Uniform2f = (PFNGLUNIFORM2FPROC)load("glUniform2f");
        gl.Uniform2fv = (PFNGLUNIFORM2FVPROC)load("glUniform2fv");
        gl.Uniform2i = (PFNGLUNIFORM2IPROC)load("glUniform2i");
        gl.Uniform2iv = (PFNGLUNIFORM2IVPROC)load("glUniform2iv");
        gl.Uniform3f = (PFNGLUNIFORM3FPROC)load("glUniform3f");
        gl.Uniform3fv = (PFNGLUNIFORM3FVPROC)load("glUniform3fv");
        gl.Uniform3i = (PFNGLUNIFORM3IPROC)load("glUniform3i");
        gl.Uniform3iv = (PFNGLUNIFORM3IVPROC)load("glUniform3iv");
        gl.Uniform4f = (PFNGLUNIFORM4FPROC)load("glUniform4f");
        gl.Uniform4fv = (PFNGLUNIFORM4FVPROC)load("glUniform4fv");
        gl.Uniform4i = (PFNGLUNIFORM4IPROC)load("glUniform4i");
        gl.Uniform4iv = (PFNGLUNIFORM4IVPROC)load("glUniform4iv");
        gl.UniformMatrix2fv = (PFNGLUNIFORMMATRIX2FVPROC)load(
            "glUniformMatrix2fv"
        );
        gl.UniformMatrix3fv = (PFNGLUNIFORMMATRIX2FVPROC)load(
            "glUniformMatrix3fv"
        );
        gl.UniformMatrix4fv = (PFNGLUNIFORMMATRIX2FVPROC)load(
            "glUniformMatrix4fv"
        );
        gl.UseProgram = (PFNGLUSEPROGRAMPROC)load("glUseProgram");
        gl.ValidateProgram = (PFNGLVALIDATEPROGRAMPROC)load("glValidateProgram");
        gl.VertexAttrib1f = (PFNGLVERTEXATTRIB1FPROC)load("glVertexAttrib1f");
        gl.VertexAttrib1fv = (PFNGLVERTEXATTRIB1FVPROC)load("glVertexAttrib1fv");
        gl.VertexAttrib2f = (PFNGLVERTEXATTRIB2FPROC)load("glVertexAttrib2f");
        gl.VertexAttrib2fv = (PFNGLVERTEXATTRIB2FVPROC)load("glVertexAttrib2fv");
        gl.VertexAttrib3f = (PFNGLVERTEXATTRIB3FPROC)load("glVertexAttrib3f");
        gl.VertexAttrib3fv = (PFNGLVERTEXATTRIB3FVPROC)load("glVertexAttrib3fv");
        gl.VertexAttrib4f = (PFNGLVERTEXATTRIB4FPROC)load("glVertexAttrib4f");
        gl.VertexAttrib4fv = (PFNGLVERTEXATTRIB4FVPROC)load("glVertexAttrib4fv");
        gl.VertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)load(
            "glVertexAttribPointer"
        );
        gl.Viewport = (PFNGLVIEWPORTPROC)load("glViewport");
        gl.BindVertexArray = (PFNGLBINDVERTEXARRAYPROC)load("glBindVertexArray");
        gl.GenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)load("glGenVertexArrays");
        gl.DeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)load(
            "glDeleteVertexArrays"
        );
        gl.IsVertexArray = (PFNGLISVERTEXARRAYPROC)load("glIsVertexArray");
        gl.DebugMessageCallback = (PFNGLDEBUGMESSAGECALLBACKPROC)load(
            "glDebugMessageCallback"
        );

        return gl;
    }

    #ifdef AVEN_GL_NDEBUG
        #define aven_gl_check_error(gl) (void)(gl)
    #else
        #define aven_gl_check_error(gl) do { \
                if ((gl)->GetError() != 0) { \
                    aven_panic("opengl error"); \
                } \
            } while (0)
    #endif

    #define aven_gl_shader(gl, str) ( \
            gl->es ? "#version 300 es\n" str : "#version 430\n" str \
        )

    static inline void aven_gl_ActiveTexture(AvenGl *gl, GLenum texture) {
        gl->ActiveTexture(texture);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_AttachShader(
        AvenGl *gl,
        GLuint program,
        GLuint shader
    ) {
        gl->AttachShader(program, shader);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_BindAttribLocation(
        AvenGl *gl,
        GLuint program,
        GLuint index,
        const GLchar *name
    ) {
        gl->BindAttribLocation(program, index, name);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_BindBuffer(
        AvenGl *gl,
        GLenum target,
        GLuint buffer
    ) {
        gl->BindBuffer(target, buffer);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_BindFramebuffer(
        AvenGl *gl,
        GLenum target,
        GLuint framebuffer
    ) {
        gl->BindFramebuffer(target, framebuffer);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_BindRenderbuffer(
        AvenGl *gl,
        GLenum target,
        GLuint renderbuffer
    ) {
        gl->BindRenderbuffer(target, renderbuffer);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_BindTexture(
        AvenGl *gl,
        GLenum target,
        GLuint texture
    ) {
        gl->BindTexture(target, texture);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_BlendColor(
        AvenGl *gl,
        GLfloat red,
        GLfloat green,
        GLfloat blue,
        GLfloat alpha
    ) {
        gl->BlendColor(red, green, blue, alpha);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_BlendEquation(AvenGl *gl, GLenum mode) {
        gl->BlendEquation(mode);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_BlendEquationSeparate(
        AvenGl *gl,
        GLenum modeRGB,
        GLenum modeAlpha
    ) {
        gl->BlendEquationSeparate(modeRGB, modeAlpha);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_BlendFunc(
        AvenGl *gl,
        GLenum sfactor,
        GLenum dfactor
    ) {
        gl->BlendFunc(sfactor, dfactor);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_BlendFuncSeparate(
        AvenGl *gl,
        GLenum sfactorRGB,
        GLenum dfactorRGB,
        GLenum sfactorAlpha,
        GLenum dfactorAlpha
    ) {
        gl->BlendFuncSeparate(
            sfactorRGB,
            dfactorRGB,
            sfactorAlpha,
            dfactorAlpha
        );
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_BufferData(
        AvenGl *gl,
        GLenum target,
        GLsizeiptr size,
        const void *data,
        GLenum usage
    ) {
        gl->BufferData(target, size, data, usage);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_BufferSubData(
        AvenGl *gl,
        GLenum target,
        GLintptr offset,
        GLsizeiptr size,
        const void *data
    ) {
        gl->BufferSubData(target, offset, size, data);
        aven_gl_check_error(gl);
    }

    static inline GLenum aven_gl_CheckFramebufferStatus(
        AvenGl *gl,
        GLenum target
    ) {
        GLenum res = gl->CheckFramebufferStatus(target);
        aven_gl_check_error(gl);
        return res;
    }

    static inline void aven_gl_Clear(AvenGl *gl, GLbitfield mask) {
        gl->Clear(mask);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_ClearColor(
        AvenGl *gl,
        GLfloat red,
        GLfloat green,
        GLfloat blue,
        GLfloat alpha
    ) {
        gl->ClearColor(red, green, blue, alpha);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_ClearDepthf(AvenGl *gl, GLfloat d) {
        gl->ClearDepthf(d);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_ClearStencil(AvenGl *gl, GLint s) {
        gl->ClearStencil(s);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_ColorMask(
        AvenGl *gl,
        GLboolean red,
        GLboolean green,
        GLboolean blue,
        GLboolean alpha
    ) {
        gl->ColorMask(red, green, blue, alpha);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_CompileShader(AvenGl *gl, GLuint shader) {
        gl->CompileShader(shader);
        GLint vertex_compiled = 0;
        gl->GetShaderiv(shader, GL_COMPILE_STATUS, &vertex_compiled);
        if (!vertex_compiled) {
            GLint info_len = 0;
            gl->GetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_len);

            char info_buffer[4096];
            if (info_len > 0 and (size_t)info_len <= sizeof(info_buffer)) {
                AvenStr info_tot = slice_array(info_buffer);
                AvenStr info = aven_str_head(info_tot, (size_t)info_len);
                gl->GetShaderInfoLog(shader, info_len, NULL, info.ptr);
                aven_io_perrf(
                    "error compiling vertex shader:\n{}",
                    aven_fmt_str(info)
                );
            }
            aven_panic("failed to compile shader");
        }
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_CompressedTexImage2D(
        AvenGl *gl,
        GLenum target,
        GLint level,
        GLenum internalformat,
        GLsizei width,
        GLsizei height,
        GLint border,
        GLsizei imageSize,
        const void *data
    ) {
        gl->CompressedTexImage2D(
            target,
            level,
            internalformat,
            width,
            height,
            border,
            imageSize,
            data
        );
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_CompressedTexSubImage2D(
        AvenGl *gl,
        GLenum target,
        GLint level,
        GLint xoffset,
        GLint yoffset,
        GLsizei width,
        GLsizei height,
        GLenum format,
        GLsizei imageSize,
        const void *data
    ) {
        gl->CompressedTexSubImage2D(
            target,
            level,
            xoffset,
            yoffset,
            width,
            height,
            format,
            imageSize,
            data
        );
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_CopyTexImage2D(
        AvenGl *gl,
        GLenum target,
        GLint level,
        GLenum internalformat,
        GLint x,
        GLint y,
        GLsizei width,
        GLsizei height,
        GLint border
    ) {
        gl->CopyTexImage2D(
            target,
            level,
            internalformat,
            x,
            y,
            width,
            height,
            border
        );
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_CopyTexSubImage2D(
        AvenGl *gl,
        GLenum target,
        GLint level,
        GLint xoffset,
        GLint yoffset,
        GLint x,
        GLint y,
        GLsizei width,
        GLsizei height
    ) {
        gl->CopyTexSubImage2D(
            target,
            level,
            xoffset,
            yoffset,
            x,
            y,
            width,
            height
        );
        aven_gl_check_error(gl);
    }

    static inline GLuint aven_gl_CreateProgram(AvenGl *gl) {
        GLuint res = gl->CreateProgram();
        aven_gl_check_error(gl);
        return res;
    }

    static inline GLuint aven_gl_CreateShader(AvenGl *gl, GLenum type) {
        GLuint res = gl->CreateShader(type);
        aven_gl_check_error(gl);
        return res;
    }

    static inline void aven_gl_CullFace(AvenGl *gl, GLenum mode) {
        gl->CullFace(mode);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_DeleteBuffers(
        AvenGl *gl,
        GLsizei n,
        const GLuint *buffers
    ) {
        gl->DeleteBuffers(n, buffers);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_DeleteFramebuffers(
        AvenGl *gl,
        GLsizei n,
        const GLuint *framebuffers
    ) {
        gl->DeleteFramebuffers(n, framebuffers);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_DeleteProgram(AvenGl *gl, GLuint program) {
        gl->DeleteProgram(program);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_DeleteRenderbuffers(
        AvenGl *gl,
        GLsizei n,
        const GLuint *renderbuffers
    ) {
        gl->DeleteRenderbuffers(n, renderbuffers);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_DeleteShader(AvenGl *gl, GLuint shader) {
        gl->DeleteShader(shader);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_DeleteTextures(
        AvenGl *gl,
        GLsizei n,
        const GLuint *textures
    ) {
        gl->DeleteTextures(n, textures);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_DepthFunc(AvenGl *gl, GLenum func) {
        gl->DepthFunc(func);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_DepthMask(AvenGl *gl, GLboolean flag) {
        gl->DepthMask(flag);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_DepthRangef(AvenGl *gl, GLfloat n, GLfloat f) {
        gl->DepthRangef(n, f);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_DetachShader(
        AvenGl *gl,
        GLuint program,
        GLuint shader
    ) {
        gl->DetachShader(program, shader);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_Disable(AvenGl *gl, GLenum cap) {
        gl->Disable(cap);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_DisableVertexAttribArray(
        AvenGl *gl,
        GLuint index
    ) {
        gl->DisableVertexAttribArray(index);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_DrawArrays(
        AvenGl *gl,
        GLenum mode,
        GLint first,
        GLsizei count
    ) {
        gl->DrawArrays(mode, first, count);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_DrawElements(
        AvenGl *gl,
        GLenum mode,
        GLsizei count,
        GLenum type,
        const void *indices
    ) {
        gl->DrawElements(mode, count, type, indices);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_Enable(AvenGl *gl, GLenum cap) {
        gl->Enable(cap);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_EnableVertexAttribArray(AvenGl *gl, GLuint index) {
        gl->EnableVertexAttribArray(index);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_Finish(AvenGl *gl) {
        gl->Finish();
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_Flush(AvenGl *gl) {
        gl->Flush();
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_FramebufferRenderbuffer(
        AvenGl *gl,
        GLenum target,
        GLenum attachment,
        GLenum renderbuffertarget,
        GLuint renderbuffer
    ) {
        gl->FramebufferRenderbuffer(
            target,
            attachment,
            renderbuffertarget,
            renderbuffer
        );
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_FramebufferTexture2D(
        AvenGl *gl,
        GLenum target,
        GLenum attachment,
        GLenum textarget,
        GLuint texture,
        GLint level
    ) {
        gl->FramebufferTexture2D(target, attachment, textarget, texture, level);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_FrontFace(AvenGl *gl, GLenum mode) {
        gl->FrontFace(mode);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_GenBuffers(
        AvenGl *gl,
        GLsizei n,
        GLuint *buffers
    ) {
        gl->GenBuffers(n, buffers);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_GenerateMipmap(AvenGl *gl, GLenum target) {
        gl->GenerateMipmap(target);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_GenFramebuffers(
        AvenGl *gl,
        GLsizei n,
        GLuint *framebuffers
    ) {
        gl->GenFramebuffers(n, framebuffers);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_GenRenderbuffers(
        AvenGl *gl,
        GLsizei n,
        GLuint *renderbuffers
    ) {
        gl->GenRenderbuffers(n, renderbuffers);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_GenTextures(
        AvenGl *gl,
        GLsizei n,
        GLuint *textures
    ) {
        gl->GenTextures(n, textures);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_GetActiveAttrib(
        AvenGl *gl,
        GLuint program,
        GLuint index,
        GLsizei bufSize,
        GLsizei *length,
        GLint *size,
        GLenum *type,
        GLchar *name
    ) {
        gl->GetActiveAttrib(program, index, bufSize, length, size, type, name);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_GetActiveUniform(
        AvenGl *gl,
        GLuint program,
        GLuint index,
        GLsizei bufSize,
        GLsizei *length,
        GLint *size,
        GLenum *type,
        GLchar *name
    ) {
        gl->GetActiveUniform(program, index, bufSize, length, size, type, name);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_GetAttachedShaders(
        AvenGl *gl,
        GLuint program,
        GLsizei maxCount,
        GLsizei *count,
        GLuint *shaders
    ) {
        gl->GetAttachedShaders(program, maxCount, count, shaders);
        aven_gl_check_error(gl);
    }

    static inline GLint aven_gl_GetAttribLocation(
        AvenGl *gl,
        GLuint program,
        const GLchar *name
    ) {
        GLint res = gl->GetAttribLocation(program, name);
        aven_gl_check_error(gl);
        return res;
    }

    static inline void aven_gl_GetBooleanv(
        AvenGl *gl,
        GLenum pname,
        GLboolean *data
    ) {
        gl->GetBooleanv(pname, data);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_GetBufferParameteriv(
        AvenGl *gl,
        GLenum target,
        GLenum pname,
        GLint *params
    ) {
        gl->GetBufferParameteriv(target, pname, params);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_GetFloatv(
        AvenGl *gl,
        GLenum pname,
        GLfloat *data
    ) {
        gl->GetFloatv(pname, data);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_GetFramebufferAttachmentParameteriv(
        AvenGl *gl,
        GLenum target,
        GLenum attachment,
        GLenum pname,
        GLint *params
    ) {
        gl->GetFramebufferAttachmentParameteriv(
            target,
            attachment,
            pname,
            params
        );
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_GetIntegerv(
        AvenGl *gl,
        GLenum pname,
        GLint *data
    ) {
        gl->GetIntegerv(pname, data);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_GetProgramiv(
        AvenGl *gl,
        GLuint program,
        GLenum pname,
        GLint *params
    ) {
        gl->GetProgramiv(program, pname, params);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_GetProgramInfoLog(
        AvenGl *gl,
        GLuint program,
        GLsizei bufSize,
        GLsizei *length,
        GLchar *infoLog
    ) {
        gl->GetProgramInfoLog(program, bufSize, length, infoLog);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_GetRenderbufferParameteriv(
        AvenGl *gl,
        GLenum target,
        GLenum pname,
        GLint *params
    ) {
        gl->GetRenderbufferParameteriv(target, pname, params);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_GetShaderiv(
        AvenGl *gl,
        GLuint shader,
        GLenum pname,
        GLint *params
    ) {
        gl->GetShaderiv(shader, pname, params);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_GetShaderInfoLog(
        AvenGl *gl,
        GLuint shader,
        GLsizei bufSize,
        GLsizei *length,
        GLchar *infoLog
    ) {
        gl->GetShaderInfoLog(shader, bufSize, length, infoLog);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_GetShaderPrecisionFormat(
        AvenGl *gl,
        GLenum shadertype,
        GLenum precisiontype,
        GLint *range,
        GLint *precision
    ) {
        gl->GetShaderPrecisionFormat(
            shadertype,
            precisiontype,
            range,
            precision
        );
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_GetShaderSource(
        AvenGl *gl,
        GLuint shader,
        GLsizei bufSize,
        GLsizei *length,
        GLchar *source
    ) {
        gl->GetShaderSource(shader, bufSize, length, source);
        aven_gl_check_error(gl);
    }

    static inline const GLubyte *aven_glGetString(AvenGl *gl, GLenum name) {
        const GLubyte *res = gl->GetString(name);
        aven_gl_check_error(gl);
        return res;
    }

    static inline void aven_gl_GetTexParameterfv(
        AvenGl *gl,
        GLenum target,
        GLenum pname,
        GLfloat *params
    ) {
        gl->GetTexParameterfv(target, pname, params);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_GetTexParameteriv(
        AvenGl *gl,
        GLenum target,
        GLenum pname,
        GLint *params
    ) {
        gl->GetTexParameteriv(target, pname, params);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_GetUniformfv(
        AvenGl *gl,
        GLuint program,
        GLint location,
        GLfloat *params
    ) {
        gl->GetUniformfv(program, location, params);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_GetUniformiv(
        AvenGl *gl,
        GLuint program,
        GLint location,
        GLint *params
    ) {
        gl->GetUniformiv(program, location, params);
        aven_gl_check_error(gl);
    }

    static inline GLint aven_gl_GetUniformLocation(
        AvenGl *gl,
        GLuint program,
        const GLchar *name
    ) {
        GLint res = gl->GetUniformLocation(program, name);
        aven_gl_check_error(gl);
        return res;
    }

    static inline void aven_gl_GetVertexAttribfv(
        AvenGl *gl,
        GLuint index,
        GLenum pname,
        GLfloat *params
    ) {
        gl->GetVertexAttribfv(index, pname, params);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_GetVertexAttribiv(
        AvenGl *gl,
        GLuint index,
        GLenum pname,
        GLint *params
    ) {
        gl->GetVertexAttribiv(index, pname, params);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_GetVertexAttribPointerv(
        AvenGl *gl,
        GLuint index,
        GLenum pname,
        void **pointer
    ) {
        gl->GetVertexAttribPointerv(index, pname, pointer);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_Hint(AvenGl *gl, GLenum target, GLenum mode) {
        gl->Hint(target, mode);
        aven_gl_check_error(gl);
    }

    static inline GLboolean aven_gl_IsBuffer(AvenGl *gl, GLuint buffer) {
        GLboolean res = gl->IsBuffer(buffer);
        aven_gl_check_error(gl);
        return res;
    }

    static inline GLboolean aven_gl_IsEnabled(AvenGl *gl, GLenum cap) {
        GLboolean res = gl->IsEnabled(cap);
        aven_gl_check_error(gl);
        return res;
    }

    static inline GLboolean aven_gl_IsFramebuffer(
        AvenGl *gl,
        GLuint framebuffer
    ) {
        GLboolean res = gl->IsFramebuffer(framebuffer);
        aven_gl_check_error(gl);
        return res;
    }

    static inline GLboolean aven_gl_IsProgram(AvenGl *gl, GLuint program) {
        GLboolean res = gl->IsProgram(program);
        aven_gl_check_error(gl);
        return res;
    }

    static inline GLboolean aven_gl_IsRenderbuffer(
        AvenGl *gl,
        GLuint renderbuffer
    ) {
        GLboolean res = gl->IsRenderbuffer(renderbuffer);
        aven_gl_check_error(gl);
        return res;
    }

    static inline GLboolean aven_gl_IsShader(AvenGl *gl, GLuint shader) {
        GLboolean res = gl->IsShader(shader);
        aven_gl_check_error(gl);
        return res;
    }

    static inline GLboolean aven_gl_IsTexture(AvenGl *gl, GLuint texture) {
        GLboolean res = gl->IsTexture(texture);
        aven_gl_check_error(gl);
        return res;
    }

    static inline void aven_gl_LineWidth(AvenGl *gl, GLfloat width) {
        gl->LineWidth(width);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_LinkProgram(AvenGl *gl, GLuint program) {
        gl->LinkProgram(program);
        GLint linked = 0;
        gl->GetProgramiv(program, GL_LINK_STATUS, &linked);
        if (!linked) {
            GLint info_len = 0;
            gl->GetProgramiv(program, GL_INFO_LOG_LENGTH, &info_len);

            char info_buffer[4096];
            if (info_len > 0 and (size_t)info_len <= sizeof(info_buffer)) {
                AvenStr info_tot = slice_array(info_buffer);
                AvenStr info = aven_str_head(info_tot, (size_t)info_len);
                gl->GetProgramInfoLog(program, info_len, NULL, info.ptr);
                aven_io_perrf("error linking program: {}", aven_fmt_str(info));
            }
            aven_panic("failed to link program");
        }
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_PixelStorei(
        AvenGl *gl,
        GLenum pname,
        GLint param
    ) {
        gl->PixelStorei(pname, param);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_PolygonOffset(
        AvenGl *gl,
        GLfloat factor,
        GLfloat units
    ) {
        gl->PolygonOffset(factor, units);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_ReadPixels(
        AvenGl *gl,
        GLint x,
        GLint y,
        GLsizei width,
        GLsizei height,
        GLenum format,
        GLenum type,
        void *pixels
    ) {
        gl->ReadPixels(x, y, width, height, format, type, pixels);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_ReleaseShaderCompiler(AvenGl *gl) {
        gl->ReleaseShaderCompiler();
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_RenderbufferStorage(
        AvenGl *gl,
        GLenum target,
        GLenum internalformat,
        GLsizei width,
        GLsizei height
    ) {
        gl->RenderbufferStorage(target, internalformat, width, height);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_SampleCoverage(
        AvenGl *gl,
        GLfloat value,
        GLboolean invert
    ) {
        gl->SampleCoverage(value, invert);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_Scissor(
        AvenGl *gl,
        GLint x,
        GLint y,
        GLsizei width,
        GLsizei height
    ) {
        gl->Scissor(x, y, width, height);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_ShaderBinary(
        AvenGl *gl,
        GLsizei count,
        const GLuint *shaders,
        GLenum binaryformat,
        const void *binary,
        GLsizei length
    ) {
        gl->ShaderBinary(count, shaders, binaryformat, binary, length);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_ShaderSource(
        AvenGl *gl,
        GLuint shader,
        GLsizei count,
        const GLchar *const *string,
        const GLint *length
    ) {
        gl->ShaderSource(shader, count, string, length);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_StencilFunc(
        AvenGl *gl,
        GLenum func,
        GLint ref,
        GLuint mask
    ) {
        gl->StencilFunc(func, ref, mask);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_StencilFuncSeparate(
        AvenGl *gl,
        GLenum face,
        GLenum func,
        GLint ref,
        GLuint mask
    ) {
        gl->StencilFuncSeparate(face, func, ref, mask);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_StencilMask(AvenGl *gl, GLuint mask) {
        gl->StencilMask(mask);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_StencilMaskSeparate(
        AvenGl *gl,
        GLenum face,
        GLuint mask
    ) {
        gl->StencilMaskSeparate(face, mask);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_StencilOp(
        AvenGl *gl,
        GLenum fail,
        GLenum zfail,
        GLenum zpass
    ) {
        gl->StencilOp(fail, zfail, zpass);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_StencilOpSeparate(
        AvenGl *gl,
        GLenum face,
        GLenum sfail,
        GLenum dpfail,
        GLenum dppass
    ) {
        gl->StencilOpSeparate(face, sfail, dpfail, dppass);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_TexImage2D(
        AvenGl *gl,
        GLenum target,
        GLint level,
        GLint internalformat,
        GLsizei width,
        GLsizei height,
        GLint border,
        GLenum format,
        GLenum type,
        const void *pixels
    ) {
        gl->TexImage2D(
            target,
            level,
            internalformat,
            width,
            height,
            border,
            format,
            type,
            pixels
        );
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_TexParameterf(
        AvenGl *gl,
        GLenum target,
        GLenum pname,
        GLfloat param
    ) {
        gl->TexParameterf(target, pname, param);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_TexParameterfv(
        AvenGl *gl,
        GLenum target,
        GLenum pname,
        const GLfloat *params
    ) {
        gl->TexParameterfv(target, pname, params);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_TexParameteri(
        AvenGl *gl,
        GLenum target,
        GLenum pname,
        GLint param
    ) {
        gl->TexParameteri(target, pname, param);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_TexParameteriv(
        AvenGl *gl,
        GLenum target,
        GLenum pname,
        const GLint *params
    ) {
        gl->TexParameteriv(target, pname, params);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_TexSubImage2D(
        AvenGl *gl,
        GLenum target,
        GLint level,
        GLint xoffset,
        GLint yoffset,
        GLsizei width,
        GLsizei height,
        GLenum format,
        GLenum type,
        const void *pixels
    ) {
        gl->TexSubImage2D(
            target,
            level,
            xoffset,
            yoffset,
            width,
            height,
            format,
            type,
            pixels
        );
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_Uniform1f(AvenGl *gl, GLint location, GLfloat v0) {
        gl->Uniform1f(location, v0);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_Uniform1fv(
        AvenGl *gl,
        GLint location,
        GLsizei count,
        const GLfloat *value
    ) {
        gl->Uniform1fv(location, count, value);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_Uniform1i(AvenGl *gl, GLint location, GLint v0) {
        gl->Uniform1i(location, v0);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_Uniform1iv(
        AvenGl *gl,
        GLint location,
        GLsizei count,
        const GLint *value
    ) {
        gl->Uniform1iv(location, count, value);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_Uniform2f(
        AvenGl *gl,
        GLint location,
        GLfloat v0,
        GLfloat v1
    ) {
        gl->Uniform2f(location, v0, v1);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_Uniform2fv(
        AvenGl *gl,
        GLint location,
        GLsizei count,
        const GLfloat *value
    ) {
        gl->Uniform2fv(location, count, value);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_Uniform2i(
        AvenGl *gl,
        GLint location,
        GLint v0,
        GLint v1
    ) {
        gl->Uniform2i(location, v0, v1);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_Uniform2iv(
        AvenGl *gl,
        GLint location,
        GLsizei count,
        const GLint *value
    ) {
        gl->Uniform2iv(location, count, value);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_Uniform3f(
        AvenGl *gl,
        GLint location,
        GLfloat v0,
        GLfloat v1,
        GLfloat v2
    ) {
        gl->Uniform3f(location, v0, v1, v2);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_Uniform3fv(
        AvenGl *gl,
        GLint location,
        GLsizei count,
        const GLfloat *value
    ) {
        gl->Uniform3fv(location, count, value);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_Uniform3i(
        AvenGl *gl,
        GLint location,
        GLint v0,
        GLint v1,
        GLint v2
    ) {
        gl->Uniform3i(location, v0, v1, v2);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_Uniform3iv(
        AvenGl *gl,
        GLint location,
        GLsizei count,
        const GLint *value
    ) {
        gl->Uniform3iv(location, count, value);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_Uniform4f(
        AvenGl *gl,
        GLint location,
        GLfloat v0,
        GLfloat v1,
        GLfloat v2,
        GLfloat v3
    ) {
        gl->Uniform4f(location, v0, v1, v2, v3);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_Uniform4fv(
        AvenGl *gl,
        GLint location,
        GLsizei count,
        const GLfloat *value
    ) {
        gl->Uniform4fv(location, count, value);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_Uniform4i(
        AvenGl *gl,
        GLint location,
        GLint v0,
        GLint v1,
        GLint v2,
        GLint v3
    ) {
        gl->Uniform4i(location, v0, v1, v2, v3);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_Uniform4iv(
        AvenGl *gl,
        GLint location,
        GLsizei count,
        const GLint *value
    ) {
        gl->Uniform4iv(location, count, value);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_UniformMatrix2fv(
        AvenGl *gl,
        GLint location,
        GLsizei count,
        GLboolean transpose,
        const GLfloat *value
    ) {
        gl->UniformMatrix2fv(location, count, transpose, value);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_UniformMatrix3fv(
        AvenGl *gl,
        GLint location,
        GLsizei count,
        GLboolean transpose,
        const GLfloat *value
    ) {
        gl->UniformMatrix3fv(location, count, transpose, value);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_UniformMatrix4fv(
        AvenGl *gl,
        GLint location,
        GLsizei count,
        GLboolean transpose,
        const GLfloat *value
    ) {
        gl->UniformMatrix4fv(location, count, transpose, value);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_UseProgram(AvenGl *gl, GLuint program) {
        gl->UseProgram(program);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_ValidateProgram(AvenGl *gl, GLuint program) {
        gl->ValidateProgram(program);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_VertexAttrib1f(
        AvenGl *gl,
        GLuint index,
        GLfloat x
    ) {
        gl->VertexAttrib1f(index, x);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_VertexAttrib1fv(
        AvenGl *gl,
        GLuint index,
        const GLfloat *v
    ) {
        gl->VertexAttrib1fv(index, v);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_VertexAttrib2f(
        AvenGl *gl,
        GLuint index,
        GLfloat x,
        GLfloat y
    ) {
        gl->VertexAttrib2f(index, x, y);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_VertexAttrib2fv(
        AvenGl *gl,
        GLuint index,
        const GLfloat *v
    ) {
        gl->VertexAttrib2fv(index, v);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_VertexAttrib3f(
        AvenGl *gl,
        GLuint index,
        GLfloat x,
        GLfloat y,
        GLfloat z
    ) {
        gl->VertexAttrib3f(index, x, y, z);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_VertexAttrib3fv(
        AvenGl *gl,
        GLuint index,
        const GLfloat *v
    ) {
        gl->VertexAttrib3fv(index, v);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_VertexAttrib4f(
        AvenGl *gl,
        GLuint index,
        GLfloat x,
        GLfloat y,
        GLfloat z,
        GLfloat w
    ) {
        gl->VertexAttrib4f(index, x, y, z, w);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_VertexAttrib4fv(
        AvenGl *gl,
        GLuint index,
        const GLfloat *v
    ) {
        gl->VertexAttrib4fv(index, v);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_VertexAttribPointer(
        AvenGl *gl,
        GLuint index,
        GLint size,
        GLenum type,
        GLboolean normalized,
        GLsizei stride,
        const void *pointer
    ) {
        gl->VertexAttribPointer(index, size, type, normalized, stride, pointer);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_Viewport(
        AvenGl *gl,
        GLint x,
        GLint y,
        GLsizei width,
        GLsizei height
    ) {
        gl->Viewport(x, y, width, height);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_BindVertexArray(AvenGl *gl, GLuint array) {
        gl->BindVertexArray(array);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_GenVertexArrays(
        AvenGl *gl,
        GLsizei n,
        GLuint *arrays
    ) {
        gl->GenVertexArrays(n, arrays);
        aven_gl_check_error(gl);
    }

    static inline void aven_gl_DeleteVertexArrays(
        AvenGl *gl,
        GLsizei n,
        const GLuint *arrays
    ) {
        gl->DeleteVertexArrays(n, arrays);
        aven_gl_check_error(gl);
    }

    static inline GLboolean aven_gl_IsVertexArray(AvenGl *gl, GLuint array) {
        GLboolean res = gl->IsVertexArray(array);
        aven_gl_check_error(gl);
        return res;
    }

    static inline void aven_gl_DebugMessageCallback(
        AvenGl *gl,
        GLDEBUGPROC callback,
        const void *userParam
    ) {
        gl->DebugMessageCallback(callback, userParam);
        aven_gl_check_error(gl);
    }

    typedef enum {
        AVEN_GL_BUFFER_USAGE_STATIC = GL_STATIC_DRAW,
        AVEN_GL_BUFFER_USAGE_DYNAMIC = GL_DYNAMIC_DRAW,
        AVEN_GL_BUFFER_USAGE_STREAM = GL_STREAM_DRAW,
    } AvenGlBufferUsage;
#endif // AVEN_GL_H
