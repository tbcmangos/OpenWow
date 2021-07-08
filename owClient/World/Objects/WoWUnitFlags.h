#pragma once

enum MovementFlags : uint32
{
	MOVEMENTFLAG_NONE = 0x00000000,
	MOVEMENTFLAG_FORWARD = 0x00000001,
	MOVEMENTFLAG_BACKWARD = 0x00000002,
	MOVEMENTFLAG_STRAFE_LEFT = 0x00000004,
	MOVEMENTFLAG_STRAFE_RIGHT = 0x00000008,
	MOVEMENTFLAG_LEFT = 0x00000010,
	MOVEMENTFLAG_RIGHT = 0x00000020,
	MOVEMENTFLAG_PITCH_UP = 0x00000040,
	MOVEMENTFLAG_PITCH_DOWN = 0x00000080,
	MOVEMENTFLAG_WALKING = 0x00000100,               // Walking
	MOVEMENTFLAG_ONTRANSPORT = 0x00000200,               // Used for flying on some creatures
	MOVEMENTFLAG_DISABLE_GRAVITY = 0x00000400,               // Former MOVEMENTFLAG_LEVITATING. This is used when walking is not possible.
	MOVEMENTFLAG_ROOT = 0x00000800,               // Must not be set along with MOVEMENTFLAG_MASK_MOVING
	MOVEMENTFLAG_FALLING = 0x00001000,               // damage dealt on that type of falling
	MOVEMENTFLAG_FALLING_FAR = 0x00002000,
	MOVEMENTFLAG_PENDING_STOP = 0x00004000,
	MOVEMENTFLAG_PENDING_STRAFE_STOP = 0x00008000,
	MOVEMENTFLAG_PENDING_FORWARD = 0x00010000,
	MOVEMENTFLAG_PENDING_BACKWARD = 0x00020000,
	MOVEMENTFLAG_PENDING_STRAFE_LEFT = 0x00040000,
	MOVEMENTFLAG_PENDING_STRAFE_RIGHT = 0x00080000,
	MOVEMENTFLAG_PENDING_ROOT = 0x00100000,
	MOVEMENTFLAG_SWIMMING = 0x00200000,               // appears with fly flag also
	MOVEMENTFLAG_ASCENDING = 0x00400000,               // press "space" when flying
	MOVEMENTFLAG_DESCENDING = 0x00800000,
	MOVEMENTFLAG_CAN_FLY = 0x01000000,               // Appears when unit can fly AND also walk
	MOVEMENTFLAG_FLYING = 0x02000000,               // unit is actually flying. pretty sure this is only used for players. creatures use disable_gravity
	MOVEMENTFLAG_SPLINE_ELEVATION = 0x04000000,               // used for flight paths
	MOVEMENTFLAG_SPLINE_ENABLED = 0x08000000,               // used for flight paths
	MOVEMENTFLAG_WATERWALKING = 0x10000000,               // prevent unit from falling through water
	MOVEMENTFLAG_FALLING_SLOW = 0x20000000,               // active rogue safe fall spell (passive)
	MOVEMENTFLAG_HOVER = 0x40000000,               // hover, cannot jump

	MOVEMENTFLAG_MASK_MOVING = MOVEMENTFLAG_FORWARD | MOVEMENTFLAG_BACKWARD | MOVEMENTFLAG_STRAFE_LEFT | MOVEMENTFLAG_STRAFE_RIGHT | MOVEMENTFLAG_FALLING | MOVEMENTFLAG_FALLING_FAR | MOVEMENTFLAG_ASCENDING | MOVEMENTFLAG_DESCENDING | MOVEMENTFLAG_SPLINE_ELEVATION,

	MOVEMENTFLAG_MASK_TURNING = MOVEMENTFLAG_LEFT | MOVEMENTFLAG_RIGHT | MOVEMENTFLAG_PITCH_UP | MOVEMENTFLAG_PITCH_DOWN,

	MOVEMENTFLAG_MASK_MOVING_FLY = MOVEMENTFLAG_FLYING | MOVEMENTFLAG_ASCENDING | MOVEMENTFLAG_DESCENDING,

	/// @todo if needed: add more flags to this masks that are exclusive to players
	MOVEMENTFLAG_MASK_PLAYER_ONLY = MOVEMENTFLAG_FLYING,

	/// Movement flags that have change status opcodes associated for players
	MOVEMENTFLAG_MASK_HAS_PLAYER_STATUS_OPCODE = MOVEMENTFLAG_DISABLE_GRAVITY | MOVEMENTFLAG_ROOT | MOVEMENTFLAG_CAN_FLY | MOVEMENTFLAG_WATERWALKING | MOVEMENTFLAG_FALLING_SLOW | MOVEMENTFLAG_HOVER
};

enum MovementFlags2 : uint32
{
	MOVEMENTFLAG2_NONE = 0x00000000,
	MOVEMENTFLAG2_NO_STRAFE = 0x00000001,
	MOVEMENTFLAG2_NO_JUMPING = 0x00000002,
	MOVEMENTFLAG2_UNK3 = 0x00000004,        // Overrides various clientside checks
	MOVEMENTFLAG2_FULL_SPEED_TURNING = 0x00000008,
	MOVEMENTFLAG2_FULL_SPEED_PITCHING = 0x00000010,
	MOVEMENTFLAG2_ALWAYS_ALLOW_PITCHING = 0x00000020,
	MOVEMENTFLAG2_UNK7 = 0x00000040,
	MOVEMENTFLAG2_UNK8 = 0x00000080,
	MOVEMENTFLAG2_UNK9 = 0x00000100,
	MOVEMENTFLAG2_UNK10 = 0x00000200,
	MOVEMENTFLAG2_INTERPOLATED_MOVEMENT = 0x00000400,
	MOVEMENTFLAG2_INTERPOLATED_TURNING = 0x00000800,
	MOVEMENTFLAG2_INTERPOLATED_PITCHING = 0x00001000,
	MOVEMENTFLAG2_UNK14 = 0x00002000,
	MOVEMENTFLAG2_UNK15 = 0x00004000,
	MOVEMENTFLAG2_UNK16 = 0x00008000
};

// Speed
enum UnitSpeedType
{
	MOVE_WALK = 0,
	MOVE_RUN = 1,
	MOVE_RUN_BACK = 2,
	MOVE_SWIM = 3,
	MOVE_SWIM_BACK = 4,
	MOVE_TURN_RATE = 5,
	MOVE_FLIGHT = 6,
	MOVE_FLIGHT_BACK = 7,
	MOVE_PITCH_RATE = 8
};

// Value masks for UNIT_FIELD_FLAGS
enum UnitFlags : uint32
{
	UNIT_FLAG_SERVER_CONTROLLED = 0x00000001,           // set only when unit movement is controlled by server - by SPLINE/MONSTER_MOVE packets, together with UNIT_FLAG_STUNNED; only set to units controlled by client; client function CGUnit_C::IsClientControlled returns false when set for owner
	UNIT_FLAG_NON_ATTACKABLE = 0x00000002,           // not attackable
	UNIT_FLAG_REMOVE_CLIENT_CONTROL = 0x00000004,           // This is a legacy flag used to disable movement player's movement while controlling other units, SMSG_CLIENT_CONTROL replaces this functionality clientside now. CONFUSED and FLEEING flags have the same effect on client movement asDISABLE_MOVE_CONTROL in addition to preventing spell casts/autoattack (they all allow climbing steeper hills and emotes while moving)
	UNIT_FLAG_PLAYER_CONTROLLED = 0x00000008,           // controlled by player, use _IMMUNE_TO_PC instead of _IMMUNE_TO_NPC
	UNIT_FLAG_RENAME = 0x00000010,
	UNIT_FLAG_PREPARATION = 0x00000020,           // don't take reagents for spells with SPELL_ATTR5_NO_REAGENT_WHILE_PREP
	UNIT_FLAG_UNK_6 = 0x00000040,
	UNIT_FLAG_NOT_ATTACKABLE_1 = 0x00000080,           // ?? (UNIT_FLAG_PLAYER_CONTROLLED | UNIT_FLAG_NOT_ATTACKABLE_1) is NON_PVP_ATTACKABLE
	UNIT_FLAG_IMMUNE_TO_PC = 0x00000100,           // disables combat/assistance with PlayerCharacters (PC) - see Unit::IsValidAttackTarget, Unit::IsValidAssistTarget
	UNIT_FLAG_IMMUNE_TO_NPC = 0x00000200,           // disables combat/assistance with NonPlayerCharacters (NPC) - see Unit::IsValidAttackTarget, Unit::IsValidAssistTarget
	UNIT_FLAG_LOOTING = 0x00000400,           // loot animation
	UNIT_FLAG_PET_IN_COMBAT = 0x00000800,           // on player pets: whether the pet is chasing a target to attack || on other units: whether any of the unit's minions is in combat
	UNIT_FLAG_PVP = 0x00001000,           // changed in 3.0.3
	UNIT_FLAG_SILENCED = 0x00002000,           // silenced, 2.1.1
	UNIT_FLAG_CANNOT_SWIM = 0x00004000,           // 2.0.8
	UNIT_FLAG_SWIMMING = 0x00008000,           // shows swim animation in water
	UNIT_FLAG_NON_ATTACKABLE_2 = 0x00010000,           // removes attackable icon, if on yourself, cannot assist self but can cast TARGET_SELF spells - added by SPELL_AURA_MOD_UNATTACKABLE
	UNIT_FLAG_PACIFIED = 0x00020000,           // 3.0.3 ok
	UNIT_FLAG_STUNNED = 0x00040000,           // 3.0.3 ok
	UNIT_FLAG_IN_COMBAT = 0x00080000,
	UNIT_FLAG_TAXI_FLIGHT = 0x00100000,           // disable casting at client side spell not allowed by taxi flight (mounted?), probably used with 0x4 flag
	UNIT_FLAG_DISARMED = 0x00200000,           // 3.0.3, disable melee spells casting..., "Required melee weapon" added to melee spells tooltip.
	UNIT_FLAG_CONFUSED = 0x00400000,
	UNIT_FLAG_FLEEING = 0x00800000,
	UNIT_FLAG_POSSESSED = 0x01000000,           // under direct client control by a player (possess or vehicle)
	UNIT_FLAG_NOT_SELECTABLE = 0x02000000,
	UNIT_FLAG_SKINNABLE = 0x04000000,
	UNIT_FLAG_MOUNT = 0x08000000,
	UNIT_FLAG_UNK_28 = 0x10000000,
	UNIT_FLAG_UNK_29 = 0x20000000,           // used in Feing Death spell
	UNIT_FLAG_SHEATHE = 0x40000000,
	UNIT_FLAG_IMMUNE = 0x80000000,           // Immune to damage
};

// Value masks for UNIT_FIELD_FLAGS_2
enum UnitFlags2 : uint32
{
	UNIT_FLAG2_FEIGN_DEATH = 0x00000001,
	UNIT_FLAG2_UNK1 = 0x00000002,   // Hide unit model (show only player equip)
	UNIT_FLAG2_IGNORE_REPUTATION = 0x00000004,
	UNIT_FLAG2_COMPREHEND_LANG = 0x00000008,
	UNIT_FLAG2_MIRROR_IMAGE = 0x00000010,
	UNIT_FLAG2_INSTANTLY_APPEAR_MODEL = 0x00000020,   // Unit model instantly appears when summoned (does not fade in)
	UNIT_FLAG2_FORCE_MOVEMENT = 0x00000040,
	UNIT_FLAG2_DISARM_OFFHAND = 0x00000080,
	UNIT_FLAG2_DISABLE_PRED_STATS = 0x00000100,   // Player has disabled predicted stats (Used by raid frames)
	UNIT_FLAG2_DISARM_RANGED = 0x00000400,   // this does not disable ranged weapon display (maybe additional flag needed?)
	UNIT_FLAG2_REGENERATE_POWER = 0x00000800,
	UNIT_FLAG2_RESTRICT_PARTY_INTERACTION = 0x00001000,   // Restrict interaction to party or raid
	UNIT_FLAG2_PREVENT_SPELL_CLICK = 0x00002000,   // Prevent spellclick
	UNIT_FLAG2_ALLOW_ENEMY_INTERACT = 0x00004000,
	UNIT_FLAG2_DISABLE_TURN = 0x00008000,
	UNIT_FLAG2_UNK2 = 0x00010000,
	UNIT_FLAG2_PLAY_DEATH_ANIM = 0x00020000,   // Plays special death animation upon death
	UNIT_FLAG2_ALLOW_CHEAT_SPELLS = 0x00040000    // Allows casting spells with AttributesEx7 & SPELL_ATTR7_IS_CHEAT_SPELL
};

// Value masks for UNIT_DYNAMIC_FLAGS
enum UnitDynFlags
{
	UNIT_DYNFLAG_NONE = 0x0000,
	UNIT_DYNFLAG_LOOTABLE = 0x0001,
	UNIT_DYNFLAG_TRACK_UNIT = 0x0002,
	UNIT_DYNFLAG_TAPPED = 0x0004,       // Lua_UnitIsTapped
	UNIT_DYNFLAG_TAPPED_BY_PLAYER = 0x0008,       // Lua_UnitIsTappedByPlayer
	UNIT_DYNFLAG_SPECIALINFO = 0x0010,
	UNIT_DYNFLAG_DEAD = 0x0020,
	UNIT_DYNFLAG_REFER_A_FRIEND = 0x0040,
	UNIT_DYNFLAG_TAPPED_BY_ALL_THREAT_LIST = 0x0080        // Lua_UnitIsTappedByAllThreatList
};


// byte value (UNIT_BYTES_1_OFFSET_STAND_STATE)
enum UnitStandStateType : uint8
{
	UNIT_STAND_STATE_STAND = 0,
	UNIT_STAND_STATE_SIT = 1,
	UNIT_STAND_STATE_SIT_CHAIR = 2,
	UNIT_STAND_STATE_SLEEP = 3,
	UNIT_STAND_STATE_SIT_LOW_CHAIR = 4,
	UNIT_STAND_STATE_SIT_MEDIUM_CHAIR = 5,
	UNIT_STAND_STATE_SIT_HIGH_CHAIR = 6,
	UNIT_STAND_STATE_DEAD = 7,
	UNIT_STAND_STATE_KNEEL = 8,
	UNIT_STAND_STATE_SUBMERGED = 9
};

// byte flag value (UNIT_BYTES_1_OFFSET_VIS_FLAG)
enum UnitStandFlags : uint8
{
	UNIT_STAND_FLAGS_UNK1 = 0x01,
	UNIT_STAND_FLAGS_CREEP = 0x02,
	UNIT_STAND_FLAGS_UNTRACKABLE = 0x04,
	UNIT_STAND_FLAGS_UNK4 = 0x08,
	UNIT_STAND_FLAGS_UNK5 = 0x10,
	UNIT_STAND_FLAGS_ALL = 0xFF
};

// byte flag value (UNIT_BYTES_2_OFFSET_SHEATH_STATE)
#define MAX_SHEATH_STATE 3
enum UnitSheathState : uint8
{
	SHEATH_STATE_UNARMED = 0, // non prepared weapon
	SHEATH_STATE_MELEE = 1, // prepared melee weapon
	SHEATH_STATE_RANGED = 2 // prepared ranged weapon
};

// byte flag value (UNIT_BYTES_2_OFFSET_SHEATH_STATE)
enum UnitPVPFlags : uint8
{
	UNIT_BYTE2_FLAG_PVP = 0x01,
	UNIT_BYTE2_FLAG_UNK1 = 0x02,
	UNIT_BYTE2_FLAG_FFA_PVP = 0x04,
	UNIT_BYTE2_FLAG_SANCTUARY = 0x08,
	UNIT_BYTE2_FLAG_UNK4 = 0x10,
	UNIT_BYTE2_FLAG_UNK5 = 0x20,
	UNIT_BYTE2_FLAG_UNK6 = 0x40,
	UNIT_BYTE2_FLAG_UNK7 = 0x80
};

// byte flag value (UNIT_BYTES_2_OFFSET_PET_FLAGS)
enum UnitPetFlags : uint8
{
	UNIT_CAN_BE_RENAMED = 0x01,
	UNIT_CAN_BE_ABANDONED = 0x02
};


// FLAGS FOR VALUES
enum UnitBytes0Offsets : uint8
{
	UNIT_BYTES_0_OFFSET_RACE = 0,
	UNIT_BYTES_0_OFFSET_CLASS = 1,
	UNIT_BYTES_0_OFFSET_GENDER = 2,
	UNIT_BYTES_0_OFFSET_POWER_TYPE = 3,
};

enum UnitBytes1Offsets : uint8
{
	UNIT_BYTES_1_OFFSET_STAND_STATE = 0,
	UNIT_BYTES_1_OFFSET_PET_TALENTS = 1,
	UNIT_BYTES_1_OFFSET_VIS_FLAG = 2,
	UNIT_BYTES_1_OFFSET_ANIM_TIER = 3
};

enum UnitBytes2Offsets : uint8
{
	UNIT_BYTES_2_OFFSET_SHEATH_STATE = 0,
	UNIT_BYTES_2_OFFSET_PVP_FLAG = 1,
	UNIT_BYTES_2_OFFSET_PET_FLAGS = 2,
	UNIT_BYTES_2_OFFSET_SHAPESHIFT_FORM = 3
};

// UNIT_NPC_FLAGS
enum NPCFlags : uint32
{
	UNIT_NPC_FLAG_NONE = 0x00000000,       // SKIP
	UNIT_NPC_FLAG_GOSSIP = 0x00000001,       // TITLE has gossip menu DESCRIPTION 100%
	UNIT_NPC_FLAG_QUESTGIVER = 0x00000002,       // TITLE is quest giver DESCRIPTION guessed, probably ok
	UNIT_NPC_FLAG_UNK1 = 0x00000004,
	UNIT_NPC_FLAG_UNK2 = 0x00000008,
	UNIT_NPC_FLAG_TRAINER = 0x00000010,       // TITLE is trainer DESCRIPTION 100%
	UNIT_NPC_FLAG_TRAINER_CLASS = 0x00000020,       // TITLE is class trainer DESCRIPTION 100%
	UNIT_NPC_FLAG_TRAINER_PROFESSION = 0x00000040,       // TITLE is profession trainer DESCRIPTION 100%
	UNIT_NPC_FLAG_VENDOR = 0x00000080,       // TITLE is vendor (generic) DESCRIPTION 100%
	UNIT_NPC_FLAG_VENDOR_AMMO = 0x00000100,       // TITLE is vendor (ammo) DESCRIPTION 100%, general goods vendor
	UNIT_NPC_FLAG_VENDOR_FOOD = 0x00000200,       // TITLE is vendor (food) DESCRIPTION 100%
	UNIT_NPC_FLAG_VENDOR_POISON = 0x00000400,       // TITLE is vendor (poison) DESCRIPTION guessed
	UNIT_NPC_FLAG_VENDOR_REAGENT = 0x00000800,       // TITLE is vendor (reagents) DESCRIPTION 100%
	UNIT_NPC_FLAG_REPAIR = 0x00001000,       // TITLE can repair DESCRIPTION 100%
	UNIT_NPC_FLAG_FLIGHTMASTER = 0x00002000,       // TITLE is flight master DESCRIPTION 100%
	UNIT_NPC_FLAG_SPIRITHEALER = 0x00004000,       // TITLE is spirit healer DESCRIPTION guessed
	UNIT_NPC_FLAG_SPIRITGUIDE = 0x00008000,       // TITLE is spirit guide DESCRIPTION guessed
	UNIT_NPC_FLAG_INNKEEPER = 0x00010000,       // TITLE is innkeeper
	UNIT_NPC_FLAG_BANKER = 0x00020000,       // TITLE is banker DESCRIPTION 100%
	UNIT_NPC_FLAG_PETITIONER = 0x00040000,       // TITLE handles guild/arena petitions DESCRIPTION 100% 0xC0000 = guild petitions, 0x40000 = arena team petitions
	UNIT_NPC_FLAG_TABARDDESIGNER = 0x00080000,       // TITLE is guild tabard designer DESCRIPTION 100%
	UNIT_NPC_FLAG_BATTLEMASTER = 0x00100000,       // TITLE is battlemaster DESCRIPTION 100%
	UNIT_NPC_FLAG_AUCTIONEER = 0x00200000,       // TITLE is auctioneer DESCRIPTION 100%
	UNIT_NPC_FLAG_STABLEMASTER = 0x00400000,       // TITLE is stable master DESCRIPTION 100%
	UNIT_NPC_FLAG_GUILD_BANKER = 0x00800000,       // TITLE is guild banker DESCRIPTION cause client to send 997 opcode
	UNIT_NPC_FLAG_SPELLCLICK = 0x01000000,       // TITLE has spell click enabled DESCRIPTION cause client to send 1015 opcode (spell click)
	UNIT_NPC_FLAG_PLAYER_VEHICLE = 0x02000000,       // TITLE is player vehicle DESCRIPTION players with mounts that have vehicle data should have it set
	UNIT_NPC_FLAG_MAILBOX = 0x04000000        // TITLE is mailbox
};