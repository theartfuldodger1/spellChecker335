

using namespace std;

#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <unordered_set>
#include <iomanip>
#include <vector>
#include <list>

void mySwitch();
void createDictionary(ifstream &inFile, set <string> &dictionarySet);
void createcheckFileVector(ifstream &inFile, vector <string> &checkFileVector);
string fileIterator(istream &input, char delim);
int scrollChars(istream &instructionFile);
int scrollWhiteSpace(istream &cin);
void initializationMenu();
void menu1A();
void menu2A();
bool basicSearch1(string wordIn); 
bool basicSearch2(string wordIn);
bool secondarySearch1(string wordIn, string possibleCorrects);
bool secondarySearch2(string wordIn, string possibleCorrects);

void runSearches();
void searchResults();

string toLowerCase(const string &wordIn);
void voidLowerCase(string &wordIn);

void termSplitter(string lineIn, string &term1, string &term2, string &term3, int &multiterm);

set<string> dictionarySet;
unordered_set<string> dictionarySet2;
vector<string> checkFileVector;
list<string> incorrectWords;

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
				case 1: //Use only default files
				{
					dictionarySet.clear();
					dictionarySet2.clear();
					checkFileVector.clear();

					ifstream dictionaryFile;
					dictionaryFile.open("dictionary.txt");
					createDictionary(dictionaryFile, dictionarySet);

					ifstream checkFile;
					checkFile.open("check_it.txt");
					createcheckFileVector(checkFile, checkFileVector);
					runSearches();
					searchResults();
				}
				break;
				case 2: //Add a new dictionary file but use default file to check
				{
					dictionarySet.clear();
					dictionarySet2.clear();
					checkFileVector.clear();

					menu1A();//Add a new dictionary

					ifstream checkFile;
					checkFile.open("check_it.txt");
					createcheckFileVector(checkFile, checkFileVector);

				}
				break;
				case 3: //Add a new text file to check but use default dictionary
				{
					dictionarySet.clear();
					dictionarySet2.clear();
					checkFileVector.clear();

					menu2A();//Add a new text file 

					ifstream dictionaryFile;
					dictionaryFile.open("dictionary.txt");
					createDictionary(dictionaryFile, dictionarySet);
				}
				break;
				case 4: //Add both a custom dictionary and a file to check
				{
					dictionarySet.clear();
					dictionarySet2.clear();
					checkFileVector.clear();

					menu1A();//Add a new dictionary 
					menu2A();//Add a new text file 
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

void runSearches()
{
	bool strike = 0;
	string error;
	string possibleCorrects;

	//ORDERED search BEGIN
	for (int i = 0; i < checkFileVector.size(); i++)
	{
		strike = basicSearch1(checkFileVector[i]);
		if (strike == 0)
		{
			error = checkFileVector[i];
			strike = secondarySearch1(checkFileVector[i], possibleCorrects);
			if (strike == 0)
			{
				error += " = No matches found";
				incorrectWords.push_back(error);
			}
			else
			{
				error += " = " + possibleCorrects;
				incorrectWords.push_back(error);
			}
		}	
	}
	//ORDERED search END
}

void searchResults()
{
	list<string>::iterator iter = incorrectWords.begin();
	while (iter != incorrectWords.end())
	{
		cout << "Word problems!\n"
			<<"\t\tincorrectWords: " << *iter << endl;
		iter++;
	}
}

//searcing ORDERED set. returns 1 if word is found in dictionary, else returns 0;
bool basicSearch1(string wordIn)
{
	if (dictionarySet.find(wordIn) != dictionarySet.end())
		return 1;

	return 0;
}
//searcing ORDERED set. follows basicSearch() if word is not found. searches in parts 
//returns 1 if word is found in dictionary, else returns 0;
bool secondarySearch1(string wordIn, string possibleCorrects)
{
	int tempSize = 0;
	int wordInSize = wordIn.size();
	int matches = 0;
	int win = 0;

	int strike = 0;

	string temp;
	set<string>::iterator iter = dictionarySet.begin();

	while (iter != dictionarySet.end())
	{
		temp = *iter;
		tempSize = temp.size();
		if (wordInSize == tempSize || wordInSize == (tempSize + 1) || wordInSize == (tempSize - 1))
		{
			if (wordInSize == tempSize)//Same size
			{
				for (int i = 0; i < wordInSize; i++)
				{
					if(wordIn[i] != temp[i])
					/*	matches++;
					else	*/
						strike++;
					else
						matches++;

					if (strike >= 2)
						break;
					cout << "wordIn " << wordIn << " temp " << temp << " strikes: " << strike << endl;
				}
				if (strike = 1)
				{
					//if (matches == (wordInSize - 1) || matches == wordInSize + 1)
						possibleCorrects += temp;
						win++;
				}

			}
			else if (wordInSize > tempSize)//wordIn bigger
			{
				for (int i = 0; i < tempSize; i++)
				{
					if (wordIn[i] != temp[i])
					/*	matches++;
					else	*/
						strike++;
					else
						matches++;
					if (strike >= 2)
						break;
				}
				if (strike = 1)
				{
					//if (matches == (wordInSize - 1) || matches == wordInSize + 1)
						possibleCorrects += temp;
						win++;
				}
			}
			else
			{
				for (int i = 0; i < tempSize; i++)//found word bigger
				{
					for (int i = 0; i < wordInSize; i++)
					{
						if (wordIn[i] != temp[i])
						/*	matches++;
						else	*/
							strike++;
						else
							matches++;
						if (strike >= 2)
							break;
					}
					if (strike = 1)
					{
						//if (matches == (wordInSize - 1) || matches == wordInSize + 1)
						possibleCorrects += temp;
						win++;
					}

				}
			}
		}
		iter++;
	}
	if (win ==  0)
		return 0;
	return 1;
}

//searcing UNORDERED set. returns 1 if word is found in dictionary, else returns 0;
bool basicSearch2(string wordIn)
{
	if (dictionarySet2.find(wordIn) != dictionarySet2.end())
		return 1;

	return 0;
}
//searcing UNORDERED set. follows basicSearch() if word is not found. searches in parts 
//returns 1 if word is found in dictionary, else returns 0;
bool secondarySearch2(string wordIn, string possibleCorrects)
{
	int tempSize = 0;
	int wordInSize = wordIn.size();
	int matches = 0;

	int strike = 0;

	string temp;
	unordered_set<string>::iterator iter = dictionarySet2.begin();

	while (iter != dictionarySet2.end())
	{
		temp = *iter;
		tempSize = temp.size();
		if (wordInSize == tempSize || wordInSize == (tempSize + 1) || wordInSize == (tempSize - 1))
		{
			if (wordInSize == tempSize)//sae size
			{
				for (int i = 0; i < wordInSize; i++)
				{
					if (wordIn[i] == temp[i])
						/*	matches++;
						else	*/
						strike++;

					if (strike >= 2)
						return 0;
				}
				if (strike = 1)
					//if (matches == (wordInSize - 1) || matches == wordInSize + 1)
					possibleCorrects += temp;
			}
			else if (wordInSize > tempSize)//wordIn bigger
			{
				for (int i = 0; i < tempSize; i++)
				{
					if (wordIn[i] != temp[i])
						/*	matches++;
						else	*/
						strike++;
					if (strike >= 2)
						return 0;
				}
				if (strike = 1)
					//if (matches == (wordInSize - 1) || matches == wordInSize + 1)
					possibleCorrects += temp;
			}
			else
			{
				for (int i = 0; i < tempSize; i++)//found word bigger
				{
					for (int i = 0; i < wordInSize; i++)
					{
						if (wordIn[i] == temp[i])
							/*	matches++;
							else	*/
							strike++;
						if (strike >= 2)
							return 0;
					}
					if (strike = 1)
						//if (matches == (wordInSize - 1) || matches == wordInSize + 1)
						possibleCorrects += temp;
				}
			}
		}
		iter++;
	}
	return 1;
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
			<< "\n\tPlease ensure usage of proper case. Enter Q to quit"
			<< "\n\n\t==>> ";

		cin >> fileIn;
		dictionaryFile.open(fileIn);

		if (fileIn == "q" || fileIn == "Q")
			return;
		if (!dictionaryFile.is_open())
			cerr << "\n\tCould not open file\n\n";

	} while (!dictionaryFile.is_open());

	createDictionary(dictionaryFile, dictionarySet);
}

void menu2A()
{
	string fileIn;
	ifstream checkFile;
	bool fileFail = 1;

	do
	{
		cout << right;
		cout << setw(45) << "Add a File to Check"
			<< "\n\t---------------------------------------------------"
			<< "\n\tPlease ensure usage of proper case. Enter Q to quit"
			<< "\n\n\t==>> ";

		cin >> fileIn;
		checkFile.open(fileIn);

		if (fileIn == "q" || fileIn == "Q")
			return;
		if (!checkFile.is_open())
			cerr << "\n\tCould not open file\n\n";

	} while (!checkFile.is_open());

	createcheckFileVector(checkFile, checkFileVector);
}

void createDictionary(ifstream &inFile, set <string> &dictionarySet)
{
	int wordCount = 0;
	string newWord = "I don't count...";
	char delim = '\n';

	while (!inFile.eof())
	{
		cout << "big arse file? wordCount: " << wordCount << " " << newWord << "\n";
		//Acquire words from file - BEGIN
		newWord = fileIterator(inFile, delim);
		dictionarySet.emplace(newWord);
		dictionarySet2.emplace(newWord);
		//Acquire words from file - END
		wordCount++;
	}
}

void createcheckFileVector(ifstream &inFile, vector <string> &vector)
{
	int wordCount = 0;
	string newWord = "first";
	char delim = ' ';
	string term1, term2, term3;
	int multiterm = 0;

	while (!inFile.eof())
	{
		//Acquire words from file - BEGIN
		newWord = fileIterator(inFile, delim);
		termSplitter(newWord, term1, term2, term3, multiterm);

		newWord = term1;
		voidLowerCase(newWord);
		checkFileVector.push_back(newWord);
		wordCount++;
//		cout << "checkFile wordCount: " << wordCount << " newWord: " << newWord << "\n";
		if (multiterm == 2)
		{
			newWord = term2;
			voidLowerCase(newWord);
			checkFileVector.push_back(newWord);
			wordCount++;
//			cout << "checkFile wordCount: " << wordCount << " newWord: " << newWord << "\n";
		}
		if (multiterm == 3)
		{
			newWord = term3;
			voidLowerCase(newWord);
			checkFileVector.push_back(newWord);
			wordCount++;
		//	cout << "checkFile wordCount: " << wordCount << " newWord: " << newWord << "\n";
		}
		//checkFileVector.push_back(newWord);
		//Acquire words from file - END
	}
}

//Splits strings with multiple words into seperate strings, makes all lower case and removes hyphen. Returns clean strings and a value
//reflecting the number of terms returned
void termSplitter(string lineIn, string &term1, string &term2, string &term3, int &multiterm)
{
	string newWord;
	string temp;
	term1.clear();
	term2.clear();
	term3.clear();

	multiterm  = 0;
	unsigned int i = 0;
	while (i < lineIn.size())
	{
		term1 += lineIn[i];
		multiterm = 1;
		i++;
		temp = lineIn[i];
		//cout << "Temp: " << temp << endl;
		if (temp == "," || temp == "'" || temp == ".")
		{
			if (temp == "'")
				i++;
			
			i++;
		//	cout << "punctuation caught1 " << temp << "*" << endl;
			temp = lineIn[i];
			//cout << "punctuation caught1 " << temp << "*" << endl;
		}

		if (temp == " " || temp == "-")//remove hyphens
		{
			multiterm = 2;
			i++;
			while (i < lineIn.size())
			{
				term2 += lineIn[i];
				i++;
				temp = lineIn[i];
				if (temp == "," || temp == "'" || temp == ".")
				{
					if (temp == "'")
						i++;
					i++;
					//cout << "punctuation caught2 " << temp << "*" << endl;
					temp = lineIn[i];
					//cout << "punctuation caught2 " << temp << "*" << endl;
				}
				
				if (temp == " " || temp == "-")
				{
					multiterm = 3;
					i++;
					while (i < lineIn.size())
					{
						term3 += lineIn[i];
						//lineIn[i] = ' ';
						i++;
						temp = lineIn[i];
						if (temp == "," || temp == "'" || temp == ".")
						{
							
							i++;
							if (temp == "'")
								i++;
							//cout << "punctuation caught3 " << temp << "*" << endl;
							temp = lineIn[i];
						//	cout << "punctuation caught3 " << temp << "*" << endl;
						}
					}
				}
			}
		}
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

//iterates through string input and modifies strings upper case letters to lower case if needed
//Precondition: Valid string of letters
//Postcondition: Returns string with only lower case letters
string toLowerCase(const string &wordIn)
{
	string word_temp = wordIn;

	for (int i = 0; i < word_temp.length(); i++)
	{
		if (word_temp[i] == 45)
			word_temp[i] = ' ';
		else
		{
			if ((word_temp[i] >= 'A') && (word_temp[i] <= 'Z')) //looks for upper case
				word_temp[i] = (char)((int)word_temp[i] + 32); //modifies accordingly by replacement
		}
	}
	return word_temp;
}
//iterates through string input and modifies strings upper case letters to lower case if needed
//Precondition: Valid string of letters
//Postcondition: Returns string with only lower case letters
void voidLowerCase(string &wordIn)
{
	for (unsigned int i = 0; i < wordIn.length(); i++)
	{
		if (wordIn[i] == 45)
			wordIn[i] = ' ';
		else
		{
			if ((wordIn[i] >= 'A') && (wordIn[i] <= 'Z')) //looks for upper case
				wordIn[i] = (char)((int)wordIn[i] + 32); //modifies accordingly by replacement
		}
	}
}