/*
Author: Cordell Hurst
Course : CSCI 335 Software Design and Analysis III
Instructor: Pavel Shostak
Assignment 03
18 Nov 2018
This program accepts two command line argument, a plain text file with text to check and a plain text file dictionary.
The program checks the check file against the dictionary and provides possible corrections. The dictionary is
stored in a ordered set and an unordered set. The two sets are utilized seperately and both are timed for completion
and compared.
Using C++11
Compile with g++ -o out main.cpp -std=c++11
Execute with::  out check_it.txt dictionary.txt or out <text_file> <text_file>
*/

using namespace std;

#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <unordered_set>
#include <iomanip>
#include <vector>
#include <list>
#include <chrono>

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
bool secondarySearch1(string wordIn);
bool secondarySearch2(string wordIn);

void runSearchesOrdered();
void runSearchesUnordered();
void searchResults();

string toLowerCase(const string &wordIn);
void voidLowerCase(string &wordIn);

void termSplitter(string lineIn, string &term1, string &term2, string &term3, int &multiterm);

double counter = 0;
set<string> dictionarySet;
unordered_set<string> dictionarySet2;
vector<string> checkFileVector;
set<string> incorrectWords;
set<string> checkOutcome;

int main(int argc, char* argv[])
{

	//for command line execution
	if (argc != 3)//for attempted launch w/o param
	{
		cerr << "\n\tUsage: " << argv[0] << " <Filename> <Filename>\n\t\tA filename for a plain text file to check"
			<< "\n\tand a filename for a plain text dicionary must follow executable.\n\n";
		return 1;
	}
	ifstream checkFile(argv[1]);
	ifstream dictionaryFile(argv[2]);

	if (!checkFile.is_open() || !dictionaryFile.is_open())
	{
		cerr << "\n\tCould not open file\n\ns";
		return 1;
	}

/*
	//for IDE execution
	///////////////////////////////////
	cout << "Starting..." << endl;
	ifstream dictionaryFile;
	//dictionaryFile.open("dic1.txt");
	dictionaryFile.open("dictionary.txt");
	ifstream checkFile;
	//checkFile.open("test1.txt");
	checkFile.open("check_it.txt");
	///////////////////////////////////
*/
	createDictionary(dictionaryFile, dictionarySet);
	createcheckFileVector(checkFile, checkFileVector);
	dictionaryFile.close();
	checkFile.close();

	runSearchesOrdered();
	searchResults();
	runSearchesUnordered();
	searchResults();
	
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
					
					dictionaryFile.close();
					checkFile.close();
					
					runSearchesOrdered();
					searchResults();
					runSearchesUnordered();
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
					
					checkFile.close();
					
					runSearchesOrdered();
					searchResults();
					runSearchesUnordered();
					searchResults();
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

					dictionaryFile.close();

					runSearchesOrdered();
					searchResults();
					runSearchesUnordered();
					searchResults();
				}
				break;
				case 4: //Add both a custom dictionary and a file to check
				{
					dictionarySet.clear();
					dictionarySet2.clear();
					checkFileVector.clear();

					menu1A();//Add a new dictionary 
					menu2A();//Add a new text file 

					runSearchesOrdered();
					searchResults();
					runSearchesUnordered();
					searchResults();
				}
				break;
				case 5: //Quit
				{
					//exit
					cout << "\tBye!" << endl;
				}
				break;
			}
	} while (param != 5);

}

void runSearchesOrdered()
{
	bool strike = 0;
	bool runSecondary = 0;
	bool found = 0;

	cout << "\n\tBegin Ordered:: " << endl;
	chrono::steady_clock::time_point a1, a2;
	a1 = chrono::steady_clock::now();
	//ORDERED search BEGIN
	for (int i = 0; i < checkFileVector.size(); i++)
	{
		strike = basicSearch1(checkFileVector[i]);
		
		if (strike == 1)
			runSecondary = 1;
	}
	
	if (runSecondary == 1)
	{
		set<string>::iterator iter = incorrectWords.begin();
		while (iter != incorrectWords.end())
		{
			secondarySearch1(*iter);
			iter++;
		}
	}
	//ORDERED search END
	a2 = chrono::steady_clock::now();
	cout << "\n\t" << setw(19) << setfill(' ') << "ORDERED Set Time ==>> "
		<< chrono::duration_cast<chrono::milliseconds>(a2 - a1).count() << " ms";

	auto AVLTime = chrono::duration_cast<chrono::milliseconds>(a2 - a1).count();
	cout << "\n\t-----------------------------";
}
void runSearchesUnordered()
{
	bool strike = 0;
	bool runSecondary = 0;
	bool found = 0;
	incorrectWords.clear();

	cout << "\n\tBegin Unordered:: " << endl;
	chrono::steady_clock::time_point a1, a2;
	a1 = chrono::steady_clock::now();
	//UNORDERED search BEGIN
	for (int i = 0; i < checkFileVector.size(); i++)
	{
		strike = basicSearch1(checkFileVector[i]);

		if (strike == 1)
			runSecondary = 1;
	}

	if (runSecondary == 1)
	{
		set<string>::iterator iter = incorrectWords.begin();
		while (iter != incorrectWords.end())
		{
			secondarySearch1(*iter);
			iter++;
		}
	}
	//UNORDERED search END
	a2 = chrono::steady_clock::now();
	cout << "\n\t" << setw(19) << setfill(' ') << "UNORDERED Set Time ==>> "
		<< chrono::duration_cast<chrono::milliseconds>(a2 - a1).count() << " ms";

	auto AVLTime = chrono::duration_cast<chrono::milliseconds>(a2 - a1).count();
	cout << "\n\t-----------------------------";
}
void searchResults()
{
	ofstream outfile;
	outfile.open("progOutput.txt");
	set<string>::iterator iter = checkOutcome.begin();
	while (iter != checkOutcome.end())
	{
		outfile << *iter << endl;
		cout <<"\t\tcheckOutcome: " << *iter << endl;
		iter++;
	}
}

//searcing ORDERED set. returns 1 if word is found in dictionary, else returns 0;
bool basicSearch1(string wordIn)
{
//	cout << "BASIC SEARCH" << endl;
	if (dictionarySet.find(wordIn) == dictionarySet.end())
	{
		incorrectWords.emplace(wordIn);
		return 1;
	}
	return 0;
}
//searcing ORDERED set. follows basicSearch() if word is not found. searches in parts 
//returns 1 if word is found in dictionary, else returns 0;
bool secondarySearch1(string wordIn)
{
	bool breakOut = 0;
	int tempSize = 0;
	int wordInSize = wordIn.size();
	int matches = 0;
	int strike = 0;
	int wins = 0;
	int j = 0;
	int i = 0;
	string temp;
	string possibleCorrects = wordIn;
	unsigned short int param = 0;

	possibleCorrects += " = ";
///////////////////////////change for ordered/unordered
	set<string>::iterator iter = dictionarySet.begin();
////////////////////////////////////////////////////////////////////////////////////////
	while (iter != dictionarySet.end())
	{
		matches = 0;
		strike = 0;
		temp = *iter;
		tempSize = temp.size();
		if (wordInSize == tempSize || ((wordInSize+1) == tempSize) || ((wordInSize-1) == tempSize))
		{
			i = 0;
			j = 0;
			param = 0;
			breakOut = 0;

			do
			{
				if (breakOut == 0 && wordInSize == tempSize)//Same size
					param = 1;
				if (breakOut == 0 && wordInSize > tempSize)//wordIn bigger
					param = 2;
				if (breakOut == 0 && wordInSize < tempSize)//wordIn smaller
					param = 3;

				switch(param)
				{
					case 1://Same size
					{
						for (; i < wordInSize; i++)
						{
							if (wordIn[i] == temp[j])//match!
							{
								matches++;
								breakOut = 0;
							}
							else if (wordIn[i] == temp[j + 1] && wordIn[i + 1] == temp[j])//special
							{
								i++;
								i++;
								j++;
								j++;
								strike++;
								matches++;
								matches++;
								if (strike == 2)
								{
									matches = 0;
									break;
								}
							}
							else if (wordIn[i + 1] == temp[j])//wordIn is now shorter
							{
								strike++;
								matches++;
								i++;
								breakOut = 1;
								param = 3;
								break;
							}
							else if (wordIn[i] == temp[j + 1])//wordIn is now bigger
							{
								strike++;
								matches++;
								j++;
								breakOut = 1;
								param = 2;
								break;
							}
							else
							{
								strike++;
								if (strike == 2)
								{
									matches = 0;
									break;
								}
							}
							j++;

							if ((j == tempSize && i != wordInSize) || (i == wordInSize && j != tempSize))
							{
								if (matches != wordInSize - 1 && matches != wordInSize)
								{
									param = 4;
									breakOut = 0;
									strike++;
									break;
								}
								else
								{
									param = 4;
									breakOut = 0;
									if (strike == 2)
									{
										matches = 0;
										break;
									}
								}
							}
						}
						if (wordInSize == 3 && tempSize == 3)
						{
							if ((wordIn[0] == temp[0] && wordIn[2] == temp[2] || wordIn[1] == temp[1]) || (wordIn[0] == temp[0] && wordIn[1] == temp[2] && wordIn[2] == temp[1]))
							{
								param = 4;
								breakOut = 0;
							}
							else
							{
								param = 4;
								breakOut = 0;
								strike++;
							}	
						}
						if (wordInSize == 4 && tempSize == 4)
						{
							if ((wordIn[0] == temp[0] && (wordIn[2] == temp[2] || wordIn[1] == temp[1]) && temp[3] == wordIn[3]) || ((wordIn[0] == temp[0] && wordIn[1] == temp[2] && wordIn[2] == temp[1]) && temp[3] == wordIn[3]))
							{
								param = 4;
								breakOut = 0;
							}
							else
							{
								param = 4;
								breakOut = 0;
								strike++;
							}
						}
						if (wordInSize == 3 && matches == 2 && tempSize == 4 || tempSize == 5)
						{
							strike++;
							param == 4;
							breakOut == 0;
							break;
						}
						if (strike >= 2)
						{
							param = 4;
							breakOut = 0;
							break;
						}
					}
					break;
					case 2://wordIn bigger
					{
						for (; i < wordInSize; i++)
						{
							if (wordIn[i] == temp[j])//match!
							{
								matches++;
								breakOut = 0;
							}
							else if (wordIn[i] == temp[j + 1] && wordIn[i + 1] == temp[j])//special
							{
								i++;
								i++;
								j++;
								j++;
								strike++;
								matches++;
								matches++;
								if (strike == 2)
								{
									matches = 0;
									break;
								}

							}
							else if (wordIn[i + 1] == temp[j])//wordIn is now shorter
							{
								strike++;
								matches++;
								i++;
								breakOut = 1;
								param = 3;
								break;
							}
							else if (wordIn[i] == temp[j+1])//wordIn is now bigger
							{
								strike++;
								matches++;
								j++;
								breakOut = 1;
								param = 2;
								break;
							}
							else
							{
								strike++;
								if (strike == 2)
								{
									matches = 0;
									break;
								}
							}
							j++;

							if ((j == tempSize && i != wordInSize) || (i == wordInSize && j != tempSize))
							{
								if (matches != wordInSize - 1 && matches != wordInSize)
								{
									param = 4;
									breakOut = 0;
									strike++;
									break;
								}
								else
								{
									if (wordInSize == 3 && tempSize == 2)
									{
										if ((wordIn[0] == temp[0] && wordIn[1] == temp[1] || wordIn[2] == temp[1]) || (wordIn[0] == temp[1] && wordIn[1] == temp[0]))
										{
											param = 4;
											breakOut = 0;
										}
										else
										{
											param = 4;
											breakOut = 0;
											strike++;
										}
									}
									else if (j == tempSize)
									{
										strike++;
										break;
									}
									else
									{
										param = 4;
										breakOut = 0;
									}
									if (strike == 2)
									{
										matches = 0;
										break;
									}
								}
							}
						}
						if (strike >= 2)
						{
							param = 4;
							breakOut = 0;
							break;
						}
					}
					break;
					case 3://wordIn smaller
					{
						for (; i < wordInSize; i++)
						{
							if (wordIn[i] == temp[j])//match!
							{
								matches++;
								breakOut = 0;
							}
							else if (wordIn[i] == temp[j + 1] && wordIn[i + 1] == temp[j])//special
							{
								j++;
								strike++;
								matches++;
								matches++;
								if (strike == 2)
								{
									matches = 0;
									break;
								}								
							}
							else if (wordIn[i + 1] == temp[j])//wordIn is now shorter
							{
								strike++;
								matches++;
								i++;
								i++;
								j++;
								breakOut = 1;
								param = 1;
								break;
							}
							else if (wordIn[i] == temp[j + 1])//wordIn is now same size
							{
								strike++;
								matches++;
								j++;
								breakOut = 1;
								param = 1;
								break;
							}
							else
							{
								strike++;
								if (strike == 2)
								{
									matches = 0;
									break;
								}
							}
							j++;
							if ((j == tempSize && i != wordInSize) || (i == wordInSize && j != tempSize))
							{
								if (matches != wordInSize - 1 && matches != wordInSize)
								{
									param = 4;
									breakOut = 0;
									break;
								}
								else
								{
									param = 4;
									breakOut = 0;

									if (strike == 2)
									{
										matches = 0;
										break;
									}
								}
							}
						}
						if (strike >= 2)
						{
							param = 4;
							breakOut = 0;
							break;
						}
					}
					break;
				}
				if (wordInSize == 3 && matches == 2 && tempSize == 4 || tempSize == 5)
				{
					strike++;
					param == 4;
					breakOut == 0;
					break;
				}
				if (i == wordInSize || j == tempSize)
					param = 4;
			}while(param != 4);
		}
		if (strike == 1)
		{
			if((matches == tempSize || matches ==  tempSize+1 || matches == tempSize-1) && (matches == wordInSize || matches == wordInSize+1 ||  matches == wordInSize-1))
			{
				possibleCorrects += temp + " ";
				wins++;
				checkOutcome.emplace();
				breakOut = 0;
			}
		}
		iter++;
	}
	if (wins == 0)
	{
		checkOutcome.emplace(possibleCorrects);
		return 0;
	}
	checkOutcome.emplace(possibleCorrects);
	return 1;
}

//searching UNORDERED set. returns 1 if word is found in dictionary, else returns 0;
bool basicSearch2(string wordIn)
{
	if (dictionarySet2.find(wordIn) == dictionarySet2.end())
	{
		incorrectWords.emplace(wordIn);
		return 1;
	}
	return 0;
}
//searcing UNORDERED set. follows basicSearch() if word is not found. searches in parts 
//returns 1 if word is found in dictionary, else returns 0;
bool secondarySearch2(string wordIn)
{
	bool breakOut = 0;
	int tempSize = 0;
	int wordInSize = wordIn.size();
	int matches = 0;
	int strike = 0;
	int wins = 0;
	int j = 0;
	int i = 0;
	string temp;
	string possibleCorrects = wordIn;
	unsigned short int param = 0;

	possibleCorrects += " = ";
	///////////////////////////change for ordered/unordered
	unordered_set<string>::iterator iter = dictionarySet2.begin();
	////////////////////////////////////////////////////////////////////////////////////////
	while (iter != dictionarySet2.end())
	{
		matches = 0;
		strike = 0;
		temp = *iter;
		tempSize = temp.size();
		if (wordInSize == tempSize || ((wordInSize + 1) == tempSize) || ((wordInSize - 1) == tempSize))
		{
			i = 0;
			j = 0;
			param = 0;
			breakOut = 0;

			do
			{
				if (breakOut == 0 && wordInSize == tempSize)//Same size
					param = 1;
				if (breakOut == 0 && wordInSize > tempSize)//wordIn bigger
					param = 2;
				if (breakOut == 0 && wordInSize < tempSize)//wordIn smaller
					param = 3;

				switch (param)
				{
				case 1://Same size
				{
					for (; i < wordInSize; i++)
					{
						if (wordIn[i] == temp[j])//match!
						{
							matches++;
							breakOut = 0;
						}
						else if (wordIn[i] == temp[j + 1] && wordIn[i + 1] == temp[j])//special
						{
							i++;
							i++;
							j++;
							j++;
							strike++;
							matches++;
							matches++;
							if (strike == 2)
							{
								matches = 0;
								break;
							}
						}
						else if (wordIn[i + 1] == temp[j])//wordIn is now shorter
						{
							strike++;
							matches++;
							i++;
							breakOut = 1;
							param = 3;
							break;
						}
						else if (wordIn[i] == temp[j + 1])//wordIn is now bigger
						{
							strike++;
							matches++;
							j++;
							breakOut = 1;
							param = 2;
							break;
						}
						else
						{
							strike++;
							if (strike == 2)
							{
								matches = 0;
								break;
							}
						}
						j++;

						if ((j == tempSize && i != wordInSize) || (i == wordInSize && j != tempSize))
						{
							if (matches != wordInSize - 1 && matches != wordInSize)
							{
								param = 4;
								breakOut = 0;
								strike++;
								break;
							}
							else
							{
								param = 4;
								breakOut = 0;
								if (strike == 2)
								{
									matches = 0;
									break;
								}
							}
						}
					}
					if (wordInSize == 3 && tempSize == 3)
					{
						if ((wordIn[0] == temp[0] && wordIn[2] == temp[2] || wordIn[1] == temp[1]) || (wordIn[0] == temp[0] && wordIn[1] == temp[2] && wordIn[2] == temp[1]))
						{
							param = 4;
							breakOut = 0;
						}
						else
						{
							param = 4;
							breakOut = 0;
							strike++;
						}
					}
					if (wordInSize == 4 && tempSize == 4)
					{
						if ((wordIn[0] == temp[0] && (wordIn[2] == temp[2] || wordIn[1] == temp[1]) && temp[3] == wordIn[3]) || ((wordIn[0] == temp[0] && wordIn[1] == temp[2] && wordIn[2] == temp[1]) && temp[3] == wordIn[3]))
						{
							param = 4;
							breakOut = 0;
						}
						else
						{
							param = 4;
							breakOut = 0;
							strike++;
						}
					}
					if (wordInSize == 3 && matches == 2 && tempSize == 4 || tempSize == 5)
					{
						strike++;
						param == 4;
						breakOut == 0;
						break;
					}
					if (strike >= 2)
					{
						param = 4;
						breakOut = 0;
						break;
					}
				}
				break;
				case 2://wordIn bigger
				{
					for (; i < wordInSize; i++)
					{
						if (wordIn[i] == temp[j])//match!
						{
							matches++;
							breakOut = 0;
						}
						else if (wordIn[i] == temp[j + 1] && wordIn[i + 1] == temp[j])//special
						{
							i++;
							i++;
							j++;
							j++;
							strike++;
							matches++;
							matches++;
							if (strike == 2)
							{
								matches = 0;
								break;
							}

						}
						else if (wordIn[i + 1] == temp[j])//wordIn is now shorter
						{
							strike++;
							matches++;
							i++;
							breakOut = 1;
							param = 3;
							break;
						}
						else if (wordIn[i] == temp[j + 1])//wordIn is now bigger
						{
							strike++;
							matches++;
							j++;
							breakOut = 1;
							param = 2;
							break;
						}
						else
						{
							strike++;
							if (strike == 2)
							{
								matches = 0;
								break;
							}
						}
						j++;

						if ((j == tempSize && i != wordInSize) || (i == wordInSize && j != tempSize))
						{
							if (matches != wordInSize - 1 && matches != wordInSize)
							{
								param = 4;
								breakOut = 0;
								strike++;
								break;
							}
							else
							{
								if (wordInSize == 3 && tempSize == 2)
								{
									if ((wordIn[0] == temp[0] && wordIn[1] == temp[1] || wordIn[2] == temp[1]) || (wordIn[0] == temp[1] && wordIn[1] == temp[0]))
									{
										param = 4;
										breakOut = 0;
									}
									else
									{
										param = 4;
										breakOut = 0;
										strike++;
									}
								}
								else if (j == tempSize)
								{
									strike++;
									break;
								}
								else
								{
									param = 4;
									breakOut = 0;
								}
								if (strike == 2)
								{
									matches = 0;
									break;
								}
							}
						}
					}
					if (strike >= 2)
					{
						param = 4;
						breakOut = 0;
						break;
					}
				}
				break;
				case 3://wordIn smaller
				{
					for (; i < wordInSize; i++)
					{
						if (wordIn[i] == temp[j])//match!
						{
							matches++;
							breakOut = 0;
						}
						else if (wordIn[i] == temp[j + 1] && wordIn[i + 1] == temp[j])//special
						{
							j++;
							strike++;
							matches++;
							matches++;
							if (strike == 2)
							{
								matches = 0;
								break;
							}
						}
						else if (wordIn[i + 1] == temp[j])//wordIn is now shorter
						{
							strike++;
							matches++;
							i++;
							i++;
							j++;
							breakOut = 1;
							param = 1;
							break;
						}
						else if (wordIn[i] == temp[j + 1])//wordIn is now same size
						{
							strike++;
							matches++;
							j++;
							breakOut = 1;
							param = 1;
							break;
						}
						else
						{
							strike++;
							if (strike == 2)
							{
								matches = 0;
								break;
							}
						}
						j++;
						if ((j == tempSize && i != wordInSize) || (i == wordInSize && j != tempSize))
						{
							if (matches != wordInSize - 1 && matches != wordInSize)
							{
								param = 4;
								breakOut = 0;
								break;
							}
							else
							{
								param = 4;
								breakOut = 0;

								if (strike == 2)
								{
									matches = 0;
									break;
								}
							}
						}
					}
					if (strike >= 2)
					{
						param = 4;
						breakOut = 0;
						break;
					}
				}
				break;
				}
				if (wordInSize == 3 && matches == 2 && tempSize == 4 || tempSize == 5)
				{
					strike++;
					param == 4;
					breakOut == 0;
					break;
				}
				if (i == wordInSize || j == tempSize)
					param = 4;
			} while (param != 4);
		}
		if (strike == 1)
		{
			if ((matches == tempSize || matches == tempSize + 1 || matches == tempSize - 1) && (matches == wordInSize || matches == wordInSize + 1 || matches == wordInSize - 1))
			{
				possibleCorrects += temp + " ";
				wins++;
				checkOutcome.emplace();
				breakOut = 0;
			}
		}
		iter++;
	}
	if (wins == 0)
	{
		checkOutcome.emplace(possibleCorrects);
		return 0;
	}
	checkOutcome.emplace(possibleCorrects);
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

	dictionaryFile.close();
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

	checkFile.close();
}

void createDictionary(ifstream &inFile, set <string> &dictionarySet)
{
	int wordCount = 0;
	string newWord = "I don't count...";
	char delim = '\n';

	while (!inFile.eof())
	{
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
	string newWord = "I don't count either...";
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
		
		for (int i = 0; i < newWord.size(); i++)//trims trailing whitespace...
		{
			if (newWord[i] == 10)//removes trailing whitespace
			{
				string temp;
				int size = newWord.size() - 1;
				for (int j = 0; j < size; j++)
				temp += newWord[j];
				newWord = temp;
			}
		}

		if (newWord != " " && newWord != "")
		{
			checkFileVector.push_back(newWord);
			wordCount++;
		}
		if (multiterm == 2)
		{
			newWord = term2;
			voidLowerCase(newWord);
			if (newWord != " " && newWord != "")
			{
				checkFileVector.push_back(newWord);
				wordCount++;
			}
		}
		if (multiterm == 3)
		{
			newWord = term3;
			voidLowerCase(newWord);
			if (newWord != " " && newWord != "")
			{
				checkFileVector.push_back(newWord);
				wordCount++;
			}
		}
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
		if (temp == "," || temp == "'" || temp == ".")
		{
			if (temp == "'")
				i++;
			
			i++;
			temp = lineIn[i];
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
					temp = lineIn[i];
				}
				
				if (temp == " " || temp == "-")
				{
					multiterm = 3;
					i++;
					while (i < lineIn.size())
					{
						term3 += lineIn[i];
						i++;
						temp = lineIn[i];
						if (temp == "," || temp == "'" || temp == ".")
						{
							
							i++;
							if (temp == "'")
								i++;
							temp = lineIn[i];
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
	int sheesh = 0;

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