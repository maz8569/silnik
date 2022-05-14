#include "Application.h"

namespace GameEngine {

	Application app;

	Application::Application()
		: m_gameState(GameState::MenuState), m_cursorLocked(true)
	{
		if (windowManager.createWindow() == -1)
		{
			exit(-1);
		}
		windowManager.freeCursor();
		//windowManager.blockCursor();

		lastX = (float)windowManager.SCR_WIDTH / 2.0;
		lastY = (float)windowManager.SCR_HEIGHT / 2.0;

		mouseX = (float)windowManager.SCR_WIDTH / 2.0;
		mouseY = (float)windowManager.SCR_HEIGHT / 2.0;

		EventSystem::InitEventSystem(windowManager.window, &m_EventQueue);

		//renderer.init();
		// Queue a window resize event to properly scale the cameras (according to the window dimensions)


		m_scene = new Scene("new scene");
		Event e;
		e.type = EventTypes::WindowResize;
		e.wx = windowManager.SCR_WIDTH;
		e.wy = windowManager.SCR_HEIGHT;
		m_EventQueue.push_back(e);

		jsonParser = CreateRef<Json>();
		if (jsonParser->print() == 1)
		{
			exit(-1);
		}

		audioManager = CreateRef<AudioManager>();
		if (!audioManager->success)
		{
			exit(-1);
		}

		audioManager->readMonoData("TestSound");

		textRenderer = CreateRef<TextRenderer>(m_scene->m_camera, "res/fonts/PressStart2P.ttf", "res/shaders/GUIText.vert", "res/shaders/GUIText.frag");
		if (!textRenderer->success)
		{
			exit(-1);
		}
		colMan = std::make_shared<Collision>();
		inputManager = std::make_shared<InputManager>(windowManager.window);
		guiRenderer = CreateRef<GUIRenderer>(m_scene->m_camera);
	}

	Application::~Application()
	{
		windowManager.closeWindow();
		textRenderer->clean();
		audioManager->clean();
		quad->clean();
	}

	void Application::RunLoop()
	{
		// configure global opengl state
		// -----------------------------
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);

		ourShader = std::make_shared<Shader>(Shader("res/shaders/basic.vert", "res/shaders/basic.frag"));
		shadowMap = std::make_shared<Shader>("res/shaders/shadowmapping.vert", "res/shaders/shadowmapping.frag");
		debugDepth = std::make_shared<Shader>("res/shaders/debugdepth.vert", "res/shaders/debugdepth.frag");
		masterRenderer = CreateRef<MasterRenderer>();
		masterRenderer->setQuadShader(ourShader);

		Ref<Model> b = CreateRef<Model>(Model("res/models/cube/cube.obj"));
		Ref<Model> bu = CreateRef<Model>(Model("res/models/statek/untitled.obj"));
		Ref<Model> dom = CreateRef<Model>(Model("res/models/dom/dom_p.obj"));
		Ref<Model> pacz = CreateRef<Model>(Model("res/models/paczka/paczka.obj"));
		Ref<Model> mo = CreateRef<Model>(Model("res/models/most/niby_most_p.obj"));
		Ref<Model> island = CreateRef<Model>(Model("res/models/island/island.obj"));

		// Setup Dear ImGui binding
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		ourShader->use();
		ourShader->setInt("shadowMap", 1);
		ourShader->setInt("ourTexture", 0);

		debugDepth->use();
		debugDepth->setInt("depthMap", 0);

		float a = 0;
		bool should_render = false;

		glGenFramebuffers(1, &depthMapFBO);

		glGenTextures(1, &depthMap);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		// attach depth texture as FBO's depth buffer
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		quad = CreateRef<Shape>(Shape(Coords::COORDSTEXT));
		light = CreateRef<DirectionalLight>(DirectionalLight({ -1.5f, -3.0f, 1.5f }));

		light->activate_lights(ourShader, m_scene->m_camera);

		root = std::make_shared<SceneNode>(SceneNode());

		mousePicker = CreateRef<MousePicker>(MousePicker(m_scene->m_camera, inputManager));
		player = std::make_shared<Player>(inputManager, b, colMan);
		courier = std::make_shared<Courier>(mousePicker, bu, colMan);
		courier->set_local_position({ 2, 0, 0 });
		//courier->set_local_rotation({ 90, 0, 0 });
		player->set_local_position({ -2, 0, 0 });
		//player->set_render_AABB(true);
		courier->set_render_AABB(true);
		//courier->set_color({ 1, 0.0, 0.0 });

		// TODO: move to scene
		{
			Ref<GObject> gdom = CreateRef<GObject>(dom, colMan);
			gdom->set_local_position({ 0, -7, -13 });
			gdom->getAABB()->setStatic(true);
			gdom->set_render_AABB(true);
			gdom->set_tag("house");

			Ref<GObject> paczka = CreateRef<GObject>(pacz, colMan);
			paczka->set_local_position({ 0, -7, 13 });
			paczka->getAABB()->setStatic(true);
			//paczka->set_render_AABB(true);
			paczka->set_tag("package");

			Ref<GObject> most = CreateRef<GObject>(mo, colMan);
			most->set_local_position({ 0, -7.2, -4 });
			most->set_local_rotation({ 0, -90, 0 });
			most->getAABB()->setStatic(true);
			most->rotateAABB(Degrees::D90, Axis::Z);
			most->set_render_AABB(true);

			Ref<GObject> most2 = CreateRef<GObject>(mo, colMan);
			most2->set_local_position({ 0, -7.2, -8.5f });
			most2->set_local_rotation({ 0, 90, 0 });
			most2->getAABB()->setStatic(true);
			most2->rotateAABB(Degrees::D270, Axis::Z);
			most2->set_render_AABB(true);

			Ref<GObject> most3 = CreateRef<GObject>(mo, colMan);
			most3->set_local_position({ 0, -7.2, 8.5f });
			most3->set_local_rotation({ 0, -90, 0 });
			most3->getAABB()->setStatic(true);
			most3->rotateAABB(Degrees::D90, Axis::Z);
			most3->set_render_AABB(true);

			Ref<GObject> most4 = CreateRef<GObject>(mo, colMan);
			most4->set_local_position({ 0, -7.2, 4 });
			most4->set_local_rotation({ 0, 90, 0 });
			most4->getAABB()->setStatic(true);
			most4->rotateAABB(Degrees::D270, Axis::Z);
			most4->set_render_AABB(true);

			Ref<GObject> most5 = CreateRef<GObject>(mo, colMan);
			most5->set_local_position({4, -7.2, 0 });
			most5->set_local_rotation({ 0, 0, 0 });
			most5->getAABB()->setStatic(true);
			most5->set_render_AABB(true);

			Ref<GObject> most6 = CreateRef<GObject>(mo, colMan);
			most6->set_local_position({ 8.5f, -7.2, 0 });
			most6->set_local_rotation({ 0, 180, 0 });
			most6->getAABB()->setStatic(true);
			most6->rotateAABB(Degrees::D180, Axis::Z);
			most6->set_render_AABB(true);

			Ref<GObject> most7 = CreateRef<GObject>(mo, colMan);
			most7->set_local_position({ -8.5f, -7.2, 0 });
			most7->set_local_rotation({ 0, 0, 0 });
			most7->getAABB()->setStatic(true);
			most7->set_render_AABB(true);

			Ref<GObject> most8 = CreateRef<GObject>(mo, colMan);
			most8->set_local_position({ -4, -7.2, 0 });
			most8->set_local_rotation({ 0, 180, 0 });
			most8->getAABB()->setStatic(true);
			most8->rotateAABB(Degrees::D180, Axis::Z);
			most8->set_render_AABB(true);

			Ref<GObject> iisland2 = CreateRef<GObject>(island, colMan);
			iisland2->set_local_scale({ 0.6, 0.6, 0.6 });
			iisland2->scaleAABB({ 0.6, 0.6, 0.6 });
			iisland2->setAABBextentY(0.9f);
			iisland2->set_local_position({ 13, -10, 0 });
			iisland2->set_tag("terrain");
			iisland2->getAABB()->setStatic(true);
			iisland2->set_render_AABB(true);

			Ref<GObject> iisland3 = CreateRef<GObject>(GObject(island, colMan));
			iisland3->set_local_scale({ 0.6, 0.6, 0.6 });
			iisland3->scaleAABB({ 0.6, 0.6, 0.6 });
			iisland3->setAABBextentY(0.9f);
			iisland3->set_local_position({ -13, -10, 0 });
			iisland3->set_tag("terrain");
			iisland3->getAABB()->setStatic(true);

			Ref<GObject> iisland4 = CreateRef<GObject>(GObject(island, colMan));
			iisland4->set_local_scale({ 0.6, 0.6, 0.6 });
			iisland4->scaleAABB({ 0.6, 0.6, 0.6 });
			iisland4->setAABBextentY(0.9f);
			iisland4->set_local_position({ 0, -10, 13 });
			iisland4->set_tag("terrain");
			iisland4->getAABB()->setStatic(true);

			Ref<GObject> iisland5 = CreateRef<GObject>(GObject(island, colMan));
			iisland5->set_local_scale({ 0.6, 0.6, 0.6 });
			iisland5->scaleAABB({ 0.6, 0.6, 0.6 });
			iisland5->setAABBextentY(0.9f);
			iisland5->set_local_position({ 0, -10, -13 });
			iisland5->set_tag("terrain");
			iisland5->getAABB()->setStatic(true);
			
			Ref<GObject> iisland = CreateRef<GObject>(GObject(island, colMan));
			iisland->set_local_scale({ 0.6, 0.6, 0.6 });
			iisland->scaleAABB({ 0.6, 0.6, 0.6 });
			iisland->setAABBextentY(0.9f);
			iisland->set_local_position({ 0, -10, 0 });
			iisland->set_tag("terrain");
			iisland->getAABB()->setStatic(true);
			
			Ref<GObject> water = CreateRef<GObject>(b, colMan);
			water->set_local_scale({ 60, 1, 60 });
			water->scaleAABB({ 60, 1, 60 });
			//water->setAABBextentY(0.9f);
			water->set_local_position({ 0, -8.2, 0 });
			water->getAABB()->setStatic(true);
			water->set_tag("water");
			water->set_color({ 0.63, 0.68, 0.85 });
			water->set_render_AABB(true);


			m_scene->addObjectToScene(player);
			m_scene->addObjectToScene(courier);
			m_scene->addObjectToScene(iisland);
			m_scene->addObjectToScene(iisland2);
			m_scene->addObjectToScene(iisland3);
			m_scene->addObjectToScene(iisland4);
			m_scene->addObjectToScene(iisland5);
			m_scene->addObjectToScene(water);
			m_scene->addObjectToScene(gdom);
			m_scene->addObjectToScene(paczka);
			m_scene->addObjectToScene(most);
			m_scene->addObjectToScene(most2);
			m_scene->addObjectToScene(most3);
			m_scene->addObjectToScene(most4);
			m_scene->addObjectToScene(most5);
			m_scene->addObjectToScene(most6);
			m_scene->addObjectToScene(most7);
			m_scene->addObjectToScene(most8);
			//root->update(root->get_transform(), true);
		}
		m_scene->m_camera->player = player;

		// load and create a texture 
		// -------------------------
		stbi_set_flip_vertically_on_load(true);
		texture1.loadFromFile(std::string("res/textures/torus.png").c_str());

		lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, near_plane, far_plane);

		std::cout << "Starting Game...\n";

		passed_time = 0.0;
		double current_time = 0.0;
		should_render = false;

		double last_time = glfwGetTime();
		double unprocessed_time = 0.0;

		playAudio("TestSound");

		Ref<GuiComponent> comp = CreateRef<GuiComponent>(GuiComponent("res/textures/fullheart.png", { -150, 20 }, { 48, 48 }));

		guiRenderer->addComponent(comp);

		while (!glfwWindowShouldClose(windowManager.window))
		{

			current_time = glfwGetTime();
			passed_time = current_time - last_time;

			last_time = current_time;
			unprocessed_time += passed_time;

			OnInput();

			while (unprocessed_time >= frame_time)
			{
				should_render = true;
				unprocessed_time -= frame_time;

				OnUpdate(frame_time);
			}

			if (should_render)
			{
				should_render = false;
				OnRender();
				OnRenderUI();

				windowManager.updateWindow();
			}

		}
		std::cout << "Thanks for playing!";
	}

	void Application::RenderScene(Ref<Shader> shader)
	{
		/*
		player->render(shader);
		courier->render(shader);
		iisland->render(shader);
		iisland2->render(shader);
		iisland3->render(shader);
		iisland4->render(shader);
		iisland5->render(shader);
		water->render(shader);
		gdom->render(shader);
		paczka->render(shader);
		most->render(shader);
		most2->render(shader);
		most3->render(shader);
		most4->render(shader);
		*/
		//glBindTexture(GL_TEXTURE_2D, texture1);
		//texture1.bindTexture();

		//glm::mat4 modelmatrx = iisland->get_transform().m_world_matrix;

		//shader->setInt("texture1", 0);
		//glActiveTexture(GL_TEXTURE0);

		//shader->setVec3("color", { 0.63, 0.68, 0.85 });
		/*
		for (int i = 0; i < 16; i++)
		{
			for (int j = 0; j < 16; j++)
			{
			}
		}
		*/
		//modelmatrx = glm::translate(iisland->get_transform().m_world_matrix, { 1, 4, 1 });
		//shader->setMat4("model", modelmatrx);
		//quad->Render(36);



		//shader->setVec3("color", { 1, 1, 1 });
	}

	void Application::OnInput()
	{
		PollEvents();

		view = m_scene->m_camera->GetViewMatrix();
		inputManager->getInput();
	}

	void Application::OnUpdate(float dt)
	{
		ourShader->setMat4("view", view);

		m_scene->Update();
		m_scene->m_camera->courier = courier->get_transform().m_position;
		colMan->CollisionCheck();

		mousePicker->update();
	}

	void Application::OnRender()
	{
		masterRenderer->finishRender();

		// activate shader
		m_scene->m_camera->m_projectionMatrix = glm::perspective(glm::radians(m_scene->m_camera->Zoom), (float)windowManager.SCR_WIDTH / (float)windowManager.SCR_HEIGHT, 0.1f, 100.0f);
		lightView = glm::lookAt(-light->lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;
		shadowMap->use();
		shadowMap->setMat4("lightSpaceMatrix", lightSpaceMatrix);
		
		glDisable(GL_CULL_FACE);
		//glCullFace(GL_FRONT);
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE1);
		m_scene->RenderAll(shadowMap);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);

		ourShader->use();

		glViewport(0, 0, windowManager.SCR_WIDTH, windowManager.SCR_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ourShader->setVec3("color", { 1, 1, 1 });
		ourShader->setMat4("view", view);
		ourShader->setMat4("projection", m_scene->m_camera->m_projectionMatrix);
		ourShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
		light->activate_lights(ourShader, m_scene->m_camera);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		m_scene->RenderAll(ourShader);

		/*
		debugDepth->use();
		debugDepth->setFloat("near_plane", near_plane);
		debugDepth->setFloat("far_plane", far_plane);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		renderQuad();
		*/
	}

	void Application::OnRenderUI()
	{
		guiRenderer->Render();

		//textRenderer->RenderText("Position " + std::to_string(player->get_transform().m_position.x) + " " + std::to_string(player->get_transform().m_position.y), 10.0f, 60.0f, 0.5f, glm::vec3(1.0, 0.8f, 1.0f));
		textRenderer->RenderText("Position " + std::to_string(mouseX - windowManager.SCR_WIDTH/2) + " " + std::to_string(windowManager.SCR_HEIGHT / 2 - mouseY), 10.0f, 60.0f, 0.5f, glm::vec3(1.0, 0.8f, 1.0f));
	}

	void Application::PollEvents()
	{
		for (int i = 0; i < m_EventQueue.size(); i++)
		{
			OnEvent(m_EventQueue[i]);
		}

		m_EventQueue.clear();
	}

	void Application::playAudio(std::string filename)
	{
		audioManager->playMonoSound(filename);
	}

	void Application::renderQuad()
	{
		if (quadVAO == 0)
		{
			float quadVertices[] = {
				// positions        // texture Coords
				 0.0f,  1.0f, 0.0f, 0.0f, 1.0f,
				 0.0f,  0.0f, 0.0f, 0.0f, 0.0f,
				 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
				 1.0f,  0.0f, 0.0f, 1.0f, 0.0f,
			};
			// setup plane VAO
			glGenVertexArrays(1, &quadVAO);
			glGenBuffers(1, &quadVBO);
			glBindVertexArray(quadVAO);
			glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		}
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}

	void Application::moveCamera(Event e)
	{
		switch (e.key)
		{
		case GLFW_KEY_UP:
			m_scene->m_camera->ProcessKeyboard(FORWARD);
			break;
		case GLFW_KEY_DOWN:
			m_scene->m_camera->ProcessKeyboard(BACKWARD);
			break;
		case GLFW_KEY_LEFT:
			m_scene->m_camera->ProcessKeyboard(LEFT);
			break;
		case GLFW_KEY_RIGHT:
			m_scene->m_camera->ProcessKeyboard(RIGHT);
			break;
		default:
			break;
		}
	}

	void Application::stopCamera(Event e)
	{
		switch (e.key)
		{
		case GLFW_KEY_UP:
			m_scene->m_camera->ProcessKeyboard(BACKWARD);
			break;
		case GLFW_KEY_DOWN:
			m_scene->m_camera->ProcessKeyboard(FORWARD);
			break;
		case GLFW_KEY_LEFT:
			m_scene->m_camera->ProcessKeyboard(RIGHT);
			break;
		case GLFW_KEY_RIGHT:
			m_scene->m_camera->ProcessKeyboard(LEFT);
			break;
		default:
			break;
		}
	}

	void Application::rotateCamera(Event e)
	{
		if (firstMouse)
		{
			lastX = mouseX;
			lastY = mouseY;
			firstMouse = false;
		}

		float xoffset = e.mx - lastX;
		float yoffset = lastY - e.my; // reversed since y-coordinates go from bottom to top

		lastX = e.mx;
		lastY = e.my;
		m_scene->m_camera->ProcessMouseMovement(xoffset, yoffset);
	}

	std::vector<glm::vec4> Application::getFrustumCornersWorldSpace(const glm::mat4& proj, const glm::mat4& view)
	{
		return getFrustumCornersWorldSpace(proj * view);
	}

	std::vector<glm::vec4> Application::getFrustumCornersWorldSpace(const glm::mat4& projview)
	{
		const auto inv = glm::inverse(projview);

		std::vector<glm::vec4> frustumCorners;
		for (unsigned int x = 0; x < 2; ++x)
		{
			for (unsigned int y = 0; y < 2; ++y)
			{
				for (unsigned int z = 0; z < 2; ++z)
				{
					const glm::vec4 pt = inv * glm::vec4(2.0f * x - 1.0f, 2.0f * y - 1.0f, 2.0f * z - 1.0f, 1.0f);
					frustumCorners.push_back(pt / pt.w);
				}
			}
		}

		return frustumCorners;
	}

	void Application::OnEvent(Event e)
	{
		switch (e.type)
		{
		case EventTypes::KeyPress:
			if (e.key == GLFW_KEY_ESCAPE)
			{
				glfwSetWindowShouldClose(windowManager.window, true);
			}

			if (e.key == GLFW_KEY_M)
			{
				playAudio("TestSound");
			}

			if (e.key == GLFW_KEY_TAB)
			{
				if (m_gameState != GameState::MenuState)
				{
					m_gameState = GameState::MenuState;
					windowManager.freeCursor();
				}
				else
				{
					m_gameState = GameState::PlayingState;
					windowManager.blockCursor();
				}
			}

			if (m_gameState == GameState::PlayingState)
			{
				moveCamera(e);
			}

			break;

		case EventTypes::KeyRelease:
			if (m_gameState == GameState::PlayingState)
			{
				stopCamera(e);
			}
			break;

		case EventTypes::MouseMove:
			mouseX = e.mx;
			mouseY = e.my;
			if (m_gameState == GameState::PlayingState)
			{
				rotateCamera(e);
			}
			break;
		case EventTypes::WindowResize:
			// make sure the viewport matches the new window dimensions; note that width and 
			// height will be significantly larger than specified on retina displays.
			glViewport(0, 0, e.wx, e.wy);
			windowManager.SCR_HEIGHT = e.wy;
			windowManager.SCR_WIDTH = e.wx;

			m_scene->m_camera->scr_width = e.wx;
			m_scene->m_camera->scr_height = e.wy;

			GuiComponent::setScrWidth(e.wx);
			GuiComponent::setScrHeight(e.wy);

			break;
		default:
			break;
		}
	}

	Application* GetApp()
	{
		return &app;
	}

}