#pragma once

// FORWARD BEGIN
class CMapTile;
// FORWARD END

namespace
{
__declspec(align(16)) struct ADT_MCNK_MaterialMaterialProperties
{
	ADT_MCNK_MaterialMaterialProperties()
		: LayersCnt(1)
		, IsShadowMapExists(0)
		, IsMCCVExists(0)
		, IsNortrend(0)
	{}
	uint32 LayersCnt;
	uint32 IsShadowMapExists;
	uint32 IsMCCVExists;
	uint32 IsNortrend;
	//-------------------------- ( 32 bytes )
};
}

class CMapChunkMaterial 
	: public MaterialProxieT<ADT_MCNK_MaterialMaterialProperties>
{
public:
	CMapChunkMaterial(IRenderDevice& RenderDevice);
	virtual ~CMapChunkMaterial();

	void SetLayersCnt(uint32 value);
	void SetIsShadowMapExists(uint32 value);
	void SetIsMCCVExists(uint32 value);
	void SetIsNortrend(uint32 value);
};