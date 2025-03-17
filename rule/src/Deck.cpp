#include "Deck.hpp"

#include <Game.hpp>
#include <random>

Deck::Deck()
{
    std::string shapes[] = {"♠", "♦", "♣", "♥"};
    std::string values[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};

    for (std::string shape : shapes)
    {
        std::string color = (shape == "♠" || shape == "♣") ? "Black" : "Red";
        for (std::string value : values)
        {
            cards.push_back(std::make_shared<NormalCard>(color, shape, value));
        }
    }

    cards.push_back(std::make_shared<JokerCard>("Colored"));
    cards.push_back(std::make_shared<JokerCard>("Black & White"));

    shuffle();
}

// deck에 있는 카드를 섞는 함서
void Deck::shuffle()
{
    std::cout << "카드 섞는중 ..." << std::endl;
    srand(time(0));
    if (timeOn)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(timeSleep));
    }
    std::shuffle(cards.begin(), cards.end(), std::mt19937(std::random_device()()));
    std::cout << "deck 에 남은 카드 수: " << cards.size() << std::endl;
    if (timeOn)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(timeSleep));
    }
}

// deck에서 카드(1장)를 뽑는 함수
std::shared_ptr<Card> Deck::draw()
{
    if (cards.empty())
    {
        emptyCnt=1;
        std::cout << "덱의 모든 카드가 소진되었습니다!" << std::endl;
        //return nullptr;
        if (timeOn)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(timeSleep));
        }
        std::cout << "사용된 카드를 다시 섞어 덱을 재구성합니다..." << std::endl;
        if (timeOn)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(timeSleep));
        }
        cards.insert(cards.end(), usedCard.begin(), usedCard.end());
        //game->eraseUsedAttackCards();
        usedCard.clear(); // usedCard 비우기
        shuffle();
    }

    auto card = cards.back();
    cards.pop_back();
    return card;
}

//deck이 비었을 때 true 반환하는 함수(deck 소진 시에 shuffle하기위해서)
bool Deck::deckShuffledCheck()
{
    if (emptyCnt==1)
    {
        emptyCnt=0;
        return true;
    }
    return false;
}

//deck이 비었는지 확인
bool Deck::isEmpty() const
{
    return cards.empty();
}

//deck의 크기(남은 카드 수) 반환
int Deck::getSize() const
{
    return cards.size();
}

//사용한 카드를 usedCard vector에 저장
void Deck::addUsedCard(std::shared_ptr<Card> playedCard)
{
    usedCard.push_back(playedCard);
}

//탈락자 발생 시 사용, 탈락자 hand에 있는 카드를 deck에 추가하여 다시 shuffle
void Deck::addDeck(std::vector<std::shared_ptr<Card>> loserCard)
{
    std::cout << "덱에 카드를 추가하고 다시 섞습니다.." << std::endl;
    if (timeOn)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(timeSleep));
    }
    cards.insert(cards.end(), loserCard.begin(), loserCard.end());
    shuffle();
}
