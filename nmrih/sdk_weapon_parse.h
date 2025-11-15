#include "weapon_parse.h"
#include "networkvar.h"

// size 2044.
//--------------------------------------------------------------------------------------------------------
class CSDKWeaponInfo : public FileWeaponInfo_t
{
public:
	DECLARE_CLASS_GAMEROOT( CSDKWeaponInfo, FileWeaponInfo_t );
	
	CSDKWeaponInfo();
	
	virtual void Parse( ::KeyValues *pKeyValuesData, const char *szWeaponName );

	char m_szAnimExtension[16];		// this + 0x749 / 1865, string used to generate player animations with this weapon
	int m_iNumClips;				// this + 0x75C / 1884, number of clips in this weapon

	// Parameters for FX_FireBullets:
	int		m_iDamage;				// this + 0x760 / 1888, amount of damage to inflict on hit.
	int		m_iBullets;				// this + 0x764 / 1892, number of bullets to fire in a single shot.
	float	m_flCycleTime;			// this + 0x768 / 1896, time between each bullet in a burst.
	float   m_flCycleTimeBurst;		// this + 0x76C / 1900, time between each bullet in a burst when firing in burst mode.
	int     m_iHeadShotDamage;		// this + 0x770 / 1904, amount of damage to inflict on a headshot.
	char    m_szMuzzleFlash[32];	// this + 0x774 / 1908, name of the muzzle flash effect.

	char 	padding1;					// this + 0x793 / 1939.
	float 	m_flQuickAttackStaminaCost;	// this + 0x794 / 1940, cost of quick attack in stamina.
	int		m_iHafAmmoThreshold;		// this + 0x798 / 1944, ammo threshold for half.
	int 	m_iOverHalfAmmoThreshold;	// this + 0x79C / 1948, ammo threshold for more than over a half.
	int 	m_iAlmostFullAmmoThreshold;	// this + 0x7A0 / 1952, ammo threshold for almost full.
	int 	m_iFullAmmoThreshold;		// this + 0x7A4 / 1956, ammo threshold for full.
	int 	m_iMuzzelSmokeCount;		// this + 0x7A8 / 1960, number of muzzle smoke puffs to create.
	float 	m_flMuzzleSmokeTime;		// this + 0x7AC / 1964, lifetime of muzzle smoke puffs.
	char 	m_szSwingTrailParticleName[32];	// this + 0x7B0 / 1968, name of the melee swing trail particle effect.

	char 	padding2;					// this + 0x7CF / 1999.
	bool 	m_bAllowSuicide;			// this + 0x7D0 / 2000, whether or not this weapon can be used to commit suicide.
	char 	m_szSuicideParticle[32];	// this + 0x7D1 / 2001, name of the suicide particle effect.
	char 	padding3;					// this + 0x7F0 / 2032.
	int 	m_iDamageType;				// this + 0x7F4 / 2036. the damage type of this eapon. note: in weapon scripts, this is a string key. it get converted to number inside the game.
	float 	m_flFOV;					// this + 0x7F8 / 2040. see CWeaponSDKBase::GetWeaponFOV().
};

// Each game implements this. It can return a derived class and override Parse() if it wants.
extern FileWeaponInfo_t* CreateWeaponInfo();