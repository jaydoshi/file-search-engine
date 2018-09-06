#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include <locale>
#include <cctype>
#include <vector>
#include <stack>
#include <string>
#include <cstdlib>
#include <algorithm>
using namespace std;

//string parseLink(string fileLineContent);
void viewStack(stack<string>&);
bool checkIfSearched(string link, vector<string> allFilePaths);
bool checkVector(string link, vector<string> orderedDFSPaths);

void viewStack(stack<string> &stackToView)
{
	cout << "STACK BEFORE" << endl;
	stack<string> stackFile;
	while(!stackToView.empty())
	{
		string topStack;
		topStack = stackToView.top();
		cout << topStack << endl;
		stackToView.pop();
		stackFile.push(topStack);

	}

	while(!stackFile.empty())
	{
		string topStackFile = stackFile.top();
		stackFile.pop();
		stackToView.push(topStackFile);
	}

	cout << "STACK END" << endl;

}


bool checkIfSearched(string link, set<string> exploredPaths)
{
	set<string>::iterator i = find(exploredPaths.begin(), exploredPaths.end(), link);

	if(i == exploredPaths.end())
	{
		// the link has not been searched before
		return false;
	}
	else
	{
		return true;
	}
}

bool checkVector(string link, vector<string> orderedDFSPaths)
{
	vector<string>::iterator i = find(orderedDFSPaths.begin(), orderedDFSPaths.end(), link);

	if(i == orderedDFSPaths.end())
	{
		// the link has not been searched before
		return false;
	}
	else
	{
		return true;
	}
}

int main(int argc, char *argv[])
{

	if (argc < 2) {
	   cerr << "Please provide all files" << endl;
	   return -1;
    }

    if (argc > 2) {
	   cerr << "Maximum files allowed is 1" << endl;
	   return -1;
    }

	char* cFile = argv[1];

	ifstream ifile;
	ofstream ofile;
	stringstream ss;

	// open config file --------------------
	ifile.open(cFile);
	string configFileLine;
	vector<string> configFileVector;
	while(!ifile.eof())
	{
		getline(ifile, configFileLine);
		configFileVector.push_back(configFileLine);
	}
	ifile.close();

	// going line by line of the config file
	// query file is not used in the crawler
	string indexFile = "";
	string queryFile = "";
	string outputFile = "";
	string varName = "";
	string hold;
	vector<string> configCollection;

	for(unsigned i = 0; i < configFileVector.size(); i++)
	{
		string configLineParse = configFileVector.at(i);
		ss << configLineParse;
		while(ss >> hold)
		{
			configCollection.push_back(hold);
		}

		for(unsigned j = 0; j < configCollection.size(); j++)
		{
			// assuming it's always after the equals sign
			if(configCollection.at(j) == "INDEX_FILE")
			{
				indexFile = configCollection.at(j+2);
			}

			if(configCollection.at(j) == "QUERY_FILE")
			{
				queryFile = configCollection.at(j+2);
			}

			if(configCollection.at(j) == "OUTPUT_FILE")
			{
				outputFile = configCollection.at(j+2);
			}
		}

		configCollection.clear();
		ss.clear();
	}

	//open seed file ------------------
	string seedFile = indexFile;
	const char* charSeedFile = seedFile.c_str();

	ifile.open(charSeedFile);
	string seedPath;
	vector<string> seedPathVector;
	while(!ifile.eof())
	{
		getline(ifile, seedPath);
		seedPathVector.push_back(seedPath);
	}
	ifile.close();

	// seedPathVector holds all starting file paths from which to DFS
	vector<string> allFilePaths;
	allFilePaths = seedPathVector;
	stack<string> filePathStack;
	set<string> exploredPaths;
	vector<string> orderedDFSPaths;

	// stack insertion
	for(int i = allFilePaths.size()-1; i >= 0; i--)
	{
		filePathStack.push(allFilePaths.at(i));
	}

	// set insertion
	for(int i = allFilePaths.size()-1; i >= 0; i--)
	{
		exploredPaths.insert(allFilePaths.at(i));
	}


	while(!filePathStack.empty())
	{
		string link;
		string topVal = filePathStack.top();

		// check before pushing to vector
		bool checkVectorForLink = checkVector(topVal, orderedDFSPaths);
		if(checkVectorForLink == false)
		{
			orderedDFSPaths.push_back(topVal);
		}

		filePathStack.pop();
		const char* fileNameC = (topVal).c_str();

		ifile.open(fileNameC);

			while(!ifile.eof())
			{
				string fileLineContent;
				getline(ifile,fileLineContent);
				for(unsigned j = 0; j < fileLineContent.length(); j++)
				{
					// grab the links
					if(fileLineContent.at(j) == '(')
					{
						j++;
						while(fileLineContent.at(j) != ')')
						{
							link = link + fileLineContent.at(j);
							j++;
						}
					}

					if(link != "")
					{
						bool check = checkIfSearched(link, exploredPaths);
						bool checkVectorForLink = checkVector(link, orderedDFSPaths);
						if(check == false)
						{
							filePathStack.push(link);
							exploredPaths.insert(link);
						}

						if(checkVectorForLink == false)
						{
							orderedDFSPaths.push_back(link);
						}
					}

					link.clear();
				}
			}

		ifile.close();
	}

	ofile.open(outputFile.c_str());

	for(unsigned i = 0; i < orderedDFSPaths.size(); i++)
	{
		ofile << orderedDFSPaths.at(i) << endl;
	}

	ofile.close();

}

// 0 - havent been there
	// 1 - in stack
	// 2 - popped off

	/*

	set holds anything explored or pushed into stack

	push seeds into set & stack

	while(stack is not empty)
	{
		top value of stack
			find all links
			push onto stack all links
			push links into set

			if no links/children
				pop it off stack


	}


	*/


/*
		while(ss >> hold)
		{
			cout << hold << endl;

			if(hold == "#")
			{
			}

			if(hold == "INDEX_FILE")
			{

			}

			if(hold == "OUTPUT_FILE")
			{

			}


		}
		cout << "----------------" << endl;
		ss.clear();
*/

/*
		// on the config line to parse
		for(unsigned j = 0; j < configLineParse.length(); j++)
		{
			if(configLineParse.at(j) == '#')
			{
				cout << "YEEEEEEEEEEEEEEEEEEEE" << endl;
				continue;
			}

			if(configLineParse.at(j) == '=')
			{

			}

			varName = varName + configLineParse.at(j);

		}

		cout << "AAAAAAAAAAAAAAAA" << varName << endl;
		varName.clear();
		cout << configFileVector.at(i) << endl;

		cout << endl;
		cout << endl;
		cout << endl;
*/


/*
	for(unsigned i = 0; i < allFilePaths.size(); i++)

	{
		//searchDFS(, allFilePaths.at(i));
		//filePathStack.push(allFilePaths.at(i));
		const char* fileNameC = (allFilePaths.at(i)).c_str();

		string link;

		// DFS --------------------------------------------------------

		ifile.open(pathFromVector);

			while(!ifile.eof())
			{
				string fileLineContent;
				getline(ifile,fileLineContent);
				for(unsigned j = 0; j < fileLineContent.length(); j++)
				{
					// grab the links
					if(fileLineContent.at(j) == '(')
					{
						j++;
						while(fileLineContent.at(j) != ')')
						{
							link = link + fileLineContent.at(j);
							j++;
						}
					}

					if(link != "")
					{
						bool check = checkIfSearched(link, allFilePaths);
						if(check == false)
						{


							//filePathStack.push(link);
						}
					}


					link.clear();
				}
			}

		ifile.close();


		cout << "_______" << endl;

		cout << "STACK RESULT" << endl;

		for(unsigned q = 0; q < filePathStack.size(); q++)
		{
			cout << filePathStack.top() << endl;
			filePathStack.pop();
		}
		cout << "STACK END" << endl;

}
*/

/*

	cout << "STACK BEFORE" << endl;
	stack<string> stackFile;
	while(!filePathStack.empty())
	{
		string topStack;
		topStack = filePathStack.top();
		cout << topStack << endl;
		filePathStack.pop();
		stackFile.push(topStack);

	}

	while(!stackFile.empty())
	{
		string topStackFile = stackFile.top();
		stackFile.pop();
		filePathStack.push(topStackFile);
	}

	cout << "STACK END" << endl;
*/
