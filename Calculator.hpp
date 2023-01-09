#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

#include <windows.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include "UI_Button.hpp"
#include "UI_Scrollbar.hpp"
#include "UI_TextBox.hpp"

class Calculator
{
public:

Calculator();

~Calculator();

void Open();

private:

bool HistoryOpen = false;
bool Debug = false;

//Colors
sf::Color DetailColor = sf::Color(47,82,78);
sf::Color MainColor = sf::Color(229,235,221,255);
sf::Color MainColorHover = sf::Color(229,235,221,200);

std::stringstream ss;

bool Multiply = false;
bool Divide = false;
bool Sum = false;
bool Subtract = false;

sf::RenderWindow Window;

sf::VideoMode WindowSizeH;
sf::VideoMode WindowSize;

sf::Vector2f DisplaySize;
sf::Vector2f DisplayPos;
sf::RectangleShape DisplayShape;
sf::Font font;
sf::Text OperationLine;
float OperationLineCharSize;
sf::Text ResultLine;
float ResultLineCharSize;

sf::Vector2f PadButtonSize;
vui::Button Pad[5][4];

bool HistoryRead = false;
vui::Button History;
vui::Button Refresh;
vui::Button ClearHistory;
sf::Vector2f RefreshButtonSize;
vui::TextBox HistoryLog;

std::string OperationString;
std::string ResultString;

void createWindow();

void PadCreate();

void PadShow();

void displayCreate();

void DisplayShow();

void HandleEvents();

void ButtonComands();

bool CheckLastChar();

double readOperation(std::string operation);

std::string ReverseString(std::string str);

double calculate(double N1, double N2);

void tidyResult(std::string &ResultStr);

void ConsoleOut(std::string Output);

void createHistoryObjects();

void openHistory();

void ReadHistory();

void WriteToHistory();

void clearHistory();

void showHistory();

};


#endif