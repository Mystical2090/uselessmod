// if this shit mod gets on geode index i will switch to imgui
#pragma once

#include <Geode/ui/Popup.hpp>
#include <Geode/Geode.hpp>
#include <Geode/ui/ScrollLayer.hpp>

using namespace geode::prelude;

class PythonPopup : public geode::Popup<> {
protected:
    CCLabelBMFont* titleLabel;
    CCLabelBMFont* contentLabel;
    CCLabelBMFont* missionLabel;
    CCLabelBMFont* answerLabel;
    ScrollLayer* scrollLayer;
    CCLayer* contentLayer;
    CCLayer* backgroundLayer;
    CCMenu* chapterMenu;
    CCMenu* actionMenu;
    CCLabelBMFont* pageLabel;
    CCMenuItemSpriteExtra* prevButton;
    CCMenuItemSpriteExtra* nextButton;
    CCMenuItemSpriteExtra* showAnswerButton;
    CCMenuItemSpriteExtra* tryItButton;
    CCLabelBMFont* m_currentNotification;
    
    int currentPage = 0;
    int currentChapter = 0;
    bool showingAnswer = false;
    static const int chaptersPerPage = 8;
    static const int totalChapters = 20;

    bool setup() override {
        createAnimatedBackground(); // not done
        
        this->setTitle("Interactive Python Learning");
        titleLabel = CCLabelBMFont::create("🐍 Interactive Python Tutorial", "chatFont.fnt");
        titleLabel->setPosition({280.f, 330.f});
        titleLabel->setScale(0.8f);
        titleLabel->setColor({46, 125, 50}); // Python green
        m_mainLayer->addChild(titleLabel);

        auto subtitleLabel = CCLabelBMFont::create("Learn by doing - Complete missions and see solutions!", "chatFont.fnt");
        subtitleLabel->setPosition({280.f, 310.f});
        subtitleLabel->setScale(0.5f);
        subtitleLabel->setColor({100, 100, 100});
        m_mainLayer->addChild(subtitleLabel);

        // Create scroll area with rounded background
        auto scrollBg = CCScale9Sprite::create("square02b_001.png");
        scrollBg->setContentSize({530.f, 190.f});
        scrollBg->setPosition({280.f, 170.f});
        scrollBg->setColor({245, 245, 245});
        scrollBg->setOpacity(220);
        m_mainLayer->addChild(scrollBg);

        scrollLayer = ScrollLayer::create({520.f, 180.f});
        scrollLayer->setPosition({20.f, 80.f});
        
        contentLayer = CCLayer::create();
    
        contentLabel = CCLabelBMFont::create("🚀 Welcome to Interactive Python Programming!\n\nPython is the world's most popular programming language.\nPerfect for beginners, powerful for experts.\n\n📚 Each chapter includes:\n  • Clear explanations\n  • Hands-on missions\n  • Instant feedback\n\n👆 Select any chapter below to start your journey!", "chatFont.fnt", 500.f, kCCTextAlignmentLeft);
        contentLabel->setPosition({260.f, 150.f});
        contentLabel->setScale(0.65f);
        contentLabel->setAnchorPoint({0.5f, 0.5f});
        contentLabel->setColor({60, 60, 60});
        contentLayer->addChild(contentLabel);

        missionLabel = CCLabelBMFont::create("", "chatFont.fnt", 500.f, kCCTextAlignmentLeft);
        missionLabel->setPosition({260.f, 80.f});
        missionLabel->setScale(0.6f);
        missionLabel->setAnchorPoint({0.5f, 0.5f});
        missionLabel->setColor({0, 77, 153}); // Blue for missions
        missionLabel->setVisible(false);
        contentLayer->addChild(missionLabel);

        // Answer section (initially hidden)
        answerLabel = CCLabelBMFont::create("", "chatFont.fnt", 500.f, kCCTextAlignmentLeft);
        answerLabel->setPosition({260.f, 50.f});
        answerLabel->setScale(0.55f);
        answerLabel->setAnchorPoint({0.5f, 0.5f});
        answerLabel->setColor({0, 128, 0}); // Green for answers
        answerLabel->setVisible(false);
        contentLayer->addChild(answerLabel);
        
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
        showAnswerButton->setPosition({150.f, 40.f});
        showAnswerButton->setVisible(false);
        actionMenu->addChild(showAnswerButton);

        auto tryItBg = CCScale9Sprite::create("GJ_button_04.png");
        tryItBg->setContentSize({100.f, 35.f});
        auto tryItIcon = CCSprite::createWithSpriteFrameName("GJ_playBtn2_001.png");
        tryItIcon->setScale(0.5f);
        tryItIcon->setPosition({15.f, 17.5f});
        tryItBg->addChild(tryItIcon);
        auto tryItText = CCLabelBMFont::create("Try It!", "chatFont.fnt");
        tryItText->setPosition({65.f, 17.5f});
        tryItText->setScale(0.5f);
        tryItBg->addChild(tryItText);
        
        tryItButton = CCMenuItemSpriteExtra::create(
            tryItBg,
            this,
            menu_selector(PythonPopup::onTryIt)
        );
        tryItButton->setPosition({400.f, 40.f});
        tryItButton->setVisible(false);
        actionMenu->addChild(tryItButton);

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

        // Next button
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
                    bgSprite->setColor({240, 248, 255}); // Light blue
                    bgSprite->setOpacity(100);
                    break;
                case 1:
                    bgSprite->setColor({230, 245, 255}); // Slightly darker
                    bgSprite->setOpacity(80);
                    bgSprite->setScale(bgSprite->getScale() * 0.9f);
                    break;
                case 2:
                    bgSprite->setColor({220, 240, 255}); // Even darker
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
            icon->setColor({255, 215, 0}); // Gold color
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
        showingAnswer = !showingAnswer;
        answerLabel->setVisible(showingAnswer);
        
        auto answerBg = static_cast<CCScale9Sprite*>(showAnswerButton->getNormalImage());
        auto answerText = static_cast<CCLabelBMFont*>(answerBg->getChildren()->objectAtIndex(1));
        answerText->setString(showingAnswer ? "Hide Answer" : "Show Answer");
        
        scrollLayer->scrollToTop();
    }

    void onTryIt(CCObject*) {
        auto notification = CCLabelBMFont::create("🎉 Great job! You completed the mission!", "chatFont.fnt");
        notification->setPosition({280.f, 200.f});
        notification->setScale(0.6f);
        notification->setColor({0, 150, 0});
        m_mainLayer->addChild(notification);
        
        auto fadeOut = CCFadeOut::create(2.0f);
        auto remove = CCCallFunc::create(this, callfunc_selector(PythonPopup::removeNotification));
        auto sequence = CCSequence::create(fadeOut, remove, nullptr);
        notification->runAction(sequence);
        
        m_currentNotification = notification;
    }

    void removeNotification() {
        if (m_currentNotification) {
            m_currentNotification->removeFromParent();
            m_currentNotification = nullptr;
        }
    }

    void onChapter(CCObject* sender) {
        currentChapter = static_cast<CCNode*>(sender)->getTag();
        showingAnswer = false;
        std::string title, content, mission, answer;
        
        getChapterContent(currentChapter, title, content, mission, answer);
        
        titleLabel->setString(title.c_str());
        contentLabel->setString(content.c_str());
        missionLabel->setString(mission.c_str());
        answerLabel->setString(answer.c_str());
        
        missionLabel->setVisible(true);
        showAnswerButton->setVisible(true);
        tryItButton->setVisible(true);
        answerLabel->setVisible(false);
        
        auto answerBg = static_cast<CCScale9Sprite*>(showAnswerButton->getNormalImage());
        auto answerText = static_cast<CCLabelBMFont*>(answerBg->getChildren()->objectAtIndex(1));
        answerText->setString("Show Answer");

        auto contentSize = contentLabel->getContentSize();
        auto missionSize = missionLabel->getContentSize();
        auto answerSize = answerLabel->getContentSize();
        
        float totalHeight = contentSize.height + missionSize.height + answerSize.height + 150.f;
        scrollLayer->m_contentLayer->setContentSize({520.f, totalHeight});
        contentLayer->setContentSize({520.f, totalHeight});
        
        contentLabel->setPosition({260.f, totalHeight - contentSize.height / 2 - 25.f});
        missionLabel->setPosition({260.f, totalHeight - contentSize.height - missionSize.height / 2 - 75.f});
        answerLabel->setPosition({260.f, totalHeight - contentSize.height - missionSize.height - answerSize.height / 2 - 125.f});
        
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
            title = "🔧 Chapter 8: Functions & Reusability";
            content = "Define reusable blocks of code with functions.\n\nExample:\ndef greet(name):\n  print(f'Hello, {name}!')";
            mission = "🎯 MISSION: Function Builder\n\nWrite a function that takes a number and returns its square.";
            answer = "✅ SOLUTION:\n\ndef square(num):\n  return num * num\n\nprint(square(5))";
            break;

            case 9:
            title = "📂 Chapter 9: Dictionaries & Key-Value Pairs";
            content = "Store data as key-value pairs.\n\nExample:\nperson = {'name': 'Alice', 'age': 30}\nprint(person['name'])";
            mission = "🎯 MISSION: Dictionary Lookup\n\nCreate a dictionary of three countries and their capitals and print each.";
            answer = "✅ SOLUTION:\n\ncountries = {'USA': 'Washington', 'France': 'Paris', 'Japan': 'Tokyo'}\nfor country, capital in countries.items():\n  print(f'{country}: {capital}')";
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
            mission = "🎯 MISSION: File Writer\n\nWrite a program that saves a user’s input to a file.";
            answer = "✅ SOLUTION:\n\ntext = input('Enter text to save: ')\nwith open('output.txt', 'w') as f:\n  f.write(text)";
            break;

            case 12:
            title = "⚙️ Chapter 12: Error Handling & Exceptions";
            content = "Make your program robust with try-except blocks.\n\nExample:\ntry:\n  x = 1 / 0\nexcept ZeroDivisionError:\n  print('Cannot divide by zero')";
            mission = "🎯 MISSION: Safe Division\n\nWrite a program that divides two numbers but handles division by zero gracefully.";
            answer = "✅ SOLUTION:\n\ntry:\n  num1 = int(input('Enter numerator: '))\n  num2 = int(input('Enter denominator: '))\n  result = num1 / num2\n  print(f'Result: {result}')\nexcept ZeroDivisionError:\n  print('Error: Cannot divide by zero')";
            break;

            case 13:
            title = "📐 Chapter 13: Modules & Importing";
            content = "Reuse code by importing modules.\n\nExample:\nimport math\nprint(math.sqrt(16))";
            mission = "🎯 MISSION: Module Use\n\nWrite a program that imports random and prints a random number between 1 and 10.";
            answer = "✅ SOLUTION:\n\nimport random\nprint(random.randint(1, 10))";
            break;

            case 14:
            title = "💻 Chapter 14: Object-Oriented Programming";
            content = "Structure your code with classes and objects.\n\nExample:\nclass Dog:\n  def __init__(self, name):\n    self.name = name";
            mission = "🎯 MISSION: Class Creation\n\nCreate a class `Car` with attributes and a method to display details.";
            answer = "✅ SOLUTION:\n\nclass Car:\n  def __init__(self, make, model):\n    self.make = make\n    self.model = model\n  def show(self):\n    print(f'{self.make} {self.model}')\n\ncar = Car('Toyota', 'Corolla')\ncar.show()";
            break;

            case 15:
            title = "🔄 Chapter 15: Recursion & Self-Calling Functions";
            content = "Functions that call themselves to solve problems.\n\nExample:\ndef factorial(n):\n  if n == 0:\n    return 1\n  else:\n    return n * factorial(n-1)";
            mission = "🎯 MISSION: Recursive Factorial\n\nWrite a recursive function to compute factorial.";
            answer = "✅ SOLUTION:\n\ndef factorial(n):\n  if n == 0:\n    return 1\n  return n * factorial(n-1)\n\nprint(factorial(5))";
            break;

            case 16:
            title = "📊 Chapter 16: Data Analysis Basics";
            content = "Use Python to analyze simple datasets.\n\nExample:\ndata = [5, 10, 15]\navg = sum(data) / len(data)\nprint(avg)";
            mission = "🎯 MISSION: Calculate Average\n\nWrite a program to input numbers and calculate their average.";
            answer = "✅ SOLUTION:\n\nnumbers = list(map(int, input('Enter numbers separated by space: ').split()))\navg = sum(numbers) / len(numbers)\nprint(f'Average: {avg}')";
            break;

            case 17:
            title = "🌐 Chapter 17: Web Scraping Intro";
            content = "Extract data from websites using Python libraries.\n\nExample:\nimport requests\nresponse = requests.get('http://example.com')";
            mission = "🎯 MISSION: Fetch Title\n\nWrite a program to fetch and print the title of a webpage (using BeautifulSoup).";
            answer = "✅ SOLUTION:\n\nimport requests\nfrom bs4 import BeautifulSoup\nurl = 'http://example.com'\nresponse = requests.get(url)\nsoup = BeautifulSoup(response.text, 'html.parser')\nprint(soup.title.string)";
            break;

            case 18:
            title = "🔐 Chapter 18: Introduction to Encryption";
            content = "Learn simple data encryption concepts.\n\nExample:\ntext = 'hello'\nencrypted = ''.join(chr(ord(c) + 1) for c in text)";
            mission = "🎯 MISSION: Caesar Cipher\n\nWrite a program to encrypt a message by shifting letters by 3.";
            answer = "✅ SOLUTION:\n\nmessage = input('Enter message: ')\nencrypted = ''.join(chr((ord(c) - 65 + 3) % 26 + 65) if c.isupper() else c for c in message)\nprint(encrypted)";
            break;

            case 19:
            title = "⚡ Chapter 19: Multithreading Basics";
            content = "Run multiple threads for concurrency.\n\nExample:\nimport threading\ndef task():\n  print('Thread running')";
            mission = "🎯 MISSION: Thread Runner\n\nWrite a program that starts two threads running a simple function.";
            answer = "✅ SOLUTION:\n\nimport threading\n\ndef print_hello():\n  print('Hello from thread')\n\nthread1 = threading.Thread(target=print_hello)\nthread2 = threading.Thread(target=print_hello)\nthread1.start()\nthread2.start()";
            break;

            case 20:
            title = "🧠 Chapter 20: Introduction to AI & Machine Learning";
            content = "Basics of AI with Python libraries.\n\nExample:\nfrom sklearn.linear_model import LinearRegression";
            mission = "🎯 MISSION: Simple Prediction\n\nWrite a program to train a simple linear regression model and predict a value.";
            answer = "✅ SOLUTION:\n\nfrom sklearn.linear_model import LinearRegression\nimport numpy as np\n\nX = np.array([[1], [2], [3], [4]])\ny = np.array([2, 4, 6, 8])\nmodel = LinearRegression().fit(X, y)\nprint(model.predict([[5]]))";
            break;

            default:
            title = "Chapter Not Found";
            content = "Content for this chapter is not available.";
            mission = "";
            answer = "";
            break;
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
