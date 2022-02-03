#ifndef INCLUDE_API_H
#define INCLUDE_API_H

#include "Consts.h"
#include <algorithm>
#include <chrono>
#include <deque>
#include <string>
#include <vector>

#define FWD_DECLARE_JSON_SERIALIZABLE() \
    std::string toString();             \
    void fromString(const std::string &str);

// Generic game error type; represents an error that is surfacable
// to a user.
struct GameError {
    GameError(std::string err) : error(err) {}
    const std::string &what() const { return error; }
    std::string toString() const;
    std::string error;
};

namespace API {
    // Represents the state of a player in a particular session
    struct PlayerState {
        PlayerState() : session(""), name(""), score(0), win_count(0), connected(true) {}
        std::string session;
        std::string name;
        int score;
        int win_count;
        bool connected;

        FWD_DECLARE_JSON_SERIALIZABLE()
    };

    // Represents the state of one game / room
    struct GameState {
        std::vector<PlayerState> players;
        std::deque<std::string> chatLog;
        uint turn_index = 0;
        int rolls[DICE_COUNT];
        bool used[DICE_COUNT];
        bool rolled = false;
        bool victory = false;
        bool privateSession;

        FWD_DECLARE_JSON_SERIALIZABLE()
    };

    // PlayerState, but with secrets left out
    struct PlayerStateSanitized : PlayerState {
        PlayerStateSanitized() : PlayerState() {}
        PlayerStateSanitized(const PlayerState &state) : PlayerState(state) {
            if (state.session.find("guest:") != 0) user_id = state.session;
        }
        std::string user_id;
        FWD_DECLARE_JSON_SERIALIZABLE()
    };

    // Initial msg sent to clients when they connect
    struct Welcome : GameState {
        Welcome(const GameState &state) : GameState(state) {
            players.reserve(state.players.size());
            std::copy(state.players.begin(), state.players.end(), std::back_inserter(players));
        }
        std::vector<PlayerStateSanitized> players;
        int id;
        FWD_DECLARE_JSON_SERIALIZABLE()
    };

    // Client should redirect to the specified room
    struct Redirect {
        std::string room;

        FWD_DECLARE_JSON_SERIALIZABLE()
    };

    // Another player reconnected
    struct Reconnect {
        int id;

        FWD_DECLARE_JSON_SERIALIZABLE()
    };

    struct Room {
        std::string code;
        std::string host_name;
        std::string last_updated;
        int player_count;

        FWD_DECLARE_JSON_SERIALIZABLE()
    };

    // A list of public rooms available
    struct Room_List {
        std::vector<Room> rooms;

        FWD_DECLARE_JSON_SERIALIZABLE()
    };
} // namespace API

#endif