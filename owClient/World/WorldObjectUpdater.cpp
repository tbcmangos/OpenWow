#include "stdafx.h"

// General
#include "WorldObjectUpdater.h"

// Additional
#include "WorldServer.h"

// Additional (zlib)
#include "zlib/source/zlib.h"
#pragma comment(lib, "zlib.lib")


namespace
{
	enum class OBJECT_UPDATE_TYPE : uint8
	{
		UPDATETYPE_VALUES = 0,
		UPDATETYPE_MOVEMENT = 1,
		UPDATETYPE_CREATE_OBJECT = 2,
		UPDATETYPE_CREATE_OBJECT2 = 3, // New object
		UPDATETYPE_OUT_OF_RANGE_OBJECTS = 4,
		UPDATETYPE_NEAR_OBJECTS = 5
	};
}


CWorldObjectUpdater::CWorldObjectUpdater(CWorldServer& WoWWorld, IScene & Scene)
	: m_Scene(Scene)
	, m_ServerWorld(WoWWorld)
{
	m_ServerWorld.AddHandler(SMSG_COMPRESSED_UPDATE_OBJECT, std::bind(&CWorldObjectUpdater::On_SMSG_COMPRESSED_UPDATE_OBJECT, this, std::placeholders::_1));
	m_ServerWorld.AddHandler(SMSG_UPDATE_OBJECT, std::bind(&CWorldObjectUpdater::On_SMSG_UPDATE_OBJECT, this, std::placeholders::_1));
}

CWorldObjectUpdater::~CWorldObjectUpdater()
{}



void CWorldObjectUpdater::On_SMSG_COMPRESSED_UPDATE_OBJECT(CServerPacket& Buffer)
{
	uint32 dataSize;
	Buffer >> dataSize;

	CServerPacket uncompressed(dataSize,  Buffer.GetPacketOpcode());
	uncompressed.writeDummy(dataSize);
	uncompressed.seek(0);

	uLongf uncompressedSize = dataSize;
	if (::uncompress(uncompressed.getDataEx(), &uncompressedSize, Buffer.getDataFromCurrent(), Buffer.getSize() - 4) != Z_OK)
		throw CException("CWorldObjectUpdater::On_SMSG_COMPRESSED_UPDATE_OBJECT: Error while ::uncompress data.");

	Buffer.seekRelative(Buffer.getSize() - 4);

	if (dataSize != uncompressedSize)
		throw CException("CWorldObjectUpdater::On_SMSG_COMPRESSED_UPDATE_OBJECT: Uncompressed data size (%d bytes) don't match expected data size (%d bytes).", uncompressedSize, dataSize);

	ProcessUpdatePacket(uncompressed);
}

void CWorldObjectUpdater::On_SMSG_UPDATE_OBJECT(CServerPacket& Buffer)
{
	ProcessUpdatePacket(Buffer);
}



//
// Private
//
void CWorldObjectUpdater::ProcessUpdatePacket(CServerPacket& Bytes)
{
	uint32 blocksCount;
	Bytes >> blocksCount;

	for (uint32 i = 0u; i < blocksCount; i++)
	{
		OBJECT_UPDATE_TYPE updateType;
		Bytes >> updateType;

		switch (updateType)
		{
			case OBJECT_UPDATE_TYPE::UPDATETYPE_VALUES:
			{
				CowGuid guid;
				Bytes.ReadPackedGuid(&guid);

				if (false == m_ServerWorld.GetWorldObjects().IsExists(guid))
					throw CException("Object not exists");

				auto object = m_ServerWorld.GetWorldObjects().Get(guid);
				object->Do_UPDATETYPE_VALUES(Bytes);
			}
			break;


			case OBJECT_UPDATE_TYPE::UPDATETYPE_MOVEMENT:
			{
				CowGuid guid;
				Bytes.ReadPackedGuid(&guid);

				if (false == m_ServerWorld.GetWorldObjects().IsExists(guid))
					throw CException("Object not exists");

				auto object = m_ServerWorld.GetWorldObjects().Get(guid);
				object->Do_UPDATETYPE_MOVEMENT(Bytes);
			}
			break;


			case OBJECT_UPDATE_TYPE::UPDATETYPE_CREATE_OBJECT:
			case OBJECT_UPDATE_TYPE::UPDATETYPE_CREATE_OBJECT2: // isNewObject
			{
				CowGuid guid;
				Bytes.ReadPackedGuid(&guid);

				EWoWObjectTypeID typeID; // Different from GUID TypeID for Container TODO
				Bytes >> typeID;

				auto object = m_ServerWorld.GetWorldObjects().Create(guid, typeID);

				object->Do_UPDATETYPE_MOVEMENT(Bytes);
				object->Do_UPDATETYPE_VALUES(Bytes);
			}
			break;


			case OBJECT_UPDATE_TYPE::UPDATETYPE_OUT_OF_RANGE_OBJECTS:
			{
				uint32 outOfRangeGUIDsCount;
				Bytes >> outOfRangeGUIDsCount;

				std::vector<CowGuid> outOfRangeGUIDs;
				outOfRangeGUIDs.resize(outOfRangeGUIDsCount);

				for (uint32 i = 0; i < outOfRangeGUIDsCount; i++)
					Bytes.ReadPackedGuid(&outOfRangeGUIDs[i]);

				auto& worldObjects = m_ServerWorld.GetWorldObjects();
				for (const auto& outOfRangeGUID : outOfRangeGUIDs)
				{
					if (false == m_ServerWorld.GetWorldObjects().IsExists(outOfRangeGUID))
						throw CException("Object doesn't exists.");

					if (auto object = worldObjects.Get(outOfRangeGUID))
					{
						object->Destroy();
						worldObjects.Delete(object);
					}
				}
			}
			break;

			case OBJECT_UPDATE_TYPE::UPDATETYPE_NEAR_OBJECTS:
				throw CException("Not implemented.");


			default:
				throw CException("CWorldObjectUpdater::ProcessUpdatePacket: Unknown update type '%d.'", updateType);
		}
	}
}