#include "Calculator.hpp"

//PUBLIC:
Calculator::Calculator()
{
    ShowWindow(GetConsoleWindow(), SW_HIDE);

    WindowSize = sf::VideoMode((sf::VideoMode::getDesktopMode().width/1.8)/2,
                               sf::VideoMode::getDesktopMode().height/1.2);

    WindowSizeH = sf::VideoMode((sf::VideoMode::getDesktopMode().width/1.8),
                               sf::VideoMode::getDesktopMode().height/1.2);

    PadButtonSize.x = (WindowSize.width/4)+0.25;
    PadButtonSize.y = PadButtonSize.x;

    DisplaySize.x = WindowSize.width;
    DisplaySize.y = WindowSize.height - (5 * PadButtonSize.y) - 42;

    OperationLineCharSize = DisplaySize.y/3;
    ResultLineCharSize = DisplaySize.y/6;

    DisplayPos.x = 0;
    DisplayPos.y = 40;

    font.loadFromFile("Resources/Fonts/Cyberverse.otf");
}

Calculator::~Calculator(){}

void Calculator::Open()
{
    createWindow();

    createHistoryObjects();

    PadCreate();

    displayCreate();

    while(Window.isOpen())
    {
        HandleEvents();

        Window.clear();

        if(HistoryOpen)
            showHistory();
        if(History.isButtonPressed(Window))
            openHistory();
        History.displayTo(Window);

        ButtonComands();

        PadShow();
        
        DisplayShow();

        Window.display();
    }
}
//PRIVATE:

void Calculator::createWindow()
{
    if(!HistoryOpen)
        Window.create(WindowSize, "Calculator", sf::Style::Close);
    else
        Window.create(WindowSizeH, "Calculator", sf::Style::Close);
    sf::Image Icon;
    Icon.loadFromFile("Resources/Icon/CalculatorIcon.png");
    Window.setIcon(Icon.getSize().x, Icon.getSize().y, Icon.getPixelsPtr());
}

void Calculator::PadCreate()
{
    float BigButton = 0;
    std::string ButtonText;
    for(int y = 0; y < 5; y++){
        float Ypos = WindowSize.height - ((y+1) * PadButtonSize.y) - 1;
        for(int x = 0; x < 4; x++)
        {
            if(y == 0)
            {
                if(x == 0) ButtonText = "%";
                if(x == 1) ButtonText = "0";
                if(x == 2) ButtonText = ".";
                if(x == 3) continue; 
            }
            if(y == 1)
            {
                if(x == 0) ButtonText = "1";
                if(x == 1) ButtonText = "2";
                if(x == 2) ButtonText = "3";
                if(x == 3)
                {
                    ButtonText = "=";
                    BigButton = PadButtonSize.y;
                } 
            }
            if(y == 2)
            {
                if(x == 0) ButtonText = "4";
                if(x == 1) ButtonText = "5";
                if(x == 2) ButtonText = "6";
                if(x == 3) ButtonText = "+";
            }
            if(y == 3)
            {
                if(x == 0) ButtonText = "7";
                if(x == 1) ButtonText = "8";
                if(x == 2) ButtonText = "9";
                if(x == 3) ButtonText = "-";
            }
            if(y == 4)
            {
                if(x == 0) ButtonText = "C";
                if(x == 1) ButtonText = "/";
                if(x == 2) ButtonText = "X";
                if(x == 3) ButtonText = "<";
            }
            float Xpos = x * PadButtonSize.x;
            Pad[y][x].loadFontFromFile("Resources/Fonts/Cyberverse.otf");
            Pad[y][x].hasOutline(-1);
            Pad[y][x].changeButtonOutlineColor(DetailColor);
            Pad[y][x].create(ButtonText, PadButtonSize.y*0.7, 
                             sf::Vector2f(PadButtonSize.x, PadButtonSize.y + BigButton), 
                             sf::Vector2f(Xpos,Ypos),MainColor, MainColorHover);
            Pad[y][x].changeButtonTextColor(DetailColor);
            BigButton = 0;
        }
    }
}

void Calculator::PadShow()
{
    for(int y = 0; y < 5; y++)
        for(int x = 0; x < 4; x++)
            Pad[y][x].displayTo(Window);
}

void Calculator::displayCreate()
{
    DisplayShape.setSize(DisplaySize);
    DisplayShape.setFillColor(MainColor);
    DisplayShape.setPosition(DisplayPos);
    
    OperationLine.setCharacterSize(OperationLineCharSize);
    ResultLine.setCharacterSize(ResultLineCharSize);

    OperationLine.setPosition(DisplayPos.x + 2, DisplayPos.y+2);
    ResultLine.setPosition(2, 2 + 2*OperationLine.getCharacterSize());

    OperationLine.setFillColor(DetailColor);
    ResultLine.setFillColor(DetailColor);

    OperationLine.setFont(font);
    ResultLine.setFont(font);

    OperationLine.setString(OperationString);
    
}

void Calculator::DisplayShow()
{
    if(OperationLine.getString() != OperationString)
    {
        
        OperationLine.setString(OperationString);
        std::string Op = OperationString;
        ResultString = std::to_string(readOperation(OperationString));
        tidyResult(ResultString);
        if(OperationString.empty()) ResultString.clear();
    }
    
    if(OperationLine.getPosition().x + OperationLine.getGlobalBounds().width > DisplaySize.x-2) 
        OperationLine.setCharacterSize(OperationLine.getCharacterSize() * 0.8);

    if(ResultLine.getString() != ResultString) 
        ResultLine.setString(ResultString);

    if(ResultLine.getPosition().x + ResultLine.getGlobalBounds().width > DisplaySize.x-2) 
        ResultLine.setCharacterSize(ResultLine.getCharacterSize() * 0.8);
    
    Window.draw(DisplayShape);
    Window.draw(OperationLine);
    Window.draw(ResultLine);
}

void Calculator::HandleEvents()
{
    sf::Event Event;
    while(Window.pollEvent(Event))
    {
        switch(Event.type)
        {
        case sf::Event::Closed:
            Window.close();
            break;
        case sf::Event::KeyReleased:
            switch(Event.key.code)
            {
            case sf::Keyboard::Numpad0:
                OperationString += '0';
                break;
            case sf::Keyboard::Numpad1:
                OperationString += '1';
                break;
            case sf::Keyboard::Numpad2:
                OperationString += '2';
                break;
            case sf::Keyboard::Numpad3:
                OperationString += '3';
                break;
            case sf::Keyboard::Numpad4:
                OperationString += '4';
                break;
            case sf::Keyboard::Numpad5:
                OperationString += '5';
                break;
            case sf::Keyboard::Numpad6:
                OperationString += '6';
                break;
            case sf::Keyboard::Numpad7:
                OperationString += '7';
                break;
            case sf::Keyboard::Numpad8:
                OperationString += '8';
                break;
            case sf::Keyboard::Numpad9:
                OperationString += '9';
                break;
            case sf::Keyboard::Add:
                if(CheckLastChar()) OperationString.pop_back();
                    OperationString += '+';
                    if(OperationString.length() == 1)
                        OperationString.pop_back();
                break;
            case sf::Keyboard::Subtract:
                if(CheckLastChar()) OperationString.pop_back();
                    OperationString += '-';
                break;
            case sf::Keyboard::Return:
                WriteToHistory();
                OperationString = ResultString;
                break;
            case sf::Keyboard::Multiply:
                if(CheckLastChar()) OperationString.pop_back();
                    OperationString += 'x';
                    if(OperationString.length() == 1)
                        OperationString.pop_back();
                break;
            case sf::Keyboard::Divide:
                if(CheckLastChar()) OperationString.pop_back();
                    OperationString += '/';
                    if(OperationString.length() == 1)
                        OperationString.pop_back();
                break;
            case sf::Keyboard::Period:
                if(OperationString.back() != '.')
                    OperationString += '.';
                break;
            case sf::Keyboard::BackSpace:
                if(!OperationString.empty())
                    OperationString.pop_back();
                if(OperationLine.getCharacterSize() / 0.8 < OperationLineCharSize)
                    OperationLine.setCharacterSize(OperationLineCharSize);
                if(ResultLine.getCharacterSize() / 0.8 < ResultLineCharSize)
                    ResultLine.setCharacterSize(ResultLineCharSize);
                break;
            case sf::Keyboard::H:
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
                    openHistory();
                break;
            default:
                break;
            }
            break;
        }
    }
}

void Calculator::ButtonComands()
{
//Percent
    if(Pad[0][0].isButtonPressed(Window))
    {
        if(OperationString.back() != '%')
            OperationString += '%';
    }
//Numbers
    if(Pad[0][1].isButtonPressed(Window))
    {
        OperationString += '0';
    }
    if(Pad[1][0].isButtonPressed(Window))
    {
        OperationString += '1';
    }
    if(Pad[1][1].isButtonPressed(Window))
    {
        OperationString += '2';
    }
    if(Pad[1][2].isButtonPressed(Window))
    {
        OperationString += '3';
    }
    if(Pad[2][0].isButtonPressed(Window))
    {
        OperationString += '4';
    }
    if(Pad[2][1].isButtonPressed(Window))
    {
        OperationString += '5';
    }
    if(Pad[2][2].isButtonPressed(Window))
    {
        OperationString += '6';
    }
    if(Pad[3][0].isButtonPressed(Window))
    {
        OperationString += '7';
    }
    if(Pad[3][1].isButtonPressed(Window))
    {
        OperationString += '8';
    }
    if(Pad[3][2].isButtonPressed(Window))
    {
        OperationString += '9';
    }

//Operations

    //Clear
    if(Pad[4][0].isButtonPressed(Window))
    {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
        {
            ConsoleOut("Debug Deactivated!\n");
            Debug = (Debug)? false : true;
            if(Debug)
                ShowWindow(GetConsoleWindow(), SW_RESTORE);
            else
                ShowWindow(GetConsoleWindow(), SW_HIDE);
            ConsoleOut("Debug Activated!\n");
        }
        OperationLine.setCharacterSize(OperationLineCharSize);
        OperationString.clear();
        ResultLine.setCharacterSize(ResultLineCharSize);
        ResultString.clear();
    }
    //Division
    if(Pad[4][1].isButtonPressed(Window))
    {
        if(CheckLastChar()) OperationString.pop_back();
        OperationString += '/';
        if(OperationString.length() == 1)
            OperationString.pop_back();
    }
    //Multiplication
    if(Pad[4][2].isButtonPressed(Window))
    {
        if(CheckLastChar()) OperationString.pop_back();
        OperationString += 'x';
        if(OperationString.length() == 1)
            OperationString.pop_back();
    }
    //Subtraction
    if(Pad[3][3].isButtonPressed(Window))
    {
        if(CheckLastChar()) OperationString.pop_back();
        OperationString += '-';
    }
    //Sumation
    if(Pad[2][3].isButtonPressed(Window))
    {
        if(CheckLastChar()) OperationString.pop_back();
        OperationString += '+';
        if(OperationString.length() == 1)
            OperationString.pop_back();
    }
    //Delete
    if(Pad[4][3].isButtonPressed(Window))
    {
        if(!OperationString.empty())
            OperationString.pop_back();
        if(OperationLine.getCharacterSize() / 0.8 < OperationLineCharSize)
            OperationLine.setCharacterSize(OperationLineCharSize);
        if(ResultLine.getCharacterSize() / 0.8 < ResultLineCharSize)
            ResultLine.setCharacterSize(ResultLineCharSize);
    }

//Point
    if(Pad[0][2].isButtonPressed(Window))
    {
        if(OperationString.back() != '.')
            OperationString += '.';
    }
//Equals
    if(Pad[1][3].isButtonPressed(Window))
    {
        WriteToHistory();
        OperationString = ResultString;
    }
}

bool Calculator::CheckLastChar()
{
    char c = OperationString.back();
    if(c == '+' || c == '-' || c == '/' || c == 'x') return true;
    return false;
}

double Calculator::readOperation(std::string operation)
{
    ConsoleOut("Calculating...\n" + operation + "\n");
    
    int lefti, righti;
    
    std::stringstream ss;
    std::string N1;
    std::string N2;
    
    double Num1, Num2;
    double re;
    
    Multiply = false;
    Divide = false;
    Sum = false;
    Subtract = false;
    
    //Look for percentages
    
    size_t f = operation.find('%');

    ConsoleOut("Looking for percentages\n");
    
    if(f != std::string::npos)
    {
        if(f == 0) ResultString = "Error";
        else
        {
            righti = f;
            lefti = f;
            for(int i = f-1; i > -1; i--)
            {
                char c = operation[i];
                if((c == '+' || c == '-' || c == '/' || c == 'x'))
                {
                    break;
                }
                N1 += c;
                lefti--;
            }
            ConsoleOut("Percentage number: " + N1 +'\n');
            if(!N1.empty())
            {
                ss.clear();
                ss << ReverseString(N1);
                ss >> Num1;
                Num1 = Num1 / 100;
                N1 = std::to_string(Num1);
                tidyResult(N1);
                operation.erase(operation.begin() +lefti, operation.begin()+righti+1);
                operation.insert(lefti, N1);
                ConsoleOut("Operation after percent detection:" + operation + '\n');
                if(operation == N1)
                {
                    return Num1;
                }
                return readOperation(operation);
            }
            else 
                ResultString = "Error";
        }
    }

    //Look for division then multiplication
    std::string Div_Mult = "/x";
    
    for(auto Operator: Div_Mult)
    {
        size_t f = operation.find(Operator);
        
        if(Operator == '/') 
            ConsoleOut("Looking for operation: Divison\n");
        if(Operator == 'x') 
            ConsoleOut("Looking for operation: Multiplication\n");
        
        if(f != std::string::npos)
        {
            if(Operator == '/') 
                Divide = true;
            if(Operator == 'x') 
                Multiply = true;
            
            int i = f;
            righti = i;
            for(int j = i+1; j < operation.length(); j++)
            {
                char c = operation[j];
                if((c == '+' || c == '-' || c == '/' || c == 'x'))
                    break;
                
                righti++;
                N2 += c;
            }
            if(N2.empty()) break;
            ss << N2;
            ss >> Num2;
            ss.clear();
            
            lefti = i;
            for(int j = i-1; j > -1; j--)
            {
                char c = operation[j];
                if((c == '+' || c == '-' || c == '/' || c == 'x'))
                    break;
            
                lefti--;
                N1 += c;
            }
            ss << ReverseString(N1);
            ss >> Num1;
            ss.clear();
            
            ConsoleOut(std::to_string(Num1) + Operator + std::to_string(Num2) + '\n');
            ConsoleOut("Left index: " + std::to_string(lefti) + '\n');
            ConsoleOut("Right index: " + std::to_string(righti) + '\n');
            
            operation.erase(operation.begin() +lefti, operation.begin()+righti+1);
            re = calculate(Num1, Num2);
            operation.insert(lefti, std::to_string(re));
            ConsoleOut("Result: " + std::to_string(re) + '\n');
            
            if(operation != std::to_string(re))
            {
                re = readOperation(operation);
            }
            
            return re;
        }
    }
    
    //If there are no divisions or multiplications read operation form left to right
    ss.clear();
    ConsoleOut("Reading form left to right!\n");
    char op;
    bool right = false;
    for(int i = 0; i < operation.length(); i++)
    {
        char c = operation[i]; 
        if(c == '+' || c == '-')
        {   
            if(!right)right = true;
            else if(!N2.empty())
            {
                ss.clear();
                ss << N1;
                ss >> Num1;
                ss.clear();
                N1.clear();
                ss << N2;
                ss >> Num2;
                ss.clear();
                N2.clear();
                ConsoleOut(std::to_string(Num1) + op + std::to_string(Num2) + '\n');
                Num1 = calculate(Num1, Num2);
                N1 = std::to_string(Num1);
                ConsoleOut("Result: " + N1 + '\n');
            }
            else
            {
                ConsoleOut("SECOND NUMBER NOT FOUND!\n");
            }

            if(c == '+')
            {
                ConsoleOut("Trying to sum!\n");
                op = '+';
                Sum = true;
            }
            else
            {
                ConsoleOut("Trying to subtract!\n");
                op = '-';
                Subtract = true;
            }
            continue;
        }
        (right)? N2 += c : N1 += c;
        
        
    }
    if(!N2.empty())
    {
        ss.clear();
        ss << N1;
        ss >> Num1;
        ss.clear();
        N1.clear();
        ss << N2;
        ss >> Num2;
        ss.clear();
        N2.clear();
        ConsoleOut(std::to_string(Num1) + op + std::to_string(Num2) + '\n');
        Num1 = calculate(Num1, Num2);
        N1 = std::to_string(Num1);
        ConsoleOut("Result: " + N1 + '\n');
    }
    else
    {
        ConsoleOut("SECOND NUMBER NOT FOUND!\n");
    }
    re = Num1;
    return re;
    
}

std::string Calculator::ReverseString(std::string str)
{
    ConsoleOut("String before reverse: " + str + '\n');
    int len = str.length();
    int n = len-1;
    int i = 0;
    while(i<=n)
    {
        std::swap(str[i],str[n]);
        n = n-1;
        i = i+1;
    }
    ConsoleOut("String after reverse: " + str + '\n');
    return str;
}

double Calculator::calculate(double N1, double N2)
{
    double re;
    if(Multiply)
    {
        re = N1 * N2;
        Multiply = false;
    }
    if(Divide)
    {
        re = N1 / N2;
        Divide = false;
    }
    if(Sum)
    {
        re = N1 + N2;
        Sum = false;
    }
    if(Subtract)
    {
        re = N1 - N2;
        Subtract = false;
    }
    return re;
}

void Calculator::tidyResult(std::string &ResultStr)
{
    while(ResultStr.back() == '0')
    {
        ResultStr.pop_back();
    }
    if(ResultStr.back() == '.') ResultStr.pop_back();
}

void Calculator::ConsoleOut(std::string Output)
{
    if(Debug)
    {
        std::cout << Output;
    }
}

void Calculator::createHistoryObjects()
{
    History.loadFontFromFile("Resources/Fonts/Cyberverse.otf");
    History.create("SHOW HISTORY >>", 30, 
                    sf::Vector2f(WindowSize.width, 40), 
                    sf::Vector2f(0,0),
                    MainColor, MainColorHover);
    History.changeButtonTextColor(DetailColor);
    
    RefreshButtonSize = sf::Vector2f((WindowSizeH.width - WindowSize.width)/2, 40);
    Refresh.loadFontFromFile("Resources/Fonts/Cyberverse.otf");
    Refresh.changeButtonTextColor(DetailColor);
    Refresh.create("REFRESH", 30, 
                    RefreshButtonSize, 
                    sf::Vector2f(WindowSize.width,0),
                    MainColor, MainColorHover);

    ClearHistory.loadFontFromFile("Resources/Fonts/Cyberverse.otf");
    ClearHistory.changeButtonTextColor(DetailColor);
    ClearHistory.create("CLEAR", 30, 
                    RefreshButtonSize, 
                    sf::Vector2f(WindowSize.width + RefreshButtonSize.x,0),
                    MainColor, MainColorHover);

    HistoryLog.setFont("Resources/Fonts/Cyberverse.otf");
    HistoryLog.create(sf::Vector2f(WindowSizeH.width-WindowSize.width, WindowSizeH.height-40),
                        sf::Vector2f(WindowSize.width, 40), 30);
    HistoryLog.changeBoxFillColor(MainColor);
    HistoryLog.changeTextFillColor(DetailColor);

}

void Calculator::openHistory()
{
    HistoryOpen = HistoryOpen? false : true;
    createWindow();
    if(!HistoryOpen)
        History.create("SHOW HISTORY >>", 30, sf::Vector2f(WindowSize.width, 40),
                        sf::Vector2f(0,0), MainColor, MainColorHover);
                        
    else
        History.create("HIDE HISTORY <<", 30, sf::Vector2f(WindowSize.width, 40),
                        sf::Vector2f(0,0), MainColor, MainColorHover);
}

void Calculator::ReadHistory()
{
    if(!HistoryRead)
    {
        HistoryLog.displayTextFromFile("History/History.txt");
        HistoryRead = true;
    }
}

void Calculator::WriteToHistory()
{
    if(!OperationString.empty() && !ResultString.empty())
    {
        std::ofstream file;
        file.open("History/History.txt", std::ios::app);
        if(file.is_open())
            file << OperationString << " = " << ResultString << '\n' << '\n';
        file.close();
        HistoryRead = false;
    }

}

void Calculator::clearHistory()
{
    std::ofstream file;
    file.open("History/History.txt");
    if(file.is_open())
        file.clear();
    file.close();
    HistoryRead = false;
}

void Calculator::showHistory()
{
    if(Refresh.isButtonPressed(Window))
        HistoryRead = false;
    if(ClearHistory.isButtonPressed(Window))
        clearHistory();
    
    ReadHistory();
    
    Refresh.displayTo(Window);
    ClearHistory.displayTo(Window);
    HistoryLog.displayTo(Window);
}