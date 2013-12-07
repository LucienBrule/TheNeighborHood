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

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>

enum buildings{house = 1, buisness = 2, store = 3, tree = 4};
enum sizes{small = 3,medium = 4,large = 8,DAMN = 16};
enum moveDirections{down = 1, up = 2, left = 3, right = 4};

typedef struct{
	int xpos;
	int ypos;
	int money;
	int health;
	int isNinja;
}Player;

typedef struct {
	int size;
	int streetspacing;// street spacing from origin
	char** map;
	char playerTile;
	char carTile;
}World;

Player player = {0,0,100,100,0};
World myWorld = {0,3};


void genWorld(int size,World* myWorld){

	//allocate (size) spaces for 2d array (map), of chars (Day 1)
	printf("Mallocing");
	myWorld->map = malloc(sizeof(char*) * size);
	int i;
	for(i = 0; i < size; i++)
		myWorld->map[i]= (char*) malloc(size * sizeof(char));

	//Gen Roads
	for(i = 0;i < size-1; i++){
		int j;
		for(j = 0; j < size-1; j++){
			if(((i + 1) % myWorld->streetspacing == 0) && ((j + 1) % myWorld->streetspacing == 0)){
				myWorld->map[i][j] = '+';//intersections
			}
			else if(((j + 1) % myWorld->streetspacing == 0)){
				myWorld->map[i][j] = '-';// y = n roads
			}
			else if((i + 1) % myWorld->streetspacing == 0){
				myWorld->map[i][j] = '|';// x = n roads
			}else//Trees as place holders between roads
				myWorld->map[i][j] = 'x';
		}
	myWorld->size = size;
	}

	//Gen Buildings
	srand(time(NULL)); //seed the earth
	for(i = 0; i < size- 1; i++){
		int j;
		for(j = 0; j < size -1; j++){
			if(myWorld->map[i][j] == 'x'){
				int randy =  rand()%(4) + 1;
				switch(randy){
				case house:
					myWorld->map[i][j] = 'H';
					break;
				case buisness:
					myWorld->map[i][j] = 'B';
					break;
				case store:
					myWorld->map[i][j] = 'S';
					break;
				case tree:
					myWorld->map[i][j] = 'X';
					break;
				default:
					puts("Dun goofed, placing a tree");
					myWorld->map[i][j] = 'X';
					break;
				}
			}
		}
	}
}
int clearScreen(){
	int i =printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	if(i > 0){
		return 1;
	}else
		return -1;
}
void splash(){
	printf("This will be the splash animation\n");
	sleep(0);
}
int putPlayer(int newX, int newY){
	myWorld.playerTile = myWorld.map[newX][newY];
	myWorld.map[newX][newY] = 'o';
	player.xpos = newX;
	player.ypos = newY;
	return 0;
}

int spawnPlayer(int size){ //returns -1 if can't spawn, 0 if spawned homeless(on a street), or 1 if spawned in a house.. To be called succesive times until return > -1
	int ranX =  rand()%(size - 0 + 1) + 1;// high - low + 1
	int ranY =  rand()%(size - 0 + 1) + 1;//ditto

	if(myWorld.map[ranX][ranY] == 'H'){
		putPlayer(ranX,ranY);
		return 1;
	}
	else if((myWorld.map[ranX][ranY] == '+') || (myWorld.map[ranX][ranY] == '|') || (myWorld.map[ranX][ranY] == '-')){
		putPlayer(ranX,ranY);
		return 0;
	}
	else{
		return -1;
	}
	return -1;
}


void drawFrame(){
	clearScreen();
	int length = myWorld.size - 1;
	int i;
	for(i = 0; i < length; i ++){
		int j;
		for(j = 0; j < length; j++){
			printf("%c",myWorld.map[i][j]);
		}
		printf("\n");
	}
	printf("$: %d \t H: %d", player.money, player.money);


}
int addMoney(int amt){
	player.money += amt;
	printf("%d moni get\n",amt);
	return 0;
}
int subtractMoney(int amt){
	player.money -= amt;
	printf("%d moni lost\n",amt);
	return 0;
}
int interactBuilding(int buildingType){
	clearScreen();
	switch(buildingType){
	case house:
		printf("<ascii hosue>\n");
		printf("You enter the house, there seems to be people here, but you snoop around stuff anyway");
		if((rand()% 100) <50){
			printf("After going through their things, you find %d monies",(rand() % 100 + 2));
		}
		break;
	case buisness:
		printf("This is a business");
		break;
	case store:
		printf("This is a store");
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
			printf("They scurry about, but there is no treasure to be found here");
		}
		player.isNinja++;
		break;
	default:
		return -1;
		break;
	}
	return 0;
}
int movePlayer(int newX, int newY){// returns -1 if couldnt move to pos, 0 if moved.

	switch (myWorld.map[newX][newY]){
	case 'H':
		return interactBuilding(house);
		break;
	case 'B':
		return interactBuilding(buisness);
		break;
	case 'S':
		return interactBuilding(store);
		break;
	case 'X':
		return interactBuilding(tree);
		break;
	case '|':
	case '-':
	case '+':
		return putPlayer(newX,newY);
		break;
	default:
		return -1;
		break;
	}
	return 0;
}
int initialize(){
	printf("How big is the hood?");
		int input = -1;
		char uin;
		while(input < 0){
			printf("[1 Small][2 Medium][3 Large][4 DAMN!]");
			scanf("%i",&input);
		}
		printf(" You chose %u\n", input);
		genWorld(input, &myWorld);
		drawFrame();
		int i = 0;// iterator
		int r = -2; // return check
		while(r < 0){
			r = spawnPlayer(input);// successive call to spawnPlayer
			i++;// increment i
			if(i > 5){ //needed incase genworld produces a bad world
				genWorld(input,&myWorld);
			}
			//defaultWorld() would go here
		}
		printf("%i",r);
		if(r == 1){
			printf("Congrats homie, you're neet");
			addMoney(100);
		}
		else if( r == 0){
			printf("Its hard out here on these streets! Keep it g");
			player.isNinja += 4;
			subtractMoney(50);
		}

		printf("Prepare your body for the new world...");
		return 0;
}
void printHelp(){
	printf("This is the help page");
}

void debugMenu(){

}

int main(){
	splash();
	initialize();
	sleep(1);
	char input = 'm';
	int turns;
	//Engine

	while(input != 'q'){
		//Draw the map and HUD
		drawFrame();
		//Saftey loop to ensure good value
		while((input !='q')||(input !='h')||(input !='w')||(input !='a')||(input !='s')||(input !='d')||(input !='~')){
			scanf("%c",&input);
		}

		switch(input){
		case 'q':
			printf("Are you sure you want to quit? y/n");
			scanf("%c",&input);
			if(input == 'y'){
				printf("Peace homie");
			}
			break;
		case 'h':
			printHelp();
			break;
		case 'w':
			movePlayer(player.xpos, player.ypos + 1);
			break;
		case 'a':
			movePlayer(player.xpos -1, player.ypos);
			break;
		case 's':
			movePlayer(player.xpos , player.ypos -1);
			break;
		case 'd':
			movePlayer(player.xpos +1, player.ypos);
			break;
		case '~':

			break;
		default:
			printf("DUN GOOFED!");
		}
		turns++;
		printf("turn %i complete", turns);
		printf("updated map:");
		drawFrame();
	}
	return 0;
}
