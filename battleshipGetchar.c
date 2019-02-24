#include <stdio.h>
#include <string.h>

#define y_max 10	// constants for the max sizes for board
#define x_max 10	//notice its going to be FOR A INT ARRAY it is exactly correct size 10x10 grid array
/*
prototypes of functinos are here at the beginning

we also required to have struct point return type for askCoords()
*/

//in linux this versino is known as DELTAVERSION
struct point{
	int ycoord;
	int xcoord;
};

struct shipDescriptions{
	int hitpoints;
	int shipsize;
	int ship_id;	// id can be different for different ships... because each aircraft carrier is different  object like USS Enterprise USS Yorktown etc...
	char name[21];
}Player1Fleet[7];

char askHorizVert();	//asks user if ship is horizontal / vert
struct point askCoords();	// asks user what is the deployment point of ship
int checkIfShipOnBoard(int shipsize, char horizvert, struct point deploymentpoint);	//determines if the ship is on board or off the board 
int checkIfShipTouchesShip(int shipsize, char horizvert, struct point deploymentpoint, int board[][10]);	//determines if ship overlaps ship
void deployShip(int shipsize, int shipIdentifier, char horizvert, struct point deploymentpoint, int board[][10]);	//actually deploys the ship and thus saves the  data into the gameboard
void printShips(int ownboard[][10]);	//prints the updated status of  board
void populateEmptyBoard(int board[][10]);	// just creates empty sea in the board into the array
int deployAllShipsOfSameType( int numberOfShips, struct shipDescriptions blueprint, int board[][10]);
int advancedCheckShipDeployment(int shipsize, char horizvert, struct point deploymentpoint, int board[][10]);
int switchTurnSafely(int whoseTurnEndedPreviously);	//switches turns safely so that the player1 doesnt see player2 and vice-versa

/*Here are some safe functions for taking user inputts for shooting, deployment, and asking ship direction in deployment*/
struct point safeAskCoords();
char safeAskHorizVert();
struct point safeAskShootingPoint();
//

//testing getchar askCoords
struct point getcharAskCoords();

struct point getcharAskShootingPoint();
//
//print rules function
void printGameRules();
//

// print functions combatModePrintShips() and printIntegerAsChar() are used in combat-mode print-outs
char printIntegerAsChar(int shipIdentifier);		//juts converts shipIds on gameboard from int--> to char for print purposes
void combatModePrintShips(int enemyBoard[][10]);// enemy fleet is coverred by the so-called 'fog-of-war' so you cant see it unless you 'recon- by -fire'
//

/*Here are some shooting related functions*/
int executeShootingProcedure(int targetBoardBeingShot[][10],  struct shipDescriptions targetFleetBeingShot[7]);//kesken vielä /// should work just fine now!
struct point askShootingPoint();
int hitOrMissAndUpdateShots(struct point shootingpoint, int targetBoardBeingShot[][10], struct shipDescriptions targetFleet[7]);	//returns true or false, either you hit ship, or you hit ocean
int computeRemainingHitPoints(struct shipDescriptions targetFleet[7], int targetBoard[][10]); //return value (keep game going)OR (stop game) -->implies-->the guys who was shooting won the game
/*NOTE because gameboards are 2D int type arrays, the hitmarker(hitship) and missmarker(hitocean) have to be different integer from (0, 1, 2, 3, 4, 5, 6, 7)
let hitmarker be equal to 8 on gameboard
let missmarker be equal to 9 on gameboard
*/


//int updateShootingResultsToBoard();		//updates hit ship, or hit ocean into the board targetedboard./// NOT NEEDED maybe
//void osuiJaUpposi(struct shipDescriptions targetFleet, struct point shootingpoint  , int targetBoard[][10], int viableShootingResults);// probably some old functions, probably not used anywhere


/*THid is the main program starting point
general structure of the main program is as follows:

1. creating and defining values for structs such as shipDescriptions and deploymentpoint and hittingpoint (where the shooter shoots)
2.create and populate player1 and player2 gameboards with empty sea (empty sea is valued at int 0)
3. Playeyr1 deploys his/her fleet entirely	(ships are valued at values from the set of {1,2,3,4,5,6,7} )
4.Player 2 deploys his/her fleet entirely

maingame begins
5.  player1 starts shooting at player2
6. check shooting results
7.check if game ends or continues
8. player2 shoots at player1 
9.check shooting results
10. check if game ends or continues
11. IF keep going ->go back to 5.
maingame ends?!?!
*/

int main(void){

	struct point deploymentpoint;// initialize some start values for deployment point, later we ask the user for values,though
	deploymentpoint.ycoord=0;
	deploymentpoint.xcoord=0;
	struct point hittingpoint;
	hittingpoint.ycoord=0;
	hittingpoint.xcoord=0;

	char horizontalVertical = 'h';	//variable to check how user wants to orientate ship at deployment
	int legallyDeployableShip=0;	//int variables to act as booleans
	int shipWasInsideGameboard=0;
	int turncounter=0;	// keeps check of turns, 0==player1turn and  1==player2turn
	int keepGameGoing=1; //acts like boolean in maingame loop when both p1 and p2 are shooting at each other(i.e. after both had deployed)

	int finishCVdeploy=0, finishBBdeploy=0, finishCAdeploy=0, finishDDdeploy=0, finishSSdeploy=0;	//un needed variables left from  earlier version

	int ownPlayerBoard[y_max][x_max];	//create playerboard with null data
	int enemyBoard[y_max][x_max];		// create enemyboard with null data
	populateEmptyBoard(ownPlayerBoard);	// puts empty sea into the board squares integer 0
	populateEmptyBoard(enemyBoard);	// puts empty sea into the board squares integer 0

	struct shipDescriptions Player2Fleet[7];

	// player2fleet data is below
	strcpy(Player2Fleet[0].name, "Aircraft Carrier");
	Player2Fleet[0].ship_id= 7;
	Player2Fleet[0].shipsize=5;
	Player2Fleet[0].hitpoints=5;

	strcpy(Player2Fleet[1].name, "Battleship");
	Player2Fleet[1].shipsize=4;
	Player2Fleet[1].ship_id=6;
	Player2Fleet[1].hitpoints=4;

	strcpy(Player2Fleet[2].name, "Cruiser1");
	Player2Fleet[2].shipsize=3;
	Player2Fleet[2].ship_id=4;
	Player2Fleet[2].hitpoints=3;

	strcpy(Player2Fleet[3].name, "Cruiser2");
	Player2Fleet[3].ship_id=5;
	Player2Fleet[3].shipsize=3;
	Player2Fleet[3].hitpoints=3;

	strcpy(Player2Fleet[4].name, "Destroyer1");
	Player2Fleet[4].shipsize=2;
	Player2Fleet[4].ship_id=2;
	Player2Fleet[4].hitpoints=2;

	strcpy(Player2Fleet[5].name, "Destroyer2");
	Player2Fleet[5].ship_id=3;
	Player2Fleet[5].shipsize=2;
	Player2Fleet[5].hitpoints=2;

	strcpy(Player2Fleet[6].name, "Submarine");
	Player2Fleet[6].ship_id=1;
	Player2Fleet[6].shipsize=1;
	Player2Fleet[6].hitpoints=1;
	

	//player1 fleet data
	strcpy(Player1Fleet[0].name, "Aircraft Carrier");
	Player1Fleet[0].ship_id= 7;
	Player1Fleet[0].shipsize=5;
	Player1Fleet[0].hitpoints=5;

	strcpy(Player1Fleet[1].name, "Battleship");
	Player1Fleet[1].shipsize=4;
	Player1Fleet[1].ship_id=6;
	Player1Fleet[1].hitpoints=4;

	strcpy(Player1Fleet[2].name, "Cruiser1");
	Player1Fleet[2].shipsize=3;
	Player1Fleet[2].ship_id=4;
	Player1Fleet[2].hitpoints=3;

	strcpy(Player1Fleet[3].name, "Cruiser2");
	Player1Fleet[3].ship_id=5;
	Player1Fleet[3].shipsize=3;
	Player1Fleet[3].hitpoints=3;

	strcpy(Player1Fleet[4].name, "Destroyer1");
	Player1Fleet[4].shipsize=2;
	Player1Fleet[4].ship_id=2;
	Player1Fleet[4].hitpoints=2;

	strcpy(Player1Fleet[5].name, "Destroyer2");
	Player1Fleet[5].ship_id=3;
	Player1Fleet[5].shipsize=2;
	Player1Fleet[5].hitpoints=2;

	strcpy(Player1Fleet[6].name, "Submarine");
	Player1Fleet[6].ship_id=1;
	Player1Fleet[6].shipsize=1;
	Player1Fleet[6].hitpoints=1;

	printGameRules();// prints gamerules to players can see some information abotu what to do...

/*
Player1 deploys his entire fleet now..
*/
	printf("Player1 deploys entire fleet now...\n");
	deployAllShipsOfSameType(1, Player1Fleet[0], ownPlayerBoard);	//1xCV
	deployAllShipsOfSameType(1, Player1Fleet[1], ownPlayerBoard);	//1xBB
	deployAllShipsOfSameType(1, Player1Fleet[2], ownPlayerBoard); // 1x CA1
	deployAllShipsOfSameType(1, Player1Fleet[3], ownPlayerBoard);	// 1x CA2
	deployAllShipsOfSameType(1, Player1Fleet[4], ownPlayerBoard);// 1xDD1
	deployAllShipsOfSameType(1, Player1Fleet[5], ownPlayerBoard);// 1x DD2
	deployAllShipsOfSameType(1, Player1Fleet[6], ownPlayerBoard);// 1xSS
	printShips(ownPlayerBoard);
	turncounter=switchTurnSafely(turncounter);	// parameter is the guy whose turn just ended...(i.e. p1) 
	//output (return value) into turncounter gives the updated value of turncounter i.e. the active turn (i.e. p2)

/*player1 has deployed all ships

Player2 deploys his entire fleet, now...*/
	printf("Player2 deployes entire fleet now...\n");
	deployAllShipsOfSameType(1, Player2Fleet[0], enemyBoard);
	deployAllShipsOfSameType(1, Player2Fleet[1], enemyBoard);
	deployAllShipsOfSameType(1, Player2Fleet[2], enemyBoard);
	deployAllShipsOfSameType(1, Player2Fleet[3], enemyBoard);
	deployAllShipsOfSameType(1, Player2Fleet[4], enemyBoard);
	deployAllShipsOfSameType(1, Player2Fleet[5], enemyBoard);
	deployAllShipsOfSameType(1, Player2Fleet[6], enemyBoard);
	printShips(enemyBoard);
	turncounter=switchTurnSafely(turncounter);



	while(1){	//main game loop shooting starts now...
		printf("enemy board (p2) looks like this:\n");
		combatModePrintShips(enemyBoard);
		printf("\n");
		printf("own board (p1)  looks like this:\n");
		printShips(ownPlayerBoard);
		printf("\n");
		printf("Player1 starts shooting now...\n");
		keepGameGoing= executeShootingProcedure(enemyBoard, Player2Fleet);		//if the player2 has no ships alive after one shot from player1
		if(keepGameGoing==0){													// then it implies player1 won becaus he was doing the shooting
			printf("player1 WINS THE GAME\n");
			break;
		}
		turncounter=switchTurnSafely(turncounter);
		printf("enemy board (p1) looks like this:\n");
		combatModePrintShips(ownPlayerBoard);
		printf("\n");
		printf("own board (p2) looks like this:\n");
		printShips(enemyBoard);
		printf("\n");
		printf("Player2 starts shooting now...\n");
		keepGameGoing= executeShootingProcedure(ownPlayerBoard, Player1Fleet);
		if(keepGameGoing==0){
			printf("player2 WINS THE GAME\n");
			break;
		}
		turncounter=switchTurnSafely(turncounter);


	}







	return 0;
}

void combatModePrintShips(int enemyBoard[][10]){
	int y=0,x=0;
	int asciichar=65;
	int boardmarker=-10;

	
	printf("  %c ",asciichar);//prints first char on x axis label on top notice theprint is actually   'two spaces, followed by capital A'

	// I guess I just first printed single A, and then started the printloop from capital B --> J carried over from older code
	//the letters had to be aligned properly and be tested

	for(asciichar=66; asciichar<75; asciichar++){	//print B C D E... aka continue the x-axis labels print same  line...
		printf(" %c ", asciichar);
	}
	printf("\n");

	for(y=0; y<=9; y++){
		printf("%d", y); //prints y axis labels 0 1 2 3...
		for(x=0; x<=9; x++){
			boardmarker=enemyBoard[y][x];
			if(  boardmarker<=7 && boardmarker>=0   ){	//empty sea and any shipmarkers stay hidden ???  I guess thats how battleship is
				printf(" . ");
			}
			else if(boardmarker==9){//missmarker
				printf(" o ");
			}
			else if(boardmarker==8){//hitmarker
				printf(" x ");
			}
		}
		printf("\n");//starts newline after one row of values
	}



}


char printIntegerAsChar(int shipIdentifier){
	// NOTE functin is for being used together with  the printShips function so it looks nicer, maybe
	int ship_id=shipIdentifier;
	int ascii=254;	// try ascii 254 or ascii 178
	char letter=ascii;// ascii for square marker
	
	if(ship_id==1){//sub
		letter='s';
	}
	else if(ship_id==2 || ship_id==3 ){//all DDs are essentually the same from player's perspective, but only game logic makes difference between dd1 and dd2 becuaes of "hit and sank" messages etc.
		letter= 'd';
	}
	else if(ship_id==4 || ship_id==5){//all CAs  essentially same from players point of view.
		letter='c';
	}
	else if(ship_id==6){//battleship
		letter='b';
	}
	else if(ship_id==7){
		letter='a';//aircraft carrier
	}

	else if(ship_id==8){//hitmarker
		letter='x';
	}
	else if(ship_id==9){// missmarker
		letter='o';
	}
	else{	//empty sea
		letter='.';
	}
	return letter;

}

void printGameRules(){
	printf("HELLO AND WELCOME TO BATTLESHIP GAME!\n");
	printf("deployment rules:\n");
	printf("1.) Player1(p1) deploys first, then Player2(p2) deploys...\n" );
	printf("2.) HORIZONTAL means that the ship extends RIGHTWARDS from coordinates depending on shipsize\n" );
	printf("3.) VERTICAL means that the ship extends DOWNWARDS from coordinates depending on shipsize\n");
	printf("4.) Ships are allowed to touch eachother ONLY AT THE CORNERS\n");
	printf("5.) when you give coordinates for deploymentpoint/shootingpoint, write only in lowercase and in format 'j5' \n");
	printf("6.) x means hit, o means miss on the gameboard\n");
	printf("\n");
	printf("Shipsizes are as follows: \n");
	printf("Aircraft Carrier(a)= 5\n");
	printf("Battleship(b)= 4\n");
	printf("Cruiser1 and Cruiser2(c)= 3\n");
	printf("Destroyer1 and Destroyer2(d)= 2\n");
	printf("Submarine(s)= 1\n");
	printf("\n");

}

int executeShootingProcedure(int targetBoard[][10],  struct shipDescriptions targetFleet[7] ){
	int undamagedShipWasHit=0;	//acts like booleans
	int gameKeepsGoing=1;
	int i=0,j=0;	//apparently unneeded variables here
	//struct point shootingpoint=askShootingPoint();//ask the user for target coordinates for target square
	struct point shootingpoint=getcharAskShootingPoint(); //safely asks user for target square
	undamagedShipWasHit= hitOrMissAndUpdateShots(shootingpoint, targetBoard, targetFleet);//check what happened to the ships/ocean and update the square into the targetboard appropriately
	if(undamagedShipWasHit==1){										// NOTE IMPORTANT hitOrMiss func also checks if "HitAndSank" or simply "Hit" or "Miss"
		gameKeepsGoing=computeRemainingHitPoints(targetFleet, targetBoard); //computeRemainingHP calculates new hitpoint values for ships, and also updates the hitpoint values to structs(i.e. the fleet data)
		if(gameKeepsGoing==1){												//NOTE IMPORTANT computeRemainingHP func also checks if the targeted player lost the game (i.e. did all his ship die)
			printf("there are still ships alive game goes on to next turn\n");

		}
		else{
			printf("gamestops now, all enemy ships are dead\n");
			gameKeepsGoing=0;
		}
	}
	else{
		gameKeepsGoing=1;	//nothing valuavle was hit by the player ->implies->game cannot end yet->implies->game goes on
	}
	return gameKeepsGoing;
}

int computeRemainingHitPoints(struct shipDescriptions targetFleet[7], int targetBoard[][10]){
	int keepGameGoing=1; //return value for function, if all the targetships died ->implies ->the shooting guy won nthe game
	int j=0; //loop variable
	int x=0,y=0;
	
	int countCV=0,countBB=0,countCA1=0,countCA2=0,countDD1=0,countDD2=0,countSS=0;
	/*we will iterate thru the entire targetboard and count how many instances of ship_identifiers ("alive" squares of ships) we can find
	because each ship was built from unique integers, we can use this fact to count the remaining hitpoints now 
	*/

	for (y=0; y<=9; y++){
		for(x=0; x<=9; x++){
			if(targetBoard[y][x]== 1){
				countSS++;
			}
			else if(targetBoard[y][x]== 2){
				countDD1++;
			}
			else if(targetBoard[y][x]== 3){
				countDD2++;
			}
			else if(targetBoard[y][x]== 4){
				countCA1++;
			}
			else if(targetBoard[y][x]== 5){
				countCA2++;
			}
			else if(targetBoard[y][x]== 6){
				countBB++;
			}
			else if(targetBoard[y][x]== 7){
				countCV++;
			}
			else{
				//do nothing it's either missmarker or hitmarker
			}
		}
	}

	//update the fleet ships' hitpoint values into the memory...
	targetFleet[0].hitpoints= countCV;
	targetFleet[1].hitpoints= countBB;
	targetFleet[2].hitpoints= countCA1;
	targetFleet[3].hitpoints= countCA2;
	targetFleet[4].hitpoints= countDD1;
	targetFleet[5].hitpoints= countDD2;
	targetFleet[6].hitpoints= countSS;
	

	// compute the remaining hitpoints in the fleet... essentially
	int sumHP=0;
	sumHP= countCV+countBB+countCA1+countCA2+countDD1+countDD2+countSS;

	if(sumHP==0){	//targeted player loses game, shooting playerr wins game
		keepGameGoing=0;
	}
	else{	// game keeps going on...
		keepGameGoing=1;
	}

	return keepGameGoing;



}

int hitOrMissAndUpdateShots(struct point shootingpoint, int targetBoard[][10], struct shipDescriptions targetFleet[7]){
	// return results variable should tell us if the shots did anything meaningfuil such as hitting an undamaged part of some ship
	//if they hit the same missed spot, or the same already hit spot, then it's meaningless and result=0
	/*
	hitmarker for SHIP == 8
	hitmarker for OCEAN == 9
	*/
	int yshot=shootingpoint.ycoord;
	int xshot=shootingpoint.xcoord;
	int hitmarker; //return value variable
	int results=0;// did your shooting achieve anything viable i.e. did you hit any undamaged parts of any ships
	int shipIdentifier=-1;
	char c='m';
	switch(xshot){		//convert the x-coord numerical into ->char for print purposes
		case 0: c='a'; break;
		case 1: c='b'; break;
		case 2: c='c'; break;
		case 3: c='d'; break;
		case 4: c='e'; break;
		case 5: c='f'; break;
		case 6: c='g'; break;
		case 7: c='h'; break;
		case 8: c='i'; break;
		case 9: c='j'; break;
	}

	/*first we should check if the shot hit ocean or if it hit any ships*/

	if(targetBoard[yshot][xshot]==0  ){		//HITS OCAEN 
		hitmarker=9;
		targetBoard[yshot][xshot]=hitmarker;//update hit ocean marker into the board
		results=0;
		printf("shot missed at location %c%d where you aimed at\n", c, yshot );
	}
	else if(targetBoard[yshot][xshot]==9){
		printf("shot missed AT SAME SPOT at location %c%d where you aimed at\n", c, yshot );
		//do nothing YOU MISSED THE SAME SPOT TWICE YOU DUMMY xDDD
		results=0;
	}
	else if(targetBoard[yshot][xshot]==8){
		printf("shot hit the SAME SPOT at location %c%d\n", c,yshot );
		//do nothing YOU HIT THE SAME SPOT TWICE YOU DUMMY xDDD it doesn't affect hitpoint loss 
		results=0;
	}
	else if(targetBoard[yshot][xshot]>= 1 && targetBoard[yshot][xshot]<= 7){//HITS SHIP 

		hitmarker=8;		
		results=1;
		shipIdentifier=targetBoard[yshot][xshot];	//find out which ship was hit and check the hitpoints if they are about to die or they survive
		switch(shipIdentifier){
			case 1:	//hit submarine size 1->insta dead
				printf("Hit and Sank at location %c%d\n", c, yshot);
				targetBoard[yshot][xshot]=hitmarker;//updates the ship's marker into -> hitmarker, later other functions can compute hitpoints remianing
				break;
			case 2://hit dd1
				if(targetFleet[4].hitpoints==1){	//notice how we check if the last remaining hitpoint is for the targeted ship or not...
					printf("Hit and Sank at location %c%d\n", c, yshot);
				}
				else{
					printf("shot hit a ship at location %c%d\n",c,yshot );
				}
				targetBoard[yshot][xshot]=hitmarker;	// update the targetboard with the hitmarker AFTER HAVING checked for either "hitAndSank" or "hit"
				break;

			case 3://hit dd2
				if(targetFleet[5].hitpoints==1){
					printf("Hit and Sank at location %c%d\n", c, yshot);
				}
				else{
					printf("shot hit a ship at location %c%d\n",c,yshot );
				}
				targetBoard[yshot][xshot]=hitmarker;
				break;

			case 4://hit cruiser1
				if(targetFleet[2].hitpoints==1){
					printf("Hit and Sank at location %c%d\n", c, yshot);
				}
				else{
					printf("shot hit a ship at location %c%d\n",c,yshot );
				}
				targetBoard[yshot][xshot]=hitmarker;
				break;

			case 5://hit cruiser2
				if(targetFleet[3].hitpoints==1){
					printf("Hit and Sank at location %c%d\n", c, yshot);
				}
				else{
					printf("shot hit a ship at location %c%d\n",c,yshot );
				}
				targetBoard[yshot][xshot]=hitmarker;
				break;
			case 6://hit battleship
				if(targetFleet[1].hitpoints==1){
					printf("Hit and Sank at location %c%d\n", c, yshot);
				}
				else{
					printf("shot hit a ship at location %c%d\n",c,yshot );
				}
				targetBoard[yshot][xshot]=hitmarker;
				break;

			case 7://hit aircraftcarrier
				if(targetFleet[0].hitpoints==1){
					printf("Hit and Sank at location %c%d\n", c, yshot);
				}
				else{
					printf("shot hit a ship at location %c%d\n",c,yshot );
				}
				targetBoard[yshot][xshot]=hitmarker;
				break;
		}


	}

	return results;
	
}

struct point getcharAskShootingPoint(){
	int count=0;
	char string[3]="ZZ";
	int correctinput=0;
	char tempchar='X';
	struct point temppoint;

	while(correctinput==0 ){
			printf("Give shooting coords (ex. d4): ");

			/*the nested getchar loop shoud clear the input buffer with this condition, and final value of tempchar is assigned  entersign*/
			while(((tempchar=getchar())!='\n') ){//careful about operator precedence and brackets
				if (count<2){
					string[count]=tempchar;/* code */
				}
				
				count++;
			}

					//checking the results of string careful about comparison type
			if(string[0]>='a' && string[0]<='j' && string[1]>='0' && string[1]<='9'){
					correctinput=1;	
				//here we have to clear the input buffer for sure 	
			}

			else{
					correctinput=0;		
					printf("SORRY wrong input try again!\n", string,count);
					count=0;
					
					// here we have to clear the input buffer for sure, im a bit paranoid now
					
			}	
	}

	int y=-100, x=-200;
	switch(string[1]){	//ycoord which was a char type "number"
		case '0': y=0 ; break;
		case '1': y= 1; break;
		case '2': y= 2; break;
		case '3': y= 3; break;
		case '4': y= 4; break;
		case '5': y= 5; break;
		case '6': y= 6; break;
		case '7': y= 7; break;
		case '8': y= 8; break;
		case '9': y= 9; break;
	}
	switch(string[0]){	// xcoord which was char type letter --> int numbers
		case 'a': x=0 ; break;
		case 'b': x= 1; break;
		case 'c': x= 2; break;
		case 'd': x= 3; break;
		case 'e': x= 4; break;
		case 'f': x= 5; break;
		case 'g': x= 6; break;
		case 'h': x= 7; break;
		case 'i': x= 8; break;
		case 'j': x= 9; break;
	}

	temppoint.ycoord=y;
	temppoint.xcoord=x;

	return temppoint;




}

struct point getcharAskCoords(){

	int count=0;
	char string[3]="ZZ";
	int correctinput=0;
	char tempchar='X';
	struct point temppoint;

	while(correctinput==0 ){
			printf("Give deployment coords (ex. d4): ");

			/*the nested getchar loop shoud clear the input buffer with this condition, and final value of tempchar is assigned  entersign*/
			while(((tempchar=getchar())!='\n') ){//careful about operator precedence and brackets
				if (count<2){
					string[count]=tempchar;/* code */
				}
				
				count++;
			}

					//checking the results of string careful about comparison type
			if(string[0]>='a' && string[0]<='j' && string[1]>='0' && string[1]<='9'){
					correctinput=1;	
				//here we have to clear the input buffer for sure 	
			}

			else{
					correctinput=0;		
					printf("SORRY wrong input try again!\n", string,count);
					count=0;
					
					// here we have to clear the input buffer for sure, im a bit paranoid now
					
			}	
	}

	int y=-100, x=-200;
	switch(string[1]){	//ycoord which was a char type "number"
		case '0': y=0 ; break;
		case '1': y= 1; break;
		case '2': y= 2; break;
		case '3': y= 3; break;
		case '4': y= 4; break;
		case '5': y= 5; break;
		case '6': y= 6; break;
		case '7': y= 7; break;
		case '8': y= 8; break;
		case '9': y= 9; break;
	}
	switch(string[0]){	// xcoord which was char type letter --> int numbers
		case 'a': x=0 ; break;
		case 'b': x= 1; break;
		case 'c': x= 2; break;
		case 'd': x= 3; break;
		case 'e': x= 4; break;
		case 'f': x= 5; break;
		case 'g': x= 6; break;
		case 'h': x= 7; break;
		case 'i': x= 8; break;
		case 'j': x= 9; break;
	}

	temppoint.ycoord=y;
	temppoint.xcoord=x;

	return temppoint;

}

struct point safeAskShootingPoint(){
//essentially boilerplate code from safeAskDeploymentpoint func

	struct point temppoint;
	int illegal=1;
	char tempstring[4]="RR";

	while(illegal==1){
		printf("Give shooting point coordinates (ex. j9): ");
		/*scanf("%2s",tempstring);
		if(tempstring[0]=='\n'){}else{		while(getchar()!='\n');	//kills extra chars from input so they don't come back to haunt us
}*/		fgets(tempstring, 4, stdin);
		
		

		if(tempstring[0]>='a' && tempstring[0]<='j' && tempstring[1]>='0' && tempstring[1]<='9'  ){
			illegal=0;
			//printf("you gave legal coordinates\n");
			printf("press enter\n");
			while(getchar()!='\n');
		}
		else{
			printf("SORRY you gave ILLEGAL coordinates, try again!\n");
			printf("press enter\n");
			while(getchar()!='\n');	// getchar is like the garbage man of C-programming, he takes out the trash from the input buffer, so the program can work after user input mistakes!!!
		}

	}

	//convert correctly obtained string/chars into --> int type variables, --> transfer data into struct point values and return struct point
	int y=-100, x=-200;
	switch(tempstring[1]){	//ycoord which was a char type "number"
		case '0': y=0 ; break;
		case '1': y= 1; break;
		case '2': y= 2; break;
		case '3': y= 3; break;
		case '4': y= 4; break;
		case '5': y= 5; break;
		case '6': y= 6; break;
		case '7': y= 7; break;
		case '8': y= 8; break;
		case '9': y= 9; break;
	}
	switch(tempstring[0]){	// xcoord which was char type letter --> int numbers
		case 'a': x=0 ; break;
		case 'b': x= 1; break;
		case 'c': x= 2; break;
		case 'd': x= 3; break;
		case 'e': x= 4; break;
		case 'f': x= 5; break;
		case 'g': x= 6; break;
		case 'h': x= 7; break;
		case 'i': x= 8; break;
		case 'j': x= 9; break;
	}

	temppoint.ycoord=y;
	temppoint.xcoord=x;

	return temppoint;

}



struct point askShootingPoint(){

	struct point temppoint;
	int y,x;
	char x_coord_char;
	printf("Give shooting point coordinates (ex. j9): ");
	scanf("%c%d", &x_coord_char, &y);
	getchar();
	/*IF I wasn't such a lazy coder, I would probably have to have some kind of error-checking the answers in this function
	such that the user cannot input crazy coordinates

	but I'm little bit lazy now...*/
	/*NOTICE IMPORTANT THE CHAR CHOICE must be converted into --> integer value for proper integer xy coordinates in the board*/
	switch(x_coord_char){
		case 'a':
			x=0;
			break;
		case 'b':
			x=1;
			break;
		case 'c':
			x=2;
			break;
		case 'd':
			x=3;
			break;
		case 'e':
			x=4;
			break;
		case 'f':
			x=5;
			break;
		case 'g':
			x=6;
			break;
		case 'h':
			x=7;
			break;
		case 'i':
			x=8;
			break;
		case 'j':
			x=9;
			break;
	}

	temppoint.xcoord=x;
	temppoint.ycoord=y;
	return temppoint;
}


void printShips(  int board[][10]  ){
	/*NOTE IMPORTANT THIS FUCNTION ONLY
	TO BE USED FOR DEPLOYMENTS of ships, and friendly ships only, because it revaels all ships...

	*/
	int ycoord=0,xcoord=0;// oops un-needed variables...
	int asciichar=65;
	int i=0,j=0;

	printf("  %c ",asciichar);//prints first char on x axis label on top notice theprint is actually   'two spaces, followed by capital A'

	asciichar++;	// I guess I just first printed single A, and then started the printloop from capital B --> J carried over from older code
	//the letters had to be aligned properly and be tested
/*
this loop prints ABCD... on the top of the x-axis */
	for(asciichar; asciichar<75; asciichar++){
		printf(" %c ", asciichar);
	}
	printf("\n");
/*this loop prints 12345... on the vertical y-axis side of it
loop also prints contents of each row a.k.a. 
the ocean contents of array*/

	for(i=0; i<10; i++){
		printf("%d", i);
		for(j=0; j<10; j++){
			if(board[i][j]==0){
				printf(" . ");
			}
			else{
				printf(" %c ", printIntegerAsChar(board[i][j])   ); // we could print some character sign here instead of the contents 5???
			}
		}
		printf("\n");

	}
	i=0;
	j=0;

}

int switchTurnSafely(int thatWhoseTurnEnded){
	int j=0;
	if(thatWhoseTurnEnded==0){
		printf("Player1's turn ended, now it's Player2's turn... press some something and enter to continue: ");
		while(getchar()!='\n');
		/*prints empty enters to "clear the screen" such that player 2 doesnt see what player1 inputted
		linux terminal does the same thing with clear command LOL xDDD
		*/

		for(j=0; j<=49; j++){	 
			printf("\n");
		}
		
		printf("\n");
		return 1;
	}
	else{
		printf("Player2's turn ended, now it's Player1's turn... press something and enter to continue: ");
		while(getchar()!='\n');

		for(j=0; j<=49; j++){
			printf("\n");
		}
		
		printf("\n");
		return 0;

	}


}

int deployAllShipsOfSameType( int numberToDeploy, struct shipDescriptions blueprint, int board[][10]){
	/*actually it seems that the name of this function is a bit misleading
	because in the end... all the ships had to be unique objects,
	 so just put one ship at a time into the function parameters and deploy it
	*/
	int finishedDeployment=0; //acts like boolean for return value to see if they deployed successfully or not, so that we can move on to other ships for deploying
	int numberOfShips=numberToDeploy;
	int shipsize=blueprint.shipsize;
	char shipname[21];
	strcpy(shipname, blueprint.name);
	char horizvert='h';
	struct point deploymentpoint;
	int shipWasInsideBoard=0;
	int legallyDeployableShip=0;
	int shipIdentifier=blueprint.ship_id;

	while(shipWasInsideBoard==0 || legallyDeployableShip==0 || numberOfShips>0){

		printShips( board);
		printf("Ready to deploy %dx %s\n", numberOfShips, shipname);
		printf("shipsize was %d\n", shipsize);
		//horizvert=askHorizVert();
		horizvert=safeAskHorizVert();
		//deploymentpoint=askCoords();
		deploymentpoint=getcharAskCoords();
		shipWasInsideBoard=checkIfShipOnBoard(shipsize, horizvert, deploymentpoint);
		//legallyDeployableShip=checkIfShipTouchesShip(shipsize, horizvert, deploymentpoint, board);// obsolete version of checkShipTouching function
		legallyDeployableShip=advancedCheckShipDeployment(shipsize, horizvert, deploymentpoint, board);	//modern version of checkShipTouching, it checks if the sides of ships touch, but corner-touching is OK

		/*prints for debugging*/

		/*
		printf("xcoord was %d ycoord was %d\n", deploymentpoint.xcoord, deploymentpoint.ycoord );
		if(horizvert=='h'){
			printf("choice was horizontal\n");
		}
		else if(horizvert=='v'){
			printf("choice was vertical\n");
		}
		*/
		
		if(shipWasInsideBoard==1){
			printf("ship was inside gameboard\n");
		}
		else{
			printf("SHIP ILLEGAL OUTSIDE GAMEBOARD\n");
		}
		
		if(shipWasInsideBoard && legallyDeployableShip){
			printf("ship was legally deployable\n");
		}
		else if(!legallyDeployableShip && shipWasInsideBoard){
			printf("ship WAS ILLEGALLY TOUCHING\n");
		}


		if(legallyDeployableShip && shipWasInsideBoard){
			deployShip(shipsize, shipIdentifier, horizvert, deploymentpoint,  board);
			numberOfShips--;
			shipIdentifier++;	// for cruiser ship specifically, there are two ships such that the ship_id has to be different fo each cruiser
		}
		else{}//nothing happens -> back to loop beginning

	}
	
	if(numberOfShips==0){
		finishedDeployment=1;
	}

	return finishedDeployment;

}

int advancedCheckShipDeployment(int shipsize, char horizvert, struct point deploymentpoint, int board[][10]){
	int x=deploymentpoint.xcoord, y=deploymentpoint.ycoord;
	int ymax=y+shipsize-1;
	int xmax=x+shipsize-1; // in loops rememmber iterate <=ymax or <=xmax
	int legallyDeployableShip=0;
	int size=shipsize;
	int orig_y=y;	//just to be sure we take the original coordinates into safe memory because x and y will be iterated over as loop variables
	int orig_x=x;
	int error=0;	//boolean style flag variable to see if the logic seems to work out or not for ship touching checks

	/* In this advanced function we will check IF ship overlaps another SHIP AND ALSO WE CHECK
	
	IF the ships are touching each other at horizontal / vertyical edges 

	CORNER TOUCHING IS OK AND COMPLETELY LEGAL IN BATTLESHIP THAT IS MY OPINION ON THE MATTER*/

	//vertical choice begins below
	if(horizvert=='v'){		
		if(x==0){		// so-called case a
			if(y==0){	//check x0 y0
				for(y; y<=ymax; y++){
					if( (board[y][x]!=0 ) || (board[y][x+1]!=0)){	//checks no overlap and rightside
						error=1;
						break;
					}
				}
				y=orig_y;
				if(board[ymax+1][x]!=0){	//checks the square below the ship
					error=1;
				}
			}
			else if(ymax==9){	//check x0 and end of ship touches bottom border i.e. last square of ship == (x0, ymax)
				for(y; y<=ymax;y++){
					if( (board[y][x]!=0) || (board[y][x+1]!=0) ){	//checks rightside and no overlap
						error=1;
						break;
					}
				}
				y=orig_y;
				if(board[y-1][x]!=0){	// checks one above deployment point square
					error=1;
				}

			}
			else if(y>=1 && ymax<=8){// check x0 and y biggerthan 1 and ymax <=8 included i.e. ship is vertical at x0 but such that it doesn't touch top and bottom
				for(y; y<=ymax; y++){
					if( (board[y][x]!=0) || (board[y][x+1]!=0) ){
						error=1;
						break;
					}
				}
				y=orig_y;
				if(  (board[ymax+1][x]!=0)  || ( board[y-1][x]!=0 )   ){ //cheks   one square below andone square above
					error=1;
				}
			}
		}		
		else if(x==9){		// so-called case b, ship is vertical and x=9
						
			if(y==0){//check x9 y0
				for(y; y<=ymax; y++){
					if( (board[y][x]!=0)||(board[y][x-1]!=0)  ){ //checks no overlap abd the leftside squares
						error=1;
						break;
					}
				}
				y=orig_y;
				if(board[ymax+1][x]!=0){ //checks the below square
					error=1;
				}
			}
			else if(ymax==9){	//ship is vertical and x=9 and ship touches bottom border
				for(y; y<=ymax; y++){
					if( (board[y][x]!=0) || (board[y][x-1]!=0) ){	//check no overlap and the leftside
						error=1;
						break;
					}
				}
				y=orig_y;
				if(board[y-1][x]!=0){		// checks the above square
					error=1;
				}
			}
			else if(y>=1 && ymax<= 8){		// ship is vert, and x=9 , and ship doesnt touch top and bottom borders
				for(y; y<=ymax; y++){
					if( (board[y][x]!=0) || (board[y][x-1])  ){	//checks no overlap and the leftside
						error=1;
						break;
					}
				}
				y=orig_y;
				if( (board[ymax+1][x]!=0) ||   (board[y-1][x]!=0) ){	//checks aboe and below squares
					error=1;
				}
			}
			
		}
		else if(x>=1 && x <= 8){	//so -called case c, ship is vertical and x coords between 1 and 8 
			if(y==0){	//ship touches top border
				for(y; y<=ymax; y++){
					if( (board[y][x]!=0) || (board[y][x-1]!=0) || (board[y][x+1]!=0) ){	//check leftside rightside no overlap
						error=1;
						break;
					}
				}
				y=orig_y;
				if(board[ymax+1][x]!=0){		//check  bottom square
					error=1;
				}
			}
			else if(ymax==9){	//ship is vert, xcoord between 1 and 8, ship touches bottom border
				for(y; y<=ymax; y++){
					if( (board[y][x]!=0) || (board[y][x-1]!=0) || (board[y][x+1]!=0) ){	//check leftside rightside no overlap
						error=1;
						break;
					}
				}
				y=orig_y;
				if(board[y-1][x]!=0){//check above square
					error=1;
				}
			}
			else if(ymax<=8 && y>=1){	//ship is vert, ship doesnt touch any borders
				for(y; y<=ymax; y++){
					if( (board[y][x]!=0) || (board[y][x-1]!=0) || (board[y][x+1]!=0) ){	//check leftside rightside no overlap
						error=1;
						break;
					}
				}
				y=orig_y;
				if( (board[ymax+1][x]!=0) || (board[y-1][x]!=0)  ){	//check above and below squares
					error=1;
				}
			}
		}

	}
	//vertical choice ended above...

	//horizontal choice begins here
	else if(horizvert=='h'){
		if(y==0){		// so-called case a 
			if(x==0){	//ship is horiz, x=0, y=0, ship touches top and left border
				for(x; x<=xmax; x++){
					if( (board[y][x]!=0) || (board[y+1][x]!=0)   ){	//check no overlap and bottomsides
						error=1;
						break;
					}
				}
				x=orig_x;
				if( board[y][xmax+1]!=0 ){	//check rightmost square
					error=1;
				}
			}
			else if(xmax==9){	//ship is horiz, y=0, ship touches right and top border
				for(x; x<=xmax; x++){
					if( (board[y][x]!=0) || (board[y+1][x]!=0)   ){	//check no overlap and bottomsides
						error=1;
						break;
					}
				}
				x=orig_x;
				if(board[y][x-1]!=0){	//check the leftsided square
					error=1;
				}
			}
			else if(x>=1 && xmax<=8){		//ship is horiz, y=0, ship touches only top border
				for(x; x<=xmax; x++){
					if( (board[y][x]!=0) || (board[y+1][x]!=0)   ){	//check no overlap and bottomsides
						error=1;
						break;
					}
				}
				x=orig_x;
				if(  (board[y][xmax+1]!=0) ||  (board[y][x-1]!=0) ){	//check left square and right square
					error=1;
				}
			}
		}
		else if(y==9){	// so-called case b
			if(x==0){	//ship is horiz, y=9 x=0, ship touches left  and bottom border
				for(x; x<=xmax; x++){
					if( (board[y][x]!=0) || (board[y-1][x]!=0) ){	//check no overlap and topsides
						error=1;
						break;
					}
				}
				x=orig_x;
				if(board[y][xmax+1]!=0){		//checks rightsquare
					error=1;
				}
			}
			else if(xmax==9){//ship is horiz, y=9, ship touches right and bottom border
				for(x; x<=xmax; x++){
					if( (board[y][x]!=0) || (board[y-1][x]!=0) ){//checks no overlap and topsides
						error=1;
						break;
					}
				}
				x=orig_x;
				if(board[y][x-1]!=0){	//checks leftsquare
					error=1;
				}
			}
			else if(x>=1 && xmax<=8){  //ship is horiz, but only touches bottom border
				for(x; x<=xmax; x++){
					if( (board[y][x]!=0) || (board[y-1][x]!=0) ){//checks no overlap and topsides
						error=1;
						break;
					}
				}
				x=orig_x;
				if( (board[y][xmax+1]!=0) || (board[y][x-1]!=0) ){	//checks rightmost and leftmost squares
					error=1;
				}
			}
		}
		else if(y>=1 && y<=8){	// so-called case c
			if(x==0){	//ship is horiz, y is between 1 and 8 and x=0, ship touches leftborder
				for(x; x<=xmax; x++){		
					if( (board[y][x]!=0) || (board[y-1][x]!=0)  || (board[y+1][x]!=0)  ){			// check no overlap topsides and bottomsides
						error=1;
						break;
					}
				}
				x=orig_x;
				if(board[y][xmax+1]!=0){//checks rightmost square
					error=1;
				}
			}
			else if(xmax==9){	//ship is horiz, y is between 1 and 8 (i.e. doesnt touch top or bottom) but does touch rightborder
				for(x; x<=xmax; x++){		
					if( (board[y][x]!=0) || (board[y-1][x]!=0)  || (board[y+1][x]!=0)  ){			// check no overlap topsides and bottomsides
						error=1;
						break;
					}
				}
				x=orig_x;
				if( board[y][x-1]!=0 ){	//checks lefstmost square
					error=1;
				}
			}
			else if(x>=1 && xmax<=8){	//ship is horiz, but doesnt touch any borders anywhere
				for(x; x<=xmax; x++){		
					if( (board[y][x]!=0) || (board[y-1][x]!=0)  || (board[y+1][x]!=0)  ){			// check no overlap topsides and bottomsides
						error=1;
						break;
					}
				}
				x=orig_x;
				if( (board[y][x-1]!=0) || (board[y][xmax+1]!=0) ){	//checks leftmost square and rightmost square 
					error=1;
				}
			}
		}
	}

	//collect the results of the boolean logic
	if(error==1){
		legallyDeployableShip=0;
		return legallyDeployableShip;
	}
	else{
		legallyDeployableShip=1;
		return legallyDeployableShip;
	}

}

void deployShip(int shipsize, int shipIdentifier ,char horizvert, struct point deploymentpoint, int board[][10]){
		/*NOTE NEW PARAMETER shipIdentifier of the integer type
		because gameboard is int type, we will put cruiser==shipsize3== represented by some integer inside the board

		ALSO NOTE because each ship must be unique set of squares inside the gameboard
		e.g.
		....
		cruiser alpha = 333;
		cruiser beta= 444;
		cruiser charlie = 555;
		or soemthing like that, there are three integer numbers each different integer for different discrete cruiser ship,
		because there are 3 cruiser ships, where each cruiser consists of three squares, 
		same is true for destroyers because there are more than one destroyer on the gameboard.
		
		only for battleships and aircraft carriers, because there is only one ship of each type battleship or carrier, so in that case it's just one ship each.

		*/

	//This function simply deployes already legally checked ships into the proper coordinates.
		int y=deploymentpoint.ycoord, x=deploymentpoint.xcoord;
		int ymax=y+shipsize;
		int xmax=x+shipsize;
		if(horizvert=='v'){
				for(y; y<ymax; y++){
					board[y][x]=shipIdentifier;
				}
		}
		else if(horizvert=='h'){
				for(x; x<xmax; x++){
					board[y][x]=shipIdentifier;
				}
		}
}

int checkIfShipOnBoard(int shipsize, char horizvert, struct point deploymentpoint){

	int shipLegality=0;// is the ship legally deployable THIS ACTS LIKE BOOLEAN VARIABLE
	int i=0,j=0;
	int y=deploymentpoint.ycoord;
	int x=deploymentpoint.xcoord;


	if(horizvert=='v'){
		if(y+shipsize<=10 && (x<=9 && x>=0)  ){	// checks if there exists enough empty space downwards from deployment point
			shipLegality=1;
		}
		else{
			shipLegality=0;
		}

	}
	else if(horizvert=='h'){
		if(x+shipsize<=10 && (y<=9 && y>=0) ){	//checks if there exists enough empty space rightwards from deployment point
			shipLegality=1;
		}
		else{
			shipLegality=0;
		}
	}

	return shipLegality;
}
char safeAskHorizVert(){
	
	int illegal=1;
	char answer='W';
	

	while(illegal==1){
		printf("Deploy ship horiz/vert (h/v): ");
		scanf("%c", &answer);
		if(answer=='\n'){
		}
		else{
		while(getchar()!='\n');//kills extra chars from input so they don't come back to haunt us

		}

		if(answer=='h' || answer=='v'){
			illegal=0;
			//printf("you gave legal answer\n" );
		}
		else{
			printf("SORRY you gave ILLEGAL answer, try again!\n");
		}

	}

	return answer;

}

char askHorizVert(){
	/*IF I wasn't such a lazy coder, I would probably have to have some kind of error-checking the answers in this function
	such that the user cannot input crazy stuff

	but I'm little bit lazy now...*/
	char r='r';
	printf("Deploy ship horizontal/vertical (h/v)?: ");
	scanf("%c",&r);
	getchar();
	return r;
}

struct point safeAskCoords(){
	struct point temppoint;
	int illegal=1;
	char tempstring[4]="RR";

	while(illegal==1){
		printf("Give deployment point coordinates (ex. a5): ");
		//scanf("%2s",tempstring);
		fgets(tempstring, 4, stdin);
		/*if(tempstring[0]=='\n' || tempstring[1]=='\n'){

		}
		else{		while(getchar()!='\n');	//kills extra chars from input so they don't come back to haunt us
}*/

		if(tempstring[0]>='a' && tempstring[0]<='j' && tempstring[1]>='0' && tempstring[1]<='9'  ){
			illegal=0;
			//printf("you gave legal coordinates\n");
			printf("press enter\n");
			while(getchar()!='\n');
		}
		else{
			printf("SORRY you gave ILLEGAL coordinates, try again!\n");
			printf("press enter\n");
			while(getchar()!='\n');
		}

	}
	//convert correctly obtained string/chars into --> int type variables, --> transfer data into struct point values and return struct point
	int y=-100, x=-200;
	switch(tempstring[1]){	//ycoord which was a char type "number"
		case '0': y=0 ; break;
		case '1': y= 1; break;
		case '2': y= 2; break;
		case '3': y= 3; break;
		case '4': y= 4; break;
		case '5': y= 5; break;
		case '6': y= 6; break;
		case '7': y= 7; break;
		case '8': y= 8; break;
		case '9': y= 9; break;
	}
	switch(tempstring[0]){	// xcoord which was char type letter --> int numbers
		case 'a': x=0 ; break;
		case 'b': x= 1; break;
		case 'c': x= 2; break;
		case 'd': x= 3; break;
		case 'e': x= 4; break;
		case 'f': x= 5; break;
		case 'g': x= 6; break;
		case 'h': x= 7; break;
		case 'i': x= 8; break;
		case 'j': x= 9; break;
	}

	temppoint.ycoord=y;
	temppoint.xcoord=x;

	return temppoint;


}

struct point askCoords(){
	struct point temppoint;
	int y,x;
	char x_coord_char;
	printf("Give deployment point coordinates (ex. a5) : ");
	scanf("%c%d", &x_coord_char, &y);
	getchar();
	/*IF I wasn't such a lazy coder, I would probably have to have some kind of error-checking the answers in this function
	such that the user cannot input crazy coordinates

	but I'm little bit lazy now...*/
	/*NOTICE IMPORTANT THE CHAR CHOICE must be converted into --> integer value for proper integer xy coordinates in the board*/
	switch(x_coord_char){
		case 'a':
			x=0;
			break;
		case 'b':
			x=1;
			break;
		case 'c':
			x=2;
			break;
		case 'd':
			x=3;
			break;
		case 'e':
			x=4;
			break;
		case 'f':
			x=5;
			break;
		case 'g':
			x=6;
			break;
		case 'h':
			x=7;
			break;
		case 'i':
			x=8;
			break;
		case 'j':
			x=9;
			break;
	}

	temppoint.xcoord=x;
	temppoint.ycoord=y;
	return temppoint;
}


void populateEmptyBoard(int board[][10]){
/*
this function puts "empty sea" in this case int 0 into empty sea at the beginning of the deployments*/
	int ycoord=0;
	int xcoord=0;
	int emptyocean=0;

	for (ycoord=0; ycoord<=9; ycoord++){
		for(xcoord=0; xcoord<=9; xcoord++){
			board[ycoord][xcoord]=emptyocean;
		}
	}
}


int checkIfShipTouchesShip(int shipsize, char horizvert, struct point deploymentpoint, int board[][10]){

	///NOTE MOST IMPORTANT this is an obsolete function which only checks for overlapping ship squares not touching at the edges

	// NOTE IMPORTANT empty sea square == int 0, we have to iterate the board for empty sea squares
	//ALSO NOTICE it is important to 
	/*
		NOT CALL THIS FUNCTION in main program, in SUCH CASES 
	where	the ship was ALREADY OUTSIDE THE GAMEBOARD	
	because the counting will becom fucked up!!!	*/
	
	int i=0,j=0, x=deploymentpoint.xcoord, y=deploymentpoint.ycoord;
	int legallyDeployableShip=0;
	int size=shipsize;
	int ymax=y+size;	//boundary value for iterating to see if the ENTIRE ship overlaps ONLY empty ocean
	int xmax=x+size;
	if(horizvert=='v'){
		for(y; y<ymax; y++){
			if(board[y][x]==0){
				size--;// decrement shipsize to find out later WHETHER / OR NOT the entire ship fitted into empty sea squares
			}
					
		}
	}

	else if(horizvert=='h'){
		for(x; x<xmax; x++){
			if(board[y][x]==0){
				size--;
			}
		}
	}

	if(size==0){
		legallyDeployableShip=1;
		printf("remaining size was %d\n",size);
		
	}
	else if(size>0){
		legallyDeployableShip=0;
		printf("remaining size was %d\n",size);
		
	}
	return legallyDeployableShip;

}

void showPlayerChoices(struct point deppoint, char horizvert, int wasShipInside, int shipsize){

	/*
	NOTE this function existed only for a short time for debugging purposes
	*/
	printf("xcoord was %d and ycoord was %d \n", deppoint.xcoord, deppoint.ycoord );
	if(horizvert=='h'){
		printf("deploymentchoice was horizontal extending ->rightwards\n");
	}
	else{
		printf("deploymentchoice was vertical extending \\|/  downwards \n");
	}
	printf("shipsize was %d\n", shipsize);

	if(wasShipInside){
		printf("ship was inside gameboard\n");
	}
	else{
		printf("SHIP WAS OUTSIDE GAMEBOARD ILLEGAL\n");
	}

}
