#include "baseanimating.h"

class CNMRiH_BaseItem : public CBaseAnimating
{
public:
	DECLARE_CLASS(CNMRiH_BaseItem, CBaseAnimating);

public:
	DECLARE_DATADESC();

    ~CNMRiH_BaseItem();

	virtual void Spawn( void );
    virtual void Precache( void );
    virtual int ObjectCaps( void );

    virtual bool CanBeUsed( CBasePlayer *pPlayer ) { return true; };
    virtual void Drop( const Vector &vecVelocity );
	virtual void FallInit( void );						// prepare to fall to the ground
	virtual void FallThink( void );						// make the weapon fall to the ground after spawning
};