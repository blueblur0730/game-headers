#include "player.h"
#include "server_class.h"
#include "sdk_shareddefs.h"
#include "commonmacros.h"

#include "baseentity_shared.h"
#include "basemultiplayerplayer.h"
#include "sdk_playeranimstate.h"
#include "sdk_player_shared.h"

enum SDKPlayerState
{
	STATE_ACTIVE = 0,		// Happily running around in the game.
	STATE_WELCOME,			// First entering the server (shows level intro screen).
    STATE_DEATH_ANIM,       // Playing death animation.
	STATE_OBSERVER_MODE,	// Game observer mode.
	STATE_COUNT
};

struct SDKPlayerStateInfo
{
    int				m_nPlayerState;
	const char		*m_pStateName;

	// Enter/Leave state.
	void ( CSDKPlayer::*pfnEnterState )();	
	void ( CSDKPlayer::*pfnLeaveState )();

	// PreThink (called every frame).
	void ( CSDKPlayer::*pfnPreThink )();
};

class CSDKPlayer : public CBaseMultiplayerPlayer, public ISDKPlayerAnimStateHelpers
{
public:
	DECLARE_CLASS( CSDKPlayer, CBaseMultiplayerPlayer );
	DECLARE_SERVERCLASS();
	DECLARE_PREDICTABLE();
	DECLARE_DATADESC();

	CSDKPlayer();
	~CSDKPlayer();

public:
    QAngle m_angEyeAngles;                  // this + 0x12B8 / 4792. size = 12. Note: 'm_angEyeAngles[0]' and 'm_angEyeAngles[1]' is sendprop.
    int m_iShotsFired;                      // this + 0x12C4 / 4804, sendprop.
    EHANDLE m_hRagdoll;                     // this + 0x12C8 / 4808, sendprop.
    CSDKPlayerShared m_Shared;              // this + 0x12CC / 4812, sendprop, size = 44.
    SDKPlayerState m_iPlayerState;          // this + 0x12F8 / 4856, sendprop.
    SDKPlayerStateInfo *m_pCurrentStateInfo;// this + 0x12FC / 4860.
    CBaseEntity *m_pPenetratingEntity;      // this + 0x1300 / 4864. see CSDKPlayer::FireBullets(). assumming this is the penetrating entity our bullets are hitting. it is stored temporarily only in FireBullets(), every penetration will update this varible and call again FireBullets().
    int m_iCommandNumber;                   // this + 0x1304 / 4868. see CSDKPlayer::NoteWeaponFired(). m_iCommandNumber = m_pCurrentCommand->command_number, used on CSDKPlayer::WantsLagCompensationOnEntity().
    char padding[4];
    EHANDLE m_hIntroCamera;                 // this + 0x130C / 4876, the 'point_viewcontrol' entity that this player is using for. sometimes it will be 'info_player_nmrih' if the camera not found.
    float m_flTimeToMoveNextCamera;         // this + 0x1310 / 4880. the timestamp that you will be move to the next camera. this is hardcoded as 6.0f in seconds in CSDKPlayer::MoveToNextIntroCamera().
    ISDKPlayerAnimState *m_PlayerAnimState; // this + 0x1314 / 4884.
    bool m_bSpawnInterpCounter;             // this + 0x1318 / 4888, unknwon, sendprop.
    bool m_bJoinedGame;                     // this + 0x1319 / 4889. assuming this is set to true if the player has sent the command 'join_game', see CSDKPlayer::ClientCommand(). This is a required key to tell the game spawn players.
    bool m_bActiveNow;                      // this + 0x131A / 4890. assuming this is set to true if the player state is STATE_ACTIVE, see CSDKPlayer::ClientCommand(). This is set to true before sending the command 'join_game'. also, entity 'predicted_viewmodel' can only be created if this is true, see CSDKPlayer::CreateViewModel().
    int m_iSpawnArmorValue;                 // this + 0x131C / 4892. left unused.
    Vector m_vecLastDamageForce;            // this + 0x1320 / 4896. Temprorarily stores the damage force. size = 12.

public:
    virtual bool ShouldCollide( int collisionGroup, int contentsMask ) const { return BaseClass::ShouldCollide( collisionGroup, contentsMask ); };
	virtual void Spawn( void );
    virtual void Precache( void ) {}

protected:
	virtual void TraceAttack( const CTakeDamageInfo &info, const Vector &vecDir, trace_t *ptr, CDmgAccumulator *pAccumulator = NULL );

public:
    virtual int	OnTakeDamage( const CTakeDamageInfo &info );
    virtual void Event_Killed( const CTakeDamageInfo &info );
    virtual bool BecomeRagdollOnClient( const Vector &force ) { return true; };
    virtual void Weapon_Equip( CBaseCombatWeapon *pWeapon );
    virtual int OnTakeDamage_Alive(const CTakeDamageInfo &info);
    virtual void PostThink( void );
    virtual void CreateViewModel( int viewmodelindex = 0 );
    virtual void SetupVisibility( CBaseEntity *pViewEntity, unsigned char *pvs, int pvssize );
	virtual bool WantsLagCompensationOnEntity( const CBasePlayer	*pPlayer, const CUserCmd *pCmd, const CBitVec<MAX_EDICTS> *pEntityTransmitBits ) const;
	virtual void SharedSpawn();
    virtual void InitialSpawn( void );
    virtual void PlayerDeathThink( void ) { };
    virtual void PreThink( void );
    const Vector GetPlayerMins( void ) const;
    const Vector GetPlayerMaxs( void ) const;
    virtual int FlashlightIsOn( void ) { return IsEffectActive( EF_DIMLIGHT ); }
    virtual void FlashlightTurnOn( void ) { AddEffects( EF_DIMLIGHT ); };
	virtual void FlashlightTurnOff( void ) { RemoveEffects( EF_DIMLIGHT ); };
    virtual void SetAnimation( PLAYER_ANIM playerAnim ) { };
    virtual void CheatImpulseCommands( int iImpulse );
    virtual bool ClientCommand( const CCommand &args );
    virtual bool ModeWantsSpectatorGUI( int iMode ) { return iMode - 1 > 1; }
    virtual CBaseEntity	*EntSelectSpawnPoint( void );
    virtual void LeaveVehicle(const Vector &vecExitPoint = vec3_origin, const QAngle &vecExitAngles = vec3_angle) { CBasePlayer::LeaveVehicle(vecExitPoint, vecExitAngles); };
    virtual void CreateRagdollEntity( void );
    virtual void CommitSuicide( bool bExplode = false, bool bForce = false );
    virtual int SpawnArmorValue( void ) const { return m_iSpawnArmorValue; }
    virtual void GiveDefaultItems() { };
    virtual void SetSpawnArmorValue( int iValue ) { m_iSpawnArmorValue = iValue; }
    virtual bool CanAttack( void ); const { return true; }
    virtual int GetPlayerStance() { return IsSprinting() ? 2 : m_iStance /* Upper class variable */; };
    virtual void TryIncreaseBloodiness( int iBlodiness ) { };

    virtual void State_Enter_ACTIVE( void );
    virtual void State_PreThink_ACTIVE( void ) {};
    virtual void State_Enter_OBSERVER_MODE( void );
    virtual void State_PreThink_OBSERVER_MODE( void );
    virtual void State_Enter_DEATH_ANIM( void );
    virtual void State_PreThink_DEATH_ANIM( void );

    void State_Enter_WELCOME( void );   // fires event 'player_welcome'.
    void State_PreThink_WELCOME( void );
    SDKPlayerStateInfo *State_LookupInfo( SDKPlayerState nState );

    virtual bool CanSprint() { return (!( m_Local.m_bDucked && !m_Local.m_bDucking ) && (GetWaterLevel() != 3) ); };
    virtual void InitSpeeds( void );
    virtual void ThrowActiveWeapon( void );
    virtual void SDKThrowWeapon( CWeaponSDKBase *pWeapon, Vector const& vecOrigin, QAngle const& vecAngle, float flScale );
    virtual void SDKThrowWeaponDir( CWeaponSDKBase *pWeapon, Vector const& vecOrigin, Vector *vecUnkown );  // only called by SDKThrowWeapon.

    void SetSprinting( bool bSprinting ) { m_Shared.m_bIsSprinting = bSprinting; }
};

SDKPlayerStateInfo *CSDKPlayer::State_LookupInfo( SDKPlayerState nState )
{
	static SDKPlayerStateInfo playerStateInfos[] =
	{
		{ STATE_ACTIVE,				"STATE_ACTIVE",				&CSDKPlayer::State_Enter_ACTIVE,			NULL,	&CSDKPlayer::State_PreThink_ACTIVE },
		{ STATE_WELCOME,			"STATE_WELCOME",			&CSDKPlayer::State_Enter_WELCOME,			NULL,	&CSDKPlayer::State_PreThink_WELCOME },
		{ STATE_DEATH_ANIM,			"STATE_DEATH_ANIM",			&CSDKPlayer::State_Enter_DEATH_ANIM,		NULL,	&CSDKPlayer::State_PreThink_DEATH_ANIM },
		{ STATE_OBSERVER_MODE,		"STATE_OBSERVER_MODE",		&CSDKPlayer::State_Enter_OBSERVER_MODE,		NULL,	&CSDKPlayer::State_PreThink_OBSERVER_MODE },
	};

	for ( int iState = 0; iState < ARRAYSIZE( playerStateInfos ); ++iState )
	{
		if ( playerStateInfos[iState].m_nPlayerState == nState )
			return &playerStateInfos[nState];
	}

	return NULL;
}
