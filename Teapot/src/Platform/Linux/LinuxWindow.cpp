#include "LinuxWindow.h"

namespace Teapot
{
	static bool s_GLFWInitialized = false;

	LinuxWindow::LinuxWindow(const WindowProps& props)
	{
		Init(props);
	}

	LinuxWindow::~LinuxWindow()
	{
		Shutdown();
	}

	void LinuxWindow::Init(const WindowProps& props)
	{
		std::cout << "Linux Window Init" << std::endl;
		m_WindowData.Title = props.Title;
		m_WindowData.Height = props.Height;
		m_WindowData.Width = props.Width;

		glfwSetErrorCallback(error_callback);
		
		if (!s_GLFWInitialized && glfwInit())
		{
			s_GLFWInitialized = true;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_WindowData.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_WindowData);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
		}

		//sceneBuffer = new FrameBuffer(props.Width, props.Height);

		const char* glsl_version = "#version 410";
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

		ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
		ImGui_ImplOpenGL3_Init(glsl_version);
	}

	void LinuxWindow::OnFistUpdate()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
			ImGuiWindowFlags_NoBackground;

		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("InvisibleWindow", nullptr, windowFlags);
		ImGui::PopStyleVar(3);

		ImGuiID dockSpaceId = ImGui::GetID("InvisibleWindowDockSpace");

		ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
		ImGui::End();

		ImGuizmo::SetOrthographic(false);
		ImGuizmo::BeginFrame();
		
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
	}

	void LinuxWindow::OnLastUpdate()
	{
		ImGui::Begin("Scene");
		{
			float width = ImGui::GetContentRegionAvail().x;
			float height = ImGui::GetContentRegionAvail().y;
			m_WindowData.Height = static_cast<int>(height);
			m_WindowData.Width = static_cast<int>(width);
			ImGui::BeginChild("GameRender");
			ImGui::Image(
				(ImTextureID)sceneBuffer->GetFrameTexture(),
				ImGui::GetContentRegionAvail(),
				ImVec2(0, 1),
				ImVec2(1, 0)
			);

			RenderGizmo();
		}
		ImGui::EndChild();
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void LinuxWindow::UpdateViewport()
	{
		glViewport(0, 0, m_WindowData.Width, m_WindowData.Height);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		sceneBuffer->RescaleFrameBuffer(m_WindowData.Width, m_WindowData.Height);
	}

	void LinuxWindow::RenderSceneOnImGuiWindow()
	{
		sceneBuffer = std::make_unique<Teapot::FrameBuffer>(m_WindowData.Width, m_WindowData.Height);
	}

	void LinuxWindow::RenderGizmo() const
	{
		if (IsGizmoActive)
		{
			ImGuizmo::SetDrawlist();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, static_cast<float>(m_WindowData.Width), static_cast<float>(m_WindowData.Height));
			ImGuizmo::Manipulate(
				glm::value_ptr(m_camera->GetViewMatrix()),
				glm::value_ptr(m_camera->GetProjMatrix()),
				ImGuizmo::OPERATION::TRANSLATE,
				ImGuizmo::MODE::LOCAL,
				glm::value_ptr(Model::s_Models[Model::s_SelectedModel]->objModel)
			);

			float viewManipulateRight = ImGui::GetWindowPos().x + ImGui::GetContentRegionAvail().x;
			float viewManipulateTop = ImGui::GetWindowPos().y;

			ImGuizmo::ViewManipulate(glm::value_ptr(m_camera->GetViewMatrix()), 1.0f, ImVec2(viewManipulateRight - 128, viewManipulateTop), ImVec2(128, 128), 0x10101010);

			if (ImGuizmo::IsUsing())
			{
				Model::s_Models[Model::s_SelectedModel]->objTranslation = glm::vec3(Model::s_Models[Model::s_SelectedModel]->objModel[3]);
			}
		}
	}

	void LinuxWindow::ActivateGizmo(std::shared_ptr<Camera> camera)
	{
		m_camera = camera;
		IsGizmoActive = true;
	}

	void LinuxWindow::Shutdown()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		
		glfwDestroyWindow(m_Window);
	}
}