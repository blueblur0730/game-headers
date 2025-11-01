#include "networkvar.h"

// size 44.
class CSDKPlayerShared 
{
	DECLARE_EMBEDDED_NETWORKVAR()
	DECLARE_CLASS_NOBASE( CSDKPlayerShared );

	friend class CSDKPlayer;
	typedef CSDKPlayer OuterClass;

    virtual void StartSprinting( void );
    virtual void StopSprinting( void );
    virtual float GetSprintPenalty( void );

public:
                                    // vptr this + 0x0 / 0.
    bool m_bIsSprinting;            // this + 0x4 / 4, sendprop. (4816)
    bool m_bGaveSprintPenalty;      // this + 0x5 / 5.  (4817)
    float m_flStamina;              // this + 0x8 / 8, sendprop. (4820)
    bool m_bIsJumping;              // this + 0xC / 12. (4824)
    Vector m_vecSpeed;              // this + 0x14 / 18 (4832). szie = 12.
    char padding[10];

private:
    OuterClass *m_pOuter;       // this + 0x28 / 40. (4854)
};