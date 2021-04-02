#pragma once
#include "ECS/ecs.h"
#include <iostream>
#include <string_view>

class Reader
{
public:

	enum class LineState
	{
		EntityID,
		CompData
	};

	Reader(const std::string fileName)
	{
		is = std::make_unique<std::ifstream>();
		is->open(fileName);
	}

	eID getEntityID()
	{
		std::string line = GetCurrentLine();
		std::string searchString = "Entity ID:";
		int start = line.find(searchString) + searchString.length()+1;
		eID entity = std::stoi(line.substr(start));
		return entity;
	}

	eID GetCompID()
	{
		std::string line = GetCurrentLine();
		std::string searchString = "Type:";
		int start = line.find(searchString) + searchString.length();
		int end = line.find(";");
		CompType compID = std::stoi(line.substr(start, end - start));
		return compID;
	}

	std::vector<std::string> GetProperty(const std::string name)
	{
		std::string buffer = GetCurrentLine();

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
				property = property.substr(sep_index + 1);
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

	std::string GetCurrentLine()
	{
		std::string line;

		prev_pos = is->tellg();
		std::getline(*is, line);
		curr_pos = is->tellg();
		is->seekg(prev_pos);

		return line;
	}

	void NextLine()
	{
		is->seekg(curr_pos);
	}

	bool IsDone()
	{
		return GetCurrentLine() == "";
	}

	LineState GetLineState()
	{
		std::string line = GetCurrentLine();

		auto index = line.find("Entity ID");
		if (index != std::string::npos)
			return LineState::EntityID;
		else
			return LineState::CompData;
	}

private:
	int curr_pos{};
	int prev_pos{};

	std::unique_ptr<std::ifstream> is;
};

