

using namespace std;

#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <iomanip>

void mySwitch(set<string> dictionarySet);
void createDictionary(ifstream &inFile, set <string> &dictionarySet);
string fileIterator(istream &input, char delim);
int scrollChars(istream &instructionFile);
int scrollWhiteSpace(istream &cin);
void initializationMenu();
void menu1A();

set<string> dictionarySet;

int main()
{
	
	return 0;
}

void initializationMenu()
{
	cout << right;
	cout << setw(45) << setfill(' ') << "Initialization Menu"
		<< "\n\t----------------------------------------------------"
		<< "\n\t1. Add dictionary file"
		<< "\n\t2. Use default dictionary"
		<< "\n\t3. Add text file to check"
		<< "\n\t4. Use default text file"
		<< "\n\t5. Use only default files"
		<< "\n\t6. Quit"
		<< "\n\n\t==>> ";
}

void mySwitch (set<string> dictionarySet)
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
			case 1: //Add Instruction File
			{
				//param = menu1A();
			}
			break;
			case 2: //Add Instruction Line By Line
			{
				//param = menu1B();
			}
			break;
			case 3: //Display Empty Registers
			{
				//MyCache.printRegisters();
			}
			break;
			case 4: //Display Instructions
			{
				//printInstructions();//no stepping
			}
			break;
			case 5: //Display Empty Memory
			{
				//MyMemory.printMemory();
			}
			break;
			case 6: //Help
			{
				//call help file needed
				cout << "\t\nHelp File incomplete..." << endl;
				//bitset<16> tt("0000010011010111");
				//LDR(tt);
				//bitset<16> aa = MyCache.getGeneralPurposeRegisters_GPRs(3);
				//STR(aa);
				//bitset<16> dd("1010010000001001");
				//LDX(dd);
				//bitset<16> xx("1010100000110010");
				//STX(xx);
				//bitset<16> comp("0100010000000000");
				//CMP(comp);
				//cout << "ZF: " << MyCache.get_ZF() << " CF: " << MyCache.get_CF() << " SF: " << MyCache.get_SF() << endl;
			//bitset<16> load_val("0000000000001010");
			//int load_v;
				//bitset<16> comp_val("0100010000000000");
				//bitset<16> head("");
				/*while (MyCache.get_ZF() != 1) {
					MyCache.setGeneralPurposeRegisters_GPRs(0, load_val);
					CMP(MyCache.getIndexRegister_X0());
					load_v = load_val.to_ulong();
					load_v--;
					cout << MyCache.getIndexRegister_X0().to_ulong() << endl;
					cout << load_v << endl;
					bitset<16> temp(load_v);
					load_val = temp;
					cout << "ZF: " << MyCache.get_ZF() << " CF: " << MyCache.get_CF() << " SF: " << MyCache.get_SF() << endl;
				}*/
				
				//cout << "ZF: " << MyCache.get_ZF() << " CF: " << MyCache.get_CF() << " SF: " << MyCache.get_SF() << endl;
				//JGE();
			}
			break;
			case 7: //Quit
			{
				//exit
				cout << "Bye!" << endl;
			}
			break;
			}
	} while (param != 7);

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

		if (!dictionaryFile.is_open())
			cerr << "\n\tCould not open file\n\n";


	} while (!dictionaryFile.is_open());

	createDictionary(dictionaryFile, dictionarySet);
}

void createDictionary(ifstream &inFile, set <string> &dictionarySet)
{
	int wordCount = 0;

	while (!inFile.eof())
	{
		//Acquire words from file - BEGIN
		string newWord = inFile.getline;
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
