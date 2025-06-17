#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

class $modify(MyMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) {
            return false;
        }

        log::debug("Hello from my MenuLayer::init hook! This layer has {} children.", this->getChildrenCount());

        auto sprite = CCSprite::create("spritw.png");
        if (!sprite) {
            log::error("Failed to load custom sprite: my_button.png");
            return true;
        }

        auto myButton = CCMenuItemSpriteExtra::create(
            sprite,
            this,
            menu_selector(MyMenuLayer::onMyButton)
        );

        auto menu = typeinfo_cast<CCMenu*>(this->getChildByID("bottom-menu"));
        if (!menu) {
            log::error("bottom-menu not found or not a CCMenu");
            return true;
        }

        myButton->setID("my-button"_spr);
        myButton->setPosition(menu->getContentSize().width / 2, 50);

        menu->addChild(myButton);
        menu->updateLayout();

        return true;
    }

    void onMyButton(CCObject*) {
        FLAlertLayer::create("Geode", "Hello from my custom mod!", "OK")->show();
    }
};

