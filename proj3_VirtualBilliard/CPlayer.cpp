#include "CPlayer.h"
#include <d3d9.h>

//만약 hit by override 을 고치면 공하고 player하고 고칠수있을것같아
// 리팩토링하는 준혁아 재범아 힘내 ^^ 
//★★★★★자료형 리팩토링 * char* 을 스트링으로 바꾸어줘 ^^ 안녕 !! ★★★★★★★
//파일 입출력으로 또한 그 모지 모지 당구 같은경우 내가 알기로는 자기 몇타? 몇점 친다하면
//그점수서 시작하는걸로알고 목표점수 (즉 쳐야할 점수에서 + ) 
// goal 점수를 만드는것도괜찮을듯
//어는 목표점수 이상넘어가면 결정되는거니까 
//목표 점수 
//즉 decide win 하는 함수 ? 
//decide win..? decideWinne 

CPlayer::CPlayer() {

	myBall = "NULL";
	myScore = 0;
	goalScore = 0;
	hitRed1 = false;
	hitRed2 = false;
	hitball = false;
	//bool hitWall; //쓰리쿠션 구현 ?
	myTurn = true;
	
}

CPlayer::CPlayer(std::string name) {
	myBall = name;
	//this->myBall = name;
	myScore = 0;
	goalScore = 100;
	hitRed1 = false;
	hitRed2 = false;
	hitball = false; //상대공 쳤는지 
					 //bool hitWall; //쓰리쿠션 구현 ?
	myTurn = true;
}

CPlayer::CPlayer(std::string name, int goalScore) {
	myBall = name;
	//this->myBall = name;
	myScore = 0;
	goalScore = goalScore;
	hitRed1 = false;
	hitRed2 = false;
	hitball = false; //상대공 쳤는지 
	//bool hitWall; //쓰리쿠션 구현 ?
	myTurn = true;
	}

CPlayer::~CPlayer() {}


void CPlayer::setHitBall(std::string ballname) {
	if (ballname.compare("Red1"))
		this->hitRed1 = true;
	else if (ballname.compare("Red2"))
		this->hitRed2 = true;
	/*else if (this -> ballname.compare(ballname))
		return;
	*/
	else this->hitball = true;
}

void CPlayer::setHitBall(std::string ball1, std::string ball2) {
	this->setHitBall(ball1);
	this->setHitBall(ball2);

}
void CPlayer::decideScore() {
	myTurn = false;
	if (hitball || (!(hitRed1) && !(hitRed2)) ) //공타 
		myScore -= 10; //우선 10점 까기는 함 
	else if (hitRed1 && hitRed2) {
		myScore += 10;
		myTurn = true;
	}

	resetHit();
}


void CPlayer::create(std::string name) {
	this->myBall = name;
}