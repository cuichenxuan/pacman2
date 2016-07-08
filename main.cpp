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
    "|## ########### ##   ###### ##|",
    "|                             |",
    "| |#######  |           |     |",
    "| |         | |###      |  |  |",
    "|   ######  |        ## |     |",
    "| |         | |###         |  |",
    "| |##### ###         ##       |",
    "|            ####  ####### ###|",
    "|                             |",
    "|# ### ####      ###   #### ##|",
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

vector<targetPath> walk_queue1;
vector<targetPath> walk_queue2;

vector<walk> BFSArray1;//walk all map for enemy 1
vector<walk> BFSArray2; // walk all map for enemy 2

void fillTheMapEnemy1( int x, int y, int back ){
	if( tmp_map[y][x] == ' ' || tmp_map[y][x] == '.' ){
		tmp_map[y][x] = '#'; //set the position to # so that next round of path search it wont go back.
		walk tmp;
		tmp.x = x;
		tmp.y = y;
		tmp.back = back;
		BFSArray1.push_back( tmp ); //push back each every possible step
	}
}

void FindPathEnemy1( int e1x, int e1y, int hx, int hy ){//everytime calling FindPath, the tmp_map is reset to map
	memcpy( tmp_map, gameMap, sizeof(gameMap) );//copy map to another temporary 2d array called tmp_map
	BFSArray1.clear();
	walk currentEnemyPos;//store initial position of enemy
	currentEnemyPos.x = e1x;
	currentEnemyPos.y = e1y;
	currentEnemyPos.back = -1;  //the initial position of enemy
	BFSArray1.push_back( currentEnemyPos );

	for(int i=0; i<BFSArray1.size(); i++){//for loop that fill the map until we fint the hero
		if( BFSArray1[i].x == hx && BFSArray1[i].y == hy ){ //while filling the map, if we found the target(Hero)
			walk_queue1.clear();
			targetPath step;
			while( BFSArray1[i].back != -1 ){//before reaching the initial position of enemy itself
				step.x = BFSArray1[i].x;
				step.y = BFSArray1[i].y;
				walk_queue1.push_back( step );
//importan! this lead the path from hero to enemy. back stores the position of previous step position in BFSArray1.
				i = BFSArray1[i].back;
			}

			break;
		}

		fillTheMapEnemy1( BFSArray1[i].x+1, BFSArray1[i].y, i );
		fillTheMapEnemy1( BFSArray1[i].x-1, BFSArray1[i].y, i );
		fillTheMapEnemy1( BFSArray1[i].x, BFSArray1[i].y+1, i );
		fillTheMapEnemy1( BFSArray1[i].x, BFSArray1[i].y-1, i );
	}

	BFSArray1.clear();
}

void fillTheMapEnemy2( int x, int y, int back ){
	if( tmp_map[y][x] == ' ' || tmp_map[y][x] == '.' ){
		tmp_map[y][x] = '#'; //set the position to # so that next round of path search it wont go back.
		walk tmp;
		tmp.x = x;
		tmp.y = y;
		tmp.back = back;
		BFSArray2.push_back( tmp ); //push back each every possible step
	}
}

void FindPathEnemy2( int e2x, int e2y, int hx, int hy ){//everytime calling FindPath, the tmp_map is reset to map
	memcpy( tmp_map, gameMap, sizeof(gameMap) );//copy map to another temporary 2d array called tmp_map
	BFSArray2.clear();
	walk currentEnemyPos;//store initial position of enemy
	currentEnemyPos.x = e2x;
	currentEnemyPos.y = e2y;
	currentEnemyPos.back = -1;  //the initial position of enemy
	BFSArray2.push_back( currentEnemyPos );

	for(int i=0; i<BFSArray2.size(); i++){//for loop that fill the map until we fint the hero
		if( BFSArray2[i].x == hx && BFSArray2[i].y == hy ){ //while filling the map, if we found the target(Hero)
			walk_queue2.clear();
			targetPath step;
			while( BFSArray2[i].back != -1 ){//before reaching the initial position of enemy itself
				step.x = BFSArray2[i].x;
				step.y = BFSArray2[i].y;
				walk_queue2.push_back( step );
//importan! this lead the path from hero to enemy. back stores the position of previous step position in BFSArray1.
				i = BFSArray2[i].back;
			}

			break;
		}

		fillTheMapEnemy2( BFSArray2[i].x+1, BFSArray2[i].y, i );
		fillTheMapEnemy2( BFSArray2[i].x-1, BFSArray2[i].y, i );
		fillTheMapEnemy2( BFSArray2[i].x, BFSArray2[i].y+1, i );
		fillTheMapEnemy2( BFSArray2[i].x, BFSArray2[i].y-1, i );
	}

	BFSArray1.clear();
}

int main() {
    while(play) { //if later player lost and chose to quit (q) then play set to false.
        int hx = 15; // initialize hero x
        int hy = 16; // initialize hero y

        int e1x = 1; // initialize first enemy x
        int e1y = 1; // initialize first enemy y
        int e2x = 29; // initialize second enemy x
        int e2y = 1; // initialize second enemy y

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

        FindPathEnemy1( e1x,e1y,hx,hy );//e1x e1y: enemy 1 position, x y: hero position
        FindPathEnemy2( e2x,e2y,hx,hy ); //find the path for second enemy to reach hero
        bool thisRound=true;//start the new round. if later player is dead, then thisRound reset to false

        while(thisRound){
            gotoxy( 32, 1 ); cout << pts;//display score
            Sleep( 90 ); //delay
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
                FindPathEnemy1( e1x,e1y,hx,hy );// then reset findpath)
                FindPathEnemy2( e2x,e2y,hx,hy );// refind path for second enemy
            }
            gotoxy( hx,hy ); cout << "H";
            //End of Updating the location of Hero

            //start updating enemies' positions and placing . on that position
            gameMap[e1y][e1x] = '.';//let original map has a dot there, then hero can eat it even if the map is refreshed (tmp_map)
            gotoxy( e1x, e1y ); cout << ".";//put cursor there and display that dot
            gameMap[e2y][e2x] = '.';//let original map has a dot there, then hero can eat it even if the map is refreshed (tmp_map)
            gotoxy( e2x, e2y ); cout << ".";//put cursor there and display that dot

            if( frame==speedmod && walk_queue1.size() != 0 ){
                frame=0; //reset frame, so it wont overflow
                e1x = walk_queue1.back().x;
                e1y = walk_queue1.back().y;
                walk_queue1.pop_back();
                int e2x_tmp=e2x; int e2y_tmp=e2y; //temporarily save the second enemy's location
                e2x = walk_queue2.back().x;//update the second enemy
                e2y = walk_queue2.back().y;
                if(e1x==e2x&&e1y==e2y) {//if the updated second enemy's location is same as the first enemy's location
                    e2x = e2x_tmp;//then the second enemy stop moving for one step if order to prevent colliding with the
                    e2y = e2y_tmp;//first enemy
                }
                else {
                    walk_queue2.pop_back();
                }
            }
            gotoxy( e1x, e1y ); cout << "E";
            gotoxy( e2x, e2y ); cout << "E";
            if((e1x == hx && e1y == hy) || (e2x == hx && e2y == hy) ){//if either enemy reaches the hero restart or quit
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
                        "|## ########### ##   ###### ##|",
                        "|                             |",
                        "| |#######  |           |     |",
                        "| |         | |###      |  |  |",
                        "|   ######  |        ## |     |",
                        "| |         | |###         |  |",
                        "| |##### ###         ##       |",
                        "|            ####  ####### ###|",
                        "|                             |",
                        "|# ### ####      ###   #### ##|",
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
