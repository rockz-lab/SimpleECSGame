#pragma once

#include <set>
#include <unordered_map>
#include <memory>

#include "ecsTypes.h"



class System
{
public:
	std::set<eID> m_entities;

};


class SystemManager
{
public:
	
	template <typename S>
	std::shared_ptr<S> RegisterSystem()
	{
		const char* strID = typeid(S).name();
		auto system = std::make_shared<S>();
		m_systems.insert({ strID, system });
		return system;
	}

	template <typename S>
	void SetSignature(Signature signature)
	{
		const char* strID = typeid(S).name();

		assert(m_systems.find(strID) != m_systems.end() && "System has not yet been registered!");
		m_signatures.insert({ strID, signature });
	}

    void OnEntityDestroyed(eID entity)
	{
		for (auto const& sys : m_systems)
		{
			sys.second->m_entities.erase(entity);
			
		}
	}
	
	void OnEntitySignatureChanged(eID entity, Signature entitySignature)
	{
		// Loop over all Systems / signatures and remove/add the entitties according to the signature
		for (auto const& sig : m_signatures)
		{
			auto const& sys = m_systems[sig.first];
			auto const& sysSignature = sig.second;
			// check if the entity has all the components of the system -> This is solved with the bitwise and
			if ((sysSignature & entitySignature) == sysSignature)
			{
				// insert entitiy into System
				sys->m_entities.insert(entity);
			}
			else // No matching signature
			{
				sys->m_entities.erase(entity);
			}
		}
	}

private:
	std::unordered_map<const char*, std::shared_ptr<System>> m_systems;
	std::unordered_map<const char* , Signature> m_signatures;

};

