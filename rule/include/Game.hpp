//
// Created by jintae on 25. 2. 12.
//

#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <vector>
#include <memory>
#include <thread>
#include <chrono>
#include <sstream>
#include "Card.hpp"
#include "Deck.hpp"
#include "Player.hpp"

constexpr bool timeOn = false;
constexpr int timeSleep = 10;

class Game
{
private:
    Deck deck;
    std::vector<std::shared_ptr<Player>> players;
    std::shared_ptr<Card> dummyCard;
    bool reverseOrder = false;
    int attackStack = 0;
    std::vector<std::shared_ptr<Card>> usedAttackCards;  // 이미 사용된 공격 카드 추적
    std::vector<std::shared_ptr<Player>> ranking;
    std::vector<std::shared_ptr<Player>> losers;

public:
    Game();
    void start();
    void pushRanking(std::shared_ptr<Player>& currentPlayer);
    void printRanking();
    void eraseUsedAttackCards(); // deck을 다시 shuffle할 때 초기화 해줘야함.
};

#endif // GAME_HPP

