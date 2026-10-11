#pragma once
// OpenGL 3.3 core entry points resolved through SDL after a context is current.
#include <SDL3/SDL_opengl.h>

#define DQIV_GL11_FUNCTIONS(X) \
    X(Clear,glClear) X(ClearColor,glClearColor) X(ClearDepth,glClearDepth) X(ClearStencil,glClearStencil) \
    X(Viewport,glViewport) X(Enable,glEnable) X(Disable,glDisable) X(BlendFunc,glBlendFunc) \
    X(DepthFunc,glDepthFunc) X(DepthMask,glDepthMask) X(StencilFunc,glStencilFunc) X(StencilOp,glStencilOp) \
    X(StencilMask,glStencilMask) X(ColorMask,glColorMask) X(TexParameteri,glTexParameteri) \
    X(TexImage2D,glTexImage2D) X(TexSubImage2D,glTexSubImage2D) X(BindTexture,glBindTexture) \
    X(GenTextures,glGenTextures) X(DeleteTextures,glDeleteTextures) X(DrawArrays,glDrawArrays) \
    X(ReadPixels,glReadPixels) X(PixelStorei,glPixelStorei) X(GetError,glGetError) X(Scissor,glScissor) \
    X(GetIntegerv,glGetIntegerv) X(GetString,glGetString) X(Finish,glFinish)

#define DQIV_GL_FUNCTIONS(X) \
    X(CreateShader,PFNGLCREATESHADERPROC,glCreateShader) X(ShaderSource,PFNGLSHADERSOURCEPROC,glShaderSource) \
    X(CompileShader,PFNGLCOMPILESHADERPROC,glCompileShader) X(GetShaderiv,PFNGLGETSHADERIVPROC,glGetShaderiv) \
    X(GetShaderInfoLog,PFNGLGETSHADERINFOLOGPROC,glGetShaderInfoLog) X(DeleteShader,PFNGLDELETESHADERPROC,glDeleteShader) \
    X(CreateProgram,PFNGLCREATEPROGRAMPROC,glCreateProgram) X(AttachShader,PFNGLATTACHSHADERPROC,glAttachShader) \
    X(LinkProgram,PFNGLLINKPROGRAMPROC,glLinkProgram) X(GetProgramiv,PFNGLGETPROGRAMIVPROC,glGetProgramiv) \
    X(GetProgramInfoLog,PFNGLGETPROGRAMINFOLOGPROC,glGetProgramInfoLog) X(UseProgram,PFNGLUSEPROGRAMPROC,glUseProgram) \
    X(DeleteProgram,PFNGLDELETEPROGRAMPROC,glDeleteProgram) X(BindAttribLocation,PFNGLBINDATTRIBLOCATIONPROC,glBindAttribLocation) \
    X(BindFragDataLocation,PFNGLBINDFRAGDATALOCATIONPROC,glBindFragDataLocation) \
    X(GetUniformLocation,PFNGLGETUNIFORMLOCATIONPROC,glGetUniformLocation) X(Uniform1i,PFNGLUNIFORM1IPROC,glUniform1i) \
    X(Uniform1f,PFNGLUNIFORM1FPROC,glUniform1f) X(Uniform2f,PFNGLUNIFORM2FPROC,glUniform2f) \
    X(Uniform4f,PFNGLUNIFORM4FPROC,glUniform4f) X(Uniform1fv,PFNGLUNIFORM1FVPROC,glUniform1fv) \
    X(Uniform1iv,PFNGLUNIFORM1IVPROC,glUniform1iv) \
    X(Uniform3fv,PFNGLUNIFORM3FVPROC,glUniform3fv) X(Uniform4fv,PFNGLUNIFORM4FVPROC,glUniform4fv) \
    X(GenVertexArrays,PFNGLGENVERTEXARRAYSPROC,glGenVertexArrays) X(BindVertexArray,PFNGLBINDVERTEXARRAYPROC,glBindVertexArray) \
    X(DeleteVertexArrays,PFNGLDELETEVERTEXARRAYSPROC,glDeleteVertexArrays) X(GenBuffers,PFNGLGENBUFFERSPROC,glGenBuffers) \
    X(BindBuffer,PFNGLBINDBUFFERPROC,glBindBuffer) X(BufferData,PFNGLBUFFERDATAPROC,glBufferData) \
    X(DeleteBuffers,PFNGLDELETEBUFFERSPROC,glDeleteBuffers) X(VertexAttribPointer,PFNGLVERTEXATTRIBPOINTERPROC,glVertexAttribPointer) \
    X(VertexAttribIPointer,PFNGLVERTEXATTRIBIPOINTERPROC,glVertexAttribIPointer) \
    X(EnableVertexAttribArray,PFNGLENABLEVERTEXATTRIBARRAYPROC,glEnableVertexAttribArray) \
    X(GenFramebuffers,PFNGLGENFRAMEBUFFERSPROC,glGenFramebuffers) X(BindFramebuffer,PFNGLBINDFRAMEBUFFERPROC,glBindFramebuffer) \
    X(FramebufferTexture2D,PFNGLFRAMEBUFFERTEXTURE2DPROC,glFramebufferTexture2D) \
    X(CheckFramebufferStatus,PFNGLCHECKFRAMEBUFFERSTATUSPROC,glCheckFramebufferStatus) \
    X(DeleteFramebuffers,PFNGLDELETEFRAMEBUFFERSPROC,glDeleteFramebuffers) X(DrawBuffers,PFNGLDRAWBUFFERSPROC,glDrawBuffers) \
    X(BlitFramebuffer,PFNGLBLITFRAMEBUFFERPROC,glBlitFramebuffer) X(ActiveTexture,PFNGLACTIVETEXTUREPROC,glActiveTexture) \
    X(BlendFuncSeparate,PFNGLBLENDFUNCSEPARATEPROC,glBlendFuncSeparate) X(GenerateMipmap,PFNGLGENERATEMIPMAPPROC,glGenerateMipmap) \
    X(ClearBufferfv,PFNGLCLEARBUFFERFVPROC,glClearBufferfv) X(ClearBufferfi,PFNGLCLEARBUFFERFIPROC,glClearBufferfi) \
    X(MapBuffer,PFNGLMAPBUFFERPROC,glMapBuffer) X(UnmapBuffer,PFNGLUNMAPBUFFERPROC,glUnmapBuffer) \
    X(GenQueries,PFNGLGENQUERIESPROC,glGenQueries) X(DeleteQueries,PFNGLDELETEQUERIESPROC,glDeleteQueries) \
    X(BeginQuery,PFNGLBEGINQUERYPROC,glBeginQuery) X(EndQuery,PFNGLENDQUERYPROC,glEndQuery) \
    X(GetQueryObjectiv,PFNGLGETQUERYOBJECTIVPROC,glGetQueryObjectiv) \
    X(GetQueryObjectui64v,PFNGLGETQUERYOBJECTUI64VPROC,glGetQueryObjectui64v)

namespace pc::gl {
#define DQIV_GL11_DECLARE(name,symbol) extern decltype(&::symbol) name;
#define DQIV_GL_DECLARE(name,type,symbol) extern type name;
DQIV_GL11_FUNCTIONS(DQIV_GL11_DECLARE)
DQIV_GL_FUNCTIONS(DQIV_GL_DECLARE)
#undef DQIV_GL11_DECLARE
#undef DQIV_GL_DECLARE
// Resolves every entry point; throws when the driver lacks one.
void load();
}
