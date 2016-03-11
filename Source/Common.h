#pragma once

#ifndef __GlobalHeaders
#define __GlobalHeaders

#define VERTEX_HAVE_UV 1

#if 0
#define VLD_FORCE_ENABLE 1
#include <vld.h>
#endif

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

#include "Types.h"

#include "Renderer/IWindowRenderer.h"
#include "Window.h"

#endif