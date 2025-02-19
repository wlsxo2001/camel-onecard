//
// Created by jintae on 25. 2. 12.
//
#include "JokerCard.hpp"
#include <iostream>
#include <string>

JokerCard::JokerCard(std::string t) : type(t) {}

void JokerCard::display() const
{
    std::cout << type << " Joker";
}
// Type : Normal or Joker
std::string JokerCard::getType() const
{
    return "Joker";
}
// type : Color or Black & White
std::string JokerCard::getValue() const
{
    return type;
}

std::string JokerCard::getColor() const
{
    return type;
}

std::string JokerCard::getShape() const
{
    return "None";
}

std::string JokerCard::getFullInfo() const
{
    return type + " Joker";
}

int JokerCard::getAttackPower() const
{
    return (type == "Colored") ? 7 : (type == "Black & White") ? 5 : 0;
}

bool JokerCard::canPlayJoker(const std::string& dummyColor) const
{
    return (type == "Colored" && (dummyColor == "Red")) ||
           (type == "Black & White" && (dummyColor == "Black"));
}

void JokerCard::changeShape(const std::string& newShape)
{
}
void JokerCard::changeColor(const std::string& newColor)
{
}

void JokerCard::change7(const std::shared_ptr<Card>& dummyCard)
{
}