#pragma once

#ifdef USE_WMO_MODELS

#include "WMOGroup.h"
#include "WMO_Portal.h"

// FORWARD BEGIN
class CWMO_Base_Instance;
#ifdef USE_M2_MODELS
class CWMO_Doodad_Instance;
#endif
class CWMO_Liquid_Instance;
class CWorldClient;
// FORWARD END

class CWMO_Group_Instance 
	: public CSceneNode
	, public CLoadableObject
	, public IPortalRoom
{
public:
#ifdef USE_M2_MODELS
	typedef std::vector<CWMO_Doodad_Instance*> DoodadInstances;
#endif
	typedef std::vector<CWMO_Liquid_Instance*> LuqidInstances;

public:
	CWMO_Group_Instance(IScene& Scene, CWorldClient& WorldClient, const std::shared_ptr<CWMOGroup>& WMOGroupObject);
	virtual ~CWMO_Group_Instance();

	// ILoadable
	bool Load() override;
	bool Delete() override;

	// IPortalRoom
	void AddPortal(const std::shared_ptr<IPortal>& Portal) override final;
	const std::vector<std::shared_ptr<IPortal>>& GetPortals() const override final;
	void AddRoomObject(const std::weak_ptr<IPortalRoomObject>& RoomObject) override final;
	const std::vector<std::weak_ptr<IPortalRoomObject>>& GetRoomObjects() const override final;
	bool IsIndoor() const override final;
	bool IsOutdoor() const override final;
	void Reset() override final;
	BoundingBox GetBoundingBox() const override final;
	bool IsPointInside(const glm::vec3& CameraPosition) const override final;
	void SetVisibilityState(bool Value) override final;
	bool IsVisible() const override final;
	void SetCalculatedState(bool Value) override final;
	bool IsCalculated() const override final;

	// CWMO_Group_Instance
	CWorldClient& GetWorldClient() const { return m_WorldClient; }
	const CWMOGroup& GetWMOGroup() const;
#ifdef USE_WMO_PORTALS_CULLING
	void CreatePortals(const std::shared_ptr<CWMO_Base_Instance>& BaseInstance);
#endif

	// ISceneNode
	void Initialize() override;
	void Accept(IVisitor* visitor) override;

private:
	std::vector<std::shared_ptr<IPortal>>         m_RoomPortals;
	std::vector<std::weak_ptr<IPortalRoomObject>> m_PortalRoomObjects;
	bool                                          m_IsThisRoomVisible;
	bool                                          m_Calculated;

private:
	CWorldClient& m_WorldClient;
	const CWMOGroup& m_WMOGroup;
};

#endif
