//
// Created by jintae on 25. 2. 12.
//
#include "NormalCard.hpp"

NormalCard::NormalCard(std::string c, std::string s, std::string v) : color(c), shape(s), value(v) {}

void NormalCard::display() const
{
    //std::cout << color << " " << shape << " " << value;
    std::cout << shape << " " << value;
}

std::string NormalCard::getType() const
{
    return "Normal";
}

std::string NormalCard::getValue() const
{
    return value;
}

std::string NormalCard::getShape() const
{
    return shape;
}

std::string NormalCard::getColor() const
{
    return color;
}

std::string NormalCard::getFullInfo() const
{
    //return color + " " + shape + " " + value;
    return shape + " " + value;
}

int NormalCard::getAttackPower() const
{
    if (value == "2") return 2;
    if (value == "A") return 3;
    return 0;
}
//                                                      what I have
bool NormalCard::canDefend(const std::shared_ptr<Card>& attackCard, const std::shared_ptr<Card>& dummyCard) const
{
    return ((attackCard->getValue() == "2" || attackCard->getValue() == "A" || attackCard->getValue() == "3")
            && dummyCard->getShape() == attackCard->getShape())
            //when defend by using Joker
            || ( (attackCard->getColor()=="Colored" && dummyCard->getColor()=="Red") || (attackCard->getColor()=="Black & White" && dummyCard->getColor()=="Black") );
}

// function to repeat one more turn
// Q can be ignored in 1 vs 1 playing
bool NormalCard::useJQK() const
{
    return ( value == "J" || value == "K" || value == "Q" );
    // if ( value == "J" || value == "K" ) return true;
    // else return false;
}

void NormalCard::changeShape(const std::string& newShape)
{
    shape = newShape;
}
void NormalCard::changeColor(const std::string& newColor)
{
    color = newColor;
}


bool NormalCard::change7(const std::shared_ptr<Card>& dummyCard)
{
    if (dummyCard->getValue() != "7") return false;  // 7이 아니면 실행 X
    //currentPlayer->showHand();
    int choice;
    std::cout << "변경할 shape을 선택하세요 (0: 유지 , 1: ♠, 2: ♦, 3: ♣, 4: ♥) : ";
    std::cin >> choice;

    switch (choice)
    {
    case 0:
        std::cout << "shape을 유지합니다." << std::endl;
        return false;  // 변경 없이 종료
    case 1:
        dummyCard->changeShape("♠");
        dummyCard->changeColor("Black");
        break;
    case 2:
        dummyCard->changeShape("♦");
        dummyCard->changeColor("Red");
        break;
    case 3:
        dummyCard->changeShape("♣");
        dummyCard->changeColor("Black");
        break;
    case 4:
        dummyCard->changeShape("♥");
        dummyCard->changeColor("Red");
        break;
    default:
        std::cout << "잘못된 입력입니다. 변경을 취소합니다." << std::endl;
        return false;
    }
    std::cout << "카드가 " << dummyCard->getShape() << " (" << dummyCard->getColor() << ") 로 변경되었습니다." << std::endl;
    return true;
}
