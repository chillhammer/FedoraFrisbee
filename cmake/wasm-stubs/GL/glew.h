// Wasm/Emscripten stub for GLEW.
// On the desktop build, the real Vendor/Lib/GLEW/include/GL/glew.h is used.
// On the wasm build, this directory is added via -isystem so the existing
// #include <GL/glew.h> sites resolve here instead. Emscripten provides WebGL2
// via its own headers; GLEW's extension-loader machinery is unnecessary.
#pragma once

#ifndef __EMSCRIPTEN__
#error "wasm-stubs/GL/glew.h must only be reachable in the Emscripten build."
#endif

#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h>

// GLEW init is a no-op under WebGL — the context is ready as soon as GLFW returns it.
typedef int GLenum_glew_status;
#ifndef GLEW_OK
#define GLEW_OK 0
#endif
static inline int glewInit(void) { return GLEW_OK; }

// GLEW_STATIC is defined for the desktop build; harmless to leave defined here.
