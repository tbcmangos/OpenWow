#pragma once

#include "M2.h"

// Components
#include "M2_AnimatorComponent.h"
#include "M2_SkeletonComponent.h"
#include "M2_ParticlesComponent.h"
#include "M2_CameraComponent.h"
#include "M2_LightComponent.h"

class ZN_API CM2_Base_Instance 
	: public CSceneNode
	, public CLoadableObject
{
public:
	CM2_Base_Instance(IScene& Scene, const std::shared_ptr<CM2>& M2Object);
	virtual ~CM2_Base_Instance();

	// CM2_Base_Instance
	const CM2&                          GetM2() const;

	void                                Attach(EM2_AttachmentPoint AttachmentType);
	void                                Detach();
	void                                UpdateAttachPositionAfterSkeletonUpdate();

	std::shared_ptr<CM2CameraComponent> CreateCameraComponent(uint16 CameraDirectIndex);

	// Color & Alpha
	void                                setColor(glm::vec4 Color) { m_Color = Color; }
	const glm::vec4&                    getColor() const { return m_Color; }
	void                                setAlpha(float Alpha) { m_Alpha = Alpha; }
	float                               getAlpha() const { return m_Alpha; }

	// Mesh & textures provider
	virtual bool                        isMeshEnabled(uint32 _index) const;
	void                                setSpecialTexture(SM2_Texture::Type _type, const std::shared_ptr<ITexture>& _texture);
	const std::shared_ptr<ITexture>&    getSpecialTexture(SM2_Texture::Type _type) const;

	// Components
	const std::shared_ptr<CM2AnimatorComponent>& GetAnimatorComponent() const { return m_AnimatorComponent; }
	const std::shared_ptr<CM2SkeletonComponent>  getSkeletonComponent() const { return m_SkeletonComponent; }
#ifdef USE_M2_PARTICLES
	const std::shared_ptr<CM2ParticlesComponent> getParticleComponent() const { return m_ParticleComponent; }
#endif

	// ISceneNode
	void								Initialize() override;
	virtual void                        RegisterComponents() override;
	virtual void                        Update(const UpdateEventArgs& e) override;
	virtual void                        Accept(IVisitor* visitor) override;

	// ILoadable
	bool                                Load() override;
	bool                                Delete() override;

protected:
	virtual glm::mat4                   CalculateLocalTransform() const override;

private:
	// This M2Instance attached to parent
	EM2_AttachmentPoint                 m_AttachmentType;

	// Color & Alpha
	glm::vec4                           m_Color;
	float                               m_Alpha;

	// Mesh & Replaces textures
	std::shared_ptr<ITexture>		    m_SpecialTextures[SM2_Texture::Type::COUNT];

	// Animtion
	std::shared_ptr<CM2AnimatorComponent>           m_AnimatorComponent;
	std::shared_ptr<CM2SkeletonComponent> m_SkeletonComponent;
#ifdef USE_M2_PARTICLES
	std::shared_ptr<CM2ParticlesComponent>m_ParticleComponent;
#endif
	std::shared_ptr<CM2LightComponent> m_LightComponent;

private:
	std::shared_ptr<CM2> m_M2;
};
