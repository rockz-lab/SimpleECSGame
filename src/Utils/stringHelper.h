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

