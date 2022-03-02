#include<iostream>

FILE* GameFile; //Local file transfer protocol
char GameStatePath[200] = "E:/Gdansk University of Technology/first semester/Basics of computer programming/ProjectOfLecture/Project1-File_Version/GameFILE/GameState.txt";
char GameParameterPath[200]= "E:/Gdansk University of Technology/first semester/Basics of computer programming/ProjectOfLecture/Project1-File_Version/GameFILE/1-GameParameter.txt";

const int MaxSize = 20; //The max quantity of card of other colour in the game.The STOS system will test the code for max o=20, so I change here into 20.
const int MaxColour = 7;   //The quatity of kind of colour in the game is 7,so I suppose the max of colour is 7 --- 0  green,1 blue,2 red,3 violet,4 yellow,5 white,6 black

int ActivePlayer;          //the active player's index
int n;  //n-the amount of players
int k;  //k-the number of cauldrons corresponding to a number of colors of cards(without green)
int g;  //g-a number of green cards
int gv; //gv-a value of green cards
int o;  //o-a number of cards for each other color except green                                   //The first line
int ALLvalue[MaxSize]; //used to store the value of card(without green)                           //The second line
int ExplosionThreshold;  //the explosion threshold

typedef struct       //the card has elements of value and colour
{
	int CardValue;
	char CardColour[7];
}Card;
 Card Deck[MaxColour * MaxSize];  //array deck used to store cards


void inputALLvalue(int valueo)  //input the value of each card in one colour(without green)----input of second line
{
	for (int i = 0; i < valueo; i++)
	{
		std::cin >> ALLvalue[i];
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

void ForgeGreen(int valueg, int valuegv)  //Forge the card of green to the Deck;
{
	for (int i = 0; i < valueg; i++)
	{
		Deck[i].CardValue = valuegv;
		char colourname[7] = "green";
		inputcardcolour(colourname, Deck[i].CardColour);
	}
}

void ForgeBlue(int valueo, int valueg)  //Forge Blue Card
{
	for (int i = valueg; i < valueg + valueo; i++)
	{
		Deck[i].CardValue = ALLvalue[i - valueg];
		char colourname[7] = "blue";
		inputcardcolour(colourname, Deck[i].CardColour);
	}
}

void ForgeRed(int valueo, int valueg)  //Forge Red Card
{
	for (int i = valueg + valueo; i < valueg + 2 * valueo; i++)
	{
		Deck[i].CardValue = ALLvalue[i - valueg - valueo];
		char colourname[7] = "red";
		inputcardcolour(colourname, Deck[i].CardColour);
	}
}

void ForgeViolet(int valueo, int valueg)  //Forge Violet Card
{
	for (int i = valueg + 2 * valueo; i < valueg + 3 * valueo; i++)
	{
		Deck[i].CardValue = ALLvalue[i - valueg - 2 * valueo];
		char colourname[7] = "violet";
		inputcardcolour(colourname, Deck[i].CardColour);
	}
}

void ForgeYellow(int valueo, int valueg)  //Forge Yellow Card
{
	for (int i = valueg + 3 * valueo; i < valueg + 4 * valueo; i++)
	{
		Deck[i].CardValue = ALLvalue[i - valueg - 3 * valueo];
		char colourname[7] = "yellow";
		inputcardcolour(colourname, Deck[i].CardColour);
	}
}

void ForgeWhite(int valueo, int valueg)  //Forge White Card
{
	for (int i = valueg + 4 * valueo; i < valueg + 5 * valueo; i++)
	{
		Deck[i].CardValue = ALLvalue[i - valueg - 4 * valueo];
		char colourname[7] = "white";
		inputcardcolour(colourname, Deck[i].CardColour);
	}
}

void ForgeBlack(int valueo, int valueg)  //Forge Black Card
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


int main()
{
	freopen_s(&GameFile, GameParameterPath, "r", stdin);//use game state file as input

	std::cin >> n >> k >> g >> gv >> o;    //input the first line 

	inputALLvalue(o);                 //input the second line----input the value of each card in one colour(without green)

	std::cin >> ExplosionThreshold;
	std::cin >> ActivePlayer;                                               //the first player's index

	fclose(stdin);  //close the file

	freopen_s(&GameFile, GameStatePath, "w", stdout);//output to the game state file

	ForgeDeck(k, g, gv, o);              //forge the deck

	Shuffle();

	std::cout << "active player = " << ActivePlayer << std::endl << "players  number = " << n << std::endl;  //output the first and second lines
	std::cout << "explosion threshold = " << ExplosionThreshold << std::endl;

	DealCards(n);            //deal cards

	for (int PileIndex = 1; PileIndex <= k; PileIndex++)            //output "index pile cards: " 
	{
		std::cout << PileIndex << " pile cards:" << std::endl;
	}

	fclose(stdout);

	Show_GameState();//display the result in screen

	return 0;
}