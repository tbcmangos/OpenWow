#pragma once

#ifdef ENABLE_WOW_CLIENT

#include "WoWWorldObject.h"

// FORWARD BEGIN
class CWoWWorld;
// FORWARD END

class ZN_API WoWCorpse
	: public CWoWWorldObject
{
public:
	WoWCorpse(IScene& Scene, CWoWWorld& WoWWorld, CWoWGuid Guid);
	virtual ~WoWCorpse();

public:
	static std::shared_ptr<WoWCorpse> Create(CWoWWorld& WoWWorld, IScene& Scene, CWoWGuid Guid);

private:

};

#endif