#include "Chess.h"
#include "ChessLogic.h"
#include "ChessBoard.h"
#include <SFML/Graphics.hpp>
#include <typeinfo>
#include <iostream>
#include<string>
#include <windows.h>  //���ͷ�ļ��������
#include <iomanip>
#include <fstream>

using namespace std;

wstring stringTowstring(string str)
{
    int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
    if (len == 0)
        return wstring(L"");
    wchar_t* wct = new wchar_t[len];
    if (!wct)
        return std::wstring(L"");

    MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, wct, len);
    wstring wstr(wct);
    delete[] wct;
    wct = NULL;
    return wstr;
}
//��������
int word_line_num = 0;
///MoveAble
MoveAble::MoveAble(bool canmove, Chess *sp, int nx, int ny) : 
canmove(canmove), sp(sp) ,nx(nx), ny(ny) {}

bool MoveAble::getCanMove(){
    return canmove;
}

Chess *MoveAble::getChess(){
    return sp;
}

int MoveAble::getX(){
    return nx;
}

int MoveAble::getY(){
    return ny;
}

///OneMove
OneMove::OneMove(int sx,int sy,Chess *s,Chess *t) :
sx(sx),sy(sy),s(s),t(t){}

Chess *OneMove::getS(){
    return s;
}

Chess *OneMove::getT(){
    return t;
}

int OneMove::getSx(){
    return sx;
}

int OneMove::getSy(){
    return sy;
}

///MoveAction
MoveAction::MoveAction(){
    turn = -1;
    selected = -1;
    while(!withdrawstk.empty()) withdrawstk.pop();
}

int MoveAction::getSelected(){
    return selected;
}

int MoveAction::getTurn(){
    return turn;
}
//������ת��Ϊ����
std::string MoveAction::numToText(int line_num)
{
    switch (line_num) {
    //case 1: return L"һ";
    //case 2: return L"��";
    //case 3: return L"��";
    //case 4: return L"��";
    //case 5: return L"��";
    //case 6: return L"��";
    //case 7: return L"��";
    //case 8: return L"��";
    //case 9: return L"��";
    case 1: return "һ";
    case 2: return "��";
    case 3: return "��";
    case 4: return "��";
    case 5: return "��";
    case 6: return "��";
    case 7: return "��";
    case 8: return "��";
    case 9: return "��";
    default: return "";
    }
}
//����ת��Ϊ��������
std::string MoveAction::numToType(int chessName)
{
    //�����ڸ� 
    switch (chessName) {
    //case 4: return L"˧";
    //case -4: return L"��";
    //case 1: return L"��";
    //case -1: return L"��";
    //case 3: return L"��";
    //case -3: return L"ʿ";
    //case 2: return L"��";
    //case -2: return L"��";
    //case 7: return L"��";
    //case -7: return L"��";
    //case 8: return L"܇";
    //case -8: return L"܇";
    //case 6: return L"��";
    //case -6: return L"��";

    case 4: return "˧";
    case -4: return "��";
    case 1: return "��";
    case -1: return "��";
    case 3: return "��";
    case -3: return "ʿ";
    case 2: return "��";
    case -2: return "��";
    case 7: return "��";
    case -7: return "��";
    case 8: return "܇";
    case -8: return "܇";
    case 6: return "��";
    case -6: return "��";
    default: return "";
    }
}
//����ƽ
//�����á�ƽ������Է�����ǰ���á��������򼺷����ߺ����á��ˡ����췽�ú��֣��ڷ��ð��������ֱ�ʾ��
//flag��y�Ĳ� old_num - new_num
std::string MoveAction::threeWords(int flagy)
{
    //if (flagy == 0)
    //{
    //    return L"ƽ";
    //}
    //if (flagy > 0)
    //{
    //    return L"��";
    //}
    //if (flagy < 0)
    //{
    //    return L"��";
    //}
    if (flagy == 0)
    {
        return "ƽ";
    }
    if (flagy > 0)
    {
        return "��";
    }
    if (flagy < 0)
    {
        return "��";
    }
}
// ��������
//����ƽ��y ������x
//line_num������ ����ҵ��� �ڴ�����
void MoveAction::setText(int old_line_num, int chessName, int flagy,int new_line_num)
{
    std::string str1, str2, str3, str4;

    str1 = numToType(chessName); // ��
    //����Ǻ� line_num��ת
    if (chessName > 0)
    {
        old_line_num = 9 - old_line_num;
        new_line_num = 9 - new_line_num;
    }
    //����Ǻ� line_num+1
    if (chessName < 0)
    {
        old_line_num += 1;
        new_line_num += 1;
    }
    if (chessName > 0)
    {
        str2 = numToText(old_line_num); // ��
    }
    if (chessName < 0)
    {
        //str2.push_back(old_line_num);
       // str2.append(old_line_num); // ��
        str2.append(to_string(old_line_num));
    }
    //����Ǻ� flag��ת
    if (chessName > 0)
    {
        flagy = -flagy;
    }
    str3 = threeWords(flagy); // ��
    if (flagy < 0)
    {
        flagy = -flagy;
    }
    //�����܇ �� �� �� �����Ǹ���
    if (chessName == 8 || chessName == 7 || chessName == 6 || chessName == 4
        ||chessName == -8 || chessName == -7 || chessName == -6 || chessName == -4)
    {
        if (flagy != 0)
        {
            if (chessName > 0)
            {
                str4 = numToText(flagy); // ��
            }
            else {
                str4.append(to_string(flagy));
            }
        }
        //�����ƽ
        if (flagy == 0)
        {
            if (chessName > 0)
            {
                str4 = numToText(new_line_num); // ��
            }
            else {
                str4.append(to_string(new_line_num));
            }
        }

    }
    //������� �� ��
    if (chessName == 1 || chessName == 2 || chessName == 3
        || chessName == -1 || chessName == -2 || chessName == -3)
    {
        if (chessName > 0)
        {
            str4 = numToText(new_line_num); // ��
        }
        else {
            str4.append(to_string(new_line_num));
        }
    }
    //���֮ǰ��
    //���뻻��
    str.append("\n");
    wstring wstr1 = stringTowstring(str1);
    wstring wstr2 = stringTowstring(str2);
    wstring wstr3 = stringTowstring(str3);
    wstring wstr4 = stringTowstring(str4);
    wstr += L"\n";
    wstr.append(wstr1);
    wstr.append(wstr2);
    wstr.append(wstr3);
    wstr.append(wstr4);
    //append����
    str.append(str1);
    str.append(str2);
    str.append(str3);
    str.append(str4);
    stringTowstring(str4);
    std::wcout << wstr << std::endl;
    text.setString(wstr);
}
void MoveAction::stepDescribe(sf::Font& font)
{
    text.setFont(font);
    text.setStyle(sf::Text::Bold);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::Red);
    text.setPosition(930, 30 + word_line_num * 20);
    word_line_num++;
}
void MoveAction::reverseColor(){
    if (text.getFillColor() == sf::Color::Red) {
        text.setFillColor(sf::Color::Black);
    }
    else text.setFillColor(sf::Color::Red);
}
sf::Text& MoveAction::getText() {
    return text;
}
MoveAble MoveAction::canMove(Chess **arr,ChessBoard &chessboard,sf::Vector2i &pos){
    int dex = -1;
    //�����һ�����ӱ�����
    for (int i = 0; i < CHESS_NUMBER; i++){
        if (arr[i]->getSprite().getGlobalBounds().contains(pos.x, pos.y)){
            dex = i;
            break;
        }
    }
    // std::cout << "dex = " << dex << endl;
    // if(dex != -1) std::cout << "type = " << arr[dex]->getType() << endl;
    if (dex != -1){
        if (selected == -1){
            if (arr[dex]->getType() == turn) selected = dex;
        }else{
            //�ظ�ѡ
            if (arr[selected]->getType() == arr[dex]->getType()) selected = dex;
            //����ѡ
            else{
                // cout << "enter1" << endl;
                if (arr[selected]->legalMove(arr[dex]->getX(), arr[dex]->getY(), chessboard))
                    return MoveAble(true,arr[selected],arr[dex]->getX(),arr[dex]->getY());
                else{
                    selected = -1;
                    return MoveAble(false);
                }
            }
        }
    }
    //����Ƿ����ƶ����Ǹ���
    else{
        // cout << "enter" << endl;
        if (selected == -1) return MoveAble(false);
        int nx = -1, ny;
        //�ҵ��Ǹ��㣬���м���
        for (int i = 0; i < 9; i++){
            for (int j = 0; j < 10; j++){
                int ox = 90 + i * 89;
                int oy = 50 + j * 89;
                if ((ox - pos.x) * (ox - pos.x) + (oy - pos.y) * (oy - pos.y) <= 1936 /* 44^2 */){
                    nx = i;
                    ny = j;
                    break;
                }
            }
            if (nx != -1) break;
        }
        //û�ҵ�
        if (nx == -1){
            selected = -1;
            return MoveAble(false);
        }
        // std::cout << "nx = " << nx << "ny = " << ny << endl;
        if (arr[selected]->legalMove(nx, ny, chessboard)) return MoveAble(true,arr[selected],nx,ny);
        else selected = -1;
    }
    return MoveAble(false);
}

bool MoveAction::checkMate(Chess **arr,ChessBoard &chessboard,int kingdex){
    for(int i = 0 ; i < CHESS_NUMBER ; i++ ){
        if(arr[i]->getX() == -2) continue;
        if(arr[i]->legalMove(arr[kingdex]->getX(),arr[kingdex]->getY(),chessboard)){
            return true;
        }
    }
    int kingdex2 = -1;
    for(int i = 0 ; i < CHESS_NUMBER ; i++ ){
        if(typeid(*arr[i]) == typeid(King) && i != kingdex){
            kingdex2 = i;
            break;
        }
    }
    if(arr[kingdex]->getX() != arr[kingdex2]->getX()) return false;
    bool facetoface = true;
    int y1 = arr[kingdex]->getY(),y2 = arr[kingdex2]->getY();
    for(int i = std::min(y1,y2) + 1 ; i < std::max(y1,y2) ; i++ ){
        if(chessboard.hasChess(arr[kingdex]->getX(),i)){
            facetoface = false;
        }
    }
    return facetoface;
}

bool MoveAction::move(Chess **arr,ChessBoard &chessboard,MoveAble moveable){
    // �õ�λ������
    bool ifeat = false;
    Chess *itr = nullptr;
    for(int i = 0 ; i < CHESS_NUMBER ; i++ ){
        if(arr[i]->getX() == moveable.getX() && arr[i]->getY() == moveable.getY()){
            itr = arr[i];
            ifeat = true;
            break;
        }
    }
   // ��ǰ������
    ChessBoard tmp = chessboard;
    // �õ������ߵ�����
    int sx = moveable.getChess()->getX();
    int sy = moveable.getChess()->getY();
    //���ӵ�����
    int chessname = moveable.getChess()->getchessName();
    tmp.setVal(sx,sy,0);
    tmp.setVal(moveable.getX(),moveable.getY(),turn);
    if(itr != nullptr){
        itr->setX(-2);
        itr->setY(-2);
    }
    //�����ӷŵ��µ�λ��
    moveable.getChess()->setX(moveable.getX());
    moveable.getChess()->setY(moveable.getY());
  /*  if (ifeat)
    {
        eat.play();
    }*/
    // �õ����������
    int sx2 = moveable.getChess()->getX();
    int sy2 = moveable.getChess()->getY();
    //�¼���
    int flagy = sy2 - sy;
    //��������
    setText(sx, chessname, flagy, sx2);

    int dex = -1;
    for(int i = 0 ; i < CHESS_NUMBER ; i++ ){
        if(arr[i]->getType() == turn && typeid(*arr[i]) == typeid(King)){
            dex = i;
            break;
        }
    }
    if(checkMate(arr,tmp,dex)){
        selected = -1;
        moveable.getChess()->setX(sx);
        moveable.getChess()->setY(sy);
        if(itr != nullptr){
            itr->setX(moveable.getX());
            itr->setY(moveable.getY());
        }
        return false;
    }
    selected = -1;
    turn = -turn;
    chessboard = tmp;
    withdrawstk.push(OneMove(sx,sy,moveable.getChess(),itr));
    return true;
}
//ת��λ��
bool MoveAction::changeTurn()
{
    turn = -turn;
    return true;
}

// ��������

//bool MoveAction::stepDescribe(sf::Font& font, int x, int y)
//{
//    return true
//}
bool MoveAction::win(Chess **arr,ChessBoard &chessboard){
    int dex = -1;
    for(int i = 0 ; i < CHESS_NUMBER ; i++ ){
        if(arr[i]->getType() == turn && typeid(*arr[i]) == typeid(King)){
            dex = i;
            break;
        }
    }
    // Maybe not checkmate at this moment, but will checkmate
    // if do any legal move
    // if(!checkMate(arr,chessboard,dex)) return false;
    bool escape = false;
    Chess *itr = nullptr;
    ChessBoard tmp = chessboard;
    for(int i = 0 ; i < CHESS_NUMBER && !escape; i++ ){
        if(arr[i]->getType() != turn) continue;
        int sx = arr[i]->getX(),sy = arr[i]->getY();
        for(int j = 0 ; j < 9 && !escape; j++ ){
            for(int k = 0 ; k < 10 && !escape; k++ ){
                if(arr[i]->legalMove(j,k,chessboard)){
                    for(int p = 0 ; p < CHESS_NUMBER ; p++ ){
                        if(arr[p]->getX() == j && arr[p]->getY() == k){
                            itr = arr[p];
                            break;
                        }
                    }
                    if(itr != nullptr){
                        itr->setX(-2);
                        itr->setY(-2);
                    }
                    arr[i]->setX(j);
                    arr[i]->setY(k);
                    tmp.setVal(j,k,turn);
                    tmp.setVal(sx,sy,0);
                    if(!checkMate(arr,tmp,dex)){
                        escape = true;
                    }
                    arr[i]->setX(sx);
                    arr[i]->setY(sy);
                    tmp.setVal(j,k,0);
                    if(itr != nullptr){
                        itr->setX(j);
                        itr->setY(k);
                        tmp.setVal(j,k,-turn);
                        itr = nullptr;
                    }
                    tmp.setVal(sx,sy,turn);
                }
            }
        }
    }
    return !escape;
}

bool MoveAction::repentance(ChessBoard &chessboard){
    //����
    if(withdrawstk.empty()) return false;
    //����
    OneMove onemove = withdrawstk.top();
    withdrawstk.pop();
    //��ԭT
    int tx = onemove.getS()->getX();
    int ty = onemove.getS()->getY();
    //����гԵ�
    if(onemove.getT() != nullptr){
        onemove.getT()->setX(tx);
        onemove.getT()->setY(ty);
        chessboard.setVal(tx,ty,onemove.getT()->getType());
    }
    //ԭ��λ��Ϊ��
    else chessboard.setVal(tx,ty,0);
    //��ԭS
    onemove.getS()->setX(onemove.getSx());
    onemove.getS()->setY(onemove.getSy());
    chessboard.setVal(onemove.getSx(),onemove.getSy(),onemove.getS()->getType());
    turn = -turn;
    selected = -1;
    return true;
}

///StopWatch
StopWatch::StopWatch(sf::Font &font){
    text.setFont(font);
    text.setFillColor(sf::Color::Red);
    text.setPosition(892 / 2 - 38, 900 / 2 - 15);
}

void StopWatch::reStart() {
    clock.restart();
}
void StopWatch::setText(){
    int t = clock.getElapsedTime().asSeconds();
    t = TIME_LIMIT_PER_MOVE - t;
    int minutes = t / 60;
    int seconds = t % 60;
    std::string str;
    str.push_back('0' + minutes / 10);
    str.push_back('0' + minutes % 10);
    str.push_back(':');
    str.push_back('0' + seconds / 10);
    str.push_back('0' + seconds % 10);
    text.setString(str);
}

void StopWatch::reverseColor(){
    if(text.getFillColor() == sf::Color::Red){
        text.setFillColor(sf::Color::Black);
    }else text.setFillColor(sf::Color::Red);
}

// ��Ϊ��˽�г�Ա ֱ���ò��� ��Ҫ�ó�Ա�����õ�
sf::Text &StopWatch::getText(){
    return text;
}

float StopWatch::getElapsedTime(){
    return clock.getElapsedTime().asSeconds();
}