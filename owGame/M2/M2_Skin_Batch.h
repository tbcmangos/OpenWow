#pragma once

#include "M2_Part_Material.h"
#include "M2_SkinSection.h"

// FORWARD BEGIN
class CM2;
class CM2_Base_Instance;
// FORWARD END


namespace
{
	__declspec(align(16)) struct ShaderM2BatchProperties
	{
		ShaderM2BatchProperties()
			: gBlendMode(0)
			, gShader(0)
			, gTextureWeight(1.0f)
			, gTextureAnimEnable(0)

			, gMaterialColorAndAlpha(glm::vec4(1.0f))

			, gTextureAnimMatrix(glm::mat4(1.0f))
		{}

		uint32     gBlendMode;
		uint32     gShader;
		float      gTextureWeight;
		uint32     gTextureAnimEnable;
		//--------------------------------------------------------------( 16 bytes )

		glm::vec4  gMaterialColorAndAlpha;
		//--------------------------------------------------------------( 16 bytes )

		glm::mat4  gTextureAnimMatrix;
		//--------------------------------------------------------------( 16 * 4 bytes )
	};
}

/**
  * M2 Batch ���-�� ����� ������� ��� ����������. ����� ���� ��������, ����� � ��.
  * ��������� ������ ����� �������� ���� � �� �� ���������
*/
class CM2_Skin_Batch 
	: public MaterialProxieT<ShaderM2BatchProperties>
{
public:
	CM2_Skin_Batch(const IBaseManager& BaseManager, IRenderDevice& RenderDevice, const CM2& M2Model, const SM2_SkinBatch& SkinBatchProto);
	virtual ~CM2_Skin_Batch();

	int8 GetPriorityPlan() const { return m_SkinBatchProto.priorityPlane; }
	const std::shared_ptr<const CM2_Part_Material>& GetM2Material() const {	return m_M2ModelMaterial; }

	void UpdateMaterialProps(const RenderEventArgs& RenderEventArgs, const CM2_Base_Instance* M2Instance);

private:
	void FixMyShaderID();

private:
	int32												m_ShaderID;

	// Material props
	std::shared_ptr<const CM2_Part_Color>				m_Color;
	std::shared_ptr<const CM2_Part_Material>			m_M2ModelMaterial;
	std::vector<std::weak_ptr<const CM2_Part_Texture>>	m_Textures;
	int16												m_TextureUnit;
	std::shared_ptr<const CM2_Part_TextureWeight>		m_TextureWeight;
	std::shared_ptr<const CM2_Part_TextureTransform>	m_TextureTransform;

	const IBaseManager&                                 m_BaseManager;
	IRenderDevice&                                      m_RenderDevice;
	const CM2&                                          m_M2Model;
	const SM2_SkinBatch                                 m_SkinBatchProto;
};
