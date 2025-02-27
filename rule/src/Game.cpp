#include "Game.hpp"

Game::Game()
{
    std::cout << "게임 시작!" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::string input;
    std::cout << "Player 이름을 ','로 구분하여 입력하세요: ";
    std::getline(std::cin, input);

    // 입력된 문자열을 ',' 기준으로 파싱
    std::stringstream ss(input);
    std::string playerName;

    while (std::getline(ss, playerName, ','))
    {
        // 공백 제거 (이름 앞뒤의 공백을 없애기)
        playerName.erase(0, playerName.find_first_not_of(" \t\n\r"));
        playerName.erase(playerName.find_last_not_of(" \t\n\r") + 1);

        if (!playerName.empty())
        {
            players.push_back(std::make_shared<Player>(playerName));
        }
    }

    // 생성된 플레이어 출력
    std::cout << "\n생성된 플레이어 목록:\n";
    for (const auto& player : players)
    {
        std::cout << player->getName() << "/";
    }
    std::cout << "\b" <<std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "\n플레이 순서:\n";
    for (const auto& player : players)
    {
        std::cout << player->getName() << "->";
    }
    std::cout << "\b\b" <<std::endl << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "인당 5장씩 카드를 배분합니다.." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    for (auto &player : players)
    {
        for (int i = 0; i < 5; i++)
        {
            player->drawCard(deck.draw()); // 5장 뽑기
        }
        player->showHand();
    }
    dummyCard = deck.draw();
    deck.addUsedCard(dummyCard);
    do
    {
        if (dummyCard->getAttackPower() > 0)
        {
            std::cout << "현재 더미 카드: " << std::endl;
            dummyCard->display();
            std::cout << "더미의 첫 카드가 공격카드이므로 다시 뽑습니다." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            dummyCard = deck.draw();
            deck.addUsedCard(dummyCard);
        }
    }while (dummyCard->getAttackPower() != 0);
}

// ranking을 만드는 함수 (승리자, 탈락자 구분하여 저장)
void Game::pushRanking(std::shared_ptr<Player>& currentPlayer)
{
    if (currentPlayer->checkLoseWin() == 1) // 패배한 경우
    {
        losers.push_back(currentPlayer);
    }
    else if (currentPlayer->checkLoseWin() == 2) // 승리한 경우
    {
        ranking.push_back(currentPlayer);
    }
}

// 최종 ranking 출력 함수
void Game::printRanking()
{
    // losers에 남아 있는 플레이어가 있다면 ranking에 합치기
    if (!losers.empty())
    {
        std::reverse(losers.begin(), losers.end()); // 먼저 탈락한 사람이 ranking의 뒤로 가도록 정렬
        ranking.insert(ranking.end(), losers.begin(), losers.end()); // ranking 뒤에 losers 추가
        losers.clear(); // 중복 방지
    }

    std::cout << "=========최종순위=========" << std::endl;
    for (size_t i=0; i<ranking.size() ; i++)
    {
        std::cout << "|      " << i+1 << "등 : ";
        std::cout << (ranking[i])->getName() << "        |"<< std::endl;
    }

    std::cout << "========================" << std::endl;
}

void Game::eraseUsedAttackCards()
{
    if (deck.deckShuffledCheck())
    {
        usedAttackCards.erase(usedAttackCards.begin(),usedAttackCards.end());
    }
}


//진태,찬우,가성,지희,민성
void Game::start()
{
    size_t currentPlayerIndex = 0;
    int attackStack = 0;

    while (true)
    {

        std::cout << "================================" << std::endl;
        std::cout << "현재 더미 카드: ";
        dummyCard->display();
        std::cout << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "deck 에 남은 카드 수: " << deck.getSize() << std::endl;
        std::cout << "================================" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));

        bool defended = false;
        auto currentPlayer = players[currentPlayerIndex];
        int Jcnt = 0;
        int Qcnt = 0;
        //attackStack 은 공격카드를 낼 때 누적시킴
        // 공격 카드 처리
        if (dummyCard->getAttackPower() > 0 && std::find(usedAttackCards.begin(), usedAttackCards.end(), dummyCard) == usedAttackCards.end())
        {
            // 조커 공격 처리 , 조커는 무조건 1장씩이므로 attackStack 누적을 고려할 필요없음.
            if (dummyCard->getType() == "Joker")
            {
                std::cout << currentPlayer->getName() << " is Joker 공격을 당해 " << dummyCard->getAttackPower() << "장의 카드를 뽑아야 합니다." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));
                for (int i = 0; i < dummyCard->getAttackPower(); i++)
                {
                    auto drawnCard = deck.draw();
                    eraseUsedAttackCards();
                    currentPlayer->drawCard(drawnCard);
                    std::cout << currentPlayer->getName() << " is 덱에서 " << drawnCard->getFullInfo() << "를 뽑았습니다." << std::endl;
                }
                currentPlayer->showHand();
                std::this_thread::sleep_for(std::chrono::seconds(1));
                attackStack = 0;
                usedAttackCards.push_back(dummyCard);
            }

            else // 일반 공격카드 대응
            {
                //방어 및 반격 가능 여부 확인 및 카드 제출
                std::shared_ptr<Card> card = currentPlayer->counterCard(dummyCard); // 최적의 카드 선택(각자 player가 함수를 design)
                // //player에 따라 다른 최적화 함수를 사용하게되면 이런식으로 하면 될듯
                // //각자가 만든 최적화 함수를 사용하도록
                // //필요한 정보들(ex.상대방이 냈던 카드들)은 각자의 방식으로 함수의 매개변수로 끌어다쓰면됨.(cheating이 아닌 범주 안에서)
                // if (currentPlayer->getName()=="진태")
                // {
                //     std::shared_ptr<Card> card = currentPlayer->counterCard(dummyCard);
                // }
                // else if (currentPlayer->getName()=="찬우")
                // {
                //     std::shared_ptr<Card> card = currentPlayer->myCounter(dummyCard);
                // }
                if (card && card->canDefend(card,dummyCard)) //counterCard로 return된 card가 제출 가능한지 확인 필요
                {
                    defended = true;
                    currentPlayer->playCard(card,deck);//                 반격 시                       방어 시
                    attackStack = (card->getAttackPower() > 0) ? attackStack + card->getAttackPower() : 0;
                    dummyCard = card;
                    std::cout << currentPlayer->getName() << " is " << card->getFullInfo() << "로 대응했습니다." << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    currentPlayer->showHand();
                    if ( card->getValue() == "3" ) attackStack = 0; // else make higher attackStack
                    dummyCard->display();
                    std::cout << std::endl;
                }

                // 대응 실패 시 카드 뽑기
                if (!defended)
                {
                    std::cout << currentPlayer->getName() << " is 공격당해 " << attackStack << "장의 카드를 뽑습니다." << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    for (int i = 0; i < attackStack; i++)
                    {
                        auto drawnCard = deck.draw();
                        eraseUsedAttackCards();
                        currentPlayer->drawCard(drawnCard);
                        std::cout << currentPlayer->getName() << " is 덱에서 " << drawnCard->getFullInfo() << "를 뽑았습니다." << std::endl;
                    }
                    currentPlayer->showHand();
                    usedAttackCards.push_back(dummyCard);
                    attackStack = 0;
                }
            }
        }

        else
        {
            // 일반 카드 상대할 때 플레이
            bool played = false;
            int cnt = 0;
            Qcnt = 0;
            int Kcnt = 0;
            int change7cnt = 0;
            Jcnt = 0;
            do {
                played = false;
                // 어떤카드를 낼것인지
                std::shared_ptr<Card> card = currentPlayer->optimalCard(dummyCard, cnt);
                // //player에 따라 다른 최적화 함수를 사용하게되면 이런식으로 하면 될듯
                // //각자가 만든 최적화 함수를 사용하도록
                // //필요한 정보들(ex.상대방이 냈던 카드들)은 각자의 방식으로 함수의 매개변수로 끌어다쓰면됨.(cheating이 아닌 범주 안에서)
                // if (currentPlayer->getName()=="진태")
                // {
                //     std::shared_ptr<Card> card = currentPlayer->optimalCard(dummyCard, cnt);
                // }
                // else if (currentPlayer->getName()=="찬우")
                // {
                //     std::shared_ptr<Card> card = currentPlayer->opCard(dummyCard, cnt);
                // }


                // 낼수있는 카드가 있으면
                if (card && currentPlayer->canPlayCard(card,dummyCard,cnt))
                {
                    if (card->getAttackPower()>0)
                    {
                        attackStack += card->getAttackPower();
                    }
                    std::cout << currentPlayer->getName() << " is " << card->getFullInfo() << "를 냈습니다." << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    cnt++;
                    dummyCard = card;
                    //// 7 카드 사람이 직접 모양 바꾸기
                    //if ( card->getValue() == "7") currentPlayer->showHand(); //바꾸기전 편의성을 위해서
                    // if ( card->change7(dummyCard))
                    // {
                    //     change7cnt++;// 바꿨으면 count 후 턴 종료
                    // }
                    //// currentPlayer에 따라서 다른 알고리즘의 함수로 7 카드 shape변경 해도됨
                    played = true;
                    currentPlayer->playCard(card,deck);
                    currentPlayer->showHand();
                    // JQK 기능 처리
                    if (card->useJQK())
                    {
                        if (card->getValue()=="K")
                        {
                            std::cout << currentPlayer->getName() << " is " << card->getValue() << "를 냈으므로 한턴 더 플레이 합니다." << std::endl;
                            std::this_thread::sleep_for(std::chrono::seconds(1));
                            Kcnt++;
                        }
                        else if (card->getValue()=="J")
                        {
                            std::cout << currentPlayer->getName() << " is " << card->getValue() << "를 냈으므로 다음 플레이어를 건너뜁니다." << std::endl;
                            std::this_thread::sleep_for(std::chrono::seconds(1));
                            Jcnt++;
                        }
                        else if (card->getValue()=="Q")
                        {
                            std::cout << currentPlayer->getName() << " is " << card->getValue() << "를 냈으므로 플레이 방향이 반대로됩니다." << std::endl;
                            std::this_thread::sleep_for(std::chrono::seconds(1));
                            Qcnt++;
                        }
                    }
                }
                if (Kcnt > 0 || change7cnt > 0) break;
            } while (played);
            if (Kcnt > 0) continue; // one more turn
            // 카드가 없으면 한 장 뽑기
            if (cnt == 0)
            {
                auto drawnCard = deck.draw();
                eraseUsedAttackCards();
                currentPlayer->drawCard(drawnCard);
                std::cout << currentPlayer->getName() << " is 낼 카드가 없어 한 장을 뽑습니다." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));
                std::cout << currentPlayer->getName() << " is 덱에서 " << drawnCard->getFullInfo() << "를 뽑았습니다." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));
                currentPlayer->showHand();
            }
        }
        //승리/탈락 조건 확인
        if (currentPlayer->checkGameOver(currentPlayer, players, currentPlayerIndex, deck))
        {
            pushRanking(currentPlayer);
            players.erase(players.begin() + currentPlayerIndex); // 플레이어 제거

            //erase되는 동시에 다음 플레이어가 현재 플레이어 index번호에 들어오기때문에
            //다음턴이 제대로 넘어가기 위해서 -1 해줘함.
            currentPlayerIndex = currentPlayerIndex - 1 + players.size(); // 음수 방지
            //마지막 1명만 남으면 winner에 push_back, loser는 아니니까
            if (players.size()==1)
            {
                ranking.push_back(players[0]);
                pushRanking(players[0]);
                players.erase(players.begin()); // 플레이어 제거
                std::cout << "=================================" << std::endl;
                std::cout << "마지막 1명만 남았으므로 게임을 종료합니다." << std::endl;
                std::cout << "=================================" << std::endl;
                printRanking();
                break;
            }
        }

        //진태,찬우,가성,지희,민성
        //턴 넘기기(J,Q case 포함)
        bool isReversed = false;
        if ( Jcnt > 0 ) currentPlayerIndex = (currentPlayerIndex + Jcnt * 2 ) % players.size();
        else if ( Qcnt > 0 ) //진태,찬우,가성,지희,민성
        {
            // Qcnt가 홀수일 때만 reverse 수행
            if (Qcnt % 2 == 1)
            {
                std::reverse(players.begin(), players.end());
                std::cout << "\n플레이 순서:\n";
                for (size_t i=0; i<players.size() ;i++)
                {
                    std::cout << players[i]->getName() << "->";
                }
                std::cout << "\b\b" <<std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }

            // 순서 반전 이후의 index를 찾아서 다음 플레이어를 지정
            auto idx = std::find(players.begin(), players.end(), currentPlayer);
            int newIndex = std::distance(players.begin(), idx);
            //std::cout << currentPlayer->getName() <<"의 현재 index : " <<newIndex << std::endl;
            currentPlayerIndex = (newIndex + 1) % players.size();

        }
        else currentPlayerIndex = (currentPlayerIndex + 1 ) % players.size();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

}
