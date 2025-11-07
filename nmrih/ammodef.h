#include "convar.h"

// size 60.
struct Ammo_t
{
    char *pName;         // this + 0x4 / 4.
    int nDamageType;     // this + 0x8 / 8.
    int eTracerType;     // this + 0xC / 12.
    float flDamageForce; // this + 0x10 / 16.
    int nMinSplashSize;  // this + 0x14 / 20.
    int nMaxSplashSize;  // this + 0x18 / 24.

    int nFlags;                 // this + 0x1C / 28.
    bool bUseSpecificCarryCVar; // this + 0x1E / 30. If false, use the global carry cvar 'inv_ammoweight' and 'inv_maxcarry' results. see CAmmoDef::MaxCarry(). nmrih spcifical.

    // Values for player/NPC damage and carrying capability
    // If the integers are set, they override the CVars
    int pPlrDmg;                    // this + 0x20 / 32, CVar for player damage amount
    int pNPCDmg;                    // this + 0x24 / 36, CVar for NPC damage amount
    int pHeadShotDmg;               // this + 0x28 / 40, CVar for headshot damage amount. nmrih specific.
    int pMaxCarry;                  // this + 0x2C / 44, CVar for max carry amount
    const ConVar *pPlrDmgCVar;      // this + 0x30 / 48, CVar for player damage amount
    const ConVar *pNPCDmgCVar;      // this + 0x34 / 52, CVar for NPC damage amount
    const ConVar *pHeadShotDmgCVar; // this + 0x38 / 56, CVar for headshot damage amount. nmrih specific.
    const ConVar *pMaxCarryCVar;    // this + 0x3C / 60, CVar for max carry amount
};

// Used to tell AmmoDef to use the cvars, not the integers
#define USE_CVAR -1
// Ammo is infinite
#define INFINITE_AMMO -2

enum AmmoTracer_t
{
    TRACER_NONE,
    TRACER_LINE,
    TRACER_RAIL,
    TRACER_BEAM,
    TRACER_LINE_AND_WHIZ,
};

enum AmmoFlags_t
{
    AMMO_FORCE_DROP_IF_CARRIED = 0x1,
    AMMO_INTERPRET_PLRDAMAGE_AS_DAMAGE_TO_PLAYER = 0x2,
};

#include "shareddefs.h"

// size 1924.
//=============================================================================
//	>> CAmmoDef
//=============================================================================
class CAmmoDef
{

public:
    Ammo_t m_AmmoType[MAX_AMMO_TYPES]; // this + 0x0 / 0, size = 1920.
    int m_nAmmoIndex;                  // this + 0x784 / 1924.

    Ammo_t *GetAmmoOfIndex(int nAmmoIndex);
    int Index(const char *psz);
    int PlrDamage(int nAmmoIndex);
    int NPCDamage(int nAmmoIndex);
    int MaxCarry(int nAmmoIndex);
    int DamageType(int nAmmoIndex);
    int TracerType(int nAmmoIndex);
    float DamageForce(int nAmmoIndex);
    int MinSplashSize(int nAmmoIndex);
    int MaxSplashSize(int nAmmoIndex);
    int Flags(int nAmmoIndex);
    int GetNumAmmoTypes(void) const { return m_nAmmoIndex; } // nmrih specific.

    void AddAmmoType(char const *name, int damageType, int tracerType, int plr_dmg, int npc_dmg, int carry, float physicsForceImpulse, int nFlags, int minSplashSize = 4, int maxSplashSize = 8);
    void AddAmmoType(char const *name, int damageType, int tracerType, char const *plr_cvar, char const *npc_var, char const *carry_cvar, float physicsForceImpulse, int nFlags, int minSplashSize = 4, int maxSplashSize = 8);
    void AddAmmoType(char const *name, int damageType, int tracerType, char const *plr_cvar, char const *npc_var, char const *headshot_cvar, char const *carry_cvar, float physicsForceImpulse, int nFlags, int minSplashSize = 4, int maxSplashSize = 8);
    void AddAmmoType(char const *name, int damageType, int tracerType, int plr_dmg, int npc_dmg, int headshot_dmg, int carry, float physicsForceImpulse, int nFlags, int minSplashSize = 4, int maxSplashSize = 8);

    CAmmoDef(void);
    virtual ~CAmmoDef(void);

private:
    bool AddAmmoType(char const *name, int damageType, int tracerType, int nFlags, int minSplashSize, int maxSplashSize);
};

// Get the global ammodef object. This is usually implemented in each mod's game rules file somewhere,
// so the mod can setup custom ammo types.
CAmmoDef *GetAmmoDef();

extern ConVar inv_maxcarry;
extern ConVar inv_ammoweight;

// nmrih specific.
int CAmmoDef::MaxCarry(int nAmmoIndex)
{
    if (nAmmoIndex < 1 || nAmmoIndex >= m_nAmmoIndex)
        return 0;

    if (!m_AmmoType[nAmmoIndex].bUseSpecificCarryCVar)
    {
        int ammoweight = inv_ammoweight.GetInt();
        if (ammoweight)
        {
            return inv_maxcarry.GetInt() / ammoweight;
        }
    }

    return 0;
}

// this should be implemented in gamerules file.
CAmmoDef *GetAmmoDef()
{
    static CAmmoDef def;
    static bool bInitted = false;

    if (!bInitted)
    {
        bInitted = true;

        def.AddAmmoType("9mm", 2, 4, 25, 25, 300, 60, 381.69882, 0, 4, 8);
        def.AddAmmoType("45ACP", 2, 4, 30, 30, 600, 28, 381.69882, 0, 4, 8);
        def.AddAmmoType("357", 2, 4, 60, 60, 600, 24, 6231.8174, 0, 4, 8);
        def.AddAmmoType("12gauge", 536870912, 4, 20, 20, 200, 20, 747.81812, 0, 4, 8);
        def.AddAmmoType("22LR", 2, 4, 100, 100, 200, 120, 381.69882, 0, 4, 8);
        def.AddAmmoType("308", 2, 4, 150, 150, 9999, 30, 934.77264, 0, 4, 8);
        def.AddAmmoType("556", 2, 4, 30, 30, 300, 60, 381.69882, 0, 4, 8);
        def.AddAmmoType("762mm", 2, 4, 350, 350, 600, 80, 934.77264, 0, 4, 8);
        def.AddAmmoType("grenades", 64, 1, 600, 600, 600, 4, 1.0, 0x10000, 4, 8);
        def.AddAmmoType("molotov", 64, 1, 600, 600, 600, 4, 1.0, 0x10000, 4, 8);
        def.AddAmmoType("tnt", 64, 1, 600, 600, 600, 4, 1.0, 0x10000, 4, 8);
        def.AddAmmoType("arrow", 4, 0, 0, 0, 10, 1.0, 0, 4, 8);
        def.AddAmmoType("fuel", 4, 0, 0, 0, 200, 1.0, 0, 4, 8);
        def.AddAmmoType("board", 0, 0, 0, 0, 5, 1.0, 0, 4, 8);
        def.AddAmmoType("flare", 0, 0, 0, 0, 5, 1.0, 0, 4, 8);
    }

    return &def;
}