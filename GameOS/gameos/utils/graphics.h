#pragma once

#include <glad/gl.h>

#ifndef PLATFORM_WINDOWS
// because APIENTRY is undefined at the end of glew.h and we use it in shader_builder.h
#ifndef APIENTRY
#define APIENTRY
#endif
#else
#include <windows.h>
#endif
