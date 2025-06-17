#pragma once
#include <Geode/ui/Popup.hpp>
using namespace geode::prelude;

class MyPopup : public geode::Popup<> {
protected:
    CCLabelBMFont* m_label = nullptr;

    bool setup() override {
        this->setTitle("useless mod");

        m_label = CCLabelBMFont::create("useless mod", "bigFont.fnt");
        m_label->setPosition({160, 140});
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
        button->setPosition({160, 40});
        menu->addChild(button);

        return true;
    }

    void onButtonClick(CCObject*) {
        this->setTitle("Useless Mod");
        if (m_label) {
            m_label->setString("Useless Mod");
        }
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
