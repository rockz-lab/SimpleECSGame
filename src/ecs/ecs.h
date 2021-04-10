#pragma once


#include "Entity.h"
#include "Component.h"
#include "System.h"

#include <fstream>

//#include "nlohmann/json.hpp"
//using json = nlohmann::json;

#include "Utils/Serialization.h"

class ECSManager
{
public:
	void Init()
	{
		// Create pointers to each manager

		m_systemManager = std::make_unique<SystemManager>();
		m_compManager = std::make_unique<CompManager>();
		m_entityManager = std::make_unique<EntityManager>();
	}
	//~ECSManager() = default;

	template <typename ... CompTypes>
	void Serialize(const std::string& fileName)
	{

		json outJson;

		auto allEntities = m_entityManager->GetActiveEntities();

		for (auto entity : allEntities)
		{
			m_numSerializedCompTypes = 0;

			json entityJson;
			entityJson["ID"] = entity;

			//file << "Entity ID: ";
			//file << entity << "\n";
			
			json componentsJson;
			SerializeComponents<CompTypes...>(entity, componentsJson);
			entityJson["Components"] = componentsJson;
			outJson.push_back(entityJson);
		}
		
		assert(m_compManager->GetNumCompArrays() == m_numSerializedCompTypes && "Please check if all the types in Serialize match the registered types!");
		std::ofstream file;
		file.open(fileName);

		file << outJson.dump(4);
	}
	
	template <typename ...CompTypes>
	void Deserialize(const std::string& fileName)
	{
		// parse the Entities:
		
		std::ifstream file(fileName);
		json inputJson;
		file >> inputJson;
		
		eID current_entity = InvalidIndex;
		
		for (auto& entityJson : inputJson)
		{
			current_entity = entityJson["ID"];
			
			DeserializeComponents<CompTypes...>(current_entity, entityJson["Components"]);
		}
		/*switch (reader.GetLineState())
			{
			case Reader::LineState::EntityID:
				current_entity = reader.getEntityID();
				reader.NextLine();
				break;
			case Reader::LineState::CompData:
				assert(current_entity != InvalidIndex);
				DeserializeComponents<CompTypes...>(current_entity, reader);
				break;
			}*/
		
	}
		
	eID CreateEntity()
	{
		return m_entityManager->CreateEntity();
	}

    void DestroyEntity(eID entity)
    {
        m_entityManager->DestroyEntity(entity);

        m_compManager->OnEntityDestroyed(entity);
        m_systemManager->OnEntityDestroyed(entity);
    }
	// Component Methods

	template <typename Comp>
	void RegisterComponent()
	{
		m_compManager->RegisterComponent<Comp>();
	}

	template <typename None = void, typename Comp, typename ...OtherComp>
	void RegisterComponents()
	{
		RegisterComponent<Comp>();
		RegisterComponents<None, OtherComp...>();
	}

	template <typename Comp>
	void RegisterComponents()
	{
		RegisterComponent<Comp>();
	}

	template <typename C>
	void AddComponent(eID entity, C component)
	{
		m_compManager->AddComponent(entity, component);

		// signature changes!!
		auto newSignature = m_entityManager->GetSignature(entity);
		newSignature.set(m_compManager->GetCompType<C>(), true);
		m_entityManager->SetSignature(entity, newSignature);

		m_systemManager->OnEntitySignatureChanged(entity, newSignature);
	}

	template <typename C>
	void RemoveComponent(eID entity)
	{
        m_compManager->RemoveComponent<C>(entity);

		// signature changes!!
		auto newSignature = m_entityManager->GetSignature(entity);
		newSignature.set(m_compManager->GetCompType<C>(), false	);
        m_entityManager->SetSignature(entity, newSignature);

		m_systemManager->OnEntitySignatureChanged(entity, newSignature);
	}

	template <typename C>
	C& GetComponent(eID entity)
	{
		return m_compManager->GetComponent<C>(entity);
	}

	
	template <typename C>
	CompType GetCompType()
	{
		return m_compManager->GetCompType<C>();
	}


	// System Methods

	template <typename T>
	std::shared_ptr<T> RegisterSystem()
	{
		return m_systemManager->RegisterSystem<T>();
	}

	template <typename None = void, typename S, typename Comp, typename ...otherComps>
	void SetSystemSignature()
	{
		Signature signature;
		signature.set(GetCompType<Comp>());
		SetSystemSignature_impl<None, S, otherComps ...>(signature);
	}
	template <typename S, typename Comp>
	void SetSystemSignature()
	{
		Signature signature;
		signature.set(GetCompType<Comp>());
		m_systemManager->SetSignature<S>(signature);
	}

	Signature& GetSystemSignature(eID entity)
	{
		return m_entityManager->GetSignature(entity);
	}

	template <typename Comp>
	bool CheckCompType(eID entity)
	{
		Signature signature = m_entityManager->GetSignature(entity);
		return signature.test(GetCompType<Comp>());
	}

private:

	template <typename S, typename Comp>
	void SetSystemSignature_impl(Signature& signature)
	{
		signature.set(GetCompType<Comp>());
		
		m_systemManager->SetSignature<S>(signature);
	}

	template <typename None = void, typename S, typename Comp, typename ...otherComps>
	void SetSystemSignature_impl(Signature& signature)
	{
		signature.set(GetCompType<Comp>());
		SetSystemSignature_impl<None, S, otherComps...>(signature);
	}
	
	template <typename CompType>
	void SerializeComponent(eID entity, json& componentsJson)
	{
		if (CheckCompType<CompType>(entity))
		{
			auto data = GetComponent<CompType>(entity);
			json newEntry;
			newEntry["type"] = data.ID;
			newEntry["data"] = data;
			componentsJson.push_back(newEntry);
		}
		m_numSerializedCompTypes++;
	}
	
	template <typename None = void, typename Comp, typename ...otherComps>
	void SerializeComponents(eID entity, json& componentsJson)
	{
		SerializeComponent<Comp>(entity, componentsJson);
		SerializeComponents<None, otherComps...>(entity, componentsJson);
	}

	template <typename Comp>
	void SerializeComponents(eID entity, json& componentsJson)
	{
		SerializeComponent<Comp>(entity, componentsJson);
	}
	

	template <typename None = void, typename Comp, typename ...otherComps>
	void DeserializeComponents(eID entity, json& compJson) 
	{
		DeserializeComponent<Comp>(entity, compJson);
		DeserializeComponents<None, otherComps...>(entity, compJson);
	}

	template <typename Comp>
	void DeserializeComponents(eID entity, json& compJson)
	{
		DeserializeComponent<Comp>(entity, compJson);
	}

	template <typename Comp>
	void DeserializeComponent(eID entity, json& compJson)
	{
		for (auto& comp : compJson)
		{
			CompType ID = comp["type"];
			if (Comp::ID == ID)
			{
				Comp newComponent = comp["data"];
				manager.AddComponent<Comp>(entity, newComponent);
			}
		}
	}

	
	CompType m_numSerializedCompTypes{};
	std::unique_ptr<SystemManager> m_systemManager;
	std::unique_ptr<CompManager> m_compManager;
	std::unique_ptr<EntityManager> m_entityManager;
};

