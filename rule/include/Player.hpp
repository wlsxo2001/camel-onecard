//
// Created by jintae on 25. 2. 12.
//

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <iostream>
#include <vector>
#include <memory>
#include <sstream>
#include "Card.hpp"
#include "Deck.hpp"


class Player
{
protected:
    std::vector<std::shared_ptr<Card>> hand;
    std::string name;
    bool loser = false;
    bool winner = false;

public:
    Player(std::string n);
    void drawCard(std::shared_ptr<Card> card);
    void showHand();

    std::string getName();
    int getHandSize();
    std::vector<std::shared_ptr<Card>> getHand() const;

    void playCard(std::shared_ptr<Card> playedCard,Deck& deck);

    bool hasBothJokers();
    bool hasMore15();
    bool hasNoCards();

    bool canPlayCard(const std::shared_ptr<Card>& card, const std::shared_ptr<Card>& dummyCard, bool isFirstCard);
    bool checkGameOver(std::shared_ptr<Player> currentPlayer, std::vector<std::shared_ptr<Player>>& players, int currentPlayerIndex, Deck& deck);
    int checkLoseWin();

    std::shared_ptr<Card> optimalCard(std::shared_ptr<Card>& dummyCard , int cnt);
    std::shared_ptr<Card> KMSoptimalCard(std::shared_ptr<Card>& dummyCard, int cnt);
    std::shared_ptr<Card> optimalCardGaseong(std::shared_ptr<Card>& dummyCard , int cnt);
    std::shared_ptr<Card> optimalCardCw(std::shared_ptr<Card>& dummyCard , int cnt);
    std::shared_ptr<Card> optimalCardTaegun(std::shared_ptr<Card>& dummyCard, int cnt);

    std::shared_ptr<Card> counterCard(std::shared_ptr<Card>& dummyCard );
    std::shared_ptr<Card> counterCardGaseong(std::shared_ptr<Card>& dummyCard );
    std::shared_ptr<Card> KMScounterCard(std::shared_ptr<Card>& dummyCard);
    std::shared_ptr<Card> hyOptimalCard(std::shared_ptr<Card>& dummyCard , int cnt);

    bool card7change(const std::shared_ptr<Card>& dummyCard);
};

#endif // PLAYER_HPP

//