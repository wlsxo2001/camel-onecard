//
// Created by jintae on 25. 2. 12.
//
#include "../rule/include/Game.hpp"
// 진태,찬우,가성,지희,민성,태건,혜연
int main()
{
    Game game;
    std::string winner;
    int win_count[7] = {0};
    int count = 0;
    for (int i=0; i<1; i++)
    {
        winner = game.start();
        count++;
        if (winner == "가성") {win_count[0]++;}
        if (winner == "진태") {win_count[1]++;}
        if (winner == "찬우") {win_count[2]++;}
        if (winner == "태건") {win_count[3]++;}
        if (winner == "헤연") {win_count[4]++;}
        if (winner == "지희") {win_count[5]++;}
        if (winner == "민성") {win_count[6]++;}
    }
    std::cout << count << std::endl;
    std::cout << "가성" << " " << "진태" << " " << "찬우" << " " << "태건" << " " << "헤연" << " " << "지희" << " " << "민성" << " ";
    std::cout << std::endl;

    for (int i=0; i<7; i++)
    {
        std::cout << win_count[i] << " ";
    }

    return 0;
}
//추가할것들
//deck 에서 모든 카드를 쓴 경우 처리
//joker 카드 canPlay 조건 처리 (완)
//J,Q,K 기능 추가 (완)
//첫 카드가 공격카드인 경우 공격을 받아버림. (첫 카드가 공격카드이면 공격카드가 아닐때까지 다시 뽑게함.) (완)
//공격카드를 2개 이상 냈을 때 중첩X. (룰에 따라서 다르게 적용되므로 아직은 그대로 놔둠./연속 공격이 가능할때만)
//필요한 player기능들
//마지막 1장 남았을때 원카드 외치기 (굳이 할 필요 없어보임)
//최적의 카드 제출하기(지금은 그냥 반복문으로 읽는 순서대로 체크하고 있음)
//7 나왔을 때 문양 바꾸기
