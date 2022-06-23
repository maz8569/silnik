#include "Application.h"

namespace GameEngine {

	Application app;

	void changeScene()
	{
		app.sceneNumb++;

		switch (app.sceneNumb)
		{
		case 0:
			app.makemenucene();
			break;
		case 1:
			app.make1scene();
			break;
		case 2:
			app.maketestscene();
			break;
		default:
			app.sceneNumb = 0;
			app.makemenucene();
			break;
		}
	}

	void retryLev()
	{
		app.loadScene();
	}

	void nextLev()
	{
		app.sceneNumb++;
		app.loadScene();
	}

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

		mouseCursor = CreateRef<MouseCursor>();

		mouseCursor->mousePos.x = (float)windowManager.SCR_WIDTH / 2.0;
		mouseCursor->mousePos.y = (float)windowManager.SCR_HEIGHT / 2.0;

		EventSystem::InitEventSystem(windowManager.window, &m_EventQueue);

		// Queue a window resize event to properly scale the cameras (according to the window dimensions)

		m_scene = new Scene("scene1");
		Event e;
		e.type = EventTypes::WindowResize;
		e.wx = windowManager.SCR_WIDTH;
		e.wy = windowManager.SCR_HEIGHT;
		m_EventQueue.push_back(e);

		jsonParser = CreateRef<Json>();
		if (jsonParser->testscene() == 1)
		{
			exit(-1);
		}

		audioManager = AudioManager::getInstance();
		if (!audioManager->success)
		{
			exit(-1);
		}

		inputManager = CreateRef<InputManager>(windowManager.window);

		particleRenderer = CreateRef<ParticleRenderer>();
	}

	Application::~Application()
	{
		windowManager.closeWindow();
		m_scene->textRenderer->clean();
		audioManager->clean();
		quad->clean();
		ResourceManager::clear();
		delete m_scene;
	}

	void Application::RunLoop()
	{
		// configure global opengl state
		// -----------------------------
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);

		ResourceManager::init();


		ourShader = ResourceManager::getShader("ourShader");
		animShader = ResourceManager::getShader("animatedShader");
		refrShader = ResourceManager::getShader("refract");
		waterShader = ResourceManager::getShader("water");
		shadowMap = ResourceManager::getShader("shadowMap");
		foaMap = ResourceManager::getShader("foaMap");
		debugDepth = ResourceManager::getShader("debugDepth");
		masterRenderer = CreateRef<MasterRenderer>();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		float ooo = 0.0075;

		ourShader->use();
		ourShader->setInt("shadowMap", 1);
		ourShader->setInt("cameraDepthMap", 2);
		ourShader->setInt("ourTexture", 0);
		ourShader->setFloat("amount", ooo);

		animShader->use();
		animShader->setInt("shadowMap", 1);
		animShader->setInt("cameraDepthMap", 2);
		animShader->setInt("ourTexture", 0);
		animShader->setFloat("amount", ooo);

		refrShader->use();
		refrShader->setInt("shadowMap", 1);
		refrShader->setInt("cameraDepthMap", 2);
		waterShader->setInt("colorTexture", 3);
		refrShader->setInt("ourTexture", 0);
		refrShader->setFloat("amount", ooo);

		waterShader->use();
		waterShader->setInt("foamTexture", 2);
		waterShader->setInt("refractTexture", 3);
		waterShader->setInt("noiseTexture", 4);
		waterShader->setInt("ourTexture", 0);
		waterShader->setFloat("amount", ooo);

		debugDepth->use();
		debugDepth->setInt("depthMap", 0);

		foaMap->use();
		foaMap->setFloat("amount", ooo);

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
		
		glGenFramebuffers(1, &foamMapFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, foamMapFBO);

		glGenTextures(1, &gPosition);
		glBindTexture(GL_TEXTURE_2D, gPosition);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, windowManager.SCR_WIDTH, windowManager.SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glGenTextures(1, &foamMap);
		glBindTexture(GL_TEXTURE_2D, foamMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, windowManager.SCR_WIDTH, windowManager.SCR_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		// attach depth texture as FBO's depth buffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, foamMap, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glGenFramebuffers(1, &refrFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, refrFBO);

		glGenTextures(1, &refrColor);
		glBindTexture(GL_TEXTURE_2D, refrColor);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, windowManager.SCR_WIDTH, windowManager.SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, refrColor, 0);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);

		glGenRenderbuffers(1, &refRBO);
		glBindRenderbuffer(GL_RENDERBUFFER, refRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, windowManager.SCR_WIDTH, windowManager.SCR_HEIGHT);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, refRBO);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glGenFramebuffers(1, &finalFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, finalFBO);

		glGenTextures(1, &finalColor);
		glBindTexture(GL_TEXTURE_2D, finalColor);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, windowManager.SCR_WIDTH, windowManager.SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, finalColor, 0);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);

		glGenRenderbuffers(1, &finalRBO);
		glBindRenderbuffer(GL_RENDERBUFFER, finalRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, windowManager.SCR_WIDTH, windowManager.SCR_HEIGHT);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, finalRBO);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//TODO: move to scene
		makemenucene();
		//make3scene();

		float n = 0.1f;
		float f = 100.0f;

		ucamera_params = glm::vec4(1/f, f, (1 - f/ n)/2, (1 + f / n) / 2);

		// load and create a texture 
		// -------------------------
		stbi_set_flip_vertically_on_load(true);
		texture1.loadFromFile(std::string("res/textures/torus.png").c_str());

		lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, near_plane, far_plane);
		//jsonParser->print();

		perlinTexture = ResourceManager::getTexture("perlinoise");

		std::cout << "Starting Game...\n";

		passed_time = 0.0;
		double current_time = 0.0;
		should_render = false;

		double last_time = glfwGetTime();
		double unprocessed_time = 0.0;

		//audioManager->loopMonoSound("menu");
		//guiManager->addComponent(std::string("res/textures/torus.png"), glm::vec2( 550, -310 ), glm::vec2( 62, 6 ), 0);
		//Ref<Slider> slider = guiManager->addSlider(-1, 2.2, &defV, std::string("res/textures/fullheart.png"), glm::vec2( 550, -310 ), glm::vec2( 24, 24));


		//Ref<Constraints> constraints = CreateRef<Constraints>(glm::vec2(500, 600), glm::vec2(0, 0), false, true);
		//slider->setConstraints(constraints);
		

		while (!glfwWindowShouldClose(windowManager.window))
		{

			current_time = glfwGetTime();
			passed_time = current_time - last_time;

			last_time = current_time;
			unprocessed_time += passed_time;

			if (totalTime > 360)
			{
				totalTime = 0.0f;
			}

			totalTime += passed_time;

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
		std::cout << "\nThanks for playing!";
	}

	void Application::RenderScene(Ref<Shader> shader)
	{
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
		animShader->setMat4("view", view);
		refrShader->setMat4("view", view);
		waterShader->setMat4("view", view);

		m_scene->Update(dt);

		ourShader->setVec3("cameraPos", m_scene->m_camera->Position);
		animShader->setVec3("cameraPos", m_scene->m_camera->Position);
		refrShader->setVec3("cameraPos", m_scene->m_camera->Position);
		waterShader->setVec3("cameraPos", m_scene->m_camera->Position);

		if (!m_scene->stateCh)
		{
			Ref<GuiComponent> but;
			switch (m_scene->gameManager->isWin())
			{
			case GState::Playing:
				m_scene->gameManager->setTime(m_scene->gameManager->getTime() - dt);
				break;
			case GState::Win:
				m_scene->guiManager->addComponent(std::string("win"), glm::vec2(0, 0), glm::vec2(1000, 550));
				but = m_scene->guiManager->addComponent(std::string("next"), glm::vec2(0, -300), glm::vec2(200, 70));
				but->setOnClickFunction(nextLev);
				m_scene->stateCh = true;
				break;
			case GState::Lose:
				m_scene->guiManager->addComponent(std::string("lose"), glm::vec2(0, 0), glm::vec2(1000, 550));
				but = m_scene->guiManager->addComponent(std::string("retry"), glm::vec2(0, -300), glm::vec2(200, 70));
				but->setOnClickFunction(retryLev);
				m_scene->stateCh = true;
				break;
			default:
				break;
			}
		}

		inputManager->postUpdate();
	}

	void Application::OnRender()
	{
		masterRenderer->finishRender();

		// activate shader
		m_scene->m_camera->m_projectionMatrix = glm::perspective(glm::radians(m_scene->m_camera->Zoom), (float)windowManager.SCR_WIDTH / (float)windowManager.SCR_HEIGHT, 0.1f, 100.0f);
		center = m_scene->m_camera->Position - glm::vec3(2,  8, 24);
		lightView = glm::lookAt(center -light->lightPos, center, glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;
		//lightSpaceMatrix = light->getLightSpaceMatrix(m_scene->m_camera);
		shadowMap->use();
		shadowMap->setMat4("lightSpaceMatrix", lightSpaceMatrix);
		
		glDisable(GL_CULL_FACE);
		//glCullFace(GL_FRONT);
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE1);
		m_scene->RenderAllShadow(shadowMap);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);

		foaMap->use();
		foaMap->setMat4("view", view);
		foaMap->setMat4("projection", m_scene->m_camera->m_projectionMatrix);
		foaMap->setVec3("cameraPos", m_scene->m_camera->Position);
		
		//shadowMap->use();
		//shadowMap->setMat4("lightSpaceMatrix", m_scene->m_camera->m_projectionMatrix * view);
		
		glDisable(GL_BLEND);
		//glCullFace(GL_FRONT);
		glViewport(0, 0, windowManager.SCR_WIDTH, windowManager.SCR_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, foamMapFBO);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_scene->RenderAllShadow(foaMap);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//glCullFace(GL_BACK);
		glEnable(GL_BLEND);
		
		refrShader->use();
		refrShader->setVec3("color", { 1, 1, 1 });
		refrShader->setMat4("view", view);
		refrShader->setMat4("projection", m_scene->m_camera->m_projectionMatrix);
		refrShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);

		light->activate_lights(refrShader, m_scene->m_camera);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, foamMap);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, refrColor);
		perlinTexture->Bind(4);
		glEnable(GL_DEPTH_TEST);

		glViewport(0, 0, windowManager.SCR_WIDTH, windowManager.SCR_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, refrFBO);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_scene->RenderAllFoam(refrShader);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		ourShader->use();

		glViewport(0, 0, windowManager.SCR_WIDTH, windowManager.SCR_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, finalFBO);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ourShader->setVec3("color", { 1, 1, 1 });
		ourShader->setMat4("view", view);
		ourShader->setMat4("projection", m_scene->m_camera->m_projectionMatrix);
		ourShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
		light->activate_lights(ourShader, m_scene->m_camera);

		animShader->use();
		animShader->setVec3("color", { 1, 1, 1 });
		animShader->setMat4("view", view);
		animShader->setMat4("projection", m_scene->m_camera->m_projectionMatrix);
		animShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
		light->activate_lights(animShader, m_scene->m_camera);

		waterShader->use();

		waterShader->setVec3("color", { 1, 1, 1 });
		waterShader->setMat4("view", view);
		waterShader->setMat4("projection", m_scene->m_camera->m_projectionMatrix);
		waterShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
		waterShader->setFloat("uTime", totalTime);
		waterShader->setFloat("near_plane", near_plane);
		waterShader->setFloat("far_plane", far_plane);
		waterShader->setVec4("camera_params", ucamera_params);
		waterShader->setVec4("uScreenSize", uScreenSize);

		light->activate_lights(waterShader, m_scene->m_camera);

		//m_scene->Render(ourShader);
		m_scene->RenderAllWitTheirShader();
		particleRenderer->Render(m_scene->m_camera);
		//OnRenderUI();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		debugDepth->use();
		debugDepth->setFloat("near_plane", 0.1);
		debugDepth->setFloat("far_plane", 100.0);
		debugDepth->setFloat("uTime", totalTime);
		debugDepth->setFloat("gamma", defV);
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, finalColor);
		glDisable(GL_DEPTH_TEST);
		renderQuadFullScreen();
		glEnable(GL_DEPTH_TEST);
		
	}

	void Application::OnRenderUI()
	{
		m_scene->guiManager->Render();

		//textRenderer->RenderText("Position " + std::to_string(player->get_transform().m_position.x) + " " + std::to_string(player->get_transform().m_position.y), 10.0f, 60.0f, 0.5f, glm::vec3(1.0, 0.8f, 1.0f));
		//textRenderer->RenderText("Position " + std::to_string(mouseCursor->mousePos.x) + " " + std::to_string(mouseCursor->mousePos.y), 10.0f, 60.0f, 0.5f, glm::vec3(1.0, 0.8f, 1.0f));
		switch (m_scene->gameManager->isWin())
		{
		case GState::Win:
			//m_scene->textRenderer->RenderText("win", 10.0f, 60.0f, 0.5f, glm::vec3(1.0, 0.8f, 1.0f));
			break;
		case GState::Lose:
			//m_scene->textRenderer->RenderText("lost", 10.0f, 60.0f, 0.5f, glm::vec3(1.0, 0.8f, 1.0f));
			break;
		case GState::Playing:
			//textRenderer->RenderText("value: " + std::to_string(defV), 10.0f, 60.0f, 0.5f, glm::vec3(1.0, 0.8f, 1.0f));
			break;
		default:
			break;
		}

		if(m_scene->gameManager->isWin() != GState::Menu)
			m_scene->textRenderer->RenderText(std::to_string(m_scene->gameManager->getTime()), 1600.0f, 800.0f, 1.0f, glm::vec3(1.0, 1.0f, 1.0f));

		//textRenderer->RenderText("Position " + std::to_string(mouseX) + " " + std::to_string( mouseY), 10.0f, 60.0f, 0.5f, glm::vec3(1.0, 0.8f, 1.0f));

		GuiComponent::windowCh = false;
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

	void Application::renderQuadFullScreen()
	{
		if (quadVAO == 0)
		{
			float quadVertices[] = {
				// positions        // texture Coords
				 -1.0f,  1.0f, -1.5f, 0.0f, 1.0f,
				 -1.0f,  -1.0f, -1.5f, 0.0f, 0.0f,
				 1.0f,  1.0f, -1.5f, 1.0f, 1.0f,
				 1.0f,  -1.0f, -0.5f, 1.0f, 0.0f,
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
		float xoffset = e.mx - lastX;
		float yoffset = lastY - e.my; // reversed since y-coordinates go from bottom to top

		lastX = e.mx;
		lastY = e.my;
		m_scene->m_camera->ProcessMouseMovement(xoffset, yoffset);
	}

	void Application::loadScene()
	{
		switch (sceneNumb) {
		case 0:
			makemenucene();
			break;
		case 1:
			make1scene();
			break;
		case 2:
			make2scene();
			break;
		case 3: 
			maketestscene();
			break;
		case 4:
			make3scene();
			break;
		case 5:
			make4scene();
			break;
		default:
			makemenucene();
			break;
		}
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
				if (m_scene->getName() != "mainMenu")
				{
					makemenucene();
				}
				else
				{
					glfwSetWindowShouldClose(windowManager.window, true);

				}
			}

			if (e.key == GLFW_KEY_M)
			{
				playAudio("TestSound");
			}

			if (e.key == GLFW_KEY_1)
			{
				make1scene();
			}
			
			if (e.key == GLFW_KEY_2)
			{
				make2scene();
			}

			if (e.key == GLFW_KEY_3)
			{
				maketestscene();
			}

			if (e.key == GLFW_KEY_4)
			{
				make3scene();
			}

			if (e.key == GLFW_KEY_5)
			{
				make4scene();
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

			if (e.key == GLFW_KEY_K)
			{
				jsonParser->SerializeScene(m_scene);
			}

			break;

		case EventTypes::MousePress:
			if (e.button == 0)
			{
				m_scene->guiManager->Click();
			}
			break;
		case EventTypes::MouseRelease:
			if (e.button == 0)
			{
				m_scene->guiManager->stopClick();
			}
			break;
		case EventTypes::KeyRelease:
			if (m_gameState == GameState::PlayingState)
			{
				stopCamera(e);
			}
			break;

		case EventTypes::MouseMove:
			mouseCursor->mousePos.x = e.mx - windowManager.SCR_WIDTH / 2;
			mouseCursor->mousePos.y = windowManager.SCR_HEIGHT / 2 - e.my;

			if (m_gameState == GameState::PlayingState)
			{
				if (firstMouse)
				{
					lastX = e.mx;
					lastY = e.my;
					firstMouse = false;
				}

				rotateCamera(e);
			}
			break;
		case EventTypes::WindowResize:
			// make sure the viewport matches the new window dimensions; note that width and 
			// height will be significantly larger than specified on retina displays.
			glViewport(0, 0, e.wx, e.wy);
			if (e.wy == 0)
			{
				windowManager.SCR_HEIGHT = 1;
			}
			else
			{
				windowManager.SCR_HEIGHT = e.wy;
			}

			if (e.wx == 0)
			{
				windowManager.SCR_WIDTH = 1;
			}
			else 
			{
				windowManager.SCR_WIDTH = e.wx;
			}

			m_scene->m_camera->scr_width = windowManager.SCR_WIDTH;
			m_scene->m_camera->scr_height = windowManager.SCR_HEIGHT;

			GuiComponent::setScrWidth(windowManager.SCR_WIDTH);
			GuiComponent::setScrHeight(windowManager.SCR_HEIGHT);

			uScreenSize = glm::vec4(windowManager.SCR_WIDTH, windowManager.SCR_HEIGHT, 1 / windowManager.SCR_WIDTH, 1 / windowManager.SCR_HEIGHT);

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