#include "vector.h"
#include "basehandle.h"
#include "baseentity.h"

// total size 292 = 4 + 12 + 12 + 4 + 260
struct TurnedZombieEntry_t {
    EHANDLE m_hRagDollHandle;      // 924
    Vector m_vecTurnedPosition;         // 928
    QAngle m_angTurnedAngle;            // 940
    float m_flTurnedTime;               // 952
    char m_szModel[260];                // 956
};

// entity size 3552
class CNMRiH_TurnedZombie_Watcher : public CBaseEntity, public IGameEventListener2 
{
                                                    // vptr, this + 0x394 / 916
    bool m_bEventHooked;                            // this + 0x398 / 920
    TurnedZombieEntry_t m_TurnedZombieEntry[9];     // this + 0x39C / 924
};