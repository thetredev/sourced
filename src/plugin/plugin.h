#ifndef SOURCED_PLUGIN_H_
#define SOURCED_PLUGIN_H_


// ========= DEFINES =========
#ifndef PLUGIN_VERSION
#define PLUGIN_VERSION "dev"
#endif // PLUGIN_VERSION


// ========= INCLUDES =========
// HL2SDK
#include <eiface.h>
#include <igameevents.h>
#include <engine/iserverplugin.h>


// ========= PLUGIN INTERFACE DECLARATION =========
namespace sourced::plugin {


typedef struct {
    int client_command_index;
    IServerGameDLL *server_game_dll;
} SourceEngineData;


typedef struct {
    const char *name;
    const char *author;
} Info;


const char *format_version_string(const Info &info);


// Declare plugin data interface
class Data {
private:
    Data() = default;

public:
    ~Data() = default;

public:
    Data(int client_command_index, IServerGameDLL *server_game_dll, const char *name, const char *author);

public:
    SourceEngineData source_engine;
    Info info;
};


// Declare Plugin interface
class Plugin:
    public IServerPluginCallbacks,
    public IGameEventListener {
public:
    Plugin() = default;

public:
    ~Plugin() = default;

public:
    virtual bool Load(CreateInterfaceFn interface_factory, CreateInterfaceFn game_server_factory);
    virtual void Unload(void);
    virtual void Pause(void);
    virtual void UnPause(void);

public:
    virtual int GetCommandIndex();
    virtual const char *GetPluginDescription(void);

public:
    virtual void SetCommandClient(int index);

public:
    virtual void LevelInit(const char *level_name);
    virtual void LevelShutdown(void);

public:
    virtual PLUGIN_RESULT ClientConnect(
        bool *client_allowed,
        edict_t *client_entity, const char *client_name, const char *client_address,
        char *reject, int max_reject_len
    );
    virtual void ClientActive(edict_t *client_entity);
    virtual void ClientDisconnect(edict_t *client_entity);
    virtual void ClientPutInServer(edict_t *client_entity, const char *player_name);
    virtual PLUGIN_RESULT ClientCommand(edict_t *client_entity, const CCommand &args);
    virtual void ClientSettingsChanged(edict_t *client_entity);

public:
    virtual void ServerActivate(edict_t *edict_list, int edict_count, int client_count_max);

public:
    virtual void GameFrame(bool simulating);

public:
    virtual PLUGIN_RESULT NetworkIDValidated(const char *client_name, const char *network_id);

public:
    virtual void OnQueryCvarValueFinished(
        QueryCvarCookie_t query_cvar_cookie,
        edict_t *client_entity, EQueryCvarValueStatus query_cvar_value_status,
        const char *cvar_name, const char *cvar_value
    );

public:
    virtual void OnEdictAllocated(edict_t *edict);
    virtual void OnEdictFreed(const edict_t *edict);

public:
    virtual void FireGameEvent(KeyValues *event_data);

private:
    Data *m;
}; // class Plugin


} // namespace sourced::plugin


#endif // SOURCED_PLUGIN_H_
