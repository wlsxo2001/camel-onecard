//
// Created by jintae on 25. 2. 12.
//
#include "NormalCard.hpp"

NormalCard::NormalCard(std::string c, std::string s, std::string v) : color(c), shape(s), value(v) {}

//card의 shape과 value 출력
void NormalCard::display() const
{
    ////std::cout << color << " " << shape << " " << value;
    //std::cout << shape << " " << value;
}

//card의 type("Normal"/"Joker") 반환
std::string NormalCard::getType() const
{
    return "Normal";
}

//card의 value("A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K") 반환
std::string NormalCard::getValue() const
{
    return value;
}

//card의 shape("♠", "♦", "♣", "♥") 반환
std::string NormalCard::getShape() const
{
    return shape;
}

//card의 color("Red", "Black") 반환
std::string NormalCard::getColor() const
{
    return color;
}

//card의 전체 정보 반환
std::string NormalCard::getFullInfo() const
{
    //return color + " " + shape + " " + value;
    return shape + " " + value;
}

//card의 공격수치 반환(공격카드 이외에는 모두 0)
int NormalCard::getAttackPower() const
{
    if (value == "2") return 2;
    if (value == "A") return 3;
    return 0;
}

//공격받은 상황에서 사용, 지금 나의 카드와 dummy 카드를 비교하여 방어 가능한 상황인지를 판단(true/false)
//                                                      what I have
bool NormalCard::canDefend(const std::shared_ptr<Card>& attackCard, const std::shared_ptr<Card>& dummyCard) const
{
    // return ((attackCard->getValue() == "2" || attackCard->getValue() == "A" || attackCard->getValue() == "3")
    //         && dummyCard->getShape() == attackCard->getShape());
            //when defend by using Joker
            //|| ( (attackCard->getColor()=="Colored" && dummyCard->getColor()=="Red") || (attackCard->getColor()=="Black & White" && dummyCard->getColor()=="Black") );
    return ((attackCard->getValue() == "2" || attackCard->getValue() == "A" || attackCard->getValue() == "3") &&
        ( attackCard->getValue() == dummyCard->getValue() || dummyCard->getShape() == attackCard->getShape()) );

}

//J,Q,K 의 사용 여부 반환
bool NormalCard::useJQK() const
{
    return ( value == "J" || value == "K" || value == "Q" );
    // if ( value == "J" || value == "K" ) return true;
    // else return false;
}

//현재 shape을 변경 ( change7 함수에서 사용)
void NormalCard::changeShape(const std::string& newShape)
{
    shape = newShape;
}

//현재 color를 변경 ( change7 함수에서 사용)
void NormalCard::changeColor(const std::string& newColor)
{
    color = newColor;
}

//7번 카드가 나왔을 때, 모양을 직접 변경 (현재는 사용X)
bool NormalCard::change7(const std::shared_ptr<Card>& dummyCard)
{
    if (dummyCard->getValue() != "7") return false;  // 7이 아니면 실행 X
    //currentPlayer->showHand();
    int choice;
    //std::cout << "변경할 shape을 선택하세요 (0: 유지 , 1: ♠, 2: ♦, 3: ♣, 4: ♥) : ";
    std::cin >> choice;

    switch (choice)
    {
    case 0:
        //std::cout << "shape을 유지합니다." << std::endl;
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
        //std::cout << "잘못된 입력입니다. 변경을 취소합니다." << std::endl;
        return false;
    }
    //std::cout << "카드가 " << dummyCard->getShape() << " (" << dummyCard->getColor() << ") 로 변경되었습니다." << std::endl;
    return true;
}
