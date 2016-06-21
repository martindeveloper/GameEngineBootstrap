#include "Log.h"

#define LOG_SEVERITY_TO_TEXT(X) ( (X) == Severity::Notice ? "[Notice]" : ( (X) == Severity::Warning ? "[Warning]" : "[Critical]" ) )

void Log::Write(const char* module, const char* format, Severity severity, ...)
{
#if IS_DEBUG
	constexpr uint32_t bufferSize = 512;
	constexpr uint32_t bufferSizeMinusOne = bufferSize - 1;

	va_list args;
	va_start(args, severity);

	char buffer[bufferSize];
	char finalBuffer[bufferSize];

	char* _severity = LOG_SEVERITY_TO_TEXT(severity);

	sprintf_s(buffer, "%s %s: %s\n\0", _severity, module, format);

	vsnprintf(finalBuffer, bufferSizeMinusOne, buffer, args);

	std::cout << finalBuffer << std::endl;

#if _WIN32
	OutputDebugStringA(finalBuffer);

#endif

	va_end(args);
#endif
}

void Log::WriteNotice(const char* module, const char* format, ...)
{

}

#undef LOG_SEVERITY_TO_TEXT