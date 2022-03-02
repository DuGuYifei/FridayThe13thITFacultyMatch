#include<iostream>

const int MaxSize = 20;    //The max quantity of card of each colour in the game.
const int MaxColour = 7;   //The quatity of kind of colour in the game is 7,so I suppose the max of colour is 7 --- 0  green,1 blue,2 red,3 violet,4 yellow,5 white,6 black

int n;      //n-the amount of players
int k;      //k-the amount of cauldrons. 
typedef struct                                 //the card has elements of value and colour
{
	int CardValue;                             //value of card
	std::string CardColour;                        //colour of card
}Card;                                         //the card
Card Deck[MaxSize * MaxColour];

typedef struct                                 //the player has elements of index,quantities,cards
{
	int Index_Player;                          //index of player
	int Number_Hand;                           //how many cards in hand
	int Number_Graveyard;                      //how many cards in graveyard
}Player;                                       //the player
Player Players[6];                             //array for players

typedef struct                                 //the cauldron has element of index, quantity, cards
{
	int Index_Cauldron;                        //index of cauldron
	int Number_CardsInCauldron;                //how many cards in the cauldron
}Cauldron; 
Cauldron Cauldrons[6];                         //array to store all cauldrons


int CountColours(int index_card, int kvalue)                                          //how many colours without green
{             
	kvalue = 0;//There is a colour--green should be delete,so it should count from -1, but the last colour in last card will not be count,so it should count from 0;
	for (int i = 0; i < index_card - 1; i++)                                //check every card with cards after it
	{
		int SameOrDifer = 0;                                                //if it will have the same colour, it will change into 1
		for (int j = i + 1; j < index_card; j++)                       //every card after the card being checked
		{
			if (Deck[i].CardColour == Deck[j].CardColour)                   //if they are the same colour
			{
				SameOrDifer = 1;                                            //change it to 1
			}
		}
		if (SameOrDifer == 0)                                               //if don't have the same colour after it
		{
			kvalue++;                                                       //count the colour
		}
	}
	return kvalue;
}


void GameScreenShots(int number)                        //input the game situation
{
	int Index_Card = 0;
	for (int i = 0; i < number; i++)                                       //input one player by one
	{
		scanf_s("%d player hand cards:", &(Players[i].Index_Player));      //input
		Players[i].Number_Hand = 0;
		Players[i].Number_Graveyard = 0;
		while (std::cin.get() != '\n')                                     //count the card on hand,until input'\n'
		{
			std::cin >> Deck[Index_Card].CardValue;                        //input the card value
			std::cin >> Deck[Index_Card].CardColour;                       //input the card colour
			Index_Card++;                                                  //for the next card
			Players[i].Number_Hand++;                                      //count the card above
		}
		scanf_s("%d player deck cards:", &(Players[i].Index_Player));      //count the card in front,until input '\n'
		while (std::cin.get() != '\n')
		{
			std::cin >> Deck[Index_Card].CardValue;
			std::cin >> Deck[Index_Card].CardColour;
			Index_Card++;
			Players[i].Number_Graveyard++;                                  //count the card above
		}
	}

	
	k = CountColours(Index_Card, k);                                       //count how many cauldrons i.e. piless

	for (int i = 0; i < k; i++)                                             //input cards in cauldrons
	{
		scanf_s("%d pile cards:", &(Cauldrons[i].Index_Cauldron));          //input which cauldron
		while (std::cin.get() != '\n')                                      //count the card in cauldron, unrtil input until '\n'
		{
			std::cin >> Deck[Index_Card].CardValue;                         //input value
			std::cin >> Deck[Index_Card].CardColour;                        //input colour
			Index_Card++;                                                   //for next card 
			Cauldrons[i].Number_CardsInCauldron++;                          //count the card above
		}
	}
}


void DeckTracker(int number)                              //cout the deck
{
	//print cards of players
	for (int i = 0; i < number; i++)
	{
		printf("%d player has %d cards on hand\n%d player has %d cards in front of him\n", Players[i].Index_Player, Players[i].Number_Hand, Players[i].Index_Player, Players[i].Number_Graveyard);
	}
	//print cards of piles
	for (int i = 0; i < k; i++)
	{
		printf("there are %d cards on %d pile\n", Cauldrons[i].Number_CardsInCauldron, Cauldrons[i].Index_Cauldron);
	}
}


int main() {
	scanf_s("active player = 1\nplayers number = %d", &n);  //input the first and second lines

	GameScreenShots(n);                           //input the situation

	DeckTracker(n);                               //output the decak tracker

	return 0;
}