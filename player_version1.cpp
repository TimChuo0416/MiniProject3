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

struct Cell //位置座標
{
	//Point p;
    int can;
};

struct Point //點
{
	int x;
    int y;
	int val;
};

const int Maxdepth = 2;

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

const int eval[8] = { 0, 2, 12, 18, 96, 144, 800, 1200 };
class Board{
    public:
        Board();
        ~Board();
        Board(const std::array<std::array<int, SIZE>, SIZE> &rhs);
        std::array<std::array<int, SIZE>, SIZE> table;
        Cell cell[16][16];
        int step = 0;//現在第幾步
        int alpha = INT_MIN,beta = INT_MAX;
        int chesstype[3][8] = {0};
        int _win[3] = {0};
        int double3[3] = {0};
        Point best;
        Point minimax(int role, int depth);
        Point search();
        void init();
        void outchess();
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
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            table[i][j] = rhs[i][j];
            if(board[i][j] == BLACK || board[i][j] == WHITE){                
                for(int a = -1; a <= 1;a++){
                    for(int b = -1;b <= 1;b++){
                        if(!CheckXY(i+a,j+b)|| (a==0 && b == 0)) continue;
                        cell[i+a][j+b].can++;
                    }
                }
            }
        }
    }
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
        // cout << step <<endl;
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
            bestmove = root.minimax(player,0);
            // bestmove = root.search();
            x = bestmove.x;
            y = bestmove.y;
            // cout << x << "," << y << endl;
            // cout << bestmove.val << endl;
            // root.outchess();
        }
        if (board[x][y] == EMPTY) {
            // cout << x << "," << y << endl;
            fout << x << " " << y << std::endl;
            // Remember to flush the output to ensure the last action is written to file.
            fout.flush();
        }
    }
}
void Board::outchess(){
    cout << "Player:";
    for(int i = 1;i <= win;i++){
        cout  << chesstype[player][i] << ",";
    }
    cout << endl;
    cout << "Enemy:";
    for(int i = 1;i <= win;i++){
        cout  << chesstype[enemy][i] << ",";
    }
    cout << endl;
}

Point Board::minimax(int role,int depth){
    Point ans;
    int st = 0;
    if(step <= 20)
        st = 3;
    else
        st = 0;
    
    if(depth == Maxdepth){
        // cout << "in" << endl;
        Board a = board;
        best = a.search();       
        return best;
    }
    else if(role == player){ //Maximize
        best.val = INT_MIN;
        for(int i = st;i < SIZE - st;i++){
            for(int j = st;j < SIZE - st;j++){
                if(table[i][j] == EMPTY){
                    if(!cell[i][j].can)continue;
                    Board a = board;
                    a.table[i][j] = player;
                    Point tmp = a.minimax(enemy,depth+1);
                    if(tmp.val > best.val){
                        best.x = tmp.x;
                        best.y = tmp.y;
                        best.val = tmp.val;
                    }
                    alpha = max(best.val,alpha);
                    if(alpha >= beta){
                        //fout << best.x << " " << best.y << endl;
                        return best;
                    }
                    // table[i][j] = EMPTY;
                    // cout << best.x << "," << best.y << "value:" << best.val <<endl;
                }
            }
            // cout << "\n";
        }
        return best;
    }
    else if(role == enemy){ //Minimize
        best.val = INT_MAX;
        for(int i = st;i < SIZE - st;i++){
            for(int j = st;j < SIZE - st;j++){
                if(table[i][j] == EMPTY){
                    if(!cell[i][j].can)continue;
                    Board a = board;
                    a.table[i][j] = enemy;
                    Point tmp = a.minimax(player,depth+1);
                    if(tmp.val < best.val){
                        best.x = tmp.x;
                        best.y = tmp.y;
                        best.val = tmp.val;
                    }
                    beta = min(best.val,beta);
                    // table[i][j] = EMPTY;
                    if(alpha >= beta)
                        return best;
                }
            }
            
        }       
        return best;
    }   
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
    if(_win[player]) value += 12000;
    if(_win[enemy]) {
        value -= 10000;
        // cout<<"Enemy wining!\n";
    }
    if(double3[player] >= 2)
        value += 8000;
    if(double3[enemy] >= 2){
        // cout << "DOUBLE3\n";
        value -= 7000;
    }
    // outchess();
    /*
    if(chesstype[player][flex3] && chesstype[player][flex4]){
        value += 600;
    }else if(chesstype[enemy][flex3] && chesstype[enemy][flex4]){
        value -= 800;
    }
    if(chesstype[enemy][flex4] || chesstype[enemy][block4]){
        value -= 1000;
    }
    */
    return value;
}

Point Board::search(){
    int val;
    best.val = INT_MIN;
    for(int i = 0;i < SIZE;i++){
        for(int j = 0;j < SIZE;j++){
            if(table[i][j] == EMPTY){
                Board a = board;
                a.table[i][j] = player;
                val = a.evaluate();
                if(val >= best.val){
                    best.x = i;
                    best.y = j;
                    best.val = val;
                }
                if(best.val >= 12000)
                    return best;
                // table[i][j] = EMPTY;
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
    }else if(Check4(x, y, role)){
        type = flex4;
    }else if(Check3(x,y,role)){
        type = flex3;
    }else if(Check2(x,y,role)){
        type = flex2;
    }
    // this->chesstype[role][type]++; 
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
            return flex2;
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
            }
        }
        if(cnt == 5){
            double3[role]++;
            return flex3;
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
            if(!econt){
                //cout << "flex4\n";
                _win[role] = true;
                return flex4;
            }
            else if(econt < 2)
                //cout << "block4\n";
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
        if(cnt == 5){
            //cout << "win"<<endl;
            _win[role] = true;
            return true;
        }                    
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
