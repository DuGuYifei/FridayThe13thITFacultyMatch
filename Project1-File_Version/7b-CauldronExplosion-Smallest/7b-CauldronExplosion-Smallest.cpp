#include<iostream>

FILE* GameFile; //Local file transfer protocol
char GameStatePath[200] = "E:/Gdansk University of Technology/first semester/Basics of computer programming/ProjectOfLecture/Project1-File_Version/GameFILE/GameState.txt";

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

typedef struct                                 //the card used to play
{
	Card BasicInfo;                            //battle card value and colour
	int BattleCauldron;                        //which pile canbe used
	int IndexInPlayerHand;                     //this card is No.? in player's hand
}BattleCard;
BattleCard BattleDeck[MaxSize * MaxColour];    //store the Battle card that can be consider to use which one
int Index_BattleCard;                          //the index of card in battle deck. also can be used to count how many card in it

typedef struct                                 //the player has elements of index,quantities,cards
{
	int Index_Player;                          //index of player
	int Number_Hand;                           //how many cards in hand
	int Number_Graveyard;                      //how many cards in graveyard
	Card Card_Hand[MaxSize * MaxColour];       //store cards in hand
	Card Card_Graveyard[MaxSize * MaxColour];  //store cards in front
}Player;                                       //the player
Player Players[Maxn];                          //array for players

typedef struct                                 //the cauldron has element of index, quantity, cards
{
	int Index_Cauldron;                        //index of cauldron
	int Number_CardsInCauldron;                //how many cards in the cauldron
	Card CardsInCauldron[MaxSize * MaxColour];             //used to store cards in cauldron

	char PileColour[7];                                   //it can be empty, green, other colour
	int SumValue;                                         //store the sum value of this pile
}Cauldron;
Cauldron Cauldrons[MaxColour - 1];                         //array to store all cauldrons


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
		strcpy_s(Cauldrons[i].PileColour, "empty");                        //initialize with empty
		Cauldrons[i].SumValue = 0;                                         //initialize sum value with 0
		Cauldrons[i].Number_CardsInCauldron = 0;
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
				Cauldrons[i].SumValue += atoi(cardvalue);                  //add the value to sum
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
				if (strcmp(cardcolour, Cauldrons[i].PileColour) != 0)
				{
					if (strcmp(Cauldrons[i].PileColour, "empty") == 0 || strcmp(Cauldrons[i].PileColour, "green") == 0)
					{
						strcpy_s(Cauldrons[i].PileColour, cardcolour);
					}
				}
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


void MoveCardIntoCauldron(int activeplayer, int index_card_move, int index_cauldron_wheremoveto) //move to last one of caudron
{
	Cauldrons[index_cauldron_wheremoveto - 1].CardsInCauldron[Cauldrons[index_cauldron_wheremoveto - 1].Number_CardsInCauldron] = Players[activeplayer].Card_Hand[index_card_move];
	Cauldrons[index_cauldron_wheremoveto - 1].Number_CardsInCauldron++;
	Cauldrons[index_cauldron_wheremoveto - 1].SumValue += Players[activeplayer].Card_Hand[index_card_move].CardValue;
}


void ResortPlayerHandCard(int activeplayer, int index_card_move)
{
	for (int i = index_card_move; i < Players[activeplayer].Number_Hand - 1; i++)                  //move toward 1 step
	{
		Players[activeplayer].Card_Hand[i] = Players[activeplayer].Card_Hand[i + 1];               //one card euqal to next card
	}
	Players[activeplayer].Number_Hand--;                                                           //number of cards in hand decrease 1
	Players[activeplayer].Card_Hand[Players[activeplayer].Number_Hand].CardValue = 0;              //reinitialize the last card
	strcpy_s(Players[activeplayer].Card_Hand[Players[activeplayer].Number_Hand].CardColour, "0");
}


void ForgeBattleDeck(int activeplayer)
{
	Index_BattleCard = 0; //initialize with 0
	for (int i = 0; i < Players[activeplayer].Number_Hand; i++)   //check every card whether can be used, i.e. not explode
	{
		int Exist_RightPile = 0;                                  //if there is a pile can be used, it will change into 1
		int GreenBeInBattleDeck = 0;//if have been find a pile can be added the green card, it will be 1;it is used to decide whether should compare the sumvalue of cauldron
		for (int j = 0; j < k; j++)                                      //check with right pile                              
		{
			if (strcmp(Players[activeplayer].Card_Hand[i].CardColour, "green") == 0)  //if this card is green
			{
				if ((Players[activeplayer].Card_Hand[i].CardValue + Cauldrons[j].SumValue) <= ExplosionThreshold)  //if the card can be used
				{
					if (GreenBeInBattleDeck == 0) 
					{
						BattleDeck[Index_BattleCard].BasicInfo = Players[activeplayer].Card_Hand[i];                 // store card in to battle deck
						BattleDeck[Index_BattleCard].BattleCauldron = j + 1;                                         //store the cauldron can be used
						BattleDeck[Index_BattleCard].IndexInPlayerHand = i;                                          //store the index that the card in player's hand
						Index_BattleCard++;
						Exist_RightPile = 1;                                                                         //there is a pile can store this card
						GreenBeInBattleDeck = 1;
					}
					if (GreenBeInBattleDeck != 0)                                                                    //if we have chosen a battle pile for the green card
					{
						if (Cauldrons[BattleDeck[Index_BattleCard - 1].BattleCauldron - 1].Number_CardsInCauldron > Cauldrons[j].Number_CardsInCauldron)//choose the smaller quantity pile
						{
							BattleDeck[Index_BattleCard - 1].BattleCauldron = j + 1;
						}
					}
				}
			}
			else           //if the card is not green
			{
				if (strcmp(Players[activeplayer].Card_Hand[i].CardColour, Cauldrons[j].PileColour) == 0)  //if there is a pile with same colour
				{
					if ((Players[activeplayer].Card_Hand[i].CardValue + Cauldrons[j].SumValue) <= ExplosionThreshold)  //if the card can be used
					{
						BattleDeck[Index_BattleCard].BasicInfo = Players[activeplayer].Card_Hand[i];
						BattleDeck[Index_BattleCard].BattleCauldron = j + 1;
						BattleDeck[Index_BattleCard].IndexInPlayerHand = i;                                      //store the index that the card in player's hand
						Index_BattleCard++;
					}
					Exist_RightPile = 1;                                    //there is a cauldron with same colour
					break;
				}
			}
		}
		if (Exist_RightPile == 0)                //this card is green which will explode every pile or a card with colour that no pile have same colour
		{
			if (strcmp(Players[activeplayer].Card_Hand[i].CardColour, "green") != 0) //if the card is  not green, there must be a empty caudron(with green is OK)
			{
				int BeInBattleDeck = 0;//if have been find a pile can be added, it will be 1;it is used to decide whether should compare the sumvalue of cauldron
				for (int j = 0; j < k; j++)                                                                  //check which cauldron can be used
				{
					if (strcmp(Cauldrons[j].PileColour, "empty") == 0|| strcmp(Cauldrons[j].PileColour, "green") == 0)  //if the cauldron is green or empty
					{
						if ((Players[activeplayer].Card_Hand[i].CardValue + Cauldrons[j].SumValue) <= ExplosionThreshold)  //if the card can be used
						{
							if (BeInBattleDeck == 0)  //if it has not add a suitable pile
							{
								BattleDeck[Index_BattleCard].BasicInfo = Players[activeplayer].Card_Hand[i];                 //the card can be used to battle
								BattleDeck[Index_BattleCard].IndexInPlayerHand = i;                                          //store the index that the card in player's hand
								BattleDeck[Index_BattleCard].BattleCauldron = j + 1;                                         //this battle card can be moved here
								Index_BattleCard++;
								BeInBattleDeck = 1;
							}
							else                       //if it has add a suitable pile
							{
								if (Cauldrons[BattleDeck[Index_BattleCard - 1].BattleCauldron - 1].SumValue > Cauldrons[j].SumValue) //but new one have a smaller sum value
								{
									BattleDeck[Index_BattleCard - 1].BattleCauldron = j + 1; //change the battle cauldron to the new one
								}
							}
						}
					}
				}
			}
		}
	}
}


int SearchRightCauldron(BattleCard Medium)
{
	if (strcmp(Medium.BasicInfo.CardColour, "green") == 0)         //if the card is green
	{
		Medium.BattleCauldron = 1;                                 //put from the first pile
		int NumberTest = Cauldrons[0].Number_CardsInCauldron;      //how many card in the cauldron
		for (int i = 1; i < k; i++)                                //test every pile
		{
			if (NumberTest > Cauldrons[i].Number_CardsInCauldron)  //if there are less card
			{
				Medium.BattleCauldron = i + 1;                     //put card here
				NumberTest = Cauldrons[i].Number_CardsInCauldron;
			}
		}
	}
	else                                                         //if card is not green
	{
		int Exist_RightPile = 0;                                  //if there is a pile with same colour, it will change into 1
		for (int i = 0; i < k; i++)                              //test every pile whether have same colour
		{
			if (strcmp(Medium.BasicInfo.CardColour, Cauldrons[i].PileColour) == 0)
			{
				Medium.BattleCauldron = i + 1;
				Exist_RightPile = 1;
				break;
			}
		}
		if (Exist_RightPile == 0)                               //if there isn't same colour pile
		{
			int NumberTest = 2 * MaxSize;                       //there must be a empty or green one
			for (int i = 0; i < k; i++)                         //search a empty or green who has the least card quantity
			{
				if (strcmp(Cauldrons[i].PileColour, "green") == 0 || strcmp(Cauldrons[i].PileColour, "empty") == 0)
				{
					if (Cauldrons[i].Number_CardsInCauldron < NumberTest)
					{
						NumberTest = Cauldrons[i].Number_CardsInCauldron;
						Medium.BattleCauldron = i + 1;
					}
				}
			}
		}
	}
	return Medium.BattleCauldron;
}


BattleCard ChooseBiggestCard(int activeplayer)                                 //choose biggest card and smallest quantity of pile
{
	BattleCard Medium;                                                           //store the biggest card
	Medium.BasicInfo = Players[activeplayer].Card_Hand[0];                       //initialize the biggest card is the first card in hand
	Medium.IndexInPlayerHand = 0;
	Medium.BattleCauldron = SearchRightCauldron(Medium);

	for (int i = 1; i < Players[activeplayer].Number_Hand; i++)                  //check every hand card
	{
		if (Players[activeplayer].Card_Hand[i].CardValue > Medium.BasicInfo.CardValue)     //if meet bigger value
		{
			Medium.BasicInfo = Players[activeplayer].Card_Hand[i];                         //change the biggest battle card
			Medium.IndexInPlayerHand = i;
			Medium.BattleCauldron = SearchRightCauldron(Medium);
		}
		if (Players[activeplayer].Card_Hand[i].CardValue == Medium.BasicInfo.CardValue)    //if meet same value
		{
			BattleCard Medium2;
			Medium2.BasicInfo = Players[activeplayer].Card_Hand[i];
			Medium2.IndexInPlayerHand = i;
			Medium2.BattleCauldron = SearchRightCauldron(Medium2);
			if (Cauldrons[Medium.BattleCauldron-1].Number_CardsInCauldron > Cauldrons[SearchRightCauldron(Medium2)-1].Number_CardsInCauldron) //&&the less card number in the pile
			{
				Medium = Medium2;
			}
		}
	}
	return Medium;
}


BattleCard ChooseSmallestCard()    //choose the smallest and smallest value of caudron
{
	BattleCard medium = BattleDeck[0];   //initialize with the first card in battle deck, and it will change if meet more suitable
	for (int i = 1; i < Index_BattleCard; i++)  //compare to every card in battle deck
	{
		if (BattleDeck[i].BasicInfo.CardValue < medium.BasicInfo.CardValue)    //if meet smaller one
		{
			medium = BattleDeck[i];                                            //store the new
		}
		if (BattleDeck[i].BasicInfo.CardValue == medium.BasicInfo.CardValue)   //if meet the same value card
		{
			if (Cauldrons[BattleDeck[i].BattleCauldron - 1].SumValue < Cauldrons[medium.BattleCauldron - 1].SumValue)  //compare the caudrons value
			{
				medium = BattleDeck[i];
			}
		}
	}
	return medium;
}


void CauldronExplosion(int index_cauldron_wheremoveto, int activeplayer)
{
	if (Cauldrons[index_cauldron_wheremoveto].SumValue > ExplosionThreshold)                                                    //if it is bigger than threshold
	{
		for (int i = 0; i < Cauldrons[index_cauldron_wheremoveto].Number_CardsInCauldron; i++)                //for every card in this pile
		{
			Players[activeplayer].Card_Graveyard[Players[activeplayer].Number_Graveyard] = Cauldrons[index_cauldron_wheremoveto].CardsInCauldron[i];   //copy into the graveyard
			Players[activeplayer].Number_Graveyard++;                               //increase the graveyard
			Cauldrons[index_cauldron_wheremoveto].CardsInCauldron[i].CardValue = 0;        //reinitialize the pile
			strcpy_s(Cauldrons[index_cauldron_wheremoveto].CardsInCauldron[i].CardColour, "0");
		}
		Cauldrons[index_cauldron_wheremoveto].Number_CardsInCauldron = 0;                   //reinitialize the pile
		Cauldrons[index_cauldron_wheremoveto].SumValue = 0;
		strcpy_s(Cauldrons[index_cauldron_wheremoveto].PileColour, "empty");
	}
}


void EasyMoveAndCauldronExplosion(int activeplayer)
{
	if (Players[activeplayer - 1].Number_Hand == 0) //if no hand card then skip him
	{
		return;
	}
    ForgeBattleDeck(activeplayer - 1);             //forge a battle deck that store the card can be used (not explode) PS: for choosing smallest cauldron, when it should be put into emty or green,we should add some codes here

	//SMALLEST choose a card(can not cause explode, if explode, choose the biggest one)
	int Index_Card_Move;
	int Index_Cauldron_WhereMoveTo;
	if (Index_BattleCard != 0)                     //if there are cards can not cause explode
	{
		BattleCard mediumBC = ChooseSmallestCard();      //move which card is smallest(PS: there have been some code for choosing smallest cauldron changed above)
		Index_Card_Move = mediumBC.IndexInPlayerHand;
		Index_Cauldron_WhereMoveTo = mediumBC.BattleCauldron;
	}
	else                                           //if every card will cause explode
	{
		BattleCard  mediumBC = ChooseBiggestCard(activeplayer - 1);        //move biggest one and least card in the pile
		Index_Card_Move = mediumBC.IndexInPlayerHand;
		Index_Cauldron_WhereMoveTo = mediumBC.BattleCauldron;
	}

	MoveCardIntoCauldron(activeplayer - 1, Index_Card_Move, Index_Cauldron_WhereMoveTo); //move card into cauldron

	ResortPlayerHandCard(activeplayer - 1, Index_Card_Move);                              //resort the player hand card

	CauldronExplosion(Index_Cauldron_WhereMoveTo-1, activeplayer - 1);
}


void OutputSituation()
{
	ActivePlayer = (ActivePlayer + n) % n + 1;            //because the last one will turn to the first one
	printf("active player = %d\nplayers number = %d\nexplosion threshold = %d", ActivePlayer, n, ExplosionThreshold);
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


void Show_GameState()                    //output the content of file into screen
{
	std::cin.clear();                 //clear the buffer
	freopen_s(&GameFile, GameStatePath, "r", stdin);      //open the file
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

	freopen_s(&GameFile, GameStatePath, "w", stdout);        //output to the file

	EasyMoveAndCauldronExplosion(ActivePlayer);             //move the first card of active player into the right pile

	OutputSituation();                                      //output the situation

	fclose(stdout);                                         //close the file

	Show_GameState();                                       //display output in the screen

	return 0;
}