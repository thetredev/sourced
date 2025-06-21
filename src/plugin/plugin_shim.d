module sourced.plugin.shim;

import std.stdio : writeln;

// D bindings for Source Engine types
extern(C++) struct edict_t; // @suppress(dscanner.style.phobos_naming_convention)
extern(C++) struct KeyValues;
extern(C++) struct CCommand;

extern(C++) enum PLUGIN_RESULT {
    PLUGIN_CONTINUE = 0,
    PLUGIN_OVERRIDE,
    PLUGIN_STOP
}

alias QueryCvarCookie_t = int;

extern(C++) enum EQueryCvarValueStatus {
    eQueryCvarValueStatus_ValueIntact = 0,
    eQueryCvarValueStatus_CvarNotFound = 1,
    eQueryCvarValueStatus_NotACvar = 2,
    eQueryCvarValueStatus_CvarProtected = 3
}

extern(C++, "sourced", "plugin", "shim") {
    extern(C++) class Shim {
    public:
        this() {
            writeln("Shim::constructor");
        }

        ~this() {
            writeln("Shim::destructor");
        }

        // Lifecycle methods
        bool load() {
            writeln("Shim::load");
            return true;
        }

        void unload() {
            writeln("Shim::unload");
        }

        void pause() {
            writeln("Shim::pause");
        }

        void unPause() {
            writeln("Shim::unPause");
        }

        // Level events
        void onLevelInit(const(char)* level_name) {
            writeln("Shim::onLevelInit");
        }

        void onLevelShutdown() {
            writeln("Shim::onLevelShutdown");
        }

        // Client events
        PLUGIN_RESULT onClientConnect(
            bool* client_allowed,
            edict_t* client_entity, const(char)* client_name, const(char)* client_address,
            char* reject, int max_reject_len
        ) {
            writeln("Shim::onClientConnect");
            return PLUGIN_RESULT.PLUGIN_CONTINUE;
        }

        void onClientActive(edict_t* client_entity) {
            writeln("Shim::onClientActive");
        }

        void onClientDisconnect(edict_t* client_entity) {
            writeln("Shim::onClientDisconnect");
        }

        void onClientPutInServer(edict_t* client_entity, const(char)* player_name) {
            writeln("Shim::onClientPutInServer");
        }

        PLUGIN_RESULT onClientCommand(edict_t* client_entity, ref const(CCommand) args) {
            writeln("Shim::onClientCommand");
            return PLUGIN_RESULT.PLUGIN_CONTINUE;
        }

        void onClientSettingsChanged(edict_t* client_entity) {
            writeln("Shim::onClientSettingsChanged");
        }

        // Server events
        void onServerActivate(edict_t* edict_list, int edict_count, int client_count_max) {
            writeln("Shim::onServerActivate");
        }

        // Game events
        void onGameFrame(bool simulating) {
            //writeln("Shim::onGameFrame");
        }

        // Network events
        PLUGIN_RESULT onNetworkIDValidated(const(char)* client_name, const(char)* network_id) {
            writeln("Shim::onNetworkIDValidated");
            return PLUGIN_RESULT.PLUGIN_CONTINUE;
        }

        // Query events
        void onQueryCvarValueFinished(
            QueryCvarCookie_t query_cvar_cookie,
            edict_t* client_entity, EQueryCvarValueStatus query_cvar_value_status,
            const(char)* cvar_name, const(char)* cvar_value
        ) {
            writeln("Shim::onQueryCvarValueFinished");
        }

        // Entity events
        void onEdictAllocated(edict_t* edict) {
            //writeln("Shim::onEdictAllocated");
        }

        void onEdictFreed(const(edict_t)* edict) {
            //writeln("Shim::onEdictFreed");
        }

        // Game events
        void onFireGameEvent(KeyValues* event_data) {
            writeln("Shim::onFireGameEvent");
        }
    }
}
