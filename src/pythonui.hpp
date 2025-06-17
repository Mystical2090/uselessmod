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
    
    int currentPage = 0;
    int currentChapter = 0;
    bool showingAnswer = false;
    static const int chaptersPerPage = 8;
    static const int totalChapters = 20;

    bool setup() override {
        // Create animated background
        createAnimatedBackground();
        
        this->setTitle("Interactive Python Learning");

        // Main title with modern font
        titleLabel = CCLabelBMFont::create("🐍 Interactive Python Tutorial", "chatFont.fnt");
        titleLabel->setPosition({280.f, 330.f});
        titleLabel->setScale(0.8f);
        titleLabel->setColor({46, 125, 50}); // Python green
        m_mainLayer->addChild(titleLabel);

        // Subtitle
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
        
        // Welcome content with better formatting
        contentLabel = CCLabelBMFont::create("🚀 Welcome to Interactive Python Programming!\n\nPython is the world's most popular programming language.\nPerfect for beginners, powerful for experts.\n\n📚 Each chapter includes:\n  • Clear explanations\n  • Hands-on missions\n  • Instant feedback\n\n👆 Select any chapter below to start your journey!", "chatFont.fnt", 500.f, kCCTextAlignmentLeft);
        contentLabel->setPosition({260.f, 150.f});
        contentLabel->setScale(0.65f);
        contentLabel->setAnchorPoint({0.5f, 0.5f});
        contentLabel->setColor({60, 60, 60});
        contentLayer->addChild(contentLabel);

        // Mission section (initially hidden)
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

        // Chapter menu
        chapterMenu = CCMenu::create();
        chapterMenu->setPosition({0, 0});
        m_mainLayer->addChild(chapterMenu);

        // Action buttons menu
        actionMenu = CCMenu::create();
        actionMenu->setPosition({0, 0});

        // Show Answer button
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

        // Try It button
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

        // Navigation menu
        auto navMenu = CCMenu::create();
        navMenu->setPosition({0, 0});

        // Previous button with modern style
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

        // Page indicator
        pageLabel = CCLabelBMFont::create("Page 1/3", "chatFont.fnt");
        pageLabel->setPosition({280.f, 15.f});
        pageLabel->setScale(0.5f);
        pageLabel->setColor({120, 120, 120});
        navMenu->addChild(pageLabel);

        // Close button
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
        
        // Create gradient-like background with multiple layers
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
            
            // Add subtle animation
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
            // Create modern button style
            auto buttonBg = CCScale9Sprite::create("GJ_button_05.png");
            buttonBg->setContentSize({120.f, 40.f});
            
            // Add chapter icon
            auto icon = CCSprite::createWithSpriteFrameName("GJ_starsIcon_001.png");
            icon->setScale(0.4f);
            icon->setPosition({20.f, 20.f});
            icon->setColor({255, 215, 0}); // Gold color
            buttonBg->addChild(icon);
            
            // Chapter title
            auto chapterText = CCLabelBMFont::create(("Chapter " + std::to_string(i)).c_str(), "chatFont.fnt");
            chapterText->setPosition({75.f, 25.f});
            chapterText->setScale(0.45f);
            chapterText->setColor({40, 40, 40});
            buttonBg->addChild(chapterText);
            
            // Subtitle based on chapter
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
        // Simulate "Try It" functionality
        auto notification = CCLabelBMFont::create("🎉 Great job! You completed the mission!", "chatFont.fnt");
        notification->setPosition({280.f, 200.f});
        notification->setScale(0.6f);
        notification->setColor({0, 150, 0});
        m_mainLayer->addChild(notification);
        
        // Fade out notification
        auto fadeOut = CCFadeOut::create(2.0f);
        auto remove = CCCallFunc::create(notification, [](CCNode* node) {
            node->removeFromParent();
        });
        auto sequence = CCSequence::create(fadeOut, remove, nullptr);
        notification->runAction(sequence);
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
        
        // Show mission and action buttons
        missionLabel->setVisible(true);
        showAnswerButton->setVisible(true);
        tryItButton->setVisible(true);
        answerLabel->setVisible(false);
        
        // Update answer button text
        auto answerBg = static_cast<CCScale9Sprite*>(showAnswerButton->getNormalImage());
        auto answerText = static_cast<CCLabelBMFont*>(answerBg->getChildren()->objectAtIndex(1));
        answerText->setString("Show Answer");

        // Adjust content layout
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
            default:
                title = "Select a Chapter";
                content = "🎓 Welcome to Interactive Python Learning!\n\nEach chapter includes:\n• Clear explanations with examples\n• Fun, practical missions\n• Complete solutions\n• Step-by-step guidance\n\nClick any chapter to start your Python journey! 🚀";
                mission = "";
                answer = "";
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
