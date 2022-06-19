#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <array>
#include <climits>
//#include "Board.h"
//#include "AI.h"
using namespace std;


enum SPOT_STATE {
    EMPTY = 0,
    BLACK = 1,
    WHITE = 2,
    OUTSIDE = 3
};

struct Pos //位置座標
{
	int x;
	int y;
};

struct Point //點
{
	int x;
    int y;
	int val;
};

const int Maxdepth = 1;

const int dx[4]={1,0,1,1};
const int dy[4]={0,1,1,-1};

const int win = 7; // 連五
const int flex4 = 6; // 活四
const int block4 = 5; // 衝四
const int flex3 = 4; // 活三
const int block3 = 3; // 死三
const int flex2 = 2; // 活二
const int block2 = 1; // 死二
int step;
int player,enemy;
const int SIZE = 15;
std::array<std::array<int, SIZE>, SIZE> board;

const int eval[8] = { 0, 2, 10, 16, 90, 140, 1000, 1500 };
class Board{
    public:
        Board();
        ~Board();
        Board(const std::array<std::array<int, SIZE>, SIZE> &rhs);
        std::array<std::array<int, SIZE>, SIZE> table;
        int step = 0;//現在第幾步
        //int depth = 0;//搜索層數
        int chesstype[3][8] = {0};
        Point best;
        int minimax(int role, int depth);
        Point search();
        int evaluate();
        int CheckXY(int x,int y);
        int Checktype(int x, int y, int role);
        int Check2(int x, int y, int role);
        int Check3(int x, int y, int role);
        int Check4(int x, int y, int role);
        int Check5(int x, int y, int role);
        //Pos
                
    private:
        
};

Board::Board(const std::array<std::array<int, SIZE>, SIZE> &rhs){
    table = rhs;
}
Board::~Board(){

}


void read_board(std::ifstream& fin) {
    fin >> player;
    enemy = 3 - player;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fin >> board[i][j];
            if(board[i][j] == BLACK || board[i][j] == WHITE)step++;
        }
    }
}

void write_valid_spot(std::ofstream& fout) {
    srand(time(NULL));
    int x, y;
    // Keep updating the output until getting killed.
    while(true) {
        // Choose a random spot.
        Board root = board;
        Point bestmove;
        bestmove.val = INT_MIN;
        // Pos best = root.minimax(player,0);
        int x = (rand() % SIZE);
        int y = (rand() % SIZE);
        if(step == 0){
            x = y = 7;
        }
        else if(step == 1 || step == 2){
            int rx, ry;
		    srand(time(NULL));
            for (int i = 0; i < SIZE; i++) {
                for (int j = 0; j < SIZE; j++) {
                    if(board[i][j] == enemy && step == 1){
                        rx = i + rand() % (step * 2 + 1) - step;
                        ry = j + rand() % (step * 2 + 1) - step;
                        break;
                    }
                    if(board[i][j] == player && step == 2){
                        rx = i + rand() % (step * 2 + 1) - step;
                        ry = j + rand() % (step * 2 + 1) - step;
                        break;
                    }
                }
            }
            x = rx;
            y = ry; 
        }
        else{
            bestmove = root.search();
            x = bestmove.x;
            y = bestmove.y;
            // cout << x << "," << y << endl;
            // cout << bestmove.val << endl;
        }
        if (board[x][y] == EMPTY) {
            fout << x << " " << y << std::endl;
            // Remember to flush the output to ensure the last action is written to file.
            fout.flush();
        }
    }
}

int Board::minimax(int role,int depth){
    int value;
    if(depth == Maxdepth){
        
    }
    else if(role == player){ //Maximize
        value = INT_MIN;
        for(int i = 0;i < SIZE;i++){

        }
    }
    else if(role == enemy){ //Minimize
        value = INT_MAX;
    }

    return value;
}

int Board::evaluate(){
    int value = 0;
    for(int i = 0;i < SIZE;i++){
        for(int j = 0;j < SIZE;j++){
            if(this->table[i][j] == player){
                int type = Checktype(i,j,player);
                if(type)
                    value += 1.2 * eval[type];                
            }else if(this->table[i][j] == enemy){
                int type = Checktype(i,j,enemy);
                if(type)
                    value -= eval[type]; 
            }
        }
    }
    if(chesstype[player][flex3] && chesstype[player][flex4]){
        value += 600;
    }else if(chesstype[enemy][flex3] && chesstype[enemy][flex4]){
        value -= 800;
    }
    return value;
}

Point Board::search(){
    int val;
    best.val = -1000;
    for(int i = 0;i < SIZE;i++){
        for(int j = 0;j < SIZE;j++){
            if(table[i][j] == EMPTY){
                table[i][j] = player;
                val = evaluate();
                if(val >= best.val){
                    best.x = i;
                    best.y = j;
                    best.val = val;
                }
                table[i][j] = EMPTY;
            }
        }
    }
    return best;
}
int Board::CheckXY(int x, int y){
    if(x >= 0 && x < SIZE && y >=0 && y < SIZE)
        return true;
    else
        return false;
}

int Board:: Checktype(int x, int y, int role){
    int type = 0;
    int c = 0;
    if(Check5(x,y,role)){
        type = win;   
    }else if(c = Check4(x, y, role)){
        type = c;
    }else if(c = Check3(x,y,role)){
        type = c;
    }else if(Check2(x,y,role)){
        type = flex2;
    }
    chesstype[role][type]++; 
    return type;
}

int Board::Check2(int x, int y, int role){
    int cnt;
    for(int i = 0; i < 4;i++){
        cnt = 0;
        int a = x - 2 * dx[i];
        int b = y - 2 * dy[i];
        if(!CheckXY(a,b)) continue;
        for(int j = 0;j < 4;j++){
            int tx = a + j * dx[i];
            int ty = b + j * dy[i];
            if(!CheckXY(tx,ty)) break;
            if(table[tx][ty] == (3 -role)) break;
            if(table[tx][ty] == role && (j == 1 || j == 2)){
                cnt++;
            }else if(table[tx][ty] == EMPTY && (j == 0|| j == 3)){
                cnt++;
            }
        }
        if(cnt == 4)
            return true;
    }
    return false;
}
int Board::Check3(int x, int y, int role){
    int cnt = 0,econt = 0;
    for(int i = 0; i < 4;i++){
        cnt = 0;
        int a = x - 2 * dx[i];
        int b = y - 2 * dy[i];
        if(!CheckXY(a,b)) continue;
        for(int j = 0;j < 5;j++){
            int tx = a + j * dx[i];
            int ty = b + j * dy[i];
            if(!CheckXY(tx,ty)) break;
            //if(table[tx][ty] == (3 -role)) break;
            if(table[tx][ty] == role && (j >= 1 || j <= 3)){
                cnt++;
            }else if(table[tx][ty] == EMPTY && (j < 1 || j > 3)){
                cnt++;
            }else if(table[tx][ty] == (3-role) && (j < 1 || j > 3)){
                econt++;
                cnt++;
            }
        }
        if(cnt == 5){
            if(!econt)
                return flex3;
            else if(econt < 2)
                return block3;
        }            
    }    
    return false;
}

int Board::Check4(int x, int y, int role){
    int cnt = 0,econt = 0;
    for(int i = 0; i < 4;i++){
        cnt = 0;
        int a = x - 2 * dx[i];
        int b = y - 2 * dy[i];
        if(!CheckXY(a,b)) continue;
        for(int j = 0;j < 6;j++){
            int tx = a + j * dx[i];
            int ty = b + j * dy[i];
            if(!CheckXY(tx,ty)) break;
            //if(table[tx][ty] == (3 -role)) break;
            if(table[tx][ty] == role && (j >= 1 || j <= 4)){
                cnt++;
            }else if(table[tx][ty] == EMPTY && (j < 1 || j > 4)){
                cnt++;
            }else if(table[tx][ty] == (3-role) && (j < 1 || j > 4)){
                econt++;
                cnt++;
            }
        }
        if(cnt == 6){
            if(!econt)
                return flex4;
            else if(econt < 2)
                return block4;      
        }
    }    
    return false;
}

int Board::Check5(int x, int y, int role){
    int cnt = 0,econt = 0;
    for(int i = 0; i < 4;i++){
        cnt = 0;
        int a = x - 2 * dx[i];
        int b = y - 2 * dy[i];
        if(!CheckXY(a,b)) continue;
        for(int j = 0;j < 5;j++){
            int tx = a + j * dx[i];
            int ty = b + j * dy[i];
            if(!CheckXY(tx,ty)) break;
            if(table[tx][ty] != role) break;
            if(table[tx][ty] == role)
                cnt++;            
        }
        if(cnt == 5)
            return true;                    
    }    
    return false;
}

int main(int, char** argv) {
    std::ifstream fin(argv[1]);
    std::ofstream fout(argv[2]);
    read_board(fin);
    write_valid_spot(fout);
    fin.close();
    fout.close();
    return 0;
}
