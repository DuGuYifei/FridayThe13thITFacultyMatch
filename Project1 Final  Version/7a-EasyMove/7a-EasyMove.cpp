#include<iostream>

const int MaxSize = 20;    //The max quantity of card of each colour in the game.
const int MaxColour = 7;   //The quatity of kind of colour in the game is 7,so I suppose the max of colour is 7 --- 0  green,1 blue,2 red,3 violet,4 yellow,5 white,6 black

int ActivePlayer;          //the active player's index
int n;                     //n-the amount of players
int k;                     //k-the amount of cauldrons.
int ExplodeThreshold;      //the explode threshold

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
	Card Card_Hand[MaxSize * MaxColour];       //store cards in hand
	Card Card_Graveyard[MaxSize * MaxColour];  //store cards in front
}Player;                                       //the player
Player Players[6];                             //array for players

typedef struct                                 //the cauldron has element of index, quantity, cards
{
	int Index_Cauldron;                        //index of cauldron
	int Number_CardsInCauldron;                //how many cards in the cauldron
	Card CardsInCauldron[2*MaxSize];             //used to store cards in cauldron
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
				Players[i].Card_Hand[Players[i].Number_Hand].CardValue = atoi(cardvalue); //put the card into Players too
				j++;                                                       //skip the space between value and colour
				char cardcolour[7];                                        //store one colour
				int cc = 0;                                                //index of array cardcolour
				for (; HandString[j] != ' ' && HandString[j] != '\0'; j++, cc++)    //cut the colour like value
				{
					cardcolour[cc] = HandString[j];
				}
				cardcolour[cc] = '\0';
				strcpy_s(Deck[index_card].CardColour, cardcolour);         //input the colour into deck
				strcpy_s(Players[i].Card_Hand[Players[i].Number_Hand].CardColour, cardcolour); //put the card into the Players too
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
				Players[i].Card_Graveyard[Players[i].Number_Graveyard].CardValue = atoi(cardvalue); //put the card into Players too
				j++;                                                       //skip the space between value and colour
				char cardcolour[7];                                        //store one colour
				int cc = 0;                                                //index of array cardcolour
				for (; FrontString[j] != ' ' && FrontString[j] != '\0'; j++, cc++)    //cut the colour like value
				{
					cardcolour[cc] = FrontString[j];
				}
				cardcolour[cc] = '\0';                                     //add the end of string
				strcpy_s(Deck[index_card].CardColour, cardcolour);         //input the colour into deck
				strcpy_s(Players[i].Card_Graveyard[Players[i].Number_Graveyard].CardColour, cardcolour); //put the card into the Players too
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
				Cauldrons[i].CardsInCauldron[Cauldrons[i].Number_CardsInCauldron].CardValue = atoi(cardvalue); //put the card value into Cauldron,too 
				j++;                                                       //skip the space between value and colour
				char cardcolour[7];                                        //store one colour
				int cc = 0;                                                //index of array cardcolour
				for (; PileString[j] != ' ' && PileString[j] != '\0'; j++, cc++)    //cut the colour like value
				{
					cardcolour[cc] = PileString[j];
				}
				cardcolour[cc] = '\0';
				strcpy_s(Deck[index_card].CardColour, cardcolour);         //input the colour into deck
				strcpy_s(Cauldrons[i].CardsInCauldron[Cauldrons[i].Number_CardsInCauldron].CardColour, cardcolour);      ////put the card color into Cauldron,too 
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


void MoveCardIntoCauldron(int activeplayer, int index_card_move)
{
	if (strcmp(Players[activeplayer].Card_Hand[index_card_move].CardColour, "green") == 0)  // if the card is green
	{
		Cauldrons[0].CardsInCauldron[Cauldrons[0].Number_CardsInCauldron] = Players[activeplayer].Card_Hand[index_card_move];//copy to the last
		Cauldrons[0].Number_CardsInCauldron++;  //increase the number of card in cauldron
		return;
	}
	int cauldronindex = 0;      //if there is a cauldron have this colour, it will become the cauldron index
	for (int i = 0; i < k; i++) //check every cauldron
	{
		for (int j = 0; j < Cauldrons[i].Number_CardsInCauldron; j++) //check every card in this cauldron whether there is a card's colour the same with moving one
		{
			if (strcmp(Players[activeplayer].Card_Hand[index_card_move].CardColour, Cauldrons[i].CardsInCauldron[j].CardColour) == 0) //if there is a same colour card
			{
				cauldronindex = i + 1;  //store the index
				break;                  //break
			}
		}
		if (cauldronindex != 0)         //if there is the card with same colour
		{
			break;                      //exit the loop
		}
	}

	if (cauldronindex != 0) //if there is a card with same colour, move card into it
	{
		Cauldrons[cauldronindex - 1].CardsInCauldron[Cauldrons[cauldronindex - 1].Number_CardsInCauldron] = Players[activeplayer].Card_Hand[index_card_move];//copy to the last
		Cauldrons[cauldronindex - 1].Number_CardsInCauldron++;  //increase the number of card in cauldron
	}
	else           //if there isn't this colour in cauldron, we can randomly input to an empty one(but with green is ok)
	{
		for (int i = 0; i < k; i++)                                //check cauldron one by one
		{
			int Number_Colour_NotGreen = 0;                        //if there is a colour without green, it cannot be put into the card
			for (int j = 0; j < Cauldrons[i].Number_CardsInCauldron; j++) //every card in this cauldron
			{
				if (strcmp("green", Cauldrons[i].CardsInCauldron[j].CardColour) != 0) //if there is a colour without green,i.e. there have been a type of card in it
				{
					Number_Colour_NotGreen++;
					break;
				}
			}
			if (Number_Colour_NotGreen == 0)                        //if there isn't other colour in it
			{
				cauldronindex = i + 1;
				break;
			}
		}
		if (cauldronindex >= 1)   //just used to remove the warning of visual studio, in fact, in the logic of the code, cauldronindex must larger than 0 here
		{
			Cauldrons[cauldronindex - 1].CardsInCauldron[Cauldrons[cauldronindex - 1].Number_CardsInCauldron] = Players[activeplayer].Card_Hand[index_card_move];//copy to the last
			Cauldrons[cauldronindex - 1].Number_CardsInCauldron++;
		}
	}
}


void ResortPlayerHandCard(int activeplayer)
{
	for (int i = 0; i < Players[activeplayer].Number_Hand - 1; i++)                                //move toward 1 step
	{
		Players[activeplayer].Card_Hand[i] = Players[activeplayer].Card_Hand[i + 1];               //one card euqal to next card
	}
	Players[activeplayer].Number_Hand--;                                                           //number of cards in hand decrease 1
	Players[activeplayer].Card_Hand[Players[activeplayer].Number_Hand].CardValue = 0;              //reinitialize the last card
	strcpy_s(Players[activeplayer].Card_Hand[Players[activeplayer].Number_Hand].CardColour, "0");
}


void EasyMove(int activeplayer)
{
	int Index_Card_Move = 0;

	MoveCardIntoCauldron(activeplayer-1, Index_Card_Move);

	ResortPlayerHandCard(activeplayer-1);
}


void OutputSituation()
{
	ActivePlayer = (ActivePlayer + n) % n + 1;            //because the last one will turn to the first one
	printf("active player = %d\nplayers number = %d\nexplosion threshold = %d", ActivePlayer, n, ExplodeThreshold);
	for (int i = 0; i < n; i++)                                            //output the player
	{
		printf("\n%d player hand cards: ", Players[i].Index_Player);
		for (int j = 0; j < Players[i].Number_Hand; j++)
		{
			printf("%d %s ", Players[i].Card_Hand[j].CardValue, Players[i].Card_Hand[j].CardColour);
		}
		printf("\n%d player deck cards: ", Players[i].Index_Player);
		for (int j = 0; j < Players[i].Number_Graveyard; j++)
		{
			printf("%d %s ", Players[i].Card_Graveyard[j].CardValue, Players[i].Card_Graveyard[j].CardColour);
		}
	}
	for (int i = 0; i < k; i++)                                              //output the cauldrons
	{
		printf("\n%d pile cards: ", Cauldrons[i].Index_Cauldron);
		for (int j = 0; j < Cauldrons[i].Number_CardsInCauldron; j++)
		{
			printf("%d %s ", Cauldrons[i].CardsInCauldron[j].CardValue, Cauldrons[i].CardsInCauldron[j].CardColour);
		}
	}
}


int main() {
	scanf_s("active player = %d\nplayers number = %d\nexplosion threshold = %d", &ActivePlayer, &n, &ExplodeThreshold);  //input the first and second lines

	GameScreenShots(n);                                     //input the situation

	EasyMove(ActivePlayer);                                 //move the first card of active player into the right pile

	OutputSituation();                                     //output the situation

	return 0;
}