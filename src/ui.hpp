#pragma once
#include <Geode/ui/Popup.hpp>
using namespace geode::prelude;

class PythonPopup : public geode::Popup<> {
protected:
    CCLabelBMFont* m_contentLabel = nullptr;

    bool setup() override {
        this->setTitle("Useless Mod - Learn Python");

        m_contentLabel = CCLabelBMFont::create(
            "Select a chapter to learn Python!",
            "bigFont.fnt"
        );
        m_mainLayer->addChildAtPosition(m_contentLabel, Anchor::Center + Vec2(0, 50));

        auto menu = CCMenu::create();
        menu->setPosition(Vec2::ZERO);

        const float startX = 60.f;
        const float startY = 220.f;
        const float spacingY = 25.f;

        for (int i = 1; i <= 10; i++) {
            std::string chapterName = "Chapter " + std::to_string(i);
            auto label = CCLabelBMFont::create(chapterName.c_str(), "bigFont.fnt");
            auto btn = CCMenuItemLabel::create(label, this, menu_selector(PythonPopup::onChapterSelected));
            btn->setPosition(Vec2(startX, startY - spacingY * (i - 1)));
            btn->setTag(i);
            menu->addChild(btn);
        }

        auto closeBtn = CCMenuItemLabel::create(
            CCLabelBMFont::create("Close", "bigFont.fnt"),
            this,
            menu_selector(PythonPopup::onClose)
        );
        closeBtn->setPosition(Vec2(480 - 60, 320 - 40));
        menu->addChild(closeBtn);

        m_mainLayer->addChild(menu);

        return true;
    }

    void onChapterSelected(CCObject* sender) {
        auto btn = static_cast<CCMenuItemLabel*>(sender);
        int chapter = btn->getTag();

        static const char* lessons[] = {
            "",
            "Chapter 1: Introduction to Python\nPython is a versatile language...",
            "Chapter 2: Variables and Types\nLearn how to store data...",
            "Chapter 3: Control Structures\nIf statements and loops...",
            "Chapter 4: Functions\nReusable blocks of code...",
            "Chapter 5: Lists and Tuples\nCollections of data...",
            "Chapter 6: Dictionaries\nKey-value pairs...",
            "Chapter 7: Classes and Objects\nObject-oriented programming...",
            "Chapter 8: Modules and Packages\nOrganizing code...",
            "Chapter 9: File I/O\nReading and writing files...",
            "Chapter 10: Exception Handling\nManaging errors gracefully..."
        };

        if (chapter >= 1 && chapter <= 10) {
            m_contentLabel->setString(lessons[chapter]);
        }
    }

    void onClose(CCObject*) override {
        this->close();
    }

public:
    static PythonPopup* create() {
        auto ret = new PythonPopup();
        if (ret->initAnchored(480.f, 320.f)) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }
};
