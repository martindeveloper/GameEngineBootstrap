#pragma once

#ifndef __GlobalHeaders
#define __GlobalHeaders

#define VERTEX_HAVE_UV 1
#undef VERTEX_HAVE_NORMAL

#define IS_DEBUG 1

#if 0
#define VLD_FORCE_ENABLE 1
#include <vld.h>
#endif

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <windows.h>

#include <exception>
#include <cmath>
#include <iostream>
#include <map>
#include <vector>
#include <unordered_map>
#include <assert.h>

#include <string>
#include <fstream>
#include <streambuf>

#include "../External/glm/glm.hpp"
#include "../External/glm/gtx/transform.hpp"

#include "Types.h"
#include "Core/IResource.h"

#include "Core/Log.h"

#include "Renderer/Material.h"
#include "Renderer/IWindowRenderer.h"
#include "Window.h"

#endif