#include <bangtal.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

int width, Px, Py, ver = 1;

ObjectID* pieces;

float easyhighscore = 3600;
float hardhighscore = 3600;

SceneID easyscene;
SceneID hardscene;
// 이미지 파일의 위치
const char* easyimgFileLocate[9] = { "images\\1\\1-1.png", "images\\1\\1-2.png", "images\\1\\1-3.png", "images\\1\\1-4.png", "images\\1\\1-5.png", "images\\1\\1-6.png",
        "images\\1\\1-7.png", "images\\1\\1-8.png" , "images\\1\\1-9.png" };

const char* hardimgFileLocate[16] = { "images\\2\\in1.jpg", "images\\2\\in2.jpg", "images\\2\\in3.jpg", "images\\2\\in4.jpg", "images\\2\\in5.jpg", "images\\2\\in6.jpg",
        "images\\2\\in7.jpg", "images\\2\\in8.jpg" , "images\\2\\in9.jpg" , "images\\2\\in10.jpg" , "images\\2\\in11.jpg" , "images\\2\\in12.jpg",
        "images\\2\\in13.jpg" , "images\\2\\in14.jpg" , "images\\2\\in15.jpg", "images\\2\\in16.jpg" };


ObjectID startButton;
ObjectID restartButton;
ObjectID menu1, menu2, start;
TimerID timer1;

int hardArr[4][4];
int easyArr[3][3];

int currentX[16], currentY[16];
int dx[4] = { 1, 0, -1, 0 }, dy[4] = { 0, 1, 0, -1 };
int hX = 0, hY = 0, hNum = 0;

bool correct[16];

bool playing = false;
bool finish = false;


int arrayX(int num) {
    return num % ver;
}
// num 번째 조각에 배열에서의 원래 위치의 X좌표
int arrayY(int num) {
    return (int)(num / ver);
}

int coolX(int x, int y) {
    return Px + width * x;
}
int coolY(int x, int y) {
    return Py - width * y;
}

int number(int x, int y) {
    return (ver * y + x);
}

// num 번쨰 조각의 원래 위치의 이미지상 X좌표
int coolX(int num) {
    return coolX(arrayX(num), 0);
}
// num 번쨰 조각의 원래 위치의 이미지상 Y좌표.
// 주의! 이미지 좌표는 자체정의한 배열의 Y좌표 방향과 반대.
int coolY(int num) {
    return coolY(0, arrayY(num));
}

int myRand(int n) {

    if (n > 0) {
        int k = rand() % n;
        return k;
    }
    return 0;

}

bool checkIn(int x, int y, int direction) {
    int tx = x + dx[direction], ty = y + dy[direction];
    return (tx > -1 && tx < ver&& ty > -1 && ty < ver);
}

// hPiece와 붙어있는지 확인하고, 근처에 있으면 방향 번호 리턴
int checkHPieceDirection(int x, int y) {
    int tx = 0, ty = 0;
    for (int i = 0; i < 4; i++) {
        tx = x + dx[i];
        ty = y + dy[i];
        if (tx == hX && ty == hY) {
            return i;  // hidePiece가 근처에 있으면 방향 번호 리턴
        }
    }
    return -1;
}

int oppositeDirection(int d) {
    return ((d + 2) % ver);
}

int findNum(ObjectID object) {
    for (int i = 0; i < ver * ver; i++) {
        if (pieces[i] == object) {
            return i;
        }
    }
    return -1; // piece가 아닌 object가 들어오면
}

namespace easy {
    ObjectID startButton;
    ObjectID start;
    TimerID timer1;

    // currentX[i] : 현재 i번쨰 조각의 X좌표
    int currentX[9], currentY[9];

    // 맞는 조각
    bool correct[9];

    // playing : 게임진행중, finish : 게임끝
    bool playing = false;
    bool finish = false;

    void initializeArr(bool initObject) {
        int num = 0;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                easyArr[j][i] = num;
                if (initObject) {
                    locateObject(pieces[num], easyscene, coolX(num), coolY(num));
                }
                num++;
            }
        }
    }
    // currentX,Y 초기화
    void initializeCurrentXY() {
        for (int i = 0; i < 9; i++) {
            currentX[i] = arrayX(i);
            currentY[i] = arrayY(i);
        }
    }
    // correct 초기화
    void initializeCorrect() {
        for (int i = 0; i < 9; i++) {
            correct[i] = true;
        }
    }
    bool checkFin() {
        for (int i = 0; i < 9; i++) {
            if (!correct[i]) {
                return false;
            }
        }
        return true;
    }

    void updateCorrect(int x, int y) {
        int num = number(x, y);
        if (easyArr[x][y] == num) {
            correct[num] = true;
        }
        else {
            correct[num] = false;
        }
    }

    void shuffle() {

        hNum = myRand(9);    //여기!!!!!!!!!!!!!!!!!!!!!!!!
        hX = arrayX(hNum);
        hY = arrayY(hNum);
        hideObject(pieces[hNum]);


        // 128번 섞기
        for (int j = 0; j < 128; j++) {

            int directionIn[4] = { 0, 0, 0, 0 };
            int n = 0, r = -1;

            for (int i = 0; i < 4; i++) {
                if (checkIn(hX, hY, i)) {
                    n++;
                    directionIn[n - 1] = i;
                }
            }

            r = myRand(n);

            int dNum = directionIn[r];

            int sx = hX + dx[dNum], sy = hY + dy[dNum];

            if (sx < 3) {
                int sNum = easyArr[sx][sy];

                int tx = hX, ty = hY;

                hX = sx, hY = sy;
                currentX[hNum] = sx, currentY[hNum] = sy;
                easyArr[sx][sy] = hNum;

                currentX[sNum] = tx, currentY[sNum] = ty;
                easyArr[tx][ty] = sNum;

                locateObject(pieces[hNum], easyscene, coolX(sx, 0), coolY(0, sy));
                locateObject(pieces[sNum], easyscene, coolX(tx, 0), coolY(0, ty));

                updateCorrect(sx, sy);
                updateCorrect(tx, ty);
            }

        }
    }


}

void initializeArr(bool initObject) {
    int num = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            hardArr[j][i] = num;
            if (initObject) {
                locateObject(pieces[num], hardscene, coolX(num), coolY(num));
            }
            num++;
        }
    }
}

void initializeCurrentXY() {
    for (int i = 0; i < 16; i++) {
        currentX[i] = arrayX(i);
        currentY[i] = arrayY(i);
    }
}

void initializeCorrect() {
    for (int i = 0; i < 16; i++) {
        correct[i] = true;
    }
}
bool checkFin() {
    for (int i = 0; i < 16; i++) {
        if (!correct[i]) {
            return false;
        }
    }
    return true;
}

void updateCorrect(int x, int y) {
    int num = number(x, y);
    if (hardArr[x][y] == num) {
        correct[num] = true;
    }
    else {
        correct[num] = false;
    }
}

void shuffle() {

    hNum = myRand(16);
    hX = arrayX(hNum);
    hY = arrayY(hNum);
    hideObject(pieces[hNum]);

    for (int j = 0; j < 128; j++) {

        int directionIn[4] = { 0, 0, 0, 0 };
        int n = 0, r = -1;


        for (int i = 0; i < 4; i++) {
            if (checkIn(hX, hY, i)) {
                n++;
                directionIn[n - 1] = i;
            }
        }

        r = myRand(n);

        int dNum = directionIn[r];

        int sx = hX + dx[dNum], sy = hY + dy[dNum];

        if (sx < 4) {
            int sNum = hardArr[sx][sy];

            int tx = hX, ty = hY;

            hX = sx, hY = sy;
            currentX[hNum] = sx, currentY[hNum] = sy;
            hardArr[sx][sy] = hNum;

            currentX[sNum] = tx, currentY[sNum] = ty;
            hardArr[tx][ty] = sNum;

            locateObject(pieces[hNum], hardscene, coolX(sx, 0), coolY(0, sy));
            locateObject(pieces[sNum], hardscene, coolX(tx, 0), coolY(0, ty));

            updateCorrect(sx, sy);
            updateCorrect(tx, ty);
        }

    }
}

void mouseCallback(ObjectID object, int x, int y, MouseAction) {
    if (object == startButton) {
        hideObject(startButton);
        shuffle();
        finish = false;
        playing = true;

        hideTimer();
        setTimer(timer1, 3600.0f);
        startTimer(timer1);

    }

    else if (object == restartButton) {
        hideObject(restartButton);

        hideTimer();

        showObject(menu1);
        showObject(menu2);
        startGame(start);
    }

    else if (object == easy::startButton) {
        hideObject(easy::startButton);
        easy::shuffle();
        easy::finish = false;
        easy::playing = true;

        hideTimer();
        setTimer(easy::timer1, 3600.0f);
        startTimer(easy::timer1);
    }

    else if (object == menu1) {
        width = 150;
        Px = 340;
        Py = 510;
        ver = 4;

        pieces = new ObjectID[ver * ver];

        for (int i = 0; i < 16; i++) {
            pieces[i] = createObject(hardimgFileLocate[i]);
            locateObject(pieces[i], hardscene, coolX(i), coolY(i));
            showObject(pieces[i]);
        }

        startButton = createObject("Images\\2\\start.png");
        locateObject(startButton, hardscene, 590, 80);
        showObject(startButton);

        initializeArr(false);
        initializeCurrentXY();
        initializeCorrect();

        timer1 = createTimer(3600.0f);

        startGame(hardscene);
    }

    else if (object == menu2) {
        width = 200;
        Px = 398;
        Py = 461;
        ver = 3;

        pieces = new ObjectID[ver * ver];

        for (int j = 0; j < 9; j++) {
            pieces[j] = createObject(easyimgFileLocate[j]);
            locateObject(pieces[j], easyscene, coolX(j), coolY(j));
            showObject(pieces[j]);
        }

        easy::startButton = createObject("Images\\2\\start.png");
        locateObject(easy::startButton, easyscene, 590, 80);
        showObject(easy::startButton);

        easy::initializeArr(false);
        easy::initializeCurrentXY();
        easy::initializeCorrect();

        easy::timer1 = createTimer(3600.0f);

        startGame(easyscene);
    }

    else if (easy::playing) {

        int num = findNum(object);

        if (num >= 0) {
            int cx = easy::currentX[num], cy = easy::currentY[num];
            int direction = checkHPieceDirection(cx, cy);

            if (direction > -1) {

                int tx = hX, ty = hY;


                hX = cx, hY = cy;
                easy::currentX[hNum] = cx, easy::currentY[hNum] = cy;
                easyArr[cx][cy] = hNum;

                easy::currentX[num] = tx, easy::currentY[num] = ty;
                easyArr[tx][ty] = num;

                locateObject(pieces[hNum], easyscene, coolX(cx, 0), coolY(0, cy));
                locateObject(pieces[num], easyscene, coolX(tx, 0), coolY(0, ty));

                easy::updateCorrect(cx, cy);
                easy::updateCorrect(tx, ty);

                easy::finish = easy::checkFin();
                if (easy::finish) {
                    easy::playing = false;
                    showObject(easy::startButton);
                    restartButton = createObject("Images\\2\\restart.png");
                    locateObject(restartButton, easyscene, 590, 80);
                    showObject(restartButton);
                    showObject(pieces[hNum]);
                    stopTimer(easy::timer1);
                    float time = 3600 - getTimer(easy::timer1);
                    setTimer(easy::timer1, time);
                    showTimer(easy::timer1);
                    if (easyhighscore > time) {
                        easyhighscore = time;
                        showMessage("최고 기록 갱신!!");
                    }
                    delete[] pieces;
                }
            }
        }

    }

    else if (playing) {

        int num = findNum(object);


        if (num >= 0) {
            int cx = currentX[num], cy = currentY[num];
            int direction = checkHPieceDirection(cx, cy);

            if (direction > -1) {

                int tx = hX, ty = hY;

                hX = cx, hY = cy;
                currentX[hNum] = cx, currentY[hNum] = cy;
                hardArr[cx][cy] = hNum;

                currentX[num] = tx, currentY[num] = ty;
                hardArr[tx][ty] = num;

                //이미지 갱신
                locateObject(pieces[hNum], hardscene, coolX(cx, 0), coolY(0, cy));
                locateObject(pieces[num], hardscene, coolX(tx, 0), coolY(0, ty));

                updateCorrect(cx, cy);
                updateCorrect(tx, ty);

                finish = checkFin();
                if (finish) {
                    playing = false;
                    showObject(startButton);
                    restartButton = createObject("Images\\2\\restart.png");
                    locateObject(restartButton, hardscene, 590, 80);
                    showObject(restartButton);
                    showObject(pieces[hNum]);
                    stopTimer(timer1);
                    float time = 3600 - getTimer(timer1);
                    setTimer(timer1, time);
                    showTimer(timer1);
                    if (hardhighscore > time) {
                        hardhighscore = time;
                        showMessage("최고 기록 갱신!!");
                    }
                    delete[] pieces;
                }
            }
        }

    }
}

int main()
{
    setMouseCallback(mouseCallback);

    setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
    setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);
    setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);

    hardscene = createScene("퍼즐", "Images\\2\\배경.jpg");
    easyscene = createScene("퍼즐", "Images\\1\\배경1.png");
    start = createScene("퍼즐", "Images\\바탕.png");
    menu1 = createObject("Images\\메뉴1.png");
    locateObject(menu1, start, 800, 300);
    showObject(menu1);

    menu2 = createObject("Images\\메뉴2.png");
    locateObject(menu2, start, 200, 300);
    showObject(menu2);

    startGame(start);

}