#pragma once

#include <deque>
#include <array>
#include <vector>

#include "ECSTypes.h"

#include <iostream>

class EntityManager
{
public:
	EntityManager()
	{
		// init the queue
		for (eID i = 0; i < maxEntities; i++)
		{
			m_availableIDs.push_back(i);
		}
	}
	eID CreateEntity()
	{
		eID entity = m_availableIDs.front();
		m_availableIDs.pop_front();

		m_activeEntities.set(entity);
		return entity;
	}

	void DestroyEntity(eID entity)
	{
		m_availableIDs.push_back(entity); // so you can later resuse IDs that have been deleted
		m_activeEntities.set(entity, 0);
	}

	void SetSignature(eID entity, Signature s)
	{
		m_signatures[entity] = s;
	}

	Signature& GetSignature(eID entity)
	{
		return m_signatures[entity];
	}

	//friend std::ostream& operator << (std::ostream& os, EntityManager& manager)
	//{
	//	const char* signaturesBegin = reinterpret_cast<const char*>(manager.m_signatures.data());
	//	size_t signaturesSize = sizeof manager.m_signatures;

	//	os << "Signatures\n";
	//	os << signaturesSize << "\n";
	//	os.write(signaturesBegin, signaturesSize);
	//	os << "\n";
	//
	//	manager.SerializeAvailableIDs(os);

	//	return os;
	//}

	std::vector<eID> GetActiveEntities()
	{
		std::vector<eID> m_activeEntitiesOut;
		for (eID i = 0; i < maxEntities; i++)
		{
			if (m_activeEntities[i])
				m_activeEntitiesOut.push_back(i);
		}
		return m_activeEntitiesOut;
	}

private:
	std::array<Signature, maxEntities> m_signatures;
	std::deque<eID> m_availableIDs;
	std::bitset<maxEntities> m_activeEntities;

	
};

