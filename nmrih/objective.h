#include "utlvector.h"
#include "utlrbtree.h"
#include "string_t.h"
#include "variant.h"
#include "igamesystem.h"
#include "baseentity.h"

// size 64.
class CNMRiH_Objective
{
public:
    CNMRiH_Objective();
    ~CNMRiH_Objective();

public:
    int m_iID;                          // this
    const char *m_szName;               // this + 0x4
    const char *m_szDesc;               // this + 0x8
    CUtlVector<string_t> m_aEntityList; // this + 0xC  / 12
    int m_iEntityCount;                 // this + 0x18 / 24

    CUtlVector<int> m_aLinkList; // this + 0x20 / 32
    int m_iLinkCount;            // this + 0x2C / 44

    bool m_bIsAntiObjective;         // this + 0x34 / 52
    const char *m_szObjBoundrayName; // this + 0x38 / 56
    HSCRIPT m_hScriptInstance;       // this + 0x3C / 60
};

// size 128.
class CNMRiH_ObjectiveManager : public CAutoGameSystemPerFrame, public IGameEventListener2
{
public:
    virtual void LevelInitPostEntity(void);
    virtual void LevelShutdownPostEntity(void);

    CNMRiH_ObjectiveManager();
    ~CNMRiH_ObjectiveManager();

    virtual void FireGameEvent(IGameEvent *pEvent);

public:
    char unknown[4];                                        // this + 0x10 / 16.
    CUtlVector<CNMRiH_Objective *> m_aObjectiveVector;      // this + 0x14 / 20.
    int m_iObjectivesCount;                                 // this + 0x20 / 32.

    CUtlVector<CNMRiH_Objective *> m_aAntiObjectiveVector;  // this + 0x28 / 40.
    int m_iAntiObjectivesCount;                             // this + 0x34 / 52.

    CUtlRBTree<string_t> m_pExtractionRBTree;               // this + 0x3C / 60, size 28.

    CUtlVector<int> m_aObjectiveChainVector;                // this + 0x58 / 88.
    int m_iObjectiveChainCount;                             // this + 0x64 / 100.
    char unknow3[4];                                        // this + 0x68 / 104.
    bool m_bIsCompleted;                                    // this + 0x6C / 108.
    bool m_bIsFailed;                                       // this + 0x6D / 109.
    bool m_bHasStarted;                                     // this + 0x6E / 110, indicates wheather currently an objected has been started.

    int m_iCurrentObjectiveIndex;                           // this + 0x70 / 112.
    CNMRiH_Objective *m_pCurrentObjective;                  // this + 0x74 / 116.
    EHANDLE m_hCurrentObjectiveBoundrayHandle;              // this + 0x78 / 120.
    EHANDLE m_hExtractionEntityHandle;                      // this + 0x7C / 124.
};

// entity size 1076
class CNMRiH_ObjectiveBoundary : public CBaseEntity
{
public:
    DECLARE_CLASS(CNMRiH_ObjectiveBoundary, CBaseEntity);
    DECLARE_SERVERCLASS();
    DECLARE_DATADESC();

    CNMRiH_ObjectiveBoundary();
    ~CNMRiH_ObjectiveBoundary();

    virtual int UpdateTransmitState(void);
    virtual void PostConstructor(const char *szClassname);

public:
    char m_szGlowEntityNames[40];           // this + 0x394 / 916, glow entity names, sendprop.
    Color m_GlowEntityColors;               // this + 0x3BC / 956, glow entity colors, sendprop.

    bool m_bActive;                         // this + 0x3E4 / 996, indicates whether the boundary is active or not, sendprop.
    bool m_bBlip;                           // this + 0x3E5 / 997, unkown bool, sendprop.
    int m_iID;                              // this + 0x3E8 / 1000, unique objective id of which this boundary belongs to.
    const char *m_szName;                   // this + 0x3EC / 1004, unique objective name of which this boundary belongs to.

    CUtlVector<string_t> m_aUnkownVector;   // this + 0x3F0 / 1008, unknown vector, size 12.
    int m_iUnkownCount;                     // this + 0x3FC / 1020, unknown count.

    COutputEvent m_OnStart;                 // this + 0x404 / 1028, size 24.
    COutputEvent m_OnFinish;                // this + 0x41C / 1052.
};