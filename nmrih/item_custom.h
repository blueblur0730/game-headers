#include "baseitem.h"

// 1184.
// entity size 1324.
class CNMRiH_ItemCustom : public CNMRiH_BaseItem
{
public:
    DECLARE_CLASS(CNMRiH_ItemCustom, CNMRiH_BaseItem);

    const char *m_iszLabel;         // this + 0x510 / 1296, sendprop.
    const char *m_iszIcon;          // this + 0x514 / 1300, sendprop.
    int m_iWeight;                  // this + 0x518 / 1304, sendprop.
    bool m_bHighlight;              // this + 0x51C / 1308, sendprop.
    bool m_bHoverSelect;            // this + 0x51D / 1309, sendprop.

    char *m_szLabel;                // this + 0x524 / 1316.
    char *m_szIcon;                 // this + 0x528 / 1320.
};