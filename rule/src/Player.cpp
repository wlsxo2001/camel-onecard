#include "Player.hpp"

#include "Game.hpp"


Player::Player(std::string n) : name(n) {}

//draw한 card를 hand에 저장
void Player::drawCard(std::shared_ptr<Card> card)
{
    hand.push_back(card);
}

//hand에 있는 모든 card 출력
void Player::showHand()
{
    //std::cout << name << "'s Hand (" << hand.size() << " cards): ";
    for (size_t i = 0; i < hand.size(); i++)
    {
        if (i > 0) //std::cout << " / ";
        hand[i]->display();
    }
    //std::cout << std::endl;
}

//현재 player의 이름 반환
std::string Player::getName()
{
    return name;
}

//현재 player의 hand에 card 수 반환
int Player::getHandSize()
{
    //if (hand.size() == 1) //std::cout << getName() << ": 원카드!" << std::endl;
    return hand.size();
}

//hand에 있는 모든 card 값 반환
std::vector<std::shared_ptr<Card>> Player::getHand() const
{
    return hand;
}

//사용한 카드를 usedCard에 추가하고 hand에서 제거
void Player::playCard(std::shared_ptr<Card> playedCard, Deck& deck)
{
    auto it = std::find(hand.begin(), hand.end(), playedCard);
    deck.addUsedCard(playedCard);
    if (it != hand.end())
    {
        hand.erase(it);
    }
}

//Joker카드 2장 보유 여부
bool Player::hasBothJokers()
{
    bool hasColored = false, hasBlackWhite = false;
    for (auto &card : hand)
    {
        if (card->getValue() == "Colored") hasColored = true;
        if (card->getValue() == "Black & White") hasBlackWhite = true;
    }
    return hasColored && hasBlackWhite;
}

//보유 카드 수 초과 여부 (현재는 10장으로 설정)
bool Player::hasMore15()
{
    return hand.size() >= CARD_LIM;
}

//hand의 카드 소진 여부
bool Player::hasNoCards()
{
    return hand.empty();
}

//hand의 card와 dummyCard를 비교하여 낼 수 있는 상황인지 판단(true/false)
bool Player::canPlayCard(const std::shared_ptr<Card>& card, const std::shared_ptr<Card>& dummyCard, bool isFirstCard)
{
    if (!isFirstCard) // 턴의 처음에는 shape, value 둘 중 하나만 같으면 됨. or canPlayJoker이거나
    {
        if (dummyCard->getType() == "Normal")
        {
            return card->getValue() == dummyCard->getValue() || card->getShape() == dummyCard->getShape() || card->canPlayJoker(dummyCard->getColor());
        }
        // if dummy is Joker, check only color
        else if (dummyCard->getType() == "Joker")
        {
            return (card->getColor()=="Red" && dummyCard->getColor()=="Colored") || (card->getColor()=="Black" && dummyCard->getColor()=="Black & White");
        }
    }
    else // 같은 턴의 두번째 카드부터는 value 가 같은 경우에만 제출 가능
    {
        return card->getValue() == dummyCard->getValue();
    }
}

//player의 승리/탈락 조건 확인
bool Player::checkGameOver(std::shared_ptr<Player> currentPlayer, std::vector<std::shared_ptr<Player>>& players, int currentPlayerIndex, Deck& deck)
{

    if (currentPlayer->hasMore15() || currentPlayer->hasNoCards() || currentPlayer->hasBothJokers())
    {
        //std::cout << "=============================================" << std::endl;

        if (currentPlayer->hasMore15())
        {
            //std::cout << currentPlayer->getName() << "is 10장 이상의 카드를 보유하여 탈락하였습니다!" << std::endl;
            //std::cout << currentPlayer->getName() << "의 Hand에 있던 카드는 모두 deck에 추가됩니다." << std::endl;
            //std::cout << "=============================================" << std::endl;
            deck.addDeck(currentPlayer->getHand());
            loser = true;

        }

        else if (currentPlayer->hasBothJokers())
        {
            //std::cout << currentPlayer->getName() << "is 두 개의 조커를 가지고 있어 탈락하였습니다!" << std::endl;
            // //std::cout << currentPlayer->getName() << "의 Hand에 있던 카드는 모두 deck에 추가됩니다." << std::endl;
            //std::cout << "=============================================" << std::endl;
            deck.addDeck(currentPlayer->getHand());
            loser = true;
        }

        else if (currentPlayer->hasNoCards())
        {
            //std::cout << currentPlayer->getName() << "is 모든 카드를 사용하여 승리하였습니다!" << std::endl;
            //std::cout << "(남은 사람이 있으면 순위 결정을 위해서 계속 진행합니다.)" << std::endl;
            //std::cout << "=============================================" << std::endl;
            winner = true;
        }

        //players.erase(players.begin() + currentPlayerIndex); // 플레이어 제거
        //erase되는 동시에 다음 플레이어가 현재 플레이어 index번호에 들어옴.
        return true;
    }
    return false;
}

// 승리자(2)/탈락자(1)/둘다아닌자(0) 구분
int Player::checkLoseWin()
{
    if (loser) return 1;
    else if (winner) return 2;
    return 0;
}

// 여기부터는 각자가 원하는 이름의 함수를 만들어야함 (구분이 용이하게 각자의 이름을 포함하는 것을 권함)
// 기본적으로 함수의 매개변수로는 dummyCard를 가져옴.
// 그러나 원한다면 상대 player가 지금까지 제출한 카드들에 대한 정보는 따로 함수를 또 만들어 저장해서 사용하여도됨.
// 엄중경고) 상대 player의 hand나 deck의 카드 등 open된 적이 없는 카드를 참조하는 함수를 사용해선 안됨.(부정행위임. cheating)

// 기본적으로 함수를 3가지 만들어야함
// 각 함수를 선언하는 곳은 Game 클래스에 주석으로 각각 설명되어있음.

// 1) 공격 받았을 때 사용하는 함수
// 방어 및 반격 가능 여부 확인 후 카드 제출 (각 player가 최적의 return값을 design해야함.)
std::shared_ptr<Card> Player::counterCard(std::shared_ptr<Card>& dummyCard )
{
    for (const auto& card : this->getHand())
    {
        if (card->canDefend(card, dummyCard))
        // canDefend를 굳이 한번 더 체크하지않아도됨. Game 클래스에서 한번 더 확인함.
        // but Game 클래스에서 canDefend가 만족되지않으면 바로 대응 실패로 간주됨.
        {
            return card;
        }
    }
    return nullptr;
}

std::shared_ptr<Card> Player::counterCardGaseong(std::shared_ptr<Card>& dummyCard )
{
    for (const auto& card : this->getHand())
    {
        if (card->canDefend(card, dummyCard))
            // canDefend를 굳이 한번 더 체크하지않아도됨. Game 클래스에서 한번 더 확인함.
                // but Game 클래스에서 canDefend가 만족되지않으면 바로 대응 실패로 간주됨.
        {
            return card;
        }
    }
    return nullptr;
}

std::shared_ptr<Card> Player::KMScounterCard(std::shared_ptr<Card>& dummyCard)
{
    std::shared_ptr<Card> bestCard = nullptr;
    int minAttack = 1000;
    for (const auto& card : this->getHand())
    {
        if (card->canDefend(card, dummyCard))
        {
            int attackPower = card->getAttackPower();
            if (attackPower < minAttack)
            {
                bestCard = card;
                minAttack = attackPower;
            }
        }
    }
    if (!bestCard)
    {
        for (const auto& card : this->getHand())
        {
            if (card->getType() == "Joker")
            {
                return card;
            }
        }
    }
    return bestCard;
}


// 2) 평시(공격 받은게 아닐 때) 사용하는 함수
// 어떤카드를 낼것인지 (각 player가 최적의 return값을 design해야함.)
std::shared_ptr<Card> Player::optimalCard(std::shared_ptr<Card>& dummyCard , int cnt) // cnt는 턴 내에서 첫번째로 내는 카드인지 구분하기 위함.
{
    for (const auto& card : this->getHand())
    {
        if (this->canPlayCard(card, dummyCard, cnt))
        // canPlayCard를 굳이 한번 더 체크하지않아도됨. Game 클래스에서 한번 더 확인함.
        // but Game 클래스에서 canPlayCard가 만족되지않으면 낼 카드가 없다고 간주됨.
        {
            return card;
        }
    }
    return nullptr;
}

// number : string으로 들어오고 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, J, Q, K
// shape :

// std::string shapes[] = {"♠", "♦", "♣", "♥"};
// std::string values[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};

// int shapeCount[] = {0, 0, 0, 0}

std::shared_ptr<Card> Player::optimalCardGaseong(std::shared_ptr<Card>& dummyCard , int cnt) // cnt는 턴 내에서 첫번째로 내는 카드인지 구분하기 위함.
{
    std::vector<std::shared_ptr<Card>> playableNormalCard;
    std::vector<std::shared_ptr<Card>> playableAttackCard;
    std::vector<std::shared_ptr<Card>> playableDefenceCard;
    std::vector<std::shared_ptr<Card>> playableJokerCard;
    std::vector<std::shared_ptr<Card>> playableKingCard;
    std::vector<std::shared_ptr<Card>> playableLastCard;

    std::string shapes[] = {"♠", "♦", "♣", "♥"};
    std::string values[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};


    for (const auto& card : this->getHand())
    {
        if (this->canPlayCard(card, dummyCard, cnt))
        {
            if (card -> getType() == "Joker")
            {
                playableJokerCard.push_back(card);
                // return card;
            }

            if (card -> getType() == "K")
            {
                playableKingCard.push_back(card);
                // return card;
            }

            if (card -> getType() == "A" or card -> getType() == "2")
            {
                playableAttackCard.push_back(card);
            }

            if (card -> getType() == "J" or card -> getType() == "Q")
            {
                playableLastCard.push_back(card);
            }

            if (card -> getValue() == "3")
            {
                playableDefenceCard.push_back(card);
            }
            else
            {
                playableNormalCard.push_back(card);
            }
        }
    }


    for (const auto& card: playableJokerCard)
    {
        return card;
    }

    for (const auto& card: playableKingCard)
    {
        return card;
    }

    for (const auto& card: playableAttackCard)
    {
        return card;
    }

    for (const auto& card : playableNormalCard)
    {
        return card;
    }

    for (const auto& card: playableLastCard)
    {
        return card;
    }

    for (const auto& card: playableDefenceCard)
    {
        return card;
    }

    return nullptr;
}

std::shared_ptr<Card> Player::KMSoptimalCard(std::shared_ptr<Card>& dummyCard, int cnt)
{
    {
        for (const auto& card : this->getHand())
        {
            if (this->canPlayCard(card, dummyCard, cnt) && card->getAttackPower() > 0)
            {
                return card;
            }
            else if (this->canPlayCard(card, dummyCard, cnt))
            {
                return card;
            }
        }
        return nullptr;
    }
}

std::shared_ptr<Card> Player::optimalCardCw(std::shared_ptr<Card>& dummyCard , int cnt) // cnt는 턴 내에서 첫번째로 내는 카드인지 구분하기 위함.
{

    std::unordered_map<std::string, int> customOrder = {
        {"K", 1}, {"Colored", 2}, {"Black & White", 3}, {"A", 4}, {"2", 5}, {"7", 6}, {"5", 7},
        {"6", 8}, {"8", 9}, {"9", 10}, {"10", 11}, {"4", 12}, {"J", 13},
        {"Q", 14}, {"3", 15}
    };

    // std::unordered_map<std::string, int> customOrder = {
    //     {"J", 1}, {"Colored", 2}, {"Black & White", 3}, {"7", 4}, {"2", 5}, {"4", 6}, {"5", 7},
    //     {"6", 8}, {"8", 9}, {"9", 10}, {"10", 11}, {"Q", 12}, {"K", 13},
    //     {"A", 14}, {"3", 15}
    // };

    std::sort(hand.begin(), hand.end(), [&](const std::shared_ptr<Card>& a, const std::shared_ptr<Card>& b)
    {
        return customOrder[a->getValue()] < customOrder[b->getValue()];
    });

    for (const auto& card : this->getHand())
    {
        if (this->canPlayCard(card, dummyCard, cnt))
            return card;
    }
    return nullptr;
}

std::shared_ptr<Card> Player::optimalCardTaegun(std::shared_ptr<Card>& dummyCard , int cnt) // cnt는 턴 내에서 첫번째로 내는 카드인지 구분하기 위함.
{
    std::vector<std::shared_ptr<Card>> playableNormalCard;
    std::string shapes[] = {"♠", "♦", "♣", "♥"};
    std::string values[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};

    for (const auto& card : this->getHand())
    {
        if (this->canPlayCard(card, dummyCard, cnt))
        {
            if (card->getType() == "Joker")
            {
                return card;
            }
            else
            {
                playableNormalCard.push_back(card);
            }
        }
    }

    int shapeCount[4] = {0};
    int numberCount[13] = {0};

    for (auto& card : playableNormalCard)
    {
        if (card->getShape() == "♠") { shapeCount[0]++; }
        else if (card->getShape() == "♦") { shapeCount[1]++; }
        else if (card->getShape() == "♣") { shapeCount[2]++; }
        else if (card->getShape() == "♥") { shapeCount[3]++; }
        if (card->getValue() == "A") { numberCount[0]++; }
        else if (card->getValue() == "2") { numberCount[1]++; }
        else if (card->getValue() == "3") { numberCount[2]++; }
        else if (card->getValue() == "4") { numberCount[3]++; }
        else if (card->getValue() == "5") { numberCount[4]++; }
        else if (card->getValue() == "6") { numberCount[5]++; }
        else if (card->getValue() == "7") { numberCount[6]++; }
        else if (card->getValue() == "8") { numberCount[7]++; }
        else if (card->getValue() == "9") { numberCount[8]++; }
        else if (card->getValue() == "10") { numberCount[9]++; }
        else if (card->getValue() == "J") { numberCount[10]++; }
        else if (card->getValue() == "Q") { numberCount[11]++; }
        else if (card->getValue() == "K") { numberCount[12]++; }
    }
    int maxShapeIdx = std::distance(shapeCount, std::max_element(shapeCount, shapeCount + 4));
    int maxNumberIdx = std::distance(numberCount, std::max_element(numberCount, numberCount + 13));

    for (auto& card : playableNormalCard)
    {
        if (card->getValue() == "2" || card->getValue() == "A")
        {
            return card; // 공격 카드 우선 사용
        }
    }

    if (shapeCount[maxShapeIdx] > numberCount[maxNumberIdx])
    {
        for (auto& card : playableNormalCard)
        {
            if (card->getShape() == shapes[maxShapeIdx])
            {
                return card;
            }
        }
    }
    else
    {
        for (auto& card : playableNormalCard)
        {
            if (card->getValue() == values[maxNumberIdx])
            {
                return card;
            }
        }
    }

    return nullptr;
}

std::shared_ptr<Card> Player::hyOptimalCard(std::shared_ptr<Card>& dummyCard , int cnt) // cnt는 턴 내에서 첫번째로 내는 카드인지 구분하기 위함.
{
    std::vector<std::shared_ptr<Card>> cards1,cards2,cards3;
    for (const auto& card : this->getHand())
    {
        if (this->canPlayCard(card, dummyCard, cnt))
            // canPlayCard를 굳이 한번 더 체크하지않아도됨. Game 클래스에서 한번 더 확인함.
                // but Game 클래스에서 canPlayCard가 만족되지않으면 낼 카드가 없다고 간주됨.
        {
            if (card->getValue() == "2" || card->getValue()=="A") {
                cards1.push_back(card);
            }
            else if (card->getValue() == "3") {
                cards2.push_back(card);
            }
            else {
                cards3.push_back(card);
            }
        }
    }
    if (cards3.empty()) {
        if (cards2.empty()) {
            if (cards1.empty())
                return nullptr;
            return cards1.front();
        }
        return cards2.front();
    }
    return cards3.front();
}



// 3) 7번 카드가 나왔을 때 처리 함수
// 일단은 hand내에서 최빈 shape으로 바꾸도록 해놨는데 뭔가 제대로 안됨
// shape 변경 여부 확인을 위해서 (턴 계산에 필요) bool로 return
bool Player::card7change(const std::shared_ptr<Card>& dummyCard)
{
    if (dummyCard->getValue() != "7") return false;  // 7이 아니면 실행 X

    std::unordered_map<std::string, int> shapeCount;
    //                      first   second
    // 각 shape의 빈도수 계산
    for (const auto& card : hand)
    {
        shapeCount[card->getShape()]++;
    }

    // 최빈 shape 찾기
    std::string mostFrequentShape;
    int maxCount = 0;

    for (const auto& pair : shapeCount)
    {
        if (pair.second > maxCount)
        {
            maxCount = pair.second;
            mostFrequentShape = pair.first;
        }
    }

    if ( mostFrequentShape == dummyCard->getShape() )
    {
        ////std::cout << "카드의 모양을 변경하지 않습니다." << std::endl; //탬플릿 통일을 위해서 Game class에서 실행됨
        return false;
    }

    // 최빈 shape로 변경
    if (!mostFrequentShape.empty())
    {
        dummyCard->changeShape(mostFrequentShape);
        ////std::cout << "카드의 모양이 " << mostFrequentShape << "로 변경되었습니다." << std::endl; //탬플릿 통일을 위해서 Game class에서 실행됨
        return true;
    }
    return false;
}