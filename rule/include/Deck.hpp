//
// Created by jintae on 25. 2. 12.
//

#ifndef DECK_HPP
#define DECK_HPP

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <ctime>
#include "Card.hpp"
#include "NormalCard.hpp"
#include "JokerCard.hpp"


class Deck
{
private:
    std::vector<std::shared_ptr<Card>> cards;
    std::vector<std::shared_ptr<Card>> usedCard;
    int emptyCnt=0;

public:
    Deck();

    void shuffle();
    std::shared_ptr<Card> draw();
    bool deckShuffledCheck();
    bool isEmpty() const;
    int getSize() const;
    void addUsedCard(std::shared_ptr<Card> playedCard);
    void addDeck(std::vector<std::shared_ptr<Card>> loserCard);
};

#endif // DECK_HPP

