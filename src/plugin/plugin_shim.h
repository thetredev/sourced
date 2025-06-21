#ifndef SOURCED_PLUGIN_SHIM_H_
#define SOURCED_PLUGIN_SHIM_H_


// ========= INCLUDES =========
// HL2SDK
#include <eiface.h>


// ========= PLUGIN SHIM INTERFACE DECLARATION =========
namespace sourced::plugin::shim {


// Declare Plugin interface
class Shim {
public:
    Shim() = default;

public:
    ~Shim() = default;

public:
    bool load(/*IServerGameDLL *server_game_dll*/);
    void unload(void);
    void pause(void);
    void unPause(void);

public:
    void onLevelInit(const char *level_name);
    void onLevelShutdown(void);

public:
    PLUGIN_RESULT onClientConnect(
        bool *client_allowed,
        edict_t *client_entity, const char *client_name, const char *client_address,
        char *reject, int max_reject_len
    );
    void onClientActive(edict_t *client_entity);
    void onClientDisconnect(edict_t *client_entity);
    void onClientPutInServer(edict_t *client_entity, const char *player_name);
    PLUGIN_RESULT onClientCommand(edict_t *client_entity, const CCommand &args);
    void onClientSettingsChanged(edict_t *client_entity);

public:
    void onServerActivate(edict_t *edict_list, int edict_count, int client_count_max);

public:
    void onGameFrame(bool simulating);

public:
    PLUGIN_RESULT onNetworkIDValidated(const char *client_name, const char *network_id);

public:
    void onQueryCvarValueFinished(
        QueryCvarCookie_t query_cvar_cookie,
        edict_t *client_entity, EQueryCvarValueStatus query_cvar_value_status,
        const char *cvar_name, const char *cvar_value
    );

public:
    void onEdictAllocated(edict_t *edict);
    void onEdictFreed(const edict_t *edict);

public:
    void onFireGameEvent(KeyValues *event_data);
}; // class Shim


} // namespace sourced::plugin::shim


#endif // SOURCED_PLUGIN_SHIM_H_
