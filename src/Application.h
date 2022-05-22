#pragma once

#include <string>
#include <vector>
#include <typedefs.h>
#include <Rendering/MasterRenderer.h>
#include <Scene/Scene.h>
#include <Rendering/WindowManager.h>
#include <Events/EventStruct.h>
#include <Events/Events.h>
#include <Rendering/MasterRenderer.h>
#include <Rendering/Shape.h>
#include "Utils/Json.h"
#include "Audio/AudioManager.h"
#include "Rendering/TextRenderer.h"
#include <Scene/Light.h>
#include "Physics/Collisions/Collision.h"
#include "Physics/RayCasting/MousePicker.h"
#include <Scene/Courier.h>
#include <Scene/Box.h>
#include <Scene/GameManager.h>
#include <Blocks/Block.h>
#include <GUI/GuiManager.h>

namespace GameEngine {

	enum GameState : uint8_t {
		MenuState = 0,
		PlayingState,
		Win,
		Lose
	};

	class Application
	{
	public:
		WindowManager windowManager;

		Application();
		~Application();

		void RunLoop();

		void RenderScene(Ref<Shader> shader);
		void OnInput();
		void OnUpdate(float dt);
		void OnRender();
		void OnRenderUI();

		inline Scene* getScene() {
			return m_scene;
		}

		inline GameState getState()
		{
			return m_gameState;
		}
		const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

		Ref<AudioManager> audioManager = nullptr;

		void playAudio(std::string filename);
		void renderQuad();

		void moveCamera(Event e);
		void stopCamera(Event e);
		void rotateCamera(Event e);

		std::vector<glm::vec4> getFrustumCornersWorldSpace(const glm::mat4& proj, const glm::mat4& view);
		std::vector<glm::vec4> getFrustumCornersWorldSpace(const glm::mat4& projview);

	private:
		void OnEvent(Event e);
		void PollEvents();

		std::vector<Event> m_EventQueue;
		GameState m_gameState;
		bool m_cursorLocked;
		bool m_VSync = true;

		Ref<Shape> quadd;
		Ref<Shader> basicShader;

		Scene* m_scene;

		MasterRenderer renderer;

		double frame_time = 1.0 / 60.0;

		Ref<Json> jsonParser = nullptr;
		Ref<TextRenderer> textRenderer = nullptr;
		Ref<Collision> colMan = nullptr;

		Ref<GuiManager> guiManager = nullptr;

		unsigned int quadVAO = 0;
		unsigned int quadVBO;

		Ref<Shader> ourShader = nullptr;
		Ref<Shader> shadowMap = nullptr;
		Ref<Shader> foaMap = nullptr;
		Ref<Shader> debugDepth = nullptr;
		Ref<Shader> waterShader = nullptr;

		//Ref<MousePicker> mousePicker = nullptr;
		//Ref<Block> block = nullptr;

		float lastX = 0.0f;
		float lastY = 0.0f;

		Ref<MouseCursor> mouseCursor;

		bool firstMouse = true;

		// timing
		double passed_time = 0.0;
		double current_time = 0.0;
		float deltaTime = 0.0f;
		float lastFrame = 0.0f;
		float ftime = 0.0f;

		glm::vec3 direction;

		glm::mat4 view;
		glm::mat4 punktLightpos;
		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		float near_plane = 0.5f, far_plane = 40.f;
		BasicTexture texture1;
		Ref<DirectionalLight> light;

		float totalTime = 0.0f;

		int fps = 0;
		float defV = 0.1f;

		Ref<InputManager> inputManager = nullptr;

		Ref<Player> player;
		//Ref<Courier> courier;
		Ref<GameManager> gameManager;

		Ref<MasterRenderer> masterRenderer;
		Ref<Shape> quad;

		glm::mat4 saas;
		glm::vec4 ucamera_params;
		glm::vec4 uScreenSize;

		unsigned int depthMapFBO, depthMap;
		
		unsigned int foamMapFBO, foamMap, gPosition, gNormal, gColorSpec;
	};

	extern Application app;

	Application* GetApp();
}
