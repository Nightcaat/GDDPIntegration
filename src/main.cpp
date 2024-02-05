//geode header
#include <Geode/Geode.hpp>

//other headers
#include <Geode/cocos/include/ccTypes.h>

#include <Geode/modify/CreatorLayer.hpp>

#include <Geode/binding/LevelListLayer.hpp>
#include <Geode/binding/LevelBrowserLayer.hpp>
#include <Geode/binding/GJSearchObject.hpp>
#include <Geode/binding/GJListLayer.hpp>
#include <Geode/binding/BoomListView.hpp>
#include <Geode/binding/LoadingCircle.hpp>

#include <Geode/Enums.hpp>

#include <Geode/loader/Loader.hpp>

#include <Geode/ui/BasedButton.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>
#include <Geode/ui/ListView.hpp>
#include <Geode/binding/ListCell.hpp>

#include <Geode/utils/web.hpp>
#include <Geode/utils/JsonValidation.hpp>

//dplayer callback
#include "DPLayer.hpp"

//geode namespace
using namespace geode::prelude;

/*
List IDs:

[UF] = Unofficial

Main:
25409 - Beginner
25408 - Bronze
25406 - Silver
25403 - Gold
25399 - Amber
24994 - Platinum
24993 - Sapphire
24991 - Jade
24989 - Emerald
24986 - Ruby
24721 - Diamond
24711 - Onyx
24702 - Amethyst
24694 - Azurite
24687 - Obsidian

Legacy:
? - Beginner
? - Bronze
? - Silver
? - Gold
? - Amber
161032 - Platinum
160741 - Sapphire
160743 - Jade
160745 - Emerald
? - Ruby
? - Diamond
? - Onyx
? - Amethyst
? - Azurite
? - Obsidian

Bonus:
? - 12DoC 2023
? - Minigame
? - #GD10
? - 12DoC 2022
? - Nine Circles
? - Memory I
? - Memory II
? - Memory III
? - 2 Player Solo
? - Invisible
? - 1.9 GDPS
? - XXL
? - Speedrun
? - Trusta
? - Temple
? - Coder
? - Kria
? - AeonAir
? - GDDL 1
? - GDDL 2
? - Apocalyptic Trilogy
? - Plasma
? - RGB
? - Sonic
? - Lightwave
? - Sonic Wave

Monthly:
? - Current

*/

//add button to creator layer
class $modify(CreatorLayer) {

	static void onModify(auto& self) {
        self.setHookPriority("CreatorLayer::init", -42);
    }

	bool init() {
        if (!CreatorLayer::init()) return false;

		auto menu = this->getChildByID("creator-buttons-menu");

		auto spr = CCSprite::create("DP_demonProgressionBtn_001.png"_spr);
        if (Loader::get()->isModLoaded("capeling.goodbye_unnecessary_buttons")) {
			spr->setScale(0.85f);
		} else {
			spr->setScale(0.8f);
		}

		auto dpBtn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(DPLayer::callback));
		dpBtn->setPosition({ 451, 153 });
		dpBtn->setID("demon-progression-button");
		menu->addChild(dpBtn);
		
		if (Loader::get()->isModLoaded("capeling.goodbye_unnecessary_buttons")) {
			menu->getChildByID("quests-button")->setPositionX(45.8);
			menu->getChildByID("daily-button")->setPositionX(135.4);
			menu->getChildByID("weekly-button")->setPositionX(225);
			menu->getChildByID("gauntlets-button")->setPositionX(314.6);
			menu->getChildByID("demon-progression-button")->setPositionX(406.2);
		} else {
			menu->getChildByID("map-button")->setPositionX(2.9);
			menu->getChildByID("daily-button")->setPositionX(91.5);
			menu->getChildByID("weekly-button")->setPositionX(181);
			menu->getChildByID("event-button")->setPositionX(270.5);
			menu->getChildByID("gauntlets-button")->setPositionX(360.1);
		}
		
		if (Loader::get()->isModLoaded("cvolton.betterinfo")) {
			log::info("{}", "BetterInfo Detected. Moved Button.");
			this->getChildByID("cvolton.betterinfo/center-right-menu")->setPositionY(260);
		}

        return true;
    }

};

//Main DP Layer
void DPLayer::callback(CCObject*) {
	auto scene = CCScene::create(); // creates the scene
	auto dpLayer = DPLayer::create(); //creates the layer

	scene->addChild(dpLayer);

	CCDirector::sharedDirector()->pushScene(CCTransitionFade::create(0.5f, scene)); // push transition
}

void DPLayer::keyBackClicked() {
	CCDirector::sharedDirector()->popSceneWithTransition(0.5f, PopTransition::kPopTransitionFade);
}

void DPLayer::backButton(CCObject*) {
	keyBackClicked();
}

DPLayer* DPLayer::create() {
	auto pRet = new DPLayer();
	if (pRet && pRet->init()) {
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet); //don't crash if it fails
	return nullptr;
}

void DPLayer::infoCallback(CCObject*) {
	FLAlertLayer::create(nullptr, "GDDP Info", "OK", nullptr, "Beat the required number of levels in each pack to get a special rank and move on to the next pack.\n\nMod Developer: Minemaker0430\nGDDP Creator: Trusta\nSpecial Thanks: FireMario211 (Firee), HJFod, The GDP Discord Server, & The Geode Discord Server")->show();
}

void DPLayer::soonCallback(CCObject*) {
	FLAlertLayer::create("Coming Soon!", "This feature hasn't been implemented yet but will be in the future!", "OK")->show();
}

void DPLayer::openList(CCObject* sender) {
	//FLAlertLayer::create("the", "bingle bong", "OK")->show();
	auto btn = static_cast<CCMenuItemSpriteExtra*>(sender);
	auto id = btn->getTag();

	auto searchObj = GJSearchObject::create(SearchType::LevelListsOnClick, std::to_string(id));

	auto scene = CCScene::create();
	//scene->addChild(LevelListLayer::create(list));
	scene->addChild(LevelBrowserLayer::create(searchObj));
	CCDirector::sharedDirector()->pushScene(CCTransitionFade::create(0.5f, scene)); // push transition
	//auto list = GJLevelList::create();
	//list->m_listID = id;
}

bool DPLayer::init() {
	if (!CCLayer::init()) return false;

        log::info("{}", "Opened the Demon Progression menu.");

        //auto testMenu = CCMenu::create();

        auto menu = CCMenu::create();
        auto director = CCDirector::sharedDirector();
	    auto size = director->getWinSize();

	    auto bg = createLayerBG();
        this->addChild(bg);

	    auto lCornerSprite = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
	    lCornerSprite->setAnchorPoint({ 0, 0 });
	    this->addChild(lCornerSprite);

	    auto rCornerSprite = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
	    rCornerSprite->setAnchorPoint({ 1, 0 });
	    rCornerSprite->setPosition({ size.width - 70, 0 });
	    rCornerSprite->setScaleX(-1);
	    this->addChild(rCornerSprite);

        this->addChild(menu);

		//back button
		auto backSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
		auto backButton = CCMenuItemSpriteExtra::create(backSprite, this, menu_selector(DPLayer::backButton));
		auto backMenu = CCMenu::create();
		backMenu->addChild(backButton);
		backMenu->setPosition({25, size.height - 25});
		this->addChild(backMenu);

		//list bg
		auto listMiddle = CCLayerColor::create({194, 114, 62, 255});
		auto listLeft = CCSprite::createWithSpriteFrameName("GJ_table_side_001.png");
		auto listRight = CCSprite::createWithSpriteFrameName("GJ_table_side_001.png");
		auto listTop = CCSprite::createWithSpriteFrameName("GJ_table_top02_001.png");
		auto listBottom = CCSprite::createWithSpriteFrameName("GJ_table_bottom_001.png");

		listMiddle->setPosition({100, 40});
		listMiddle->setContentSize({370, 230});

		listLeft->setPosition({101, 155});
		listRight->setPosition({469, 155});
		listTop->setPosition({size.width / 2, 280});
		listBottom->setPosition({size.width / 2, 30});

		listRight->setScaleX(-1);
		listRight->setScaleY(3.6f);
		listLeft->setScaleY(3.6f);

		listLeft->setZOrder(9);
		listRight->setZOrder(9);
		listTop->setZOrder(10);
		listBottom->setZOrder(10);

		this->addChild(listMiddle);
		this->addChild(listLeft);
		this->addChild(listRight);
		this->addChild(listTop);
		this->addChild(listBottom);

		//LoadingCircle* loadCircle = LoadingCircle::create();
		//loadCircle->setParentLayer(this);
		//loadCircle->setID("loading");
        //loadCircle->show();

		// download data
		auto mainFile = web::fetch("https://raw.githubusercontent.com/Minemaker0430/gddp-mod-database/main/main-list.json");
		if (!mainFile) {
			FLAlertLayer::create("ERROR", "Something went wrong getting the List Data.", "OK")->show(); 
		} else {
			std::string response = mainFile.value();
			matjson::Value jsonData = matjson::parse(response);
			
			//FLAlertLayer::create("Data gotten", jsonData["main"][0]["sprite"].as_string(), "OK")->show();
			
			auto mainPacks = jsonData["main"].as_array();
			
			//setup main cells
			auto mainListCells = CCArray::create();
			for (int i = 0; i < sizeof(mainPacks) + 3; i++) {
				
				auto name = jsonData["main"][i]["name"].as_string();
				auto sprite = jsonData["main"][i]["sprite"].as_string();
				auto plusSprite = jsonData["main"][i]["plusSprite"].as_string();
				auto listID = jsonData["main"][i]["listID"].as_int();
				auto reqLevels = jsonData["main"][i]["reqLevels"].as_int();
				
				auto fullTitle = name + " Demons";
				auto fullSprite = sprite + ".png";
				auto fullPlusSprite = plusSprite + ".png";
				
				CCNode* cell = ListCell::create();
				
				CCNode* packText = CCLabelBMFont::create(fullTitle.c_str(), "bigFont.fnt");
				packText->setScale(0.65f);
				packText->setAnchorPoint({0, 1});
				packText->setPosition({53, 49});
				
				CCNode* packSpr = CCSprite::create(Mod::get()->expandSpriteName(fullSprite.c_str()));
				packSpr->setScale(0.15f);
				packSpr->setAnchorPoint({0, 0});
				packSpr->setPosition({5, 1});
				
				//CCNode* packPlusSpr = CCSprite::create(Mod::get()->expandSpriteName(fullPlusSprite.c_str()));
				
				/*auto packProgressBack = CCSprite::create("GJ_progressBar_001.png");
				packProgressBack->setAnchorPoint({0, 0.5});
				packProgressBack->setPosition({53, 15});
				packProgressBack->setScaleX(0.48f);
				packProgressBack->setScaleY(0.64f);
				packProgressBack->setColor({0, 0, 0});
				
				auto packProgressFront = CCSprite::create("GJ_progressBar_001.png");
				packProgressFront->setAnchorPoint({0, 0.5});
				packProgressFront->setPosition({3.74f, 10});
				packProgressFront->setScaleX(0.975f);
				packProgressFront->setScaleY(0.76f);
				packProgressFront->setColor({255, 84, 50});
				
				CCNode* packProgressText = CCLabelBMFont::create("0/13", "bigFont.fnt");
				packProgressText->setPosition({170, 11});
				packProgressFront->setScaleX(1);
				packProgressFront->setScaleY(0.65f);*/

				auto cellMenu = CCMenu::create();
				cellMenu->setID("cell-menu");
				cellMenu->setPosition({0, 0});
				auto viewSpr = extension::CCScale9Sprite::create("GJ_button_01.png");
				viewSpr->setPosition({35, 16});
				viewSpr->setContentSize({66, 30});
				auto viewText = CCLabelBMFont::create("View", "bigFont.fnt");
				viewText->setPosition({32, 16});
				viewText->setScale(0.6f);
				auto viewBtn = CCMenuItemSpriteExtra::create(viewSpr, this, menu_selector(DPLayer::openList));
				viewBtn->setPosition({320, 25});
				viewBtn->setTag(listID);
				viewSpr->addChild(viewText);
				cellMenu->addChild(viewBtn);
				//cellMenu->setHandlerPriority(-1);

				//packProgressBack->addChild(packProgressFront);
				//packProgressBack->addChild(packProgressText);
				cell->addChild(cellMenu);
				cell->addChild(packText);
				cell->addChild(packSpr);
				//cell->addChild(packProgressBack);
				mainListCells->addObject(cell);
			};
		
			//main list
			CCNode* mainListMenu = ListView::create(mainListCells, 50.0, 358.0, 220.0);
			mainListMenu->setAnchorPoint({ 0.5, 0.5 });
			mainListMenu->setPosition({ 104.5, 44.0 });
			mainListMenu->setID("main-list-menu");
			//mainListMenu->setVisible(true);
			this->addChild(mainListMenu);			
		};
		
		//extra buttons
		auto achievementBtnSprite = CCSprite::createWithSpriteFrameName("GJ_achBtn_001.png");
		auto leaderboardsBtnSprite = CCSprite::createWithSpriteFrameName("GJ_statsBtn_001.png");
		auto monthlyPackBtnSprite = CCSprite::createWithSpriteFrameName("GJ_rateDiffBtnMod_001.png");
		auto leaderboardButton = CCMenuItemSpriteExtra::create(leaderboardsBtnSprite, this, menu_selector(DPLayer::soonCallback));
		auto achievementButton = CCMenuItemSpriteExtra::create(achievementBtnSprite, this, menu_selector(DPLayer::soonCallback));
		auto monthlyPackButton = CCMenuItemSpriteExtra::create(monthlyPackBtnSprite, this, menu_selector(DPLayer::soonCallback));
		achievementButton->setPosition({ size.width - 30, 30 });
		leaderboardButton->setPosition({ size.width - 30, 80 });
		monthlyPackButton->setPosition({ size.width - 80, 30 });
		auto extrasMenu = CCMenu::create();
		extrasMenu->setPosition({ 0, 0 });
		extrasMenu->addChild(leaderboardButton);
		extrasMenu->addChild(achievementButton);
		extrasMenu->addChild(monthlyPackButton);
		//this->addChild(extrasMenu);

		//list tabs
		auto listTabs = CCMenu::create();
		listTabs->setID("list-tabs");
		
		auto mainPacksBtn = TabButton::create("Main", this, menu_selector(DPLayer::soonCallback));
		mainPacksBtn->setPosition(-136.f, 133.5f);
		mainPacksBtn->setTag(static_cast<int>(DPListType::Main));
		listTabs->addChild(mainPacksBtn);

		auto legacyPacksBtn = TabButton::create("Legacy", this, menu_selector(DPLayer::soonCallback));
		legacyPacksBtn->setPosition(-45.f, 133.5f);
		legacyPacksBtn->setTag(static_cast<int>(DPListType::Legacy));
		listTabs->addChild(legacyPacksBtn);

		auto bonusPacksBtn = TabButton::create("Bonus", this, menu_selector(DPLayer::soonCallback));
		bonusPacksBtn->setPosition(45.f, 133.5f);
		bonusPacksBtn->setTag(static_cast<int>(DPListType::Bonus));
		listTabs->addChild(bonusPacksBtn);
		
		auto monthlyPacksBtn = TabButton::create("Monthly", this, menu_selector(DPLayer::soonCallback));
		monthlyPacksBtn->setPosition(136.f, 133.5f);
		monthlyPacksBtn->setTag(static_cast<int>(DPListType::Monthly));
		listTabs->addChild(monthlyPacksBtn);
		
		//this->addChild(listTabs);

		this->setKeyboardEnabled(true);
		this->setKeypadEnabled(true);
		
		//load data
        /*web::AsyncWebRequest()
            .fetch("https://raw.githubusercontent.com/Minemaker0430/gddp-mod-database/main/main-list")
            .text()
            .then([](std::string const& packs) {
				//static_cast<CCArray*>(packs);
				//generateMainList(packs);
				CCNode* packData = static_cast<CCArray*>(packs);
				FLAlertLayer::create("Data", packs, "OK")->show();
                geode::log::info("{}", "Main Pack Data Loaded.");
            })
            .expect([](std::string const& error) {
                FLAlertLayer::create("ERROR", "Something went wrong, oops!", "OK")->show(); 
            });*/

        return true;
}

/*void DPLayer::generateMainList(CCArray* packs) {
	auto mainListCells = CCArray::create();
	for (int i = 0; i < 9; i++) {
		CCNode* cell = ListCell::create();
		CCNode* packText = CCLabelBMFont::create("Beginner Demons", "bigFont.fnt");
		CCNode* packSpr = CCSprite::create("DP_Beginner.png"_spr);
		packSpr->setScale(0.175f)
		//CCNode* packText = CCLabelBMFont::create(static_cast<std::string>(mainData[i][0]) + " Demons", "bigFont.fnt");
		//CCNode* packSpr = CCSprite::create(static_cast<std::string>(mainData[i][1]) + ".png"_spr);
		cell->addChild(packText);
		cell->addChild(packSpr);
		mainListCells->addObject(cell);
	}
		
	//main list
	CCNode* mainListMenu = ListView::create(mainListCells, 50.0, 358.0, 220.0);
	mainListMenu->setAnchorPoint({ 0.5, 0.5 });
	mainListMenu->setPosition({ 104.5, 44.0 });
	mainListMenu->setID("main-list-menu");
	//mainListMenu->setVisible(true);
	this->addChild(mainListMenu);
}*/

/*void DPLayer::onTab(CCObject* pSender) {
    if (pSender) {
        g_tab = static_cast<ModListType>(pSender->getTag());
    }
    this->reloadList(false);

    auto toggleTab = [this](CCMenuItemToggler* member) -> void {
        auto isSelected = member->getTag() == static_cast<int>(g_tab);
        auto targetMenu = isSelected ? m_topMenu : m_menu;
        member->toggle(isSelected);
        if (member->getParent() != targetMenu) {
            member->retain();
            member->removeFromParent();
            targetMenu->addChild(member);
            member->release();
        }
        if (isSelected && m_tabsGradientStencil)
            m_tabsGradientStencil->setPosition(member->m_onButton->convertToWorldSpace({0.f, 0.f}));
    };

    toggleTab(m_downloadTabBtn);
    toggleTab(m_installedTabBtn);
    toggleTab(m_featuredTabBtn);
}*/

DPLayer::~DPLayer() {
    this->removeAllChildrenWithCleanup(true);
}