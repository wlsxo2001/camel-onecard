//
// Created by jintae on 25. 2. 12.
//
#include "JokerCard.hpp"
#include <iostream>
#include <string>

JokerCard::JokerCard(std::string t) : type(t) {}

//card의 shape과 value 출력
void JokerCard::display() const
{
    std::cout << type << " Joker";
}

//card의 type("Normal"/"Joker") 반환
std::string JokerCard::getType() const
{
    return "Joker";
}

//card의 value("Color", "Black & White") 반환
std::string JokerCard::getValue() const
{
    return type;
}

//card의 color("Color", "Black & White") 반환
std::string JokerCard::getColor() const
{
    return type;
}

//card의 shape("None") 반환
std::string JokerCard::getShape() const
{
    return "None";
}

//card의 전체 정보 반환
std::string JokerCard::getFullInfo() const
{
    return type + " Joker";
}

//card의 공격포인트 반환 (Joker는 4로 통일)
int JokerCard::getAttackPower() const
{
    //return (type == "Colored") ? 7 : (type == "Black & White") ? 5 : 0;
    return 4;
}

//Joker를 낼 수 있는 상황인지 판단
bool JokerCard::canPlayJoker(const std::string& dummyColor) const
{
    return (type == "Colored" && (dummyColor == "Red")) ||
           (type == "Black & White" && (dummyColor == "Black"));
}


//아래 함수들은 사용X
void JokerCard::changeShape(const std::string& newShape)
{
}
void JokerCard::changeColor(const std::string& newColor)
{
}

bool JokerCard::change7(const std::shared_ptr<Card>& dummyCard)
{
}