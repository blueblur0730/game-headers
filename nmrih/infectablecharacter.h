#include "basecombatcharacter.h"

// size 12.
class CInfectableCharacter : public CBaseCombatCharacter
{
public:
	DECLARE_CLASS(CInfectableCharacter, CBaseCombatCharacter);

public:
	DECLARE_DATADESC();
	DECLARE_SERVERCLASS();
	// script description
	DECLARE_ENT_SCRIPTDESC();

    CInfectableCharacter();
    ~CInfectableCharacter();

    virtual void Spawn(void);
    virtual void Event_Killed(const CTakeDamageInfo &info);
    virtual int OnTakeDamage_Alive(const CTakeDamageInfo &info);

    virtual void PostThink(void);
    virtual void PostNPCThink(void);
    virtual bool IsInfected() const { return (m_flInfectionTime != -1.0f); };
    virtual float GetInfectionTime() const { return m_flInfectionTime; };
    virtual float GetInfectionDeathTime() const { return m_flInfectionDeathTime; };
    virtual void CureInfection();
    virtual void BecomeInfected();

    virtual void TakePills();
    virtual void InnerThink();

    float m_flInfectionTime;            // this + 0x8A4 / 2212, sendprop.
    float m_flInfectionDeathTime;       // this + 0x8A8 / 2216, sendprop.
    float m_flPillTime;                 // this + 0x8AC / 2220. = sv_curelength + gpGlobals->curtime.
    bool __m_bDiedWhileInfected;        // this + 0x8B0 / 2224, see CInfectableCharacter::DiedWhileInfected(). this is not a sendprop.
};

class CBasePlayer : public CInfectableCharacter
{
};