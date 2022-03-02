#include<iostream>

const int MaxSize = 20;    //The max quantity of card of each colour in the game.
const int MaxColour = 7;   //The quatity of kind of colour in the game is 7,so I suppose the max of colour is 7 --- 0  green,1 blue,2 red,3 violet,4 yellow,5 white,6 black

int n;  //n-the amount of players
 
typedef struct                                 //the card has elements of value and colour
{
	int CardValue;                             //value of card
	char CardColour[7];                        //colour of card
}Card;                                         //the card

typedef struct                                 //the player has elements of index,quantities,cards
{
	int Index_Player;                          //index of player
	int Number_Hand;                           //how many cards in hand
	int Number_Graveyard;                      //how many cards in graveyard
	Card Hand[MaxSize * MaxColour];            //store hand
	Card Graveyard[MaxSize * MaxColour];       //store graveyard
}Player;                                       //the player
Player Players[6];                             //array for players

void GameScreenShots(Player players[6], int number)                        //input the game situation
{
	for (int i = 0; i < number; i++)                                       //input one player by one
	{
		scanf_s("%d player hand cards:", &(players[i].Index_Player));      //input
		players[i].Number_Hand = 0;          
		players[i].Number_Graveyard = 0;
		int j = 0;
		while (std::cin.get() != '\n')                                     //count the card on hand 
		{
			std::cin >> players[i].Hand[j].CardValue;                      //input the card value
			std::cin >> players[i].Hand[j].CardColour;                     //input the card colour
			j++;                                                           //for the next card
			players[i].Number_Hand++;                                      //count the card above
		}

		scanf_s("%d player deck cards:", &(players[i].Index_Player));      //count the card in front
		j = 0;
		while (std::cin.get() != '\n')
		{
			std::cin >> players[i].Graveyard[j].CardValue;
			std::cin >> players[i].Graveyard[j].CardColour;
			j++;
			players[i].Number_Graveyard++;                                  //count the card above
		}
	}
}


void DeckTracker(Player players[6], int number)                              //cout the deck
{
	//print the quantity of cards in players
	for (int i = 0; i < number; i++)
	{
		printf("%d player has %d cards on hand\n%d player has %d cards in front of him\n", players[i].Index_Player, players[i].Number_Hand, players[i].Index_Player, players[i].Number_Graveyard);
	}
}


int main() {
	scanf_s("active player = 1\nplayers number = %d",&n);  //input the first and second lines
	
	GameScreenShots(Players, n);                           //input the situation
	
	DeckTracker(Players, n);                               //output the deck tracker
	
	return 0;
}