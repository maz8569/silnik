#pragma once
#include "glm/glm.hpp"
#include <string>
#include "typedefs.h"
#include <vector>
#include <Scene/GObject.h>
#include "Camera.h"
#include "Skybox.h"
#include <Rendering/TextRenderer.h>
#include <GUI/GuiManager.h>

namespace GameEngine {
	class Scene
	{
	private:

		std::string m_name;
		Ref<GObject> m_root = nullptr;
		std::vector<Ref<GObject>> m_GObjects;
		std::vector<Ref<GObject>> m_seenGObjects;

		Ref<Skybox> m_skybox;

		int i = 0;


	public:

		Ref<Camera> m_camera;
		Ref<GameManager> gameManager;
		glm::mat4 m_lightSpace;
		Ref<Collision> colMan = nullptr;
		Ref<TextRenderer> textRenderer = nullptr;
		Ref<GuiManager> guiManager = nullptr;

		Scene(std::string name);
		//virtual ~Scene();

		void Start();
		void Update(float dt);
		void FixedUpdate();
		void LateUpdate();
		void Render(Ref<Shader> shader);
		void RenderAllShadow(Ref<Shader> shader);
		void RenderAllFoam(Ref<Shader> shader);
		void RenderAllWitTheirShader();

		void checkFrustum();

		void addObjectToScene(Ref<GObject> obj);
		virtual Ref<GObject> getObjectByName(const std::string& name);
		virtual std::vector<Ref<GObject>> getAllObjects();
		virtual std::vector<Ref<GObject>> getAllObjectsByName(const std::string& name);
		virtual Ref<GObject> getObjectByID(unsigned int ID);

		std::string getName();

		bool stateCh;

	};
}

