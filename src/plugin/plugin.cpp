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
    }
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
    Msg("[%s] Loaded successfully!\n", m->info.name);

    // indicate to SRCDS that the plugin loaded successfully
    return true;
}

// Cleanup
void Plugin::Unload(void) {
    Msg("[%s] Unloading...\n", m->info.name);
    delete m;
}

// This string is returned when `plugin_print` is typed into the SRCDS console
const char *Plugin::GetPluginDescription(void) {
    return format_version_string(m->info);
}

// ========= PLUGIN INTERFACE STUB =========
// These are required, but the stub implementations are fine
// for this plugin's use case. Defined here to make the plugin compile.
int Plugin::GetCommandIndex() {
    return m->source_engine.client_command_index;
}

void Plugin::SetCommandClient(int index) {
    m->source_engine.client_command_index = index;
}

PLUGIN_RESULT Plugin::ClientConnect(bool *, edict_t *, const char *, const char *, char *, int) {
    return PLUGIN_CONTINUE;
}

PLUGIN_RESULT Plugin::ClientCommand(edict_t *, const CCommand &) {
    return PLUGIN_CONTINUE;
}

PLUGIN_RESULT Plugin::NetworkIDValidated(const char *, const char *) {
    return PLUGIN_CONTINUE;
}

// These are optional. Defined here to make the plugin compile.
void Plugin::Pause(void) {}
void Plugin::UnPause(void) {}
void Plugin::LevelInit(const char *) {}
void Plugin::ServerActivate(edict_t *, int, int) {}
void Plugin::LevelShutdown(void) {}
void Plugin::ClientActive(edict_t *) {}
void Plugin::ClientDisconnect(edict_t *) {}
void Plugin::ClientPutInServer(edict_t *, const char *) {}
void Plugin::ClientSettingsChanged(edict_t *) {}
void Plugin::OnQueryCvarValueFinished(QueryCvarCookie_t, edict_t *, EQueryCvarValueStatus, const char *, const char *) {}
void Plugin::OnEdictAllocated(edict_t *) {}
void Plugin::OnEdictFreed(const edict_t *) {}
void Plugin::FireGameEvent(KeyValues *) {}
void Plugin::GameFrame(bool) {}

} // namespace sourced::plugin
