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
    bool m_bIsSprinting;            // this + 0x4 / 4, sendprop. (4816).
    bool m_bGaveSprintPenalty;      // this + 0x5 / 5.  (4817).
    float m_flStamina;              // this + 0x8 / 8, sendprop. (4820).
    bool m_bIsJumping;              // this + 0xC / 12. (4824).
    char padding[4];
    float m_flSpeedNormal;          // this + 0x14 / 18 (4832).
    float m_flSpeedSprint;          // this + 0x18 / 22 (4836).
    float m_flSpeedUnknown;         // this + 0x1C / 26 (4840).
    float m_flSpeedSights;          // this + 0x20 / 30 (4844).
    float m_flSpeedRifleSights;     // this + 0x24 / 34 (4848). seems left unused.
    char padding2[6];

private:
    OuterClass *m_pOuter;       // this + 0x28 / 40. (4854)
};