#include "shareddefs.h"
#include "tier1/utldict.h"

class KeyValues;

// only initialize once.
static CUtlDict< FileAmmoInfo_t *, unsigned short > m_AmmoInfoDatabase;

// size 172
class FileAmmoInfo_t
{
public:
	FileAmmoInfo_t();
	
	virtual void Parse( ::KeyValues *pKeyValuesData);

    bool m_bParsed;             // this + 0x4 / 4, mark this instaced as parsed the file.
    int m_iCount[4];            // this + 0x8 / 8
    char m_szModelName[80];     // this + 0xC / 12
    char m_szAmmoType[80];      // this + 0x5C / 92
};

extern FileAmmoInfo_t *CreateAmmoInfo();