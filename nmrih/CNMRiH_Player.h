#include "sdk_player.h"

// entity size 5600.
class CNMRiH_Player : public CSDKPlayer
{
    float m_flDeployTime;                // this + 0x132C / 4908, spawn timestamp? sendprop.
    float m_flThrowDropTimer;            // this + 0x1330 / 4912, a countdown timer as the cooldown for droping items, sendprop.
    int _carriedWeight;                  // this + 0x1334 / 4916, sendprop.
    int m_iActionsThisClick;             // this + 0x1338 / 4920, unknown, sendprop.
    int m_iDeaths;                       // this + 0x133C / 4924, death times, sendprop.
    int m_iTokens;                       // this + 0x1340 / 4928, the respawn tokens, sendprop.
    bool m_bTalkingWalkie;               // this + 0x1344 / 4932, sendprop.
    bool m_bStickyIronsight;             // this + 0x1345 / 4933, using irongsight or not, sendprop.
    bool m_bIsExtracted;                 // this + 0x1346 / 4934, sendprop.
    bool _vaccinated;                    // this + 0x1347 / 4935, sendprop.
    bool m_bSprintEnabled;               // this + 0x1348 / 4936, sendprop.
    bool m_bDiedWhileInfected;           // this + 0x1349 / 4937, sendprop.
    bool m_bGrabbed;                     // this + 0x134A / 4938, grapped by zombie, gives you flag 'FL_ATCONTROLS', sendprop.
    bool _bleedingOut;                   // this + 0x134B / 4939, sendprop.
    bool _contemplatingSuicide;          // this + 0x134C / 4940, sendprop.
    bool _usingToolSecondary;            // this + 0x134D / 4941, sendprop.
    bool m_bUsingProgressTrigger;        // this + 0x134E / 4942, sendprop.
    bool m_bGrabbedByCrawler;            // this + 0x134F / 4943, sendprop.
    bool m_bLevelTransition;             // this + 0x1350 / 4944, sendprop.
    bool m_bBarricading;                 // this + 0x1351 / 4945, sendprop.
    bool m_bPlacingBoard;                // this + 0x1352 / 4946, sendprop.
    bool m_bSpawned;                     // this + 0x1353 / 4947, see CNMRiH_Player::Spawn() and COverlord_Wave_Controller::SpawnNewPlayers(). assuming used for survival mode to see whether or not spawn dead players when the new wave begins.
    bool m_bStateEnterActive;            // this + 0x1354 / 4948, see CNMRiH_Player::State_Enter_ACTIVE().
    bool m_bShouldIgnoreFallDamage;      // this + 0x1355 / 4949. this is used by CNMRiH_GameRules::FlPlayerFallDamage() and trigger_no_fall_damage
    float _nextRespawnTime;              // this + 0x1358 / 4952, sendprop.
    float m_flWaterSplashTime;           // this + 0x135C / 4956, unknown, sendprop.
    float m_flPartialBlindnessEffectEnd; // this + 0x1360 / 4960, sendprop.
    char m_szVoiceSetIdent[16];          // this + 0x1364 / 4964, the voice identity of the player's character, retrieved on player initial spawn by checking client convar 'cl_voice_set', sendprop.
    float m_flNextVoiceCmdTime;          // this + 0x1374 / 4980, next timestamp you can voice command. see CNMRiH_Player::VoiceCommand().
    float m_flVoiceCmdCooldown;          // this + 0x1378 / 4984, cooldown duration.
    float m_flNextAudibleReloadTime;     // this + 0x137C / 4988, see TE_AudibleReload(). used for this temp entity to play the reload sound.
    float m_flAudibleReloadCooldown;     // this + 0x1380 / 4992, see TE_AudibleReload(). use the value of 'sv_voice_cooldown'.
    int m_iAtControlCount;               // this + 0x1384 / 4996, setting this > 1 will add the flag 'FL_ATCONTROLS' on the player. (Player can't move, but keeps key inputs for controlling another entity). 0 otherwise remove the flag.
    EHANDLE m_hGrabber;                  // this + 0x1388 / 5000, The zombie who is grabbing me.
    float m_flExractedFrozenTime;        // this + 0x138C / 5004, Time to frozen the player after extracted, 'sv_extract_pause' + gpGlobals->curtime.
    int m_iKills;                        // this + 0x1390 / 5008, Increamented by 1 from 'npc_killed' event. once reached the amount one respawn token required, this is reset to 0.
    float m_flNextSpawnTryTime;          // this + 0x1394 / 5012, the timestamp for the next attempt trying to spawn you at the spawn point.
    int _playerModelIndex;               // this + 0x1398 / 5016, sendprop.
    char *m_szRespawnWeaponName;         // this + 0x139C / 5020.
    int m_iRespawnAmmoPercentage;        // this + 0x13A0 / 5024. see convar 'sv_respawn_ammo_pct'.
    float m_flNextMovableAmmoPickupTime; // this + 0x13A4 / 5028, see CItem_AmmoBox::Use(). not clear what this is used for. related convar: 'sv_movable_ammo_pickup_delay'
    float m_flSpeedModifier;             // this + 0x13A8 / 5032, sendprop.
    float m_flSpeedOverride;             // this + 0x13AC / 5036, sendprop.
    float m_flTriggerSpeedModifier;      // this + 0x13B0 / 5040, sendprop.
    EHANDLE m_hCustomItems[128];         // this + 0x13B4 / 5044, sendprop, size = 4 * 128 = 512.
    int m_iCustomItemCount;              // this + 0x15B4 / 5556. indicates the number of custom items the player has.
    float m_flNextThrottledVoiceCommand; // this + 0x15B8 / 5560, next time we could use voice command, see 'CNMRiH_Player::ThrottledEmitSound' and ' CNMRiH_GameRules::ClientCommand'.
    float m_flThrottledVoiceCooldown;    // this + 0x15BC / 5564, left unused. See unused function 'CNMRiH_Player::ThrottledEmitSound'.
    float m_flNextPainSound;             // this + 0x15C0 / 5568, the next time you can hear pain sound.
    float m_flPainSoundCoodown;          // this + 0x15C4 / 5572, the pain sound cooldown duration.
    float m_flAutoKickTimestamp;         // this + 0x15C8 / 5574, m_flAutoKickToleration + gpGlobals->curtime.
    float m_flAutoKickToleration;        // this + 0x15CC / 5580, use value of convar 'sv_idle_autokick_time'.
    float m_flIdleItmeDropTimestamp;     // this + 0x15D0 / 5584, m_flIdleItmeDropDruation + gpGlobals->curtime.
    float m_flIdleItmeDropDruation;      // this + 0x15D4 / 5588, uses value of 'sv_idle_item_drop_time'.
    float m_flVoiceCmdForceTransmitTime; // this + 0x15D8 / 5592, uses value of 'sv_voice_cmd_force_transmit_time'. also used on  CNMRiH_Player::ShouldTransmit().
    string_t m_szHeldEntityName;         // this + 0x15DC / 5596, see CNMRiH_Player::IsHoldingEntity(), notice this: return *(_DWORD *)(m_hUseEntity + 100) == *((_DWORD *)this + 1399). this is testing strings.
};