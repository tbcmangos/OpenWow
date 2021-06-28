#pragma once

#ifdef USE_WMO_MODELS

#include "../../WMO/WMO_Base_Instance.h"
#include "../Map_Headers.h"

class ZN_API CMapWMOInstance 
	: public CWMO_Base_Instance
{
public:
	CMapWMOInstance(IScene& Scene, const std::shared_ptr<CWMO>& WMOObject, const SMapTile_MODF& _placementInfo);
	virtual ~CMapWMOInstance();

	// CWMO_Base_Instance
	uint16 GetDoodadSetIndex() const override;

	// ISceneNode
	void Initialize() override;
	void Accept(IVisitor* visitor) override;

protected:
	virtual glm::mat4 CalculateLocalTransform() const override;
	
private:
	const SMapTile_MODF m_PlacementInfo;

public:	// Static
	static void reset();
private:
	static std::map<uint32, const CMapWMOInstance*> m_AlreadyDraw;
};

#endif