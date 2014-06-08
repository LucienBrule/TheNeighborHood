/**
 * @Author: Lucien Brule
 * @Date: November 28th 2013
 * @Desc: "A simple text based game"
 *
 *     This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */



#ifdef __unix__
# include <stdio.h>
# include <stdlib.h>
# include <time.h>
# include <math.h>
# include <unistd.h>
#define clear(x) printf("\033[2J");

#elif defined(_WIN32) || defined(WIN32)
# include <windows.h>
# include <stdio.h>
#include <stdlib.h>
#define clear(x) system("cls");
#define sleep(x) Sleep(1000* x)
#endif

int quit = 0;
int turns = 1;
enum buildings{house = 1, buisness = 2, store = 3, tree = 4};
// enum sizes{small = 3,medium = 4,large = 8,DAMN = 16};
enum moveDirections{down = 1, up = 2, left = 3, right = 4};
//char map[10][10] = {{'|','H','X','X','X','H','X','|','X','X'},
//					{'|','H','H','|','H','H','X','|','H','X'},
//					{'|','H','H','|','H','H','X','|','B','X'},
//					{'|','H','H','|','X','H','X','|','H','B'},
//					{'|','-','-','-','-','-','-','+','-','-'},
//					{'|','X','H','|','H','X','X','|','B','S'},
//					{'|','B','H','-','+','H','H','|','B','S'},
//					{'|','B','X','H','|','X','H','|','-','-'},
//					{'|','S','X','H','-','|','H','|','H','H'},
//					{'|','B','X','X','H','B','H','|','H','X'},
//};
char **map;


typedef struct{
	int xpos;
	int ypos;
	int money;
	int health;
	int isNinja;
	char name[10];
}Player;

typedef struct {
	int size;
	int streetspacing;// street spacing from origin
	char playerTile;
//	char carTile;
}World;

Player player = {0,0,100,100,0,0};
World myWorld = {10,3,'o'};

void genWorld(int size,World* myWorld){
//
//	//allocate (size) spaces for 2d array (map), of chars (Day 1)
//	printf("Mallocing");
	map = malloc (sizeof(char*) * size);
	int i;
	for(i = 0; i < size; i++){
		map[i]= malloc(sizeof(char*) * size);
	}
	//Gen Roads
	for(i = 0;i < size; i++){
		int j;
		for(j = 0; j < size; j++){
			if(((i + 1) % myWorld->streetspacing == 0) && ((j + 1) % myWorld->streetspacing == 0)){
				map[i][j] = '+';//intersections
			}
			else if(((j + 1) % myWorld->streetspacing == 0)){
				map[i][j] = '|';// y = n roads
			}
			else if((i + 1) % myWorld->streetspacing == 0){
				map[i][j] = '-';// x = n roads
			}else//Trees as place holders between roads
				map[i][j] = 'x';
		}
	myWorld->size = size;
	}
	//Gen Buildings
	srand(time(NULL)); //seed the earth
	for(i = 0; i < size- 1; i++){
		int j;
		for(j = 0; j < size -1; j++){
			if(map[i][j] == 'x'){
				int randy =  rand()%(4) + 1;
				switch(randy){
				case house:
					map[i][j] = 'H';
					break;
				case buisness:
					map[i][j] = 'B';
					break;
				case store:
					map[i][j] = 'S';
					break;
				case tree:
					map[i][j] = 'X';
					break;
				default:
					puts("Dun goofed, placing a tree");
					map[i][j] = 'X';
					break;
				}
			}
		}
	}
}
//void genWorld(){
//	myWorld.map = {
//			{'X','X','|','X','X'}
//			{'B','B','|','X','X'}
//			{'-','-','+','-','-'}
//			{'S','H','|','H','H'}
//			{'X','H','|','X','X'}
////
//	};
//}
int wait(){
	printf("[Enter] to continue...");
	getchar();
	return 0;
}

int clearScreen(){
////	int i =printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
//
//	if(i > 0){
//		return 1;
//	}else
//		return -1;
	clear();
	return 1;
}
void rollEffect(char str[],int length, int iters){ // works on occasion
	int i;
	int j;
	printf("\n");
	for(j = 1; j <= iters; j ++){
		for(i = 0; i < length; i++){
			printf(" %c", str[i]);
//			sleep(.25);
		}
	}
	printf("\n");
}
void printHelp(){
	clearScreen();
	printf("\nHELP_________\n");
	printf("Movement: W A S D\n");
	printf("OBJECTIVE: roam the neighbor hood, there are many \'features\' \n'");
	wait();
}
void splash(){
//	printf(name);
	rollEffect("Welcome to the Neighborhood",0,1);
	printf("\t     Written by Lucien Brulé\n");
	rollEffect("'`~,-`^",0,1);
	printf("Type [h] for HELP and COMMANDS,or [Enter] to continue");
	if(getchar() == 'h'){
		printHelp();
	}
	clearScreen();
}
int putPlayer(int newX, int newY){
	myWorld.playerTile = map[newX][newY];
	map[newX][newY] = 'o';
	map[player.xpos][player.ypos] = myWorld.playerTile;
	player.xpos = newX;
	player.ypos = newY;
	return 0;
}

//int spawnPlayer(int size){ //returns -1 if can't spawn, 0 if spawned homeless(on a street), or 1 if spawned in a house.. To be called succesive times until return > -1
//	int ranX =  rand()%(size - 0 + 1) + 1;// high - low + 1
//	int ranY =  rand()%(size - 0 + 1) + 1;//ditto
//
//	if(map[ranX][ranY] == 'H'){
//		putPlayer(ranX,ranY);
//		return 1;
//	}
//	else if((map[ranX][ranY] == '+') || (map[ranX][ranY] == '|') || (map[ranX][ranY] == '-')){
//		putPlayer(ranX,ranY);
//		return 0;
//	}
//	else{
//		return -1;
//	}
//	return -1;
//}

int spawnPlayer(){
	srand(time(NULL));
	if((rand() % 10) < 5){
		putPlayer(0,3);
		return 1;

	}

	else{
		putPlayer(0,1);
		return 0;
	}
	return -1;
}

void drawFrame(){
	clearScreen();
	int length = myWorld.size;
	int bar;
	printf("\t\t The Neighbor Hood\n");
	printf("\t");
	for(bar = length*3; bar > 0; bar --){
		printf("#");
	}
	printf("\n\t");
	int i;
	for(i = 0; i < length; i ++){
		int j;
		printf("#");
		for(j = 0; j < length; j++){
			printf("%c ",map[i][j]);
			if(j == length -1){
				printf("#");
			}
			if(i == 0 && j == length -1)
				printf("turn %i", turns);
		}
		printf("\n\t");
	}
	for(bar = length*3; bar > 0; bar --){
			printf("#");
		}
	printf("\n\t%s $: %d     H: %d\n",player.name, player.money, player.health);
	printf("//>");
}
int addMoney(int amt){
	player.money += amt;
	printf("< %d points added>\n",amt);
	return amt;
}
int subtractMoney(int amt){
	player.money -= amt;
	printf("< %d points lost>\n",amt);
	return 0;
}
int interactBuilding(int buildingType){
	switch(buildingType){
	case house:
		printf("<ascii house>\n");
		printf("You enter the house, there seems to be people here, but you snoop around stuff anyway\n");
		if((rand()% 100) <50){
			printf("After going through their things, you find %d monies\n",addMoney((rand() % 100 + 2)));
		}
		break;
	case buisness:
		printf("This is a business\n");
		break;
	case store:
		printf("This is a store\n");

		break;
	case tree:
		printf("<ascii tree>\n");
		printf("You approach the tree, you look up and see some sneaky squirrels");
		if(player.isNinja % 3 == 0){
			printf("Since you are a ninja, the squirrels look up to you and present you with\n 10 coins");
			addMoney(4);
			printf("You think to yourself:Too bad squirrels can't count.");

		}
		else{
			printf("They scurry about, but there is no treasure to be found here\n");
		}
		player.isNinja++;
		break;
	default:
		return -1;
		break;
	}
	printf("\n");
	wait();
	clearScreen();
	return 0;
}
void movePlayer(int newX, int newY){// returns -1 if couldnt move to pos, 0 if moved.

	switch (map[newX][newY]){
	case 'H':
		 interactBuilding(house);
		break;
	case 'B':
		 interactBuilding(buisness);
		break;
	case 'S':
		 interactBuilding(store);
		break;
	case 'X':
		 interactBuilding(tree);
		break;
	case '|':
	case '-':
	case '+':
		 putPlayer(newX,newY);
		break;
	}
}
int initialize(){
	printf("How big is the hood?");
		int input = -1;
//		char uin;
		printf("[1 Small][2 Medium][3 Large][4 DAMN!]");
		scanf("%i",&input);
		printf(" You chose %u\n", input);
		genWorld(input, &myWorld);
		drawFrame();
//		int i = 0;// iterator
		int r = -2; // return check
//		while(r < 0){
//			r = spawnPlayer(input);// successive call to spawnPlayer
//			i++;// increment i
//			if(i > 5){ //needed incase genworld produces a bad world
//				genWorld(input,&myWorld);
//			}
//			//defaultWorld() would go here
//		}
		r = spawnPlayer();
//		printf("%i",r);
		printf("What is your name?");
		scanf("%s",player.name);
		printf("As the sun rises on the Neighborhood, you wake up and realize...\n\t");
		if(r == 1){
			printf("Life's awesome, you're neet(You spawned in a house ^-^)\n");
			addMoney(100);
		}
		else if( r == 0){
			printf("Its hard out here on these streets!\n Today you gotta keep it g. (You spawned on the STREETS)\n");
			player.isNinja += 4;
			subtractMoney(50);
		}else{
			printf("DUN GOOFED on lifestyle\n");
		}

		printf("Prepare your body for the new world...\n");
		int i = 0;
		for(i = 0; i < 5; i++){
			printf(".");
		}
		wait();
		drawFrame();
		return 0;
}

void debugMenu(){
	printf("This is the debug page");
	printf("\nThe Neighborhood  Copyright (C) 2013  Lucien Brule\nThis program comes with ABSOLUTELY NO WARRANTY;\n This is free software, and you are welcome to redistribute it\n under certain conditions; type `show c' for details.");
	sleep(1);
}

int main(){
	splash();
	initialize();
	char input = 'm'; // an unused key
//	sleep(1);
	//Engine
	while(quit != 1){
		//Draw the map and HUD
		//Saftey loop to ensure good value

//		while((input !='q')||(input !='h')||(input !='w')||(input !='a')||(input !='s')||(input !='d')||(input !='~')){
			input = getchar();
			getchar();
//			printf("You chose %c\n",input);
//		}
			char sinput;
		switch(input){
		case 'q':
			printf("Are you sure you want to quit? y/n");
			sinput = getchar();
			getchar();
			if(sinput == 'y'){
				printf("Peace homie");
				sleep(2);
				quit = 1;
			}
			break;
		case 'h':
			printHelp();
			break;
		case 'd':
			movePlayer(player.xpos ,player.ypos +1);
			break;
		case 'w':
			movePlayer((player.xpos -1), player.ypos);
			break;
		case 'a':
			movePlayer(player.xpos ,(player.ypos -1));
			break;
		case 's':
			movePlayer((player.xpos +1), player.ypos);
			break;
		case '~':

			break;
		default:
			printf("%c is an Invaild key press", input);
			turns--;
		}
		turns++;
		drawFrame();
		input = 'm';
	}
	return 0;
}
