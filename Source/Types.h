#pragma once

// Global types
typedef unsigned char byte;

#if _WIN64
typedef unsigned __int64 uint;
#else
typedef unsigned __int32 uint;
#endif

typedef unsigned __int64 uint64;
typedef unsigned __int32 uint32;
typedef unsigned __int16 uint16;

// OpenGL types
typedef uint32 OpenGL4Buffer;
typedef uint32 OpenGL4ShaderProgram;
typedef uint32 OpenGL4Texture;
