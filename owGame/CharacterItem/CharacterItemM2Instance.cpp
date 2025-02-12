#include "stdafx.h"

#ifdef USE_M2_MODELS

// Include
#include "../Character/Character.h"
#include "CharacterItem.h"
#include "World/WorldClient.h"

// General
#include "CharacterItemM2Instance.h"

// Additional
#include "CharacterItemEffectM2Instance.h"

CCharacterItemM2Instance::CCharacterItemM2Instance(IScene& Scene, CWorldClient& WorldClient, const std::shared_ptr<CM2>& M2Object, const CCharacterItem& CharacterItem, EM2_AttachmentPoint AttachmentPoint)
	: CM2_Base_Instance(Scene, WorldClient, M2Object)
	, m_CharacterItem(CharacterItem)
	, m_AttachmentPoint(AttachmentPoint)
{}

CCharacterItemM2Instance::~CCharacterItemM2Instance()
{}



//
// CCharacterItemM2Instance
//
const CCharacterItem& CCharacterItemM2Instance::GetCharacterItem() const 
{ 
	//if (auto loadable = dynamic_cast<const ILoadable*>(&m_CharacterItem))
	//{
	//	auto state = loadable->GetState();
	//	if (state != ILoadable::ELoadableState::Loaded)
	//		throw CException("CCharacterItemM2Instance::GetCharacterItem: CharacterItem isn't loaded. State = '%d'.", state);
	//}

	return m_CharacterItem; 
}



//
// ILoadable
//
void CCharacterItemM2Instance::OnAfterLoad()
{
	// Item enchants
	const DBC_ItemVisualsRecord* itemVisualsRecord = GetBaseManager().GetManager<CDBCStorage>()->DBC_ItemVisuals()[GetCharacterItem().GetTemplate().EnchantAuraID];
	if (itemVisualsRecord != nullptr)
	{
		for (uint32 itemAttachmentPoint = 0; itemAttachmentPoint < DBC_ItemVisuals_VisualEffect_Count; itemAttachmentPoint++)
		{
			uint32 itemVisualEffectID = itemVisualsRecord->Get_VisualEffect(itemAttachmentPoint);
			const DBC_ItemVisualEffectsRecord* itemVisualEffectRecord = GetBaseManager().GetManager<CDBCStorage>()->DBC_ItemVisualEffects()[itemVisualEffectID];
			if (itemVisualEffectRecord == nullptr)
				continue;

			std::string itemVisualEffectRecordModelFilename = itemVisualEffectRecord->Get_Name();
			if (itemVisualEffectRecordModelFilename.empty())
				continue;

			// M2 model
			auto itemVisualEffectModel = GetWorldClient().GetCreator()->LoadM2(GetRenderDevice(), itemVisualEffectRecordModelFilename);
			if (itemVisualEffectModel == nullptr)
				throw CException("CCharacterItem::InitializeItemModels: itemVisualEffectM2Model is nullptr. ItemVisualsRecord ID = '%d', ItemVisualEffectsRecord = '%d' Index = '%d', Path '%s'.", GetCharacterItem().GetTemplate().EnchantAuraID, itemVisualEffectID, itemAttachmentPoint, itemVisualEffectRecordModelFilename.c_str());

			// M2 Instance
			auto itemVisualEffectInstance = CreateSceneNode<CCharacterItemEffectM2Instance>(GetWorldClient(), itemVisualEffectModel, m_CharacterItem);
			itemVisualEffectInstance->AddParentLoadable(std::dynamic_pointer_cast<ILoadable>(shared_from_this()));
			GetBaseManager().GetManager<ILoader>()->AddToLoadQueue(itemVisualEffectInstance);

			// If item contains attachment, then attach to item's attachment point, else to item same position
			if (GetM2().getMiscellaneous().isAttachmentExists((EM2_AttachmentPoint)itemAttachmentPoint))
			{
				auto itemModelAttachment = GetM2().getMiscellaneous().getAttachment((EM2_AttachmentPoint)itemAttachmentPoint);
				itemVisualEffectInstance->Attach(itemModelAttachment->GetAttachmentType()); // Attach visual effect to itemInstace attachment
			}
		}
	}
}

#endif