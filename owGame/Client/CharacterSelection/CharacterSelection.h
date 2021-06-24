#pragma once

#include "WorldSocket.h"
#include "Opcodes.h"

// FORWARD BEGIN
class CWoWClient;
// FORWARD END

class CWoWClientCharactedSelection
{
public:
	CWoWClientCharactedSelection(CWoWClient& WoWClient, IScene& Scene);
	virtual ~CWoWClientCharactedSelection();

	void On_SMSG_CHAR_ENUM(CServerPacket& Bytes);

	void AddHandler(Opcodes Opcode, std::function<void(CServerPacket&)> Handler);
	bool ProcessHandler(Opcodes Opcode, CServerPacket& Bytes);

private:
	CWoWClient& m_WoWClient;
	IScene& m_Scene;
	std::unordered_map<Opcodes, std::function<void(CServerPacket&)>> m_Handlers;
};