#include<iostream>
#include<Windows.h>

FILE* GameFile;         //Local file transfer protocol
char DeckPath[200] = "E:/Gdansk University of Technology/first semester/Basics of computer programming/ProjectOfLecture/Project1-FinalProject/GameFILE/Deck.txt";
char GameParameterPath[200] = "E:/Gdansk University of Technology/first semester/Basics of computer programming/ProjectOfLecture/Project1-FinalProject/GameFILE/1-GameParameter.txt";
char GameStatePath[200] = "E:/Gdansk University of Technology/first semester/Basics of computer programming/ProjectOfLecture/Project1-FinalProject/GameFILE/GameState.txt";
char LoadGamePath[200] = "E:/Gdansk University of Technology/first semester/Basics of computer programming/ProjectOfLecture/Project1-FinalProject/GameFILE/3-GameLoad.txt";
char CheckGreenCardsPath[200] = "E:/Gdansk University of Technology/first semester/Basics of computer programming/ProjectOfLecture/Project1-FinalProject/GameFILE/4a-CheckGreenCard.txt";
char CheckCardsNumbersPath[200] = "E:/Gdansk University of Technology/first semester/Basics of computer programming/ProjectOfLecture/Project1-FinalProject/GameFILE/4b-CheckCardsNumbers.txt";
char CheckCardsValuesPath[200] = "E:/Gdansk University of Technology/first semester/Basics of computer programming/ProjectOfLecture/Project1-FinalProject/GameFILE/5-CheckCardsValues.txt";
char CheckStatePath[200] = "E:/Gdansk University of Technology/first semester/Basics of computer programming/ProjectOfLecture/Project1-FinalProject/GameFILE/6-CheckState.txt";
char GGPath[200] = "E:/Gdansk University of Technology/first semester/Basics of computer programming/ProjectOfLecture/Project1-FinalProject/GameFILE/GG.txt";

const int Maxn = 20;      //The max number of player //support 20 player
const int MaxSize = 50;   //The max quantity of card of other colour in the game.The STOS system will test the code for max o=20, so I change here into 20.//for 20 player, change into 50
const int MaxColour = 7;  //The quatity of kind of colour in the game is 7,so I suppose the max of colour is 7 --- 0  green,1 blue,2 red,3 violet,4 yellow,5 white,6 black

int ActivePlayer;         //who is the active player
int n;                    //n-the amount of players
int k;                    //k-the number of cauldrons corresponding to a number of colors of cards(without green)
int g;                    //g-a number of green cards
int gv;                   //gv-a value of green cards
int o;                    //o-a number of cards for each other color except green                                  
int ALLvalue[MaxSize];    //used to store the value of card(without green)                        
int ExplosionThreshold;   //the explosion threshold

typedef struct                                 //the card has elements of value and colour
{
	int CardValue;
	char CardColour[7];                        //used to store colour
}Card;
Card Deck[MaxColour * MaxSize];                //array deck used to store cards
int Index_Card;                                //used to input card into deck
Card BlueDeck[MaxSize], RedDeck[MaxSize], VioletDeck[MaxSize], YellowDeck[MaxSize], WhiteDeck[MaxSize], BlackDeck[MaxSize]; //used to classify the cards by colour
int Number_Each_Colour[MaxColour - 1];         //used to count the number of each colour and be the index of colour deck

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
	Card CardsInCauldron[MaxSize * MaxColour]; //used to store cards in cauldron

	char PileColour[7];                        //it can be empty, green, other colour
	int SumValue;                              //store the sum value of this pile
}Cauldron;
Cauldron Cauldrons[MaxColour - 1];             //array to store all cauldrons


//1-Deck Generation
void inputALLvalue(int valueo)                 //input the value of each card in one colour(without green)----input of second line
{
	for (int i = 0; i < valueo; i++)
	{
		std::cin >> ALLvalue[i];
	}
}


void EstimateValue(int valuen, int valuek, int valueg, int valuegv, int valueo)  //estimate whether the value exceed the requirement
{
	while (valuen < 2 || valuen > Maxn)                                            //estimate n, input n again if n is not conform to the requirement 
	{
		std::cerr << "2<=n<=" << Maxn << " input n again" << std::endl;
		std::cin >> n;
		valuen = n;
	}
	while (valuek < 1 || valuek > 6)                                             //estimate k, input k again if k is not conform to the requirement 
	{
		std::cout << "1<=k<=6,input k again" << std::endl;
		std::cin >> k;
		valuek = k;
	}

	while (valueg < 1 || valueg > 20)//estimate g
	{
		std::cout << "1<=g<=20,input g again" << std::endl;
		std::cin >> g;
		valueg = g;
	}

	while (valuegv < 1 || valuegv > 10) //estimate gv
	{
		std::cout << "1<=gv<=10,input gv again" << std::endl;
		std::cin >> gv;
		valuegv = gv;
	}

	while (valueo < 1 || valueo > MaxSize) //estimate o
	{
		std::cout << "1<=o<=" << MaxSize << ",input o again" << std::endl;
		std::cin >> o;
		valueo = o;
	}
}


void sortALLvalue(int valueo)                                                 //sort the value to make sure that cards arranged in non-decreasing order
{
	for (int i = 0; i < valueo - 1; i++)                                      //use every number      
	{
		for (int j = i + 1; j < valueo; j++)                                  //to compare to every number behind it
		{
			if (ALLvalue[i] > ALLvalue[j])                                    //exchange them if the number is bigger than the number behind it
			{
				int exchange = ALLvalue[i];
				ALLvalue[i] = ALLvalue[j];
				ALLvalue[j] = exchange;
			}
		}
	}
}

void inputcardcolour(char colourname[7], char CardColour[7]) {
	int j = 0;
	while (colourname[j] != '\0')
	{
		CardColour[j] = colourname[j];
		j++;
	}
	CardColour[j] = '\0';
}

void ForgeGreen(int valueg, int valuegv)          //Forge the card of green to the Deck;
{
	for (int i = 0; i < valueg; i++)
	{
		Deck[i].CardValue = valuegv;
		char colourname[7] = "green";
		inputcardcolour(colourname, Deck[i].CardColour);
	}
}

void ForgeBlue(int valueo, int valueg)             //Forge Blue Card
{
	for (int i = valueg; i < valueg + valueo; i++)
	{
		Deck[i].CardValue = ALLvalue[i - valueg];
		char colourname[7] = "blue";
		inputcardcolour(colourname, Deck[i].CardColour);
	}
}

void ForgeRed(int valueo, int valueg)               //Forge Red Card
{
	for (int i = valueg + valueo; i < valueg + 2 * valueo; i++)
	{
		Deck[i].CardValue = ALLvalue[i - valueg - valueo];
		char colourname[7] = "red";
		inputcardcolour(colourname, Deck[i].CardColour);
	}
}

void ForgeViolet(int valueo, int valueg)            //Forge Violet Card
{
	for (int i = valueg + 2 * valueo; i < valueg + 3 * valueo; i++)
	{
		Deck[i].CardValue = ALLvalue[i - valueg - 2 * valueo];
		char colourname[7] = "violet";
		inputcardcolour(colourname, Deck[i].CardColour);
	}
}

void ForgeYellow(int valueo, int valueg)           //Forge Yellow Card
{
	for (int i = valueg + 3 * valueo; i < valueg + 4 * valueo; i++)
	{
		Deck[i].CardValue = ALLvalue[i - valueg - 3 * valueo];
		char colourname[7] = "yellow";
		inputcardcolour(colourname, Deck[i].CardColour);
	}
}

void ForgeWhite(int valueo, int valueg)           //Forge White Card
{
	for (int i = valueg + 4 * valueo; i < valueg + 5 * valueo; i++)
	{
		Deck[i].CardValue = ALLvalue[i - valueg - 4 * valueo];
		char colourname[7] = "white";
		inputcardcolour(colourname, Deck[i].CardColour);
	}
}

void ForgeBlack(int valueo, int valueg)          //Forge Black Card
{
	for (int i = valueg + 5 * valueo; i < valueg + 6 * valueo; i++)
	{
		Deck[i].CardValue = ALLvalue[i - valueg - 5 * valueo];
		char colourname[7] = "black";
		inputcardcolour(colourname, Deck[i].CardColour);
	}
}

void ForgeDeck(int valuek, int valueg, int valuegv, int valueo)    //forge the deck
{
	ForgeGreen(valueg, valuegv);                                   //forge green card
	for (int i = 0; i < valuek; i++)                               //forge other cards as the quantity of "k",which means how many other colours.
	{
		switch (i)
		{
		case 0:
			ForgeBlue(valueo, valueg);
			break;
		case 1:
			ForgeRed(valueo, valueg);
			break;
		case 2:
			ForgeViolet(valueo, valueg);
			break;
		case 3:
			ForgeYellow(valueo, valueg);
			break;
		case 4:
			ForgeWhite(valueo, valueg);
			break;
		case 5:
			ForgeBlack(valueo, valueg);
			break;
		}
	}
}


void Show_Deck()                    //output the content of file into screen
{
	freopen_s(&GameFile, DeckPath, "r", stdin);          //open the file
	char line[5000];                                     //store every line

	std::cerr << std::endl;
	while (std::cin.getline(line, 5000))                 //while get a line of the file        
	{
		std::cerr << line << std::endl;                  //print in screen
	}

	fclose(stdin);	                      //close the file
}


void StoreGameParameter()
{
	freopen_s(&GameFile, GameParameterPath, "w+", stdout);//open file, and make output into the deck file
	printf("%d %d %d %d %d ", n, k, g, gv, o);
	for (int i = 0; i < o; i++)
	{
		std::cout << ALLvalue[i] << " ";
	}
	printf("%d %d", ExplosionThreshold, ActivePlayer);
	fclose(stdout);
}
//1-deck generation

//2-Deal Cards(shuffle)
void DealCardsForPlayer(int valuen, int valueplayerindex)  //deal the cards for one player
{
	for (valueplayerindex; valueplayerindex <= k * o + g; valueplayerindex = valueplayerindex + valuen)         //There're ko+g cards.And player gets one card per n£¨how many players) times.
	{
		std::cout << Deck[valueplayerindex - 1].CardValue << " " << Deck[valueplayerindex - 1].CardColour << " ";  //index of player starts from 1, so it need minus 1.
	}
}

void DealCards(int valuen)               //deal the cards
{
	for (int PlayerIndex = 1; PlayerIndex <= n; PlayerIndex++) //output the deck in order
	{
		std::cout << PlayerIndex << " player hand cards: ";                    //output "index player hand cards: "        
		DealCardsForPlayer(valuen, PlayerIndex);                                 //deal the cards
		std::cout << std::endl << PlayerIndex << " player deck cards:" << std::endl;  //output "index player deck cards: "
	}
}


void Show_GameState()                    //output the content of file into screen
{
	std::cin.clear();                  //clear stream
	freopen_s(&GameFile, GameStatePath, "r", stdin);      //open the file
	char line[1000];                                      //store every line

	while (std::cin.getline(line, 1000))                  //while get a line of the file        
	{
		std::cerr << line << std::endl;                   //print in screen
	}

	fclose(stdin);	                      //close the file
}


void Shuffle()
{
	srand((int)time(0));
	for (int exchange1 = 0; exchange1 < o * k + g; exchange1++)
	{
		int exchange2 = rand() % (o * k + g);
		Card Medium;
		Medium = Deck[exchange1];
		Deck[exchange1] = Deck[exchange2];
		Deck[exchange2] = Medium;
	}
}
//2-deal cards(shuffle)

//7-Play cards
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
		for (int j = 0; j < k; j++)                                      //check with right pile                              
		{
			if (strcmp(Players[activeplayer].Card_Hand[i].CardColour, "green") == 0)  //if this card is green
			{
				if ((Players[activeplayer].Card_Hand[i].CardValue + Cauldrons[j].SumValue) <= ExplosionThreshold)  //if the card can be used
				{
					BattleDeck[Index_BattleCard].BasicInfo = Players[activeplayer].Card_Hand[i];                 // store card in to battle deck
					BattleDeck[Index_BattleCard].BattleCauldron = j + 1;                                         //store the cauldron can be used
					BattleDeck[Index_BattleCard].IndexInPlayerHand = i;                                          //store the index that the card in player's hand
					Index_BattleCard++;
					Exist_RightPile = 1;                                  //there is a pile can store this card
					break;
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
				for (int j = 0; j < k; j++)                                                                  //check which cauldron can be used
				{
					if (strcmp(Cauldrons[j].PileColour, "green") == 0 || strcmp(Cauldrons[j].PileColour, "empty") == 0)  //if the cauldron is green or empty
					{
						if ((Players[activeplayer].Card_Hand[i].CardValue + Cauldrons[j].SumValue) <= ExplosionThreshold)  //if the card can be used
						{
							BattleDeck[Index_BattleCard].BasicInfo = Players[activeplayer].Card_Hand[i];                 //the card can be used to battle
							BattleDeck[Index_BattleCard].IndexInPlayerHand = i;                                          //store the index that the card in player's hand
							BattleDeck[Index_BattleCard].BattleCauldron = j + 1;                                         //this battle card can be moved here
							Index_BattleCard++;
							break;
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
		if (Exist_RightPile == 0)                              //if there isn't same colour pile
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
			if (Cauldrons[Medium.BattleCauldron - 1].Number_CardsInCauldron > Cauldrons[SearchRightCauldron(Medium2) - 1].Number_CardsInCauldron) //&&the less card number in the pile
			{
				Medium = Medium2;
			}
		}
	}
	return Medium;
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
	ForgeBattleDeck(activeplayer - 1);             //forge a battle deck that store the card can be used (not explode)

	//RANDOMLY choose a card(can not cause explode, if explode, choose the biggest one)
	srand((int)time(0));
	int Index_Card_Move;
	int Index_Cauldron_WhereMoveTo;
	if (Index_BattleCard != 0)                     //if there are cards can not cause explode
	{
		BattleCard mediumBC = BattleDeck[rand() % Index_BattleCard];      //move which card randomly
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

	CauldronExplosion(Index_Cauldron_WhereMoveTo - 1, activeplayer - 1);
}


void OutputSituation()
{
	std::cerr << "I'm Player " << ActivePlayer << ", after my playing it will be" << std::endl;
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
//7-move and explode

//8-Check end and print result
int EndCheck()                                        //check the end of round
{
	std::cin.clear();                                //clear the buffer
	freopen_s(&GameFile, GameStatePath, "r", stdin);        //use the file last program create

	scanf_s("active player = %d\nplayers number = %d\nexplosion threshold = %d\n", &ActivePlayer, &n, &ExplosionThreshold);  //input the first and second lines

	GameScreenShots(n);                                     //input the situation

	fclose(stdin);                                          //close the file

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
	IDs_Waiting_Tested[0].Index_Player = -1;                        //initialize, otherwise visual studio will give warning
	IDs_Waiting_Tested[0].Quantity_SomeColour = -1;
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
//8-end of round

//3-load game
void DeckTracker(int number)                               //cout the deck
{
	//print the quantity of cards in players
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


void Show_LoadGame()                    //output the content of file into screen
{
	std::cin.clear();                 //clear the buffer
	freopen_s(&GameFile, LoadGamePath, "r", stdin);      //open the file
	char line[1000];                                      //store every line

	while (std::cin.getline(line, 1000))                  //while get a line of the file        
	{
		std::cerr << line << std::endl;                   //print in screen
	}

	fclose(stdin);	                      //close the file
}
//3-load game

//4a-check green  cards
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


void Show_CheckGreenCards()                    //output the content of file into screen
{
	std::cin.clear();                 //clear the buffer
	freopen_s(&GameFile, CheckGreenCardsPath, "r", stdin);      //open the file
	char line[1000];                                      //store every line

	while (std::cin.getline(line, 1000))                  //while get a line of the file        
	{
		std::cerr << line << std::endl;                   //print in screen
	}
	fclose(stdin);	                      //close the file
}
//4a-check green cards

//4b-check cards number
void CheckCardsNumbers()                                  //check green and out put
{
	int Number_Each_Colour[MaxColour - 1] = { 0 };                        //initialize all numbers 0;  0-blue,1-red,2-violet,3-yellow,4-white,5-black
	for (int i = 0; i < Index_Card; i++)                         //check all cards in the deck
	{
		if (strcmp(Deck[i].CardColour, "blue") == 0)                  //increase each colour number respectively
		{
			Number_Each_Colour[0]++;
		}
		else if (strcmp(Deck[i].CardColour, "red") == 0)
		{
			Number_Each_Colour[1]++;
		}
		else if (strcmp(Deck[i].CardColour, "violet") == 0)
		{
			Number_Each_Colour[2]++;
		}
		else if (strcmp(Deck[i].CardColour, "yellow") == 0)
		{
			Number_Each_Colour[3]++;
		}
		else if (strcmp(Deck[i].CardColour, "white") == 0)
		{
			Number_Each_Colour[4]++;
		}
		else if (strcmp(Deck[i].CardColour, "black") == 0)
		{
			Number_Each_Colour[5]++;
		}
	}
	//output
	int SameOrDifer = 0;                               //if there are different numbers. it will be 1
	for (int i = 1; i < MaxColour - 1; i++)            //use blue number compare with other numbers
	{
		if (Number_Each_Colour[i] != 0)                //remove the colour which is not appear
		{
			if (Number_Each_Colour[0] != Number_Each_Colour[i])              //if there are different value
			{
				SameOrDifer = 1;
				break;                                                       //break out loop
			}
		}
	}
	if (SameOrDifer == 0)                                                    //if there are no different numbers
	{
		std::cout << "The number cards of all colors is equal: " << Number_Each_Colour[0];             // output same number
	}
	else
	{
		std::cout << "At least two colors with a different number of cards were found:" << std::endl;  //if there are diffrent colours, output these
		std::cout << "blue cards are " << Number_Each_Colour[0] << std::endl;
		if (Number_Each_Colour[1] != 0)
		{
			std::cout << "red cards are " << Number_Each_Colour[1] << std::endl;
		}
		if (Number_Each_Colour[2] != 0)
		{
			std::cout << "violet cards are " << Number_Each_Colour[2] << std::endl;
		}
		if (Number_Each_Colour[3] != 0)
		{
			std::cout << "yellow cards are " << Number_Each_Colour[3] << std::endl;
		}
		if (Number_Each_Colour[4] != 0)
		{
			std::cout << "white cards are " << Number_Each_Colour[4] << std::endl;
		}
		if (Number_Each_Colour[5] != 0)
		{
			std::cout << "black cards are " << Number_Each_Colour[5] << std::endl;
		}
	}
}


void Show_CheckCardsNumbers()                    //output the content of file into screen
{
	std::cin.clear();                 //clear the buffer
	freopen_s(&GameFile, CheckCardsNumbersPath, "r", stdin);      //open the file
	char line[1000];                                      //store every line

	while (std::cin.getline(line, 1000))                  //while get a line of the file        
	{
		std::cerr << line << std::endl;                   //print in screen
	}

	fclose(stdin);	                      //close the file
}
//4b-check cards number

//5-check cards values
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
	for (int i = 0; i < MaxColour - 1; i++)
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
	for (int i = 1; i < MaxColour - 1; i++)                     //use blue number compare with other numbers
	{
		if (Number_Each_Colour[i] != 0)                        //remove the colour which is not appear
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
//5-check cards values


//6-check state
int CheckPlayersHandCards(int number, int* ok)
/*Check Players Cards:
   1.cannot smaller than (all - out)/n ||  bigger than it +1;
   2.cannot appear situation like:44545(only appears when k >= 4)
	 i.e. x!=x-1, x = x-2, x-1 = x+1, and considering the first player and last player,we should use module--% */
{
	int SumNumberCauldron = 0;                     //how many cards in all cauldron

	for (int i = 0; i < k; i++)
	{
		SumNumberCauldron += Cauldrons[i].Number_CardsInCauldron;
	}
	int SumNumberGraveyard = 0;
	for (int i = 0; i < n; i++)
	{
		SumNumberGraveyard += Players[i].Number_Graveyard;
	}
	for (int i = 0; i < number; i++)
	{
		if (Players[i].Number_Hand < (Index_Card - SumNumberCauldron - SumNumberGraveyard) / n || Players[i].Number_Hand>(Index_Card - SumNumberCauldron - SumNumberGraveyard) / n + 1)
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
//6-check state


int main()
{
	//Deck Gerneration(input parameters and store deck and parameters)
	std::cout << "Input game parameters(Pressing Enter confirm you have finished input)\n"; //input the necessary values
	std::cout << "the number of players(n):";
	std::cin >> n;
	std::cout << "the number of cauldrons(k): ";
	std::cin >> k;
	std::cout << "the number of poisonous(green) cards(g): ";
	std::cin >> g;
	std::cout << "the value of posionous(green) cards(gv): ";
	std::cin >> gv;
	std::cout << "the number of one type of card(without green)(o): ";
	std::cin >> o;
	EstimateValue(n, k, g, gv, o);       //as the requirement, input the value in right domain, if is is wrong, you can input again
	std::cout << "input all value of one type of card(" << o << " numbers you should input):";
	inputALLvalue(o);                    //input the second line----input the value of each card in one colour(without green)
	sortALLvalue(o);                     //Sort the value of the second line
	std::cout << "Explosion threshold: ";
	std::cin >> ExplosionThreshold;
	std::cout << "The first active player is: Player 1 ";   //when deal card, we should decide who is the first one to go
	ActivePlayer = 1;                                       //the first player's index
	std::cout << std::endl;

	freopen_s(&GameFile, DeckPath, "w", stdout);//open file, and make output into the deck file
	ForgeDeck(k, g, gv, o);                     //forge the deck
	for (int i = 0; i < g + k * o; i++)         //output the deck in order
	{
		std::cout << Deck[i].CardValue << " " << Deck[i].CardColour << " ";
	}
	fclose(stdout);       //close the file
	Show_Deck();          //display the result in screen
	std::cerr << std::endl;
	StoreGameParameter(); //store the parameters that can be read by program 2


	//Deal Cards(shuffle)
	std::cin.clear();                                //clear the buffer
	freopen_s(&GameFile, GameStatePath, "w", stdout);//output to the game state file
	Shuffle();

	std::cout << "active player = " << ActivePlayer << std::endl << "players  number = " << n << std::endl;  //output the first and second lines
	std::cout << "explosion threshold = " << ExplosionThreshold << std::endl;

	DealCards(n);                                                   //deal cards
	for (int PileIndex = 1; PileIndex <= k; PileIndex++)            //output "index pile cards: " 
	{
		std::cout << PileIndex << " pile cards:" << std::endl;
	}
	fclose(stdout);
	Show_GameState();                                               //display the result in screen
	std::cerr << std::endl;


	//play card and check everything(check end of round, load game,check green cards, check cards numbers,check cards values, check state)
	while (EndCheck() != 0) //check end of round
	{
		//3-load game
		freopen_s(&GameFile, LoadGamePath, "w", stdout);          //output to the file
		DeckTracker(n);                                           //output the deck tracker
		fclose(stdout);                                           //close the file
		Show_LoadGame();                                          //display output in the screen
		std::cerr << std::endl;

		//4a-check green cards									  
		freopen_s(&GameFile, CheckGreenCardsPath, "w", stdout);   //output to the file
		CheckGreenCards();                                        //check green and out put
		fclose(stdout);                                           //close the file
		Show_CheckGreenCards();                                   //display output in the screen
		std::cerr << std::endl;

		//4b-check cards number
		freopen_s(&GameFile, CheckCardsNumbersPath, "w", stdout);  //output to the file
		CheckCardsNumbers();                                       //check cards numebers
		fclose(stdout);                                            //close the file
		Show_CheckCardsNumbers();                                  //display output in the screen
		std::cerr << std::endl;

		//5-check cards values
		freopen_s(&GameFile, CheckCardsValuesPath, "w", stdout);   //output to the file
		CheckCardsValues();                                        //check cards numebers
		fclose(stdout);                                            //close the file
		Show_CheckCardsValues();                                   //display output in the screen
		std::cerr << std::endl;

		//6-check state
		freopen_s(&GameFile, CheckStatePath, "w", stdout);         //output to the file
		int OK = 0;                                                //if there is wrong,it will be 1
		CheckPlayersHandCards(n, &OK);                             //whether cards number in player hands is right
		CheckColourInCauldrons(&OK);                               //check colours in cauldron
		CheckExplosion(ExplosionThreshold, &OK);                   //check whether it should explosion
		if (OK == 0)                                               //if there isn't wrong
		{
			std::cout << "Current state of the game is ok";
		}
		fclose(stdout);                                            //close the file
		Show_CheckState();                                         //display output in the screen
		std::cerr << std::endl;

		//PLAY
		freopen_s(&GameFile, GameStatePath, "w", stdout);        //output to the file
		EasyMoveAndCauldronExplosion(ActivePlayer);              //move the first card of active player into the right pile
		OutputSituation();                                       //output the situation
		fclose(stdout);                                          //close the file
		Show_GameState();                                        //display output in the screen
		std::cerr << std::endl;
		std::cerr << std::endl;

		Sleep(4 * 1000);
	}

	//show result
	freopen_s(&GameFile, GGPath, "w", stdout);        //output to the file
	GameOver();                                       //print the score
	fclose(stdout);                                   //close the file
	Show_GG();                                        //display output in the screen

	return 0;
}