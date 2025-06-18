#pragma once

#include <Geode/ui/Popup.hpp>
#include <Geode/Geode.hpp>
#include <Geode/ui/ScrollLayer.hpp>

using namespace geode::prelude;

class AnswerPopup : public geode::Popup<> {
protected:
    std::string answerText;

    bool setup(std::string const& answer) override {
        answerText = answer;
        
        this->setTitle("Solution");
        
        auto scrollBg = CCScale9Sprite::create("square02b_001.png");
        scrollBg->setContentSize({480.f, 280.f});
        scrollBg->setPosition({240.f, 150.f});
        scrollBg->setColor({245, 245, 245});
        scrollBg->setOpacity(220);
        m_mainLayer->addChild(scrollBg);

        auto scrollLayer = ScrollLayer::create({470.f, 270.f});
        scrollLayer->setPosition({5.f, 15.f});
        
        auto contentLayer = CCLayer::create();
        
        auto answerLabel = CCLabelBMFont::create(answerText.c_str(), "chatFont.fnt", 450.f, kCCTextAlignmentLeft);
        answerLabel->setPosition({235.f, 135.f});
        answerLabel->setScale(0.6f);
        answerLabel->setAnchorPoint({0.5f, 0.5f});
        answerLabel->setColor({0, 128, 0});
        contentLayer->addChild(answerLabel);
        
        auto contentSize = answerLabel->getContentSize();
        float totalHeight = std::max(270.f, contentSize.height * 0.6f + 50.f);
        scrollLayer->m_contentLayer->setContentSize({470.f, totalHeight});
        contentLayer->setContentSize({470.f, totalHeight});
        
        answerLabel->setPosition({235.f, totalHeight / 2});
        
        scrollLayer->m_contentLayer->addChild(contentLayer);
        m_mainLayer->addChild(scrollLayer);

        auto closeSprite = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
        closeSprite->setScale(0.8f);
        auto closeButton = CCMenuItemSpriteExtra::create(
            closeSprite, 
            this, 
            menu_selector(AnswerPopup::onClose)
        );
        closeButton->setPosition({450.f, 280.f});
        
        auto closeMenu = CCMenu::create();
        closeMenu->setPosition({0, 0});
        closeMenu->addChild(closeButton);
        m_mainLayer->addChild(closeMenu);
        
        return true;
    }

    void onClose(CCObject*) override {
        this->setKeypadEnabled(false);
        this->removeFromParentAndCleanup(true);
    }

public:
    static AnswerPopup* create(std::string const& answer) {
        auto ret = new AnswerPopup();
        if (ret->initAnchored(480.f, 320.f, answer)) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }
};

class PythonPopup : public geode::Popup<> {
protected:
    CCLabelBMFont* titleLabel;
    CCLabelBMFont* contentLabel;
    CCLabelBMFont* missionLabel;
    ScrollLayer* scrollLayer;
    CCLayer* contentLayer;
    CCLayer* backgroundLayer;
    CCMenu* chapterMenu;
    CCMenu* actionMenu;
    CCLabelBMFont* pageLabel;
    CCMenuItemSpriteExtra* prevButton;
    CCMenuItemSpriteExtra* nextButton;
    CCMenuItemSpriteExtra* showAnswerButton;
    CCLabelBMFont* m_currentNotification;
    
    int currentPage = 0;
    int currentChapter = 0;
    std::string currentAnswer = "";
    static const int chaptersPerPage = 8;
    static const int totalChapters = 20;

    bool setup() override {
        createAnimatedBackground();
        
        this->setTitle("Interactive Python Learning");
        titleLabel = CCLabelBMFont::create("🐍 Interactive Python Tutorial", "chatFont.fnt");
        titleLabel->setPosition({280.f, 330.f});
        titleLabel->setScale(0.8f);
        titleLabel->setColor({46, 125, 50});
        m_mainLayer->addChild(titleLabel);

        auto subtitleLabel = CCLabelBMFont::create("Learn by doing - Complete missions and see solutions!", "chatFont.fnt");
        subtitleLabel->setPosition({280.f, 310.f});
        subtitleLabel->setScale(0.5f);
        subtitleLabel->setColor({100, 100, 100});
        m_mainLayer->addChild(subtitleLabel);

        auto scrollBg = CCScale9Sprite::create("square02b_001.png");
        scrollBg->setContentSize({530.f, 190.f});
        scrollBg->setPosition({280.f, 170.f});
        scrollBg->setColor({245, 245, 245});
        scrollBg->setOpacity(220);
        m_mainLayer->addChild(scrollBg);

        scrollLayer = ScrollLayer::create({520.f, 180.f});
        scrollLayer->setPosition({20.f, 80.f});
        
        contentLayer = CCLayer::create();
    
        contentLabel = CCLabelBMFont::create("🚀 Welcome to Interactive Python Programming!\n\nPython is the world's most popular programming language.\nPerfect for beginners, powerful for experts.\n\n📚 Each chapter includes:\n  • Clear explanations\n  • Hands-on missions\n  • Instant solutions\n\n👆 Select any chapter below to start your journey!", "chatFont.fnt", 500.f, kCCTextAlignmentLeft);
        contentLabel->setPosition({260.f, 90.f});
        contentLabel->setScale(0.65f);
        contentLabel->setAnchorPoint({0.5f, 0.5f});
        contentLabel->setColor({60, 60, 60});
        contentLayer->addChild(contentLabel);

        missionLabel = CCLabelBMFont::create("", "chatFont.fnt", 500.f, kCCTextAlignmentLeft);
        missionLabel->setPosition({260.f, 50.f});
        missionLabel->setScale(0.6f);
        missionLabel->setAnchorPoint({0.5f, 0.5f});
        missionLabel->setColor({0, 77, 153});
        missionLabel->setVisible(false);
        contentLayer->addChild(missionLabel);
        
        scrollLayer->m_contentLayer->addChild(contentLayer);
        m_mainLayer->addChild(scrollLayer);

        chapterMenu = CCMenu::create();
        chapterMenu->setPosition({0, 0});
        m_mainLayer->addChild(chapterMenu);

        actionMenu = CCMenu::create();
        actionMenu->setPosition({0, 0});

        auto answerBg = CCScale9Sprite::create("GJ_button_01.png");
        answerBg->setContentSize({120.f, 35.f});
        auto answerIcon = CCSprite::createWithSpriteFrameName("GJ_completesIcon_001.png");
        answerIcon->setScale(0.6f);
        answerIcon->setPosition({15.f, 17.5f});
        answerBg->addChild(answerIcon);
        auto answerText = CCLabelBMFont::create("Show Answer", "chatFont.fnt");
        answerText->setPosition({75.f, 17.5f});
        answerText->setScale(0.5f);
        answerBg->addChild(answerText);
        
        showAnswerButton = CCMenuItemSpriteExtra::create(
            answerBg,
            this,
            menu_selector(PythonPopup::onShowAnswer)
        );
        showAnswerButton->setPosition({280.f, 40.f});
        showAnswerButton->setVisible(false);
        actionMenu->addChild(showAnswerButton);

        m_mainLayer->addChild(actionMenu);

        auto navMenu = CCMenu::create();
        navMenu->setPosition({0, 0});

        auto prevBg = CCScale9Sprite::create("GJ_button_02.png");
        prevBg->setContentSize({80.f, 30.f});
        auto prevArrow = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
        prevArrow->setFlipX(true);
        prevArrow->setScale(0.6f);
        prevArrow->setPosition({25.f, 15.f});
        prevBg->addChild(prevArrow);
        auto prevText = CCLabelBMFont::create("Prev", "chatFont.fnt");
        prevText->setPosition({55.f, 15.f});
        prevText->setScale(0.4f);
        prevBg->addChild(prevText);
        
        prevButton = CCMenuItemSpriteExtra::create(
            prevBg,
            this,
            menu_selector(PythonPopup::onPrevPage)
        );
        prevButton->setPosition({50.f, 15.f});
        navMenu->addChild(prevButton);

        auto nextBg = CCScale9Sprite::create("GJ_button_02.png");
        nextBg->setContentSize({80.f, 30.f});
        auto nextArrow = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
        nextArrow->setScale(0.6f);
        nextArrow->setPosition({55.f, 15.f});
        nextBg->addChild(nextArrow);
        auto nextText = CCLabelBMFont::create("Next", "chatFont.fnt");
        nextText->setPosition({25.f, 15.f});
        nextText->setScale(0.4f);
        nextBg->addChild(nextText);
        
        nextButton = CCMenuItemSpriteExtra::create(
            nextBg,
            this,
            menu_selector(PythonPopup::onNextPage)
        );
        nextButton->setPosition({510.f, 15.f});
        navMenu->addChild(nextButton);

        pageLabel = CCLabelBMFont::create("Page 1/3", "chatFont.fnt");
        pageLabel->setPosition({280.f, 15.f});
        pageLabel->setScale(0.5f);
        pageLabel->setColor({120, 120, 120});
        navMenu->addChild(pageLabel);

        auto closeSprite = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
        closeSprite->setScale(0.8f);
        auto closeButton = CCMenuItemSpriteExtra::create(
            closeSprite, 
            this, 
            menu_selector(PythonPopup::onClose)
        );
        closeButton->setPosition({530.f, 330.f});
        navMenu->addChild(closeButton);

        m_mainLayer->addChild(navMenu);
        
        updateChapterButtons();
        updateNavigationButtons();
        
        return true;
    }

    void createAnimatedBackground() {
        backgroundLayer = CCLayer::create();
        
        for (int i = 0; i < 3; i++) {
            auto bgSprite = CCSprite::create("square02_001.png");
            bgSprite->setScaleX(20.f);
            bgSprite->setScaleY(15.f);
            bgSprite->setPosition({280.f, 180.f});
            
            switch(i) {
                case 0:
                    bgSprite->setColor({240, 248, 255});
                    bgSprite->setOpacity(100);
                    break;
                case 1:
                    bgSprite->setColor({230, 245, 255});
                    bgSprite->setOpacity(80);
                    bgSprite->setScale(bgSprite->getScale() * 0.9f);
                    break;
                case 2:
                    bgSprite->setColor({220, 240, 255});
                    bgSprite->setOpacity(60);
                    bgSprite->setScale(bgSprite->getScale() * 0.8f);
                    break;
            }
            
            backgroundLayer->addChild(bgSprite);

            auto moveAction = CCMoveBy::create(8.0f + i * 2, {10.f, 5.f});
            auto moveBack = CCMoveBy::create(8.0f + i * 2, {-10.f, -5.f});
            auto sequence = CCSequence::create(moveAction, moveBack, nullptr);
            auto repeat = CCRepeatForever::create(sequence);
            bgSprite->runAction(repeat);
        }
        
        m_mainLayer->addChild(backgroundLayer, -1);
    }

    void updateChapterButtons() {
        chapterMenu->removeAllChildren();
        
        int startChapter = currentPage * chaptersPerPage + 1;
        int endChapter = std::min(startChapter + chaptersPerPage - 1, totalChapters);
        
        for (int i = startChapter; i <= endChapter; ++i) {
            auto buttonBg = CCScale9Sprite::create("GJ_button_05.png");
            buttonBg->setContentSize({120.f, 40.f});
            
            auto icon = CCSprite::createWithSpriteFrameName("GJ_starsIcon_001.png");
            icon->setScale(0.4f);
            icon->setPosition({20.f, 20.f});
            icon->setColor({255, 215, 0});
            buttonBg->addChild(icon);
            
            auto chapterText = CCLabelBMFont::create(("Chapter " + std::to_string(i)).c_str(), "chatFont.fnt");
            chapterText->setPosition({75.f, 25.f});
            chapterText->setScale(0.45f);
            chapterText->setColor({40, 40, 40});
            buttonBg->addChild(chapterText);
            
            std::string subtitle = getChapterSubtitle(i);
            auto subtitleText = CCLabelBMFont::create(subtitle.c_str(), "chatFont.fnt");
            subtitleText->setPosition({75.f, 12.f});
            subtitleText->setScale(0.35f);
            subtitleText->setColor({100, 100, 100});
            buttonBg->addChild(subtitleText);
            
            auto button = CCMenuItemSpriteExtra::create(
                buttonBg,
                this,
                menu_selector(PythonPopup::onChapter)
            );
            button->setTag(i);

            int buttonIndex = i - startChapter;
            float x = 80.f + (buttonIndex % 4) * 130.f;
            float y = 300.f - (buttonIndex / 4) * 50.f;
            button->setPosition({x, y});
            chapterMenu->addChild(button);
        }
    }

    std::string getChapterSubtitle(int chapter) {
        switch (chapter) {
            case 1: return "Getting Started";
            case 2: return "Store Data";
            case 3: return "User Interaction";
            case 4: return "Math & Logic";
            case 5: return "Make Decisions";
            case 6: return "Repeat Actions";
            case 7: return "Collect Items";
            case 8: return "Key-Value Pairs";
            case 9: return "Reusable Code";
            case 10: return "Text Processing";
            case 11: return "Read & Write";
            case 12: return "Handle Errors";
            case 13: return "Objects & Classes";
            case 14: return "Import Libraries";
            case 15: return "Smart Lists";
            case 16: return "Quick Functions";
            case 17: return "Code Enhancers";
            case 18: return "Web Requests";
            case 19: return "Project Setup";
            case 20: return "Best Practices";
            default: return "Learn Python";
        }
    }

    void updateNavigationButtons() {
        int totalPages = (totalChapters + chaptersPerPage - 1) / chaptersPerPage;
        
        prevButton->setVisible(currentPage > 0);
        nextButton->setVisible(currentPage < totalPages - 1);
        
        pageLabel->setString(("Page " + std::to_string(currentPage + 1) + "/" + std::to_string(totalPages)).c_str());
    }

    void onPrevPage(CCObject*) {
        if (currentPage > 0) {
            currentPage--;
            updateChapterButtons();
            updateNavigationButtons();
        }
    }

    void onNextPage(CCObject*) {
        int totalPages = (totalChapters + chaptersPerPage - 1) / chaptersPerPage;
        if (currentPage < totalPages - 1) {
            currentPage++;
            updateChapterButtons();
            updateNavigationButtons();
        }
    }

    void onShowAnswer(CCObject*) {
        if (!currentAnswer.empty()) {
            auto answerPopup = AnswerPopup::create(currentAnswer);
            answerPopup->show();
        }
    }

    void removeNotification() {
        if (m_currentNotification) {
            m_currentNotification->removeFromParent();
            m_currentNotification = nullptr;
        }
    }

    void onChapter(CCObject* sender) {
        currentChapter = static_cast<CCNode*>(sender)->getTag();
        std::string title, content, mission, answer;
        
        getChapterContent(currentChapter, title, content, mission, answer);
        currentAnswer = answer;
        
        titleLabel->setString(title.c_str());
        contentLabel->setString(content.c_str());
        missionLabel->setString(mission.c_str());
        
        missionLabel->setVisible(true);
        showAnswerButton->setVisible(true);

        auto contentSize = contentLabel->getContentSize();
        auto missionSize = missionLabel->getContentSize();
        
        float totalHeight = contentSize.height + missionSize.height + 100.f;
        scrollLayer->m_contentLayer->setContentSize({520.f, totalHeight});
        contentLayer->setContentSize({520.f, totalHeight});
        
        contentLabel->setPosition({260.f, totalHeight - contentSize.height / 2 - 25.f});
        missionLabel->setPosition({260.f, totalHeight - contentSize.height - missionSize.height / 2 - 50.f});
        
        scrollLayer->scrollToTop();
    }

    void getChapterContent(int chapter, std::string& title, std::string& content, std::string& mission, std::string& answer) {
        switch (chapter) {
            case 1:
                title = "🚀 Chapter 1: Introduction & Installation";
                content = "Welcome to Python! 🐍\n\nPython is a high-level programming language that's:\n• Easy to read and write\n• Perfect for beginners\n• Used by companies like Google, Netflix, Instagram\n• Great for web development, AI, and data science\n\nInstalling Python:\n1. Visit python.org\n2. Download the latest version (3.11+)\n3. Run installer and check 'Add Python to PATH'\n4. Verify: Open terminal, type 'python --version'\n\nYour First Program:\nprint('Hello, World!')";
                mission = "🎯 MISSION: Write Your First Program!\n\nCreate a program that prints your name and favorite hobby.\nExample output:\n  Hello, my name is Alex\n  I love coding!\n\nHint: Use the print() function twice!";
                answer = "✅ SOLUTION:\n\nprint('Hello, my name is Alex')\nprint('I love coding!')\n\n# You can also use variables:\nname = 'Alex'\nhobby = 'coding'\nprint(f'Hello, my name is {name}')\nprint(f'I love {hobby}!')";
                break;
            case 2:
                title = "📦 Chapter 2: Variables & Data Types";
                content = "Variables are containers for storing data! 📦\n\nBasic Data Types:\n• int: Whole numbers (42, -17, 0)\n• float: Decimal numbers (3.14, -2.5)\n• str: Text ('Hello', \"Python\")\n• bool: True or False\n\nCreating Variables:\nage = 25          # Integer\nheight = 5.9      # Float\nname = 'Alice'    # String\nis_student = True # Boolean\n\nPython is dynamically typed - no need to declare types!\n\nChecking Types:\nprint(type(age))    # <class 'int'>\nprint(type(name))   # <class 'str'>";
                mission = "🎯 MISSION: Create a Profile!\n\nCreate variables for:\n• Your name (string)\n• Your age (integer)\n• Your height in meters (float)\n• Whether you like pizza (boolean)\n\nThen print all variables with their types.\n\nBonus: Calculate your age in days (age * 365)!";
                answer = "✅ SOLUTION:\n\nname = 'Sarah'\nage = 22\nheight = 1.65\nlikes_pizza = True\n\nprint(f'Name: {name} (type: {type(name)})')\nprint(f'Age: {age} (type: {type(age)})')\nprint(f'Height: {height}m (type: {type(height)})')\nprint(f'Likes pizza: {likes_pizza} (type: {type(likes_pizza)})')\n\n# Bonus:\nage_in_days = age * 365\nprint(f'Age in days: {age_in_days}')";
                break;
            case 3:
                title = "💬 Chapter 3: Input & Output";
                content = "Let's make interactive programs! 💬\n\nGetting User Input:\nname = input('What is your name? ')\nage = int(input('How old are you? '))\n\nNote: input() always returns a string!\nUse int(), float(), bool() to convert.\n\nPowerful Print Formatting:\n# f-strings (modern way)\nprint(f'Hello {name}, you are {age} years old!')\n\n# .format() method\nprint('Hello {}, you are {} years old!'.format(name, age))\n\n# Multiple values\nprint('Name:', name, 'Age:', age)\n\nSpecial Characters:\n\\n = new line\n\\t = tab\n\\' = single quote\n\\\" = double quote";
                mission = "🎯 MISSION: Build a Calculator!\n\nCreate a simple calculator that:\n1. Asks for two numbers\n2. Shows their sum, difference, product, and division\n3. Formats the output nicely\n\nExample:\n  Enter first number: 10\n  Enter second number: 3\n  Results:\n    10 + 3 = 13\n    10 - 3 = 7\n    10 × 3 = 30\n    10 ÷ 3 = 3.33";
                answer = "✅ SOLUTION:\n\n# Get input\nnum1 = float(input('Enter first number: '))\nnum2 = float(input('Enter second number: '))\n\n# Calculate\nsum_result = num1 + num2\ndiff_result = num1 - num2\nproduct_result = num1 * num2\ndiv_result = num1 / num2\n\n# Display results\nprint('Results:')\nprint(f'  {num1} + {num2} = {sum_result}')\nprint(f'  {num1} - {num2} = {diff_result}')\nprint(f'  {num1} × {num2} = {product_result}')\nprint(f'  {num1} ÷ {num2} = {div_result:.2f}')";
                break;
            case 4:
                title = "🔢 Chapter 4: Math & Logic Basics";
                content = "Explore Python's math operators and logical expressions.\n\nOperators:\n• +, -, *, /\n• // (floor division), % (modulus)\n\nLogical Operators:\n• and, or, not\n\nExample:\nresult = (5 > 3) and (2 < 4)  # True";
                mission = "🎯 MISSION: Calculate & Compare\n\nWrite a program that calculates the remainder when dividing two numbers.\nThen check if the remainder is zero and print the result.";
                answer = "✅ SOLUTION:\n\nnum1 = 10\nnum2 = 3\nremainder = num1 % num2\nprint(f'Remainder: {remainder}')\nprint(f'Is divisible? {remainder == 0}')";
                break;
            case 5:
                title = "⚖️ Chapter 5: Conditional Statements";
                content = "Make decisions in your code using if, elif, and else.\n\nExample:\nif score >= 90:\n  print('Excellent')\nelif score >= 75:\n  print('Good')\nelse:\n  print('Keep trying')";
                mission = "🎯 MISSION: Grade Checker\n\nWrite a program that asks for a grade and prints a message based on the value.";
                answer = "✅ SOLUTION:\n\ngrade = int(input('Enter your grade: '))\nif grade >= 90:\n  print('Excellent')\nelif grade >= 75:\n  print('Good')\nelse:\n  print('Keep trying')";
                break;
            case 6:
                title = "🔄 Chapter 6: Loops - Repetition";
                content = "Learn how to repeat tasks with for and while loops.\n\nExample for loop:\nfor i in range(5):\n  print(i)";
                mission = "🎯 MISSION: Loop Practice\n\nWrite a program that prints all even numbers from 2 to 20.";
                answer = "✅ SOLUTION:\n\nfor num in range(2, 21, 2):\n  print(num)";
                break;
            case 7:
                title = "📦 Chapter 7: Lists & Collections";
                content = "Manage multiple items using lists.\n\nExample:\nnumbers = [1, 2, 3, 4]\nprint(numbers[0])  # 1";
                mission = "🎯 MISSION: List Manipulation\n\nCreate a list of your favorite fruits and print each one.";
                answer = "✅ SOLUTION:\n\nfruits = ['apple', 'banana', 'cherry']\nfor fruit in fruits:\n  print(fruit)";
                break;
            case 8:
                title = "🗝️ Chapter 8: Dictionaries & Key-Value Pairs";
                content = "Store data as key-value pairs.\n\nExample:\nperson = {'name': 'Alice', 'age': 30}\nprint(person['name'])";
                mission = "🎯 MISSION: Dictionary Lookup\n\nCreate a dictionary of three countries and their capitals and print each.";
                answer = "✅ SOLUTION:\n\ncountries = {'USA': 'Washington', 'France': 'Paris', 'Japan': 'Tokyo'}\nfor country, capital in countries.items():\n  print(f'{country}: {capital}')";
                break;
            case 9:
                title = "🔧 Chapter 9: Functions & Reusability";
                content = "Define reusable blocks of code with functions.\n\nExample:\ndef greet(name):\n  print(f'Hello, {name}!')";
                mission = "🎯 MISSION: Function Builder\n\nWrite a function that takes a number and returns its square.";
                answer = "✅ SOLUTION:\n\ndef square(num):\n  return num * num\n\nprint(square(5))";
                break;
            case 10:
                title = "🧩 Chapter 10: Working with Strings";
                content = "Manipulate text data using strings.\n\nExample:\nname = 'ChatGPT'\nprint(name.upper())";
                mission = "🎯 MISSION: String Formatter\n\nWrite a program that takes a user input string and prints it reversed.";
                answer = "✅ SOLUTION:\n\ntext = input('Enter text: ')\nprint(text[::-1])";
                break;
            case 11:
                title = "🔍 Chapter 11: File Handling Basics";
                content = "Read and write files to save data.\n\nExample:\nwith open('file.txt', 'w') as f:\n  f.write('Hello')";
                mission = "🎯 MISSION: File Writer\n\nWrite a program that saves a user's input to a file.";
                answer = "✅ SOLUTION:\n\ntext = input('Enter text to save: ')\nwith open('output.txt', 'w') as f:\n  f.write(text)";
                break;
            case 12:
                title = "⚙️ Chapter 12: Error Handling & Exceptions";
                content = "Make your program robust with try-except blocks.\n\nExample:\ntry:\n  x = 1 / 0\nexcept ZeroDivisionError:\n  print('Cannot divide by zero')";
                mission = "🎯 MISSION: Safe Division\n\nWrite a program that divides two numbers but handles division by zero gracefully.";
                answer = "✅ SOLUTION:\n\ntry:\n  num1 = int(input('Enter numerator: '))\n  num2 = int(input('Enter denominator: '))\n  result = num1 / num2\n  print(f'Result: {result}')\nexcept ZeroDivisionError:\n  print('Error: Cannot divide by zero')";
                break;
            case 13:
                title = "🏗️ Chapter 13: Object-Oriented Programming";
                content = "Structure your code with classes and objects.\n\nExample:\nclass Dog:\n  def __init__(self, name):\n    self.name = name";
                mission = "🎯 MISSION: Class Creation\n\nCreate a class `Car` with attributes and a method to display details.";
                answer = "✅ SOLUTION:\n\nclass Car:\n  def __init__(self, make, model):\n    self.make = make\n    self.model = model\n  def show(self):\n    print(f'{self.make} {self.model}')\n\ncar = Car('Toyota', 'Corolla')\ncar.show()";
                break;
            case 14:
                title = "📚 Chapter 14: Import Libraries";
                content = "Extend Python's functionality with modules and packages.\n\nBuilt-in Modules:\nimport math\nprint(math.sqrt(16))  # 4.0\nprint(math.pi)        # 3.14159...\n\nimport random\nprint(random.randint(1, 10))  # Random number 1-10\n\nimport datetime\nnow = datetime.datetime.now()\nprint(now.strftime('%Y-%m-%d %H:%M'))\n\nInstalling External Packages:\npip install requests\npip install numpy\n\nImport Variations:\nfrom math import sqrt, pi\nimport math as m\nfrom datetime import datetime as dt";
                mission = "🎯 MISSION: Random Generator!\n\nCreate a program that:\n1. Generates 5 random numbers between 1-100\n2. Calculates the square root of each\n3. Shows the current date and time\n\nUse the math, random, and datetime modules!";
                answer = "✅ SOLUTION:\n\nimport math\nimport random\nimport datetime\n\nprint('Random Number Generator')\nprint('=' * 25)\n\nfor i in range(5):\n    num = random.randint(1, 100)\n    sqrt_num = math.sqrt(num)\n    print(f'Number: {num}, Square root: {sqrt_num:.2f}')\n\nnow = datetime.datetime.now()\nprint(f'\\nGenerated on: {now.strftime(\"%Y-%m-%d at %H:%M:%S\")}')";
                break;
            case 15:
                title = "⚡ Chapter 15: List Comprehensions";
                content = "Create lists efficiently with elegant one-liners! ⚡\n\nBasic Syntax:\n[expression for item in iterable]\n\nExamples:\n# Traditional way\nsquares = []\nfor x in range(10):\n    squares.append(x**2)\n\n# List comprehension way\nsquares = [x**2 for x in range(10)]\n\n# With conditions\neven_squares = [x**2 for x in range(10) if x % 2 == 0]\n\n# String manipulation\nwords = ['hello', 'world', 'python']\nupper_words = [word.upper() for word in words]\n\n# Nested lists\nmatrix = [[i*j for j in range(3)] for i in range(3)]";
                mission = "🎯 MISSION: Data Transformer!\n\nGiven this list of temperatures in Celsius:\ncelsius = [0, 10, 20, 30, 40]\n\nUse list comprehensions to:\n1. Convert all to Fahrenheit (F = C * 9/5 + 32)\n2. Filter only temperatures above 50°F\n3. Create a list of temperature descriptions:\n   - Below 32°F: 'Freezing'\n   - 32-80°F: 'Moderate'\n   - Above 80°F: 'Hot'";
                answer = "✅ SOLUTION:\n\ncelsius = [0, 10, 20, 30, 40]\n\n# 1. Convert to Fahrenheit\nfahrenheit = [c * 9/5 + 32 for c in celsius]\nprint(f'Fahrenheit: {fahrenheit}')\n\n# 2. Filter above 50°F\nhot_temps = [f for f in fahrenheit if f > 50]\nprint(f'Above 50°F: {hot_temps}')\n\n# 3. Temperature descriptions\ndef get_description(f):\n    if f < 32:\n        return 'Freezing'\n    elif f <= 80:\n        return 'Moderate'\n    else:\n        return 'Hot'\n\ndescriptions = [get_description(f) for f in fahrenheit]\nprint(f'Descriptions: {descriptions}')";
                break;
            case 16:
                title = "🚀 Chapter 16: Lambda Functions";
                content = "Create quick, anonymous functions with lambda! 🚀\n\nSyntax:\nlambda arguments: expression\n\nBasic Examples:\n# Regular function\ndef square(x):\n    return x**2\n\n# Lambda equivalent\nsquare = lambda x: x**2\nprint(square(5))  # 25\n\nUseful with built-in functions:\nnumbers = [1, 2, 3, 4, 5]\nsquared = list(map(lambda x: x**2, numbers))\neven = list(filter(lambda x: x % 2 == 0, numbers))\n\n# Sorting with lambda\nstudents = [('Alice', 85), ('Bob', 90), ('Charlie', 78)]\nstudents.sort(key=lambda student: student[1])  # Sort by grade\n\n# Multiple arguments\nadd = lambda x, y: x + y\nprint(add(3, 5))  # 8";
                mission = "🎯 MISSION: Lambda Toolkit!\n\nCreate a program that uses lambda functions to:\n1. Calculate the area of circles (π * r²) for radii [1, 2, 3, 4, 5]\n2. Filter names longer than 4 characters from ['Ana', 'Bob', 'Charlie', 'Diana', 'Ed']\n3. Sort this list of products by price:\n   products = [('Laptop', 999), ('Mouse', 25), ('Keyboard', 75)]\n\nUse map(), filter(), and sort() with lambdas!";
                answer = "✅ SOLUTION:\n\nimport math\n\n# 1. Circle areas\nradii = [1, 2, 3, 4, 5]\nareas = list(map(lambda r: math.pi * r**2, radii))\nprint('Circle areas:')\nfor r, area in zip(radii, areas):\n    print(f'  Radius {r}: {area:.2f}')\n\n# 2. Filter long names\nnames = ['Ana', 'Bob', 'Charlie', 'Diana', 'Ed']\nlong_names = list(filter(lambda name: len(name) > 4, names))\nprint(f'\\nNames longer than 4 chars: {long_names}')\n\n# 3. Sort products by price\nproducts = [('Laptop', 999), ('Mouse', 25), ('Keyboard', 75)]\nproducts.sort(key=lambda product: product[1])\nprint('\\nProducts sorted by price:')\nfor name, price in products:\n    print(f'  {name}: ${price}')";
                break;
            case 17:
                title = "🎨 Chapter 17: Decorators";
                content = "Enhance functions with decorators - Python's superpowers! 🎨\n\nBasic Decorator:\ndef my_decorator(func):\n    def wrapper():\n        print('Before function')\n        func()\n        print('After function')\n    return wrapper\n\n@my_decorator\ndef say_hello():\n    print('Hello!')\n\nTiming Decorator:\nimport time\ndef timer(func):\n    def wrapper(*args, **kwargs):\n        start = time.time()\n        result = func(*args, **kwargs)\n        end = time.time()\n        print(f'{func.__name__} took {end-start:.4f} seconds')\n        return result\n    return wrapper\n\n@timer\ndef slow_function():\n    time.sleep(1)";
                mission = "🎯 MISSION: Function Enhancer!\n\nCreate decorators that:\n1. Count how many times a function is called\n2. Validate that function arguments are positive numbers\n3. Cache function results to avoid recalculation\n\nTest with a factorial function and see the magic!";
                answer = "✅ SOLUTION:\n\nfrom functools import wraps\n\n# 1. Call counter decorator\ndef call_counter(func):\n    @wraps(func)\n    def wrapper(*args, **kwargs):\n        wrapper.calls += 1\n        print(f'{func.__name__} called {wrapper.calls} times')\n        return func(*args, **kwargs)\n    wrapper.calls = 0\n    return wrapper\n\n# 2. Validation decorator\ndef validate_positive(func):\n    @wraps(func)\n    def wrapper(*args, **kwargs):\n        for arg in args:\n            if isinstance(arg, (int, float)) and arg < 0:\n                raise ValueError('Arguments must be positive')\n        return func(*args, **kwargs)\n    return wrapper\n\n# 3. Cache decorator\ndef cache(func):\n    @wraps(func)\n    def wrapper(*args):\n        if args not in wrapper.cache_dict:\n            wrapper.cache_dict[args] = func(*args)\n            print(f'Calculated {func.__name__}{args}')\n        else:\n            print(f'Retrieved from cache {func.__name__}{args}')\n        return wrapper.cache_dict[args]\n    wrapper.cache_dict = {}\n    return wrapper\n\n# Test function with all decorators\n@call_counter\n@validate_positive\n@cache\ndef factorial(n):\n    if n <= 1:\n        return 1\n    return n * factorial(n - 1)\n\n# Test\nprint(factorial(5))\nprint(factorial(5))  # From cache\nprint(factorial(6))";
                break;
            case 18:
                title = "🌐 Chapter 18: Web Requests & APIs";
                content = "Connect to the internet and fetch data from APIs! 🌐\n\nInstall requests:\npip install requests\n\nBasic GET Request:\nimport requests\n\nresponse = requests.get('https://api.github.com/users/octocat')\nif response.status_code == 200:\n    data = response.json()\n    print(data['name'])\n\nPOST Request:\ndata = {'key': 'value'}\nresponse = requests.post('https://httpbin.org/post', json=data)\n\nHeaders and Parameters:\nheaders = {'User-Agent': 'MyApp/1.0'}\nparams = {'q': 'python', 'sort': 'stars'}\nresponse = requests.get('https://api.github.com/search/repositories', \n                       headers=headers, params=params)\n\nError Handling:\ntry:\n    response = requests.get('https://api.example.com/data', timeout=5)\n    response.raise_for_status()  # Raises exception for bad status\nexcept requests.exceptions.RequestException as e:\n    print(f'Error: {e}')";
                mission = "🎯 MISSION: Weather App!\n\nCreate a weather app that:\n1. Fetches weather data from a free API (like OpenWeatherMap)\n2. Asks user for a city name\n3. Displays temperature, description, and humidity\n4. Handles errors gracefully (invalid city, no internet)\n\nNote: You'll need to sign up for a free API key!\nAlternatively, use a mock API like JSONPlaceholder for practice.";
                answer = "✅ SOLUTION:\n\nimport requests\nimport json\n\ndef get_weather(city, api_key):\n    \"\"\"Fetch weather data for a city\"\"\"\n    base_url = 'http://api.openweathermap.org/data/2.5/weather'\n    params = {\n        'q': city,\n        'appid': api_key,\n        'units': 'metric'  # Celsius\n    }\n    \n    try:\n        response = requests.get(base_url, params=params, timeout=5)\n        response.raise_for_status()\n        return response.json()\n    except requests.exceptions.RequestException as e:\n        print(f'Error fetching weather: {e}')\n        return None\n\ndef display_weather(weather_data):\n    \"\"\"Display weather information\"\"\"\n    if weather_data:\n        city = weather_data['name']\n        temp = weather_data['main']['temp']\n        description = weather_data['weather'][0]['description']\n        humidity = weather_data['main']['humidity']\n        \n        print(f'\\n🌤️  Weather in {city}:')\n        print(f'   Temperature: {temp}°C')\n        print(f'   Description: {description.title()}')\n        print(f'   Humidity: {humidity}%')\n    else:\n        print('Unable to fetch weather data')\n\n# Main program\napi_key = 'YOUR_API_KEY_HERE'  # Get from openweathermap.org\ncity = input('Enter city name: ')\n\nweather = get_weather(city, api_key)\ndisplay_weather(weather)\n\n# Alternative using JSONPlaceholder for demo:\ndef demo_api():\n    response = requests.get('https://jsonplaceholder.typicode.com/posts/1')\n    if response.status_code == 200:\n        data = response.json()\n        print(f'Post Title: {data[\"title\"]}')\n        print(f'Post Body: {data[\"body\"][:50]}...')\n\ndemo_api()";
                break;
            case 19:
                title = "📦 Chapter 19: Package Management & Virtual Environments";
                content = "Manage Python projects like a pro! 📦\n\nVirtual Environments:\n# Create virtual environment\npython -m venv myproject_env\n\n# Activate (Windows)\nmyproject_env\\Scripts\\activate\n\n# Activate (Mac/Linux)\nsource myproject_env/bin/activate\n\n# Deactivate\ndeactivate\n\nPackage Management:\npip install package_name\npip install package_name==1.2.3  # Specific version\npip install -r requirements.txt   # From file\npip freeze > requirements.txt     # Save current packages\npip list                          # Show installed packages\npip uninstall package_name        # Remove package\n\nProject Structure:\nmyproject/\n├── venv/\n├── src/\n│   └── main.py\n├── tests/\n├── requirements.txt\n├── README.md\n└── .gitignore\n\nBest Practices:\n• One virtual environment per project\n• Keep requirements.txt updated\n• Use descriptive project names";
                mission = "🎯 MISSION: Project Setup Master!\n\nCreate a complete Python project setup:\n1. Create a virtual environment named 'calculator_env'\n2. Install requests and matplotlib packages\n3. Create a requirements.txt file\n4. Write a simple script that uses both packages\n5. Create a basic project structure\n\nDocument the setup process in a README.md file!";
                answer = "✅ SOLUTION:\n\n# Terminal commands (run these in order):\n\n# 1. Create virtual environment\npython -m venv calculator_env\n\n# 2. Activate environment\n# Windows: calculator_env\\Scripts\\activate\n# Mac/Linux: source calculator_env/bin/activate\n\n# 3. Install packages\npip install requests matplotlib\n\n# 4. Save requirements\npip freeze > requirements.txt\n\n# Project files to create:\n\n# main.py\nimport requests\nimport matplotlib.pyplot as plt\nimport json\n\ndef fetch_data():\n    \"\"\"Fetch sample data from API\"\"\"\n    response = requests.get('https://jsonplaceholder.typicode.com/posts')\n    return response.json()[:5]  # First 5 posts\n\ndef create_chart(data):\n    \"\"\"Create a chart of post lengths\"\"\"\n    titles = [post['title'][:20] + '...' for post in data]\n    lengths = [len(post['body']) for post in data]\n    \n    plt.figure(figsize=(10, 6))\n    plt.bar(range(len(titles)), lengths)\n    plt.xlabel('Posts')\n    plt.ylabel('Body Length')\n    plt.title('Post Body Lengths')\n    plt.xticks(range(len(titles)), titles, rotation=45)\n    plt.tight_layout()\n    plt.show()\n\nif __name__ == '__main__':\n    data = fetch_data()\n    create_chart(data)\n\n# README.md\n# Calculator Project\n\n## Setup\n1. Create virtual environment: `python -m venv calculator_env`\n2. Activate: `source calculator_env/bin/activate` (Mac/Linux) or `calculator_env\\Scripts\\activate` (Windows)\n3. Install dependencies: `pip install -r requirements.txt`\n4. Run: `python main.py`\n\n## Dependencies\n- requests: For API calls\n- matplotlib: For data visualization\n\n# requirements.txt content:\n# certifi==2023.5.7\n# charset-normalizer==3.1.0\n# contourpy==1.0.7\n# cycler==0.11.0\n# fonttools==4.39.4\n# idna==3.4\n# kiwisolver==1.4.4\n# matplotlib==3.7.1\n# numpy==1.24.3\n# packaging==23.1\n# Pillow==9.5.0\n# pyparsing==3.0.9\n# python-dateutil==2.8.2\n# requests==2.31.0\n# six==1.16.0\n# urllib3==2.0.2";
                break;
            case 20:
                title = "🏆 Chapter 20: Best Practices & Code Quality";
                content = "Write clean, maintainable Python code! 🏆\n\nPEP 8 Style Guide:\n# Good\ndef calculate_area(radius):\n    return 3.14159 * radius ** 2\n\n# Bad\ndef calculateArea(r):\n    return 3.14159*r**2\n\nDocstrings:\ndef greet(name: str) -> str:\n    \"\"\"\n    Greet a person by name.\n    \n    Args:\n        name: The person's name\n        \n    Returns:\n        A greeting message\n    \"\"\"\n    return f'Hello, {name}!'\n\nType Hints:\nfrom typing import List, Dict, Optional\n\ndef process_data(items: List[int]) -> Dict[str, float]:\n    return {'average': sum(items) / len(items)}\n\nError Handling:\ndef safe_divide(a: float, b: float) -> Optional[float]:\n    try:\n        return a / b\n    except ZeroDivisionError:\n        print('Cannot divide by zero')\n        return None\n\nTesting:\nimport unittest\n\nclass TestMath(unittest.TestCase):\n    def test_addition(self):\n        self.assertEqual(2 + 2, 4)\n\nif __name__ == '__main__':\n    unittest.main()";
                mission = "🎯 MISSION: Code Quality Champion!\n\nRefactor this messy code to follow best practices:\n\ndef calc(x,y,op):\n  if op=='+':\n    return x+y\n  elif op=='-':\n    return x-y\n  elif op=='*':\n    return x*y\n  elif op=='/':\n    return x/y\n\nMake it:\n• PEP 8 compliant\n• Include type hints\n• Add docstrings\n• Handle errors\n• Add unit tests\n• Use enums for operations";
                answer = "✅ SOLUTION:\n\nfrom enum import Enum\nfrom typing import Union\nimport unittest\n\n\nclass Operation(Enum):\n    \"\"\"Mathematical operations.\"\"\"\n    ADD = '+'\n    SUBTRACT = '-'\n    MULTIPLY = '*'\n    DIVIDE = '/'\n\n\ndef calculate(x: Union[int, float], y: Union[int, float], \n             operation: Operation) -> Union[int, float]:\n    \"\"\"\n    Perform a mathematical operation on two numbers.\n    \n    Args:\n        x: First number\n        y: Second number\n        operation: Mathematical operation to perform\n        \n    Returns:\n        Result of the mathematical operation\n        \n    Raises:\n        ZeroDivisionError: When dividing by zero\n        ValueError: For unsupported operations\n    \"\"\"\n    if operation == Operation.ADD:\n        return x + y\n    elif operation == Operation.SUBTRACT:\n        return x - y\n    elif operation == Operation.MULTIPLY:\n        return x * y\n    elif operation == Operation.DIVIDE:\n        if y == 0:\n            raise ZeroDivisionError('Cannot divide by zero')\n        return x / y\n    else:\n        raise ValueError(f'Unsupported operation: {operation}')\n\n\nclass TestCalculator(unittest.TestCase):\n    \"\"\"Test cases for the calculator function.\"\"\"\n    \n    def test_addition(self):\n        \"\"\"Test addition operation.\"\"\"\n        result = calculate(2, 3, Operation.ADD)\n        self.assertEqual(result, 5)\n    \n    def test_subtraction(self):\n        \"\"\"Test subtraction operation.\"\"\"\n        result = calculate(5, 3, Operation.SUBTRACT)\n        self.assertEqual(result, 2)\n    \n    def test_multiplication(self):\n        \"\"\"Test multiplication operation.\"\"\"\n        result = calculate(4, 3, Operation.MULTIPLY)\n        self.assertEqual(result, 12)\n    \n    def test_division(self):\n        \"\"\"Test division operation.\"\"\"\n        result = calculate(6, 2, Operation.DIVIDE)\n        self.assertEqual(result, 3)\n    \n    def test_division_by_zero(self):\n        \"\"\"Test division by zero raises exception.\"\"\"\n        with self.assertRaises(ZeroDivisionError):\n            calculate(5, 0, Operation.DIVIDE)\n    \n    def test_float_numbers(self):\n        \"\"\"Test with floating point numbers.\"\"\"\n        result = calculate(2.5, 1.5, Operation.ADD)\n        self.assertAlmostEqual(result, 4.0)\n\n\ndef main():\n    \"\"\"Example usage of the calculator.\"\"\"\n    try:\n        result = calculate(10, 2, Operation.DIVIDE)\n        print(f'10 ÷ 2 = {result}')\n        \n        result = calculate(5, 0, Operation.DIVIDE)\n        print(f'This should not print')\n        \n    except ZeroDivisionError as e:\n        print(f'Error: {e}')\n\n\nif __name__ == '__main__':\n    # Run tests\n    unittest.main(argv=[''], exit=False, verbosity=2)\n    \n    # Run example\n    print('\\nExample usage:')\n    main()";
                break;
            default:
                title = "🐍 Python Tutorial";
                content = "Select a chapter to start learning!";
                mission = "";
                answer = "";
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
        if (ret->initAnchored(560.f, 360.f)) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }
};
