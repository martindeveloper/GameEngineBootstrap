#pragma once

#include "../Common.h"
#include <stdarg.h>
#include <cstdio>

class Log
{
public:
	enum class Severity
	{
		Notice,
		Warning,
		Critical
	};

	static void Write(const char* module, const char* format, Severity severity, ...);
	static void WriteNotice(const char* module, const char* format, ...);

};