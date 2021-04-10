#pragma once
#include <iostream>
#include <string_view>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "Components/gameComponents.h"

inline void to_json(json& j, const vec2& obj) {
	j = json{ {"x", obj.x}, {"y", obj.y} };
}
inline void from_json(const json& j, vec2& obj) {
	j.at("x").get_to(obj.x);
	j.at("y").get_to(obj.y);
}

inline void to_json(json& j, const Transform2D& obj) {
	j = json{ {"pos", obj.pos}, {"rotation", obj.rotation} };
}
inline void from_json(const json& j, Transform2D& obj) {
	j.at("pos").get_to(obj.pos);
	j.at("rotation").get_to(obj.rotation);
}

inline void to_json(json& j, const Gravity& obj) {
	j = json{ {"g", obj.g} };
}
inline void from_json(const json& j, Gravity& obj) {
	j.at("g").get_to(obj.g);
}

inline void to_json(json& j, const Circle& obj) {
	j = json{ {"radius", obj.radius} };
}
inline void from_json(const json& j, Circle& obj) {
	j.at("radius").get_to(obj.radius);
}

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

		auto index = line.find(searchString);
		assert(index = std::string::npos && "No Type identifier found in this line.");
		int start = index + searchString.length();
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
		GetCurrentLine();
		is->seekg(curr_pos);
		prev_pos = curr_pos;
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

