#pragma once

#include <iostream>
#include <memory>

#include "Core.h"
#include "Window.h"
#include "Ui/WindowControlUI.h"
#include "Light/Light.h"
#include "Scene/SceneContext.h"
#include "ModelReader.h"
#include "Models/Model/ModelHandler.h"
#include "Models/DebugModel/DebugModelHandler.h"

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
		inline WindowControlUI& GetUI() { return *mp_windowUI; }
		inline Light& GetLight() { return mp_modelHandler->light; }
		inline std::shared_ptr<Teapot::ModelHandler> GetModelHandler() { return mp_modelHandler; }
		inline std::shared_ptr<Teapot::DebugModelHandler> GetDebugModelHandler() { return mp_debugModelHandler; }

		inline void CreateModel(Shapes::Shape& shapes, const std::string& nameObject){ mp_modelHandler->CreateModel(shapes, nameObject); };
		inline void CreateDebugModel(Shapes::Shape& shapes, const std::string& nameObject) { mp_debugModelHandler->CreateModel(shapes, nameObject); };
		inline void CreateModelsFromXML(const std::string& path) { mp_modelReader->CreateSceneFromXML(path); };

	private:
		std::unique_ptr<Teapot::WindowControlUI> mp_windowUI;
		std::shared_ptr<Teapot::ModelHandler> mp_modelHandler;
		std::shared_ptr<Teapot::DebugModelHandler> mp_debugModelHandler;
		std::unique_ptr<Teapot::ModelReader> mp_modelReader;


		bool m_running = true;
		static Application* s_Instance;
	};
}