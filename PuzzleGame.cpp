#include <bangtal.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#pragma comment (lib, "Bangtal.lib")
using namespace std;

namespace easy {
    SceneID scene1;
    ObjectID startButton;
    ObjectID piece[9];
    ObjectID menu2, start;
    TimerID timer1;

    int arr[3][3];
    // currentX[i] : 현재 i번쨰 조각의 X좌표
    int currentX[9], currentY[9];
    int dx[4] = { 1, 0, -1, 0 }, dy[4] = { 0, 1, 0, -1 };

    // 이미지 파일의 위치
    const char* imgFileLocate[9] = { "images\\1\\1-1.png", "images\\1\\1-2.png", "images\\1\\1-3.png", "images\\1\\1-4.png", "images\\1\\1-5.png", "images\\1\\1-6.png",
            "images\\1\\1-7.png", "images\\1\\1-8.png" , "images\\1\\1-9.png" };

    // hideX,Y,Num.  hide된 piece의 X,Y좌표와 몇번째?
    int hX = 0, hY = 0, hNum = 0;

    // 맞는 조각
    bool correct[9];

    // playing : 게임진행중, finish : 게임끝
    bool playing = false;
    bool finish = false;


    // 0 ~ n-1 중 random한 숫자 생성
    int myRand(int n) {
        if (n == 0) {
            return 0;
        }
        int k = rand() % n;
        if (n > 0) {
            return k;
        }
        return 0;

    }

    // num 번째 조각에 배열에서의 원래 위치의 X좌표
    int arrayX(int num) {
        return num % 3;
    }
    // num 번째 조각에 배열에서의 원래 위치의 X좌표
    int arrayY(int num) {
        return (int)(num / 3);
    }
    // 배열에서 x,y 좌표는 원래 number번째 조각.
    int number(int x, int y) {
        return (3 * y + x);
    }
    // 반댓방향 번호
    int oppositeDirection(int d) {
        return ((d + 2) % 3);
    }

    int coolX(int x, int y) {
        return 398 + 200 * x;
    }
    int coolY(int x, int y) {
        return 461 - 200 * y;
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

    // 마우스 입력을 받았을때 쓰일 함수.
    // 클릭된 object가 몇번째 piece인지 return
    int findNum(ObjectID object) {
        for (int i = 0; i < 9; i++) {
            if (piece[i] == object) {
                return i;
            }
        }
        return -1; // piece가 아닌 object가 들어오면
    }

    bool checkIn(int x, int y, int direction) {
        int tx = x + dx[direction], ty = y + dy[direction];
        return (tx > -1 && tx < 3 && ty > -1 && ty < 3);
    }

    void initializeArr(bool initObject) {
        int num = 0;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                arr[j][i] = num;
                if (initObject) {
                    locateObject(piece[num], scene1, coolX(num), coolY(num));
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
        if (arr[x][y] == num) {
            correct[num] = true;
        }
        else {
            correct[num] = false;
        }
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


    void shuffle() {

        hNum = myRand(16);    //여기!!!!!!!!!!!!!!!!!!!!!!!!
        hX = arrayX(hNum);
        hY = arrayY(hNum);
        hideObject(piece[hNum]);


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
                int sNum = arr[sx][sy];

                int tx = hX, ty = hY;

                hX = sx, hY = sy;
                currentX[hNum] = sx, currentY[hNum] = sy;
                arr[sx][sy] = hNum;

                currentX[sNum] = tx, currentY[sNum] = ty;
                arr[tx][ty] = sNum;

                locateObject(piece[hNum], scene1, coolX(sx, 0), coolY(0, sy));
                locateObject(piece[sNum], scene1, coolX(tx, 0), coolY(0, ty));

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

        else if (object == menu2) {
            startGame(scene1);
        }
        else if (playing) {
            // num : 클릭한 조각 번호
            int num = findNum(object);

            if (num >= 0) {
                int cx = currentX[num], cy = currentY[num];
                int direction = checkHPieceDirection(cx, cy);

                if (direction > -1) {

                    int tx = hX, ty = hY;

                    //hPiece 옮기기
                    hX = cx, hY = cy;
                    currentX[hNum] = cx, currentY[hNum] = cy;
                    arr[cx][cy] = hNum;

                    //클릭한 piece 옮기기
                    currentX[num] = tx, currentY[num] = ty;
                    arr[tx][ty] = num;

                    //이미지 갱신
                    locateObject(piece[hNum], scene1, coolX(cx, 0), coolY(0, cy));
                    locateObject(piece[num], scene1, coolX(tx, 0), coolY(0, ty));

                    updateCorrect(cx, cy);
                    updateCorrect(tx, ty);

                    finish = checkFin();
                    if (finish) {
                        playing = false;
                        setObjectImage(startButton, "Images\\2\\restart.png");
                        showObject(startButton);
                        showObject(piece[hNum]);
                        stopTimer(timer1);
                        float time = 3600 - getTimer(timer1);
                        setTimer(timer1, time);
                        showTimer(timer1);

                    }
                }
            }

        }
    }
}

SceneID scene1;
ObjectID startButton;
ObjectID piece[16];
ObjectID menu1,menu2, start;
TimerID timer1;


int arr[4][4];

int currentX[16], currentY[16];
int dx[4] = { 1, 0, -1, 0 }, dy[4] = { 0, 1, 0, -1 };

const char* imgFileLocate[16] = { "images\\2\\in1.jpg", "images\\2\\in2.jpg", "images\\2\\in3.jpg", "images\\2\\in4.jpg", "images\\2\\in5.jpg", "images\\2\\in6.jpg",
        "images\\2\\in7.jpg", "images\\2\\in8.jpg" , "images\\2\\in9.jpg" , "images\\2\\in10.jpg" , "images\\2\\in11.jpg" , "images\\2\\in12.jpg",
        "images\\2\\in13.jpg" , "images\\2\\in14.jpg" , "images\\2\\in15.jpg", "images\\2\\in16.jpg" };


int hX = 0, hY = 0, hNum = 0;


bool correct[16];

bool playing = false;
bool finish = false;


int myRand(int n) {
    int k = rand() % n;
    if (n > 0) {
        return k;
    }
    return 0;

}


int arrayX(int num) {
    return num % 4;
}

int arrayY(int num) {
    return (int)(num / 4);
}

int number(int x, int y) {
    return (4 * y + x);
}

int oppositeDirection(int d) {
    return ((d + 2) % 4);
}


int coolX(int x, int y) {
    return 340 + 150 * x;
}
int coolY(int x, int y) {
    return 510 - 150 * y;
}

int coolX(int num) {
    return coolX(arrayX(num), 0);
}

int coolY(int num) {
    return coolY(0, arrayY(num));
}

int findNum(ObjectID object) {
    for (int i = 0; i < 16; i++) {
        if (piece[i] == object) {
            return i;
        }
    }
    return -1; // piece가 아닌 object가 들어오면 -1.
}


bool checkIn(int x, int y, int direction) {
    int tx = x + dx[direction], ty = y + dy[direction];
    return (tx > -1 && tx < 4 && ty > -1 && ty < 4);
}

void initializeArr(bool initObject) {
    int num = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            arr[j][i] = num;
            if (initObject) {
                locateObject(piece[num], scene1, coolX(num), coolY(num));
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
    if (arr[x][y] == num) {
        correct[num] = true;
    }
    else {
        correct[num] = false;
    }
}


int checkHPieceDirection(int x, int y) {
    int tx = 0, ty = 0;
    for (int i = 0; i < 4; i++) {
        tx = x + dx[i];
        ty = y + dy[i];
        if (tx == hX && ty == hY) {
            return i; 
        }
    }
    return -1; 
}


void shuffle() {

    hNum = myRand(16);
    hX = arrayX(hNum);
    hY = arrayY(hNum);
    hideObject(piece[hNum]);



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
            int sNum = arr[sx][sy];

            int tx = hX, ty = hY;

            hX = sx, hY = sy;
            currentX[hNum] = sx, currentY[hNum] = sy;
            arr[sx][sy] = hNum;

            currentX[sNum] = tx, currentY[sNum] = ty;
            arr[tx][ty] = sNum;

            locateObject(piece[hNum], scene1, coolX(sx, 0), coolY(0, sy));
            locateObject(piece[sNum], scene1, coolX(tx, 0), coolY(0, ty));

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
        startGame(scene1);
    }

    else if (object == menu2) {
        startGame(easy::scene1);
    }

    else if (easy::playing) {

        int num = easy::findNum(object);
        
        if (num >= 0) {
            int cx = easy::currentX[num], cy = easy::currentY[num];
            int direction = easy::checkHPieceDirection(cx, cy);

            if (direction > -1) {

                int tx = easy::hX, ty = easy::hY;

 
                easy::hX = cx, easy::hY = cy;
                easy::currentX[easy::hNum] = cx, easy::currentY[easy::hNum] = cy;
                easy::arr[cx][cy] = easy::hNum;

                easy::currentX[num] = tx, easy::currentY[num] = ty;
                easy::arr[tx][ty] = num;

                locateObject(easy::piece[easy::hNum], easy::scene1, easy::coolX(cx, 0), easy::coolY(0, cy));
                locateObject(easy::piece[num], easy::scene1, easy::coolX(tx, 0), easy::coolY(0, ty));

                easy::updateCorrect(cx, cy);
                easy::updateCorrect(tx, ty);

                easy::finish = easy::checkFin();
                if (easy::finish) {
                    easy::playing = false;
                    setObjectImage(easy::startButton, "Images\\2\\restart.png");
                    showObject(easy::startButton);
                    showObject(easy::piece[easy::hNum]);
                    stopTimer(easy::timer1);
                    float time = 3600 - getTimer(easy::timer1);
                    setTimer(easy::timer1, time);
                    showTimer(easy::timer1);

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
                arr[cx][cy] = hNum;

                currentX[num] = tx, currentY[num] = ty;
                arr[tx][ty] = num;

                //이미지 갱신
                locateObject(piece[hNum], scene1, coolX(cx, 0), coolY(0, cy));
                locateObject(piece[num], scene1, coolX(tx, 0), coolY(0, ty));

                updateCorrect(cx, cy);
                updateCorrect(tx, ty);

                finish = checkFin();
                if (finish) {
                    playing = false;
                    setObjectImage(startButton, "Images\\2\\restart.png");
                    showObject(startButton);
                    showObject(piece[hNum]);
                    stopTimer(timer1);
                    float time = 3600 - getTimer(timer1);
                    setTimer(timer1, time);
                    showTimer(timer1);

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

    scene1 = createScene("퍼즐", "Images\\2\\배경.jpg");
    easy::scene1 = createScene("퍼즐", "Images\\1\\배경1.png");
    start = createScene("퍼즐", "Images\\바탕.png");
    menu1 = createObject("Images\\메뉴1.png");
    locateObject(menu1, start, 800, 300);
    showObject(menu1);

    menu2 = createObject("Images\\메뉴2.png");
    locateObject(menu2, start, 200, 300);
    showObject(menu2);

    // 조각 생성
    for (int i = 0; i < 16; i++) {
        piece[i] = createObject(imgFileLocate[i]);
        locateObject(piece[i], scene1, coolX(i), coolY(i));
        showObject(piece[i]);
    }

    startButton = createObject("Images\\2\\start.png");
    locateObject(startButton, scene1, 590, 80);
    showObject(startButton);

    initializeArr(false);
    initializeCurrentXY();
    initializeCorrect();

    timer1 = createTimer(3600.0f);

    for (int j = 0; j < 9; j++) {
        easy::piece[j] = createObject(easy::imgFileLocate[j]);
        locateObject(easy::piece[j], easy::scene1, easy::coolX(j), easy::coolY(j));
        showObject(easy::piece[j]);
    }

    easy::startButton = createObject("Images\\2\\start.png");
    locateObject(easy::startButton, easy::scene1, 590, 80);
    showObject(easy::startButton);

    easy::initializeArr(false);
    easy::initializeCurrentXY();
    easy::initializeCorrect();

    easy::timer1 = createTimer(3600.0f);


    startGame(start);

}