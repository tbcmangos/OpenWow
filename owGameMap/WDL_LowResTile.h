#pragma once

// FORWARD BEGIN
class MapController;
// FORWARD END

class CWDL_LowResTile : public SceneNode
{
public:
	CWDL_LowResTile(std::weak_ptr<MapController> _parent, uint32 _indexX, uint32 _indexZ, std::shared_ptr<IMesh> _geom);

	// SceneNode
	void Render(RenderEventArgs& renderEventArgs);

private:
	const int				           m_IndexX, m_IndexZ;
	std::shared_ptr<IMesh>              m_Geom;

private: // PARENT
	const std::weak_ptr<MapController> m_MapController;
};