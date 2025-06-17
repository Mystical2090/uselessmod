#pragma once
#include <Geode/ui/Popup.hpp>
using namespace geode::prelude;

class MyPopup : public geode::Popup<> {
protected:
    bool setup() override {
        this->setTitle("Useless Mod");

        auto label = CCLabelBMFont::create("Hello from my custom mod!", "bigFont.fnt");
        m_mainLayer->addChildAtPosition(label, Anchor::Center + Vec2(0, 40));  // slightly up

        auto menu = CCMenu::create();
        menu->setPosition(Vec2::ZERO);
        m_mainLayer->addChild(menu);

        auto okSprite = CCSprite::createWithSpriteFrameName("GJ_button_01.png");
        auto okButton = CCMenuItemSpriteExtra::create(okSprite, this, menu_selector(MyPopup::onOk));
        okButton->setPosition(Vec2(120, 40));
        menu->addChild(okButton);

        auto okLabel = CCLabelBMFont::create("OK", "bigFont.fnt");
        okLabel->setPosition(okButton->getPosition());
        m_mainLayer->addChild(okLabel);

        auto cancelSprite = CCSprite::createWithSpriteFrameName("GJ_button_01.png");
        auto cancelButton = CCMenuItemSpriteExtra::create(cancelSprite, this, menu_selector(MyPopup::onCancel));
        cancelButton->setPosition(Vec2(240, 40));
        menu->addChild(cancelButton);

        auto cancelLabel = CCLabelBMFont::create("Cancel", "bigFont.fnt");
        cancelLabel->setPosition(cancelButton->getPosition());
        m_mainLayer->addChild(cancelLabel);

        return true;
    }

    void onOk(CCObject*) {
        log::info("OK button clicked");
        this->close();
    }

    void onCancel(CCObject*) {
        log::info("Cancel button clicked");
        this->close();
    }

public:
    static MyPopup* create() {
        auto ret = new MyPopup();
        if (ret->initAnchored(320.f, 180.f)) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }
};
