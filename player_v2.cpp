#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <array>
#include <climits>
#include <cstring>
//#include <cstdlib>
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
const int runtime = 1;
const int Maxdepth = 2;
int print = 1;
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
double start;
int player,enemy;
const int SIZE = 15;
std::array<std::array<int, SIZE>, SIZE> board;

const int eval[8] = { 0, 2, 12, 18, 80, 90, 800, 1500 };
class Board{
    public:
        Board();
        ~Board();
        Board(const std::array<std::array<int, SIZE>, SIZE> &rhs);
        std::array<std::array<int, SIZE>, SIZE> table;
        Cell cell[16][16];
        int step = 0;//現在第幾步
        int v,ev;
        int alpha = INT_MIN,beta = INT_MAX;
        int chesstype[3][8] = {0};
        int _win[3] = {0};
        int double3[3];
        int enemy_wining,me_wining;//win immediately
        int ewin,mwin;//win next step
        Point best;
        // Point minimax(int role, int depth);
        // int minimax(Board root,int role, int depth);
        Point search(int role);
        void init();
        void outchess();        
        int evaluate();

        int Checktype(int x, int y, int role);
        int Check2(int x, int y, int role);
        int Check3(int x, int y, int role);
        int Check4(int x, int y, int role);
        int Check5(int x, int y, int role);
        //Pos
                
    private:
        
};
void find_win(Board& root);
void find_lose(Board& root);
int CheckXY(int x,int y);
int mini_max(Board& now, int role,int depth,int alpha,int beta);
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
    int t = 0;
    // Keep updating the output until getting killed.
    while(runtime != t++) {
        // Choose a random spot.
        Board root(board);
        // root.outchess();
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
                        do{
                            rx = i + rand() % (3) - step;
                            ry = j + rand() % (3) - step;
                        } while(board[rx][ry] != EMPTY);
                        break;
                    }
                    if(board[i][j] == player && step == 2){
                        do{
                            rx = i + rand() % (3) - 1;
                            ry = j + rand() % (3) - 1;
                        } while(board[rx][ry] != EMPTY);
                        break;
                    }
                }
            }
            x = rx;
            y = ry; 
        }
        else{
            
            find_win(root);
            if(CheckXY(root.best.x,root.best.y)){
                x = root.best.x;
                y = root.best.y;
                fout << x << " " << y << std::endl;
                // Remember to flush the output to ensure the last action is written to file.
                fout.flush();
                return;
            }
            find_lose(root);
            if(CheckXY(root.best.x,root.best.y)){
                x = root.best.x;
                y = root.best.y;
                fout << x << " " << y << std::endl;
                // Remember to flush the output to ensure the last action is written to file.
                fout.flush();
                return;
            }
            start = clock();

            int max_value = mini_max(root,player,0,INT_MIN,INT_MAX);
            x = root.best.x;
            y = root.best.y;
            // root.outchess();
            // bestmove = root.minimax(player,0);
            // // bestmove = root.search();
            // x = bestmove.x;
            // y = bestmove.y;
            // 
            /*
            cout <<"(" << x << "," << y << ")" <<endl;
            cout << "val <" << root.best.val<<","<< max_value << ">" << endl;
            */
            // root.outchess();
        }
        
            // cout << x << "," << y << endl;
        if(!CheckXY(x,y)){
            do{
                x = (rand() % SIZE);
                y = (rand() % SIZE);
            }while(board[x][y] != EMPTY);
        }
        fout << x << " " << y << std::endl;
            // Remember to flush the output to ensure the last action is written to file.
        fout.flush();
        
        
    }
}

//Board::Board():table(){}
Board::Board(const std::array<std::array<int, SIZE>, SIZE> &rhs){
    init();
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
    evaluate();
}
Board::~Board(){

}

void Board::init(){
    best.x = -1;
    best.y = -1;
    v = 0;
    ev = 0;
    enemy_wining = 0;
    me_wining = 0;
    mwin = 0;
    ewin = 0;
    for(int i = 0;i < 3;i++){
        for(int j = 0;j < 8;j++){
            chesstype[i][j] = 0;
        }
        double3[i] = 0;
    }
    // outchess();
    // memset(double3,0,sizeof(int)); 
}

void Board::outchess(){
    if(!print)return;
    for(int i = 0;i < SIZE;i++){
        for(int j = 0;j < SIZE;j++){
            cout << table[i][j] << " ";
        }
        cout << endl;
    }
    cout << "Player: (";
    for(int i = 1;i <= win;i++){        
        cout << chesstype[player][i] << ",";
    }
    cout << ")\n";
    cout << "Enemy: [";
    for(int i = 1;i <= win;i++){
        cout  << chesstype[enemy][i] << ",";
    }
    cout << "]\n";
    cout<< "v ev {" << v <<","<< ev <<"}\n";
    print = 0;
}


void find_win(Board& root){
    for(int i = 0;i < SIZE;i++){
        for(int j = 0;j < SIZE;j++){
            if(root.table[i][j] == EMPTY){
                if(!root.cell[i][j].can)continue;
                root.table[i][j] = player;
                Board a(root.table);
                if(a.me_wining){
                    root.best.x = i;
                    root.best.y = j;
                    return;
                }
                root.table[i][j] = EMPTY;
            }
        }
    }
}
void find_lose(Board& root){
    for(int i = 0;i < SIZE;i++){
        for(int j = 0;j < SIZE;j++){
            if(root.table[i][j] == EMPTY){
                if(!root.cell[i][j].can)continue;
                root.table[i][j] = enemy;
                Board a(root.table);
                if(a.enemy_wining){
                    root.best.x = i;
                    root.best.y = j;
                    return;
                }
                root.table[i][j] = EMPTY;
            }
        }
    }
}

/*
Point Board::minimax(int role,int depth){
    Point ans;
    int st = 0;
    if(step <= 10)
        st = 3;
    else
        st = 0;
    
    if(depth == Maxdepth){
        // cout << "in" << endl;
        Board a (table);
        best = a.search(role);       
        return best;
    }
    else if(role == player){ //Maximize
        best.val = INT_MIN;
        for(int i = st;i < SIZE - st;i++){
            for(int j = st;j < SIZE - st;j++){
                if(table[i][j] == EMPTY){
                    if(!cell[i][j].can)continue;
                    table[i][j] = player;
                    Board a(table);
                    table[i][j] = EMPTY;
                    Point tmp = a.minimax(enemy,depth+1);
                    if(tmp.val > best.val){
                        a.outchess();
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
                    table[i][j] = enemy;
                    Board a(table);
                    table[i][j] = EMPTY;
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
*/
int mini_max(Board& now ,int role,int depth ,int alpha, int beta){
    Point best;
    int st = 0;
    
    
    if(depth == Maxdepth){
        // cout << "in" << endl;
        best = now.search(role);       
        return best.val;
    }
    else if(role == player){ //Maximize
        best.val = INT_MIN;
        for(int i = st;i < SIZE - st;i++){
            for(int j = st;j < SIZE - st;j++){
                if(now.table[i][j] == EMPTY){
                    if(!now.cell[i][j].can)continue;
                    now.table[i][j] = player;
                    Board a(now.table);
                    now.table[i][j] = EMPTY;
                    if((a.v - a.ev) <= (now.v-now.ev))continue;
                    
                    if(a.mwin){
                        now.best.x = i;
                        now.best.y = j;
                        return 1;
                    }
                
                    int tmp = mini_max(a,enemy,depth+1,alpha,beta);
                    //a.outchess();
                    if(tmp == -19880){
                        //if it is bad solution
                        now.best.x = a.best.x;
                        now.best.y = a.best.y;
                        return;
                    }
                    if(tmp > best.val){
                        best.x = i;
                        best.y = j;
                        best.val = tmp;
                    }
                    alpha = max(tmp,alpha);
                    if(alpha >= beta){

                        now.best.val = best.val;
                        now.best.x = best.x;
                        now.best.y = best.y;
                        return best.val;
                    }
                    if((clock() - start) / CLOCKS_PER_SEC > 9){
                        //copy the value and xy
                        now.best.val = best.val;
                        now.best.x = best.x;
                        now.best.y = best.y;
                        return best.val;
                    }
                    
                }                
            }
            // cout << "\n";
        }
        now.best.val = best.val;
        now.best.x = best.x;
        now.best.y = best.y;
        return best.val;
    }
    else if(role == enemy){ //Minimize
        best.val = INT_MAX;
        for(int i = st;i < SIZE - st;i++){
            for(int j = st;j < SIZE - st;j++){
                if(now.table[i][j] == EMPTY){
                    if(!now.cell[i][j].can)continue;
                    now.table[i][j] = enemy;
                    Board a(now.table);
                    now.table[i][j] = EMPTY;
                    if((a.v - a.ev) > (now.v-now.ev))continue;
                    
                    if(a.ewin){
                        now.best.x = i;
                        now.best.y = j;
                        return -19880;
                    }
                    
                    int tmp = mini_max(a,player,depth+1,alpha,beta);
                    
                    if(tmp <= -20000){
                        now.best.val = tmp;
                        now.best.x = i;
                        now.best.y = j;
                        return tmp;
                    }
                    if(tmp <= best.val){
                        // a.outchess();
                        // best.x = i;
                        // best.y = j;
                        best.val = tmp;
                    }
                    beta = min(tmp,beta);
                    // table[i][j] = EMPTY;
                    if(alpha >= beta){
                        //fout << best.x << " " << best.y << endl;
                        now.best.val = best.val;
                        //now.best.x = best.x;
                        //now.best.y = best.y;
                        return best.val;
                    }
                }
            }            
        }       
        now.best.val = best.val;
        // now.best.x = best.x;
        // now.best.y = best.y;
        return best.val;
    }   
}

int Board::evaluate(){
    int value = 0,ev = 0;
    init();
    for(int i = 0;i < SIZE;i++){
        for(int j = 0;j < SIZE;j++){
            if(this->table[i][j] == player){
                int type = Checktype(i,j,player);                
            }else if(this->table[i][j] == enemy){
                int type = Checktype(i,j,enemy);
            }
        }
    }
    for(int i = 1;i < 8;i++){
        value += eval[i]*chesstype[player][i];
    }
    for(int i = 1;i < 8;i++){
        ev += eval[i]*chesstype[enemy][i]*1.1;
    }
    if(chesstype[player][win]) {
        value += 20000;
        me_wining = 1;
    }
    if(chesstype[enemy][win]){
        ev += 20000;
        enemy_wining = 1;  
    }
    if(chesstype[player][flex4]) {
        value += 3000;
        mwin = 1;
    }if(chesstype[enemy][flex4]) {
        ev += 5000;
        ewin = 1;
        // enemy_wining = 1;
        // cout<<"Enemy wining!\n";
    }
    /*
    if(chesstype [enemy][block4]){
        ev += 8000;
    }
    */
     
    if(double3[player] >= 2)
        value += 1000;
    else if(double3[player] && chesstype[player][block4])
        value += 2000;
    if(double3[enemy] >= 2){
        // cout << "DOUBLE3\n";
        ev += 1200;
    }
    else if(double3[enemy] && chesstype[enemy][block4])
        ev += 2400;    
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
   
    this->v = value;
    this->ev = ev;
    this->best.val = (value - ev);
    // if(print){
    //     outchess();
    //     print = 0;
    // }
    return (value - ev);
}

Point Board::search(int role){
    int val;
    best.val = INT_MIN;
    for(int i = 0;i < SIZE;i++){
        for(int j = 0;j < SIZE;j++){
            if(table[i][j] == EMPTY){
                if(!cell[i][j].can)continue;
                table[i][j] = role;
                Board a(table);
                table[i][j] = EMPTY;                
                val = a.v - a.ev;
                
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
    // outchess();
    return best;
}
int CheckXY(int x, int y){
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
    // outchess();
    // this->chesstype[role][type]++; 
    return type;
}

int Board::Check2(int x, int y, int role){
    int cnt,flag = 0,ecnt;
    for(int i = 0; i < 4;i++){
        cnt = 0;
        ecnt = 0;
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
            }else if(table[tx][ty] == (3-role) && (j == 0|| j == 3)){
                cnt++;
                ecnt++;
                break;
            }
        }
        if(cnt == 4 && !ecnt){
            chesstype[role][flex2]++;
            flag = 1;
        }else if(cnt == 4 && (ecnt == 1)){
            chesstype[role][block2]++;
        }
    }
    return flag;
}
int Board::Check3(int x, int y, int role){
    int cnt = 0,flag = 0;
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
            }else{
                break;
            }
        }
        if(cnt == 5){
            double3[role]++;
            chesstype[role][flex3]+=1;
            flag = 1;
            // outchess();
            // cout << chesstype[role][flex3] << endl;            
            // cout << role << " f3: (" << x << "," << y << ")" << endl;
        }            
    }    
    if(flag)
        return flex3;
}

int Board::Check4(int x, int y, int role){
    int cnt = 0,econt = 0,flag = 0;
    for(int i = 0; i < 4;i++){
        cnt = 0;
        econt = 0;
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
                chesstype[role][flex4] += 1;
                return flex4;
            }
            else if(econt < 2){
                //cout << "block4\n";
                chesstype[role][block4] += 1;
                flag = block4;
                // return block4;
            }      
        }
    }    
    return flag;
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
            chesstype[role][win] ++;
            //_win[role] = true;
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