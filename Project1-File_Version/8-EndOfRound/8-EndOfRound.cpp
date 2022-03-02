#include<iostream>

FILE* GameFile; //Local file transfer protocol
char GameStatePath[200] = "E:/Gdansk University of Technology/first semester/Basics of computer programming/ProjectOfLecture/Project1-File_Version/GameFILE/GameState.txt";
char GGPath[200] = "E:/Gdansk University of Technology/first semester/Basics of computer programming/ProjectOfLecture/Project1-File_Version/GameFILE/GG.txt";

const int Maxn = 20;       //The max number of player
const int MaxSize = 20;    //The max quantity of card of each colour in the game.
const int MaxColour = 7;   //The quatity of kind of colour in the game is 7,so I suppose the max of colour is 7 --- 0  green,1 blue,2 red,3 violet,4 yellow,5 white,6 black

int ActivePlayer;          //the active player's index
int n;                     //n-the amount of players
int k;                     //k-the amount of cauldrons.
int ExplosionThreshold;      //the explosion threshold

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
Player Players[Maxn];                             //array for players

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


int EndCheck()                                        //check the end of round
{
	for (int i = 0; i < n; i++)
	{
		if (Players[i].Number_Hand != 0)                    //check all players don't have cards on hand
		{
			return 1;                                        //have cards return 1
		}
	}
	return 0;                                               //no cards return 0
}


int WhoHasImmunityOfThisColour(char colour_tested[7])
{
	typedef struct                               //give an ID to every player to be tested
	{
		int Index_Player;
		int Quantity_SomeColour;                 //the quantity of some colour
	}ID_Qualify_Immunity;
	ID_Qualify_Immunity IDs_Waiting_Tested[Maxn];                   //store every player's ID of quanlification of immunity
	for (int i = 0; i < n; i++)                                     //produce ID for everyone
	{
		IDs_Waiting_Tested[i].Index_Player = i + 1;                 //input the index
		IDs_Waiting_Tested[i].Quantity_SomeColour = 0;              //initialize the quantity of this colour in one player's graveyard
		for (int j = 0; j < Players[i].Number_Graveyard; j++)       //check every card in the graveyard
		{
			if (strcmp(Players[i].Card_Graveyard[j].CardColour, colour_tested) == 0)   //if colour meet the requirement
			{
				IDs_Waiting_Tested[i].Quantity_SomeColour++;                            //count it
			}
		}
	}
	ID_Qualify_Immunity MaxID = IDs_Waiting_Tested[0];                               //get the max id, firstly initialize by the first element
	for (int i = 1; i < n; i++)
	{
		if (MaxID.Quantity_SomeColour < IDs_Waiting_Tested[i].Quantity_SomeColour)   //if bigger
		{
			MaxID = IDs_Waiting_Tested[i];                                           //change the max
		}
	}

	int HowManyMax = 0;                                                              //test if there are two same max value
	for (int i = 0; i < n; i++)                                                      //check every value
	{
		if (IDs_Waiting_Tested[i].Quantity_SomeColour == MaxID.Quantity_SomeColour)
		{
			HowManyMax++;
		}
	}
	if (HowManyMax == 1)                       //if there is oly one max
	{
		return MaxID.Index_Player;             //return who is max
	}
	else                                       //if there are atleast two diferent
	{
		return 0;                              //return 0 player
	}
}


int CaulculateScore(int playerindex, int blueimm, int redimm, int violetimm, int yellowimm, int whiteimm, int blackimm)
{
	char ImmunityColour[MaxColour][7] = { 0 };                   //first bracket is index, another is string to store the immunity colour
	int HowMany_immunitycolour = 0;
	if (blueimm == playerindex)                                    //if player immuny blue
	{
		strcpy_s(ImmunityColour[HowMany_immunitycolour], "blue"); //add en element "blue"
		HowMany_immunitycolour++;
	}
	if (redimm == playerindex)
	{
		strcpy_s(ImmunityColour[HowMany_immunitycolour], "red");  //red
		HowMany_immunitycolour++;
	}
	if (violetimm == playerindex)
	{
		strcpy_s(ImmunityColour[HowMany_immunitycolour], "violet");//violet
		HowMany_immunitycolour++;
	}
	if (yellowimm == playerindex)
	{
		strcpy_s(ImmunityColour[HowMany_immunitycolour], "yellow"); //yellow
		HowMany_immunitycolour++;
	}
	if (whiteimm == playerindex)
	{
		strcpy_s(ImmunityColour[HowMany_immunitycolour], "white");  //white
		HowMany_immunitycolour++;
	}
	if (blackimm == playerindex)
	{
		strcpy_s(ImmunityColour[HowMany_immunitycolour], "black"); //black
		HowMany_immunitycolour++;
	}

	int Score = 0;
	for (int i = 0; i < Players[playerindex - 1].Number_Graveyard; i++)           //check every card to score
	{
		int SameOrDifer = 0;                    //if player immunity this colour then not count the card into scores
		for (int j = 0; j < HowMany_immunitycolour; j++)   //Check this card's colour with every colour in the immunity array
		{
			if (strcmp(Players[playerindex - 1].Card_Graveyard[i].CardColour, ImmunityColour[j]) == 0)  //if there is a same colour
			{
				SameOrDifer = 1;         //change into 1
				break;
			}
		}
		if (SameOrDifer == 1)
		{
			continue;                     //if player immuny this colour ,  don't calculate this card
		}
		else                             //if not immuny
		{
			if (strcmp(Players[playerindex - 1].Card_Graveyard[i].CardColour, "green") == 0)   //if this card is green
			{
				Score += 2;                                                                   //get two points
			}
			else
			{
				Score++;                                                                       //not green, get 1 point
			}
		}
	}
	return Score;
}


void GameOver()                              //output the result
{
	char Colour_Tested[7] = "blue";                                              //check blue immunity players
	int BlueImmunityPlayer = WhoHasImmunityOfThisColour(Colour_Tested);
	if (BlueImmunityPlayer != 0)
	{
		printf("Na kolor blue odporny jest gracz %d\n", BlueImmunityPlayer);
	}
	strcpy_s(Colour_Tested, "red");                                               //red
	int RedImmunityPlayer = WhoHasImmunityOfThisColour(Colour_Tested);
	if (RedImmunityPlayer != 0)
	{
		printf("Na kolor red odporny jest gracz %d\n", RedImmunityPlayer);
	}
	strcpy_s(Colour_Tested, "violet");                                            //violet
	int VioletImmunityPlayer = WhoHasImmunityOfThisColour(Colour_Tested);
	if (VioletImmunityPlayer != 0)
	{
		printf("Na kolor violet odporny jest gracz %d\n", VioletImmunityPlayer);
	}
	strcpy_s(Colour_Tested, "yellow");                                             //yellow
	int YellowImmunityPlayer = WhoHasImmunityOfThisColour(Colour_Tested);
	if (YellowImmunityPlayer != 0)
	{
		printf("Na kolor yellow odporny jest gracz %d\n", YellowImmunityPlayer);
	}
	strcpy_s(Colour_Tested, "white");                                              //white
	int WhiteImmunityPlayer = WhoHasImmunityOfThisColour(Colour_Tested);
	if (WhiteImmunityPlayer != 0)
	{
		printf("Na kolor white odporny jest gracz %d\n", WhiteImmunityPlayer);
	}
	strcpy_s(Colour_Tested, "black");                                               //black
	int BlackImmunityPlayer = WhoHasImmunityOfThisColour(Colour_Tested);
	if (BlackImmunityPlayer != 0)
	{
		printf("Na kolor black odporny jest gracz %d\n", BlackImmunityPlayer);
	}

	for (int i = 1; i <= n; i++)             //count scores of every player
	{
		printf("Wynik gracza %d = %d\n", i, CaulculateScore(i, BlueImmunityPlayer, RedImmunityPlayer, VioletImmunityPlayer, YellowImmunityPlayer, WhiteImmunityPlayer, BlackImmunityPlayer));
	}
}


void Show_GG()                    //output the content of file into screen
{
	std::cin.clear();                 //clear the buffer
	freopen_s(&GameFile, GGPath, "r", stdin);      //open the file
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

	freopen_s(&GameFile, GGPath, "w", stdout);        //output to the file

	if (EndCheck() == 0)                                            //Check whether it is the end
	{
		GameOver();                                           //output the result
	}

	fclose(stdout);                                         //close the file

	Show_GG();                                       //display output in the screen

	return 0;
}