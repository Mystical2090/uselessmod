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

        // Chapter buttons (1-10)
        for (int i = 1; i <= 10; ++i) {
            auto normalSprite = ButtonSprite::create(("Ch. " + std::to_string(i)).c_str(), "bigFont.fnt", "GJ_button_01.png");
            auto button = CCMenuItemSpriteExtra::create(
                normalSprite,
                this,
                menu_selector(PythonPopup::onChapter)
            );
            button->setTag(i);
            float x = 60.f + ((i - 1) % 5) * 90.f;
            float y = 70.f - ((i - 1) / 5) * 50.f;
            button->setPosition({x, y});
            menu->addChild(button);
        }

        // Close button with sprite for better GD integration
        auto closeSprite = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
        auto closeButton = CCMenuItemSpriteExtra::create(
            closeSprite, 
            this, 
            menu_selector(PythonPopup::onClose)
        );
        closeButton->setPosition({220.f, 140.f}); // Top right within bounds
        menu->addChild(closeButton);

        m_mainLayer->addChild(menu);
        return true;
    }

    void onChapter(CCObject* sender) {
        int chapter = static_cast<CCNode*>(sender)->getTag();
        switch (chapter) {
            case 1:
                titleLabel->setString("Chapter 1: Variables");
                contentLabel->setString("Variables store data:\nx = 5\ny = 'Hello'\nprint(x, y)");
                break;
            case 2:
                titleLabel->setString("Chapter 2: Data Types");
                contentLabel->setString("Common types:\nint: 42\nfloat: 3.14\nstr: 'text'\nlist: [1,2,3]\ndict: {'key': 'value'}");
                break;
            case 3:
                titleLabel->setString("Chapter 3: Control Flow");
                contentLabel->setString("Conditional statements:\nif x > 5:\n    print('Big')\nelse:\n    print('Small')\n\nLoops:\nfor i in range(3):\n    print(i)");
                break;
            case 4:
                titleLabel->setString("Chapter 4: Functions");
                contentLabel->setString("Define functions:\ndef greet(name):\n    return 'Hello ' + name\n\nresult = greet('Alice')\nprint(result)");
                break;
            case 5:
                titleLabel->setString("Chapter 5: Lists");
                contentLabel->setString("Working with lists:\nfruits = ['apple', 'banana']\nfruits.append('orange')\nprint(fruits[0])\nprint(len(fruits))");
                break;
            case 6:
                titleLabel->setString("Chapter 6: Dictionaries");
                contentLabel->setString("Key-value pairs:\nperson = {\n    'name': 'Alice',\n    'age': 30\n}\nprint(person['name'])\nperson['city'] = 'NYC'");
                break;
            case 7:
                titleLabel->setString("Chapter 7: Classes");
                contentLabel->setString("Object-oriented programming:\nclass Dog:\n    def __init__(self, name):\n        self.name = name\n    def bark(self):\n        print(f'{self.name} says Woof!')");
                break;
            case 8:
                titleLabel->setString("Chapter 8: Modules");
                contentLabel->setString("Import libraries:\nimport math\nimport random\n\nprint(math.sqrt(16))\nprint(random.randint(1, 10))");
                break;
            case 9:
                titleLabel->setString("Chapter 9: File I/O");
                contentLabel->setString("Reading and writing files:\nwith open('file.txt', 'w') as f:\n    f.write('Hello World')\n\nwith open('file.txt', 'r') as f:\n    data = f.read()\n    print(data)");
                break;
            case 10:
                titleLabel->setString("Chapter 10: Exceptions");
                contentLabel->setString("Handle errors gracefully:\ntry:\n    result = 10 / 0\nexcept ZeroDivisionError:\n    print('Cannot divide by zero!')\nfinally:\n    print('Done')");
                break;
        }
    }

    void onClose(CCObject*) override {
        this->setKeypadEnabled(false);
        this->removeFromParentAndCleanup(true);
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
