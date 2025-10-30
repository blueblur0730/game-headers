#include "vector.h"
#include "basehandle.h"

class CBaseEntity;

// total size 292 = 4 + 12 + 12 + 4 + 260
struct TurnedZombieEntry_t {
    CBaseHandle *m_hRagDollHandle;      // 924
    Vector m_vecTurnedPosition;         // 928
    QAngle m_angTurnedAngle;            // 940
    float m_flTurnedTime;               // 952
    char m_szModel[260];                // 956
};

class CNMRiH_TurnedZombie_Watcher : public CBaseEntity {


private:
    TurnedZombieEntry_t m_TurnedZombieEntry[9]; // entity size 3552
};