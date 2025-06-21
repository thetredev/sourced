// ========= INCLUDES =========
// HL2SDK
#include <eiface.h>
#include <tier0/icommandline.h>

// Plugin
// #include "globals/globals.h"
// #include "hooks/hooks.h"
// #include "hooks/get_tick_interval.h"
// #include "utils/binary_utils.h"
// #include "utils/io_utils.h"
#include "plugin.h"


// memdbgon must be the last include file in a .cpp file!!!
// ... for some reason
#include <tier0/memdbgon.h>


// ========= PLUGIN INTERFACE DEFINITIONS =========
namespace sourced::plugin {


const char *format_version_string(const Info &info) {
    (void)info;
    return "some formatted version string";
}


Data::Data(int client_command_index, IServerGameDLL *server_game_dll, const char *name, const char *author)
    : source_engine{
        .client_command_index = client_command_index,
        .server_game_dll = server_game_dll,
    }, info{
        .name = name,
        .author = author,
    }, shim{}
{}


// ========= PLUGIN INTERFACE IMPLEMENTATION =========

// Prepare plugin execution
bool Plugin::Load(CreateInterfaceFn /*interface_factory*/, CreateInterfaceFn /*game_server_factory*/) {
    // // ensure availability of external dependencies
    // if (system("which strings > /dev/null 2>&1") != 0) {
    //     // print an error message
    //     Error("[%s] Cannot find the 'strings' command. Please install the 'binutils' package on the system.\n", m.name);

    //     // indicate to SRCDS that the plugin couldn't load
    //     return false;
    // }

    // // get the current game dir
    // const char *game_dir = CommandLine()->ParmValue("-game", "hl2"); // stolen from Metamod Source `InitMainStates`

    // // get the current ServerGameDLL interface version
    // const char *servergamedll_interface_version = utils::binary::get_servergamedll_interface_version(
    //     game_dir, Msg, m.name, INTERFACEVERSION_SERVERGAMEDLL
    // );
    // m.server_game_dll = static_cast<IServerGameDLL*>(game_server_factory(servergamedll_interface_version, NULL));

    // // abort if we couldn't find a reference to the current ServerGameDLL instance
    // if (!m.server_game_dll)
    // {
    //     // print an error message
    //     Error(
    //         "[%s] Failed to get a pointer on ServerGameDLL. Expected: %s, Got: NULL\n",
    //         m.name, servergamedll_interface_version
    //     );

    //     // indicate to SRCDS that the plugin couldn't load
    //     return false;
    // }

    m = new Data(0, nullptr, "Plugin Name", "Plugin Version");

    // print a message
    //Msg("[%s] Loaded successfully!\n", m->info.name);

    // indicate to SRCDS that the plugin loaded successfully
    return m->shim.load();
}

// Cleanup
void Plugin::Unload(void) {
    //Msg("[%s] Unloading...\n", m->info.name);
    m->shim.unload();
    delete m;
}

void Plugin::Pause(void) {
    m->shim.pause();
}

void Plugin::UnPause(void) {
    m->shim.unPause();
}

// This string is returned when `plugin_print` is typed into the SRCDS console
const char *Plugin::GetPluginDescription(void) {
    return format_version_string(m->info);
}

int Plugin::GetCommandIndex() {
    return m->source_engine.client_command_index;
}

void Plugin::SetCommandClient(int index) {
    m->source_engine.client_command_index = index;
}

void Plugin::LevelInit(const char *level_name) {
    m->shim.onLevelInit(level_name);
}

void Plugin::LevelShutdown(void) {
    m->shim.onLevelShutdown();
}

PLUGIN_RESULT Plugin::ClientConnect(
    bool *client_allowed,
    edict_t *client_entity, const char *client_name, const char *client_address,
    char *reject, int max_reject_len
) {
    return m->shim.onClientConnect(client_allowed, client_entity, client_name, client_address, reject, max_reject_len);
}

PLUGIN_RESULT Plugin::ClientCommand(edict_t *client_entity, const CCommand &args) {
    return m->shim.onClientCommand(client_entity, args);
}

void Plugin::ClientActive(edict_t *client_entity) {
    m->shim.onClientActive(client_entity);
}

void Plugin::ClientDisconnect(edict_t *client_entity) {
    m->shim.onClientDisconnect(client_entity);
}

void Plugin::ClientPutInServer(edict_t *client_entity, const char *player_name) {
    m->shim.onClientPutInServer(client_entity, player_name);
}

void Plugin::ClientSettingsChanged(edict_t *client_entity) {
    m->shim.onClientSettingsChanged(client_entity);
}

void Plugin::ServerActivate(edict_t *edict_list, int edict_count, int client_count_max) {
    m->shim.onServerActivate(edict_list, edict_count, client_count_max);
}

void Plugin::GameFrame(bool simulating) {
    m->shim.onGameFrame(simulating);
}

PLUGIN_RESULT Plugin::NetworkIDValidated(const char *client_name, const char *network_id) {
    return m->shim.onNetworkIDValidated(client_name, network_id);
}

void Plugin::OnQueryCvarValueFinished(
    QueryCvarCookie_t query_cvar_cookie,
    edict_t *client_entity, EQueryCvarValueStatus query_cvar_value_status,
    const char *cvar_name, const char *cvar_value
) {
    m->shim.onQueryCvarValueFinished(
        query_cvar_cookie, client_entity,
        query_cvar_value_status, cvar_name, cvar_value
    );
}

void Plugin::OnEdictAllocated(edict_t *edict) {
    m->shim.onEdictAllocated(edict);
}

void Plugin::OnEdictFreed(const edict_t *edict) {
    m->shim.onEdictFreed(edict);
}

void Plugin::FireGameEvent(KeyValues *event_data) {
    m->shim.onFireGameEvent(event_data);
}

} // namespace sourced::plugin
