#include "stdafx.h"

#ifdef USE_WMO_MODELS

// General
#include "MapWMOInstance.h"

// Additional
#include "WMO\\RenderPass_WMO.h"

CMapWMOInstance::CMapWMOInstance(IScene& Scene, const std::shared_ptr<CWMO>& WMOObject, const ADT_MODF& _placementInfo)
	: CWMO_Base_Instance(Scene, WMOObject)
	, m_PlacementInfo(_placementInfo)
{
}

CMapWMOInstance::~CMapWMOInstance()
{
}

//
// CWMO_Base_Instance
//
uint16 CMapWMOInstance::GetDoodadSetIndex() const
{
	return m_PlacementInfo.doodadSetIndex;
}

//
// ISceneNode
//
void CMapWMOInstance::Initialize()
{
	__super::Initialize();

	uint16 doodadSetIndex = m_PlacementInfo.doodadSetIndex;
	//m_DoodadSetInfo = _wmoObject->m_DoodadsSetInfos[doodadSetIndex];

	SetPosition(m_PlacementInfo.position);
	SetLocalRotationEuler(m_PlacementInfo.rotation);
}

void CMapWMOInstance::Accept(IVisitor* visitor)
{
	const auto& idIter = m_AlreadyDraw.find(m_PlacementInfo.uniqueId);
	if (idIter != m_AlreadyDraw.end())
	{
		if (idIter->second != this)
			return;
	}
	else
	{
		m_AlreadyDraw.insert(std::make_pair(m_PlacementInfo.uniqueId, this));
	}

	CWMO_Base_Instance::Accept(visitor);
}


//
// Protected
//
glm::mat4 CMapWMOInstance::CalculateLocalTransform() const
{
	glm::mat4 localTransform = glm::mat4(1.0f);
	localTransform = glm::translate(localTransform, GetPosition());
	localTransform = glm::rotate(localTransform, glm::radians(GetLocalRotationEuler().y - 90.0f), glm::vec3(0, 1, 0));
	localTransform = glm::rotate(localTransform, glm::radians(-GetLocalRotationEuler().x), glm::vec3(0, 0, 1));
	localTransform = glm::rotate(localTransform, glm::radians(GetLocalRotationEuler().z), glm::vec3(1, 0, 0));
	return localTransform;
}

//

void CMapWMOInstance::reset()
{
	m_AlreadyDraw.clear();
}
std::unordered_map<uint32, const CMapWMOInstance*> CMapWMOInstance::m_AlreadyDraw;

#endif