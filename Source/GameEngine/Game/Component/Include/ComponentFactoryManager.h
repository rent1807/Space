#pragma once
#include "stdafx.h"
#include "Component.h"
namespace SpaceGameEngine
{
	class ComponentFactoryManager			//组件工厂方法管理器
	{
	public:
		ComponentFactoryManager();
		~ComponentFactoryManager();
		void Release();

		static ComponentFactoryManager* GetMainManager();
		void SetAsMainManager();

		bool AddComponentFactory(const std::string& name, std::function<Component*(void)> functor);
		bool DeleteComponentFactory(const std::string& name);
		std::function<Component*(void)> FindComponentFactory(const std::string& name);
	private:
		Map<std::string, std::function<Component*(void)> > m_Content;

		static ComponentFactoryManager* sm_pThis;
	};

#define REGISTERCOMPONENTCLASS(name) SpaceGameEngine::ComponentManager::NewComponent<name> name::NewComponent(#name)
}