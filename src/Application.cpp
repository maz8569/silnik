#include "Application.h"

namespace GameEngine {

	Application app;

	Application::Application()
		: m_gameState(GameState::MenuState), m_cursorLocked(false)
	{
		if (windowManager.createWindow() == -1)
		{
			exit(-1);
		}
		windowManager.freeCursor();
		//windowManager.blockCursor();

		EventSystem::InitEventSystem(windowManager.window, &m_EventQueue);

		//renderer.init();
		// Queue a window resize event to properly scale the cameras (according to the window dimensions)
		Event e;
		e.type = EventTypes::WindowResize;
		m_EventQueue.push_back(e);

		m_scene = new Scene("new scene");

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

		textRenderer = CreateRef<TextRenderer>("res/fonts/PressStart2P.ttf", "res/shaders/GUI.vert", "res/shaders/GUI.frag");
		if (!textRenderer->success)
		{
			exit(-1);
		}

		colMan = std::make_shared<Collision>();
		inputManager = std::make_shared<InputManager>(windowManager.window);

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
		//Ref<Model> scene = CreateRef<Model>(Model("res/models/scene/scene.obj"));
		Ref<Model> island = CreateRef<Model>(Model("res/models/island/island.obj"));

		model_s = glm::mat4(1.0f);
		model_s = glm::scale(model_s, { 0.1f, 0.1f, 0.1f });

		// Setup Dear ImGui binding
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//ortho = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);

		ourShader->use();
		ourShader->setInt("shadowMap", 1);
		ourShader->setInt("ourTexture", 0);

		debugDepth->use();
		debugDepth->setInt("depthMap", 0);
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), (float)WindowManager::SCR_WIDTH / WindowManager::SCR_HEIGHT, 0.1f, 100.0f);
		ourShader->setMat4("projection", projection);

		float a = 0;
		bool should_render = false;

		glGenFramebuffers(1, &depthMapFBO);

		glGenTextures(1, &depthMap);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, GameEngine::app.SHADOW_WIDTH, GameEngine::app.SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// attach depth texture as FBO's depth buffer
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		quad = CreateRef<Shape>(Shape(Coords::COORDSTEXT));
		light = CreateRef<DirectionalLight>(DirectionalLight({ 0.0f, -4.0f, 0.2f }));

		light->activate_lights(ourShader, GameEngine::app.getScene()->m_camera);

		root = std::make_shared<SceneNode>(SceneNode());

		projection = glm::perspective(glm::radians(m_scene->m_camera->Zoom), (float)WindowManager::SCR_WIDTH / (float)WindowManager::SCR_HEIGHT, 0.1f, 100.0f);
		mousePicker = CreateRef<MousePicker>(MousePicker(m_scene->m_camera, projection, inputManager));
		player = std::make_shared<Player>(inputManager, b, ourShader, colMan);
		courier = std::make_shared<Courier>(mousePicker, bu, ourShader, colMan);
		courier->set_local_position({ 2, 0, 0 });
		//courier->set_local_rotation({ 90, 0, 0 });
		player->set_local_position({ -2, 0, 0 });
		//player->set_render_AABB(true);
		//courier->set_render_AABB(true);
		//courier->set_color({ 1, 0.0, 0.0 });

		gdom = CreateRef<GObject>(GObject(pacz, ourShader, colMan));
		gdom->set_local_position({ 0, -6, 13 });
		gdom->getAABB()->setStatic(true);
		gdom->Update();

		paczka = CreateRef<GObject>(GObject(dom, ourShader, colMan));
		paczka->set_local_position({ 0, -6, -13 });
		paczka->getAABB()->setStatic(true);
		paczka->Update();

		most = CreateRef<GObject>(GObject(mo, ourShader, colMan));
		most->set_local_position({ 0, -6.1f, -4 });
		most->set_local_rotation({ 0, -90, 0 });
		most->getAABB()->setStatic(true);
		most->Update();

		most2 = CreateRef<GObject>(GObject(mo, ourShader, colMan));
		most2->set_local_position({ 0, -6.1f, -8 });
		most2->set_local_rotation({ 0, 90, 0 });
		most2->getAABB()->setStatic(true);
		most2->Update();

		most3 = CreateRef<GObject>(GObject(mo, ourShader, colMan));
		most3->set_local_position({ 0, -6.1f, 8 });
		most3->set_local_rotation({ 0, -90, 0 });
		most3->getAABB()->setStatic(true);
		most3->Update();

		most4 = CreateRef<GObject>(GObject(mo, ourShader, colMan));
		most4->set_local_position({ 0, -6.1f, 4 });
		most4->set_local_rotation({ 0, 90, 0 });
		most4->getAABB()->setStatic(true);
		most4->Update();

		iisland2 = CreateRef<GObject>(GObject(island, ourShader, colMan));
		iisland2->set_local_scale({ 0.6, 0.6, 0.6 });
		iisland2->scaleAABB({ 0.6, 0.6, 0.6 });
		iisland2->set_local_position({ 13, -10, 0 });
		iisland2->set_tag("terrain");
		iisland2->getAABB()->setStatic(true);
		iisland2->Update();

		iisland3 = CreateRef<GObject>(GObject(island, ourShader, colMan));
		iisland3->set_local_scale({ 0.6, 0.6, 0.6 });
		iisland3->scaleAABB({ 0.6, 0.6, 0.6 });
		iisland3->set_local_position({ -13, -10, 0 });
		iisland3->set_tag("terrain");
		iisland3->getAABB()->setStatic(true);
		iisland3->Update();

		iisland4 = CreateRef<GObject>(GObject(island, ourShader, colMan));
		iisland4->set_local_scale({ 0.6, 0.6, 0.6 });
		iisland4->scaleAABB({ 0.6, 0.6, 0.6 });
		iisland4->set_local_position({ 0, -10, 13 });
		iisland4->set_tag("terrain");
		iisland4->getAABB()->setStatic(true);
		iisland4->Update();

		iisland5 = CreateRef<GObject>(GObject(island, ourShader, colMan));
		iisland5->set_local_scale({ 0.6, 0.6, 0.6 });
		iisland5->scaleAABB({ 0.6, 0.6, 0.6 });
		iisland5->set_local_position({ 0, -10, -13 });
		iisland5->set_tag("terrain");
		iisland5->getAABB()->setStatic(true);
		iisland5->Update();

		iisland = CreateRef<GObject>(GObject(island, ourShader, colMan));
		iisland->set_local_scale({ 0.6, 0.6, 0.6 });
		iisland->scaleAABB({ 0.6, 0.6, 0.6 });
		iisland->set_local_position({ 0, -10, 0 });
		iisland->set_tag("terrain");
		iisland->getAABB()->setStatic(true);
		iisland->Update();

		water = CreateRef<GObject>(GObject(b, ourShader, colMan));
		water->set_local_scale({ 60, 1, 60 });
		water->scaleAABB({ 60, 1, 60 });
		water->set_local_position({ 0, -10, 0 });
		water->getAABB()->setStatic(true);
		water->set_color({ 0.63, 0.68, 0.85 });
		water->Update();


		root->add_child(player);
		root->add_child(courier);
		root->add_child(iisland);
		root->add_child(iisland2);
		root->add_child(iisland3);
		root->add_child(iisland4);
		root->add_child(iisland5);
		root->add_child(water);
		root->add_child(gdom);
		root->add_child(paczka);
		root->add_child(most);
		root->add_child(most2);
		root->add_child(most3);
		root->add_child(most4);
		player->add_parent(root);
		courier->add_parent(root);
		iisland->add_parent(root);
		iisland2->add_parent(root);
		iisland3->add_parent(root);
		iisland4->add_parent(root);
		iisland5->add_parent(root);
		water->add_parent(root);
		gdom->add_parent(root);
		paczka->add_parent(root);
		most->add_parent(root);
		most2->add_parent(root);
		most3->add_parent(root);
		most4->add_parent(root);
		root->update(root->get_transform(), true);

		m_scene->m_camera->player = player;

		// load and create a texture 
		// -------------------------
		stbi_set_flip_vertically_on_load(true);
		texture1.loadFromFile(std::string("res/textures/torus.png").c_str());

		lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, near_plane, far_plane);
		lightView = glm::lookAt(-light->lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;

		std::cout << "Starting Game...\n";

		passed_time = 0.0;
		double current_time = 0.0;
		should_render = false;

		double last_time = glfwGetTime();
		double unprocessed_time = 0.0;

		playAudio("TestSound");

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
		masterRenderer->finishRender();
		player->render();
		courier->render();
		iisland->render();
		iisland2->render();
		iisland3->render();
		iisland4->render();
		iisland5->render();
		water->render();
		gdom->render();
		paczka->render();
		most->render();
		most2->render();
		most3->render();
		most4->render();
		//glBindTexture(GL_TEXTURE_2D, texture1);
		texture1.bindTexture();

		glm::mat4 modelmatrx = iisland->get_transform().m_world_matrix;

		shader->setInt("texture1", 0);
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


		m_scene->Render();

		shader->setVec3("color", { 1, 1, 1 });
	}

	void Application::OnInput()
	{
		PollEvents();

		view = GameEngine::app.getScene()->m_camera->GetViewMatrix();
		inputManager->getInput();
	}

	void Application::OnUpdate(float dt)
	{
		ourShader->setMat4("view", view);

		player->Update();
		courier->Update();
		GameEngine::app.getScene()->m_camera->courier = courier->get_transform().m_position;
		GameEngine::app.getScene()->m_camera->Move();
		colMan->CollisionCheck();

		mousePicker->update();
	}

	void Application::OnRender()
	{
		// activate shader
		glm::mat4 projection = glm::perspective(glm::radians(m_scene->m_camera->Zoom), (float)WindowManager::SCR_WIDTH / (float)WindowManager::SCR_HEIGHT, 0.1f, 100.0f);
		m_scene->m_camera->m_projectionMatrix = projection;
		shadowMap->use();
		shadowMap->setMat4("lightSpaceMatrix", lightSpaceMatrix);

		glViewport(0, 0, GameEngine::app.SHADOW_WIDTH, GameEngine::app.SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE1);
		RenderScene(shadowMap);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		ourShader->use();

		glViewport(0, 0, WindowManager::SCR_WIDTH, WindowManager::SCR_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glViewport(0, 0, WindowManager::SCR_WIDTH, WindowManager::SCR_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ourShader->setVec3("color", { 1, 1, 1 });
		ourShader->setMat4("view", view);
		ourShader->setMat4("projection", projection);
		light->activate_lights(ourShader, m_scene->m_camera);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		RenderScene(ourShader);

		debugDepth->use();
		debugDepth->setFloat("near_plane", near_plane);
		debugDepth->setFloat("far_plane", far_plane);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		//renderQuad();

	}

	void Application::OnRenderUI()
	{
		//textRenderer->RenderText("Position " + std::to_string(player->get_transform().m_position.x) + " " + std::to_string(player->get_transform().m_position.y), 10.0f, 60.0f, 0.5f, glm::vec3(1.0, 0.8f, 1.0f));
		//textRenderer->RenderText("Position " + std::to_string(inputManager->m_posx) + " " + std::to_string(inputManager->m_posy), 10.0f, 60.0f, 0.5f, glm::vec3(1.0, 0.8f, 1.0f));
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
			lastX = e.mx;
			lastY = e.my;
			firstMouse = false;
		}

		float xoffset = e.mx - lastX;
		float yoffset = lastY - e.my; // reversed since y-coordinates go from bottom to top

		lastX = e.mx;
		lastY = e.my;
		m_scene->m_camera->ProcessMouseMovement(xoffset, yoffset);
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

			moveCamera(e);

			break;

		case EventTypes::KeyRelease:
			stopCamera(e);
			break;

		case EventTypes::MouseMove:
			//rotateCamera(e);
			break;
		case EventTypes::WindowResize:
			// make sure the viewport matches the new window dimensions; note that width and 
			// height will be significantly larger than specified on retina displays.
			glViewport(0, 0, e.wx, e.wy);
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