#include<iostream>

const int MaxSize = 20;    //The max quantity of card of each colour in the game.
const int MaxColour = 7;   //The quatity of kind of colour in the game is 7,so I suppose the max of colour is 7 --- 0  green,1 blue,2 red,3 violet,4 yellow,5 white,6 black

int n;      //n-the amount of players
int k;      //k-the amount of cauldrons.

typedef struct                                 //the card has elements of value and colour
{
	int CardValue;                             //value of card
	char CardColour[7];                        //colour of card
}Card;                                         //the card
Card Deck[MaxSize * MaxColour];                //used to store cards
int Index_Card;                                //used to input card into deck

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
		for (int j = i + 1; j < index_card; j++)                            //every card after the card being checked
		{
			if (strcmp(Deck[i].CardColour, Deck[j].CardColour) == 0)                   //if they are the same colour
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


int ScreenShotPlayer(int number, int index_card)
{
	for (int i = 0; i < number; i++)                                       //input one player by one
	{
		scanf_s("%d player hand cards:", &(Players[i].Index_Player));      //input player index
		Players[i].Number_Hand = 0;                                        //initial the card number in hand
		Players[i].Number_Graveyard = 0;                                   //initial the card number in front
		char HandString[250] = { 0 };                                      //used to store the card string of hand
		char FrontString[250] = { 0 };									   //used to store the card string of front
		std::cin.getline(HandString, 250);                                 //input the whole line

		for (int j = 0; HandString[j] != '\0'; j++)                        //count the card on hand 
		{
			if (HandString[j] != ' ')                                      //from the value char
			{
				char cardvalue[3];                                         //store one value
				int cv = 0;                                                //index of array cardvalue
				for (; HandString[j] != ' '; j++, cv++)                    //input until the blank after the value
				{
					cardvalue[cv] = HandString[j];                         //cut the value into char
				}
				cardvalue[cv] = '\0';                                      //add the end of string
				Deck[index_card].CardValue = atoi(cardvalue);              //input the card value into deck
				j++;                                                       //skip the space between value and colour
				char cardcolour[7];                                        //store one colour
				int cc = 0;                                                //index of array cardcolour
				for (; HandString[j] != ' ' && HandString[j] != '\0'; j++, cc++)    //cut the colour like value
				{
					cardcolour[cc] = HandString[j];
				}
				cardcolour[cc] = '\0';
				strcpy_s(Deck[index_card].CardColour, cardcolour);         //input the colour into deck
				index_card++;                                              //point to next card
				Players[i].Number_Hand++;                                  //count the card above
			}
		}

		scanf_s("%d player deck cards:", &(Players[i].Index_Player));      //count the card in front

		std::cin.getline(FrontString, 250);
		for (int j = 0; FrontString[j] != '\0'; j++)                       //count the card in front
		{
			if (FrontString[j] != ' ')                                     //from the value char
			{
				char cardvalue[3];                                         //store one value
				int cv = 0;                                                //index of array cardvalue
				for (; FrontString[j] != ' '; j++, cv++)                   //input until the blank after the value
				{
					cardvalue[cv] = FrontString[j];                        //cut the value into char
				}
				cardvalue[cv] = '\0';                                      //add the end of string
				Deck[index_card].CardValue = atoi(cardvalue);              //input the card value into deck
				j++;                                                       //skip the space between value and colour
				char cardcolour[7];                                        //store one colour
				int cc = 0;                                                //index of array cardcolour
				for (; FrontString[j] != ' ' && FrontString[j] != '\0'; j++, cc++)    //cut the colour like value
				{
					cardcolour[cc] = FrontString[j];
				}
				cardcolour[cc] = '\0';                                     //add the end of string
				strcpy_s(Deck[index_card].CardColour, cardcolour);         //input the colour into deck
				index_card++;                                              //point to next card
				Players[i].Number_Graveyard++;                             //count the card above
			}
		}
	}
	return index_card;
}


int ScreenShotCauldron(int kvalue, int index_card)
{
	for (int i = 0; i < kvalue; i++)
	{
		scanf_s("%d pile cards:", &(Cauldrons[i].Index_Cauldron));         //input which cauldron

		char PileString[250] = { 0 };									   //used to store the card string of front
		std::cin.getline(PileString, 250);                                 //input the whole line

		for (int j = 0; PileString[j] != '\0'; j++)                        //count the card on hand 
		{
			if (PileString[j] != ' ')                                      //from the value char
			{
				char cardvalue[3];                                         //store one value
				int cv = 0;                                                //index of array cardvalue
				for (; PileString[j] != ' '; j++, cv++)                    //input until the blank after the value
				{
					cardvalue[cv] = PileString[j];                         //cut the value into char
				}
				cardvalue[cv] = '\0';                                      //add the end of string
				Deck[index_card].CardValue = atoi(cardvalue);              //input the card value into deck
				j++;                                                       //skip the space between value and colour
				char cardcolour[7];                                        //store one colour
				int cc = 0;                                                //index of array cardcolour
				for (; PileString[j] != ' ' && PileString[j] != '\0'; j++, cc++)    //cut the colour like value
				{
					cardcolour[cc] = PileString[j];
				}
				cardcolour[cc] = '\0';
				strcpy_s(Deck[index_card].CardColour, cardcolour);         //input the colour into deck
				index_card++;                                              //point to next card
				Cauldrons[i].Number_CardsInCauldron++;                     //count the card above
			}
		}
	}
	return index_card;
}


void GameScreenShots(int number)                        //input the game situation
{
	Index_Card = 0;                                     //used to input card into deck

	Index_Card = ScreenShotPlayer(number, Index_Card);       //count cards of players

	k = CountColours(Index_Card, k);                    //count how many colours i.e. how many cauldrons

	Index_Card = ScreenShotCauldron(k, Index_Card);     //count cards of cauldrons
}


void CheckGreenCards()                                  //check green and out put
{
	int GreenValue[MaxSize];                            //used to store value of green
	int index_greenvalue = 0;                           //index of array greenvalue
	int Number_Green = 0;                               //how many green cards
	for (int i = 0; i < Index_Card; i++)                //test cards in deck
	{
		if (strcmp(Deck[i].CardColour, "green") == 0)              //if the colour is green
		{
			Number_Green++;                                        //increase a green card
			int SameOrDifer = 0;                                   //if there is a same value, it will be 1
			for (int j = 0; j < index_greenvalue; j++)             //check if there is the same value before
			{												       
				if (GreenValue[j] == Deck[i].CardValue)            //if there is a same value
				{											       
					SameOrDifer = 1;                               //change into 1
				}											       
			}												       
			if (SameOrDifer == 0)                                  //if there isn't the same value
			{												       
				GreenValue[index_greenvalue] = Deck[i].CardValue;  //add the value to array
				index_greenvalue++;                                //point to next green value
			}
		}
	}
	if (index_greenvalue == 0)                                      //if there is no green value
	{
		std::cout << "Green cards does not exist";
	}
	else if (index_greenvalue > 1)                                  //if there are different values of green card
	{
		std::cout << "Different green cards values occurred";
	}
	else                                                            //there is only one green value
	{
		printf("Found %d green cards, all with %d value", Number_Green, GreenValue[0]);  
	}

}


int main() {
	scanf_s("active player = 1\nplayers number = %d", &n);  //input the first and second lines

	GameScreenShots(n);                                     //input the situation

	CheckGreenCards();                                      //check green and out put

	return 0;
}