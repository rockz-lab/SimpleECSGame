#pragma once
#include "ecsTypes.h"

#include <unordered_map>
#include <cassert>
#include <memory>
#include <typeinfo>

#include "yaml-cpp/yaml.h"

// Components derive from this class

class BaseComponent
{
public:
	
protected:
	static CompType nextID()
	{
		return m_counter++;
	};
	static CompType m_counter;

};



template <typename T>
struct Component : public BaseComponent
{
	static const CompType ID;

	void serialize_impl(std::ostream& os)
	{
		static_cast<T*>(this)->serialize_impl(os);
	}
};

// ID definition -> auto increment, so that each Component Type has it's unique ID
template <typename T>
const CompType Component<T>::ID = BaseComponent::nextID();

// virtual base class required the acess the EntityDestroyed method
class IComponentArray
{
public:
	virtual ~IComponentArray() = default;
    virtual void OnEntityDestroyed(eID entity) = 0;
};

template <typename T>
class ComponentArray : public IComponentArray
{
public:
	ComponentArray()
	{
		m_entityToIndex.fill(InvalidIndex);
		m_indexToEntity.fill(InvalidIndex);
	}
	void Insert(eID entity, T compData)
	{
		assert(m_entityToIndex[entity] == InvalidIndex && "Failed to add Component. There can only be one component of each type!");

		// Insert at the end
		m_entityToIndex[entity] = m_size;

		m_indexToEntity[m_size] = entity;

		m_components[m_size] = compData;

		// Grow the used size of the Array
		m_size++;
	}

	void Remove(eID entity)
	{
		assert(m_entityToIndex[entity] != InvalidIndex && "Component to remove does not exist.");

		// Remove from array index.

		auto originalIndex = m_entityToIndex[entity];
		auto lastIndex = m_size-1;

		// copy entity at last Index
		T copiedComp = m_components[lastIndex];

		// Move the element at the end into the free array index
		m_components[originalIndex] = copiedComp;
		//m_components[lastIndex] = InvalidIndex;

		// Update the maps
		auto lastIndexEntity = m_indexToEntity[lastIndex];
		auto originalIndexEntity = m_indexToEntity[originalIndex];

		m_entityToIndex[lastIndexEntity] = originalIndex;
		m_entityToIndex[originalIndexEntity] = InvalidIndex;

		m_indexToEntity[originalIndex] = lastIndexEntity;
		m_indexToEntity[lastIndex] = InvalidIndex;

		m_size--;
	}

	T& GetCompData(eID entity)
	{
		assert(m_entityToIndex[entity] != InvalidIndex && "Component to retrieve data from does not exist.");

		// Look up through array
		auto index = m_entityToIndex[entity];
		return m_components[index];
	}

    virtual void OnEntityDestroyed(eID entity) override final
	{
		// remove the component of the destroyed id, if existing
		if (m_entityToIndex[entity] != InvalidIndex)
			Remove(entity);
	}

private:
	// Array for all the components of an Component Type T. Size is bounded by the max number of Entities (an Entity can only have once instance of each component)
	std::array<T, maxEntities> m_components{};

	std::array<eID, maxEntities> m_entityToIndex{};
	std::array<eID, maxEntities> m_indexToEntity{};

	eID m_size = 0;
}; 



class CompManager
{
public:

	
	// probably not even needed anymore

	template <typename T>
	void RegisterComponent()
	{
		// TODO: assert stuff

		//m_compTypes.push_back()

		m_compArrays[T::ID] = std::make_shared<ComponentArray<T>>();
		m_numCompArrays++;
	}

	template <typename T>
	CompType GetCompType()
	{
		//const char* strID = typeid(T).name();

		//assert(m_compTypes.find(strID) != m_compTypes.end() && "Component does not exist!");

		return T::ID;
	}

	template <typename T>
	T& GetComponent(eID entity)
	{
		return getCompArray<T>()->GetCompData(entity);
	}

	void GetComponent(eID entity, CompType compID)
	{
		typeid(m_compArrays[compID]);
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

    void OnEntityDestroyed(eID entity)
    {
        for (CompType cmp = 0; cmp < m_numCompArrays; cmp++)
        {
			m_compArrays[cmp]->OnEntityDestroyed(entity);
        }
    }
	
	CompType GetNumCompArrays()
	{
		return m_numCompArrays;
	}
private:

	std::array<std::shared_ptr<IComponentArray>, maxCompType> m_compArrays{};
	CompType m_numCompArrays{};
	// just for book keeping/bounds checking
	std::vector<CompType> m_compTypes{};

	// To make comp Array access more convenient
	template <typename T>
	std::shared_ptr<ComponentArray<T>> getCompArray()
	{
		//const char* strID = typeid(T).name();
		assert(m_compArrays[T::ID]);
		// Component Types "know" about their IDs, so the components can be found efficiently
		return std::static_pointer_cast<ComponentArray<T>> (m_compArrays[T::ID]);
	}
};
