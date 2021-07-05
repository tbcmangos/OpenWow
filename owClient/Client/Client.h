#pragma once

#ifdef ENABLE_WOW_CLIENT

#include "../AuthSocket/AuthSocket.h"
#include "../WorldSocket/WorldSocket.h"

#include "../RealmSelection/RealmInfo.h"
#include "../CharacterSelection/CharacterSelection.h"
#include "../World/World.h"

class ZN_API CWoWClient 
	: public std::enable_shared_from_this<CWoWClient>
{
public:
	CWoWClient(IScene& Scene, const std::string& AuthServerHost, uint16 AuthServerPort = 3724);
	virtual ~CWoWClient();

	void Update(const UpdateEventArgs& e);
	void UpdateFromThread(std::future<void> PromiseExiter);

    // CWoWClient
	void BeginConnect(const std::string& Username, const std::string& Password);

	void OnRealListObtained(const std::vector<SRealmInfo>& Realms, BigNumber Key);
	void OnRealmListSelected(const SRealmInfo& SelectedRealm, BigNumber Key);
	
	void OnCharactersListObtained(const std::vector<SCharacterTemplate>& Characters);
	void OnCharacterSelected(const SCharacterTemplate& SelectedCharacter);


private:
	std::string                                   m_Host;
	uint16                                        m_Port;
	std::string                                   m_Login;

private:
	std::thread                                   m_UpdateThread;
	std::promise<void>					          m_UpdateThreadExiter;

	std::unique_ptr<CAuthSocket>                  m_AuthSocket;
	std::shared_ptr<CWorldSocket>                 m_WorldSocket;

	IScene&                                       m_Scene;
	std::unique_ptr<CWoWClientCharactedSelection> m_CharacterSelection;
	std::unique_ptr<CWoWWorld>                    m_World;
};

#endif