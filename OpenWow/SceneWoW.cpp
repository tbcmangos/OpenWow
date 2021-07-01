#include "stdafx.h"

// Gerenal
#include "SceneWoW.h"

CSceneWoW::CSceneWoW(IBaseManager& BaseManager, IRenderWindow& RenderWindow)
	: SceneBase(BaseManager, RenderWindow)
{}

CSceneWoW::~CSceneWoW()
{
	Log::Info("Scene destroyed.");
}


//
// IGameState
//
void CSceneWoW::Initialize()
{
	__super::Initialize();

	// Light
	{
		auto lightNode = CreateSceneNodeT<ISceneNode>();
		lightNode->SetName("Light");
		lightNode->SetLocalPosition(glm::vec3(-50.0f, 250.0f, -50.0f));
		lightNode->SetLocalRotationDirection(glm::vec3(-0.5, -0.5f, -0.5f));
		//lightNode->SetLocalRotationEuler(glm::vec3(45.0f, -45.0f, 0.0f));

		auto lightComponent = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IComponentFactory>()->CreateComponentT<CLightComponent>(cSceneNodeLightComponent, *lightNode.get());
		lightComponent->SetLight(MakeShared(CLight, GetBaseManager()));
		lightComponent->GetLight()->SetCastShadows(true);
		lightComponent->GetLight()->SetType(ELightType::Spot);
		lightComponent->GetLight()->SetAmbientColor(ColorRGB(0.25f));
		lightComponent->GetLight()->SetColor(ColorRGB(1.0f, 1.0f, 1.0f));
		lightComponent->GetLight()->SetRange(1000.0f);
		lightComponent->GetLight()->SetIntensity(1.0077f);
		lightComponent->GetLight()->SetSpotlightAngle(30.0f);

		lightNode->AddComponent(cSceneNodeLightComponent, lightComponent);
	}

	// Camera
	{
		auto cameraNode = CreateSceneNodeT<ISceneNode>();
		cameraNode->SetName("Camera");
		cameraNode->AddComponentT(GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IComponentFactory>()->CreateComponentT<ICameraComponent3D>(cSceneNodeCameraComponent, *cameraNode));

		SetCameraController(MakeShared(CFreeCameraController));
		GetCameraController()->SetCamera(cameraNode->GetComponentT<ICameraComponent3D>());
		GetCameraController()->GetCamera()->SetPerspectiveProjection(65.0f, static_cast<float>(GetRenderWindow().GetWindowWidth()) / static_cast<float>(GetRenderWindow().GetWindowHeight()), 1.0f, 2500.0f);
		GetCameraController()->GetCamera()->SetPosition(glm::vec3(100.0f));
		GetCameraController()->GetCamera()->SetYaw(225);
		GetCameraController()->GetCamera()->SetPitch(-45);
	}

	const float cMinimapOffset = 10.0f;
	minimap = GetRootUIControl()->CreateUIControl<CUIControlMinimap>();
	minimap->SetPosition(glm::vec2(GetRenderWindow().GetWindowWidth() - minimap->GetSize().x - cMinimapOffset, cMinimapOffset));
	minimap->SetOnClickCallback([this](const IUIControl* Node, glm::vec2 Point) { this->GoToCoord(Node, Point); });


	m_RendererStatisticText = CreateUIControlTCast<IUIControlText>();
	m_RendererStatisticText->SetLocalPosition(glm::vec2(5.0f, 200.0f));
	m_RendererStatisticText->SetText("");


	GetBaseManager().GetManager<IWoWObjectsCreator>()->InitEGxBlend(GetRenderDevice());


	m_WoWSkyManager = GetRootSceneNode()->CreateSceneNode<SkyManager>();
	GetBaseManager().AddManager<SkyManager>(m_WoWSkyManager);

	m_WoWMap = GetRootSceneNode()->CreateSceneNode<CMap>();

	//const float x = 31; //0 fire
	//const float y = 28; //0 fire
	//const uint32 mapID = 0;

	//const float x = 40; //1 barrens
	//const float y = 30; //1 barrens
	//const uint32 mapID = 1;

	//const float x = 26; //530 outland
	//const float y = 32; //530 outland
	//const uint32 mapID = 530;

	//const float x = 30; //571 nortrend
	//const float y = 21; //571 nortrend
	//const uint32 mapID = 571;


	//const float x = 32; //571 44
	//const float y = 32; //571 44
	//const uint32 mapID = 451;

	// Ulduar
	const float x = 0; //571 nortrend
	const float y = 0; //571 nortrend
	const uint32 mapID = 631;

	if (true)
	{
		m_WoWSkyManager->Load(mapID);
		

		glm::vec3 position = glm::vec3(x * C_TileSize + C_TileSize / 2.0f, 100.0f, y * C_TileSize + C_TileSize / 2.0f);

		m_WoWMap->MapPreLoad(GetBaseManager().GetManager<CDBCStorage>()->DBC_Map()[mapID]);

		minimap->SetMinimapTexture(m_WoWMap->getMinimap());

		m_WoWMap->MapLoad();
		m_WoWMap->EnterMap(position);

		GetCameraController()->GetCamera()->SetPosition(position);

		//GetCameraController()->GetCamera()->SetPosition(glm::vec3(14300, 150, 20500));
	}
	else if (true)
	{
		m_WoWSkyManager->Load(0);

		// WORLD\\WMO\\KALIMDOR\\OGRIMMAR\\OGRIMMAR.WMO

		auto wmoModel = GetBaseManager().GetManager<IWoWObjectsCreator>()->LoadWMO(GetRenderDevice(), "WORLD\\WMO\\NORTHREND\\DALARAN\\ND_DALARAN.WMO");
		
		auto wmoInstance = GetRootSceneNode()->CreateSceneNode<CWMO_Base_Instance>(wmoModel);
		GetBaseManager().GetManager<ILoader>()->AddToLoadQueue(wmoInstance);

		GetCameraController()->GetCamera()->SetPosition(glm::vec3(0.0f));
	}
	else if (false)
	{
		m_WoWSkyManager->Load(0);

		auto name = "WORLD\\GOOBER\\G_SCOURGERUNECIRCLECRYSTAL.M2";
		auto name2 = "WORLD\\EXPANSION02\\DOODADS\\NEXUS\\NEXUS_ENERGYCHAINS.M2";
		auto name3 = "WORLD\\NODXT\\GENERIC\\PASSIVEDOODADS\\VOLUMETRICLIGHTS\\LD_LIGHTSHAFT01.M2";
		auto name4 = "WORLD\\EXPANSION02\\DOODADS\\ICECROWN\\EFFECTS\\ICECROWN_ICECORE.M2";
		auto name5 = "WORLD\\EXPANSION02\\DOODADS\\DALARAN\\DALARAN_BUILDINGCRYSTAL_01.M2";
		auto name6 = "WORLD\\EXPANSION02\\DOODADS\\HOWLINGFJORD\\FOG\\HFJORD_FOG_01.M2";
		auto name7 = "WORLD\\EXPANSION02\\DOODADS\\GENERIC\\WATERFALLS\\WATERFALLS_SET2_MED_TALL_WITHCHOP.M2";
		auto name8 = "Creature\\spirithealer\\SpiritHealer.m2";

		

		auto m2Model = GetBaseManager().GetManager<IWoWObjectsCreator>()->LoadM2(GetRenderDevice(), name8);

		auto m2Instance = GetRootSceneNode()->CreateSceneNode<CM2_Base_Instance>(m2Model);
		GetBaseManager().GetManager<ILoader>()->AddToLoadQueue(m2Instance);

		GetCameraController()->GetCamera()->SetPosition(glm::vec3(0.0f));
	}
	else
	{
		m_WoWSkyManager->Load(0);

		auto creatureInstance = dynamic_cast<CWorldObjectCreator*>(GetBaseManager().GetManager<IWoWObjectsCreator>())->BuildCharacterFromDisplayInfo(GetRenderDevice(), *this, 10747);
		GetBaseManager().GetManager<ILoader>()->AddToLoadQueue(creatureInstance);

		GetCameraController()->GetCamera()->SetPosition(glm::vec3(0.0f));
	}
}

void CSceneWoW::Finalize()
{
	SceneBase::Finalize();
}

void CSceneWoW::OnUpdate(UpdateEventArgs & e)
{
	CMapWMOInstance::reset();
	CMapM2Instance::reset();

	__super::OnUpdate(e);

	m_WoWSkyManager->Update(e);
	m_WoWMap->Update(e);

	m_RendererStatisticText->SetText(GetRenderer()->GetStatisticText());
}

bool CSceneWoW::OnMousePressed(const MouseButtonEventArgs & e, const Ray& RayToWorld)
{
	auto selectedNodes = GetFinder().FindIntersection(RayToWorld, nullptr);
	if (false == selectedNodes.empty())
	{
		for (const auto& selectedNodesIt : selectedNodes)
		{
			if (auto m2Node = std::dynamic_pointer_cast<CM2_Base_Instance>(selectedNodesIt.second))
			{
				Log::Green("Selected node '%f' = '%s'.", selectedNodesIt.first, m2Node->getM2().getFilename().c_str());
			}
		}
	}

	return false;
}

void CSceneWoW::OnMouseMoved(const MouseMotionEventArgs & e, const Ray & RayToWorld)
{}



//
// Keyboard events
//
bool CSceneWoW::OnWindowKeyPressed(KeyEventArgs & e)
{
	return SceneBase::OnWindowKeyPressed(e);
}

void CSceneWoW::OnWindowKeyReleased(KeyEventArgs & e)
{
	SceneBase::OnWindowKeyReleased(e);
}



//
// Protected
//
void CSceneWoW::InitializeRenderer()
{
	auto wowRenderer = MakeShared(CRendererWoW, GetBaseManager(), *this);
	wowRenderer->Initialize(GetRenderWindow().GetRenderTarget());
	m_ForwardRenderer = wowRenderer;
	m_DefferedRenderrer = wowRenderer;

	SetRenderer(wowRenderer);
}


void CSceneWoW::GoToCoord(const IUIControl* Node, const glm::vec2& Point)
{
	Log::Green("Coord %f %f", Point.x, Point.y);
	glm::vec2 conv = (Point) * (C_TileSize * 128.0f / 512.0f);

	GetCameraController()->GetCamera()->SetPosition(glm::vec3(conv.x, GetCameraController()->GetCamera()->GetPosition().y, conv.y));
}

#if 0


CSceneWoW::CSceneWoW(IBaseManager& BaseManager)
	: SceneBase(BaseManager)
{}

CSceneWoW::~CSceneWoW()
{
	CMapWMOInstance::reset();
#ifdef USE_M2_MODELS
	CMapM2Instance::reset();
#endif

	OutputDebugStringW(L"Destroyed.");
}


//
// IGameState
//
void CSceneWoW::Initialize()
{
	SceneBase::Initialize();

	auto cameraNode = GetRootNode3D()->CreateSceneNode<SceneNode3D>();
	cameraNode->AddComponent(std::make_shared<CCameraComponent3D>(*cameraNode));

	SetCameraController(std::make_shared<CFreeCameraController>());
	GetCameraController()->SetCamera(cameraNode->GetComponent<ICameraComponent3D>());
	GetCameraController()->GetCamera()->SetPerspectiveProjection(ICameraComponent3D::EPerspectiveProjectionHand::Right, 45.0f, static_cast<float>(GetRenderWindow()->GetWindowWidth()) / static_cast<float>(GetRenderWindow()->GetWindowHeight()), 0.5f, 10000.0f);

	
	Load3D();
	Load3D_M2s();
	//TestCreateMap(MapID);
	//LoadUI();


	std::shared_ptr<CSceneCreateTypedListsPass> sceneListPass = std::make_shared<CSceneCreateTypedListsPass>(GetRenderDevice(), shared_from_this());

	m_Technique3D.AddPass(GetBaseManager().GetManager<IRenderPassFactory>()->CreateRenderPass("ClearPass", GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport(), shared_from_this()));
	//m_Technique3D.AddPass(wmoListPass);
	//m_Technique3D.AddPass(m2ListPass);
	m_Technique3D.AddPass(sceneListPass);
	m_Technique3D.AddPass(std::make_shared<CRenderPass_Sky>(GetRenderDevice(), sceneListPass)->CreatePipeline(GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport()));
	//m_Technique3D.AddPass(std::make_shared<CRenderPass_WDL>(GetRenderDevice(), shared_from_this())->CreatePipeline(GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport()));
	m_Technique3D.AddPass(std::make_shared<CRenderPass_MapChunk>(GetRenderDevice(), sceneListPass)->CreatePipeline(GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport()));
	
	m_Technique3D.AddPass(std::make_shared<CRenderPass_WMO>(GetRenderDevice(), sceneListPass)->CreatePipeline(GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport()));
	
	m_Technique3D.AddPass(std::make_shared<CRenderPass_Liquid>(GetRenderDevice(), sceneListPass)->CreatePipeline(GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport()));

	//m_Technique3D.AddPass(std::make_shared<CRenderPass_WMO2>(GetRenderDevice(), wmoListPass, shared_from_this())->CreatePipeline(GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport()));
	
	m_Technique3D.AddPass(std::make_shared<CRenderPass_M2>(GetRenderDevice(), sceneListPass, true)->CreatePipeline(GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport()));
	m_Technique3D.AddPass(std::make_shared<CRenderPass_M2>(GetRenderDevice(), sceneListPass, false)->CreatePipeline(GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport()));
	
	//m_Technique3D.AddPass(std::make_shared<CRenderPass_M2_Instanced>(GetRenderDevice(), sceneListPass, true)->CreatePipeline(GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport()));
	//m_Technique3D.AddPass(std::make_shared<CRenderPass_M2_Instanced>(GetRenderDevice(), sceneListPass, false)->CreatePipeline(GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport()));
	
	//m_Technique3D.AddPass(std::make_shared<CDrawBoundingBoxPass>(GetRenderDevice(), shared_from_this())->CreatePipeline(GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport()));
	//m_Technique3D.AddPass(std::make_shared<CMaterialParticlePass>(GetRenderDevice(), shared_from_this())->CreatePipeline(GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport()));
}

void CSceneWoW::Finalize()
{
	// Insert code here

	SceneBase::Finalize();
}


void CSceneWoW::OnRayIntersected(const glm::vec3& Point)
{

}



//
//
//
void CSceneWoW::OnRender(RenderEventArgs& e)
{
	CMapWMOInstance::reset();

#ifdef USE_M2_MODELS
	CMapM2Instance::reset();
#endif

	//m2Instance->SetRotation(glm::vec3(m2Instance->GetRotation().x, m2Instance->GetRotation().y + 0.05f * e.DeltaTime / 60.0f, 0.0f));

	SceneBase::OnRender(e);
}



//
// Keyboard events
//
bool CSceneWoW::OnWindowKeyPressed(KeyEventArgs & e)
{
	if (e.Key == KeyCode::C)
	{
		TestCreateMap(530);
		return true;
	}
	else if (e.Key == KeyCode::V)
	{
		TestDeleteMap();
		return true;
	}

	return SceneBase::OnWindowKeyPressed(e);
}

void CSceneWoW::OnWindowKeyReleased(KeyEventArgs & e)
{
	SceneBase::OnWindowKeyReleased(e);
}



//
//
//
void CSceneWoW::Load3D()
{
	//CWorldObjectCreator creator(GetBaseManager());
	//auto creature = creator.BuildCreatureFromDisplayInfo(GetRenderDevice(), this, 69, GetRootNode3D());
	//creature->getAnimator()->PlayAnimation(4, true);


	//auto wmo = GetBaseManager().GetManager<IWoWObjectsCreator>()->LoadWMO(GetRenderDevice(), /*"World\\wmo\\Lorderon\\Undercity\\Undercity.wmo"*/"World\\wmo\\Outland\\DarkPortal\\DarkPortal_Temple.wmo");
	//wmoInstance = GetRootNode3D()->CreateSceneNode<CWMO_Base_Instance>(wmo);
	//GetBaseManager().GetManager<ILoader>()->AddToLoadQueue(wmoInstance);

	//auto m2 = GetBaseManager().GetManager<IWoWObjectsCreator>()->LoadM2(GetRenderDevice(), "Creature\\ELEMENTALEARTH\\ElementalEarth.m2");
	//m2Instance = GetRootNode3D()->CreateSceneNode<CM2_Base_Instance>(m2);
	//GetBaseManager().GetManager<ILoader>()->AddToLoadQueue(m2Instance);

	//auto m2 = GetBaseManager().GetManager<IWoWObjectsCreator>()->LoadM2(GetRenderDevice(), "World\\Expansion01\\Doodads\\Netherstorm\\Float_particles\\Netherstorm_Particles_Pink.mdx");
	//m2Instance = GetRootNode3D()->CreateSceneNode<CM2_Base_Instance>(m2);
	//m2Instance->SetScale(glm::vec3(100.0f));
	//GetBaseManager().GetManager<ILoader>()->AddToLoadQueue(m2Instance);

	

	m_WoWSkyManager = GetRootNode3D()->CreateSceneNode<SkyManager>(GetRenderDevice());
	GetBaseManager().AddManager<ISkyManager>(m_WoWSkyManager);
	m_WoWSkyManager->Load(0);

	map = GetRootNode3D()->CreateSceneNode<CMap>(GetBaseManager(), GetRenderDevice());

#if 0
	auto wmo = GetBaseManager().GetManager<IWMOManager>()->Add(GetRenderDevice(), "World\\wmo\\Azeroth\\Buildings\\Stormwind\\Stormwind.wmo");
	wmoInstance = GetRootNode3D()->CreateSceneNode<CWMO_Base_Instance>(*wmo);
	wmoInstance->SetTranslate(glm::vec3(100, 500, 100));
	wmoInstance->SetRotation(glm::vec3(0.0f, glm::pi<float>(), 0.0f));
	GetBaseManager().GetManager<ILoader>()->AddToLoadQueue(wmoInstance.get());
#endif

	GetCameraController()->GetCamera()->SetTranslation(glm::vec3());
	GetCameraController()->GetCamera()->SetYaw(48.8);
	GetCameraController()->GetCamera()->SetPitch(-27.8);
}

void CSceneWoW::Load3D_M2s()
{
	Random r(time(0));

	CWorldObjectCreator creator(GetBaseManager());

	const auto& records = GetBaseManager().GetManager<CDBCStorage>()->DBC_CreatureDisplayInfo().Records();
	for (size_t i = 0; i < 25; i++)
	{
		for (size_t j = 0; j < 25; j++)
		{
			size_t id = r.NextUInt() % records.size();

			while (true)
			{
				const auto& data = records.find(id);
				if (data != records.end())
				{
					break;
				}

				id = r.NextUInt() % records.size();
			}

			auto creature = creator.BuildCreatureFromDisplayInfo(GetRenderDevice(), this, id, GetRootNode3D());
			if (creature != nullptr)
			{
				creature->SetTranslate(glm::vec3(i * 17.5f, 0.0f, j * 17.5f));
				if (creature->getAnimator())
					creature->getAnimator()->PlayAnimation(4, true);
			}
		}
	}

	GetCameraController()->GetCamera()->SetTranslation(glm::vec3(150, 150, 150));
	GetCameraController()->GetCamera()->SetYaw(235);
	GetCameraController()->GetCamera()->SetPitch(-45);
}


void CSceneWoW::LoadUI()
{
#if 1
	rootForBtns = GetRootNodeUI()->CreateSceneNode<SceneNodeUI>();

	minimap = GetRootNodeUI()->CreateSceneNode<CUITextureNode>(GetRenderDevice(), glm::vec2(256, 256));
	minimap->SetTranslate(glm::vec2(900, 800));
	minimap->SetOnClickCallback([this](const ISceneNodeUI* Node, glm::vec2 Point) { this->GoToCoord(Node, Point); });

	size_t cntrX = 0;
	size_t cntrY = 0;

	auto dbcMap = GetBaseManager().GetManager<CDBCStorage>();
	for (const auto& it : dbcMap->DBC_Map())
	{
		auto btn = rootForBtns->CreateSceneNode<CUIButtonNode>(GetRenderDevice());
		btn->CreateDefault();
		volatile std::wstring name = it->Get_Name();

		btn->SetText(it->Get_Directory());

		uint32 id = it->Get_ID();
		btn->SetOnClickCallback([this, id] (const ISceneNodeUI* Node, glm::vec2 Point) { this->TestCreateMap(id); });
		btn->SetTranslate(glm::vec2(cntrX * 180, cntrY * 38));

		cntrY++;
		if (cntrY > 20)
		{
			cntrY = 0;
			cntrX++;
		}
	}

#endif
	m_TechniqueUI.AddPass(std::make_shared<CUIButtonPass>(GetRenderDevice(), shared_from_this())->CreatePipeline(GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport()));
	m_TechniqueUI.AddPass(std::make_shared<CUIFontPass>(GetRenderDevice(), shared_from_this())->CreatePipeline(GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport()));
	m_TechniqueUI.AddPass(std::make_shared<CUITexturePass>(GetRenderDevice(), shared_from_this())->CreatePipeline(GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport()));
}

void CSceneWoW::TestCreateMap(uint32 MapID)
{
	//const int32 x = 40;
	//const int32 y = 30;
	//const uint32 mapID = 0;
	//const int32 x = 19; //FOR BC
	//const int32 y = 32;
	const int32 x = 26; //FOR BC 2
	const int32 y = 30;

	m_WoWSkyManager->Load(MapID);

	rootForBtns->GetParent().lock()->RemoveChild(rootForBtns);
	map->Unload();


	map->MapPreLoad(GetBaseManager().GetManager<CDBCStorage>()->DBC_Map()[MapID]);
	map->MapLoad();
	map->EnterMap(x, y);

	minimap->SetTexture(map->getMinimap());

	//GetCameraController()->GetCamera()->SetTranslation(glm::vec3(x * C_TileSize + C_TileSize / 2.0f, 100.0f, y * C_TileSize + C_TileSize / 2.0f));
	//GetCameraController()->GetCamera()->SetYaw(48.8);
	//GetCameraController()->GetCamera()->SetPitch(-27.8);

	//GetCameraController()->GetCamera()->SetTranslation(glm::vec3(0, 100.0f, 0));

	//if (map->getGlobalInstance())
	//{
	//	GetCameraController()->GetCamera()->SetPosition(map->getGlobalInstance()->GetPosition());
	//}
}

void CSceneWoW::GoToCoord(const IUIControl* Node, const glm::vec2& Point)
{
	Log::Green("Coord %f %f", Point.x, Point.y);
	glm::vec2 conv = (Point / Node->GetSize() * 512.0f) * (C_TileSize * 64.0f / 512.0f);

	GetCameraController()->GetCamera()->SetPosition(glm::vec3(conv.x, GetCameraController()->GetCamera()->GetPosition().y, conv.y));
}

void CSceneWoW::TestDeleteMap()
{
	map->Unload();
}

#endif