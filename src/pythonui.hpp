class PythonPopup : public FLAlertLayer {
private:
    CCLayer* m_mainLayer;
    CCLabelBMFont* titleLabel;
    CCLabelBMFont* contentLabel;
    CCLabelBMFont* missionLabel;
    CCMenuItemSprite* prevButton;
    CCMenuItemSprite* nextButton;
    CCLabelBMFont* pageLabel;
    int currentChapter = 0;
    int maxChapter = 5;

    CCLayer* backgroundLayer;

public:
    static PythonPopup* create() {
        auto ret = new PythonPopup();
        if (ret && ret->init()) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    bool init() override {
        if (!FLAlertLayer::init()) return false;

        m_mainLayer = CCLayer::create();
        addChild(m_mainLayer);

        createAnimatedBackground();
        setupUI();
        updateChapter();

        return true;
    }

    void createAnimatedBackground() {
        backgroundLayer = CCLayer::create();

        auto whiteBg = CCScale9Sprite::create("square02_001.png");
        whiteBg->setContentSize({560.f, 340.f});
        whiteBg->setPosition({280.f, 180.f});
        whiteBg->setColor({255, 255, 255});
        whiteBg->setOpacity(255);
        backgroundLayer->addChild(whiteBg);

        m_mainLayer->addChild(backgroundLayer, -1);
    }

    void setupUI() {
        // Title
        titleLabel = CCLabelBMFont::create("", "bigFont.fnt");
        titleLabel->setPosition({280.f, 320.f});
        m_mainLayer->addChild(titleLabel);

        // Content Label - multiline explanation, fixed width, word wrap
        contentLabel = CCLabelBMFont::create("", "smallFont.fnt", 520, kCCTextAlignmentLeft);
        contentLabel->setAnchorPoint({0.5f, 1.0f});
        contentLabel->setPosition({280.f, 290.f});
        m_mainLayer->addChild(contentLabel);

        // Mission Label - smaller font under content
        missionLabel = CCLabelBMFont::create("", "smallFont.fnt", 520, kCCTextAlignmentLeft);
        missionLabel->setAnchorPoint({0.5f, 1.0f});
        missionLabel->setPosition({280.f, 90.f});
        m_mainLayer->addChild(missionLabel);

        // Show Answer button (only button besides navigation)
        auto showAnswerNormal = CCSprite::createWithSpriteFrameName("GJ_button_01.png");
        auto showAnswerSelected = CCSprite::createWithSpriteFrameName("GJ_button_02.png");
        auto showAnswerButton = CCMenuItemSprite::create(showAnswerNormal, showAnswerSelected, this, menu_selector(PythonPopup::onShowAnswer));
        showAnswerButton->setPosition({280.f, 40.f});

        auto showAnswerLabel = CCLabelBMFont::create("Show Answer", "goldFont.fnt");
        showAnswerLabel->setPosition(showAnswerButton->getContentSize() / 2);
        showAnswerButton->addChild(showAnswerLabel);

        // Prev and Next Buttons
        auto prevNormal = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
        auto prevSelected = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
        prevButton = CCMenuItemSprite::create(prevNormal, prevSelected, this, menu_selector(PythonPopup::onPrev));
        prevButton->setPosition({50.f, 35.f}); // moved higher

        auto nextNormal = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
        auto nextSelected = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
        nextButton = CCMenuItemSprite::create(nextNormal, nextSelected, this, menu_selector(PythonPopup::onNext));
        nextButton->setPosition({510.f, 35.f}); // moved higher

        pageLabel = CCLabelBMFont::create("", "goldFont.fnt");
        pageLabel->setPosition({280.f, 35.f}); // moved higher

        // Menu container
        auto menu = CCMenu::create(prevButton, nextButton, showAnswerButton, nullptr);
        menu->setPosition({0, 0});
        m_mainLayer->addChild(menu);
        m_mainLayer->addChild(pageLabel);
    }

    void updateChapter() {
        std::string title, content, mission, answer;
        getChapterContent(currentChapter, title, content, mission, answer);

        titleLabel->setString(title.c_str());
        contentLabel->setString(content.c_str());
        missionLabel->setString(mission.c_str());
        pageLabel->setString(CCString::createWithFormat("Chapter %d/%d", currentChapter + 1, maxChapter)->getCString());

        prevButton->setEnabled(currentChapter > 0);
        nextButton->setEnabled(currentChapter < maxChapter - 1);
    }

    void getChapterContent(int chapter, std::string& title, std::string& content, std::string& mission, std::string& answer) {
        switch(chapter) {
            case 0:
                title = "Introduction to Python";
                content = "Python is a versatile programming language. It supports variables, functions, and more.";
                mission = "Write a simple print statement to output 'Hello, world!'";
                answer = "print('Hello, world!')";
                break;
            case 1:
                title = "Variables and Data Types";
                content = "Variables store data. Python has types like int, float, and string.";
                mission = "Create a variable x and assign it the value 10.";
                answer = "x = 10";
                break;
            case 2:
                title = "Conditional Statements";
                content = "Use if, elif, else to control flow based on conditions.";
                mission = "Write an if statement that prints 'Yes' if x > 5.";
                answer = "if x > 5:\n    print('Yes')";
                break;
            case 3:
                title = "Loops";
                content = "Loops allow repeated execution. Python has for and while loops.";
                mission = "Write a for loop to print numbers 1 to 5.";
                answer = "for i in range(1, 6):\n    print(i)";
                break;
            case 4:
                title = "Functions";
                content = "Functions group reusable code blocks. Use def keyword.";
                mission = "Define a function greet() that prints 'Hi!'.";
                answer = "def greet():\n    print('Hi!')";
                break;
            default:
                title = "Unknown Chapter";
                content = "";
                mission = "";
                answer = "";
        }
    }

    void onShowAnswer(CCObject*) {
        std::string title, content, mission, answer;
        getChapterContent(currentChapter, title, content, mission, answer);

        FLAlertLayer::create(
            nullptr,
            title.c_str(),
            "Close",
            nullptr,
            answer.c_str()
        )->show();
    }

    void onPrev(CCObject*) {
        if (currentChapter > 0) {
            --currentChapter;
            updateChapter();
        }
    }

    void onNext(CCObject*) {
        if (currentChapter < maxChapter - 1) {
            ++currentChapter;
            updateChapter();
        }
    }
};
