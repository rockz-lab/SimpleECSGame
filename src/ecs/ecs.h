#pragma once


#include "Entity.h"
#include "Component.h"
#include "System.h"

#include <fstream>

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

	void Serialize(const std::string& fileName)
	{
		std::ofstream file;
		file.open(fileName);

		file << "componentArrays" << "\n";
		file << (*m_compManager);
		file << (*m_entityManager);
	}
	// Entity Methods

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

	template <typename C>
	void RegisterComponent()
	{
		m_compManager->RegisterComponent<C>();
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


	std::unique_ptr<SystemManager> m_systemManager;
	std::unique_ptr<CompManager> m_compManager;
	std::unique_ptr<EntityManager> m_entityManager;
};

