#pragma once

#include <Geode/ui/Popup.hpp>
#include <Geode/Geode.hpp> // useless
#include <Geode/ui/ScrollLayer.hpp>

using namespace geode::prelude;

class PythonPopup : public geode::Popup<> {
protected:
    CCLabelBMFont* titleLabel;
    CCLabelBMFont* contentLabel;
    ScrollLayer* scrollLayer;
    CCLayer* contentLayer;
    CCMenu* chapterMenu;
    CCLabelBMFont* pageLabel;
    CCMenuItemSpriteExtra* prevButton;
    CCMenuItemSpriteExtra* nextButton;
    
    int currentPage = 0;
    static const int chaptersPerPage = 10;
    static const int totalChapters = 20;

    bool setup() override {
        this->setTitle("Python Tutorial");

        titleLabel = CCLabelBMFont::create("Select a Chapter to Begin Learning", "bigFont.fnt");
        titleLabel->setPosition({280.f, 320.f});
        titleLabel->setScale(0.6f);
        m_mainLayer->addChild(titleLabel);

        scrollLayer = ScrollLayer::create({520.f, 180.f});
        scrollLayer->setPosition({20.f, 80.f});
        
        contentLayer = CCLayer::create();
        
        contentLabel = CCLabelBMFont::create("Welcome to Python Programming!\n\nPython is a powerful, easy-to-learn programming language.\nIt's great for beginners and used by professionals worldwide.\n\nSelect any chapter to start learning:", "chatFont.fnt", 500.f, kCCTextAlignmentLeft);
        contentLabel->setPosition({260.f, 150.f});
        contentLabel->setScale(0.6f);
        contentLabel->setAnchorPoint({0.5f, 0.5f});
        contentLayer->addChild(contentLabel);
        
        scrollLayer->m_contentLayer->addChild(contentLayer);
        m_mainLayer->addChild(scrollLayer);

        chapterMenu = CCMenu::create();
        chapterMenu->setPosition({0, 0});
        m_mainLayer->addChild(chapterMenu);

        auto navMenu = CCMenu::create();
        navMenu->setPosition({0, 0});

        auto prevSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
        prevSprite->setFlipX(true);
        prevSprite->setScale(0.7f);
        prevButton = CCMenuItemSpriteExtra::create(
            prevSprite,
            this,
            menu_selector(PythonPopup::onPrevPage)
        );
        prevButton->setPosition({50.f, 40.f});
        navMenu->addChild(prevButton);

        auto nextSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
        nextSprite->setScale(0.7f);
        nextButton = CCMenuItemSpriteExtra::create(
            nextSprite,
            this,
            menu_selector(PythonPopup::onNextPage)
        );
        nextButton->setPosition({510.f, 40.f});
        navMenu->addChild(nextButton);

        pageLabel = CCLabelBMFont::create("Page 1/2", "goldFont.fnt");
        pageLabel->setPosition({280.f, 40.f});
        pageLabel->setScale(0.5f);
        m_mainLayer->addChild(pageLabel);

        auto closeSprite = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
        closeSprite->setScale(0.8f);
        auto closeButton = CCMenuItemSpriteExtra::create(
            closeSprite, 
            this, 
            menu_selector(PythonPopup::onClose)
        );
        closeButton->setPosition({520.f, 320.f});
        navMenu->addChild(closeButton);

        m_mainLayer->addChild(navMenu);
        
        updateChapterButtons();
        updateNavigationButtons();
        
        return true;
    }

    void updateChapterButtons() {
        chapterMenu->removeAllChildren();
        
        int startChapter = currentPage * chaptersPerPage + 1;
        int endChapter = std::min(startChapter + chaptersPerPage - 1, totalChapters);
        
        for (int i = startChapter; i <= endChapter; ++i) {
            auto normalSprite = ButtonSprite::create(("Ch." + std::to_string(i)).c_str(), "goldFont.fnt", "GJ_button_04.png");
            normalSprite->setScale(0.4f); // Smaller buttons
            auto button = CCMenuItemSpriteExtra::create(
                normalSprite,
                this,
                menu_selector(PythonPopup::onChapter)
            );
            button->setTag(i);

            int buttonIndex = i - startChapter;
            float x = 80.f + (buttonIndex % 5) * 100.f;
            float y = 300.f - (buttonIndex / 5) * 30.f;
            button->setPosition({x, y});
            chapterMenu->addChild(button);
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

    void onChapter(CCObject* sender) {
        int chapter = static_cast<CCNode*>(sender)->getTag();
        std::string title, content;
        
        switch (chapter) {
            case 1:
                title = "Chapter 1: Introduction & Installation";
                content = "What is Python?\n• High-level programming language\n• Easy to read and write\n• Used for web dev, AI, data science\n\nInstalling Python:\n1. Go to python.org\n2. Download latest version\n3. Run installer\n4. Check 'Add to PATH'\n\nFirst Program:\nprint('Hello, World!')\n\nRunning Code:\n• Save as .py file\n• Run: python filename.py\n• Or use IDLE/IDE";
                break;
            case 2:
                title = "Chapter 2: Variables & Data Types";
                content = "Variables store data:\n# Numbers\nage = 25\nheight = 5.9\n# Text\nname = 'Alice'\ncity = \"New York\"\n# Boolean\nis_student = True\n\nData Types:\n• int: whole numbers (42)\n• float: decimals (3.14)\n• str: text ('hello')\n• bool: True/False\n• list: [1, 2, 3]\n• dict: {'key': 'value'}\n\nChecking Type:\nprint(type(age))  # <class 'int'>";
                break;
            case 3:
                title = "Chapter 3: Input & Output";
                content = "Getting User Input:\nname = input('Enter your name: ')\nage = int(input('Enter age: '))\n\nPrint Function:\nprint('Hello', name)\nprint(f'You are {age} years old')\n\nFormatting:\n# f-strings (recommended)\nprint(f'{name} is {age}')\n# .format() method\nprint('{} is {}'.format(name, age))\n# % formatting\nprint('%s is %d' % (name, age))\n\nSpecial Characters:\nprint('Line 1\\nLine 2')  # New line\nprint('Tab\\there')      # Tab";
                break;
            case 4:
                title = "Chapter 4: Operators";
                content = "Arithmetic Operators:\n+ (add), - (subtract), * (multiply)\n/ (divide), // (floor divide)\n% (modulo), ** (power)\n\nExamples:\nprint(10 + 3)   # 13\nprint(10 / 3)   # 3.333...\nprint(10 // 3)  # 3\nprint(10 % 3)   # 1\nprint(2 ** 3)   # 8\n\nComparison:\n== (equal), != (not equal)\n< > <= >= (comparison)\n\nLogical:\nand, or, not\n\nAssignment:\n=, +=, -=, *=, /=";
                break;
            case 5:
                title = "Chapter 5: Conditional Statements";
                content = "if Statement:\nage = 18\nif age >= 18:\n    print('Adult')\n\nif-else:\nif age >= 18:\n    print('Adult')\nelse:\n    print('Minor')\n\nif-elif-else:\nscore = 85\nif score >= 90:\n    grade = 'A'\nelif score >= 80:\n    grade = 'B'\nelif score >= 70:\n    grade = 'C'\nelse:\n    grade = 'F'\n\nNested if:\nif age >= 18:\n    if has_license:\n        print('Can drive')";
                break;
            case 6:
                title = "Chapter 6: Loops";
                content = "for Loop:\n# Range\nfor i in range(5):\n    print(i)  # 0,1,2,3,4\n\n# List iteration\nfruits = ['apple', 'banana']\nfor fruit in fruits:\n    print(fruit)\n\nwhile Loop:\ncount = 0\nwhile count < 5:\n    print(count)\n    count += 1\n\nLoop Control:\nbreak    # Exit loop\ncontinue # Skip iteration\n\nfor i in range(10):\n    if i == 3:\n        continue\n    if i == 8:\n        break\n    print(i)";
                break;
            case 7:
                title = "Chapter 7: Lists";
                content = "Creating Lists:\nfruits = ['apple', 'banana', 'orange']\nnumbers = [1, 2, 3, 4, 5]\nmixed = [1, 'hello', True, 3.14]\n\nAccessing Elements:\nprint(fruits[0])     # apple\nprint(fruits[-1])    # orange (last)\n\nList Methods:\nfruits.append('grape')    # Add to end\nfruits.insert(1, 'kiwi')  # Insert at position\nfruits.remove('banana')   # Remove item\npopped = fruits.pop()     # Remove & return last\n\nSlicing:\nprint(fruits[1:3])   # Elements 1-2\nprint(fruits[:2])    # First 2\nprint(fruits[2:])    # From index 2";
                break;
            case 8:
                title = "Chapter 8: Dictionaries";
                content = "Creating Dictionaries:\nperson = {\n    'name': 'Alice',\n    'age': 30,\n    'city': 'NYC'\n}\n\nAccessing Values:\nprint(person['name'])     # Alice\nprint(person.get('age')) # 30\n\nModifying:\nperson['age'] = 31       # Update\nperson['job'] = 'Engineer' # Add new\ndel person['city']       # Delete\n\nDictionary Methods:\nkeys = person.keys()     # Get all keys\nvalues = person.values() # Get all values\nitems = person.items()   # Get key-value pairs\n\nLooping:\nfor key, value in person.items():\n    print(f'{key}: {value}')";
                break;
            case 9:
                title = "Chapter 9: Functions";
                content = "Defining Functions:\ndef greet(name):\n    return f'Hello, {name}!'\n\n# Calling function\nmessage = greet('Alice')\nprint(message)\n\nParameters:\ndef add(a, b):\n    return a + b\n\nresult = add(5, 3)  # 8\n\nDefault Parameters:\ndef greet(name, greeting='Hello'):\n    return f'{greeting}, {name}!'\n\ngreet('Bob')              # Hello, Bob!\ngreet('Bob', 'Hi')        # Hi, Bob!\n\nMultiple Returns:\ndef get_name_age():\n    return 'Alice', 25\n\nname, age = get_name_age()";
                break;
            case 10:
                title = "Chapter 10: String Methods";
                content = "Common String Methods:\ntext = 'Hello World'\n\n# Case methods\nprint(text.upper())      # HELLO WORLD\nprint(text.lower())      # hello world\nprint(text.title())      # Hello World\nprint(text.capitalize()) # Hello world\n\n# Search methods\nprint(text.find('World'))    # 6\nprint(text.count('l'))       # 3\nprint(text.startswith('H'))  # True\nprint(text.endswith('d'))    # True\n\n# Modify methods\nprint(text.replace('World', 'Python'))  # Hello Python\nprint(text.strip())          # Remove whitespace\n\n# Split/Join\nwords = text.split()         # ['Hello', 'World']\njoined = '-'.join(words)     # Hello-World";
                break;
            case 11:
                title = "Chapter 11: File Handling";
                content = "Reading Files:\n# Method 1\nwith open('file.txt', 'r') as f:\n    content = f.read()\n    print(content)\n\n# Method 2 - Line by line\nwith open('file.txt', 'r') as f:\n    for line in f:\n        print(line.strip())\n\nWriting Files:\nwith open('output.txt', 'w') as f:\n    f.write('Hello World\\n')\n    f.write('Python is awesome!')\n\nAppending:\nwith open('log.txt', 'a') as f:\n    f.write('New entry\\n')\n\nFile Modes:\n'r' - Read only\n'w' - Write (overwrites)\n'a' - Append\n'r+' - Read and write";
                break;
            case 12:
                title = "Chapter 12: Exception Handling";
                content = "try-except:\ntry:\n    num = int(input('Enter number: '))\n    result = 10 / num\n    print(f'Result: {result}')\nexcept ValueError:\n    print('Invalid number!')\nexcept ZeroDivisionError:\n    print('Cannot divide by zero!')\n\nMultiple Exceptions:\ntry:\n    # risky code\n    pass\nexcept (ValueError, TypeError):\n    print('Invalid input!')\n\nGeneral Exception:\ntry:\n    # code\n    pass\nexcept Exception as e:\n    print(f'Error: {e}')\nfinally:\n    print('Always runs')";
                break;
            case 13:
                title = "Chapter 13: Classes & Objects";
                content = "Defining Classes:\nclass Dog:\n    def __init__(self, name, breed):\n        self.name = name\n        self.breed = breed\n        self.age = 0\n    \n    def bark(self):\n        return f'{self.name} says Woof!'\n    \n    def birthday(self):\n        self.age += 1\n\nCreating Objects:\nmy_dog = Dog('Buddy', 'Golden Retriever')\nprint(my_dog.bark())     # Buddy says Woof!\nmy_dog.birthday()\nprint(my_dog.age)        # 1\n\nClass vs Instance Variables:\nclass Cat:\n    species = 'Feline'  # Class variable\n    \n    def __init__(self, name):\n        self.name = name    # Instance variable";
                break;
            case 14:
                title = "Chapter 14: Modules & Packages";
                content = "Importing Modules:\nimport math\nprint(math.sqrt(16))     # 4.0\nprint(math.pi)           # 3.14159...\n\n# Import specific functions\nfrom math import sqrt, pi\nprint(sqrt(25))          # 5.0\n\n# Import with alias\nimport math as m\nprint(m.cos(0))          # 1.0\n\nUseful Built-in Modules:\nimport random\nprint(random.randint(1, 10))\n\nimport datetime\nnow = datetime.datetime.now()\nprint(now)\n\nimport os\nprint(os.getcwd())       # Current directory\n\nCreating Your Own Module:\n# Save as mymodule.py\ndef my_function():\n    return 'Hello from module!'\n\n# In another file\nimport mymodule\nprint(mymodule.my_function())";
                break;
            case 15:
                title = "Chapter 15: List Comprehensions";
                content = "Basic List Comprehension:\n# Traditional way\nsquares = []\nfor x in range(10):\n    squares.append(x**2)\n\n# List comprehension\nsquares = [x**2 for x in range(10)]\nprint(squares)  # [0,1,4,9,16,25,36,49,64,81]\n\nWith Conditions:\n# Even squares only\neven_squares = [x**2 for x in range(10) if x % 2 == 0]\nprint(even_squares)  # [0,4,16,36,64]\n\n# String processing\nwords = ['hello', 'world', 'python']\ncapitalized = [word.upper() for word in words]\nprint(capitalized)  # ['HELLO', 'WORLD', 'PYTHON']\n\nNested Comprehensions:\nmatrix = [[i*j for j in range(3)] for i in range(3)]\nprint(matrix)  # [[0,0,0], [0,1,2], [0,2,4]]";
                break;
            case 16:
                title = "Chapter 16: Lambda Functions";
                content = "Lambda Functions (Anonymous):\n# Regular function\ndef square(x):\n    return x ** 2\n\n# Lambda equivalent\nsquare = lambda x: x ** 2\nprint(square(5))  # 25\n\nWith Built-in Functions:\nnumbers = [1, 2, 3, 4, 5]\n\n# map() - apply function to all items\nsquared = list(map(lambda x: x**2, numbers))\nprint(squared)  # [1, 4, 9, 16, 25]\n\n# filter() - filter items\neven = list(filter(lambda x: x % 2 == 0, numbers))\nprint(even)  # [2, 4]\n\n# sorted() with key\nstudents = [('Alice', 85), ('Bob', 92), ('Charlie', 78)]\nsorted_by_grade = sorted(students, key=lambda x: x[1])\nprint(sorted_by_grade)";
                break;
            case 17:
                title = "Chapter 17: Decorators";
                content = "Basic Decorator:\ndef my_decorator(func):\n    def wrapper():\n        print('Before function')\n        func()\n        print('After function')\n    return wrapper\n\n@my_decorator\ndef greet():\n    print('Hello!')\n\ngreet()\n# Output:\n# Before function\n# Hello!\n# After function\n\nDecorator with Arguments:\ndef repeat(times):\n    def decorator(func):\n        def wrapper(*args, **kwargs):\n            for _ in range(times):\n                result = func(*args, **kwargs)\n            return result\n        return wrapper\n    return decorator\n\n@repeat(3)\ndef say_hello():\n    print('Hello!')\n\nsay_hello()  # Prints Hello! 3 times";
                break;
            case 18:
                title = "Chapter 18: Working with APIs";
                content = "Using requests library:\nimport requests\n\n# GET request\nresponse = requests.get('https://api.github.com/users/octocat')\nif response.status_code == 200:\n    data = response.json()\n    print(data['name'])\nelse:\n    print('Error:', response.status_code)\n\n# POST request\ndata = {'name': 'John', 'email': 'john@example.com'}\nresponse = requests.post('https://httpbin.org/post', json=data)\nprint(response.json())\n\n# Headers\nheaders = {'Authorization': 'Bearer token123'}\nresponse = requests.get('https://api.example.com/data', headers=headers)\n\nNote: Install requests first:\npip install requests";
                break;
            case 19:
                title = "Chapter 19: Virtual Environments";
                content = "Why Virtual Environments?\n• Isolate project dependencies\n• Avoid version conflicts\n• Clean project setup\n\nCreating Virtual Environment:\n# Using venv (Python 3.3+)\npython -m venv myenv\n\n# Activate (Windows)\nmyenv\\Scripts\\activate\n# Activate (Mac/Linux)\nsource myenv/bin/activate\n\n# Deactivate\ndeactivate\n\nManaging Packages:\n# Install package\npip install requests\n\n# List installed packages\npip list\n\n# Save requirements\npip freeze > requirements.txt\n\n# Install from requirements\npip install -r requirements.txt\n\nBest Practices:\n• One virtual env per project\n• Always activate before working\n• Keep requirements.txt updated";
                break;
            case 20:
                title = "Chapter 20: Best Practices & Next Steps";
                content = "Python Best Practices:\n\n1. Code Style (PEP 8):\n• Use 4 spaces for indentation\n• Keep lines under 79 characters\n• Use snake_case for variables\n• Use CamelCase for classes\n\n2. Documentation:\n• Write docstrings for functions\n• Comment complex logic\n• Use meaningful variable names\n\n3. Error Handling:\n• Use specific exceptions\n• Don't ignore errors\n• Log important information\n\nNext Steps:\n• Web Development: Django, Flask\n• Data Science: pandas, numpy, matplotlib\n• Machine Learning: scikit-learn, TensorFlow\n• Automation: selenium, requests\n• Desktop GUI: tkinter, PyQt\n• Game Development: pygame\n\nResources:\n• python.org - Official docs\n• Real Python - Tutorials\n• GitHub - Open source projects\n• Stack Overflow - Q&A";
                break;
            default:
                title = "Select a Chapter";
                content = "Welcome to Python Programming!\n\nThis comprehensive tutorial covers everything you need to know.\nClick any chapter button to begin learning!";
        }
        
        titleLabel->setString(title.c_str());
        contentLabel->setString(content.c_str());

        auto contentSize = contentLabel->getContentSize();
        scrollLayer->m_contentLayer->setContentSize({520.f, contentSize.height + 50.f});
        contentLayer->setContentSize({520.f, contentSize.height + 50.f});
        contentLabel->setPosition({260.f, contentSize.height / 2 + 25.f});
        
        scrollLayer->scrollToTop();
    }

    void onClose(CCObject*) override {
        this->setKeypadEnabled(false);
        this->removeFromParentAndCleanup(true);
    }

public:
    static PythonPopup* create() {
        auto ret = new PythonPopup();
        if (ret->initAnchored(560.f, 360.f)) { // Smaller popup size
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }
};
