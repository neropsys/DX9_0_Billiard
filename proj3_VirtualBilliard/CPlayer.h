#pragma once
#include "CObject.h"
#include "d3dUtility.h"

/*
player 클래스를 만든 이유는
이 것을 안만드면 공에서 점수를 판단함
점수판단은 플레이어가 하는거지 공이하는게 아니다
또한 여기서 문제가 발생한게 virtual lego에서 공에 색깔을 입힐떄 red red yellow whit로 하는데 거기서 문제가 생긴 부분잇을수도 잇음
red1 red 이런식으로 입혓으니 혹시 놓친게 있으면 확인 바람 
즉 간단히 정리하면
1.점수얻는 상황을 쉽게파난하기위하여
2.유저의 공은 수구 위에서 red1 red 떄문에 유저의 공을 수ㄱ로 판단하기 위하여 
3. 공하고 플레이너느 다른다
 4. 플레이어에게 공의속성을 부여하게 함 
*/
class CPlayer  {
public:
	CPlayer();

	//CPlayer(char* name);																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																													
	CPlayer(std::string name);
	CPlayer(std::string name, int goalScore);

//	CPlayer(char* name,int goalScore);
	~CPlayer();

	void setHitBall(std::string ballname);
	void setHitBall(std::string ball1 ,std::string ball2); 

	void decideScore(); //decide Score + 0 - ? 
	void create(std::string name);
	inline bool changeTurn() {		return !(myTurn);	};
	inline int getScore() {		return myScore; 	}; //
	inline void setMyball(std::string color) {		this->myBall = color;	}
	inline bool getMyTurn() {		return this->myTurn;	}
	inline void resetHit() {
		hitRed1 = false;
		hitRed2 = false;
		hitball = false;

	}
private:

	//char* myBall;

	std::string myBall;

	int myScore;
	int goalScore; //목표점수  즉 goal Score 변경을 하면 안됨 한번 setting되면

	bool hitRed1;
	bool hitRed2;
	bool hitball;
	//bool hitWall; //쓰리쿠션 구현 ?
	bool myTurn;

};
/*

hitBy()

//검사
if ((this->name == 'y' && ball.name == 'w') || (this->name == 'y' && ball.name == 'w')) this->failFlag = true; // 흰공과 노란공, 노란공과 흰공이 부딪혔을 경우 벌점
else if (this->name == 'y' || this->name == 'w'){ // 아닌 경우 내 공과 빨간 공이 부딪혔을 때
if (ball.name = 'r') // 첫번째 빨간공(r)일때
switch (ball.redAttack){
case 0: ball.redAttack = 1;
break;
case 2: ball.redAttack = 3;
ball.sucFlag = true;
break;
}
else if (ball.name = 's') // 두번째 빨간공(s)일때
switch (ball.redAttack){
case 0: ball.redAttack = 2;
break;
case 1: ball.redAttack = 3;
ball.sucFlag = true;
break;
}

}

*virtualLego.cpp

//점수확인하는 함수 추가

void scoring(CSphere ball){

if (ball.name == 'w')
{ // 흰공의 턴이면
if (!(ball.failFlag && ball.sucFlag)); // 성공도 벌점도 아니면 무시
else if (ball.failFlag) ball.score += 10;// 실패일경우 무조건 벌점
else ball.score -= 10; // 성공일경우 승점

ball.initializeScore(); //공이 모두 멈추고 판정이 끝난 상태이므로 해당 점수 상태(공을 친 상태)를 초기화

}
else
{ // 노란공의 턴이면
if (!(ball.failFlag && ball.sucFlag)); // 성공도 벌점도 아니면 무시
else if (ball.failFlag) ball.score += 10; // 실패일경우 무조건 벌점
else ball.score -= 10;// 성공일경우 승점

ball.initializeScore();//공이 모두 멈추고 판정이 끝난 상태이므로 해당 점수 상태(공을 친 상태)를 초기화
}

*/