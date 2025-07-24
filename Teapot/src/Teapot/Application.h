#pragma once

#include <iostream>
#include <memory>

#include "Core.h"
#include "Window.h"
#include "Ui/WindowControlUI.h"
#include "Light/Light.h"
#include "Scene/SceneContext.h"
#include "ModelReader.h"
#include "Models/ModelHandler.h"
#include "Models/DebugModelHandler.h"

namespace Teapot 
{
	class TEAPOT_API Application
	{
	public:
		explicit Application(Teapot::WindowProps& props);
		virtual ~Application();
		virtual void OnUpdate() = 0;
		virtual void OnUpdateAwake() = 0;
		void Run();
		inline static Application& Get() { return *s_Instance; }

		inline Window& GetWindow() { return Teapot::SceneContext::Get().GetWindow(); }
		inline Camera& GetCamera() { return Teapot::SceneContext::Get().GetCamera(); }
		inline WindowControlUI& GetUI() { return *m_windowUI; }
		inline Light& GetLight() { return m_modelHandler->light; }

		inline void CreateModel(const Shapes::Shape& shapes, const std::string& nameObject){ m_modelHandler->CreateModel(shapes, nameObject); };
		inline void CreateDebugModel(const Shapes::Shape& shapes, const std::string& nameObject) { m_debugModelHander->CreateModel(shapes, nameObject); };

		inline void CreateModelsFromXML(const std::string& path) { m_modelReader->CreateSceneFromXML(path); };

	private:
		std::unique_ptr<Teapot::WindowControlUI> m_windowUI;
		std::shared_ptr<Teapot::ModelHandler> m_modelHandler;
		std::shared_ptr<Teapot::DebugModelHandler> m_debugModelHander;
		std::unique_ptr<Teapot::ModelReader> m_modelReader;


		bool m_Running = true;
		static Application* s_Instance;
	};
}