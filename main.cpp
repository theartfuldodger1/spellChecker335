

using namespace std;

#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <iomanip>

void mySwitch();
void createDictionary(ifstream &inFile, set <string> &dictionarySet);
string fileIterator(istream &input, char delim);
int scrollChars(istream &instructionFile);
int scrollWhiteSpace(istream &cin);
void initializationMenu();
void menu1A();

set<string> dictionarySet;

int main()
{
	mySwitch();

	return 0;
}

void initializationMenu()
{
	cout << right;
	cout << setw(45) << setfill(' ') << "Spell Check Project"
		<< "\n\t----------------------------------------------------"
		<< "\n\t1. Use only default files"
		<< "\n\t2. Add a new dictionary file"
		<< "\n\t3. Add a new text file to check"
		<< "\n\t4. Add both"
		<< "\n\t5. Quit"
		<< "\n\n\t==>> ";
}

void mySwitch ()
{
	unsigned short int param = 0;
	bool check = 0;
	do
	{
		cout << "\n";
		param = 0;
		if (param < 0 || param > 8)
		{
			cout << "\n\tIncorrect input:" << endl;
			param = 0;
		}
		if (param == 0)
		{
			if (check == 0)
				initializationMenu();
			cin >> param;
		}

		check = 0;
		if (cin.fail())
		{
			check = 1;
			cin.clear();
			cin.ignore(255, '\n');
		}
		if (param > 0 || param < 9)
			switch (param)
			{
				case 1: //
				{
					menu1A();
				}
				break;
				case 2: //
				{
				
				}
				break;
				case 3: //
				{

				}
				break;
				case 4: //
				{
				
				}
				break;
				case 5: //Quit
				{
					//exit
					cout << "Bye!" << endl;
				}
				break;
			}
	} while (param != 5);
}

//add dictionary file from Menu1, choice 1
void menu1A()
{
	string fileIn;
	ifstream dictionaryFile;
	bool fileFail = 1;

	do
	{
		cout << right;
		cout << setw(45) << "Add a Dictionary File"
			<< "\n\t---------------------------------------------------"
			<< "\n\tPlease ensure usage of proper case."
			<< "\n\n\t==>> ";

		cin >> fileIn;
		dictionaryFile.open(fileIn);

		if (fileIn == "q" || fileIn == "Q")
			return;
		if (!dictionaryFile.is_open())
			cerr << "\n\tCould not open file\n\n";

		cout << "stuck?\n";
	} while (!dictionaryFile.is_open());

	createDictionary(dictionaryFile, dictionarySet);
}

void createDictionary(ifstream &inFile, set <string> &dictionarySet)
{
	int wordCount = 0;

	while (!inFile.eof())
	{
		cout << "big arse file?\n";
		//Acquire words from file - BEGIN
		char newWord[256];
		inFile.get(newWord, 256);
		dictionarySet.emplace(newWord);
		//Acquire words from file - END
	}
}

//Scrolls through file data for processing specific fields as required
//Returns next data field as a string.
string fileIterator(istream &input, char delim)
{
	string newString;
	int endFlag = 0;

	getline(input, newString, delim);
	endFlag = scrollChars(input);

	return newString;
}

//allows skipping of parenthesis and iterates through whitespace after a word that may be 
//or is expected to be the last entry in order to find next entry or eof
//used only in fileIterator(). Returns next char as int
int scrollChars(istream &instructionFile)
{
	int temp = instructionFile.peek();
	//catch parenthesis here
	if (temp == '"')
	{
		do
		{
			instructionFile.ignore();
			temp = instructionFile.peek();
		} while (temp != '"');
	}
	//9 == horizontal tab
	while (temp == ' ' || temp == 9)//skip trailing white space
	{
		instructionFile.ignore();
		temp = instructionFile.peek();
	}
	return temp;
}

//iterates through whitespace after a word that may be or is expected to be the last entry
//in order to find eof. Used by UserPrompt() for user input processing (cin). Returns next char as int
//Precondition: 2nd or 3rd word of getInstruction term input
//Postcondition: cin loaded with next non-whitespace data
int scrollWhiteSpace(istream &cin)
{
	int temp = cin.peek();
	//9 == horizontal tab
	while (temp == ' ' || temp == 9)
	{
		cin.ignore();
		temp = cin.peek();
	}
	return temp;
}
