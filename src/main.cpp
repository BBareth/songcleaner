#include <Geode/Geode.hpp>
#include <Geode/modify/GameLevelManager.hpp>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

using namespace geode::prelude;

static void appendIdsFromCsv(std::string const& csv, std::vector<int>& out) {
    std::stringstream ss(csv);
    std::string tok;
    while (std::getline(ss, tok, ',')) {
        try {
            int id = std::stoi(tok);
            if (id > 0) out.push_back(id);
        } catch (...) {}
    }
}

// Custom (Newgrounds / music-library) song IDs referenced by a level.
// Built-in audio tracks have m_songID == 0 and are excluded automatically.
static std::vector<int> collectSongIDs(GJGameLevel* level) {
    std::vector<int> ids;
    if (level->m_songID > 0) ids.push_back(level->m_songID);
    appendIdsFromCsv(std::string(level->m_songIDs), ids);
    return ids;
}

static bool levelReferencesSong(GJGameLevel* lvl, int songID) {
    if (!lvl) return false;
    if (lvl->m_songID == songID) return true;
    std::stringstream ss(std::string(lvl->m_songIDs));
    std::string tok;
    while (std::getline(ss, tok, ',')) {
        try {
            if (std::stoi(tok) == songID) return true;
        } catch (...) {}
    }
    return false;
}

static bool songStillReferenced(int songID, GJGameLevel* deleted) {
    auto glm = GameLevelManager::sharedState();
    if (auto online = glm->m_onlineLevels) {
        for (auto [_, lvl] : CCDictionaryExt<int, GJGameLevel*>(online)) {
            if (lvl == deleted) continue;
            if (levelReferencesSong(lvl, songID)) return true;
        }
    }
    if (auto llm = LocalLevelManager::get(); llm && llm->m_localLevels) {
        for (auto lvl : CCArrayExt<GJGameLevel*>(llm->m_localLevels)) {
            if (lvl == deleted) continue;
            if (levelReferencesSong(lvl, songID)) return true;
        }
    }
    return false;
}

class $modify(SongCleanerGLM, GameLevelManager) {
    void deleteLevel(GJGameLevel* level) {
        auto mod = Mod::get();
        bool enabled = mod->getSettingValue<bool>("enabled");
        bool skipShared = mod->getSettingValue<bool>("ignore-shared");

        if (!enabled || !level) {
            GameLevelManager::deleteLevel(level);
            return;
        }

        // Snapshot song IDs before deletion — level may be released after.
        std::vector<int> candidates = collectSongIDs(level);

        // Let the game perform its normal deletion first so the level is gone
        // from the dictionaries we scan below.
        GameLevelManager::deleteLevel(level);

        if (candidates.empty()) return;

        auto mdm = MusicDownloadManager::sharedState();
        std::unordered_set<int> seen;
        for (int id : candidates) {
            if (!seen.insert(id).second) continue;
            if (skipShared && songStillReferenced(id, level)) {
                log::info("song {} still referenced by another level, keeping it", id);
                continue;
            }
            if (!mdm->isSongDownloaded(id)) continue;
            mdm->deleteSong(id);
            log::info("deleted song {} (was used by removed level)", id);
        }
    }
};
