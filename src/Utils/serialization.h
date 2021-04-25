#pragma once
#include <iostream>
#include <string_view>

#include <array>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "Components/gameComponents.h"

#define TO_FIELD(name) { #name, obj.name}
#define FIELDS(...) j = json{__VA_ARGS__}

#define FROM_FIELD(name) j.at(#name).get_to(obj.name)
// vec2
inline void to_json(json& j, const vec2& obj) {
	FIELDS(
		TO_FIELD(x),
		TO_FIELD(y)
	);
}
inline void from_json(const json& j, vec2& obj) {
	FROM_FIELD(x);
	FROM_FIELD(y);
}

// Transform2D
inline void to_json(json& j, const Transform2D& obj) {
	FIELDS(
		TO_FIELD(pos),
		TO_FIELD(rotation)
	);
}
inline void from_json(const json& j, Transform2D& obj) {
	FROM_FIELD(pos);
	FROM_FIELD(rotation);
}

// Gravity
inline void to_json(json& j, const Gravity& obj) {
	FIELDS(
		TO_FIELD(g),
	);
}
inline void from_json(const json& j, Gravity& obj) {
	FROM_FIELD(g);
}

// Circle
inline void to_json(json& j, const Circle& obj) {
	FIELDS(
		TO_FIELD(radius),
	);
}
inline void from_json(const json& j, Circle& obj) {
	FROM_FIELD(radius);
}

//// RigidBodyState
inline void to_json(json& j, const RigidBodyState& obj) {
	FIELDS(
		TO_FIELD(mass),
		TO_FIELD(angMass),
		TO_FIELD(centerPos),
		TO_FIELD(momentum),
		TO_FIELD(rotation),
		TO_FIELD(angMomentum),
		TO_FIELD(centerPos_o),
		TO_FIELD(momentum_o),
		TO_FIELD(rotation_o),
		TO_FIELD(angMomentum_o),
		TO_FIELD(force),
		TO_FIELD(torque)
	);
}
inline void from_json(const json& j, RigidBodyState& obj) {
	FROM_FIELD(mass);
	FROM_FIELD(angMass);
	FROM_FIELD(centerPos);
	FROM_FIELD(momentum);
	FROM_FIELD(rotation);
	FROM_FIELD(angMomentum);
	FROM_FIELD(centerPos_o);
	FROM_FIELD(momentum_o);
	FROM_FIELD(rotation_o);
	FROM_FIELD(angMomentum_o);
	FROM_FIELD(force);
	FROM_FIELD(torque);
}

inline void to_json(json& j, const Color& obj)
{
	FIELDS(
		TO_FIELD(r),
		TO_FIELD(g),
		TO_FIELD(b)
		);
}
inline void from_json(const json& j, Color& obj) {
	FROM_FIELD(r);
	FROM_FIELD(g);
	FROM_FIELD(b);
}

inline void to_json(json& j, Line& obj)
{
	FIELDS(
		TO_FIELD(p1),
		TO_FIELD(p2)
	);
}
inline void from_json(const json& j, Line& obj) {
	FROM_FIELD(p1);
	FROM_FIELD(p2);
}

template <int N>
inline void to_json(json& j, const Static_Poly<N>& obj)
{
	FIELDS(
		TO_FIELD(normals),
		TO_FIELD(vertices)
	);
}

template <int N>
inline void from_json(const json& j, Static_Poly<N>& obj)
{
	FROM_FIELD(normals);
	FROM_FIELD(vertices);
}


template <int N>
inline void to_json(json& j, const Polygon<N>& obj)
{
	FIELDS(
		TO_FIELD(vertexData)
	);
}


template <int N>
inline void from_json(const json& j, Polygon<N>& obj)
{
	FROM_FIELD(vertexData);
}

inline void to_json(json& j, const CollisionState& obj)
{
	FIELDS(
		TO_FIELD(status)
		);
}
inline void from_json(const json& j, CollisionState& obj) {
	FROM_FIELD(status);
}


template <int N>
inline void to_json(json& j, const TexCoords<N>& obj)
{
	FIELDS( TO_FIELD(coords) );
}
template <int N>
inline void from_json(const json& j, TexCoords<N>& obj)
{
	FROM_FIELD(coords);
}

//
//class Reader
//{
//public:
//
//	enum class LineState
//	{
//		EntityID,
//		CompData
//	};
//
//	Reader(const std::string fileName)
//	{
//		is = std::make_unique<std::ifstream>();
//		is->open(fileName);
//	}
//
//	eID getEntityID()
//	{
//		std::string line = GetCurrentLine();
//		std::string searchString = "Entity ID:";
//		int start = line.find(searchString) + searchString.length()+1;
//		eID entity = std::stoi(line.substr(start));
//		return entity;
//	}
//
//	eID GetCompID()
//	{
//		std::string line = GetCurrentLine();
//		std::string searchString = "Type:";
//
//		auto index = line.find(searchString);
//		assert(index = std::string::npos && "No Type identifier found in this line.");
//		int start = index + searchString.length();
//		int end = line.find(";");
//		CompType compID = std::stoi(line.substr(start, end - start));
//		return compID;
//	}
//
//	std::vector<std::string> GetProperty(const std::string name)
//	{
//		std::string buffer = GetCurrentLine();
//
//		auto start = buffer.find(name);
//
//		if (start == std::string::npos)
//		{
//			std::cerr << "The requested position was not found." << std::endl;
//		}
//
//		start += name.length() + 1;
//
//		std::string_view remaining = std::string_view{ buffer }.substr(start);
//		auto end = remaining.find_first_of(";");
//
//		std::string_view property = remaining.substr(0, end);
//		std::vector<std::string> field_entries;
//
//		while (true)
//		{
//			auto sep_index = property.find_first_of(',');
//
//			std::string fieldContents;
//			if (sep_index != std::string::npos)
//			{
//				fieldContents = property.substr(0, sep_index);
//				property = property.substr(sep_index + 1);
//				field_entries.push_back(fieldContents);
//			}
//			else
//			{
//				fieldContents = property;
//				field_entries.push_back(fieldContents);
//				break;
//			}
//		}
//		return field_entries;
//	}
//
//	std::string GetCurrentLine()
//	{
//		std::string line;
//
//		prev_pos = is->tellg();
//		std::getline(*is, line);
//		curr_pos = is->tellg();
//		is->seekg(prev_pos);
//
//		return line;
//	}
//
//	void NextLine()
//	{
//		GetCurrentLine();
//		is->seekg(curr_pos);
//		prev_pos = curr_pos;
//	}
//
//	bool IsDone()
//	{
//		return GetCurrentLine() == "";
//	}
//
//	LineState GetLineState()
//	{
//		std::string line = GetCurrentLine();
//
//		auto index = line.find("Entity ID");
//		if (index != std::string::npos)
//			return LineState::EntityID;
//		else
//			return LineState::CompData;
//	}
//
//private:
//	int curr_pos{};
//	int prev_pos{};
//
//	std::unique_ptr<std::ifstream> is;
//};

