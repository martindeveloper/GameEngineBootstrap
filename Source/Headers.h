#pragma once

#ifndef __GlobalHeaders
#define __GlobalHeaders

#include <windows.h>

#include <exception>
#include <cmath>
#include <iostream>
#include <vector>
#include <assert.h>

#include <string>
#include <fstream>
#include <streambuf>

#include "../External/glm/glm.hpp"
#include "../External/glm/gtx/transform.hpp"

#include "FileSystem/File.h"
#include "FileSystem/Image.h"
#include "FileSystem/ImageBMP.h"

#include "Graphic/ColorRGBA.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/Matrix4x4.h"
#include "Graphic/Vertex.h"
#include "Graphic/Primitive/TrianglePrimitive.h"
#include "Graphic/Primitive/CubePrimitive.h"
#include "Graphic/ConstantBuffer.h"

#include "Renderer/IWindowRenderer.h"
#include "Window.h"

#include "../External/glew/inc/glew/glew.h"

#include <gl/GL.h>
#include <gl/GLU.h>

#include "Renderer/OpenGLRenderer.h"

#include "Renderer/D3D11Renderer.h"

#endif