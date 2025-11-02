#include "sdk_player_shared.h"

extern ConVar sv_sprint_penalty;

class CNMRiH_PlayerShared : public CSDKPlayerShared
{
    DECLARE_EMBEDDED_NETWORKVAR()
    DECLARE_CLASS( CNMRiH_PlayerShared, CSDKPlayerShared );

    virtual void StartSprinting( void );
    virtual void StopSprinting( void );
    virtual float GetSprintPenalty(void) { return sv_sprint_penalty.GetFloat(); };
};