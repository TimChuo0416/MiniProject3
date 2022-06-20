#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <array>

#include <cstring>
#include <limits.h>
#include <vector>

using namespace std;

enum SPOT_STATE {
    EMPTY = 0,
    BLACK = 1,
    WHITE = 2
};

enum board_state{
    OTHER = 0,
    WIN = 1,
    LOSE = 2,
    FLEX4 = 3,
    flex4 = 4,
    BLOCK4 = 5,
    block4 = 6,
    FLEX3 = 7,
    flex3 = 8,
    BLOCK3 = 9,
    block3 = 10,
    FLEX2 = 11,
    flex2 = 12,
    BLOCK2 = 13,
    block2 = 14,
    FLEX1 = 15,
    flex1 = 16
};

int player;
const int SIZE = 15;
std::array<std::array<int, SIZE>, SIZE> board;
int type[4][4][4][4][4][4];

void init_type()
{
    memset(type,0,sizeof(type));
    //0:empty 1:black chess 2:white 3:boundry
	//w贏
	type[2][2][2][2][2][2]=WIN;
	type[2][2][2][2][2][0]=WIN;
	type[0][2][2][2][2][2]=WIN;
	type[2][2][2][2][2][1]=WIN;
	type[1][2][2][2][2][2]=WIN;
	type[2][2][2][2][2][3]=WIN;
	type[3][2][2][2][2][2]=WIN;
	//b贏
	type[1][1][1][1][1][1]=LOSE;
	type[1][1][1][1][1][0]=LOSE;
	type[0][1][1][1][1][1]=LOSE;
	type[1][1][1][1][1][2]=LOSE;
	type[2][1][1][1][1][1]=LOSE;
	type[1][1][1][1][1][3]=LOSE;
	type[3][1][1][1][1][1]=LOSE;
	//w活4 
	type[0][2][2][2][2][0]=FLEX4;
	//b
	type[0][1][1][1][1][0]=flex4;
	//w
	type[0][2][2][2][0][0]=FLEX3;
	type[0][0][2][2][2][0]=FLEX3;
	type[0][2][0][2][2][0]=FLEX3;
	type[0][2][2][0][2][0]=FLEX3;
	//b
	type[0][1][1][1][0][0]=flex3;
	type[0][0][1][1][1][0]=flex3;
	type[0][1][0][1][1][0]=flex3;
	type[0][1][1][0][1][0]=flex3;
	//w
	type[0][2][2][0][0][0]=FLEX2;
	type[0][2][0][2][0][0]=FLEX2;
	type[0][2][0][0][2][0]=FLEX2;
	type[0][0][2][2][0][0]=FLEX2;
	type[0][0][2][0][2][0]=FLEX2;
	type[0][0][0][2][2][0]=FLEX2;
	//b
	type[0][1][1][0][0][0]=flex2;
	type[0][1][0][1][0][0]=flex2;
	type[0][1][0][0][1][0]=flex2;
	type[0][0][1][1][0][0]=flex2;
	type[0][0][1][0][1][0]=flex2;
	type[0][0][0][1][1][0]=flex2;
	//w
	type[0][2][0][0][0][0]=FLEX1;
	type[0][0][2][0][0][0]=FLEX1;
	type[0][0][0][2][0][0]=FLEX1;
	type[0][0][0][0][2][0]=FLEX1;
	//b
	type[0][1][0][0][0][0]=flex1;
	type[0][0][1][0][0][0]=flex1;
	type[0][0][0][1][0][0]=flex1;
	type[0][0][0][0][1][0]=flex1;
			
	int p1,p2,p3,p4,p5,p6,x,ix,y,iy;
	for(p1=0;p1<4;p1++){
		for(p2=0;p2<3;p2++){
			for(p3=0;p3<3;p3++){
				for(p4=0;p4<3;p4++){
					for(p5=0;p5<3;p5++){
						for(p6=0;p6<4;p6++){
							x=ix=y=iy=0;//x:black y:white
                            //l_x = r_x = l_y = r_y = 0
							if(p1==1)x++;
							else if(p1==2)y++;
									
							if(p2==1){x++;ix++;}
							else if(p2==2){y++;iy++;}
									
							if(p3==1){x++;ix++;}
							else if(p3==2){y++;iy++;}
									
							if(p4==1){x++;ix++;}
							else if(p4==2){y++;iy++;}
									
							if(p5==1){x++;ix++;}
							else if(p5==2){y++;iy++;}
									
							if(p6==1)ix++;
							else if(p6==2)iy++;

									
			  				if(p1==3 || p6==3){
								if(p1==3 && p6!=3){
									if(ix==0&&iy==4){
										if(type[p1][p2][p3][p4][p5][p6]==0)
										type[p1][p2][p3][p4][p5][p6] = BLOCK4;
									}
									if(ix==4 && iy==0){
										if(type[p1][p2][p3][p4][p5][p6]==0)
											type[p1][p2][p3][p4][p5][p6] = block4;
									}
									if(ix==0 && iy==3){
										if(type[p1][p2][p3][p4][p5][p6]==0)
											type[p1][p2][p3][p4][p5][p6] = BLOCK3;
									}
									if(ix==3 && iy==0){
										if(type[p1][p2][p3][p4][p5][p6]==0)
											type[p1][p2][p3][p4][p5][p6] = block3;
									}
									if(ix==0 && iy==2){
										if(type[p1][p2][p3][p4][p5][p6]==0)
											type[p1][p2][p3][p4][p5][p6] = BLOCK2;
									}
									if(ix==2 && iy==0){
										if(type[p1][p2][p3][p4][p5][p6]==0)
											type[p1][p2][p3][p4][p5][p6] = block2;
									}
								}
										 
								else if(p1!=3 && p6==3){
									if(x==0 && y==4){
										if(type[p1][p2][p3][p4][p5][p6]==0)
											type[p1][p2][p3][p4][p5][p6] = BLOCK4;
									}
									if(x==4 && y==0){
										if(type[p1][p2][p3][p4][p5][p6]==0)
											type[p1][p2][p3][p4][p5][p6] = block4;
									}
									if(x==0 && y==3){
										if(type[p1][p2][p3][p4][p5][p6]==0)
											type[p1][p2][p3][p4][p5][p6] = BLOCK3;
									}
									if(x==3 && y==0){
										if(type[p1][p2][p3][p4][p5][p6]==0)
											type[p1][p2][p3][p4][p5][p6] = block3;
									}
									if(x==0 && y==2){
										if(type[p1][p2][p3][p4][p5][p6]==0)
											type[p1][p2][p3][p4][p5][p6] = BLOCK2;
										}
									if(x==2 && y==0){
										if(type[p1][p2][p3][p4][p5][p6]==0)
											type[p1][p2][p3][p4][p5][p6] = block2;
									}
								}
							}
							//無邊界
							else{
								if((x==0 && y==4) || (ix==0 && iy==4)){
									if(type[p1][p2][p3][p4][p5][p6]==0)
										type[p1][p2][p3][p4][p5][p6] = BLOCK4;
								}
								if((x==4 && y==0) || (ix==4 && iy==0)){
									if(type[p1][p2][p3][p4][p5][p6]==0)
										type[p1][p2][p3][p4][p5][p6] = block4;
								}
								if((x==0 && y==3)|| (ix==0 && iy==3)){
									if(type[p1][p2][p3][p4][p5][p6]==0)
										type[p1][p2][p3][p4][p5][p6] = BLOCK3;
								}
								if((x==3 && y==0) || (ix==3 && iy==0)){
									if(type[p1][p2][p3][p4][p5][p6]==0)
										type[p1][p2][p3][p4][p5][p6] = block3;
								}
								if((x==0 && y==2) || (ix==0 && iy==2)){
									if(type[p1][p2][p3][p4][p5][p6]==0)
										type[p1][p2][p3][p4][p5][p6] = BLOCK2;
								}
								if((x==2 && y==0) || (ix==2 && iy==0)){
									if(type[p1][p2][p3][p4][p5][p6]==0)
										type[p1][p2][p3][p4][p5][p6] = block2;
								}
							} 
									 
									 
						}
					}
				}
			}
		}
	}	
}

class state{
    public:
    int x,y;
    //int people;
    //int depth;
    int h_value=0;
    array<array<int, SIZE>, SIZE> board;

    state(array<array<int, SIZE>, SIZE> b){
        for(int i=0;i<SIZE;i++){
            for(int j=0;j<SIZE;j++){
                board[i][j]=b[i][j];
            }
        }
        //cout << "hi\n";
        //set_h();
    }

    state(int a,int b){
        x=a;
        y=b;

        //set_h();
    }

    void set_h()
    {
        int score;

        //score judge
        int s[17]={0,1000000,-10000000,50000,-100000,400,-100000,400,-8000,20,-50,20,-50,1,-3,1,-3};
        //int s[17]={0,10000000,-10000000,100000,-100000,100000,-100000,8000,-8000,50,-50,50,-50,3,-3,3,-3};
        //int s[17]={0,100000,-100000,10000,-10000,5000,-5000,1000,-1000,20,-50,20,-50,1,-3,1,-3};
        if(player==2){
            s[1]=10000000,s[2]=-1000000,s[3]=100000,
            s[4]=-5000,s[5]=100000,s[6]=-400,
            s[7]=8000,s[8]=-400,s[9]=50,
            s[10]=-20,s[11]=50,s[12]=-20,
            s[13]=3,s[14]=-1,s[15]=3,s[16]=-1;
        }

        //how much the state have in the board
        int stat[4][17]={0};
        for(int i=0;i<4;i++){
            for(int j=0;j<17;j++) stat[i][j]=0;
        }
        //memset(stat,0,sizeof(stat));


        int A[17][17]={0}; //board
        for(int i=0;i<17;i++) A[i][0]=3;  //boundary
        for(int i=0;i<17;i++) A[i][16]=3;
        for(int i=0;i<17;i++) A[0][i]=3;
        for(int i=0;i<17;i++) A[16][i]=3;

        for(int i=0;i<15;i++){
            for(int j=0;j<15;j++){
                A[i+1][j+1]=board[i][j];
            }
        }

        int t;

        //row
        for(int i=1;i<=15;i++){
            for(int j=0;j<12;j++){
                t=type[A[i][j]][A[i][j+1]][A[i][j+2]][A[i][j+3]][A[i][j+4]][A[i][j+5]];
                stat[0][t]++;
                //if(t==15) cout << i << j <<"1\n";
            }
        }

        //col
        for(int i=1;i<=15;i++){
            for(int j=0;j<12;j++){
                t=type[A[j][i]][A[j+1][i]][A[j+2][i]][A[j+3][i]][A[j+4][i]][A[j+5][i]];
                stat[1][t]++;
                //if(t==15) cout << i << j <<"1\n";
            }
        }

        //
        for(int i=0;i<12;i++){
            for(int j=0;j<12;j++){
                t=type[A[i][j]][A[i+1][j+1]][A[i+2][j+2]][A[i+3][j+3]][A[i+4][j+4]][A[i+5][j+5]];
                stat[2][t]++;
                //if(t==15) cout << i << j <<"1\n";
            }
        }

        ///
        for(int i=0;i<12;i++){
            for(int j=5;j<17;j++){
                t=type[A[i][j]][A[i+1][j-1]][A[i+2][j-2]][A[i+3][j-3]][A[i+4][j-4]][A[i+5][j-5]];
                stat[3][t]++;
                //if(t==15) cout << i << j <<"1\n";
            }
        }

        score=0;
        for(int i=1;i<17;i++){
            int sum=stat[0][i]+stat[1][i]+stat[2][i]+stat[3][i];
            score=score+sum*s[i];
            //cout << sum << endl;
            //cout << "here" << (stat[0][i]+stat[1][i]+stat[2][i]+stat[3][i])*s[i] <<endl;
        }
        //cout << score << endl;

        h_value=score;
        //if(player==2) h_value=score*-1;
        //else h_value=score;
        //return score;
    }


    /*void update(int newx,int newy,int maxminingPlayer)
    {
        if(maxminingPlayer) board[newx][newy]=1;
        else board[newx][newy]=2;

        x=newx;
        y=newy;

        set_h();
    }*/
};

int alphabeta(state node,int depth,int a,int b,int maximiningPlayer)
{
    if(depth==0){
        node.set_h();
        return node.h_value;
    }

    if(maximiningPlayer){
        int value=INT_MIN;
        
        for(int i=0;i<15;i++){
            for(int j=0;j<15;j++){
                if(node.board[i][j]==0){
                    node.board[i][j]=player;
                    state next(node.board);
                    node.board[i][j]=0;
                    value=max(value,alphabeta(next,depth-1,a,b,0));
                    //a=max(a,value);
                    //if(a>=b) find=0; //break;
                    //if(a>=b) return value;
                }
            }
        }
        return value;
    }

    else{
        int value=INT_MAX;

        for(int i=0;i<15;i++){
            for(int j=0;j<15;j++){
                if(node.board[i][j]==0){
                    node.board[i][j]=3-player;
                    state next(node.board);
                    node.board[i][j]=0;
                    //next.update(i,j,1);
                    value=min(value,alphabeta(next,depth-1,a,b,1));
                    //b=min(b,value);
                    //if(b<=a) find=0; //break;
                    //if(b<=a) return value;
                }
            }
        }
        return value;
    }
}

void read_board(std::ifstream& fin) {
    fin >> player;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fin >> board[i][j];
        }
    }
}

void write_valid_spot(std::ofstream& fout) {
    srand(time(NULL));
    init_type();
    //int x, y;
    // Keep updating the output until getting killed.
    int x,y;
    int empty=1;
    int index_MAX=INT_MAX;
    int index_MIN=INT_MIN;

    int now_cor=(player==2);
    while(true) {
        // Choose a random spot
        //state now(board);
        for(int i=0;i<15;i++){
            for(int j=0;j<15;j++){
                if(board[i][j]!=0){
                    empty=0;
                    continue;
                }

                if(player==1) board[i][j]=1;
                else board[i][j]=2;
                //x=4,y=4;
                state now(board);
                
                int a;
                a=alphabeta(now,0,INT_MIN,INT_MAX,now_cor);
                

                //if(i==0 && j==0) cout << "0,0 " << a << endl;
                //if(i==7 && j==6) cout << "7,6 " << a << endl;

               //cout << "a" << a << "   ";

                board[i][j]=0;

                if(player==2 && a>index_MIN){ //white
                    //cout << player << a << endl;
                    index_MIN=a;
                    x=i;
                    y=j;
                }

                if(player==1 && a<index_MAX){
                    //cout << player << a << endl;
                    index_MAX=a;
                    x=i;
                    y=j;
                }
            }
        }
        if(empty){
            x=7;
            y=7;
            empty=0;
            //fout << x << " " << y << std::endl;
            //cout << x << " " << y << 'a' << endl;
            //board[7][7]=1;
            //fout.flush();
        }

        //x = (rand() % SIZE);
        //y = (rand() % SIZE);
        //if (board[x][y] == EMPTY) {
            fout << x << " " << y << std::endl;
            // Remember to flush the output to ensure the last action is written to file.
            fout.flush();
        //}
    }
}

int main(int, char** argv) {
    cout << player << endl;
    std::ifstream fin(argv[1]);
    std::ofstream fout(argv[2]);
    read_board(fin);
    write_valid_spot(fout);
    fin.close();
    fout.close();
    return 0;
}