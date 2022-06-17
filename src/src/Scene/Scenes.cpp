#include <Application.h>

namespace GameEngine {


	void exit()
	{
		glfwSetWindowShouldClose(app.windowManager.window, true);
	}

	void testscene()
	{
		app.maketestscene();
	}

	void scene1()
	{
		app.make1scene();
	}

	void scene2()
	{
		app.make2scene();
	}

	void scene3()
	{
		app.make3scene();
	}

	void Application::maketestscene()
	{
		if (m_scene != nullptr)
			delete m_scene;

		m_scene = new Scene("scenetest");
		m_scene->guiManager = CreateRef<GuiManager>(mouseCursor, m_scene->m_camera);

		light = CreateRef<DirectionalLight>(DirectionalLight({ -0.5f, -1.0f, 0.5f }));

		light->activate_lights(ourShader, m_scene->m_camera);
		light->activate_lights(refrShader, m_scene->m_camera);
		light->activate_lights(waterShader, m_scene->m_camera);


		m_scene->guiManager->addComponent(std::string("back"), glm::vec2(-720, 370), glm::vec2(300, 150));
		m_scene->guiManager->addComponent(std::string("back"), glm::vec2(720, 370), glm::vec2(-300, 150));
		m_scene->guiManager->addComponent(std::string("hourglass"), glm::vec2(800, 370), glm::vec2(150, 150));
		m_scene->guiManager->addComponent(std::string("box"), glm::vec2(-795, 375), glm::vec2(100, 100));
		Ref<GuiComponent> numbComponent = m_scene->guiManager->addComponent(std::string("numb1"), glm::vec2(-690, 360), glm::vec2(60, 60));
		Ref<GuiComponent> but = m_scene->guiManager->addComponent(std::string("fullheart"), glm::vec2(650, -350), glm::vec2(48, 48), 0);
		but->setOnClickFunction(scene3);

		m_scene->gameManager = CreateRef<GameManager>(1, numbComponent);

		Ref<GTexture> texture0 = ResourceManager::getTexture("numb0");
		Ref<GTexture> fh = ResourceManager::getTexture("fullheart");

		m_scene->gameManager->addTexture(texture0);
		m_scene->gameManager->addTexture(numbComponent->getTexture());

		m_scene->gameManager->init();

		Ref<GObject> player = CreateRef<GObject>("ludek");
		//player->set_local_scale({ 0.01, 0.01, 0.01 });
		player->set_local_scale({ 0.005, 0.005, 0.005 });
		Ref<AABB> aabb = CreateRef<AABB>(glm::vec3(0, 0, 0), 0.5, 0.5, 0.5);
		player->setAABB(aabb);
		player->setAABBoffsetY(0.5f);
		//player->cast_shadow = false;

		Ref<Player> plaComp = CreateRef<Player>(inputManager, m_scene->gameManager);
		player->addComponent(plaComp);
		player->shader = animShader;
		player->set_local_position({ -2, 0, 0 });
		player->set_tag("player");

		Ref<GObject> boat = CreateRef<GObject>("boat");
		boat->shader = ourShader;
		boat->scaleAABB({ 0.8, 0.8, 0.8 });
		//Ref<ParticleSystem> partsys = CreateRef<ParticleSystem>(5, "fullheart");
		//player->addComponent(partsys);
		//particleRenderer->addParticleSystem(partsys);
		std::vector<glm::vec3> pos;
		boat->set_render_AABB(true);

		pos.push_back({ 6, -7.5, 6 });
		pos.push_back({ 6, -7.5, -6 });
		pos.push_back({ -6, -7.5, -6 });
		pos.push_back({ -6, -7.5, 6 });

		Ref<Boat> boatComp = CreateRef<Boat>(3, pos);
		boat->addComponent(boatComp);

		Ref<Vehicle> vehicle = CreateRef<Vehicle>(glm::vec2(0, 1), 5.f, glm::vec2(0, 1), 1.f, 10.f, 5.f);
		vehicle->addSteeringBehavior(player);

		//boat->addComponent(vehicle);

		boat->set_local_scale(glm::vec3(1.5));
		boat->set_tag("boat");

		Ref<GObject> boatCollision = CreateRef<GObject>("");
		boatCollision->set_local_position({ 0, -5, -10 });
		Ref<AABB> bAABB = CreateRef<AABB>(glm::vec3(0, 0, 0), 7, 1, 7);
		boatCollision->setAABB(bAABB);
		boatCollision->set_tag("Tboat");
		boatCollision->shader = ourShader;

		Ref<Stealing> stealing = CreateRef<Stealing>(3);
		stealing->addBoat(boat);
		boatCollision->addComponent(stealing);
		boatCollision->set_render_AABB(true);

		aabb = CreateRef<AABB>(glm::vec3(0, 0, 0), 3, 0.5, 3);
		Ref<BridgeTrigger> bri = CreateRef<BridgeTrigger>();
		Ref<GObject> bridgeTrigger = CreateRef<GObject>("");
		bridgeTrigger->set_local_position({ -6, -7, 0 });
		bridgeTrigger->shader = ourShader;

		bridgeTrigger->setAABB(aabb);
		bridgeTrigger->set_tag("Tbridge");
		bridgeTrigger->getAABB()->setStatic(true);
		bridgeTrigger->addComponent(bri);

		Ref<AABB> aabb1 = CreateRef<AABB>(glm::vec3(0, 0, 0), 3, 0.5, 3);
		Ref<BridgeTrigger> bri1 = CreateRef<BridgeTrigger>();
		Ref<GObject> bridgeTrigger1 = CreateRef<GObject>("");
		bridgeTrigger1->set_local_position({ 6, -7, 0 });
		bridgeTrigger1->shader = ourShader;

		bridgeTrigger1->setAABB(aabb1);
		bridgeTrigger1->set_tag("Tbridge");
		bridgeTrigger1->getAABB()->setStatic(true);
		bridgeTrigger1->addComponent(bri1);

		Ref<AABB> aabb2 = CreateRef<AABB>(glm::vec3(0, 0, 0), 3, 0.5, 3);
		Ref<BridgeTrigger> bri2 = CreateRef<BridgeTrigger>();
		Ref<GObject> bridgeTrigger2 = CreateRef<GObject>("");
		bridgeTrigger2->set_local_position({ 0, -7, -6 });
		bridgeTrigger2->shader = ourShader;

		bridgeTrigger2->setAABB(aabb2);
		bridgeTrigger2->set_tag("Tbridge");
		bridgeTrigger2->getAABB()->setStatic(true);
		bridgeTrigger2->addComponent(bri2);

		Ref<AABB> aabb3 = CreateRef<AABB>(glm::vec3(0, 0, 0), 3, 0.5, 3);
		Ref<BridgeTrigger> bri3 = CreateRef<BridgeTrigger>();
		Ref<GObject> bridgeTrigger3 = CreateRef<GObject>("");
		bridgeTrigger3->set_local_position({ 0, -7, 6 });
		bridgeTrigger3->shader = ourShader;

		bridgeTrigger3->setAABB(aabb3);
		bridgeTrigger3->set_tag("Tbridge");
		bridgeTrigger3->getAABB()->setStatic(true);
		bridgeTrigger3->addComponent(bri3);

		Ref<GObject> gdom = CreateRef<GObject>("house");
		Ref<Box> box = CreateRef<Box>(DeliveryColor::Blue);
		gdom->addComponent(box);
		gdom->shader = ourShader;
		gdom->set_local_position({ 0, -7, -13 });
		gdom->getAABB()->setStatic(true);
		//gdom->set_render_AABB(true);
		gdom->set_tag("house");
		/*
		Ref<GObject> paczka = CreateRef<GObject>("package");
		Ref<Box> boxp = CreateRef<Box>(DeliveryColor::Blue);
		paczka->addComponent(boxp);
		paczka->shader = ourShader;
		paczka->set_local_position({ 0, -7.2, 13 });
		paczka->getAABB()->setStatic(true);
		//paczka->set_render_AABB(true);
		paczka->set_tag("package");
		*/
		Ref<GObject> dirt = CreateRef<GObject>("dirt");
		dirt->shader = ourShader;
		dirt->set_local_position({ 13, -9.8, 0 });

		Ref<Dig> digDirt = CreateRef<Dig>(3, DeliveryColor::Blue, inputManager);
		dirt->addComponent(digDirt);

		Ref<GObject> most = CreateRef<GObject>("bridge");
		most->shader = ourShader;
		most->set_local_position({ 0, -7.2, -4 });
		most->set_local_rotation({ 0, -90, 0 });
		most->getAABB()->setStatic(true);
		most->rotateAABB(Degrees::D90, Axis::Y);
		//most->set_render_AABB(true);
		most->set_tag("dB");

		Ref<GObject> most2 = CreateRef<GObject>("bridge");
		most2->shader = ourShader;
		most2->set_local_position({ 0, -7.2, -8.5f });
		most2->set_local_rotation({ 0, 90, 0 });
		most2->getAABB()->setStatic(true);
		most2->setAABBrotation(Degrees::D270, Axis::Y);
		//most2->set_render_AABB(true);
		most2->set_tag("uB");

		bri2->bridgepart1 = most;
		bri2->bridgepart2 = most2;

		Ref<GObject> most3 = CreateRef<GObject>("bridge");
		most3->shader = ourShader;
		most3->set_local_position({ 0, -7.2, 8.5f });
		most3->set_local_rotation({ 0, -90, 0 });
		most3->getAABB()->setStatic(true);
		most3->rotateAABB(Degrees::D90, Axis::Y);
		//most3->set_render_AABB(true);
		most3->set_tag("dB");

		Ref<GObject> most4 = CreateRef<GObject>("bridge");
		most4->shader = ourShader;
		most4->set_local_position({ 0, -7.2, 4 });
		most4->set_local_rotation({ 0, 90, 0 });
		most4->getAABB()->setStatic(true);
		most4->setAABBrotation(Degrees::D270, Axis::Y);
		//most4->set_render_AABB(true);
		most4->set_tag("uB");

		bri3->bridgepart1 = most3;
		bri3->bridgepart2 = most4;

		Ref<GObject> most5 = CreateRef<GObject>("bridge");
		most5->shader = ourShader;
		most5->set_local_position({ 4, -7.2, 0 });
		most5->set_local_rotation({ 0, 0, 0 });
		most5->getAABB()->setStatic(true);
		//most5->set_render_AABB(true);

		Ref<GObject> most6 = CreateRef<GObject>("bridge");
		most6->shader = ourShader;
		most6->set_local_position({ 8.5f, -7.2, 0 });
		most6->set_local_rotation({ 0, 180, 0 });
		most6->getAABB()->setStatic(true);
		most6->setAABBrotation(Degrees::D180, Axis::Y);
		//most6->set_render_AABB(true);

		bri1->bridgepart1 = most5;
		bri1->bridgepart2 = most6;

		Ref<GObject> most7 = CreateRef<GObject>("bridge");
		most7->shader = ourShader;
		most7->set_local_position({ -8.5f, -7.2, 0 });
		most7->set_local_rotation({ 0, 0, 0 });
		most7->getAABB()->setStatic(true);
		//most7->set_render_AABB(true);

		Ref<GObject> most8 = CreateRef<GObject>("bridge");
		most8->shader = ourShader;
		most8->set_local_position({ -4, -7.2, 0 });
		most8->set_local_rotation({ 0, 180, 0 });
		most8->getAABB()->setStatic(true);
		most8->setAABBrotation(Degrees::D180, Axis::Y);
		bri->bridgepart1 = most7;
		bri->bridgepart2 = most8;

		Ref<GObject> iisland2 = CreateRef<GObject>("island");
		iisland2->shader = ourShader;
		iisland2->set_local_scale({ 0.6, 0.6, 0.6 });
		iisland2->setAABBextentY(0.9f);
		iisland2->set_local_position({ 13, -9.8, 0 });
		iisland2->set_tag("terrain");
		iisland2->getAABB()->setStatic(true);
		//iisland2->set_render_AABB(true);
		Ref<Island> island2 = CreateRef<Island>(true);
		iisland2->addComponent(island2);


		Ref<GObject> iisland3 = CreateRef<GObject>("island");
		iisland3->shader = ourShader;
		iisland3->set_local_scale({ 0.6, 0.6, 0.6 });
		//iisland3->scaleAABB({ 0.6, 0.6, 0.6 });
		iisland3->setAABBextentY(0.9f);
		iisland3->set_local_position({ -13, -9.8, 0 });
		iisland3->set_tag("terrain");
		iisland3->getAABB()->setStatic(true);
		Ref<Island> island3 = CreateRef<Island>(true);
		iisland3->addComponent(island3);


		Ref<GObject> iisland4 = CreateRef<GObject>("island");
		iisland4->shader = ourShader;
		iisland4->set_local_scale({ 0.6, 0.6, 0.6 });
		//iisland4->scaleAABB({ 0.6, 0.6, 0.6 });
		iisland4->setAABBextentY(0.9f);
		iisland4->set_local_position({ 0, -9.8, 13 });
		iisland4->set_tag("terrain");
		iisland4->getAABB()->setStatic(true);
		iisland4->set_render_AABB(true);
		Ref<Island> island4= CreateRef<Island>(true);
		iisland4->addComponent(island4);

		Ref<GObject> iisland5 = CreateRef<GObject>("island");
		iisland5->shader = ourShader;
		iisland5->set_local_scale({ 0.6, 0.6, 0.6 });
		iisland5->setAABBextentY(0.9f);
		iisland5->set_local_position({ 0, -9.8, -13 });
		iisland5->set_tag("terrain");
		iisland5->getAABB()->setStatic(true);
		Ref<Island> island5 = CreateRef<Island>(false);
		iisland5->addComponent(island5);

		Ref<GObject> iisland = CreateRef<GObject>("island");
		iisland->shader = ourShader;
		iisland->set_local_scale({ 0.6, 0.6, 0.6 });
		iisland->setAABBextentY(0.9f);
		iisland->set_local_position({ 0, -9.8, 0 });
		iisland->set_tag("terrain");
		iisland->getAABB()->setStatic(true);
		Ref<Island> islandd = CreateRef<Island>(false);
		iisland->addComponent(islandd);

		Ref<GObject> sand = CreateRef<GObject>("hidefCube");
		sand->shader = ourShader;
		sand->set_local_scale({ 60, 1, 60 });
		//water->setAABBextentY(0.9f);
		sand->set_local_position({ 0, -15, 0 });
		sand->getAABB()->setStatic(true);
		sand->set_color({ 0.63, 0.68, 0.1 });
		//sand->cast_shadow = false;


		Ref<GObject> water = CreateRef<GObject>("hidefPlane");
		water->shader = waterShader;
		water->set_local_scale({ 60, 4, 60 });
		water->setAABBextentY(1.1);
		//water->setAABBextentY(0.9f);
		water->set_local_position({ 0, -9.5, 0 });
		water->getAABB()->setStatic(true);
		water->set_tag("water");
		water->set_color({ 0.38, 0.44, 0.91 });
		water->cast_shadow = false;

		Ref<GObject> water2 = CreateRef<GObject>("hidefPlane");
		water2->shader = waterShader;
		water2->set_local_scale({ 60, 4, 60 });
		water2->setAABBextentY(1.1);
		//water->setAABBextentY(0.9f);
		water2->set_local_position({ 60, -9.5, 0 });
		water2->getAABB()->setStatic(true);
		water2->set_tag("water");
		water2->set_color({ 0.38, 0.44, 0.91 });
		water2->cast_shadow = false;


		Ref<GObject> water3 = CreateRef<GObject>("hidefPlane");
		water3->shader = waterShader;
		water3->set_local_scale({ 60, 4, 60 });
		water3->setAABBextentY(1.1);
		//water->setAABBextentY(0.9f);
		water3->set_local_position({ 0, -9.5, -60 });
		water3->getAABB()->setStatic(true);
		water3->set_tag("water");
		water3->set_color({ 0.38, 0.44, 0.91 });
		water3->cast_shadow = false;
		//water->getModel()->meshes[0].textures[0].id = texture->GetTextureID();

		m_scene->addObjectToScene(player);
		m_scene->addObjectToScene(boat);
		m_scene->addObjectToScene(boatCollision);
		//m_scene->addObjectToScene(testanim);
		m_scene->addObjectToScene(dirt);
		m_scene->addObjectToScene(iisland);
		m_scene->addObjectToScene(iisland2);
		m_scene->addObjectToScene(iisland3);
		m_scene->addObjectToScene(iisland4);
		m_scene->addObjectToScene(iisland5);
		m_scene->addObjectToScene(gdom);
		//m_scene->addObjectToScene(paczka);
		m_scene->addObjectToScene(bridgeTrigger);
		m_scene->addObjectToScene(bridgeTrigger1);
		m_scene->addObjectToScene(bridgeTrigger2);
		m_scene->addObjectToScene(bridgeTrigger3);
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
		m_scene->addObjectToScene(water2);
		m_scene->addObjectToScene(water3);

		m_scene->m_camera->player = player;
		Event e;
		e.type = EventTypes::WindowResize;
		e.wx = windowManager.SCR_WIDTH;
		e.wy = windowManager.SCR_HEIGHT;
		m_EventQueue.push_back(e);
	}

	void Application::makemenucene()
	{
		if (m_scene != nullptr)
			delete m_scene;

		m_scene = new Scene("mainMenu");
		m_scene->guiManager = CreateRef<GuiManager>(mouseCursor, m_scene->m_camera);
		light = CreateRef<DirectionalLight>(DirectionalLight({ -0.5f, -1.0f, 0.5f }));

		light->activate_lights(ourShader, m_scene->m_camera);
		light->activate_lights(refrShader, m_scene->m_camera);
		light->activate_lights(waterShader, m_scene->m_camera);

		Ref<GObject> tt = CreateRef<GObject>("");
		m_scene->m_camera->player = tt;

		m_scene->gameManager = CreateRef<GameManager>(1, nullptr);
		m_scene->gameManager->setState(GState::Menu);

		Ref<GuiComponent> butStart = m_scene->guiManager->addComponent(std::string("play"), glm::vec2(0, 200), glm::vec2(300, 100));
		Ref<GuiComponent> but = m_scene->guiManager->addComponent(std::string("exit"), glm::vec2(0, -200), glm::vec2(300, 100));
		
		butStart->setOnClickFunction(scene1);
		but->setOnClickFunction(exit);

		m_scene->addObjectToScene(tt);

		Event e;
		e.type = EventTypes::WindowResize;
		e.wx = windowManager.SCR_WIDTH;
		e.wy = windowManager.SCR_HEIGHT;
		m_EventQueue.push_back(e);
	}

	void Application::make1scene()
	{
		if (m_scene != nullptr)
			delete m_scene;

		m_scene = new Scene("scene1");
		m_scene->guiManager = CreateRef<GuiManager>(mouseCursor, m_scene->m_camera);

		light = CreateRef<DirectionalLight>(DirectionalLight({ -0.5f, -1.0f, 0.5f }));

		light->activate_lights(ourShader, m_scene->m_camera);
		light->activate_lights(refrShader, m_scene->m_camera);
		light->activate_lights(waterShader, m_scene->m_camera);

		m_scene->guiManager->addComponent(std::string("back"), glm::vec2(-720, 370), glm::vec2(300, 150));
		m_scene->guiManager->addComponent(std::string("back"), glm::vec2(720, 370), glm::vec2(-300, 150));
		m_scene->guiManager->addComponent(std::string("hourglass"), glm::vec2(800, 370), glm::vec2(150, 150));
		m_scene->guiManager->addComponent(std::string("box"), glm::vec2(-795, 375), glm::vec2(100, 100));
		Ref<GuiComponent> numbComponent = m_scene->guiManager->addComponent(std::string("numb1"), glm::vec2(-690, 360), glm::vec2(60, 60));
		Ref<GuiComponent> but = m_scene->guiManager->addComponent(std::string("fullheart"), glm::vec2(-300, 300), glm::vec2(48, 48), 0);
		but->setOnClickFunction(scene2);

		m_scene->gameManager = CreateRef<GameManager>(1, numbComponent);

		Ref<GTexture> texture0 = ResourceManager::getTexture("numb0");
		Ref<GTexture> fh = ResourceManager::getTexture("fullheart");

		m_scene->gameManager->addTexture(texture0);
		m_scene->gameManager->addTexture(numbComponent->getTexture());

		m_scene->gameManager->init();

		Ref<GObject> player = CreateRef<GObject>("cube");
		Ref<Player> plaComp = CreateRef<Player>(inputManager, m_scene->gameManager);
		player->addComponent(plaComp);
		player->shader = ourShader;
		player->set_local_position({ -2, 0, 0 });

		Ref<GObject> gdom = CreateRef<GObject>("house");
		Ref<Box> box = CreateRef<Box>(DeliveryColor::Blue);
		gdom->addComponent(box);
		gdom->shader = ourShader;
		gdom->set_local_position({ 13, -7, 0 });
		gdom->set_local_rotation({ 0, 90, 0 });
		gdom->getAABB()->setStatic(true);
		//gdom->set_render_AABB(true);
		gdom->set_tag("house");

		Ref<GObject> paczka = CreateRef<GObject>("package");
		Ref<Box> boxp = CreateRef<Box>(DeliveryColor::Blue);
		paczka->addComponent(boxp);
		paczka->shader = ourShader;
		paczka->set_local_position({ -13, -7.2, 0 });
		paczka->getAABB()->setStatic(true);
		//paczka->set_render_AABB(true);
		paczka->set_tag("package");

		Ref<GObject> most5 = CreateRef<GObject>("bridge");
		most5->shader = ourShader;
		most5->set_local_position({ 4, -7.2, 0 });
		most5->set_local_rotation({ 0, 0, 0 });
		most5->getAABB()->setStatic(true);
		//most5->set_render_AABB(true);

		Ref<GObject> most6 = CreateRef<GObject>("bridge");
		most6->shader = ourShader;
		most6->set_local_position({ 8.5f, -7.2, 0 });
		most6->set_local_rotation({ 0, 180, 0 });
		most6->getAABB()->setStatic(true);
		most6->setAABBrotation(Degrees::D180, Axis::Y);
		//most6->set_render_AABB(true);

		Ref<GObject> most7 = CreateRef<GObject>("bridge");
		most7->shader = ourShader;
		most7->set_local_position({ -8.5f, -7.2, 0 });
		most7->set_local_rotation({ 0, 0, 0 });
		most7->getAABB()->setStatic(true);
		//most7->set_render_AABB(true);

		Ref<GObject> most8 = CreateRef<GObject>("bridge");
		most8->shader = ourShader;
		most8->set_local_position({ -4, -7.2, 0 });
		most8->set_local_rotation({ 0, 180, 0 });
		most8->getAABB()->setStatic(true);
		most8->setAABBrotation(Degrees::D180, Axis::Y);

		Ref<GObject> iisland2 = CreateRef<GObject>("island");
		iisland2->shader = ourShader;
		iisland2->set_local_scale({ 0.6, 0.6, 0.6 });
		iisland2->setAABBextentY(0.9f);
		iisland2->set_local_position({ 13, -9.8, 0 });
		iisland2->set_tag("terrain");
		iisland2->getAABB()->setStatic(true);
		//iisland2->set_render_AABB(true);

		Ref<GObject> iisland3 = CreateRef<GObject>("island");
		iisland3->shader = ourShader;
		iisland3->set_local_scale({ 0.6, 0.6, 0.6 });
		//iisland3->scaleAABB({ 0.6, 0.6, 0.6 });
		iisland3->setAABBextentY(0.9f);
		iisland3->set_local_position({ -13, -9.8, 0 });
		iisland3->set_tag("terrain");
		iisland3->getAABB()->setStatic(true);

		Ref<GObject> iisland = CreateRef<GObject>("island");
		iisland->shader = ourShader;
		iisland->set_local_scale({ 0.6, 0.6, 0.6 });
		iisland->setAABBextentY(0.9f);
		iisland->set_local_position({ 0, -9.8, 0 });
		iisland->set_tag("terrain");
		iisland->getAABB()->setStatic(true);

		Ref<GObject> sand = CreateRef<GObject>("hidefCube");
		sand->shader = ourShader;
		sand->set_local_scale({ 60, 1, 60 });
		//water->setAABBextentY(0.9f);
		sand->set_local_position({ 0, -15, 0 });
		sand->getAABB()->setStatic(true);
		sand->set_color({ 0.63, 0.68, 0.1 });
		//sand->cast_shadow = false;


		Ref<GObject> water = CreateRef<GObject>("hidefPlane");
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

		m_scene->addObjectToScene(player);
		m_scene->addObjectToScene(iisland);
		m_scene->addObjectToScene(iisland2);
		m_scene->addObjectToScene(iisland3);
		m_scene->addObjectToScene(gdom);
		m_scene->addObjectToScene(paczka);
		m_scene->addObjectToScene(most5);
		m_scene->addObjectToScene(most6);
		m_scene->addObjectToScene(most7);
		m_scene->addObjectToScene(most8);
		m_scene->addObjectToScene(sand);
		m_scene->addObjectToScene(water);

		m_scene->m_camera->player = player;
		Event e;
		e.type = EventTypes::WindowResize;
		e.wx = windowManager.SCR_WIDTH;
		e.wy = windowManager.SCR_HEIGHT;
		m_EventQueue.push_back(e);
	}

	void Application::make2scene()
	{
		if (m_scene != nullptr)
			delete m_scene;

		m_scene = new Scene("scene2");
		m_scene->guiManager = CreateRef<GuiManager>(mouseCursor, m_scene->m_camera);

		light = CreateRef<DirectionalLight>(DirectionalLight({ -0.5f, -1.0f, 0.5f }));

		light->activate_lights(ourShader, m_scene->m_camera);
		light->activate_lights(refrShader, m_scene->m_camera);
		light->activate_lights(waterShader, m_scene->m_camera);

		m_scene->guiManager->addComponent(std::string("back"), glm::vec2(-720, 370), glm::vec2(300, 150));
		m_scene->guiManager->addComponent(std::string("back"), glm::vec2(720, 370), glm::vec2(-300, 150));
		m_scene->guiManager->addComponent(std::string("hourglass"), glm::vec2(800, 370), glm::vec2(150, 150));
		m_scene->guiManager->addComponent(std::string("box"), glm::vec2(-795, 375), glm::vec2(100, 100));
		Ref<GuiComponent> numbComponent = m_scene->guiManager->addComponent(std::string("numb1"), glm::vec2(-690, 360), glm::vec2(60, 60));
		Ref<GuiComponent> but = m_scene->guiManager->addComponent(std::string("fullheart"), glm::vec2(-300, 300), glm::vec2(48, 48), 0);
		but->setOnClickFunction(testscene);

		m_scene->gameManager = CreateRef<GameManager>(1, numbComponent);

		Ref<GTexture> texture0 = ResourceManager::getTexture("numb0");
		Ref<GTexture> fh = ResourceManager::getTexture("fullheart");

		m_scene->gameManager->addTexture(texture0);
		m_scene->gameManager->addTexture(numbComponent->getTexture());

		m_scene->gameManager->init();

		Ref<GObject> player = CreateRef<GObject>("cube");
		Ref<Player> plaComp = CreateRef<Player>(inputManager, m_scene->gameManager);
		player->addComponent(plaComp);
		player->shader = ourShader;
		player->set_local_position({ 0, 0, 13 });

		Ref<GObject> boat = CreateRef<GObject>("boat");
		boat->shader = ourShader;
		boat->scaleAABB({ 0.8, 0.8, 0.8 });
		std::vector<glm::vec3> pos;

		pos.push_back({ -6, -7.5, 6 });
		pos.push_back({ 6, -7.5, 6 });

		Ref<Boat> boatComp = CreateRef<Boat>(2, pos);
		boat->addComponent(boatComp);

		boat->set_local_scale(glm::vec3(1.5));
		boat->set_tag("boat");

		Ref<GObject> gdom = CreateRef<GObject>("house");
		Ref<Box> box = CreateRef<Box>(DeliveryColor::Blue);
		gdom->addComponent(box);
		gdom->shader = ourShader;
		gdom->set_local_position({ 0, -7, -13 });
		gdom->set_local_rotation({ 0, 0, 0 });
		gdom->getAABB()->setStatic(true);
		//gdom->set_render_AABB(true);
		gdom->set_tag("house");

		Ref<GObject> paczka = CreateRef<GObject>("package");
		Ref<Box> boxp = CreateRef<Box>(DeliveryColor::Blue);
		paczka->addComponent(boxp);
		paczka->shader = ourShader;
		paczka->set_local_position({ 0, -7.2, 0 });
		paczka->getAABB()->setStatic(true);
		//paczka->set_render_AABB(true);
		paczka->set_tag("package");

		Ref<GObject> most = CreateRef<GObject>("bridge");
		most->shader = ourShader;
		most->set_local_position({ 0, -7.2, -4 });
		most->set_local_rotation({ 0, -90, 0 });
		most->getAABB()->setStatic(true);
		most->rotateAABB(Degrees::D90, Axis::Y);
		//most->set_render_AABB(true);
		most->set_tag("dB");

		Ref<GObject> most2 = CreateRef<GObject>("bridge");
		most2->shader = ourShader;
		most2->set_local_position({ 0, -7.2, -8.5f });
		most2->set_local_rotation({ 0, 90, 0 });
		most2->getAABB()->setStatic(true);
		most2->setAABBrotation(Degrees::D270, Axis::Y);
		//most2->set_render_AABB(true);
		most2->set_tag("uB");

		Ref<GObject> iisland2 = CreateRef<GObject>("island");
		iisland2->shader = ourShader;
		iisland2->set_local_scale({ 0.6, 0.6, 0.6 });
		iisland2->setAABBextentY(0.9f);
		iisland2->set_local_position({ 0, -9.8, -13 });
		iisland2->set_tag("terrain");
		iisland2->getAABB()->setStatic(true);
		//iisland2->set_render_AABB(true);

		Ref<GObject> iisland3 = CreateRef<GObject>("island");
		iisland3->shader = ourShader;
		iisland3->set_local_scale({ 0.6, 0.6, 0.6 });
		//iisland3->scaleAABB({ 0.6, 0.6, 0.6 });
		iisland3->setAABBextentY(0.9f);
		iisland3->set_local_position({ 0, -9.8, 14 });
		iisland3->set_tag("terrain");
		iisland3->getAABB()->setStatic(true);

		Ref<GObject> iisland = CreateRef<GObject>("island");
		iisland->shader = ourShader;
		iisland->set_local_scale({ 0.6, 0.6, 0.6 });
		iisland->setAABBextentY(0.9f);
		iisland->set_local_position({ 0, -9.8, 0 });
		iisland->set_tag("terrain");
		iisland->getAABB()->setStatic(true);

		Ref<GObject> sand = CreateRef<GObject>("hidefCube");
		sand->shader = ourShader;
		sand->set_local_scale({ 60, 1, 60 });
		//water->setAABBextentY(0.9f);
		sand->set_local_position({ 0, -15, 0 });
		sand->getAABB()->setStatic(true);
		sand->set_color({ 0.63, 0.68, 0.1 });
		//sand->cast_shadow = false;


		Ref<GObject> water = CreateRef<GObject>("hidefPlane");
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

		m_scene->addObjectToScene(player);
		m_scene->addObjectToScene(boat);
		m_scene->addObjectToScene(iisland);
		m_scene->addObjectToScene(iisland2);
		m_scene->addObjectToScene(iisland3);
		m_scene->addObjectToScene(gdom);
		m_scene->addObjectToScene(paczka);

		m_scene->addObjectToScene(most);
		m_scene->addObjectToScene(most2);
		m_scene->addObjectToScene(sand);
		m_scene->addObjectToScene(water);

		m_scene->m_camera->player = player;
		Event e;
		e.type = EventTypes::WindowResize;
		e.wx = windowManager.SCR_WIDTH;
		e.wy = windowManager.SCR_HEIGHT;
		m_EventQueue.push_back(e);
	}

	void GameEngine::Application::make3scene()
	{
		if (m_scene != nullptr)
			delete m_scene;

		m_scene = new Scene("scenetest");
		m_scene->guiManager = CreateRef<GuiManager>(mouseCursor, m_scene->m_camera);

		light = CreateRef<DirectionalLight>(DirectionalLight({ -0.5f, -1.0f, 0.5f }));

		light->activate_lights(ourShader, m_scene->m_camera);
		light->activate_lights(refrShader, m_scene->m_camera);
		light->activate_lights(waterShader, m_scene->m_camera);

		m_scene->guiManager->addComponent(std::string("back"), glm::vec2(-720, 370), glm::vec2(300, 150));
		m_scene->guiManager->addComponent(std::string("back"), glm::vec2(720, 370), glm::vec2(-300, 150));
		m_scene->guiManager->addComponent(std::string("hourglass"), glm::vec2(800, 370), glm::vec2(150, 150));
		m_scene->guiManager->addComponent(std::string("box"), glm::vec2(-795, 375), glm::vec2(100, 100));
		Ref<GuiComponent> numbComponent = m_scene->guiManager->addComponent(std::string("numb2"), glm::vec2(-690, 360), glm::vec2(60, 60));
		Ref<GuiComponent> but = m_scene->guiManager->addComponent(std::string("fullheart"), glm::vec2(650, -350), glm::vec2(48, 48), 0);
		but->setOnClickFunction(scene1);

		m_scene->gameManager = CreateRef<GameManager>(2, numbComponent);

		Ref<GTexture> texture0 = ResourceManager::getTexture("numb0");
		Ref<GTexture> texture1 = ResourceManager::getTexture("numb1");
		Ref<GTexture> fh = ResourceManager::getTexture("fullheart");

		m_scene->gameManager->addTexture(texture0);
		m_scene->gameManager->addTexture(texture1);
		m_scene->gameManager->addTexture(numbComponent->getTexture());

		m_scene->gameManager->init();

		Ref<GObject> player = CreateRef<GObject>("ludek");
		player->set_local_scale({ 0.002, 0.002, 0.002 });
		Ref<AABB> aabb = CreateRef<AABB>(glm::vec3(0, 0, 0), 0.5, 0.5, 0.5);
		player->setAABB(aabb);
		player->setAABBoffsetY(0.5f);
		//player->cast_shadow = false;

		Ref<Player> plaComp = CreateRef<Player>(inputManager, m_scene->gameManager);
		player->addComponent(plaComp);
		player->shader = animShader;
		player->set_local_position({ -2, 0, 0 });
		player->set_tag("player");

		Ref<GObject> boat = CreateRef<GObject>("boat");
		boat->shader = ourShader;
		boat->scaleAABB({ 0.8, 0.8, 0.8 });

		//Ref<ParticleSystem> partsys = CreateRef<ParticleSystem>(5, "fullheart");
		//player->addComponent(partsys);
		//particleRenderer->addParticleSystem(partsys);

		std::vector<glm::vec3> pos;

		pos.push_back({ 10, -7.5, -19 });
		pos.push_back({ -10, -7.5, -19 });
		pos.push_back({ -10, -7.5, -7 });
		pos.push_back({ 10, -7.5, -7 });

		Ref<Boat> boatComp = CreateRef<Boat>(3, pos);
		boat->addComponent(boatComp);

		Ref<Vehicle> vehicle = CreateRef<Vehicle>(glm::vec2(0, 1), 5.f, glm::vec2(0, 1), 1.f, 10.f, 5.f);
		vehicle->addSteeringBehavior(player);

		//boat->addComponent(vehicle);

		boat->set_local_scale(glm::vec3(1.5));
		boat->set_tag("boat");

		Ref<GObject> boatCollision = CreateRef<GObject>("");
		boatCollision->set_local_position({ 0, -5, -10 });
		Ref<AABB> bAABB = CreateRef<AABB>(glm::vec3(0, 0, 0), 7, 1, 7);
		boatCollision->setAABB(bAABB);
		boatCollision->set_tag("Tboat");
		boatCollision->shader = ourShader;

		Ref<Stealing> stealing = CreateRef<Stealing>(3);
		stealing->addBoat(boat);
		boatCollision->addComponent(stealing);
		boatCollision->set_render_AABB(true);

		aabb = CreateRef<AABB>(glm::vec3(0, 0, 0), 3, 0.5, 3);
		Ref<BridgeTrigger> bri = CreateRef<BridgeTrigger>();
		Ref<GObject> bridgeTrigger = CreateRef<GObject>("");
		bridgeTrigger->set_local_position({ -6, -7, 0 });
		bridgeTrigger->shader = ourShader;

		bridgeTrigger->setAABB(aabb);
		bridgeTrigger->set_tag("Tbridge");
		bridgeTrigger->getAABB()->setStatic(true);
		bridgeTrigger->addComponent(bri);

		Ref<AABB> aabb1 = CreateRef<AABB>(glm::vec3(0, 0, 0), 3, 0.5, 3);
		Ref<BridgeTrigger> bri1 = CreateRef<BridgeTrigger>();
		Ref<GObject> bridgeTrigger1 = CreateRef<GObject>("");
		bridgeTrigger1->set_local_position({ 6, -7, 0 });
		bridgeTrigger1->shader = ourShader;

		bridgeTrigger1->setAABB(aabb1);
		bridgeTrigger1->set_tag("Tbridge");
		bridgeTrigger1->getAABB()->setStatic(true);
		bridgeTrigger1->addComponent(bri1);

		Ref<AABB> aabb2 = CreateRef<AABB>(glm::vec3(0, 0, 0), 3, 0.5, 3);
		Ref<BridgeTrigger> bri2 = CreateRef<BridgeTrigger>();
		Ref<GObject> bridgeTrigger2 = CreateRef<GObject>("");
		bridgeTrigger2->set_local_position({ 0, -7, -6 });
		bridgeTrigger2->shader = ourShader;

		bridgeTrigger2->setAABB(aabb2);
		bridgeTrigger2->set_tag("Tbridge");
		bridgeTrigger2->getAABB()->setStatic(true);
		bridgeTrigger2->addComponent(bri2);

		Ref<GObject> gdom = CreateRef<GObject>("redhouse");
		Ref<Box> box = CreateRef<Box>(DeliveryColor::Red);
		gdom->addComponent(box);
		gdom->shader = ourShader;
		gdom->set_local_position({ -13, -7, 0 });
		gdom->set_local_rotationY(270);
		gdom->getAABB()->setStatic(true);
		//gdom->set_render_AABB(true);
		gdom->set_tag("house");

		Ref<GObject> gdom2 = CreateRef<GObject>("house");
		box = CreateRef<Box>(DeliveryColor::Blue);
		gdom2->addComponent(box);
		gdom2->shader = ourShader;
		gdom2->set_local_position({ 13, -7, -26});
		gdom2->set_local_rotationY(90);
		gdom2->getAABB()->setStatic(true);
		//gdom->set_render_AABB(true);
		gdom2->set_tag("house");

		Ref<GObject> dirt = CreateRef<GObject>("dirt");
		dirt->shader = ourShader;
		dirt->set_local_position({ 13, -9.8, 0 });

		Ref<Dig> digDirt = CreateRef<Dig>(2, DeliveryColor::Blue, inputManager);
		dirt->addComponent(digDirt);

		Ref<GObject> dirt1 = CreateRef<GObject>("dirt");
		dirt1->shader = ourShader;
		dirt1->set_local_position({ -13, -9.8, -26 });

		Ref<Dig> digDirt1 = CreateRef<Dig>(3, DeliveryColor::Red, inputManager);
		dirt1->addComponent(digDirt1);

		Ref<GObject> most = CreateRef<GObject>("bridge");
		most->shader = ourShader;
		most->set_local_position({ 0, -7.2, -4 });
		most->set_local_rotation({ 0, -90, 0 });
		most->getAABB()->setStatic(true);
		most->rotateAABB(Degrees::D90, Axis::Y);
		//most->set_render_AABB(true);
		most->set_tag("dB");

		Ref<GObject> most2 = CreateRef<GObject>("bridge");
		most2->shader = ourShader;
		most2->set_local_position({ 0, -7.2, -8.5f });
		most2->set_local_rotation({ 0, 90, 0 });
		most2->getAABB()->setStatic(true);
		most2->setAABBrotation(Degrees::D270, Axis::Y);
		//most2->set_render_AABB(true);
		most2->set_tag("uB");

		bri2->bridgepart1 = most;
		bri2->bridgepart2 = most2;

		Ref<GObject> most5 = CreateRef<GObject>("bridge");
		most5->shader = ourShader;
		most5->set_local_position({ 4, -7.2, 0 });
		most5->set_local_rotation({ 0, 0, 0 });
		most5->getAABB()->setStatic(true);
		//most5->set_render_AABB(true);

		Ref<GObject> most6 = CreateRef<GObject>("bridge");
		most6->shader = ourShader;
		most6->set_local_position({ 8.5f, -7.2, 0 });
		most6->set_local_rotation({ 0, 180, 0 });
		most6->getAABB()->setStatic(true);
		most6->setAABBrotation(Degrees::D180, Axis::Y);
		//most6->set_render_AABB(true);

		bri1->bridgepart1 = most5;
		bri1->bridgepart2 = most6;

		Ref<GObject> most9 = CreateRef<GObject>("bridge");
		most9->shader = ourShader;
		most9->set_local_position({ -9, -7.2, -26 });
		most9->set_local_rotation({ 0, 0, 0 });
		most9->set_local_scale({ 4, 1.5, 3 });
		most9->scaleAABB({ 1.5, 1, 1 });
		most9->getAABB()->setStatic(true);
		most9->set_render_AABB(true);

		Ref<GObject> most10 = CreateRef<GObject>("bridge");
		most10->shader = ourShader;
		most10->set_local_position({ 9, -7.2, -26 });
		most10->set_local_rotation({ 0, 180, 0 });
		most10->set_local_scale({ 4, 1.5, 3 });
		most10->scaleAABB({ 1.5, 1, 1 });
		most10->getAABB()->setStatic(true);
		most10->setAABBrotation(Degrees::D180, Axis::Y);
		most10->set_render_AABB(true);

		Ref<GObject> most7 = CreateRef<GObject>("bridge");
		most7->shader = ourShader;
		most7->set_local_position({ -8.5f, -7.2, 0 });
		most7->set_local_rotation({ 0, 0, 0 });
		most7->getAABB()->setStatic(true);
		//most7->set_render_AABB(true);

		Ref<GObject> most8 = CreateRef<GObject>("bridge");
		most8->shader = ourShader;
		most8->set_local_position({ -4, -7.2, 0 });
		most8->set_local_rotation({ 0, 180, 0 });
		most8->getAABB()->setStatic(true);
		most8->setAABBrotation(Degrees::D180, Axis::Y);
		bri->bridgepart1 = most7;
		bri->bridgepart2 = most8;

		Ref<GObject> iisland2 = CreateRef<GObject>("island");
		iisland2->shader = ourShader;
		iisland2->set_local_scale({ 0.6, 0.6, 0.6 });
		iisland2->setAABBextentY(0.9f);
		iisland2->set_local_position({ 13, -9.8, 0 });
		iisland2->set_tag("terrain");
		iisland2->getAABB()->setStatic(true);
		//iisland2->set_render_AABB(true);
		Ref<Island> island2 = CreateRef<Island>(true);
		iisland2->addComponent(island2);

		Ref<GObject> iisland6 = CreateRef<GObject>("island");
		iisland6->shader = ourShader;
		iisland6->set_local_scale({ 0.6, 0.6, 0.6 });
		iisland6->setAABBextentY(0.9f);
		iisland6->set_local_position({ 13, -9.8, -26 });
		iisland6->set_tag("terrain");
		iisland6->getAABB()->setStatic(true);
		//iisland2->set_render_AABB(true);
		Ref<Island> island6 = CreateRef<Island>(false);
		iisland6->addComponent(island6);

		Ref<GObject> iisland3 = CreateRef<GObject>("island");
		iisland3->shader = ourShader;
		iisland3->set_local_scale({ 0.6, 0.6, 0.6 });
		//iisland3->scaleAABB({ 0.6, 0.6, 0.6 });
		iisland3->setAABBextentY(0.9f);
		iisland3->set_local_position({ -13, -9.8, 0 });
		iisland3->set_tag("terrain");
		iisland3->getAABB()->setStatic(true);
		Ref<Island> island3 = CreateRef<Island>(false);
		iisland3->addComponent(island3);

		Ref<GObject> iisland4 = CreateRef<GObject>("island");
		iisland4->shader = ourShader;
		iisland4->set_local_scale({ 0.6, 0.6, 0.6 });
		//iisland4->scaleAABB({ 0.6, 0.6, 0.6 });
		iisland4->setAABBextentY(0.9f);
		iisland4->set_local_position({ -13, -9.8, -26 });
		iisland4->set_tag("terrain");
		iisland4->getAABB()->setStatic(true);
		iisland4->set_render_AABB(true);
		Ref<Island> island4 = CreateRef<Island>(true);
		iisland4->addComponent(island4);

		Ref<GObject> iisland5 = CreateRef<GObject>("island");
		iisland5->shader = ourShader;
		iisland5->set_local_scale({ 0.6, 0.6, 0.6 });
		iisland5->setAABBextentY(0.9f);
		iisland5->set_local_position({ 0, -9.8, -13 });
		iisland5->set_tag("terrain");
		iisland5->getAABB()->setStatic(true);
		Ref<Island> island5 = CreateRef<Island>(true);
		iisland5->addComponent(island5);

		Ref<GObject> iisland = CreateRef<GObject>("island");
		iisland->shader = ourShader;
		iisland->set_local_scale({ 0.6, 0.6, 0.6 });
		iisland->setAABBextentY(0.9f);
		iisland->set_local_position({ 0, -9.8, 0 });
		iisland->set_tag("terrain");
		iisland->getAABB()->setStatic(true);
		Ref<Island> islandd = CreateRef<Island>(true);
		iisland->addComponent(islandd);

		Ref<GObject> sand = CreateRef<GObject>("hidefCube");
		sand->shader = ourShader;
		sand->set_local_scale({ 60, 1, 60 });
		//water->setAABBextentY(0.9f);
		sand->set_local_position({ 0, -15, 0 });
		sand->getAABB()->setStatic(true);
		sand->set_color({ 0.63, 0.68, 0.1 });
		//sand->cast_shadow = false;

		Ref<GObject> water = CreateRef<GObject>("hidefPlane");
		water->shader = waterShader;
		water->set_local_scale({ 60, 4, 60 });
		water->setAABBextentY(1.1);
		//water->setAABBextentY(0.9f);
		water->set_local_position({ 0, -9.5, 0 });
		water->getAABB()->setStatic(true);
		water->set_tag("water");
		water->set_color({ 0.38, 0.44, 0.91 });
		water->cast_shadow = false;

		Ref<GObject> water2 = CreateRef<GObject>("hidefPlane");
		water2->shader = waterShader;
		water2->set_local_scale({ 60, 4, 60 });
		water2->setAABBextentY(1.1);
		//water->setAABBextentY(0.9f);
		water2->set_local_position({ 60, -9.5, 0 });
		water2->getAABB()->setStatic(true);
		water2->set_tag("water");
		water2->set_color({ 0.38, 0.44, 0.91 });
		water2->cast_shadow = false;


		Ref<GObject> water3 = CreateRef<GObject>("hidefPlane");
		water3->shader = waterShader;
		water3->set_local_scale({ 60, 4, 60 });
		water3->setAABBextentY(1.1);
		//water->setAABBextentY(0.9f);
		water3->set_local_position({ 0, -9.5, -60 });
		water3->getAABB()->setStatic(true);
		water3->set_tag("water");
		water3->set_color({ 0.38, 0.44, 0.91 });
		water3->cast_shadow = false;
		//water->getModel()->meshes[0].textures[0].id = texture->GetTextureID();

		m_scene->addObjectToScene(player);
		m_scene->addObjectToScene(boat);
		m_scene->addObjectToScene(boatCollision);
		m_scene->addObjectToScene(dirt);
		m_scene->addObjectToScene(dirt1);
		m_scene->addObjectToScene(iisland);
		m_scene->addObjectToScene(iisland2);
		m_scene->addObjectToScene(iisland3);
		m_scene->addObjectToScene(iisland4);
		m_scene->addObjectToScene(iisland5);
		m_scene->addObjectToScene(iisland6);
		m_scene->addObjectToScene(gdom);
		m_scene->addObjectToScene(gdom2);
		m_scene->addObjectToScene(bridgeTrigger);
		m_scene->addObjectToScene(bridgeTrigger1);
		m_scene->addObjectToScene(bridgeTrigger2);
		m_scene->addObjectToScene(most);
		m_scene->addObjectToScene(most2);
		m_scene->addObjectToScene(most5);
		m_scene->addObjectToScene(most6);
		m_scene->addObjectToScene(most7);
		m_scene->addObjectToScene(most8);
		m_scene->addObjectToScene(most9);
		m_scene->addObjectToScene(most10);
		m_scene->addObjectToScene(sand);
		m_scene->addObjectToScene(water);
		m_scene->addObjectToScene(water2);
		m_scene->addObjectToScene(water3);

		m_scene->m_camera->player = player;
		Event e;
		e.type = EventTypes::WindowResize;
		e.wx = windowManager.SCR_WIDTH;
		e.wy = windowManager.SCR_HEIGHT;
		m_EventQueue.push_back(e);
	}

}