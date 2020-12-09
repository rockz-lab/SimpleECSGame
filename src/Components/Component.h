#pragma once
#include "ecsTypes.h"

#include <unordered_map>
#include <cassert>
#include <memory>
#include <typeinfo>

// virtual base class required the acess the EntityDestroyed method
class IComponentArray
{
public:
	virtual ~IComponentArray() = default;
	virtual void EntityDestroyed(eID entity) = 0;
};

template <typename T>
class ComponentArray : public IComponentArray
{
public:

	void Insert(eID entity, T compData)
	{
		assert(m_entityToIndex.find(entity) == m_entityToIndex.end() && "Failed to add Component. There can only be one component of each type!");

		// Insert at the end
		m_entityToIndex[entity] = m_size;
		m_indexToEntity[m_size] = entity;

		m_components[m_size] = compData;

		// Grow the used size of the Array
		m_size++;
	}

	void Remove(eID entity)
	{
		assert(m_entityToIndex.find(entity) != m_entityToIndex.end() && "Component to remove does not exist.");

		// Remove from array index.

		auto removedEntityIndex = m_entityToIndex[entity];
		auto lastIndex = m_size - 1;
		// copy last Index
		T copiedComp = m_components[lastIndex];

		// Move the element at the end into the free array index
		m_components[removedEntityIndex] = copiedComp;

		// Update the maps
		eID lastIndexEntity = m_indexToEntity[lastIndex];
		m_indexToEntity[removedEntityIndex] = lastIndexEntity;
		m_entityToIndex[lastIndexEntity] = removedEntityIndex;

		m_entityToIndex.erase(entity);
		m_indexToEntity.erase(lastIndex);

		m_size--;
	}


	T& GetCompData(eID entity)
	{
		assert(m_entityToIndex.find(entity) != m_entityToIndex.end() && "Component to retrieve data from does not exist.");

		// Look up through map
		auto index = m_entityToIndex[entity];
		return m_components[index];
	}

	virtual void EntityDestroyed(eID entity) override final
	{
		// remove the component of the destroyed id, if existing
		if (m_entityToIndex.find(entity) != m_entityToIndex.end())
			Remove(entity);
	}

private:
	// Array for all the components of an Enitity Type T. Size is bounded by the max number of Entities (an Entity can only have once instance of each component)
	std::array<T, maxEntities> m_components;
	std::unordered_map<eID, eID> m_entityToIndex;
	std::unordered_map<eID, eID> m_indexToEntity;

	eID m_size = 0;
}; 



class CompManager
{
public:
	template <typename T>
	void RegisterComponent()
	{
		const char* strID = typeid(T).name();

		assert(m_compTypes.find(strID) == m_compTypes.end() && "Component has already been registered");

		m_compTypes.insert({ strID, m_nextCompType });
		m_compArrays.insert({ strID, std::make_shared<ComponentArray<T>>() });


		m_nextCompType++;
	}



	template <typename T>
	CompType GetCompType()
	{
		const char* strID = typeid(T).name();

		assert(m_compTypes.find(strID) != m_compTypes.end() && "Component does not exist!");

		return m_compTypes[strID];
	}

	template <typename T>
	T& GetComponent(eID entity)
	{
		return getCompArray<T>()->GetCompData(entity);
	}

	template <typename T>
	void AddComponent(eID entity, T component)
	{
		getCompArray<T>()->Insert(entity, component);
	}
	
	template <typename T>
	void RemoveComponent(eID entity)
	{
		getCompArray<T>()->Remove(entity);
	}

private:
	CompType m_nextCompType = 0;
	std::unordered_map<const char*, CompType> m_compTypes{};
	std::unordered_map<const char*, std::shared_ptr<IComponentArray>> m_compArrays{};

	// To make comp Array access more convenient
	
	template <typename T>
	std::shared_ptr<ComponentArray<T>> getCompArray()
	{
		const char* strID = typeid(T).name();

		return std::static_pointer_cast<ComponentArray<T>> (m_compArrays[strID]);
	}
};