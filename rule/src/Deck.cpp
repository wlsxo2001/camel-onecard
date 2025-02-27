#include "Deck.hpp"

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

void Deck::shuffle()
{
    std::cout << "카드 섞는중 ..." << std::endl;
    srand(time(0));
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::random_shuffle(cards.begin(), cards.end());
    std::cout << "deck 에 남은 카드 수: " << cards.size() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

std::shared_ptr<Card> Deck::draw()
{
    if (cards.empty())
    {
        emptyCnt=1;
        std::cout << "덱의 모든 카드가 소진되었습니다!" << std::endl;
        //return nullptr;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "사용된 카드를 다시 섞어 덱을 재구성합니다..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        cards.insert(cards.end(), usedCard.begin(), usedCard.end());
        //game->eraseUsedAttackCards();
        usedCard.clear(); // usedCard 비우기
        shuffle();
    }

    auto card = cards.back();
    cards.pop_back();
    return card;
}

bool Deck::deckShuffledCheck()
{
    if (emptyCnt==1)
    {
        emptyCnt=0;
        return true;
    }
    return false;
}

bool Deck::isEmpty() const
{
    return cards.empty();
}

int Deck::getSize() const
{
    return cards.size();
}

void Deck::addUsedCard(std::shared_ptr<Card> playedCard)
{
    usedCard.push_back(playedCard);
}

void Deck::addDeck(std::vector<std::shared_ptr<Card>> loserCard)
{
    std::cout << "덱에 카드를 추가하고 다시 섞습니다.." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    cards.insert(cards.end(), loserCard.begin(), loserCard.end());
    shuffle();
}
