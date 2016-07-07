#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <string>
#include <vector>

using namespace std;
char tmp_map[18][32]; //a temporary map that used to calculate steps and walk path for enemy
//map
char gameMap[18][32] = {
	"+#############################+",
	"|                             |",
	"|                             |",
	"|## ########### ##   #########|",
	"|   |                         |",
	"| | |### |  |           |     |",
	"| |      |  | |###  |   |  |  |",
	"| | #####|  | |      ## |     |",
	"| |           |###  |      |  |",
	"| |##### ###         ##       |",
	"|          ######  ####### ###|",
	"|                             |",
	"|# ### ####      ###   #######|",
	"|                             |",
	"|      ###################    |",
	"|                             |",
	"|                             |",
	"+#############################+"
	};
bool play=true;
//print map
void ShowMap()
{
	for(int i = 0; i < 18; i++) {//print level by level
		cout<<gameMap[i]<<endl;
	}
}
//set cursor
void gotoxy( short x, short y )
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE) ;
    COORD position = { x, y } ;

    SetConsoleCursorPosition( hStdout, position ) ;
}


struct walk {//used to fill the map and generate path between enemy and hero.
	short x;
	short y;
	short back;
};

struct targetPath {//this is used for storing and later using the path between enemy and hero
	short x;
	short y;
};

vector<targetPath> walk_queue;

vector<walk> BFSArray;

void fillTheMap( int x, int y, int back ){
	if( tmp_map[y][x] == ' ' || tmp_map[y][x] == '.' ){
		tmp_map[y][x] = '#'; //set the position to # so that next round of path search it wont go back.
		walk tmp;
		tmp.x = x;
		tmp.y = y;
		tmp.back = back;
		BFSArray.push_back( tmp ); //push back each every possible step
	}
}

void FindPath( int ex, int ey, int hx, int hy ){//everytime calling FindPath, the tmp_map is reset to map
	memcpy( tmp_map, gameMap, sizeof(gameMap) );//copy map to another temporary 2d array called tmp_map
	BFSArray.clear();
	walk currentEnemyPos;//store initial position of enemy
	currentEnemyPos.x = ex;
	currentEnemyPos.y = ey;
	currentEnemyPos.back = -1;  //the initial position of enemy
	BFSArray.push_back( currentEnemyPos );

	for(int i=0; i<BFSArray.size(); i++){//for loop that fill the map until we fint the hero
		if( BFSArray[i].x == hx && BFSArray[i].y == hy ){ //while filling the map, if we found the target(Hero)
			walk_queue.clear();
			targetPath step;
			while( BFSArray[i].back != -1 ){//before reaching the initial position of enemy itself
				step.x = BFSArray[i].x;
				step.y = BFSArray[i].y;
				walk_queue.push_back( step );
//importan! this lead the path from hero to enemy. back stores the position of previous step position in BFSArray.
				i = BFSArray[i].back;
			}

			break;
		}

		fillTheMap( BFSArray[i].x+1, BFSArray[i].y, i );
		fillTheMap( BFSArray[i].x-1, BFSArray[i].y, i );
		fillTheMap( BFSArray[i].x, BFSArray[i].y+1, i );
		fillTheMap( BFSArray[i].x, BFSArray[i].y-1, i );
	}

	BFSArray.clear();
}


int main() {
    while(play) { //if later player lost and chose to quit (q) then play set to false.
        int hx = 15; // initialize hero x
        int hy = 16; // initialize hero y

        int ex = 1; // initialize enemy x
        int ey = 1; // initialize enemy y

        int pts = 0;  // initialize points
        int frame = 0; // starting frame. every time the thisRound loop restart, frame+1. it is for controlling speed of enemy

        cout<<endl<<"H -> Hard\nN -> Normal\nE -> Easy\n\nInput : "<<endl;//prompt to choose speed
        //choose easy normal or hard levels. speedmod is later set using frame(how many time does the map refreshes)
        char difficulty;
        int speedmod;
        cin >> difficulty;
        switch (difficulty) {
        case 'E': case 'e':
            speedmod = 3;
        break;
        case 'N': case 'n':
            speedmod = 2;
        break;
        case 'H': case 'h':
            speedmod = 1;
        break;
        default:
            speedmod = 2;
        }
        //End of setting speedmode

        system("cls");
        ShowMap(); //print map

        gotoxy( hx, hy );
        cout << "H";//locate and show hero

        FindPath( ex,ey,hx,hy );//ex ey: enemy position, x y: hero position
        bool thisRound=true;//start the new round. if later player is dead, then thisRound reset to false

        while(thisRound){
            gotoxy( 32, 1 ); cout << pts;//display score
            Sleep( 100 ); //delay
            frame++;
            int old_hx = hx;//store current user position
            int old_hy = hy;
            gotoxy(old_hx, old_hy); cout << " ";//reset the trail of hero to empty

            //start updating the location of Hero. as well as points
            if ( GetAsyncKeyState( VK_UP ) ){//the left upper corner is 1. so y-axis is flipped (less go up, larger go down)
                if( gameMap[hy-1][hx] == '.' ){gameMap[hy-1][hx] = ' '; hy--; pts++;}
                else if( gameMap[hy-1][hx] == ' ' ) hy--;
            }
            if ( GetAsyncKeyState( VK_DOWN ) ){
                if( gameMap[hy+1][hx] == '.' ){ gameMap[hy+1][hx] = ' '; hy++; pts++;}
                else if( gameMap[hy+1][hx] == ' ' ) hy++;
            }
            if ( GetAsyncKeyState( VK_LEFT ) ){
                if( gameMap[hy][hx-1] == '.' ){ gameMap[hy][hx-1] = ' '; hx--; pts++;}
                else if( gameMap[hy][hx-1] == ' ' ) hx--;
            }
            if ( GetAsyncKeyState( VK_RIGHT ) ){
                if( gameMap[hy][hx+1] == '.' ){ gameMap[hy][hx+1] = ' '; hx++; pts++;}
                else if( gameMap[hy][hx+1] == ' ' ) hx++;
            }

            if( old_hx != hx || old_hy != hy ){//if user moved(old position not equals to new position
                FindPath( ex,ey,hx,hy );// then reset findpath)
            }
            gotoxy( hx,hy ); cout << "H";
            //End of Updating the location of Hero

            //start updating enemy position and placing . on that position
            gameMap[ey][ex] = '.';//let original map has a dot there, then hero can eat it even if the map is refreshed (tmp_map)
            gotoxy( ex, ey ); cout << ".";//put cursor there and display that dot

            if( frame==speedmod && walk_queue.size() != 0 ){
                frame=0;
                ex = walk_queue.back().x;
                ey = walk_queue.back().y;
                walk_queue.pop_back();
            }
            gotoxy( ex, ey ); cout << "E";

            if( ex == hx && ey == hy ){//if enemy reaches the hero restart or quit
                char decision;
                system("cls");
                cout<<"You Lose and your score is: "<<pts<<endl;
                cout<<"continue play?\n type 'q' to quit\n type other to continue"<<endl;
                cin>>decision;
                //got the decision character, then either restart game or quit game
                if(decision == 'q' || decision == 'Q') {//if choose to quit
                    thisRound=false;//start from the beginning of main function
                    play = false; //exit!
                }
                else { //otherwise if user still want to play, jump out from 'this round' and reset map
                    thisRound=false;//start from beginning
                    char newMap[18][32] = {
                        "+#############################+",
                        "|                             |",
                        "|                             |",
                        "|## ########### ##   #########|",
                        "|   |                         |",
                        "| | |### |  |           |     |",
                        "| |      |  | |###  |   |  |  |",
                        "| | #####|  | |      ## |     |",
                        "| |           |###  |      |  |",
                        "| |##### ###         ##       |",
                        "|          ######  ####### ###|",
                        "|                             |",
                        "|# ### ####      ###   #######|",
                        "|                             |",
                        "|      ###################    |",
                        "|                             |",
                        "|                             |",
                        "+#############################+"
                        }; //reset map, original map may have '.' there
                    memcpy( tmp_map, newMap, sizeof(newMap) ); //reset gameMap
                    memcpy( gameMap, newMap, sizeof(newMap) ); //reset tmp_map
                }
            }
        } //while thisRound
    } //while play

	return 0;
}
