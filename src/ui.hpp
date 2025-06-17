#pragma once
#include <Geode/ui/Popup.hpp>
using namespace geode::prelude;

class MyPopup : public geode::Popup<> {
protected:
    CCLabelBMFont* m_label = nullptr;
    CCLabelBMFont* m_descLabel = nullptr;

    bool setup() override {
        this->setTitle("Welcome!");

        m_descLabel = CCLabelBMFont::create("Useless mod.", "bigFont.fnt");
        m_descLabel->setPosition({200, 180});
        m_mainLayer->addChild(m_descLabel);

        m_label = CCLabelBMFont::create("Most useless thing", "bigFont.fnt");
        m_label->setPosition({200, 130});
        m_mainLayer->addChild(m_label);

        auto menu = CCMenu::create();
        menu->setPosition({0, 0});
        m_mainLayer->addChild(menu);

        auto buttonSprite = ButtonSprite::create(" Useless Mod ");
        buttonSprite->setScale(0.9f);
        buttonSprite->setZOrder(0);
        buttonSprite->setContentSize({150.f, buttonSprite->getContentSize().height});

        auto button = CCMenuItemSpriteExtra::create(
            buttonSprite,
            buttonSprite,
            this,
            menu_selector(MyPopup::onButtonClick)
        );
        button->setPosition({20, 200}); // top left corner inside 400x240 popup
        menu->addChild(button);

        return true;
    }

    void onButtonClick(CCObject*) {
        this->setTitle("Useless Mod");
        if (m_label) {
            m_label->setString("Useless Mod");
        }
        if (m_descLabel) {
            m_descLabel->setString("die");
        }
    }

public:
    static MyPopup* create() {
        auto ret = new MyPopup();
        if (ret->initAnchored(400.f, 240.f)) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }
};
