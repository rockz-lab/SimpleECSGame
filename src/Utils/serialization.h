#include "ECS/ecs.h"
#include <iostream>
#include <string_view>

std::vector<std::string> getProperty(std::istream& fs, const std::string name)
{
	std::string buffer;
	std::getline(fs, buffer);

	auto start = buffer.find(name);

	if (start == std::string::npos)
	{
		std::cerr << "The requested position was not found." << std::endl;
	}

	start += name.length() + 1;
	
	std::string_view remaining = std::string_view{ buffer }.substr(start);
	auto end = remaining.find_first_of(";");

	std::string_view property = remaining.substr(0, end);
	std::vector<std::string> field_entries;

	while (true)
	{
		auto sep_index = property.find_first_of(',');

		std::string fieldContents;
		if (sep_index != std::string::npos)
		{
			fieldContents = property.substr(0, sep_index);
			property = property.substr(sep_index+1);
			field_entries.push_back(fieldContents);
		}
		else
		{
			fieldContents = property;
			field_entries.push_back(fieldContents);
			break;
		}
	}
	return field_entries;
}
