#pragma once
#include <Geode/ui/Popup.hpp>
using namespace geode::prelude;

class MyPopup : public geode::Popup<> {
protected:
    bool setup() override {
        this->setTitle("Useless Mod");

        auto label = CCLabelBMFont::create("useless mod", "bigFont.fnt");
        m_mainLayer->addChildAtPosition(label, Anchor::Center);

        return true;
    }

public:
    static MyPopup* create() {
        auto ret = new MyPopup();
        if (ret->initAnchored(240.f, 160.f)) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }
};
