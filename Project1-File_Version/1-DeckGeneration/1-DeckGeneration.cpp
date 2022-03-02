#include<iostream>

FILE* GameFile;         //Local file transfer protocol
char DeckPath[200] = "E:/Gdansk University of Technology/first semester/Basics of computer programming/ProjectOfLecture/Project1-File_Version/GameFILE/Deck.txt";
char GameParameterPath[200] = "E:/Gdansk University of Technology/first semester/Basics of computer programming/ProjectOfLecture/Project1-File_Version/GameFILE/1-GameParameter.txt";

const int MaxSize = 20;  //The max quantity of card of other colour in the game.The STOS system will test the code for max o=20, so I change here into 20.
const int MaxColour = 7; //The quatity of kind of colour in the game is 7,so I suppose the max of colour is 7 --- 0  green,1 blue,2 red,3 violet,4 yellow,5 white,6 black

int ActivePlayer;         //who is the active player
int n;                    //n-the amount of players
int k;                    //k-the number of cauldrons corresponding to a number of colors of cards(without green)
int g;                    //g-a number of green cards
int gv;                   //gv-a value of green cards
int o;                    //o-a number of cards for each other color except green                                   //The first line
int ALLvalue[MaxSize];    //used to store the value of card(without green)                        //The second line
int ExplosionThreshold;   //the explosion threshold

typedef struct       //the card has elements of value and colour
{
	int CardValue;
	char CardColour[7]; //used to store colour
}Card;
Card Deck[MaxColour * MaxSize];  //array deck used to store cards


void inputALLvalue(int valueo)  //input the value of each card in one colour(without green)----input of second line
{
	for (int i = 0; i < valueo; i++)
	{
		std::cin >> ALLvalue[i];
	}
}


void EstimateValue(int valuen, int valuek, int valueg, int valuegv, int valueo)  //estimate whether the value exceed the requirement
{
	while (valuen < 2 || valuen > 20) //estimate n, input n again if n is not conform to the requirement 
	{
		std::cerr << "2<=n<=20,input n again" << std::endl;
		std::cin >> n;
		valuen = n;
	}
	while (valuek < 1 || valuek > 6) //estimate k, input k again if k is not conform to the requirement 
	{
		std::cout << "1<=k<=6,input k again" << std::endl;
		std::cin >> k;
		valuek = k;
	}

	while (valueg < 1 || valueg > 9)//estimate g
	{
		std::cout << "1<=g<=9,input g again" << std::endl;
		std::cin >> g;
		valueg = g;
	}

	while (valuegv < 1 || valuegv > 10) //estimate gv
	{
		std::cout << "1<=gv<=10,input gv again" << std::endl;
		std::cin >> gv;
		valuegv = gv;
	}

	while (valueo < 1 || valueo > 20) //estimate o
	{
		std::cout << "1<=o<=20,input o again" << std::endl;
		std::cin >> o;
		valueo = o;
	}
}


void sortALLvalue(int valueo) //sort the value to make sure that cards arranged in non-decreasing order
{
	for (int i = 0; i < valueo - 1; i++) //use every number      
	{
		for (int j = i + 1; j < valueo; j++) //to compare to every number behind it
		{
			if (ALLvalue[i] > ALLvalue[j]) //exchange them if the number is bigger than the number behind it
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


int main()
{
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
	inputALLvalue(o);                 //input the second line----input the value of each card in one colour(without green)

	sortALLvalue(o);                  //Sort the value of the second line

	std::cout << "Explosion threshold: ";
	std::cin >> ExplosionThreshold;

	std::cout << "The first active player is: Player 1 ";   //when deal card, we should decide who is the first one to go
	ActivePlayer = 1;                                               //the first player's index
	std::cout << std::endl;

	freopen_s(&GameFile, DeckPath, "w",stdout);//open file, and make output into the deck file

	ForgeDeck(k, g, gv, o);              //forge the deck

	for (int i = 0; i < g + k * o; i++) //output the deck in order
	{
		std::cout << Deck[i].CardValue << " " << Deck[i].CardColour << " ";
	}

	fclose(stdout);   //close the file
	
	Show_Deck();      //display the result in screen

	StoreGameParameter(); //store the parameters that can be read by program 2

	return 0;
}
