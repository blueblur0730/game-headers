#include "player.h"
#include "ai_speech.h"

extern IResponseSystem *g_pResponseSystem;

// size 76
class CBaseMultiplayerPlayer : public CAI_ExpresserHost<CBasePlayer>
{
public:
    DECLARE_CLASS(CBaseMultiplayerPlayer, CAI_ExpresserHost<CBasePlayer>);
    DECLARE_ENT_SCRIPTDESC();

public:
    CBaseMultiplayerPlayer();
    ~CBaseMultiplayerPlayer();

    virtual void Spawn(void);
    virtual void PostConstructor(const char *szClassname);
    virtual void Precache(void) { BaseClass::Precache(); };

    virtual IResponseSystem *GetResponseSystem() { return g_pResponseSystem; };
    virtual void ModifyOrAppendCriteria(AI_CriteriaSet &criteriaSet);

    virtual bool ClientCommand(const CCommand &args);
    virtual bool CanHearAndReadChatFrom(CBasePlayer *pPlayer);
    virtual bool CanSpeak(void) { return true; }
    virtual CAI_Expresser *GetExpresser() { return m_pExpresser; }
    virtual bool SpeakIfAllowed(AIConcept_t concept, const char *modifiers = NULL, char *pszOutResponseChosen = NULL, size_t bufsize = 0, IRecipientFilter *filter = NULL);
    virtual bool SpeakConceptIfAllowed(int iConcept, const char *modifiers = NULL, char *pszOutResponseChosen = NULL, size_t bufsize = 0, IRecipientFilter *filter = NULL) { return false; }
    virtual bool CanBeAutobalanced() { return true; }
    virtual bool CanSpeakVoiceCommand(void) { return true; }
    virtual bool ShouldShowVoiceSubtitleToEnemy(void) { return false; }
    virtual void NoteSpokeVoiceCommand(const char *pszScenePlayed) {}
    virtual void OnAchievementEarned(int iAchievement) {}
    virtual CMultiplayer_Expresser *GetMultiplayerExpresser() { return m_pExpresser; }
    virtual int CalculateTeamBalanceScore(void);

protected:
    virtual CAI_Expresser *CreateExpresser(void);

public:
    enum
    {
        CHAT_IGNORE_NONE = 0,
        CHAT_IGNORE_ALL,
        CHAT_IGNORE_TEAM,
    };

    // inlined functions
    void SetLastForcedChangeTeamTimeToNow(void) { m_flLastForcedChangeTeamTime = gpGlobals->curtime; }
    float GetLastForcedChangeTeamTime(void) { return m_flLastForcedChangeTeamTime; }
    void SetTeamBalanceScore(int iScore) { m_iBalanceScore = iScore; }
    int GetTeamBalanceScore(void) { return m_iBalanceScore; }
    float GetConnectionTime(void) { return m_flConnectionTime; }

    //----------------------------
    int GetPerLifeCounterKV(const char *pszKey) { return m_pAchievementKV->GetInt(pszKey, 0); }

private:
    int m_iCurrentConcept; // this + 0x126C / 4716. since nmrih dose not implement concepts for CBaseMultiplayerPlayer, this is left unused.

public:
    int m_iIgnoreGlobalChat;               // this + 0x1270 / 4720.
    float m_flAreaCaptureScoreAccumulator; // this + 0x1274 / 4724.
    float m_flCapPointScoreRate;           // this + 0x1278 / 4728.

private:
    CMultiplayer_Expresser *m_pExpresser;             // this + 0x1280 / 4736.
    float m_flConnectionTime;                         // this + 0x1284 / 4740.
    float m_flLastForcedChangeTeamTime;               // this + 0x1288 / 4744.
    int m_iBalanceScore;                              // this + 0x128C / 4748. a score used to determine which players are switched to balance the teams
    KeyValues *m_pAchievementKV;                      // this + 0x1290 / 4752.
    CUtlDict<float, int> m_RateLimitLastCommandTimes; // this + 0x1294 / 4756. This lets us rate limit the commands the players can execute so they don't overflow things like reliable buffers. size = 36.
};