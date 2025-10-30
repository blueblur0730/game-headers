#include "utlvector.h"
#include "utlrbtree.h"
#include "string_t.h"
#include "variant.h"
#include "igamesystem.h"
#include "baseentity.h"

class CNMRiH_Objective                  // size 0x40 / 64
{
    int m_iID;                          // this
    const char *m_szName;               // this + 0x4
    const char *m_szDesc;               // this + 0x8
    CUtlVector<string_t> m_aEntityList; // this + 0xC  / 12
    int m_iEntityCount;                 // this + 0x18 / 24

    CUtlVector<int> m_aLinkList;        // this + 0x20 / 32
    int m_iLinkCount;                   // this + 0x2C / 44

    bool m_bIsAntiObjective;            // this + 0x34 / 52
    const char *m_szObjBoundrayName;    // this + 0x38 / 56
    HSCRIPT m_hScriptInstance;          // this + 0x3C / 60
};

class CNMRiH_ObjectiveManager : public CAutoGameSystemPerFrame
{
    char unknown[4];                                        // this + 0x10 / 16
    CUtlVector<CNMRiH_Objective *> m_aObjectiveVector;      // this + 0x14 / 20
    int m_iObjectivesCount;                                 // this + 0x20 / 32

    CUtlVector<CNMRiH_Objective *> m_aAntiObjectiveVector;  // this + 0x28 / 40
    int m_iAntiObjectivesCount;                             // this + 0x34 / 52

    CUtlRBTree<string_t> m_pExtractionRBTree;               // this + 0x3C / 60     // CUtlRBTree size 28

    CUtlVector<int> m_aObjectiveChainVector;                // this + 0x58 / 88
    int m_iObjectiveChainCount;                             // this + 0x64 / 100
    char unknow3[4];                                        // this + 0x68 / 104
    bool m_bIsCompleted;                                    // this + 0x6C / 108
    bool m_bIsFailed;                                       // this + 0x6D / 109
    bool m_bHasStarted;                                     // this + 0x6E / 110    // indicates wheather currently an objected has been started.

    int m_iCurrentObjectiveIndex;                           // this + 0x70 / 112
    CNMRiH_Objective *m_pCurrentObjective;                  // this + 0x74 / 116
    EHANDLE m_hCurrentObjectiveBoundrayHandle;              // this + 0x78 / 120
    EHANDLE m_hExtractionEntityHandle;                      // this + 0x7C / 124
};