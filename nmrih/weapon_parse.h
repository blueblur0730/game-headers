//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Weapon data file parsing, shared by game & client dlls.
//
// $NoKeywords: $
//=============================================================================//

#include "shareddefs.h"
#include "tier1/utldict.h"

class IFileSystem;

typedef unsigned short WEAPON_FILE_INFO_HANDLE;

// -----------------------------------------------------------
// Weapon sound types
// Used to play sounds defined in the weapon's classname.txt file
// This needs to match pWeaponSoundCategories in weapon_parse.cpp
// ------------------------------------------------------------
typedef enum {
	EMPTY,
	SINGLE,
	SINGLE_NPC,
	WPN_DOUBLE, // Can't be "DOUBLE" because windows.h uses it.
	DOUBLE_NPC,
	BURST,
	RELOAD,
	RELOAD_NPC,
	MELEE_MISS,
	MELEE_HIT,
	MELEE_HIT_WORLD,
	SPECIAL1,
	SPECIAL2,
	SPECIAL3,
	TAUNT,
	DEPLOY,

	// Add new shoot sound types here

	NUM_SHOOT_SOUND_TYPES,
} WeaponSound_t;

int GetWeaponSoundFromString( const char *pszString );

#define MAX_SHOOT_SOUNDS	16			// Maximum number of shoot sounds per shoot type

#define MAX_WEAPON_STRING	80
#define MAX_WEAPON_PREFIX	16
#define MAX_WEAPON_AMMO_NAME		32

#define WEAPON_PRINTNAME_MISSING "!!! Missing printname on weapon"

class CHudTexture;
class KeyValues;

// Weapon flags, from shareddefs.h
// -----------------------------------------
//	Flags - NOTE: KEEP g_ItemFlags IN WEAPON_PARSE.CPP UPDATED WITH THESE
// -----------------------------------------
#define ITEM_FLAG_SELECTONEMPTY		(1<<0)
#define ITEM_FLAG_NOAUTORELOAD		(1<<1)
#define ITEM_FLAG_LIMITINWORLD		(1<<3)
#define ITEM_FLAG_EXHAUSTIBLE		(1<<4)	// A player can totally exhaust their ammo supply and lose this weapon
#define ITEM_FLAG_DOHITLOCATIONDMG	(1<<5)	// This weapon take hit location into account when applying damage
#define ITEM_FLAG_NOAMMOPICKUPS		(1<<6)	// Don't draw ammo pickup sprites/sounds when ammo is received
#define ITEM_FLAG_NOITEMPICKUP		(1<<7)	// Don't draw weapon pickup when this weapon is picked up by the player
#define ITEM_FLAG_SKILLSHOT			(1<<8)	// This weapon can perform a skillshot, charginf when aim downsight.
#define ITEM_FLAG_PENETRATION		(1<<9)	// This weapon can penetrate multiple enemies at once.
// NOTE: KEEP g_ItemFlags IN WEAPON_PARSE.CPP UPDATED WITH THESE

// Item flags that we parse out of the file.
typedef struct
{
	const char *m_pFlagName;
	int m_iFlagValue;
} itemFlags_t;
#if !defined(_STATIC_LINKED) || defined(CLIENT_DLL)
itemFlags_t g_ItemFlags[8] =
{
	{ "ITEM_FLAG_SELECTONEMPTY",	ITEM_FLAG_SELECTONEMPTY },
	{ "ITEM_FLAG_NOAUTORELOAD",		ITEM_FLAG_NOAUTORELOAD },
	{ "ITEM_FLAG_LIMITINWORLD",		ITEM_FLAG_LIMITINWORLD },
	{ "ITEM_FLAG_EXHAUSTIBLE",		ITEM_FLAG_EXHAUSTIBLE },
	{ "ITEM_FLAG_DOHITLOCATIONDMG", ITEM_FLAG_DOHITLOCATIONDMG },
	{ "ITEM_FLAG_NOAMMOPICKUPS",	ITEM_FLAG_NOAMMOPICKUPS },
	{ "ITEM_FLAG_NOITEMPICKUP",		ITEM_FLAG_NOITEMPICKUP },
	{ "ITEM_FLAG_SKILLSHOT",		ITEM_FLAG_SKILLSHOT },
	{ "ITEM_FLAG_PENETRATION",		ITEM_FLAG_PENETRATION }
};
#else
extern itemFlags_t g_ItemFlags[8];
#endif

// The sound categories found in the weapon classname.txt files
// This needs to match the WeaponSound_t enum in weapon_parse.h
#if !defined(_STATIC_LINKED) || defined(CLIENT_DLL)
const char *pWeaponSoundCategories[ NUM_SHOOT_SOUND_TYPES ] = 
{
	"empty",
	"single_shot",
	"single_shot_npc",
	"double_shot",
	"double_shot_npc",
	"burst",
	"reload",
	"reload_npc",
	"melee_miss",
	"melee_hit",
	"melee_hit_world",
	"special1",
	"special2",
	"special3",
	"taunt",
	"deploy"
};
#else
extern const char *pWeaponSoundCategories[ NUM_SHOOT_SOUND_TYPES ];
#endif

static CUtlDict< FileWeaponInfo_t*, unsigned short > m_WeaponInfoDatabase;

// FIXME, handle differently?
static FileWeaponInfo_t gNullWeaponInfo;

// size 1864.
//-----------------------------------------------------------------------------
// Purpose: Contains the data read from the weapon's script file. 
// It's cached so we only read each weapon's script file once.
// Each game provides a CreateWeaponInfo function so it can have game-specific
// data (like CS move speeds) in the weapon script.
//-----------------------------------------------------------------------------
class FileWeaponInfo_t
{
public:
	FileWeaponInfo_t();
	
	// Each game can override this to get whatever values it wants from the script.
	virtual void Parse( KeyValues *pKeyValuesData, const char *szWeaponName );

	
public:	
	bool					bParsedScript;					 		// this + 0x4 / 4.
	bool					bLoadedHudElements;		 		 		// this + 0x5 / 5.

// SHARED
	char					szClassName[MAX_WEAPON_STRING];			// this + 0x6 / 6.
	char					szPrintName[MAX_WEAPON_STRING];			// this + 0x56 / 86, Name for showing in HUD, etc.
	char					szViewModel[MAX_WEAPON_STRING];			// this + 0xA6 / 166, View model of this weapon.
	char					szWorldModel[MAX_WEAPON_STRING];		// this + 0xF6 / 246, Model of this weapon seen carried by the player.
	char					szNPCModel[MAX_WEAPON_STRING];			// this + 0x146 / 326, Model of this npc.
	char					szAnimationPrefix[MAX_WEAPON_PREFIX];	// this + 0x196 / 406, Prefix of the animations that should be used by the player carrying this weapon.
	int						iSlot;									// this + 0x1A8 / 424, inventory slot.
	int						iPosition;								// this + 0x1AC / 428, position in the inventory slot.
	int						iMaxClip1;								// this + 0x1B0 / 432, max primary clip size (-1 if no clip).
	int						iMaxClip2;								// this + 0x1B4 / 436, max secondary clip size (-1 if no clip).
	int						iDefaultClip1;							// this + 0x1B8 / 440, amount of primary ammo in the gun when it's created.
	int						iDefaultClip2;							// this + 0x1BC / 444, amount of secondary ammo in the gun when it's created.
	int						iWeight;								// this + 0x1C0 / 448, this value used to determine this weapon's importance in autoselection.
	int						iMaxThrowForce;							// this + 0x1C4 / 452, max throw force of the weapon. used by bow and melees.
	int						iRumbleEffect;							// this + 0x1C8 / 456, Which rumble effect to use when fired? (xbox).
	bool					bAutoSwitchTo;							// this + 0x1CC / 460, whether this weapon should be considered for autoswitching to.
	bool					bAutoSwitchFrom;						// this + 0x1CD / 461, whether this weapon can be autoswitched away from when picking up another weapon or ammo
	int						iFlags;									// this + 0x1D0 / 464, miscellaneous weapon flags.
	char					szAmmo1[MAX_WEAPON_AMMO_NAME];			// this + 0x1D4 / 468, "primary" ammo type.
	char					szAmmo2[MAX_WEAPON_AMMO_NAME];			// this + 0x1F4 / 500, "secondary" ammo type.
	
	// Sound blocks
	char					aShootSounds[NUM_SHOOT_SOUND_TYPES][MAX_WEAPON_STRING];			// this + 0x214 / 532, array of shoot sound names.

	int						iAmmoType;			// this + 0x714 / 1812. The ammo index into ammo definitions.
	int						iAmmo2Type;			// this + 0x718 / 1816. The ammo index into ammo definitions.
	bool					m_bMeleeWeapon;		// this + 0x71C / 1820. Melee weapons can always "fire" regardless of ammo.

	// This tells if the weapon was built right-handed (defaults to true).
	// This helps cl_righthand make the decision about whether to flip the model or not.
	bool					m_bBuiltRightHanded;// this + 0x71D / 1821.
	bool					m_bAllowFlipping;	// this + 0x71E / 1822. False to disallow flipping the model, regardless of whether
												// it is built left or right handed.

// CLIENT DLL
	// Sprite data, read from the data file
	int						iSpriteCount;							// this + 0x720 / 1824.
	CHudTexture						*iconActive;					// this + 0x724 / 1828.
	CHudTexture	 					*iconInactive;					// this + 0x728 / 1832.
	CHudTexture 					*iconAmmo;						// this + 0x72C / 1836.
	CHudTexture 					*iconAmmo2;						// this + 0x730 / 1840.
	CHudTexture 					*iconCrosshair;					// this + 0x734 / 1844.
	CHudTexture 					*iconAutoaim;					// this + 0x738 / 1848.
	CHudTexture 					*iconZoomedCrosshair;			// this + 0x73C / 1852.
	CHudTexture 					*iconZoomedAutoaim;				// this + 0x740 / 1856.
	CHudTexture						*iconSmall;						// this + 0x744 / 1860.

// TF2 specific
	bool					bShowUsageHint;							// this + 0x748 / 1864, if true, then when you receive the weapon, show a hint about it
};