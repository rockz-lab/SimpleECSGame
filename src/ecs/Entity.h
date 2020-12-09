#pragma once

#include <queue>
#include <array>
#include "ecsTypes.h"

class EntityManager
{
public:
	EntityManager()
	{
		// init the queue
		for (eID i = 0; i < maxEntities; i++)
		{
			m_availableIDs.push(i);
		}
	}
	eID CreateEntity()
	{
		eID entity = m_availableIDs.front();
		m_availableIDs.pop();
		return entity;
	}

	void DestroyEntity(eID entity)
	{
		m_availableIDs.push(entity); // so you can later resuse IDs that have been deleted
	}

	void SetSignature(eID entity, Signature s)
	{
		m_signatures[entity] = s;
	}

	Signature GetSignature(eID entity)
	{
		return m_signatures[entity];
	}

private:
	std::array<Signature, maxEntities> m_signatures;
	std::queue<eID> m_availableIDs;
};

