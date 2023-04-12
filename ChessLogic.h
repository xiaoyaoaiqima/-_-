#ifndef CHESS_LOGIC_H__
#define CHESS_LOGIC_H__

#include "Chess.h"
#include <SFML/Graphics.hpp>
#include "ChessBoard.h"
#include <stack>

// 先定义
class MoveAble;
class OneMove;
class MoveAction;
class StopWatch;

class MoveAble{
private:
    bool canmove;
    Chess *sp;
    int nx,ny;
public:
    MoveAble(bool canmove, Chess *sp = nullptr,int nx = -1, int ny = -1);
    bool getCanMove();
    Chess *getChess();
    int getX();
    int getY();
};

class OneMove{
private:
    Chess *s,*t;
    int sx,sy;
public:
    OneMove(int sx,int sy,Chess *s,Chess *t = nullptr);
    Chess *getS();
    Chess *getT();
    int getSx();
    int getSy();
};

class MoveAction{
private:
    int turn,selected;
    // 悔棋
    std::stack<OneMove> withdrawstk;
    // 步进描述
    sf::Text text;
public:
    std::string str;
    std::wstring wstr;
    MoveAction();
    int getSelected();
    int getTurn();
    //
    MoveAble canMove(Chess **arr, ChessBoard &chessboard,sf::Vector2i &pos);
    bool checkMate(Chess **arr,ChessBoard &chessboard,int kingdex);
    bool move(Chess **arr,ChessBoard &chessboard,MoveAble moveable);
    bool win(Chess **arr,ChessBoard &chessboard);
    bool repentance(ChessBoard &chessboard);
    // 作弊部分
    bool changeTurn();
    // 步进描述部分
    void stepDescribe(sf::Font& font);
    void setText(int old_line_num, int chessName, int flagy, int new_line_num);
    void reverseColor();
    sf::Text& getText();
    std::string numToText(int line_num);
    std::string numToType(int chessName);
    std::string threeWords(int flagy);
};

class StopWatch{
private:
    sf::Clock clock;
    sf::Text text;
public:
    StopWatch(sf::Font &font);
    void setText();
    void reverseColor();
    void reStart();
    sf::Text &getText();
    float getElapsedTime();

friend class MoveAction;

};

#endif