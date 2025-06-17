#pragma once

#include <Geode/ui/Popup.hpp>

using namespace geode::prelude;

class PythonPopup : public geode::Popup<> {
protected:
    CCLabelBMFont* titleLabel;
    CCLabelBMFont* contentLabel;

    bool setup() override {
        this->setTitle("Python Tutorial");

        titleLabel = CCLabelBMFont::create("Select a Chapter", "bigFont.fnt");
        titleLabel->setPosition({240.f, 270.f});
        m_mainLayer->addChild(titleLabel);

        contentLabel = CCLabelBMFont::create("", "goldFont.fnt", 400.f, kCCTextAlignmentLeft);
        contentLabel->setPosition({240.f, 160.f});
        contentLabel->limitLabelWidth(400.f, 1.f, 0.1f);
        m_mainLayer->addChild(contentLabel);

        auto menu = CCMenu::create();
        menu->setPosition({0, 0});

        for (int i = 1; i <= 10; ++i) {
            auto label = CCLabelBMFont::create(("Ch. " + std::to_string(i)).c_str(), "bigFont.fnt");
            auto button = CCMenuItemLabel::create(label, this, menu_selector(PythonPopup::onChapter));
            button->setTag(i);
            float x = 60.f + ((i - 1) % 5) * 90.f;
            float y = 70.f - ((i - 1) / 5) * 50.f;
            button->setPosition({x, y});
            menu->addChild(button);
        }

        // Close button
        auto closeLabel = CCLabelBMFont::create("Close", "bigFont.fnt");
        auto closeButton = CCMenuItemLabel::create(closeLabel, this, menu_selector(PythonPopup::onClose));
        closeButton->setPosition({440.f, 30.f});
        menu->addChild(closeButton);

        m_mainLayer->addChild(menu);
        return true;
    }

    void onChapter(CCObject* sender) {
        int chapter = static_cast<CCNode*>(sender)->getTag();
        switch (chapter) {
            case 1:
                titleLabel->setString("Chapter 1: Variables");
                contentLabel->setString("Variables store data:\nx = 5\ny = 'Hello'");
                break;
            case 2:
                titleLabel->setString("Chapter 2: Data Types");
                contentLabel->setString("Common types:\nint, float, str, list, dict");
                break;
            case 3:
                titleLabel->setString("Chapter 3: Control Flow");
                contentLabel->setString("if, else, for, while:\nif x > 5:\n  print(x)");
                break;
            case 4:
                titleLabel->setString("Chapter 4: Functions");
                contentLabel->setString("def greet(name):\n  return 'Hi ' + name");
                break;
            case 5:
                titleLabel->setString("Chapter 5: Lists");
                contentLabel->setString("fruits = ['apple', 'banana']\nprint(fruits[0])");
                break;
            case 6:
                titleLabel->setString("Chapter 6: Dictionaries");
                contentLabel->setString("person = {'name': 'Alice', 'age': 30}\nprint(person['name'])");
                break;
            case 7:
                titleLabel->setString("Chapter 7: Classes");
                contentLabel->setString("class Dog:\n  def bark(self):\n    print('Woof!')");
                break;
            case 8:
                titleLabel->setString("Chapter 8: Modules");
                contentLabel->setString("import math\nprint(math.sqrt(16))");
                break;
            case 9:
                titleLabel->setString("Chapter 9: File I/O");
                contentLabel->setString("with open('file.txt') as f:\n  data = f.read()");
                break;
            case 10:
                titleLabel->setString("Chapter 10: Exceptions");
                contentLabel->setString("try:\n  x = 1 / 0\nexcept ZeroDivisionError:\n  print('Error')");
                break;
        }
    }

    void onClose(CCObject*) {
        this->dismiss();
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
