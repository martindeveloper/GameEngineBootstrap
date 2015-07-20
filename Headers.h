#pragma once

#ifndef __GlobalHeaders
#define __GlobalHeaders

#include <windows.h>

#include "glew/glew.h"

#include <gl/GL.h>
#include <gl/GLU.h>
#include <exception>
#include <cmath>
#include <iostream>
#include <vector>

#include <string>
#include <fstream>
#include <streambuf>

#include "ColorRGBA.h"
#include "Vector3.h"
#include "Vertex.h"
#include "TrianglePrimitive.h"

#include "IWindowRenderer.h"
#include "OpenGLRenderer.h"
#include "D3D11Renderer.h"
#include "Window.h"

#include "File.h"

#endif