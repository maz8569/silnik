#include "Application.h"

namespace GameEngine {

	Application app;

	void print()
	{
		std::cout << "OtherFunction";
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

		guiManager = CreateRef<GuiManager>(mouseCursor, m_scene->m_camera);
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

		ourShader = CreateRef<Shader>(Shader("res/shaders/basic.vert", "res/shaders/basic.frag"));
		refrShader = CreateRef<Shader>(Shader("res/shaders/refract.vert", "res/shaders/refract.frag"));
		waterShader = CreateRef<Shader>(Shader("res/shaders/water.vert", "res/shaders/water.frag"));
		shadowMap = CreateRef<Shader>("res/shaders/shadowmapping.vert", "res/shaders/shadowmapping.frag");
		foaMap = CreateRef<Shader>("res/shaders/shadowmappingBend.vert", "res/shaders/shadowmappingBend.frag");
		debugDepth = CreateRef<Shader>("res/shaders/debugdepth.vert", "res/shaders/debugdepth.frag");
		masterRenderer = CreateRef<MasterRenderer>();
		masterRenderer->setQuadShader(ourShader);

		Ref<Model> b = CreateRef<Model>(Model("res/models/cube/cube.obj"));
		Ref<Model> bu = CreateRef<Model>(Model("res/models/statek/untitled.obj"));
		Ref<Model> dom = CreateRef<Model>(Model("res/models/dom/dom_p.obj"));
		Ref<Model> pacz = CreateRef<Model>(Model("res/models/paczka/paczka.obj"));
		Ref<Model> mo = CreateRef<Model>(Model("res/models/lowpolymost/niby_most.obj"));
		//Ref<Model> island = CreateRef<Model>(Model("res/models/island/island.obj"));
		Ref<Model> island = CreateRef<Model>(Model("res/models/islandNew/wyspa.obj"));

		//Ref<GTexture> texture = CreateRef<GTexture>("res/textures/voronoi.png");

		// Setup Dear ImGui binding
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		ourShader->use();
		ourShader->setInt("shadowMap", 1);
		ourShader->setInt("cameraDepthMap", 2);
		ourShader->setInt("ourTexture", 0);

		refrShader->use();
		refrShader->setInt("shadowMap", 1);
		refrShader->setInt("cameraDepthMap", 2);
		waterShader->setInt("colorTexture", 3);
		refrShader->setInt("ourTexture", 0);

		waterShader->use();
		waterShader->setInt("foamTexture", 2);
		waterShader->setInt("refractTexture", 3);
		waterShader->setInt("ourTexture", 0);

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
		
		glGenFramebuffers(1, &foamMapFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, foamMapFBO);

		glGenTextures(1, &gPosition);
		glBindTexture(GL_TEXTURE_2D, gPosition);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, windowManager.SCR_WIDTH, windowManager.SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		/*
		glGenTextures(1, &gNormal);
		glBindTexture(GL_TEXTURE_2D, gNormal);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

		// - color + specular color buffer
		glGenTextures(1, &gAlbedoSpec);
		glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);
		*/
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

		quad = CreateRef<Shape>(Shape(Coords::COORDSTEXT));
		light = CreateRef<DirectionalLight>(DirectionalLight({ -1.5f, -3.0f, 1.5f }));

		light->activate_lights(ourShader, m_scene->m_camera);
		light->activate_lights(refrShader, m_scene->m_camera);
		light->activate_lights(waterShader, m_scene->m_camera);

		guiManager->addComponent(std::string("res/textures/back.png"), glm::vec2(-720, 370), glm::vec2(300, 150));
		guiManager->addComponent(std::string("res/textures/back.png"), glm::vec2(720, 370), glm::vec2(-300, 150));
		guiManager->addComponent(std::string("res/textures/hourglass.png"), glm::vec2(800, 370), glm::vec2(150, 150));
		guiManager->addComponent(std::string("res/textures/box.png"), glm::vec2(-795, 375), glm::vec2(100, 100));
		Ref<GuiComponent> numbComponent = guiManager->addComponent(std::string("res/textures/numb1.png"), glm::vec2(-690, 360), glm::vec2(60, 60));
		/*
		guiManager->addComponent(std::string("res/textures/numb0.png"), glm::vec2(580, 360), glm::vec2(60, 60));
		guiManager->addComponent(std::string("res/textures/numb0.png"), glm::vec2(660, 360), glm::vec2(60, 60));
		guiManager->addComponent(std::string("res/textures/numb0.png"), glm::vec2(800, 360), glm::vec2(60, 60));
		guiManager->addComponent(std::string("res/textures/numb0.png"), glm::vec2(880, 360), glm::vec2(60, 60));
		guiManager->addComponent(std::string("res/textures/numbdoublep.png"), glm::vec2(720, 360), glm::vec2(60, 60));
		*/
		gameManager = CreateRef<GameManager>(1, numbComponent);

		Ref<GTexture> texture0 = CreateRef<GTexture>("res/textures/numb0.png");;

		gameManager->addTexture(texture0);
		gameManager->addTexture(numbComponent->getTexture());

		gameManager->init();

		//mousePicker = CreateRef<MousePicker>(MousePicker(m_scene->m_camera, inputManager));
		player = CreateRef<Player>(inputManager, gameManager, b, colMan);
		player->shader = ourShader;
		//courier = std::make_shared<Courier>(mousePicker, bu, colMan);
		//courier->set_local_position({ 20, -20, 0 });
		//courier->getModel()->shader = ourShader;
		//courier->set_local_rotation({ 90, 0, 0 });
		player->set_local_position({ -2, 0, 0 });
		//player->set_render_AABB(true);
		//courier->set_render_AABB(true);
		//courier->set_color({ 1, 0.0, 0.0 });

		Ref<GObject> boat = CreateRef<GObject>(bu, colMan);
		boat->shader = ourShader;

		std::vector<glm::vec3> pos;

		pos.push_back({ 6, -7.5, 6 });
		pos.push_back({ 6, -7.5, -6 });
		pos.push_back({ -6, -7.5, -6 });
		pos.push_back({ -6, -7.5, 6 });

		Ref<Boat> boatComp = CreateRef<Boat>(2, pos);
		boat->addComponent(boatComp);
		boat->set_local_scale(glm::vec3(1.5));

		b = CreateRef<Model>(Model("res/models/hidefCube/cube.obj"));
		bu = CreateRef<Model>(Model("res/models/hidefPlane/plane.obj"));
		// TODO: move to scene
		{
			Ref<GObject> gdom = CreateRef<Box>(DeliveryColor::Blue, dom, colMan);
			gdom->shader = ourShader;
			gdom->set_local_position({ 0, -7, -13 });
			gdom->getAABB()->setStatic(true);
			//gdom->set_render_AABB(true);
			gdom->set_tag("house");

			Ref<GObject> paczka = CreateRef<Box>(DeliveryColor::Blue, pacz, colMan);
			paczka->shader = ourShader;
			paczka->set_local_position({ 0, -7.2, 13 });
			paczka->getAABB()->setStatic(true);
			//paczka->set_render_AABB(true);
			paczka->set_tag("package");

			Ref<GObject> most = CreateRef<GObject>(mo, colMan);
			most->shader = ourShader;
			most->set_local_position({ 0, -7.2, -4 });
			most->set_local_rotation({ 0, -90, 0 });
			most->getAABB()->setStatic(true);
			most->rotateAABB(Degrees::D90, Axis::Z);
			//most->set_render_AABB(true);

			Ref<GObject> most2 = CreateRef<GObject>(mo, colMan);
			most2->shader = ourShader;
			most2->set_local_position({ 0, -7.2, -8.5f });
			most2->set_local_rotation({ 0, 90, 0 });
			most2->getAABB()->setStatic(true);
			most2->rotateAABB(Degrees::D270, Axis::Z);
			//most2->set_render_AABB(true);

			Ref<GObject> most3 = CreateRef<GObject>(mo, colMan);
			most3->shader = ourShader;
			most3->set_local_position({ 0, -7.2, 8.5f });
			most3->set_local_rotation({ 0, -90, 0 });
			most3->getAABB()->setStatic(true);
			most3->rotateAABB(Degrees::D90, Axis::Z);
			//most3->set_render_AABB(true);

			Ref<GObject> most4 = CreateRef<GObject>(mo, colMan);
			most4->shader = ourShader;
			most4->set_local_position({ 0, -7.2, 4 });
			most4->set_local_rotation({ 0, 90, 0 });
			most4->getAABB()->setStatic(true);
			most4->rotateAABB(Degrees::D270, Axis::Z);
			//most4->set_render_AABB(true);

			Ref<GObject> most5 = CreateRef<GObject>(mo, colMan);
			most5->shader = ourShader;
			most5->set_local_position({4, -7.2, 0 });
			most5->set_local_rotation({ 0, 0, 0 });
			most5->getAABB()->setStatic(true);
			//most5->set_render_AABB(true);

			Ref<GObject> most6 = CreateRef<GObject>(mo, colMan);
			most6->shader = ourShader;
			most6->set_local_position({ 8.5f, -7.2, 0 });
			most6->set_local_rotation({ 0, 180, 0 });
			most6->getAABB()->setStatic(true);
			most6->rotateAABB(Degrees::D180, Axis::Z);
			//most6->set_render_AABB(true);

			Ref<GObject> most7 = CreateRef<GObject>(mo, colMan);
			most7->shader = ourShader;
			most7->set_local_position({ -8.5f, -7.2, 0 });
			most7->set_local_rotation({ 0, 0, 0 });
			most7->getAABB()->setStatic(true);
			//most7->set_render_AABB(true);

			Ref<GObject> most8 = CreateRef<GObject>(mo, colMan);
			most8->shader = ourShader;
			most8->set_local_position({ -4, -7.2, 0 });
			most8->set_local_rotation({ 0, 180, 0 });
			most8->getAABB()->setStatic(true);
			most8->rotateAABB(Degrees::D180, Axis::Z);
			//most8->set_render_AABB(true);

			Ref<GObject> iisland2 = CreateRef<GObject>(island, colMan);
			iisland2->shader = ourShader;
			iisland2->set_local_scale({ 0.6, 0.6, 0.6 });
			//iisland2->scaleAABB({ 0.6, 0.6, 0.6 });
			iisland2->setAABBextentY(0.9f);
			iisland2->set_local_position({ 13, -9.8, 0 });
			iisland2->set_tag("terrain");
			iisland2->getAABB()->setStatic(true);
			//iisland2->set_render_AABB(true);

			Ref<GObject> iisland3 = CreateRef<GObject>(island, colMan);
			iisland3->shader = ourShader;
			iisland3->set_local_scale({ 0.6, 0.6, 0.6 });
			//iisland3->scaleAABB({ 0.6, 0.6, 0.6 });
			iisland3->setAABBextentY(0.9f);
			iisland3->set_local_position({ -13, -9.8, 0 });
			iisland3->set_tag("terrain");
			iisland3->getAABB()->setStatic(true);

			Ref<GObject> iisland4 = CreateRef<GObject>(island, colMan);
			iisland4->shader = ourShader;
			iisland4->set_local_scale({ 0.6, 0.6, 0.6 });
			//iisland4->scaleAABB({ 0.6, 0.6, 0.6 });
			iisland4->setAABBextentY(0.9f);
			iisland4->set_local_position({ 0, -9.8, 13 });
			iisland4->set_tag("terrain");
			iisland4->getAABB()->setStatic(true);

			Ref<GObject> iisland5 = CreateRef<GObject>(island, colMan);
			iisland5->shader = ourShader;
			iisland5->set_local_scale({ 0.6, 0.6, 0.6 });
			//iisland5->scaleAABB({ 0.6, 0.6, 0.6 });
			iisland5->setAABBextentY(0.9f);
			iisland5->set_local_position({ 0, -9.8, -13 });
			iisland5->set_tag("terrain");
			iisland5->getAABB()->setStatic(true);
			
			Ref<GObject> iisland = CreateRef<GObject>(island, colMan);
			iisland->shader = ourShader;
			iisland->set_local_scale({ 0.6, 0.6, 0.6 });
			//iisland->scaleAABB({ 0.6, 0.6, 0.6 });
			iisland->setAABBextentY(0.9f);
			iisland->set_local_position({ 0, -9.8, 0 });
			iisland->set_tag("terrain");
			iisland->getAABB()->setStatic(true);
			
			Ref<GObject> sand = CreateRef<GObject>(b, colMan);
			sand->shader = ourShader;
			sand->set_local_scale({ 60, 1, 60 });
			//water->setAABBextentY(0.9f);
			sand->set_local_position({ 0, -15, 0 });
			sand->getAABB()->setStatic(true);
			sand->set_color({ 0.63, 0.68, 0.1 });
			//sand->cast_shadow = false;


			Ref<GObject> water = CreateRef<GObject>(bu, colMan);
			water->shader = waterShader;
			water->set_local_scale({ 60, 4, 60 });
			water->setAABBextentY(1);
			//water->setAABBextentY(0.9f);
			water->set_local_position({ 0, -9.5, 0 });
			water->getAABB()->setStatic(true);
			water->set_tag("water");
			water->set_color({ 0.38, 0.44, 0.91 });
			water->cast_shadow = false;
			//water->getModel()->meshes[0].textures[0].id = texture->GetTextureID();

			m_scene->addObjectToScene(gameManager);
			m_scene->addObjectToScene(player);
			m_scene->addObjectToScene(boat);
			m_scene->addObjectToScene(iisland);
			m_scene->addObjectToScene(iisland2);
			m_scene->addObjectToScene(iisland3);
			m_scene->addObjectToScene(iisland4);
			m_scene->addObjectToScene(iisland5);
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
			m_scene->addObjectToScene(sand);
			m_scene->addObjectToScene(water);

			//root->update_transform(root->get_transform(), true);
		}
		m_scene->m_camera->player = player;

		float n = 0.1f;
		float f = 100.0f;

		ucamera_params = glm::vec4(1/f, f, (1 - f/ n)/2, (1 + f / n) / 2);

		// load and create a texture 
		// -------------------------
		stbi_set_flip_vertically_on_load(true);
		texture1.loadFromFile(std::string("res/textures/torus.png").c_str());

		lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, near_plane, far_plane);
		jsonParser->print();

		std::cout << "Starting Game...\n";

		passed_time = 0.0;
		double current_time = 0.0;
		should_render = false;

		double last_time = glfwGetTime();
		double unprocessed_time = 0.0;

		playAudio("TestSound");
		
		//Ref<GuiComponent> but = guiManager->addComponent(std::string("res/textures/fullheart.png"), glm::vec2(-300, 300), glm::vec2(48, 48), 0);
		//guiManager->addComponent(std::string("res/textures/torus.png"), glm::vec2( 550, -310 ), glm::vec2( 62, 6 ), 0);
		//Ref<Slider> slider = guiManager->addSlider(-1, 2.2, &defV, std::string("res/textures/fullheart.png"), glm::vec2( 550, -310 ), glm::vec2( 24, 24));

		//but->setOnClickFunction(print);

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
				//OnRenderUI();

				windowManager.updateWindow();
			}

		}
		std::cout << "\nThanks for playing!";
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
		refrShader->setMat4("view", view);
		waterShader->setMat4("view", view);

		m_scene->Update(dt);

		//m_scene->m_camera->courier = courier->get_transform().m_position;
		colMan->CollisionCheck();

		guiManager->Update();
		ourShader->setVec3("cameraPos", m_scene->m_camera->Position);
		refrShader->setVec3("cameraPos", m_scene->m_camera->Position);
		waterShader->setVec3("cameraPos", m_scene->m_camera->Position);

		//mousePicker->Update();

		if(!gameManager->isWin())
			gameManager->setTime(gameManager->getTime() - dt);
	}

	void Application::OnRender()
	{
		masterRenderer->finishRender();

		// activate shader
		m_scene->m_camera->m_projectionMatrix = glm::perspective(glm::radians(m_scene->m_camera->Zoom), (float)windowManager.SCR_WIDTH / (float)windowManager.SCR_HEIGHT, 0.1f, 100.0f);
		lightView = glm::lookAt(-light->lightPos, glm::vec3(0), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;
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

		m_scene->RenderAllWitTheirShader();
		OnRenderUI();
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
		guiManager->Render();

		//textRenderer->RenderText("Position " + std::to_string(player->get_transform().m_position.x) + " " + std::to_string(player->get_transform().m_position.y), 10.0f, 60.0f, 0.5f, glm::vec3(1.0, 0.8f, 1.0f));
		//textRenderer->RenderText("Position " + std::to_string(mouseCursor->mousePos.x) + " " + std::to_string(mouseCursor->mousePos.y), 10.0f, 60.0f, 0.5f, glm::vec3(1.0, 0.8f, 1.0f));
		if (gameManager->isWin())
		{
			textRenderer->RenderText("win", 10.0f, 60.0f, 0.5f, glm::vec3(1.0, 0.8f, 1.0f));
		}
		else
		{
			textRenderer->RenderText("value: " + std::to_string(defV), 10.0f, 60.0f, 0.5f, glm::vec3(1.0, 0.8f, 1.0f));
		}
		textRenderer->RenderText(std::to_string(gameManager->getTime()), 1600.0f, 800.0f, 1.0f, glm::vec3(1.0, 1.0f, 1.0f));

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

		case EventTypes::MousePress:
			if (e.button == 0)
			{
				guiManager->Click();
			}
			break;
		case EventTypes::MouseRelease:
			if (e.button == 0)
			{
				guiManager->stopClick();
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