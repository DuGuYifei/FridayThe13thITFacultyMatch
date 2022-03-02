#include<iostream>

FILE* GameFile; //Local file transfer protocol
char GameStatePath[200] = "E:/Gdansk University of Technology/first semester/Basics of computer programming/ProjectOfLecture/Project1-File_Version/GameFILE/GameState.txt";
char CheckStatePath[200] = "E:/Gdansk University of Technology/first semester/Basics of computer programming/ProjectOfLecture/Project1-File_Version/GameFILE/6-CheckState.txt";

const int Maxn = 20;       //The max number of player
const int MaxSize = 20;    //The max quantity of card of each colour in the game.
const int MaxColour = 7;   //The quatity of kind of colour in the game is 7,so I suppose the max of colour is 7 --- 0  green,1 blue,2 red,3 violet,4 yellow,5 white,6 black

int ActivePlayer;          //the active player's index
int n;                     //n-the amount of players
int k;                     //k-the amount of cauldrons.
int ExplosionThreshold;    //the explosion threshold

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
Player Players[Maxn];                          //array for players

typedef struct                                 //the cauldron has element of index, quantity, cards
{
	int Index_Cauldron;                        //index of cauldron
	int Number_CardsInCauldron;                //how many cards in the cauldron
	Card CardsInCauldron[MaxSize];             //used to store cards in cauldron
}Cauldron;
Cauldron Cauldrons[MaxColour-1];                         //array to store all cauldrons


int CountColours(int index_card, int kvalue)                                          //how many colours without green
{
	kvalue = 0;//the last colour in last card will not be count at first ,so it should count at last;
	for (int i = 0; i < index_card - 1; i++)                                //check every card with cards after it
	{
		if (strcmp(Deck[i].CardColour, "green") != 0)
		{
			int SameOrDifer = 0;                                                //if it will have the same colour, it will change into 1
			for (int j = i + 1; j < index_card; j++)                            //every card after the card being checked
			{
				if (strcmp(Deck[j].CardColour, "green") != 0)
				{
					if (strcmp(Deck[i].CardColour, Deck[j].CardColour) == 0)                   //if they are the same colour
					{
						SameOrDifer = 1;                                            //change it to 1
					}
				}
			}
			if (SameOrDifer == 0)                                               //if don't have the same colour after it
			{
				kvalue++;                                                       //count the colour
			}
		}
	}
	if (strcmp(Deck[index_card - 1].CardColour, "green") != 0)
	{
		kvalue++;
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
		char HandString[1000] = { 0 };                                      //used to store the card string of hand
		char FrontString[1000] = { 0 };									   //used to store the card string of front
		std::cin.getline(HandString, 1000);                                 //input the whole line

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

		std::cin.getline(FrontString, 1000);
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

		char PileString[1000] = { 0 };									   //used to store the card string of front
		std::cin.getline(PileString, 1000);                                 //input the whole line

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


int CheckPlayersHandCards(int number, int* ok)
/*Check Players Cards:
   1.cannot smaller than (all - out)/n ||  bigger than it +1;
   2.cannot appear situation like:44545(only appears when k >= 4)
	 i.e. x!=x-1, x = x-2, x-1 = x+1, and considering the first player and last player,we should use module--% */
{
	int SumNumberCauldron = 0;                      //how many cards in all cauldron
	for (int i = 0; i < k; i++)
	{
		SumNumberCauldron += Cauldrons[i].Number_CardsInCauldron;
	}
	int SumNumberGraveyard = 0;
	for (int i = 0; i < n; i++)                    //how many cards in graveyard
	{
		SumNumberGraveyard += Players[i].Number_Graveyard;
	}
	for (int i = 0; i < number; i++)
	{
		if (Players[i].Number_Hand < (Index_Card - SumNumberCauldron-SumNumberGraveyard) / n || Players[i].Number_Hand>(Index_Card - SumNumberCauldron-SumNumberGraveyard) / n + 1)
		{
			//cannot smaller than(all - out) / n || bigger than it + 1
			std::cout << "The number of players cards on hand is wrong\n";
			*ok = 1;
			return 0;
		}
		//cannot appear situation like:44545(only appears when k >= 4), i.e.x != x - 1, x = x - 2, x - 1 = x + 1, and considering the first player and last player, we should use module--%
		if (number >= 4)
		{
			if (Players[i].Number_Hand != Players[(i + number - 1) % number].Number_Hand && Players[i].Number_Hand == Players[(i + number - 2) % number].Number_Hand && Players[(i + number - 1) % number].Number_Hand == Players[(i + number + 1) % number].Number_Hand)
			{
				std::cout << "The number of players cards on hand is wrong\n";
				*ok = 1;
				return 0;
			}
		}
	}
	return 0;
}


void CheckColourInCauldrons(int* ok)
{
	for (int i = 0; i < k; i++)             //check every cauldron
	{
		int notgreen;                       //search the first card without green
		for (notgreen = 0; notgreen < Cauldrons[i].Number_CardsInCauldron; notgreen++)
		{
			if (strcmp(Cauldrons[i].CardsInCauldron[notgreen].CardColour, "green") != 0)
			{
				break;
			}
		}
		for (int j = notgreen + 1; j < Cauldrons[i].Number_CardsInCauldron; j++)   //use the first card's color to compare with other cards
		{
			if (strcmp(Cauldrons[i].CardsInCauldron[j].CardColour, "green") != 0) //skip green
			{
				if (strcmp(Cauldrons[i].CardsInCauldron[notgreen].CardColour, Cauldrons[i].CardsInCauldron[j].CardColour) != 0)    //if there are diffrent colours
				{
					printf("Two different colors were found on the %d pile\n", Cauldrons[i].Index_Cauldron);
					*ok = 1;
					break;
				}
			}
		}
	}
}


void CheckExplosion(int explosiontreshold, int* ok)                      //check whether it should explosion
{
	for (int i = 0; i < k; i++)
	{
		int SumCardsValuesInCauldron = 0;                   //sum up all values in this cauldron
		for (int j = 0; j < Cauldrons[i].Number_CardsInCauldron; j++)
		{
			SumCardsValuesInCauldron += Cauldrons[i].CardsInCauldron[j].CardValue;
		}
		if (SumCardsValuesInCauldron > explosiontreshold)      //if it is bigger than the threshold
		{
			printf("Pile number %d should explosion earlier\n", Cauldrons[i].Index_Cauldron);
			*ok = 1;
		}
	}
}


void Show_CheckState()                    //output the content of file into screen
{
	std::cin.clear();                 //clear the buffer
	freopen_s(&GameFile, CheckStatePath, "r", stdin);      //open the file
	char line[1000];                                      //store every line

	while (std::cin.getline(line, 1000))                  //while get a line of the file        
	{
		std::cerr << line << std::endl;                   //print in screen
	}

	fclose(stdin);	                      //close the file
}


int main() {
	freopen_s(&GameFile, GameStatePath, "r", stdin);        //use the file last program create

	scanf_s("active player = %d\nplayers number = %d\nexplosion threshold = %d\n", &ActivePlayer, &n, &ExplosionThreshold);  //input the first and second lines

	GameScreenShots(n);                                     //input the situation

	fclose(stdin);                                          //close the file

	freopen_s(&GameFile, CheckStatePath, "w", stdout);        //output to the file

	int OK = 0;                                             //if there is wrong,it will be 1

	CheckPlayersHandCards(n, &OK);                               //whether cards number in player hands is right

	CheckColourInCauldrons(&OK);                               //check colours in cauldron

	CheckExplosion(ExplosionThreshold, &OK);                         //check whether it should explosion

	if (OK == 0)                                                //if there isn't wrong
	{
		std::cout << "Current state of the game is ok";
	}

	fclose(stdout);                                         //close the file

	Show_CheckState();                               //display output in the screen

	return 0;
}
