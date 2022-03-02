#include<iostream>

FILE* GameFile; //Local file transfer protocol
char GameStatePath[200] = "E:/Gdansk University of Technology/first semester/Basics of computer programming/ProjectOfLecture/Project1-File_Version/GameFILE/GameState.txt";
char CheckCardsValuesPath[200] = "E:/Gdansk University of Technology/first semester/Basics of computer programming/ProjectOfLecture/Project1-File_Version/GameFILE/5-CheckCardsValues.txt";

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
Card BlueDeck[MaxSize], RedDeck[MaxSize], VioletDeck[MaxSize], YellowDeck[MaxSize], WhiteDeck[MaxSize], BlackDeck[MaxSize]; //used to classify the cards by colour
int Number_Each_Colour[MaxColour-1];                     //used to count the number of each colour and be the index of colour deck

typedef struct                                 //the player has elements of index,quantities,cards
{
	int Index_Player;                          //index of player
	int Number_Hand;                           //how many cards in hand
	int Number_Graveyard;                      //how many cards in graveyard
}Player;                                       //the player
Player Players[Maxn];                             //array for players

typedef struct                                 //the cauldron has element of index, quantity, cards
{
	int Index_Cauldron;                        //index of cauldron
	int Number_CardsInCauldron;                //how many cards in the cauldron
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


int CmpCardValue(const void* card1, const void* card2)                       //used to qsort the colour deck with its values
{
	return (*(Card*)card1).CardValue - (*(Card*)card2).CardValue;    //qsort from small to big.   return -,+,0
}

void SortColourDeck()     	//qsort colour deck by the CardValue
{
	qsort(BlueDeck, Number_Each_Colour[0], sizeof(Card), CmpCardValue);
	qsort(RedDeck, Number_Each_Colour[1], sizeof(Card), CmpCardValue);
	qsort(VioletDeck, Number_Each_Colour[2], sizeof(Card), CmpCardValue);
	qsort(YellowDeck, Number_Each_Colour[3], sizeof(Card), CmpCardValue);
	qsort(WhiteDeck, Number_Each_Colour[4], sizeof(Card), CmpCardValue);
	qsort(BlackDeck, Number_Each_Colour[5], sizeof(Card), CmpCardValue);
}

void ClassifyCards()                                        //classify cards
{
	for (int i = 0; i < MaxColour-1; i++)
	{
		Number_Each_Colour[i] = { 0 };                          //initialize all numbers 0;  0-blue,1-red,2-violet,3-yellow,4-white,5-black
	}
	for (int i = 0; i < Index_Card; i++)                         //check all cards in the deck
	{
		if (strcmp(Deck[i].CardColour, "blue") == 0)                  //increase each colour number respectively
		{
			BlueDeck[Number_Each_Colour[0]] = Deck[i];                //copy the card into right colour deck
			Number_Each_Colour[0]++;                                  //increase number and point to next one
		}
		else if (strcmp(Deck[i].CardColour, "red") == 0)          //below are the same
		{
			RedDeck[Number_Each_Colour[1]] = Deck[i];
			Number_Each_Colour[1]++;
		}
		else if (strcmp(Deck[i].CardColour, "violet") == 0)
		{
			VioletDeck[Number_Each_Colour[2]] = Deck[i];
			Number_Each_Colour[2]++;
		}
		else if (strcmp(Deck[i].CardColour, "yellow") == 0)
		{
			YellowDeck[Number_Each_Colour[3]] = Deck[i];
			Number_Each_Colour[3]++;
		}
		else if (strcmp(Deck[i].CardColour, "white") == 0)
		{
			WhiteDeck[Number_Each_Colour[4]] = Deck[i];
			Number_Each_Colour[4]++;
		}
		else if (strcmp(Deck[i].CardColour, "black") == 0)
		{
			BlackDeck[Number_Each_Colour[5]] = Deck[i];
			Number_Each_Colour[5]++;
		}
	}
}


int CheckCardsNumbers_ForCheckValue(int sameordifer)              //check if they are the same quantity of cards
{
	for (int i = 1; i < MaxColour-1; i++)                     //use blue number compare with other numbers
	{
		if (Number_Each_Colour[i] != 0)             //remove the colour which is not appear
		{
			if (Number_Each_Colour[0] != Number_Each_Colour[i])              //if there are different value
			{
				sameordifer = 1;
				break;                                                       //break out loop
			}
		}
	}
	return sameordifer;
}


int CheckValuesSame(int sameordifer)                  //check whether the values are same
{
	for (int i = 0; i < Number_Each_Colour[0]; i++)                     //for every blue card
	{
		if (Number_Each_Colour[1] != 0)                                 //if there is red card
		{
			if (RedDeck[i].CardValue != BlueDeck[i].CardValue)          //compare the red and blue in the same index
			{
				sameordifer = 1;                                        //sign of different
				break;                                                  //out the loop
			}
		}
		if (Number_Each_Colour[2] != 0)                                 //if there is violet card
		{
			if (VioletDeck[i].CardValue != BlueDeck[i].CardValue)
			{
				sameordifer = 1;
				break;
			}
		}
		if (Number_Each_Colour[3] != 0)                                 //yellow
		{
			if (YellowDeck[i].CardValue != BlueDeck[i].CardValue)
			{
				sameordifer = 1;
				break;
			}
		}
		if (Number_Each_Colour[4] != 0)                                  //white
		{
			if (WhiteDeck[i].CardValue != BlueDeck[i].CardValue)
			{
				sameordifer = 1;
				break;
			}
		}
		if (Number_Each_Colour[5] != 0)                                   //black
		{
			if (BlackDeck[i].CardValue != BlueDeck[i].CardValue)
			{
				sameordifer = 1;
				break;
			}
		}
	}
	return sameordifer;
}


void CheckCardsValues()                                  //check cards values and output
{
	ClassifyCards();                                //clasify the cards in Deck

	SortColourDeck();                               //qsort colour deck by the CardValue

	int SameOrDifer = 0;                            //if there are different numbers of cards, it will be 1
	SameOrDifer = CheckCardsNumbers_ForCheckValue(SameOrDifer);

	if (SameOrDifer == 0)                           //if there are no different numbers of cards, then compare their values
	{
		SameOrDifer = CheckValuesSame(SameOrDifer); //if there are different values, it will be 1
	}

	//OUTPUT
	if (SameOrDifer == 0)                              //if values are same,output these
	{
		std::cout << "The values of cards of all colors are identical:\n";
		for (int i = 0; i < Number_Each_Colour[0]; i++)
		{
			printf("%d ", BlueDeck[i].CardValue);
		}
	}
	else                                               //if values are different,output these
	{
		std::cout << "The values of cards of all colors are not identical:\n";
		std::cout << "blue cards values: ";
		for (int i = 0; i < Number_Each_Colour[0]; i++)  //output blue value
		{
			printf("%d ", BlueDeck[i].CardValue);
		}
		if (Number_Each_Colour[1] != 0)    //red
		{
			std::cout << "\nred cards values: ";
			for (int i = 0; i < Number_Each_Colour[1]; i++)
			{
				printf("%d ", RedDeck[i].CardValue);
			}
		}
		if (Number_Each_Colour[2] != 0)  //violet
		{
			std::cout << "\nviolet cards values: ";
			for (int i = 0; i < Number_Each_Colour[2]; i++)
			{
				printf("%d ", VioletDeck[i].CardValue);
			}
		}
		if (Number_Each_Colour[3] != 0)  //yellow
		{
			std::cout << "\nyellow cards values: ";
			for (int i = 0; i < Number_Each_Colour[3]; i++)
			{
				printf("%d ", YellowDeck[i].CardValue);
			}
		}
		if (Number_Each_Colour[4] != 0)  //white
		{
			std::cout << "\nwhite cards values: ";
			for (int i = 0; i < Number_Each_Colour[4]; i++)
			{
				printf("%d ", WhiteDeck[i].CardValue);
			}
		}
		if (Number_Each_Colour[5] != 0)  //black
		{
			std::cout << "\nblack cards values: ";
			for (int i = 0; i < Number_Each_Colour[5]; i++)
			{
				printf("%d ", BlackDeck[i].CardValue);
			}
		}
	}
}


void Show_CheckCardsValues()                    //output the content of file into screen
{
	std::cin.clear();                 //clear the buffer
	freopen_s(&GameFile, CheckCardsValuesPath, "r", stdin);      //open the file
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

	freopen_s(&GameFile, CheckCardsValuesPath, "w", stdout);        //output to the file

	CheckCardsValues();                                    //check cards numebers

	fclose(stdout);                                         //close the file

	Show_CheckCardsValues();                               //display output in the screen

	return 0;
}