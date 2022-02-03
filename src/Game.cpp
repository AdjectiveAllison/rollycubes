#include <algorithm>
#include <iostream>
#include <unordered_map>

#include "Consts.h"
#include "Game.h"
#include "StringUtils.h"

bool Game::isInitialized() { return state.players.size() > 0; }
bool Game::isPrivate() const { return state.privateSession; }

bool isSignedIn(const API::PlayerState &player) {
    return (player.session.find("guest:") != 0);
}

std::string Game::hostName() const {
    if (state.players.size() < 1)
        return "unknown";
    return state.players[0].name;
}

int Game::totalRoll() {
    int total = 0;
    for (uint i = 0; i < DICE_COUNT; ++i)
        total += state.rolls[i];
    return total;
}

bool Game::isSplit() { return totalRoll() == 7; }

bool Game::isDoubles() {
    int roll1 = state.rolls[0];
    for (uint i = 1; i < DICE_COUNT; ++i) {
        if (state.rolls[i] != roll1)
            return false;
    }
    return true;
}

bool Game::hasPlayer(std::string &id) {
    for (const auto &player : state.players) {
        if (player.session == id) {
            return true;
        }
    }
    return false;
}

int Game::getPlayerId(std::string &id) {
    for (uint i = 0; i < state.players.size(); ++i) {
        if (state.players[i].session == id) {
            return i;
        }
    }
    return -1;
}

json Game::addPlayer(const PerSocketData &data) {
    json result;
    if (state.players.size() >= MAX_PLAYERS) {
        return result;
    }
    auto &player = state.players.emplace_back();
    player.session = data.session;
    if (isSignedIn(player)) {
        player.name = data.display_name;
        result["name"] = player.name;
        result["user_id"] = player.session;
    }
    result["type"] = "join";
    result["id"] = state.players.size() - 1;

    // This is our first player joining
    if (state.players.size() == 1) {
        clearTurn();
        turn_token = data.session;
        state.turn_index = 0;
    }
    return result;
}

void Game::clearTurn() {
    // Clear roll states
    for (uint i = 0; i < DICE_COUNT; ++i) {
        state.used[i] = false;
    }
    state.rolled = false;
}

bool Game::allUsed() {
    for (uint i = 0; i < DICE_COUNT; ++i) {
        if (!state.used[i])
            return false;
    }
    return true;
}

void Game::advanceTurn() {
    clearTurn();
    if (state.players.size() == 1) {
        turn_token = state.players[0].session;
        state.turn_index = 0;
    } else {
        state.turn_index = (state.turn_index + 1) % state.players.size();
        turn_token = state.players[state.turn_index].session;
    }
}

json Game::disconnectPlayer(std::string id) {
    json result;
    for (uint i = 0; i < state.players.size(); ++i) {
        if (state.players[i].session == id) {
            state.players[i].connected = false;
            result["type"] = "disconnect";
            result["id"] = i;
            break;
        }
    }
    return result;
}

bool Game::isPlayerConnected(std::string id) const {
    for (uint i = 0; i < state.players.size(); ++i) {
        if (state.players[i].session == id) {
            return state.players[i].connected;
        }
    }
    return false;
}

json Game::reconnectPlayer(std::string id) {
    json result;
    for (uint i = 0; i < state.players.size(); ++i) {
        if (state.players[i].session == id) {
            state.players[i].connected = true;
            result["type"] = "reconnect";
            result["id"] = i;
            break;
        }
    }
    return result;
}

int Game::connectedPlayerCount() {
    int count = 0;
    for (const auto &player : state.players) {
        if (player.connected)
            ++count;
    }
    return count;
}

#define ACTION(x)                 \
    {                             \
#x, std::mem_fn(&Game::x) \
    }

static const std::unordered_map<
    std::string, std::function<void(Game *, SendFunc, SendFunc, json &,
                                    const std::string &)>>
    action_map = {ACTION(chat), ACTION(leave), ACTION(kick),
                  ACTION(restart), ACTION(update_name), ACTION(roll),
                  ACTION(add), ACTION(sub), ACTION(add_nth),
                  ACTION(sub_nth)};

#undef ACTION

void Game::handleMessage(HANDLER_ARGS) {
    if (!data["type"].is_string())
        throw GameError("Type is not specified correctly");
    auto action_type = data["type"].get<std::string>();
    auto it = action_map.find(action_type);
    if (it != action_map.end()) {
        it->second(this, send, broadcast, data, session);
    } else {
        throw GameError("Unknown action type");
    }
    updated = std::chrono::system_clock::now();
}

void Game::chat(HANDLER_ARGS) {
    json res;
    if (!data["msg"].is_string())
        throw GameError("Message must be a string");
    for (uint i = 0; i < state.players.size(); ++i) {
        if (state.players[i].session == session) {
            res["type"] = data["type"];
            auto msg = data["msg"].get<std::string>();
            std::string name = state.players[i].name;
            if (name == "") {
                name = "User" + std::to_string(i + 1);
            }
            std::string fullMsg = name + ": " + msg;
            if (fullMsg.length() > MAX_CHAT_LEN) {
                fullMsg = trimString(fullMsg, MAX_CHAT_LEN, true);
            }
            state.chatLog.push_front(fullMsg);
            if (state.chatLog.size() > MAX_CHAT_LOG) {
                state.chatLog.pop_back();
            }
            res["msg"] = fullMsg;
            broadcast(res.dump());
            return;
        }
    }
};

void Game::leave(HANDLER_ARGS) {
    json res;
    res["type"] = "leave";
    for (uint i = 0; i < state.players.size(); ++i) {
        if (state.players[i].session == session) {
            res["id"] = i;
            if (turn_token == state.players[i].session) {
                advanceTurn();
                json turn;
                turn["type"] = "update_turn";
                turn["id"] = state.turn_index;
                broadcast(turn.dump());
            }
            state.players.erase(state.players.begin() + i);
            broadcast(res.dump());
            break;
        }
    }
}

void Game::kick(HANDLER_ARGS) {
    if (!data["id"].is_number())
        throw GameError("pass a number please");
    uint id = data["id"].get<uint>();
    if (state.players.empty())
        throw GameError("uhhh this should never happen");
    if (id >= state.players.size())
        throw GameError("out of bounds");

    json res;
    res["type"] = "kick";
    res["id"] = id;
    if (turn_token == state.players[id].session) {
        advanceTurn();
        if (state.turn_index > id) {
            state.turn_index--;
        }
        json turn;
        turn["type"] = "update_turn";
        turn["id"] = state.turn_index;
        broadcast(turn.dump());
    } else if (state.turn_index > id) {
        state.turn_index--;
    }
    state.players.erase(state.players.begin() + id);
    broadcast(res.dump());
}

void Game::restart(HANDLER_ARGS) {
    if (state.players.empty())
        throw GameError("uhhh this should never happen");
    if (!state.victory)
        throw GameError("game still in progress");

    json res;
    for (auto &player : state.players)
        player.score = 0;
    state.victory = false;
    advanceTurn();
    res["type"] = "restart";
    res["id"] = state.turn_index;
    broadcast(res.dump());
}

void Game::update_name(HANDLER_ARGS) {
    if (!data["name"].is_string())
        throw GameError("name must be a string");
    std::string name = data["name"].get<std::string>();
    for (uint i = 0; i < state.players.size(); ++i) {
        if (state.players[i].session == session) {
            state.players[i].name = name;
            json msg;
            msg["type"] = "update_name";
            msg["name"] = state.players[i].name;
            msg["id"] = i;
            broadcast(msg.dump());
            return;
        }
    }
}
void Game::guardUpdate(const std::string &session) {
    if (state.victory)
        throw GameError("game is over");
    if (session != turn_token)
        throw GameError("not your turn");
    if (!state.rolled)
        throw GameError("you need to roll first");
}

void Game::roll(HANDLER_ARGS) {
    if (state.victory)
        throw GameError("game is over");
    if (session != turn_token)
        throw GameError("not your turn");
    if (state.rolled)
        throw GameError("now's not the time for that");
    if (state.players.size() <= 1)
        throw GameError("invite some friends first!");
    json resp;
    resp["type"] = "roll";
    for (uint i = 0; i < DICE_COUNT; ++i) {
        std::random_device rd;
        std::mt19937 gen(rd());
        state.rolls[i] = dis(gen);
        resp["rolls"].push_back(state.rolls[i]);
    }
    state.rolled = true;
    broadcast(resp.dump());
}

void Game::add(HANDLER_ARGS) {
    guardUpdate(session);
    if (isSplit())
        throw GameError("you must add invdividually");
    data = json(totalRoll());
    update(send, broadcast, data, session);
}

void Game::sub(HANDLER_ARGS) {
    guardUpdate(session);
    if (isSplit())
        throw GameError("you must add invdividually");
    data = json(-totalRoll());
    update(send, broadcast, data, session);
}

int Game::guardNth(const json &data) {
    if (!isSplit())
        throw GameError("This is not a split");
    if (!data.is_number())
        throw GameError("NaN");
    uint n = data.get<uint>();
    if (n >= DICE_COUNT)
        throw GameError("too high");
    if (state.used[n])
        throw GameError("already spent");
    state.used[n] = true;
    return n;
}

void Game::add_nth(HANDLER_ARGS) {
    guardUpdate(session);
    int n = guardNth(data["n"]);
    data = json(state.rolls[n]);
    update(send, broadcast, data, session);
}

void Game::sub_nth(HANDLER_ARGS) {
    guardUpdate(session);
    int n = guardNth(data["n"]);
    data = json(-state.rolls[n]);
    update(send, broadcast, data, session);
}

void Game::update(HANDLER_ARGS) {
    int change = data.get<int>();
    json res;
    uint i;
    for (i = 0; i < state.players.size(); ++i) {
        if (state.players[i].session == session)
            break;
    }
    int score = state.players[i].score += change;
    res["type"] = "update";
    res["score"] = score;
    res["used"] = state.used;
    res["id"] = i;
    broadcast(res.dump());
    if (!isSplit() || allUsed()) {
        if (!isDoubles()) {
            if (TARGET_SCORES.count(score)) {
                // WIN CONDITION
                json win;
                state.players[i].win_count++;
                win["type"] = "win";
                win["id"] = i;
                state.victory = true;
                broadcast(win.dump());
            } else {
                for (uint i = 0; i < state.players.size(); ++i) {
                    if (session == state.players[i].session) {
                        continue;
                    }
                    if (score == state.players[i].score) {
                        state.players[i].score = 0;
                        json update;
                        update["type"] = "update";
                        update["id"] = i;
                        update["score"] = 0;
                        update["reset"] = true;
                        broadcast(update.dump());
                        break;
                    }
                }
                advanceTurn();
                json turn;
                turn["type"] = "update_turn";
                turn["id"] = state.turn_index;
                broadcast(turn.dump());
            }
        } else {
            for (uint i = 0; i < state.players.size(); ++i) {
                if (session == state.players[i].session) {
                    continue;
                }
                if (score == state.players[i].score) {
                    state.players[i].score = 0;
                    json update;
                    update["type"] = "update";
                    update["id"] = i;
                    update["score"] = 0;
                    broadcast(update.dump());
                    break;
                }
            }
            clearTurn();
            json rollAgain;
            rollAgain["type"] = "roll_again";
            send(rollAgain.dump());
        }
    }
}
