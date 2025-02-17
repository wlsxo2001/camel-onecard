//
// Created by jintae on 25. 2. 12.
//
#include "../rule/include/Game.hpp"

int main()
{
    Game game;
    game.start();
    return 0;
}
//추가할것들
//deck 에서 모든 카드를 쓴 경우 처리 (완)
//joker 카드 canPlay 조건 처리 (완)
//J,Q,K 기능 추가 (완)
//아래는 필요한 player기능들
//마지막 1장 남았을때 원카드 외치기
//최적의 카드 제출하기(지금은 그냥 반복문으로 읽는 순서대로 체크하고 있음)
//7 나왔을 때 문양 바꾸기