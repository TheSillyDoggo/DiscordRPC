#include <Geode/Geode.hpp>

#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/CreatorLayer.hpp>
#include <Geode/modify/LevelSearchLayer.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>

#include <discord_register.h>
#include <discord_rpc.h>

using namespace geode::prelude;

static const char* APPLICATION_ID = "1178492879627366472";

static void handleDiscordReady(const DiscordUser* user) {
	log::info("Connected to Discord RPC");
	log::info("Username: {}", user->username);
	log::info("UserID: {}", user->userId);
}

static void handleDiscordError(int errorCode, const char* message) {
	log::info("Discord RPC error");
	log::info("Error Code: {}", std::to_string(errorCode));
	log::info("Message: {}", std::string(message));
}

static void handleDiscordDisconnected(int errorCode, const char* message) {
	log::info("Discord RPC disconnected");
	log::info("Error Code: {}", std::to_string(errorCode));
	log::info("Message: {}", std::string(message));
}

static void initDiscordRP() {
	DiscordEventHandlers handlers;
	handlers.ready = handleDiscordReady;
	handlers.errored = handleDiscordError;
	handlers.disconnected = handleDiscordDisconnected;
	Discord_Initialize(APPLICATION_ID, &handlers, 1, "322170");
	Discord_RunCallbacks();

}

void updateDiscordRP(std::string details, std::string state, std::string smallImageKey = "", std::string smallImageText = "") {
	auto gm = GameManager::sharedState();
	DiscordRichPresence discordPresence{};
    discordPresence.details = details.c_str();
    discordPresence.state = state.c_str();
    discordPresence.largeImageKey = "gd_large";
    discordPresence.largeImageText = gm->m_playerName.c_str();
	if (smallImageKey != "") {
		discordPresence.smallImageKey = smallImageKey.c_str();
		discordPresence.smallImageText = smallImageText.c_str();
	}
    discordPresence.instance = 0;
    Discord_UpdatePresence(&discordPresence);
}

std::string convertGJDifficultyDemonToAssetKey(GJDifficulty difficulty) {
	switch (static_cast<int>(difficulty)) {
		case 1:
			return "easy_demon";
		case 2:
			return "medium_demon";
		case 3:
			return "hard_demon";
		case 4:
			return "insane_demon";
		case 5:
			return "extreme_demon";
	}
	return "na";
}

std::string convertGJDifficultyToAssetKey(GJDifficulty difficulty) {
	switch (static_cast<int>(difficulty)) {
		case -1:
			return "auto";
		case 0:
			return "na";
		case static_cast<int>(GJDifficulty::Easy):
			return "easy";
		case static_cast<int>(GJDifficulty::Normal):
			return "normal";
		case static_cast<int>(GJDifficulty::Hard):
			return "hard";
		case static_cast<int>(GJDifficulty::Harder):
			return "harder";
		case static_cast<int>(GJDifficulty::Insane):
			return "insane";
	}
	return "na";
}

std::string getAssetKey(int stars, GJDifficulty difficulty) {
	if (stars == 0) {
		return convertGJDifficultyToAssetKey(difficulty);
	}
	if (stars == 10) {
		return convertGJDifficultyDemonToAssetKey(difficulty);
	}

	switch (stars) {
		case 1:
			return "auto";
		case 2:
			return "easy";
		case 3:
			return "normal";
		case 4:
			return "hard";
		case 5:
			return "hard";
		case 6:
			return "harder";
		case 7:
			return "harder";
		case 8:
			return "insane";
		case 9:
			return "insane";
	}
	return "na";
}

class $modify(MenuLayer) {
	bool init() {
		if (!MenuLayer::init()) return false;
		initDiscordRP();
		updateDiscordRP("Browsing Menus", "Main Menu");

		return true;
	}
};

class $modify(CreatorLayer) {
	bool init() {
		if (!CreatorLayer::init()) return false;
		updateDiscordRP("Browsing Menus", "Creator Tab");
		return true;
	}

	void onLeaderboards(CCObject* p0) {
		updateDiscordRP("Browsing Menus", "Checking out leaderboards");
		return CreatorLayer::onLeaderboards(p0);
	}

	void onMyLevels(CCObject* p0) {
		updateDiscordRP("Browsing Menus", "Checking out created levels");
		return CreatorLayer::onMyLevels(p0);
	}

	void onSavedLevels(CCObject* p0) {
		updateDiscordRP("Browsing Menus", "Checking out saved levels");
		return CreatorLayer::onSavedLevels(p0);
	}

	void onMapPacks(CCObject* p0) {
		auto shouldBeFunny = Mod::get()->getSettingValue<bool>("funny-mode");
		std::string state = "";
		if (shouldBeFunny) {
			state = "Checking out the worst levels known to man";
		} else {
			state = "Checking out map packs";
		}
		updateDiscordRP("Browsing Menus", state);
		return CreatorLayer::onMapPacks(p0);
	}

	void onDailyLevel(CCObject* p0) {
		updateDiscordRP("Browsing Menus", "Checking out the daily level");
		return CreatorLayer::onDailyLevel(p0);
	}

	void onWeeklyLevel(CCObject* p0) {
		updateDiscordRP("Browsing Menus", "Checking out the weekly level");
		return CreatorLayer::onWeeklyLevel(p0);
	}

	void onFeaturedLevels(CCObject* p0) {
		updateDiscordRP("Browsing Menus", "Checking out the featured tab");
		return CreatorLayer::onFeaturedLevels(p0);
	}

	void onFameLevels(CCObject* p0) {
		updateDiscordRP("Browsing Menus", "Checking out the Hall of Fame");
		return CreatorLayer::onFameLevels(p0);
	}

	void onGauntlets(CCObject* p0) {
		auto shouldBeFunny = Mod::get()->getSettingValue<bool>("funny-mode");
		std::string state = "";
		if (shouldBeFunny) {
			state = "Checking out the better map packs";
		} else {
			state = "Checking out the gauntlets";
		}
		updateDiscordRP("Browsing Menus", state);
		return CreatorLayer::onGauntlets(p0);
	}
};

class $modify(LevelSearchLayer) {
	bool init() {
		if (!LevelSearchLayer::init()) return false;
		updateDiscordRP("Browsing Menus", "Search Tab");
		return true;
	}
};

class $modify(LevelInfoLayer) {
	bool init(GJGameLevel* level) {
		if (!LevelInfoLayer::init(level)) return false;
		// testing stuff, this works properly now
		///
		// log::info(std::to_string(
		// 	static_cast<int>(level->getAverageDifficulty())
		// ));
		// log::info(std::to_string(
		// 	static_cast<int>(level->m_difficulty)
		// ));
		// log::info(std::to_string(
		// 	static_cast<int>(level->m_demonDifficulty)
		// ));
		// // TODO: ignore all todos below and just use the goddamn star count. only use m_difficulty for unrated levels
		// // TODO: Determine if Easy and Normal rated level are not Easy/Medium demons somehow
		// // TODO: Then I need to add the N/A difficulty face
		// // TODO: And finally I need to distinguish between demons and non-demons
		// // ... all of this for a 512px/512px circle face on texting platform
		
		// log::info(std::to_string(level->m_stars.value()));

		bool isRated = level->m_stars.value() != 0;
		
		updateDiscordRP(
			"Viewing Level",
			std::string(level->m_levelName) + " by " + std::string(level->m_creatorName),
			getAssetKey(level->m_stars.value(), level->getAverageDifficulty()),
			(isRated) ? "Rated" : "Not Rated"
		);
		return true;
	}
};

class $modify(MyLevelEditorLayer, LevelEditorLayer) {
	bool init(GJGameLevel* level) {
		if (!LevelEditorLayer::init(level)) return false;

		MyLevelEditorLayer::updateStatus();

		return true;
	}

	void updateStatus() {
		auto shouldShowSensitive = Mod::get()->getSettingValue<bool>("private-info");
		std::string details = "";
		if (shouldShowSensitive) {
			details = "Working on level \"" + std::string(m_level->m_levelName) + "\"";
		} else {
			details = "Working on a level";
		}
		updateDiscordRP(details, std::to_string(m_level->m_objectCount.value()) + " objects as of opening the editor");
	}

	GameObject* createObject(int objectID, CCPoint pos, bool p2) {
		auto object = LevelEditorLayer::createObject(objectID, pos, p2);

		updateStatus();

		return object;
	}

	void removeObject(GameObject* object, bool p1) {
		LevelEditorLayer::removeObject(object, p1);
		updateStatus();
	}
};

class $modify(MyPlayLayer, PlayLayer) {
	bool init(GJGameLevel* level) {
		if (!PlayLayer::init(level)) return false;

		MyPlayLayer::updateRP();

		return true;
	}

	void showNewBest(bool p0, int p1, int p2, bool p3, bool p4, bool p5) {
		PlayLayer::showNewBest(p0, p1, p2, p3, p4, p5);

		MyPlayLayer::updateRP();
	}

	void resetLevel() {
		PlayLayer::resetLevel();
		MyPlayLayer::updateRP();
	}

	void levelComplete() {
		PlayLayer::levelComplete();
		MyPlayLayer::updateRP();
	}

	void updateRP() {
		bool isRated = m_level->m_stars.value() != 0;
		auto shouldShowSensitive = Mod::get()->getSettingValue<bool>("private-info");

		std::string state;

		if (m_level->m_isUploaded || shouldShowSensitive) {
			state = std::string(m_level->m_levelName) + " by " + std::string(m_level->m_creatorName) + " (" + std::to_string(m_level->m_normalPercent.value()) + "%)";
		} else if (!shouldShowSensitive) {
			state = "Playtesting a created level";
		}

		updateDiscordRP(
			"Playing Level",
			state,
			getAssetKey(m_level->m_stars.value(), m_level->getAverageDifficulty()),
			(isRated) ? "Rated" : "Not Rated"
		);
	}
};