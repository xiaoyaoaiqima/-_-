#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include<string>
#include <windows.h>  //这个头文件必须添加
#include <iomanip>
#include <fstream>
#include <cmath>
#include "Chess.h"
#include "ChessBoard.h"
#include "ChessLogic.h"

using namespace std;

sf::Texture chess,board;
sf::Font font;
sf::Font font_chinese;
//双击等待时间
sf::Clock mouseClickTimer;
//步进 描述
sf::RectangleShape rect;
Chess *arr[CHESS_NUMBER];
sf::SoundBuffer sbmove, sbeat;
sf::Sound smove, seat;
sf::Music sbgm;
int soundVolume = 30;
void init();
void recover();
int main(){

    wcin.imbue(std::locale("chs"));//载入中文字符输入方式
    wcout.imbue(std::locale("chs"));//载入中文字符输入方式
    //循环播放
   // sbgm.setVolume(soundVolume);
    init();
    sbgm.play();
    sbgm.setLoop(true);
    std::ios::sync_with_stdio(false);
    sf::RenderWindow window(sf::VideoMode(1200,900),"Chess!",sf::Style::Close | sf::Style::Titlebar);
    //rect不更新

    sf::Event event;
    sf::Sprite background(board);
    ChessBoard chessboard;
    MoveAction moveaction;
    StopWatch stopwatch(font);
    //宋体
    moveaction.stepDescribe(font_chinese);
    bool win = false;
    while(window.isOpen()){
        sf::Vector2i pos = sf::Mouse::getPosition(window);
        window.setSize(sf::Vector2u(1200, 900));
        if(!win && stopwatch.getElapsedTime() > TIME_LIMIT_PER_MOVE){
            win = true;
            if(moveaction.getTurn() == -1) std::cout << "Black Win!" << endl;
            else std::cout << "Red Win!" << endl;
        }
        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed){
                window.close();
            }
            // 双击 可以多走一步
            if (!win && event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left)
            {
                    if (mouseClickTimer.getElapsedTime().asMilliseconds() < 300)
                    {
                        std::cout << "双击" << endl;
                        stopwatch.reStart();
                        stopwatch.reverseColor();
                        moveaction.changeTurn();
   
                    }
            }

            if(!win && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left){
                //按下左键开始计时 mouseClickTimer
                mouseClickTimer.restart();
                MoveAble moveable = moveaction.canMove(arr,chessboard,pos);
                 std::cout << "selected = " << moveaction.getSelected() << endl;
                 std::cout << "canMove = " << moveable.getCanMove() << endl;
                if(!moveable.getCanMove()) continue;
                if(moveaction.move(arr,chessboard,moveable)){
                    stopwatch.reverseColor();
                    stopwatch.reStart();
                    moveaction.reverseColor();
                    seat.play();

                }
                if(moveaction.win(arr,chessboard)){
                    if(moveaction.getTurn() == -1) std::cout << "Black Win!" << endl;
                    else std::cout << "Red Win!" << endl;
                    win = true;
                }
                // std::cout << win << endl;
            }
            //右击 
            if(event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Right){
                win = false;
                if(!moveaction.repentance(chessboard)){
                    std::cout << "Can't repent at first" << endl;
                }else{
                    stopwatch.reverseColor();
                    stopwatch.reStart();
                }
            }
        }
        // std::cout << pos.x << " " << pos.y << endl;
        //window.setSize(sf::Vector2u(890, 900));
        window.clear();
        window.draw(rect);
        window.draw(background);
        window.draw(moveaction.getText());
        for(int i = 0 ; i < CHESS_NUMBER ; i++ ){
            window.draw(arr[i]->getSprite());
        }
        if(!win){
            stopwatch.setText();
            window.draw(stopwatch.getText());
        }
        window.display();
    }
    recover();
    std::system("pause");
    return 0;
}

void init(){
    //绘制 描述栏
    rect.setSize(sf::Vector2f(150, 900));//设置大小
    rect.setPosition(sf::Vector2f(890, 0));//设置位置
    rect.setFillColor(sf::Color::White);//设置颜色
    //
    chess.loadFromFile("./res/chess.png");
    board.loadFromFile("./res/background.png");
    font.loadFromFile("./res/arial.ttf");
    font_chinese.loadFromFile("./res/simsun.ttf");
    sbgm.openFromFile("./res/Audios/BGM.wav");
    sbmove.loadFromFile("./res/Audios/move.wav");
    sbeat.loadFromFile("./res/Audios/eat.wav");
    smove.setBuffer(sbmove);
    seat.setBuffer(sbeat);
    int cnt = 0;
    //黑
    arr[cnt++] = new King(4, 0, 1, chess, sf::IntRect(62, 0, 62, 62),-4);
    arr[cnt++] = new Queen(3, 0, 1, chess, sf::IntRect(62, 62, 62, 62),-3);
    arr[cnt++] = new Queen(5, 0, 1, chess, sf::IntRect(62, 62, 62, 62),-3);
    arr[cnt++] = new Bishop(2, 0, 1, chess, sf::IntRect(62, 2 * 62, 62, 62),-2);
    arr[cnt++] = new Bishop(6, 0, 1, chess, sf::IntRect(62, 2 * 62, 62, 62),-2);
    arr[cnt++] = new Knight(1, 0, 1, chess, sf::IntRect(62, 62 * 4, 62, 62),-1);
    arr[cnt++] = new Knight(7, 0, 1, chess, sf::IntRect(62, 62 * 4, 62, 62),-1);
    arr[cnt++] = new Root(0, 0, 1, chess, sf::IntRect(62, 3 * 62, 62, 62),-8);
    arr[cnt++] = new Root(8, 0, 1, chess, sf::IntRect(62, 3 * 62, 62, 62),-8);
    arr[cnt++] = new Cannon(1, 2, 1, chess, sf::IntRect(62, 5 * 62, 62, 62),-7);
    arr[cnt++] = new Cannon(7, 2, 1, chess, sf::IntRect(62, 5 * 62, 62, 62),-7);
    for(int i = 0 ; i <= 8 ; i += 2){
        arr[cnt++] = new Pawn(i, 3, 1, chess, sf::IntRect(62, 6 * 62, 62, 62),-6);
    }
    //红
    arr[cnt++] = new King(4, 9, -1, chess, sf::IntRect(0, 0, 62, 62), 4);
    arr[cnt++] = new Queen(3, 9, -1, chess, sf::IntRect(0, 62, 62, 62), 3);
    arr[cnt++] = new Queen(5, 9, -1, chess, sf::IntRect(0, 62, 62, 62), 3);
    arr[cnt++] = new Bishop(2, 9, -1, chess, sf::IntRect(0, 2 * 62, 62, 62), 2);
    arr[cnt++] = new Bishop(6, 9, -1, chess, sf::IntRect(0, 2 * 62, 62, 62), 2);
    arr[cnt++] = new Knight(1, 9, -1, chess, sf::IntRect(0, 62 * 4, 62, 62), 1);
    arr[cnt++] = new Knight(7, 9, -1, chess, sf::IntRect(0, 62 * 4, 62, 62), 1);
    arr[cnt++] = new Root(0, 9, -1, chess, sf::IntRect(0, 3 * 62, 62, 62), 8);
    arr[cnt++] = new Root(8, 9, -1, chess, sf::IntRect(0, 3 * 62, 62, 62), 8);
    arr[cnt++] = new Cannon(1, 7, -1, chess, sf::IntRect(0, 5 * 62, 62, 62), 7);
    arr[cnt++] = new Cannon(7, 7, -1, chess, sf::IntRect(0, 5 * 62, 62, 62), 7);
    for (int i = 0; i <= 8; i += 2){
        arr[cnt++] = new Pawn(i, 6, -1, chess, sf::IntRect(0, 6 * 62, 62, 62), 6);
    }
}

void recover(){
    for(int i = 0 ; i < 32 ; i++ ){
        delete arr[i];
    }
}