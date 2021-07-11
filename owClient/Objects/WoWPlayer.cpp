#include "stdafx.h"

#ifdef ENABLE_WOW_CLIENT

// General
#include "WoWPlayer.h"

// Additional
#include "../World/World.h"

WoWPlayer::WoWPlayer(IScene& Scene, CWoWWorld& WoWWorld, CWoWGuid Guid)
	: WoWUnit(Scene, WoWWorld, Guid)
{
	//m_ObjectType |= TYPEMASK_PLAYER;
	m_Values.SetValuesCount(PLAYER_END);
}

WoWPlayer::~WoWPlayer()
{
}

void WoWPlayer::OnValuesUpdated(const UpdateMask & Mask)
{
	if (Mask.GetBit(UNIT_FIELD_DISPLAYID))
	{
		uint32 displayID = m_Values.GetUInt32Value(UNIT_FIELD_DISPLAYID);

		if (m_UnitModel != nullptr)
		{
			//Log::Warn("WoWUnit: UNIT_FIELD_DISPLAYID updated, but Node already exists.");
			return;
		}

		CWorldObjectCreator creator(GetScene().GetBaseManager());
		m_UnitModel = creator.BuildEmptyCharacterFromDisplayInfo(GetScene().GetBaseManager().GetApplication().GetRenderDevice(), GetScene(), displayID);

		if (m_MountModel)
		{
			m_MountModel->AddChild(m_UnitModel);
		}
	}

	if (Mask.GetBit(UNIT_FIELD_BYTES_0))
	{
		if (auto characterModel = std::dynamic_pointer_cast<CCharacter>(m_UnitModel))
		{
			characterModel->Template().Race = (Race)GetRace();
			characterModel->Template().Class = (Class)GetClass();
			characterModel->Template().Gender = (Gender)GetGender();
		}
	}

	if (Mask.GetBit(PLAYER_BYTES))
	{
		if (auto characterModel = std::dynamic_pointer_cast<CCharacter>(m_UnitModel))
		{
			characterModel->Template().skin = GetSkinId();
			characterModel->Template().face = GetFaceId();
			characterModel->Template().hairStyle = GetHairStyleId();
			characterModel->Template().hairColor = GetHairColorId();
		}
	}

	if (Mask.GetBit(PLAYER_BYTES_2))
	{
		if (auto characterModel = std::dynamic_pointer_cast<CCharacter>(m_UnitModel))
		{
			characterModel->Template().facialStyle = GetFacialStyle();
		}
	}
	
	if (Mask.GetBit(PLAYER_BYTES_3))
	{
		auto characterModel = std::dynamic_pointer_cast<CCharacter>(m_UnitModel);
		if (auto characterModel = std::dynamic_pointer_cast<CCharacter>(m_UnitModel))
		{
			characterModel->Template().Gender = (Gender)GetNativeGender();
		}
	}

	for (uint16 i = PLAYER_VISIBLE_ITEM_1_ENTRYID; i < PLAYER_VISIBLE_ITEM_19_ENTRYID; i += 2)
	{
		if (Mask.GetBit(i))
		{
			if (auto characterModel = std::dynamic_pointer_cast<CCharacter>(m_UnitModel))
			{
				characterModel->SetItem((i - PLAYER_VISIBLE_ITEM_1_ENTRYID) / 2, GetItemDisplayInfoIDByItemID(m_Values.GetUInt32Value(i)));
			}
		}
	} 

	if (auto characterModel = std::dynamic_pointer_cast<CCharacter>(m_UnitModel))
	{
		// Items
		//characterModel->Refresh_CharacterItemsFromTemplate();

		// Textures
		//characterModel->Refresh_SkinImageFromTemplate();

		// Geosets
		//characterModel->RefreshMeshIDs();
	}

	if (Mask.GetBit(UNIT_FIELD_FLAGS))
	{
		uint32 flags = m_Values.GetUInt32Value(UNIT_FIELD_FLAGS);
		if (flags & UNIT_FLAG_MOUNT)
			OnMounted(m_Values.GetUInt32Value(UNIT_FIELD_MOUNTDISPLAYID));
		else
			OnDismounted();
	}

	//__super::OnValuesUpdated(Mask);
}



//
// Protected
//
std::shared_ptr<WoWPlayer> WoWPlayer::Create(CWoWWorld& WoWWorld, IScene& Scene, CWoWGuid Guid)
{
	std::shared_ptr<WoWPlayer> thisObj = MakeShared(WoWPlayer, Scene, WoWWorld, Guid);
	return thisObj;
}

void WoWPlayer::Destroy()
{}



#endif
