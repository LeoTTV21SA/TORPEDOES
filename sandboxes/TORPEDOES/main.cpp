#include <engine/Application.h>
#include <engine/Tilemap.h>
#include "PlayerController.h"

using namespace engine;

ECS ecs;
int checkPointNumber = 0;

void createChepoint(Vector3 position, Vector3 rotation, Vector3 scale, Model& checkPointModel, float hitboxrotation, bool finish_line = false)
{
	Entity checkpoint = ecs.newEntity();

	ecs.addComponent(checkpoint, Transform{ .position = position , .rotation = rotation , .scale = scale });
	ecs.addComponent(checkpoint, ModelRenderer{ .model = &checkPointModel });
	ecs.addComponent(checkpoint, CheckPoint{ checkPointNumber , finish_line });
	std::vector<Vector2> CheckpointcolliderVerts{ Vector2(4, 8), Vector2(4, -8), Vector2(-4, -8), Vector2(-4, 8) };
	ecs.addComponent(checkpoint, PolygonCollider({ .vertices = CheckpointcolliderVerts, .trigger = true, .visualise = false, .rotationOverride = hitboxrotation }));

	checkPointNumber++;
};

int main()
{
	NO_OPENAL = true;

	string username = "";
	string lap = "1";
	vector<string> playerNames(4);
	vector<int> playerCheckpoints(4);

	GLFWwindow* window = engine::CreateGLWindow(1600, 900, "Window");

	engine::EngineLib engine;

	engine::Camera cam = engine::Camera(1120, 630);
	cam.SetPosition(Vector3(0, 0, 1500));
	//cam.perspective = true;
	cam.SetRotation(Vector3(0, 0, 0));
	float camScale = 1;
	float aspectRatio = 16 / 9;
	float camPadding = 100;
	float camDeadzone = 10;
	//engine.physicsSystem->gravity = Vector2(0, -981);
	engine.collisionSystem->cam = &cam;

	// register component manual 
	shared_ptr<PlayerController>playerController = ecs.registerSystem< PlayerController>();
	Signature playerControllerSignature;
	playerControllerSignature.set(ecs.getComponentId< Transform>());
	playerControllerSignature.set(ecs.getComponentId< Player>());
	playerControllerSignature.set(ecs.getComponentId< Rigidbody>());
	playerControllerSignature.set(ecs.getComponentId< PolygonCollider>());
	playerControllerSignature.set(ecs.getComponentId< ModelRenderer>());
	ecs.setSystemSignature<PlayerController>(playerControllerSignature);

	Model model("assets/LaMuerte.obj");
	Model checkPointModel("assets/Checkpoint.obj");
	Model model2("assets/Finish_line.obj");
	Texture GUItexture = Texture("assets/GUI_backround.png");
	Texture torprldtexture = Texture("assets/torpedoReloading.png");
	Texture torprdytexture = Texture("assets/torpedoReady.png");
	// Font http address:
	// https://www.dafont.com/stencil-ww-ii.font
	Font stencilFont("assets/Stencil WW II.ttf", 0, 0, 48);

	for (int i = 0; i < playerNames.size(); ++i)
	{
		cout << "Please enter Player's " + to_string(i + 1) + " name: ";
		cin >> username;
		playerNames[i] = username;
	}

	Texture* winSprite = new Texture("assets/winner.png");
	Entity playerWin = ecs.newEntity();
	TextRenderer& winText = ecs.addComponent(playerWin, TextRenderer{ .font = &stencilFont, .text = "", .offset = Vector3(-1.0f, 1.1f, 1.0f), .scale = Vector3(0.02f), .color = Vector3(0.5f, 0.8f, 0.2f), .uiElement = true });
	ecs.addComponent(playerWin, SpriteRenderer{ .texture = winSprite, .enabled = false, .uiElement = true });
	ecs.addComponent(playerWin, Transform{ .position = Vector3(0, 0, 0), .scale = Vector3(0.5f) });
	PlayerController::playerWin = playerWin;

	Entity pFont1 = ecs.newEntity();
	Entity pFont2 = ecs.newEntity();
	Entity pFont3 = ecs.newEntity();
	Entity pFont4 = ecs.newEntity();
	Entity pSFont1 = ecs.newEntity();
	Entity pSFont2 = ecs.newEntity();
	Entity pSFont3 = ecs.newEntity();
	Entity pSFont4 = ecs.newEntity();

	TextRenderer& p1Win = ecs.addComponent(pSFont1, TextRenderer{ .font = &stencilFont, .text = lap + "/3", .offset = Vector3(1.2f, -0.3f, 0.0f), .scale = Vector3(0.02f), .color = Vector3(0.5f, 0.8f, 0.2f), .uiElement = true });
	ecs.addComponent(pSFont1, Transform{ .position = Vector3(-0.65, -0.9, -0.5), .scale = Vector3(0.05, 0.085, 1) });
	TextRenderer& p2Win = ecs.addComponent(pSFont2, TextRenderer{ .font = &stencilFont, .text = lap + "/3", .offset = Vector3(1.2f, -0.3f, 0.0f), .scale = Vector3(0.02f), .color = Vector3(0.5f, 0.8f, 0.2f), .uiElement = true });
	ecs.addComponent(pSFont2, Transform{ .position = Vector3(-0.15, -0.9, -0.5), .scale = Vector3(0.05, 0.085, 1) });
	TextRenderer& p3Win = ecs.addComponent(pSFont3, TextRenderer{ .font = &stencilFont, .text = lap + "/3", .offset = Vector3(1.2f, -0.3f, 0.0f), .scale = Vector3(0.02f), .color = Vector3(0.5f, 0.8f, 0.2f), .uiElement = true });
	ecs.addComponent(pSFont3, Transform{ .position = Vector3(0.25, -0.9, -0.5), .scale = Vector3(0.05, 0.085, 1) });
	TextRenderer& p4Win = ecs.addComponent(pSFont4, TextRenderer{ .font = &stencilFont, .text = lap + "/3", .offset = Vector3(1.2f, -0.3f, 0.0f), .scale = Vector3(0.02f), .color = Vector3(0.5f, 0.8f, 0.2f), .uiElement = true });
	ecs.addComponent(pSFont4, Transform{ .position = Vector3(0.75, -0.9, -0.5), .scale = Vector3(0.05, 0.085, 1) });

	static SoundSource engineSpeaker;
	static SoundSource engineSpeaker2;
	static SoundSource engineSpeaker3;
	static SoundSource engineSpeaker4;
	static SoundSource torpSpeaker;
	static SoundSource torpSpeaker2;
	static SoundSource torpSpeaker3;
	static SoundSource torpSpeaker4;
	static SoundSource torpSpeaker5;
	static SoundSource torpSpeaker6;
	static SoundSource torpSpeaker7;
	static SoundSource torpSpeaker8;
	static SoundSource explosionSpeaker;
	static SoundSource cheerSpeaker;

	uint32_t torpedoSound = SoundBuffer::getFile()->addSoundEffect("assets/torpedoshoot.wav");
	uint32_t explosionSound = SoundBuffer::getFile()->addSoundEffect("assets/explosion.wav");
	uint32_t engineSound = SoundBuffer::getFile()->addSoundEffect("assets/enginemono.wav");
	uint32_t cheerSound = SoundBuffer::getFile()->addSoundEffect("assets/cheering.wav");

	engineSpeaker.setLinearDistance(0.4f, 10.f, 400.f, 0.6f);
	engineSpeaker2.setLinearDistance(0.4f, 10.f, 400.f, 0.6f);
	engineSpeaker3.setLinearDistance(0.4f, 10.f, 400.f, 0.6f);
	engineSpeaker4.setLinearDistance(0.4f, 10.f, 400.f, 0.6f);
	torpSpeaker.setLinearDistance(1.0f, 10.f, 500.f, 0.5f);
	torpSpeaker2.setLinearDistance(1.0f, 10.f, 500.f, 0.5f);
	torpSpeaker3.setLinearDistance(1.0f, 10.f, 500.f, 0.5f);
	torpSpeaker4.setLinearDistance(1.0f, 10.f, 500.f, 0.5f);
	torpSpeaker5.setLinearDistance(1.0f, 10.f, 500.f, 0.5f);
	torpSpeaker6.setLinearDistance(1.0f, 10.f, 500.f, 0.5f);
	explosionSpeaker.setLinearDistance(1.0f, 10.f, 600.f, 1.f);
	cheerSpeaker.setLinearDistance(0.5f, 50.f, 350.f, 0.9f);


	Entity laMuerte = ecs.newEntity();
	ecs.addComponent(pFont1, TextRenderer{ .font = &stencilFont, .text = playerNames[0], .offset = Vector3(1.0f, -1.0f, 0), .scale = Vector3(0.5f), .color = Vector3(0.5f, 0.8f, 0.2f) });
	Transform& pFont1Transform = ecs.addComponent(pFont1, Transform{ .position = Vector3(1434.0f,-1449.0f, 100.0f) });
	Transform& PlayerTransform = ecs.addComponent(laMuerte, Transform{ .position = Vector3(1474.321533, -1435.868286, 100.000000), .rotation = Vector3(45.000000, 0.0000, 0.000000), .scale = Vector3(7) });
	Player& player = ecs.addComponent(laMuerte, Player{ .acerationSpeed = 300.0f, .minAceleration = 120.0f, .playerID = 0, .playerFont = pFont1, .playername = playerNames[0], .playerLap = lap });
	ecs.addComponent(laMuerte, ModelRenderer{ .model = &model });
	Rigidbody& PlayerRigidbody = ecs.addComponent(laMuerte, Rigidbody{ .drag = 0.025f });
	vector<Vector2> colliderVerts{ Vector2(2, 2), Vector2(2, -1), Vector2(-5, -1), Vector2(-5, 2) };
	PolygonCollider& collider = ecs.addComponent(laMuerte, PolygonCollider{ .vertices = colliderVerts, .callback = PlayerController::OnCollision , .visualise = false });
	engineSpeaker.Play(engineSound);
	engineSpeaker.SetLooping(1);


	Entity laMuerte2 = ecs.newEntity();

	ecs.addComponent(pFont2, TextRenderer{ .font = &stencilFont, .text = playerNames[1], .offset = Vector3(1.0f, -1.0f, 0), .scale = Vector3(0.5f), .color = Vector3(0.5f, 0.8f, 0.2f) });
	Transform& pFont2Transform = ecs.addComponent(pFont2, Transform{ .position = Vector3(1434.0f,-1349.0f, 100.0f) });
	Transform& PlayerTransform2 = ecs.addComponent(laMuerte2, Transform{ .position = Vector3(1474.321533, -1369.868286, 100.000000), .rotation = Vector3(45.000000, 0.0000, 0.000000), .scale = Vector3(7) });
	Player& player2 = ecs.addComponent(laMuerte2, Player{ .acerationSpeed = 300.0f, .minAceleration = 120.0f, .playerID = 1, .playerFont = pFont2,.playername = playerNames[1], .playerLap = lap });
	ecs.addComponent(laMuerte2, ModelRenderer{ .model = &model });
	Rigidbody& PlayerRigidbody2 = ecs.addComponent(laMuerte2, Rigidbody{ .drag = 0.025f });
	PolygonCollider& collider2 = ecs.addComponent(laMuerte2, PolygonCollider{ .vertices = colliderVerts, .callback = PlayerController::OnCollision , .visualise = false });
	engineSpeaker2.Play(engineSound);
	engineSpeaker2.SetLooping(1);
	

	Entity laMuerte3 = ecs.newEntity();

	ecs.addComponent(pFont3, TextRenderer{ .font = &stencilFont, .text = playerNames[2], .offset = Vector3(1.0f, -1.0f, 0), .scale = Vector3(0.5f), .color = Vector3(0.5f, 0.8f, 0.2f) });
	Transform& pFont3Transform = ecs.addComponent(pFont3, Transform{ .position = Vector3(1434.0f,-1549.0f, 100.0f) });
	Transform& PlayerTransform3 = ecs.addComponent(laMuerte3, Transform{ .position = Vector3(1474.321533, -1495.868286, 100.000000), .rotation = Vector3(45.000000, 0.0000, 0.000000), .scale = Vector3(7) });
	Player& player3 = ecs.addComponent(laMuerte3, Player{ .acerationSpeed = 300.0f, .minAceleration = 120.0f, .playerID = 2, .playerFont = pFont3,.playername = playerNames[2], .playerLap = lap });
	ecs.addComponent(laMuerte3, ModelRenderer{ .model = &model });
	Rigidbody& PlayerRigidbody3 = ecs.addComponent(laMuerte3, Rigidbody{ .drag = 0.025f });
	PolygonCollider& collider3 = ecs.addComponent(laMuerte3, PolygonCollider{ .vertices = colliderVerts, .callback = PlayerController::OnCollision , .visualise = false });
	engineSpeaker3.Play(engineSound);
	engineSpeaker3.SetLooping(1);


	Entity laMuerte4 = ecs.newEntity();

	ecs.addComponent(pFont4, TextRenderer{ .font = &stencilFont, .text = playerNames[3], .offset = Vector3(1.0f, -1.0f, 0), .scale = Vector3(0.5f), .color = Vector3(0.5f, 0.8f, 0.2f) });
	Transform& pFont4Transform = ecs.addComponent(pFont4, Transform{ .position = Vector3(1434.0f,-1549.0f, 100.0f) });
	Transform& PlayerTransform4 = ecs.addComponent(laMuerte4, Transform{ .position = Vector3(1474.321533, -1569.868286, 100.000000), .rotation = Vector3(45.000000, 0.0000, 0.000000), .scale = Vector3(7) });
	Player& player4 = ecs.addComponent(laMuerte4, Player{ .acerationSpeed = 300.0f, .minAceleration = 120.0f, .playerID = 3, .playerFont = pFont4,.playername = playerNames[3], .playerLap = lap });
	ecs.addComponent(laMuerte4, ModelRenderer{ .model = &model });
	Rigidbody& PlayerRigidbody4 = ecs.addComponent(laMuerte4, Rigidbody{ .drag = 0.025f });
	PolygonCollider& collider4 = ecs.addComponent(laMuerte4, PolygonCollider{ .vertices = colliderVerts, .callback = PlayerController::OnCollision , .visualise = false });
	engineSpeaker4.Play(engineSound);
	engineSpeaker4.SetLooping(1);

	Entity GUIBackround = ecs.newEntity();
	ecs.addComponent(GUIBackround, SpriteRenderer{ .texture = &GUItexture, .uiElement = true });
	ecs.addComponent(GUIBackround, Transform{ .position = Vector3(0, -0.95, -0.9), .scale = Vector3(1, 0.2, 1) });

	Entity torpIndicator1 = ecs.newEntity();
	ecs.addComponent(torpIndicator1, TextRenderer{ .font = &stencilFont, .text = playerNames[0], .offset = Vector3(0.0f, 1.25f, 0.0f), .scale = Vector3(0.013f), .color = Vector3(0.5f, 0.8f, 0.2f), .uiElement = true });
	SpriteRenderer& torpicon1 = ecs.addComponent(torpIndicator1, SpriteRenderer{ .texture = &torprdytexture, .uiElement = true });
	ecs.addComponent(torpIndicator1, Transform{ .position = Vector3(-0.75, -0.9, -0.5), .scale = Vector3(0.05, 0.085, 1) });
	Entity torpIndicator2 = ecs.newEntity();
	SpriteRenderer& torpicon2 = ecs.addComponent(torpIndicator2, SpriteRenderer{ .texture = &torprdytexture, .uiElement = true });
	ecs.addComponent(torpIndicator2, Transform{ .position = Vector3(-0.65, -0.9, -0.55), .scale = Vector3(0.05, 0.085, 1) });

	Entity torpIndicator3 = ecs.newEntity();
	ecs.addComponent(torpIndicator3, TextRenderer{ .font = &stencilFont, .text = playerNames[1], .offset = Vector3(0.0f, 1.25f, 0.0f), .scale = Vector3(0.013f), .color = Vector3(0.5f, 0.8f, 0.2f), .uiElement = true });
	SpriteRenderer& torpicon3 = ecs.addComponent(torpIndicator3, SpriteRenderer{ .texture = &torprdytexture, .uiElement = true });
	ecs.addComponent(torpIndicator3, Transform{ .position = Vector3(-0.25, -0.9, -0.5), .scale = Vector3(0.05, 0.085, 1) });
	Entity torpIndicator4 = ecs.newEntity();
	SpriteRenderer& torpicon4 = ecs.addComponent(torpIndicator4, SpriteRenderer{ .texture = &torprdytexture, .uiElement = true });
	ecs.addComponent(torpIndicator4, Transform{ .position = Vector3(-0.15, -0.9, -0.55), .scale = Vector3(0.05, 0.085, 1) });

	Entity torpIndicator5 = ecs.newEntity();
	ecs.addComponent(torpIndicator5, TextRenderer{ .font = &stencilFont, .text = playerNames[2],.offset = Vector3(0.0f, 1.25f, 0.0f), .scale = Vector3(0.013f), .color = Vector3(0.5f, 0.8f, 0.2f), .uiElement = true });
	SpriteRenderer& torpicon5 = ecs.addComponent(torpIndicator5, SpriteRenderer{ .texture = &torprdytexture, .uiElement = true });
	ecs.addComponent(torpIndicator5, Transform{ .position = Vector3(0.15, -0.9, -0.5), .scale = Vector3(0.05, 0.085, 1) });
	Entity torpIndicator6 = ecs.newEntity();
	SpriteRenderer& torpicon6 = ecs.addComponent(torpIndicator6, SpriteRenderer{ .texture = &torprdytexture, .uiElement = true });
	ecs.addComponent(torpIndicator6, Transform{ .position = Vector3(0.25, -0.9, -0.55), .scale = Vector3(0.05, 0.085, 1) });

	Entity torpIndicator7 = ecs.newEntity();
	ecs.addComponent(torpIndicator7, TextRenderer{ .font = &stencilFont, .text = playerNames[3],.offset = Vector3(0.0f, 1.25f, 0.0f), .scale = Vector3(0.013f), .color = Vector3(0.5f, 0.8f, 0.2f), .uiElement = true });
	SpriteRenderer& torpicon7 = ecs.addComponent(torpIndicator7, SpriteRenderer{ .texture = &torprdytexture, .uiElement = true });
	ecs.addComponent(torpIndicator7, Transform{ .position = Vector3(0.65, -0.9, -0.5), .scale = Vector3(0.05, 0.085, 1) });
	Entity torpIndicator8 = ecs.newEntity();
	SpriteRenderer& torpicon8 = ecs.addComponent(torpIndicator8, SpriteRenderer{ .texture = &torprdytexture, .uiElement = true });
	ecs.addComponent(torpIndicator8, Transform{ .position = Vector3(0.75, -0.9, -0.55), .scale = Vector3(0.05, 0.085, 1) });


	// create explosion Animation PlayerController 
	Animation explosionAnim = AnimationsFromSpritesheet("assets/explosion.png", 6, 1, vector<int>(6, 150))[0];
	playerController->ExplosionAnim = &explosionAnim;

	Animation crowdAnims = AnimationsFromSpritesheet("assets/CrowdCheer14.png", 3, 1, vector<int>(3, 150))[0];
	Entity crowd = ecs.newEntity();
	ecs.addComponent(crowd, Transform{ .position = Vector3(1530, -1700, 10), .scale = Vector3(100, 30, 0) });
	ecs.addComponent(crowd, SpriteRenderer{});
	ecs.addComponent(crowd, Animator{});
	AnimationSystem::AddAnimation(crowd, crowdAnims, "CrowdCheer");
	AnimationSystem::PlayAnimation(crowd, "CrowdCheer", true);
	Entity crowd1 = ecs.newEntity();
	ecs.addComponent(crowd1, Transform{ .position = Vector3(1545, -1715, 11), .scale = Vector3(100, 30, 0) });
	ecs.addComponent(crowd1, SpriteRenderer{});
	ecs.addComponent(crowd1, Animator{});
	AnimationSystem::AddAnimation(crowd1, crowdAnims, "Cheer2");
	AnimationSystem::PlayAnimation(crowd1, "Cheer2", true);
	Entity crowd2 = ecs.newEntity();
	ecs.addComponent(crowd2, Transform{ .position = Vector3(1520, -1730, 12), .scale = Vector3(100, 30, 0) });
	ecs.addComponent(crowd2, SpriteRenderer{});
	ecs.addComponent(crowd2, Animator{});
	AnimationSystem::AddAnimation(crowd2, crowdAnims, "Cheer3");
	AnimationSystem::PlayAnimation(crowd2, "Cheer3", true);
	cheerSpeaker.Play(cheerSound);
	cheerSpeaker.SetLooping(1);

	// Loand Map . Tilemap file 
	Tilemap map(&cam);
	map.loadMap("assets/torptest.tmx");
	engine.spriteRenderSystem->SetTilemap(&map);
	engine.collisionSystem->SetTilemap(&map);
	PhysicsSystem::SetTileProperty(1, TileProperty{ true });
	//call the function. createChepoint

	createChepoint(Vector3(2100.226807, -963.837402, 100.000000), Vector3(30.000000, 159.245773, 0.000000), Vector3(17), checkPointModel, 45.0f); // 0
	createChepoint(Vector3(2957.365723, -828.268005, 100.000000), Vector3(45.000000, 147.891968, 0.000000), Vector3(17), checkPointModel, 45.0f); // 1
	createChepoint(Vector3(3387.268555, -355.873444, 100.000000), Vector3(45.000000, 99.936874, 0.000000), Vector3(17), checkPointModel, 45.0f); // 2
	createChepoint(Vector3(3655.793701, -1339.042236, 100.000000), Vector3(45.000000, 147.891968, 0.000000), Vector3(17), checkPointModel, 45.0f); // 3
	createChepoint(Vector3(2795.650391, -1489.039795, 100.000000), Vector3(45.000000, -368.616577, 0.000000), Vector3(17), checkPointModel, 45.0f); // 4
	createChepoint(Vector3(2597.463135, -684.973389, 100.000000), Vector3(45.000000, 180.022018, 0.000000), Vector3(17), checkPointModel, 45.0f); // 5
	createChepoint(Vector3(1668.260010, -990.794373, 100.000000), Vector3(45.000000, 147.891968, 0.000000), Vector3(17), checkPointModel, 45.0f); // 6 
	createChepoint(Vector3(1043.635132, -875.206543, 100.000000), Vector3(45.000000, 179.241272, 0.000000), Vector3(17), checkPointModel, 45.0f); // 7
	createChepoint(Vector3(943.931152, -293.566711, 100.000000), Vector3(45.000000, 107.476852, 0.000000), Vector3(17), checkPointModel, 45.0f); // 8
	createChepoint(Vector3(586.608276, -1249.448486, 100.000000), Vector3(45.000000, 40.070156, 0.000000), Vector3(17), checkPointModel, 90.0f); // 9
	createChepoint(Vector3(1513.692383, -1462.996187, 50.000000), Vector3(90.000000, 90.901711, 0.000000), Vector3(14), model2, -1, true); // 10

	while (!glfwWindowShouldClose(window))
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);		

		// SetLitght position to Camara position & LitghtColor  
		engine.modelRenderSystem->SetLight(Vector3(cam.position.x, cam.position.y, 1500), Vector3(255));
		 // UI System 				
		
		p1Win.text = to_string(player.lap) + "/1";
		p2Win.text = to_string(player2.lap) + "/1";
		p3Win.text = to_string(player3.lap) + "/1";
		p4Win.text = to_string(player4.lap) + "/1";

		//player 1
		if (player.projectileTime1 > 0) {
			torpicon1.texture = &torprldtexture;
		}
		else {
			torpicon1.texture = &torprdytexture;
			torpSpeaker.Play(torpedoSound);
			engine.soundDevice->SetSourceLocation(torpSpeaker, PlayerTransform.position.x, PlayerTransform.position.y, 0);
		}
		if (player.projectileTime2 > 0) {
			torpicon2.texture = &torprldtexture;
		}
		else {
			torpicon2.texture = &torprdytexture;
			torpSpeaker2.Play(torpedoSound);
			engine.soundDevice->SetSourceLocation(torpSpeaker2, PlayerTransform.position.x, PlayerTransform.position.y, 0);
		}


		/// Player 2
		if (player2.projectileTime1 > 0) {
			torpicon3.texture = &torprldtexture;
		}
		else {
			torpicon3.texture = &torprdytexture;
			torpSpeaker3.Play(torpedoSound);
			engine.soundDevice->SetSourceLocation(torpSpeaker3, PlayerTransform2.position.x, PlayerTransform2.position.y, 0);
		}
		if (player2.projectileTime2 > 0) {
			torpicon4.texture = &torprldtexture;
		}
		else {
			torpicon4.texture = &torprdytexture;
			torpSpeaker4.Play(torpedoSound);
			engine.soundDevice->SetSourceLocation(torpSpeaker4, PlayerTransform2.position.x, PlayerTransform2.position.y, 0);

		}

		// palyer 3
		if (player3.projectileTime1 > 0) {
			torpicon5.texture = &torprldtexture;
		}
		else {
			torpicon5.texture = &torprdytexture;
			torpSpeaker5.Play(torpedoSound);
			engine.soundDevice->SetSourceLocation(torpSpeaker5, PlayerTransform3.position.x, PlayerTransform3.position.y, 0);
		}
		if (player3.projectileTime2 > 0) {
			torpicon6.texture = &torprldtexture;
		}
		else {
			torpicon6.texture = &torprdytexture;
			torpSpeaker6.Play(torpedoSound);
			engine.soundDevice->SetSourceLocation(torpSpeaker6, PlayerTransform3.position.x, PlayerTransform3.position.y, 0);
		}

		// palyer 4 

		if (player4.projectileTime1 > 0) {
			torpicon7.texture = &torprldtexture;
		}
		else {
			torpicon7.texture = &torprdytexture;
			torpSpeaker7.Play(torpedoSound);
			engine.soundDevice->SetSourceLocation(torpSpeaker7, PlayerTransform4.position.x, PlayerTransform4.position.y, 0);
		}
		if (player4.projectileTime2 > 0) {
			torpicon8.texture = &torprldtexture;
		}
		else {
			torpicon8.texture = &torprdytexture;
			torpSpeaker8.Play(torpedoSound);
			engine.soundDevice->SetSourceLocation(torpSpeaker8, PlayerTransform4.position.x, PlayerTransform4.position.y, 0);
		}

		//engine Sounds
		engine.soundDevice->SetSourceLocation(engineSpeaker, PlayerTransform.position.x, PlayerTransform.position.y, 0);
		engine.soundDevice->SetSourceLocation(engineSpeaker2, PlayerTransform2.position.x, PlayerTransform2.position.y, 0);
		engine.soundDevice->SetSourceLocation(engineSpeaker3, PlayerTransform3.position.x, PlayerTransform3.position.y, 0);
		engine.soundDevice->SetSourceLocation(engineSpeaker4, PlayerTransform4.position.x, PlayerTransform4.position.y, 0);
		//adding pitch based on speed
		float normalizedVelocity = PlayerRigidbody.velocity.Length() / 166.0f;
		float accLevel = std::lerp(0.0f, 1.5f, normalizedVelocity);
		engineSpeaker.setPitch(0.5f + accLevel);

		float normalizedVelocity2 = PlayerRigidbody2.velocity.Length() / 166.0f;
		float accLevel2 = std::lerp(0.0f, 1.5f, normalizedVelocity2);
		engineSpeaker2.setPitch(0.5f + accLevel2);

		float normalizedVelocity3 = PlayerRigidbody3.velocity.Length() / 166.0f;
		float accLevel3 = std::lerp(0.0f, 1.5f, normalizedVelocity3);
		engineSpeaker3.setPitch(0.5f + accLevel3);

		float normalizedVelocity4 = PlayerRigidbody4.velocity.Length() / 166.0f;
		float accLevel4 = std::lerp(0.0f, 1.5f, normalizedVelocity4);
		engineSpeaker4.setPitch(0.5f + accLevel4);
		
		
		engine.soundDevice->SetSourceLocation(cheerSpeaker, 1530, -1700, 1);
		
		if (player.playExlposionSound) {
			
			explosionSpeaker.Play(explosionSound);
			engine.soundDevice->SetSourceLocation(explosionSpeaker, PlayerTransform.position.x, PlayerTransform.position.y, 0);
			player.playExlposionSound = false;
		}
		if (player2.playExlposionSound) {

			explosionSpeaker.Play(explosionSound);
			engine.soundDevice->SetSourceLocation(explosionSpeaker, PlayerTransform2.position.x, PlayerTransform2.position.y, 0);
			player2.playExlposionSound = false;
		}
		if (player3.playExlposionSound) {

			explosionSpeaker.Play(explosionSound);
			engine.soundDevice->SetSourceLocation(explosionSpeaker, PlayerTransform3.position.x, PlayerTransform3.position.y, 0);
			player3.playExlposionSound = false;
		}
		if (player4.playExlposionSound) {

			explosionSpeaker.Play(explosionSound);
			engine.soundDevice->SetSourceLocation(explosionSpeaker, PlayerTransform4.position.x, PlayerTransform4.position.y, 0);
			player4.playExlposionSound = false;
		}
		

		//Keep the camera in bounds of the tilemap and set it to the average position of the players
		Vector3 avgPos = playerController->avgPosition / playerController->entities.size();
		float camPosX = clamp(avgPos.x, map.position.x + cam.width / 2, map.position.x + map.bounds.width - cam.width / 2);
		float camPosY = clamp(avgPos.y, map.position.y - map.bounds.height + cam.height / 2, map.position.y - cam.height / 2) - cam.height * 0.07;
		cam.SetPosition(Vector3(camPosX, camPosY, 1500));
		
		//Calculate the camera's bounds
		std::array<float, 4> camBounds{
				cam.position.y * 2 + cam.height / 2,
				cam.position.x * 2 + cam.width / 2,
				cam.position.y * 2 - cam.height / 2,
				cam.position.x * 2 - cam.width / 2};
		//Calculate the difference between the player and camera bounds
		float topDiff = camBounds[0] - playerController->playerBounds[0];
		float rightDiff = camBounds[1] - playerController->playerBounds[1];
		float bottomDiff = playerController->playerBounds[2] - camBounds[2];
		float leftDiff = playerController->playerBounds[3] - camBounds[3];

		//Zoom out
		if (topDiff - camPadding < -camDeadzone || rightDiff - camPadding < -camDeadzone || bottomDiff - camPadding < -camDeadzone || leftDiff - camPadding < -camDeadzone)
			camScale += 10 - min(topDiff, min(bottomDiff, min(rightDiff, leftDiff))) / 10;
		//Zoom in
		else if (topDiff - camPadding > camDeadzone && rightDiff - camPadding > camDeadzone && bottomDiff - camPadding > camDeadzone && leftDiff - camPadding > camDeadzone)
			camScale -= min(topDiff, min(bottomDiff, min(rightDiff, leftDiff))) / 100;

		//Clamp the camera zoom between min and max and set it's dimensions
		camScale = clamp(camScale, 800.f, 1850.f);
		cam.height = camScale;
		cam.width = cam.height * aspectRatio;

		//Reset the average player position data
		playerController->avgPosition = Vector3();
		playerController->playerBounds = { -INFINITY, -INFINITY, INFINITY, INFINITY };

		engine.soundDevice->SetLocation(camPosX, camPosY, 1);
		engine.soundDevice->SetOrientation(0.f, 0.f, -1.f, 0.f, 0.f, 1.f);

		engine.Update(&cam);

		// playerControl Update for frame 
		playerController->Update(window, engine.deltaTime);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}