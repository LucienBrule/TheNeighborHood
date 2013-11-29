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

char** map;	//playing field

enum sizes{small = 3,medium = 4,large = 8,DAMN = 16};
int streetspacing = 3;// street spacing from origin
enum buildings{house = 1, buisness = 2, store = 3, tree = 4};

struct player{
	int xpos;
	int ypos;
	int money;
	int health;
};


void splash(){
	printf("This will be the splash animation");
}
void genMap(int size){

	//allocate (size) spaces for 2d array (map), of chars (Day 1)
	map = (char**)malloc(sizeof(char*) * size);
	int i;
	for(i = 0; i < size; i++)
		map[i]= (char*) malloc(size * sizeof(char));

	//Gen Roads
	for(i = 0;i < size-1; i++){
		int j;
		for(j = 0; j < size-1; j++){
			if(((i + 1) % streetspacing == 0) && ((j + 1) % streetspacing == 0)){
				map[i][j] = '+';//intersections
			}
			else if(((j + 1) % streetspacing == 0)){
				map[i][j] = '-';// y = n roads
			}
			else if((i + 1) % streetspacing == 0){
				map[i][j] = '|';// x = n roads
			}else//Trees as place holders between roads
				map[i][j] = 'x';
		}
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

int spawnPlayer(int size){ //returns -1 if can't spawn, 0 if spawned homeless(on a street), or 1 if spawned in a house
	while(1 == 1){
		int ranX =  rand()%(size - 0 + 1) + 1;// high - low + 1
		int ranY =  rand()%(size - 0 + 1) + 1;//ditto

		if(map[ranX][ranY] == 'H'){
			map[ranX][ranY] = 'o';
		}
		else if((map[ranX][ranY] == '+') || (map[ranX][ranY] == '|') || (map[ranX][ranY] == '-')){
			map[ranX][ranY] = 'o';
		}

	}
}


int main(){
	splash();

	return 0;
}
