#pragma once

#include <Geode/ui/Popup.hpp>
#include <Geode/ui/ScrollLayer.hpp>

using namespace geode::prelude;

class PythonPopup : public geode::Popup<> {
protected:
    CCLabelBMFont* titleLabel;
    CCLabelBMFont* contentLabel;
    ScrollLayer* scrollLayer;
    CCLayer* contentLayer;
    CCSprite* backgroundSprite;
    CCMenu* navigationMenu;
    CCMenu* chapterMenu;
    int currentChapter = 0;
    int currentPage = 0;
    int totalPages = 4; // 5 chapters per page, 20 chapters = 4 pages

    bool setup() override {
        this->setTitle("Python Tutorial");

        backgroundSprite = CCSprite::create("game_bg_01_001.png");
        if (backgroundSprite) {
            backgroundSprite->setPosition({360.f, 260.f});
            backgroundSprite->setScale(2.0f);
            backgroundSprite->setOpacity(30);
            m_mainLayer->addChild(backgroundSprite, -1);
        }

        titleLabel = CCLabelBMFont::create("Select a Chapter to Begin Learning", "goldFont.fnt");
        titleLabel->setPosition({360.f, 480.f});
        titleLabel->setScale(0.7f);
        m_mainLayer->addChild(titleLabel);

        auto separatorLine = CCSprite::createWithSpriteFrameName("edit_eLine02_001.png");
        separatorLine->setPosition({360.f, 460.f});
        separatorLine->setScaleX(8.0f);
        separatorLine->setScaleY(0.5f);
        m_mainLayer->addChild(separatorLine);

        // Create scroll layer for content (moved up)
        scrollLayer = ScrollLayer::create({680.f, 220.f});
        scrollLayer->setPosition({40.f, 200.f});
        
        contentLayer = CCLayer::create();
        
        contentLabel = CCLabelBMFont::create("Welcome to Python Programming!\n\nPython is a powerful, easy-to-learn programming language.\nIt's great for beginners and used by professionals worldwide.\n\nSelect any chapter to start learning:", "chatFont.fnt", 650.f, kCCTextAlignmentLeft);
        contentLabel->setPosition({340.f, 110.f});
        contentLabel->setScale(0.65f);
        contentLabel->setAnchorPoint({0.5f, 0.5f});
        contentLayer->addChild(contentLabel);
        
        scrollLayer->m_contentLayer->addChild(contentLayer);
        m_mainLayer->addChild(scrollLayer);

        chapterMenu = CCMenu::create();
        chapterMenu->setPosition({0, 0});
        createChapterButtons();
        m_mainLayer->addChild(chapterMenu);

        navigationMenu = CCMenu::create();
        navigationMenu->setPosition({0, 0});

        auto leftArrow = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png"),
            this,
            menu_selector(PythonPopup::onPrevPage)
        );
        leftArrow->setPosition({50.f, 160.f});
        leftArrow->setRotation(180.f);
        navigationMenu->addChild(leftArrow);

        auto rightArrow = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png"),
            this,
            menu_selector(PythonPopup::onNextPage)
        );
        rightArrow->setPosition({670.f, 160.f});
        navigationMenu->addChild(rightArrow);

        auto pageLabel = CCLabelBMFont::create("Page 1/4", "bigFont.fnt");
        pageLabel->setPosition({360.f, 140.f});
        pageLabel->setScale(0.5f);
        pageLabel->setTag(100); // Tag for easy access
        navigationMenu->addChild(pageLabel);

        m_mainLayer->addChild(navigationMenu);

        auto closeMenu = CCMenu::create();
        closeMenu->setPosition({0, 0});
        
        auto closeSprite = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
        auto closeButton = CCMenuItemSpriteExtra::create(
            closeSprite, 
            this, 
            menu_selector(PythonPopup::onClose)
        );
        closeButton->setPosition({690.f, 490.f});
        closeMenu->addChild(closeButton);

        m_mainLayer->addChild(closeMenu);
        
        updateBackground();
        return true;
    }

    void createChapterButtons() {
        chapterMenu->removeAllChildren();
        
        int startChapter = currentPage * 5 + 1;
        int endChapter = std::min(startChapter + 4, 20);
        
        for (int i = startChapter; i <= endChapter; ++i) {
            auto normalSprite = ButtonSprite::create(("Ch." + std::to_string(i)).c_str(), "bigFont.fnt", "GJ_button_04.png");
            normalSprite->setScale(0.55f);
            auto button = CCMenuItemSpriteExtra::create(
                normalSprite,
                this,
                menu_selector(PythonPopup::onChapter)
            );
            button->setTag(i);
            
            // Tighter horizontal spacing, moved down
            float x = 60.f + ((i - startChapter) % 5) * 120.f;
            float y = 160.f; // Moved down significantly
            button->setPosition({x, y});
            chapterMenu->addChild(button);
        }
    }

    void updateBackground() {
        if (!backgroundSprite) return;
        
        std::string bgNames[] = {
            "game_bg_01_001.png", // Blue theme
            "game_bg_02_001.png", // Green theme  
            "game_bg_03_001.png", // Orange theme
            "game_bg_04_001.png"  // Purple theme
        };
        
        if (currentPage < 4) {
            auto newTexture = CCTextureCache::sharedTextureCache()->textureForKey(bgNames[currentPage].c_str());
            if (newTexture) {
                backgroundSprite->setTexture(newTexture);
            }
        }
    }

    void updatePageIndicator() {
        auto pageLabel = static_cast<CCLabelBMFont*>(navigationMenu->getChildByTag(100));
        if (pageLabel) {
            std::string pageText = "Page " + std::to_string(currentPage + 1) + "/4";
            pageLabel->setString(pageText.c_str());
        }
    }

    void onPrevPage(CCObject*) {
        if (currentPage > 0) {
            currentPage--;
            createChapterButtons();
            updateBackground();
            updatePageIndicator();
        }
    }

    void onNextPage(CCObject*) {
        if (currentPage < totalPages - 1) {
            currentPage++;
            createChapterButtons();
            updateBackground();
            updatePageIndicator();
        }
    }

    void onChapter(CCObject* sender) {
        currentChapter = static_cast<CCNode*>(sender)->getTag();
        std::string title, content;
        
        switch (currentChapter) {
            case 1:
                title = "Chapter 1: Introduction & Installation";
                content = "What is Python?\n• High-level programming language\n• Easy to read and write\n• Used for web development, AI, and data science\n\nInstalling Python:\n1. Visit python.org\n2. Download the latest version\n3. Run the installer\n4. Check 'Add Python to PATH'\n\nYour First Program:\n```python\nprint('Hello, World!')\n```\n\nRunning Your Code:\n• Save files with .py extension\n• Run: python filename.py\n• Or use IDLE/PyCharm/VS Code";
                break;
            case 2:
                title = "Chapter 2: Variables & Data Types";
                content = "Variables store data for later use:\n\n```python\n# Numbers\nage = 25\nheight = 5.9\n\n# Text (strings)\nname = 'Alice'\ncity = \"New York\"\n\n# Boolean (True/False)\nis_student = True\n```\n\nMain Data Types:\n• int: whole numbers (42, -10)\n• float: decimal numbers (3.14, 2.5)\n• str: text ('hello', \"world\")\n• bool: True or False\n• list: [1, 2, 3, 'mixed']\n• dict: {'name': 'Alice', 'age': 25}\n\nChecking Types:\n```python\nprint(type(age))  # <class 'int'>\n```";
                break;
            case 3:
                title = "Chapter 3: Input & Output";
                content = "Getting User Input:\n```python\nname = input('Enter your name: ')\nage = int(input('Enter your age: '))\nheight = float(input('Enter height: '))\n```\n\nPrint Function Basics:\n```python\nprint('Hello', name)\nprint('You are', age, 'years old')\n```\n\nFormatted Strings (f-strings):\n```python\nprint(f'Hello {name}!')\nprint(f'You are {age} years old')\nprint(f'{name} is {height}m tall')\n```\n\nSpecial Characters:\n```python\nprint('Line 1\\nLine 2')    # New line\nprint('Name:\\tAlice')      # Tab\nprint('Quote: \"Hello\"')    # Quote marks\n```";
                break;
            case 4:
                title = "Chapter 4: Operators";
                content = "Arithmetic Operators:\n```python\nprint(10 + 3)   # 13 (addition)\nprint(10 - 3)   # 7  (subtraction)\nprint(10 * 3)   # 30 (multiplication)\nprint(10 / 3)   # 3.33... (division)\nprint(10 // 3)  # 3  (floor division)\nprint(10 % 3)   # 1  (remainder/modulo)\nprint(2 ** 3)   # 8  (power/exponent)\n```\n\nComparison Operators:\n```python\nprint(5 == 5)   # True  (equal)\nprint(5 != 3)   # True  (not equal)\nprint(5 > 3)    # True  (greater than)\nprint(5 < 3)    # False (less than)\nprint(5 >= 5)   # True  (greater or equal)\nprint(5 <= 3)   # False (less or equal)\n```\n\nLogical Operators:\n```python\nprint(True and False)  # False\nprint(True or False)   # True\nprint(not True)        # False\n```";
                break;
            case 5:
                title = "Chapter 5: Conditional Statements";
                content = "Basic if Statement:\n```python\nage = 18\nif age >= 18:\n    print('You are an adult')\n```\n\nif-else Statement:\n```python\nif age >= 18:\n    print('You are an adult')\nelse:\n    print('You are a minor')\n```\n\nif-elif-else Chain:\n```python\nscore = 85\nif score >= 90:\n    grade = 'A'\nelif score >= 80:\n    grade = 'B'\nelif score >= 70:\n    grade = 'C'\nelif score >= 60:\n    grade = 'D'\nelse:\n    grade = 'F'\nprint(f'Your grade is: {grade}')\n```\n\nNested Conditions:\n```python\nif age >= 18:\n    if has_license:\n        print('You can drive')\n    else:\n        print('Get a license first')\n```";
                break;
            case 6:
                title = "Chapter 6: Loops";
                content = "for Loop with range():\n```python\n# Print numbers 0 to 4\nfor i in range(5):\n    print(i)\n\n# Print numbers 1 to 10\nfor i in range(1, 11):\n    print(i)\n\n# Count by 2s: 0, 2, 4, 6, 8\nfor i in range(0, 10, 2):\n    print(i)\n```\n\nfor Loop with Lists:\n```python\nfruits = ['apple', 'banana', 'orange']\nfor fruit in fruits:\n    print(f'I like {fruit}')\n```\n\nwhile Loop:\n```python\ncount = 0\nwhile count < 5:\n    print(f'Count is: {count}')\n    count += 1  # Same as count = count + 1\n```\n\nLoop Control:\n```python\nfor i in range(10):\n    if i == 3:\n        continue  # Skip this iteration\n    if i == 8:\n        break     # Exit the loop\n    print(i)\n```";
                break;
            case 7:
                title = "Chapter 7: Lists";
                content = "Creating Lists:\n```python\nfruits = ['apple', 'banana', 'orange']\nnumbers = [1, 2, 3, 4, 5]\nmixed = [1, 'hello', True, 3.14]\nempty_list = []\n```\n\nAccessing Elements:\n```python\nprint(fruits[0])     # 'apple' (first item)\nprint(fruits[1])     # 'banana' (second item)\nprint(fruits[-1])    # 'orange' (last item)\nprint(fruits[-2])    # 'banana' (second to last)\n```\n\nList Methods:\n```python\nfruits.append('grape')      # Add to end\nfruits.insert(1, 'kiwi')    # Insert at position 1\nfruits.remove('banana')     # Remove specific item\npopped = fruits.pop()       # Remove and return last\nfruits.clear()              # Remove all items\n```\n\nList Slicing:\n```python\nprint(fruits[1:3])    # Items from index 1 to 2\nprint(fruits[:2])     # First 2 items\nprint(fruits[2:])     # From index 2 to end\nprint(fruits[::-1])   # Reverse the list\n```";
                break;
            case 8:
                title = "Chapter 8: Dictionaries";
                content = "Creating Dictionaries:\n```python\nperson = {\n    'name': 'Alice',\n    'age': 30,\n    'city': 'New York',\n    'is_student': False\n}\n```\n\nAccessing Values:\n```python\nprint(person['name'])        # 'Alice'\nprint(person.get('age'))     # 30\nprint(person.get('job', 'Unemployed'))  # Default value\n```\n\nModifying Dictionaries:\n```python\nperson['age'] = 31           # Update existing\nperson['job'] = 'Engineer'   # Add new key-value\ndel person['city']           # Delete a key-value\n```\n\nDictionary Methods:\n```python\nkeys = person.keys()         # Get all keys\nvalues = person.values()     # Get all values\nitems = person.items()       # Get key-value pairs\n```\n\nLooping Through Dictionaries:\n```python\nfor key, value in person.items():\n    print(f'{key}: {value}')\n```";
                break;
            case 9:
                title = "Chapter 9: Functions";
                content = "Defining Functions:\n```python\ndef greet(name):\n    return f'Hello, {name}!'\n\n# Calling the function\nmessage = greet('Alice')\nprint(message)  # Hello, Alice!\n```\n\nFunctions with Multiple Parameters:\n```python\ndef add_numbers(a, b):\n    result = a + b\n    return result\n\nsum_result = add_numbers(5, 3)  # 8\n```\n\nDefault Parameters:\n```python\ndef greet(name, greeting='Hello'):\n    return f'{greeting}, {name}!'\n\nprint(greet('Bob'))           # Hello, Bob!\nprint(greet('Bob', 'Hi'))     # Hi, Bob!\n```\n\nReturning Multiple Values:\n```python\ndef get_name_age():\n    return 'Alice', 25\n\nname, age = get_name_age()\nprint(f'{name} is {age} years old')\n```\n\nDocstrings (Function Documentation):\n```python\ndef calculate_area(length, width):\n    \"\"\"Calculate the area of a rectangle.\"\"\"\n    return length * width\n```";
                break;
            case 10:
                title = "Chapter 10: String Methods";
                content = "Case Conversion Methods:\n```python\ntext = 'Hello World'\nprint(text.upper())       # 'HELLO WORLD'\nprint(text.lower())       # 'hello world'\nprint(text.title())       # 'Hello World'\nprint(text.capitalize())  # 'Hello world'\nprint(text.swapcase())    # 'hELLO wORLD'\n```\n\nSearch and Check Methods:\n```python\nprint(text.find('World'))     # 6 (index position)\nprint(text.count('l'))        # 3 (occurrences)\nprint(text.startswith('H'))   # True\nprint(text.endswith('d'))     # True\nprint('Hello' in text)        # True\n```\n\nModification Methods:\n```python\ntext = '  Hello World  '\nprint(text.strip())           # 'Hello World' (remove whitespace)\nprint(text.replace('World', 'Python'))  # 'Hello Python'\n```\n\nSplit and Join:\n```python\nsentence = 'apple,banana,orange'\nfruits = sentence.split(',')  # ['apple', 'banana', 'orange']\njoined = ' - '.join(fruits)   # 'apple - banana - orange'\n```";
                break;
            case 11:
                title = "Chapter 11: File Handling";
                content = "Reading Files:\n```python\n# Method 1: Read entire file\nwith open('data.txt', 'r') as file:\n    content = file.read()\n    print(content)\n\n# Method 2: Read line by line\nwith open('data.txt', 'r') as file:\n    for line in file:\n        print(line.strip())  # Remove newline characters\n\n# Method 3: Read all lines into a list\nwith open('data.txt', 'r') as file:\n    lines = file.readlines()\n```\n\nWriting Files:\n```python\n# Write mode (overwrites existing content)\nwith open('output.txt', 'w') as file:\n    file.write('Hello World\\n')\n    file.write('Python is awesome!')\n\n# Write multiple lines\nlines = ['Line 1\\n', 'Line 2\\n', 'Line 3\\n']\nwith open('output.txt', 'w') as file:\n    file.writelines(lines)\n```\n\nAppending to Files:\n```python\nwith open('log.txt', 'a') as file:\n    file.write('New log entry\\n')\n```\n\nFile Modes:\n• 'r' - Read only (default)\n• 'w' - Write (overwrites existing)\n• 'a' - Append to end\n• 'r+' - Read and write";
                break;
            case 12:
                title = "Chapter 12: Exception Handling";
                content = "Basic try-except:\n```python\ntry:\n    number = int(input('Enter a number: '))\n    result = 10 / number\n    print(f'Result: {result}')\nexcept ValueError:\n    print('That\\'s not a valid number!')\nexcept ZeroDivisionError:\n    print('Cannot divide by zero!')\n```\n\nCatching Multiple Exceptions:\n```python\ntry:\n    # Code that might raise exceptions\n    value = int(input('Enter number: '))\n    result = 100 / value\nexcept (ValueError, ZeroDivisionError) as error:\n    print(f'Error occurred: {error}')\n```\n\nGeneral Exception Handling:\n```python\ntry:\n    # Risky code here\n    risky_operation()\nexcept Exception as e:\n    print(f'An error occurred: {e}')\nelse:\n    print('No errors occurred!')\nfinally:\n    print('This always runs')\n```\n\nRaising Custom Exceptions:\n```python\ndef check_age(age):\n    if age < 0:\n        raise ValueError('Age cannot be negative')\n    return age\n```";
                break;
            case 13:
                title = "Chapter 13: Classes & Objects";
                content = "Defining a Class:\n```python\nclass Dog:\n    # Class variable (shared by all instances)\n    species = 'Canis familiaris'\n    \n    def __init__(self, name, breed):\n        # Instance variables (unique to each object)\n        self.name = name\n        self.breed = breed\n        self.age = 0\n    \n    def bark(self):\n        return f'{self.name} says Woof!'\n    \n    def have_birthday(self):\n        self.age += 1\n        return f'{self.name} is now {self.age} years old'\n```\n\nCreating and Using Objects:\n```python\n# Create instances (objects)\nmy_dog = Dog('Buddy', 'Golden Retriever')\nother_dog = Dog('Max', 'Bulldog')\n\n# Use methods\nprint(my_dog.bark())           # Buddy says Woof!\nprint(my_dog.have_birthday())  # Buddy is now 1 years old\n\n# Access attributes\nprint(my_dog.name)             # Buddy\nprint(Dog.species)             # Canis familiaris\n```\n\nInheritance:\n```python\nclass Puppy(Dog):\n    def __init__(self, name, breed):\n        super().__init__(name, breed)\n        self.is_puppy = True\n    \n    def play(self):\n        return f'{self.name} is playing!'\n```";
                break;
            case 14:
                title = "Chapter 14: Modules & Packages";
                content = "Importing Built-in Modules:\n```python\nimport math\nprint(math.sqrt(16))         # 4.0\nprint(math.pi)               # 3.14159...\nprint(math.factorial(5))     # 120\n\n# Import specific functions\nfrom math import sqrt, pi, cos\nprint(sqrt(25))              # 5.0\n\n# Import with alias\nimport math as m\nprint(m.sin(m.pi/2))         # 1.0\n```\n\nUseful Built-in Modules:\n```python\n# Random numbers\nimport random\nprint(random.randint(1, 10))     # Random integer 1-10\nprint(random.choice(['a','b','c'])) # Random choice\n\n# Date and time\nimport datetime\nnow = datetime.datetime.now()\nprint(now.strftime('%Y-%m-%d %H:%M:%S'))\n\n# Operating system interface\nimport os\nprint(os.getcwd())               # Current directory\nprint(os.listdir('.'))           # List files\n```\n\nCreating Your Own Module:\n```python\n# Save as mymodule.py\ndef greet(name):\n    return f'Hello, {name}!'\n\nPI = 3.14159\n\n# In another file:\nimport mymodule\nprint(mymodule.greet('Alice'))\nprint(mymodule.PI)\n```";
                break;
            case 15:
                title = "Chapter 15: List Comprehensions";
                content = "Basic List Comprehension:\n```python\n# Traditional way\nsquares = []\nfor x in range(10):\n    squares.append(x**2)\n\n# List comprehension (more Pythonic)\nsquares = [x**2 for x in range(10)]\nprint(squares)  # [0, 1, 4, 9, 16, 25, 36, 49, 64, 81]\n```\n\nWith Conditions:\n```python\n# Even squares only\neven_squares = [x**2 for x in range(10) if x % 2 == 0]\nprint(even_squares)  # [0, 4, 16, 36, 64]\n\n# Positive numbers only\nnumbers = [-2, -1, 0, 1, 2, 3]\npositive = [n for n in numbers if n > 0]\nprint(positive)  # [1, 2, 3]\n```\n\nString Processing:\n```python\nwords = ['hello', 'world', 'python']\ncapitalized = [word.upper() for word in words]\nprint(capitalized)  # ['HELLO', 'WORLD', 'PYTHON']\n\n# Filter and transform\nsentence = 'The quick brown fox'\nlong_words = [word.upper() for word in sentence.split() if len(word) > 3]\nprint(long_words)  # ['QUICK', 'BROWN']\n```\n\nNested Comprehensions:\n```python\n# Create a 3x3 matrix\nmatrix = [[i*j for j in range(3)] for i in range(3)]\nprint(matrix)  # [[0,0,0], [0,1,2], [0,2,4]]\n```";
                break;
            case 16:
                title = "Chapter 16: Lambda Functions";
                content = "Lambda Function Basics:\n```python\n# Regular function\ndef square(x):\n    return x ** 2\n\n# Lambda equivalent (anonymous function)\nsquare_lambda = lambda x: x ** 2\nprint(square_lambda(5))  # 25\n\n# Multiple parameters\nadd = lambda x, y: x + y\nprint(add(3, 4))  # 7\n```\n\nUsing Lambda with Built-in Functions:\n```python\nnumbers = [1, 2, 3, 4, 5]\n\n# map() - apply function to all items\nsquared = list(map(lambda x: x**2, numbers))\nprint(squared)  # [1, 4, 9, 16, 25]\n\n# filter() - filter items based on condition\neven = list(filter(lambda x: x % 2 == 0, numbers))\nprint(even)  # [2, 4]\n\n# reduce() - reduce list to single value\nfrom functools import reduce\nsum_all = reduce(lambda x, y: x + y, numbers)\nprint(sum_all)  # 15\n```\n\nSorting with Lambda:\n```python\nstudents = [('Alice', 85), ('Bob', 92), ('Charlie', 78)]\n\n# Sort by grade (second element)\nsorted_by_grade = sorted(students, key=lambda x: x[1])\nprint(sorted_by_grade)\n\n# Sort by name length\nsorted_by_name_len = sorted(students, key=lambda x: len(x[0]))\nprint(sorted_by_name_len)\n```";
                break;
            case 17:
                title = "Chapter 17: Decorators";
                content = "Basic Decorator:\n```python\ndef my_decorator(func):\n    def wrapper():\n        print('Something before the function')\n        func()\n        print('Something after the function')\n    return wrapper\n\n@my_decorator\ndef say_hello():\n    print('Hello!')\n\nsay_hello()\n# Output:\n# Something before the function\n# Hello!\n# Something after the function\n```\n\nDecorator with Arguments:\n```python\ndef repeat(times):\n    def decorator(func):\n        def wrapper(*args, **kwargs):\n            for _ in range(times):\n                result = func(*args, **kwargs)\n            return result\n        return wrapper\n    return decorator\n\n@repeat(3)\ndef greet(name):\n    print(f'Hello, {name}!')\n\ngreet('Alice')  # Prints 3 times\n```\n\nTiming Decorator:\n```python\nimport time\n\ndef timing_decorator(func):\n    def wrapper(*args, **kwargs):\n        start = time.time()\n        result = func(*args, **kwargs)\n        end = time.time()\n        print(f'{func.__name__} took {end - start:.4f} seconds')\n        return result\n    return wrapper\n\n@timing_decorator\ndef slow_function():\n    time.sleep(1)\n    return 'Done!'\n```";
                break;
            case 18:
                title = "Chapter 18: Working with APIs";
                content = "Installing requests:\n```bash\npip install requests\n```\n\nBasic GET Request:\n```python\nimport requests\n\n# Simple GET request\nresponse = requests.get('https://api.github.com/users/octocat')\n\nif response.status_code == 200:\n    data = response.json()  # Parse JSON response\n    print(f\"Name: {data['name']}\")\n    print(f\"Public repos: {data['public_repos']}\")\nelse:\n    print(f'Error: {response.status_code}')\n```\n\nPOST Request with Data:\n```python\n# Sending JSON data\nuser_data = {\n    'name': 'John Doe',\n    'email': 'john@example.com',\n    'age': 30\n}\n\nresponse = requests.post(\n    'https://httpbin.org/post',\n    json=user_data\n)\n\nprint(response.json())\n```\n\nUsing Headers and Parameters:\n```python\nheaders = {\n    'Authorization': 'Bearer your-token-here',\n    'Content-Type': 'application/json'\n}\n\nparams = {\n    'page': 1,\n    'per_page': 10\n}\n\nresponse = requests.get(\n    'https://api.example.com/data',\n    headers=headers,\n    params=params\n)\n```\n\nError Handling:\n```python\ntry:\n    response = requests.get('https://api.example.com/data', timeout=5)\n    response.raise_for_status()  # Raises exception for bad status\n    data = response.json()\nexcept requests.exceptions.Timeout:\n    print('Request timed out')\nexcept requests.exceptions.RequestException as e:\n    print(f'Request failed: {e}')\n```"
