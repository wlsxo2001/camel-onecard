#include "Player.hpp"


Player::Player(std::string n) : name(n) {}

void Player::drawCard(std::shared_ptr<Card> card)
{
    hand.push_back(card);
}

void Player::showHand()
{
    std::cout << name << "'s Hand (" << hand.size() << " cards): ";
    for (size_t i = 0; i < hand.size(); i++)
    {
        if (i > 0) std::cout << " / ";
        hand[i]->display();
    }
    std::cout << std::endl;
}

std::string Player::getName()
{
    return name;
}

int Player::getHandSize()
{
    //if (hand.size() == 1) std::cout << getName() << ": 원카드!" << std::endl;
    return hand.size();
}

std::vector<std::shared_ptr<Card>> Player::getHand() const
{
    return hand;
}

void Player::playCard(std::shared_ptr<Card> playedCard, Deck& deck) //사용했던 카드를 usedCard에 추가하고 hand에서 제거
{
    auto it = std::find(hand.begin(), hand.end(), playedCard);
    deck.addUsedCard(playedCard);
    if (it != hand.end())
    {
        hand.erase(it);
    }
}

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

bool Player::hasMore15()
{
    return hand.size() >= 10;
}

bool Player::hasNoCards()
{
    return hand.empty();
}

bool Player::canPlayCard(const std::shared_ptr<Card>& card, const std::shared_ptr<Card>& dummyCard, bool isFirstCard)
{
    if (!isFirstCard) // 턴 의처음에 는 shape, value 둘 중 하나만 같으면 됨. or canPlayJoker이거나
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
    else // 두번째 이후의 턴에서는 value 가 같은 경우에만 가능
    {
        return card->getValue() == dummyCard->getValue();
    }
}


bool Player::checkGameOver(std::shared_ptr<Player> currentPlayer, std::vector<std::shared_ptr<Player>>& players, int currentPlayerIndex, Deck& deck)
{

    if (currentPlayer->hasMore15() || currentPlayer->hasNoCards() || currentPlayer->hasBothJokers())
    {
        std::cout << "=============================================" << std::endl;

        if (currentPlayer->hasMore15())
        {
            std::cout << currentPlayer->getName() << "is 10장 이상의 카드를 보유하여 탈락하였습니다!" << std::endl;
            std::cout << currentPlayer->getName() << "의 Hand에 있던 카드는 모두 deck에 추가됩니다." << std::endl;
            std::cout << "=============================================" << std::endl;
            deck.addDeck(currentPlayer->getHand());
            loser = true;

        }

        else if (currentPlayer->hasBothJokers())
        {
            std::cout << currentPlayer->getName() << "is 두 개의 조커를 가지고 있어 탈락하였습니다!" << std::endl;
            std::cout << currentPlayer->getName() << "의 Hand에 있던 카드는 모두 deck에 추가됩니다." << std::endl;
            std::cout << "=============================================" << std::endl;
            deck.addDeck(currentPlayer->getHand());
            loser = true;
        }

        else if (currentPlayer->hasNoCards())
        {
            std::cout << currentPlayer->getName() << "is 모든 카드를 사용하여 승리하였습니다!" << std::endl;
            std::cout << "(남은 사람이 있으면 순위 결정을 위해서 계속 진행합니다.)" << std::endl;
            std::cout << "=============================================" << std::endl;
            winner = true;
        }

        //players.erase(players.begin() + currentPlayerIndex); // 플레이어 제거
        //erase되는 동시에 다음 플레이어가 현재 플레이어 index번호에 들어옴.
        return true;
    }
    return false;
}


// 방어 및 반격 가능 여부 확인 후 카드 제출 (각 player가 최적의 return값을 design해야함.)

std::shared_ptr<Card> Player::counterCard(std::shared_ptr<Card>& dummyCard )
{
    for (const auto& card : this->getHand())
    {
        if (card->canDefend(card, dummyCard)) // canDefend by Defense card or Attack card
        {
            return card;
        }
    }
    return nullptr;
}


// 어떤카드를 낼것인지 (각 player가 최적의 return값을 design해야함.)
std::shared_ptr<Card> Player::optimalCard(std::shared_ptr<Card>& dummyCard , int cnt) // cnt는 턴 내에서 첫번째로 내는 카드인지 구분하기 위함.
{
    for (const auto& card : this->getHand())
    {
        if (this->canPlayCard(card, dummyCard, cnt))
        {
            return card;
        }
    }
    return nullptr;
}


int Player::checkLoseWin()
{
    if (loser) return 1;
    else if (winner) return 2;
    return 0;
}