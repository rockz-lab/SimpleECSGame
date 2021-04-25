#pragma once

#include <set>
#include <unordered_map>
#include <memory>

#include "ecsTypes.h"



class System
{
public:
	// needs some sort of name acess
	std::vector<std::set<eID>> m_entitiyLists;
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
	void SetSignature(Signature newSignature)
	{
		const char* strID = typeid(S).name();

		assert(m_systems.find(strID) != m_systems.end() && "System has not yet been registered!");

		// initialize empty entity List
		m_systems.find(strID)->second->m_entitiyLists.push_back({});

		// Add a signature to the system if it didn't exist before
		std::unordered_map<const char*, signatureList>::iterator signatures = m_system_signatures.find(strID);
		if (signatures == m_system_signatures.end())
		{
			m_system_signatures.insert({ strID, {newSignature} });
			return;	// we're done
		}

		// ckeck for existence in List of signatures -> linear search in vector propably ok, because number of signatures will generally low
		for (auto const& signature : signatures->second)
		{
			if (signature == newSignature)
				return;	// we already have that
		}
		// when not present, add the new signature
		signatures->second.push_back(newSignature);
	}

    void OnEntityDestroyed(eID entity)
	{
		for (auto const& sys : m_systems)
		{
			for (auto& entities : sys.second->m_entitiyLists)
				entities.erase(entity);
			
		}
	}
	
	void OnEntitySignatureChanged(eID entity, const Signature entitySignature)
	{
		// Loop over all Systems / signatures and remove/add the entitties according to the signature
		for (auto const& sig : m_system_signatures)
		{
			auto const& sys = m_systems[sig.first];
			auto const& sysSignatureList = sig.second;

			assert(sysSignatureList.size() == sys->m_entitiyLists.size() && "number of system signatures and system entity Lists does not match. Something went very wrong here.");

			// Iterators for looping
			auto sysSignature = sysSignatureList.begin();
			auto entityList = sys->m_entitiyLists.begin();

			for (; sysSignature != sysSignatureList.end(); ++sysSignature, ++entityList)
			{
				// check if the entity has all the components of the system -> This is solved with the bitwise and
				if ((*sysSignature & entitySignature) == *sysSignature)
				{
					// insert entitiy into System
					entityList->insert(entity);
				}
				else // No matching signature
				{
					entityList->erase(entity);
				}
			}
		}
	}

private:
	std::unordered_map<const char*, std::shared_ptr<System>> m_systems;

	using signatureList = std::vector<Signature>;
	std::unordered_map<const char* , signatureList> m_system_signatures;

};

