#pragma once

#include <string>
#include <string.h>

void stripFileEnding(std::string& str)
{
	auto ending = str.rfind('.');
	if (ending != std::string::npos)
	{
		str = str.substr(0, ending);
	}
}

void copyString(const std::string& input, char* dst, size_t dst_size)
{
	strncpy(dst, input.c_str(), dst_size - 1);
	dst[dst_size - 1] = '\0';
}
