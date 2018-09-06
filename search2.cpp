#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include <locale>
#include <cctype>
#include <vector>
#include <string>
#include <cstdlib>
#include <iterator>
#include <algorithm>

using namespace std;

vector<string> intersectVector(vector<string> f1, vector<string> f2);
void mapPrint(map<string, vector<string> >);
void lowercaseWord(string&);
bool mapCheck(string, map<string, vector<string> >, string);
void writeOutAND(ofstream& ofile, vector<string>);
void candidateSetInsertion(set<string>&, vector<string>);
bool vectorFind(string, vector<string>);
bool descendingPageRank(const pair<string, double>&, const pair<string,double>&);

bool descendingPageRank(const pair<string,double> &a, const pair<string,double> &b)
{
       return a.second > b.second;
}

bool vectorFind(string value, vector<string> vectorCheck)
{
	for(unsigned u = 0; u < vectorCheck.size(); u++)
	{
		if(vectorCheck.at(u) == value)
		{
			return true;
		}
	}
	return false;
}


void candidateSetInsertion(set<string> &candidateSet, vector<string> searchResult)
{
	for(unsigned i = 0; i < searchResult.size(); i++)
	{
		candidateSet.insert(searchResult.at(i));
	}
}

void writeOutAND(ofstream& ofile, vector<string> result)
{
	ofile << result.size() << endl;

	for(unsigned i = 0; i < result.size(); i++)
	{
		ofile << result.at(i) << endl;
	}
}


bool mapCheck(string word, map<string, vector<string> > mapToCheck, string link)
{
	map<string, vector<string> >::iterator it;

 	it = mapToCheck.find(word);
 	if(it == mapToCheck.end())
 	{
 		return true;
 	}

 	for(unsigned i = 0; i < mapToCheck[word].size(); i++)
 	{
 		if(mapToCheck[word].at(i) == link)
 		{
 			return false;
 		}
 	}

 	return true;

}

// lowers queries and keys
void lowercaseWord(string& wordToLower)
{
	locale loc;

	for(unsigned i = 0; i < wordToLower.length(); i++)
	{
		wordToLower.at(i) = tolower(wordToLower.at(i), loc);
	}
}

// for testing
void mapPrint(map<string, vector<string> > mapName)
{
	map<string, vector<string> >::iterator it;

	cout << "MAP: " << endl;
	for(it = mapName.begin(); it != mapName.end(); ++it)
	{
		cout << (*it).first << ": " << endl;
		vector <string> fileLocationsInMap = (*it).second;
		for (unsigned i = 0; i < fileLocationsInMap.size(); i++)
		{
			cout << fileLocationsInMap[i] << " " << endl;
		}
		cout << endl;
	}
}

// AND function
vector<string> intersectVector(vector<string> f1, vector<string> f2)
{
	vector<string> result;
	for(unsigned i = 0; i < f1.size(); i++)
	{
		for(unsigned j = 0; j < f2.size(); j++)
		{
			if(f1.at(i) == f2.at(j))
			{
				result.push_back(f1.at(i));
			}
		}
	}

	return result;
}


int main(int argc, char *argv[])
{
	if (argc < 1) {
	   cerr << "Please provide all files" << endl;
	   return -1;
    }

	// CONFIG ----------------------------------------------------
	ifstream ifile;
	ofstream ofile;
	stringstream ss;

	char* cFile = argv[1];
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
	string indexStr = "";
	string queryStr = "";
	string outputStr = "";
	string restartStr = "";
	string stepStr = "";
	string varName = "";

	const char* indexFile;
	const char* queryFile;
	const char* outputFile;

	double restartProb;
	double stepNumber;

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
				indexStr = configCollection.at(j+2);
				indexFile = indexStr.c_str();
			}

			if(configCollection.at(j) == "QUERY_FILE")
			{
				queryStr = configCollection.at(j+2);
				queryFile = queryStr.c_str();
			}

			if(configCollection.at(j) == "OUTPUT_FILE")
			{
				outputStr = configCollection.at(j+2);
				outputFile = outputStr.c_str();
			}

			if(configCollection.at(j) == "RESTART_PROBABILITY")
			{
				restartStr = configCollection.at(j+2);
				restartProb = atof(restartStr.c_str());
			}

			if(configCollection.at(j) == "STEP_NUMBER")
			{
				stepStr = configCollection.at(j+2);
				stepNumber = atof(stepStr.c_str());
			}
		}

		configCollection.clear();
		ss.clear();
	}
	ss.clear();



	// END CONFIG -----------------------------------------------

 	map<string, vector<string> > bigMap;
 	map<string, vector<string> > incomingMap;
 	map<string, vector<string> > outgoingMap;

 	set<string> setOfPages;

 	// INDEX -----------------------------------------------------
	// read the index
	ifile.open(indexFile);
	string filePath;
	vector<string> filePathVector;
	while(!ifile.eof())
	{
		getline(ifile, filePath);
		filePathVector.push_back(filePath);
	}
	ifile.close();

	// convert the string to const char* to open the file
	string relativePath;
	string link;
	string content;
	string word;
	const char* path;


	// PARSE THE CONTENT -------------------------------------------
	// open and read everything
	for(unsigned i = 0; i < filePathVector.size(); i++)
	{

		relativePath = filePathVector.at(i);
		path = relativePath.c_str();
		ifile.open(path);

		while(!ifile.eof())
		{
			getline(ifile, content);

			for(unsigned j = 0; j < content.length(); j++)
			{
				if(isspace(content.at(j)))
				{
					// insert to map
					// make sure word is not empty
					// lower it, make key, push the path it came from to its vector
					// boi
					if(word != "")
					{
						lowercaseWord(word);
						bigMap[word];
						if(mapCheck(word, bigMap, relativePath) == true)
						{
							bigMap[word].push_back(relativePath);
						}
						word.clear();
						continue;
					}
				}
				else if(content.at(j) == '(')
				{
					// found a link
					// add the links to the link maps for later use
					// j++ to skip the '('
					j++;
					while(content.at(j) != ')')
					{
						link = link + content.at(j);
						//cout << "LINK: " << link << endl;
						j++;
					}

					// later on if I need to go to "hidden" files not in the index
					// filePathVector.push_back(link);
					outgoingMap[relativePath];
					outgoingMap[relativePath].push_back(link);
					incomingMap[link];
					incomingMap[link].push_back(relativePath);

					link.clear();
				}
				else if(!isdigit(content.at(j)) && !isalpha(content.at(j)))
				{

					// if a special character exists, stop and push
					if(word != "")
					{
						lowercaseWord(word);
						bigMap[word];
						if(mapCheck(word, bigMap, relativePath) == true)
						{
							bigMap[word].push_back(relativePath);
						}
						word.clear();
						continue;
					}
				}
				else
				{
					word = word + content.at(j);
				}
			}
			// capture the last word of the line
			if(word != "")
			{
				lowercaseWord(word);
				bigMap[word];
				if(mapCheck(word, bigMap, relativePath) == true)
				{
					bigMap[word].push_back(relativePath);
				}
			}
			word.clear();
			continue;

		}
		// capture the last word of the file
		if(word != "")
		{
			lowercaseWord(word);
			bigMap[word];
			if(mapCheck(word, bigMap, relativePath) == true)
			{
				bigMap[word].push_back(relativePath);
			}
		}

		word.clear();
		ifile.close();
	}


	// QUERY -------------------------------------------------------
	// read the query
	stringstream collect;
	string storage;
	string command;
	string fileLineContent;
	string ANDsearchWord;
	string ORsearchWord;
	string fullQuery;

	ifile.open(queryFile);
	string queryLine;
	vector<string> queryVector;
	vector<string> storageVector;

	while(!ifile.eof())
	{
		getline(ifile, queryLine);
		queryVector.push_back(queryLine);
	}
	ifile.close();

	ofile.open(outputFile);

	for(unsigned i = 0; i < queryVector.size(); i++)
	{
		// all extraction
		set<string> candidateSet;
		map<string, double> fileToVisitCount;
		command = queryVector.at(i);
		fullQuery = queryVector.at(i);
		ss << queryVector.at(i);
		ss >> command;
		//cout << command << endl;

		if(ss.eof())
		{
			fullQuery = command;
			lowercaseWord(fullQuery);
			storageVector.push_back(fullQuery);

			vector<string> intersectResult;

			ANDsearchWord = storageVector.at(0);

			// the vector that is here is going to have all the file names to print
			ofile << bigMap[ANDsearchWord].size() << endl;

			for(unsigned l = 0; l < bigMap[ANDsearchWord].size(); l++)
			{
				ofile << bigMap[ANDsearchWord].at(l) << endl;
			}

			ss.str("");
		}
		else if(command == "AND") // || !ss, empty stringstream
		{
			// pull off "AND" command to use the string
			//ofile << "AND: " << endl;
			stringstream collect;
			fullQuery.erase(0,4);
			lowercaseWord(fullQuery);

			//cout << fullQuery << endl;
			collect << fullQuery;

			// store the query for use
			while(collect >> storage)
			{
				storageVector.push_back(storage);
			}

			vector<string> intersectResult;

				// find both if exists
				if(storageVector.size() == 1)
				{
					ANDsearchWord = storageVector.at(0);
					// the vector that is here is going to have all the file names to print
					ofile << bigMap[ANDsearchWord].size() << endl;

					for(unsigned l = 0; l < bigMap[ANDsearchWord].size(); l++)
					{
						ofile << bigMap[ANDsearchWord].at(l) << endl;
					}
				}
				else if(storageVector.size() == 2)
				{
					// SEARCH --------------------------------------------------
					intersectResult = intersectVector(bigMap[storageVector.at(0)],
					bigMap[storageVector.at(1)]);
					// -------------------------------------------------------


					// CANDIDATE--------------------------------------------
					candidateSetInsertion(candidateSet, intersectResult);
					set<string>::iterator setIt;
					for(setIt = candidateSet.begin(); setIt != candidateSet.end(); setIt++)
					{
						candidateSetInsertion(candidateSet, outgoingMap[*setIt]);

						candidateSetInsertion(candidateSet, incomingMap[*setIt]);
					}
					// ----------------------------------------------------------

					// PAGERANK-----------------------------------------------------------

					map<string, double> pageRankMap;
					map<string, double>::iterator mapIt;

					for(setIt = candidateSet.begin(); setIt != candidateSet.end(); setIt++)
					{
						pageRankMap[*setIt] = 1.0/candidateSet.size();
					}

					double t = stepNumber;
					double eVal = restartProb;

					const double selfConstVal = eVal*(1.0/(candidateSet.size()));

					for(int i = 0; i < t; i++)
					{

						for(setIt = candidateSet.begin(); setIt != candidateSet.end(); setIt++)
						{
							double prob = 0.0;
							double duVar = outgoingMap[*setIt].size()+1;
							prob += (pageRankMap[*setIt])*(1.0/duVar);

							for(unsigned j = 0; j < incomingMap[*setIt].size(); j++)
							{
								duVar = outgoingMap[incomingMap[*setIt].at(j)].size()+1;
								prob += (pageRankMap[incomingMap[*setIt].at(j)])*(1.0/duVar);

							}

							prob = (1.0-eVal)*prob + selfConstVal;
							pageRankMap[*setIt] = prob;
						}
					}

					// END PAGERANK --------------------------------------------------------------

					// WRITE OUT -------------------------------------------------------
					vector< pair<string, double> > outVector;
					for(mapIt = pageRankMap.begin(); mapIt != pageRankMap.end(); ++mapIt)
					{
						string s = (*mapIt).first;
						double d = (*mapIt).second;
						outVector.push_back(make_pair(s, d));
					}
					sort(outVector.begin(), outVector.end(), descendingPageRank);

					ofile << candidateSet.size() << endl;
					for(unsigned e = 0; e < outVector.size(); e++)
					{
						ofile << outVector[e].first << endl;
					}
					// ------------------------------------------------------------------
					outVector.clear();
					pageRankMap.clear();

				}
				else if(storageVector.size() > 2)
				{
					intersectResult = intersectVector(bigMap[storageVector.at(0)],
					bigMap[storageVector.at(1)]);

					for(unsigned j = 2; j < storageVector.size(); j++)
					{
						intersectResult = intersectVector(intersectResult,
						bigMap[storageVector.at(j)]);
					}

					// CANDIDATE--------------------------------------------
					candidateSetInsertion(candidateSet, intersectResult);
					set<string>::iterator setIt;
					for(setIt = candidateSet.begin(); setIt != candidateSet.end(); setIt++)
					{
						candidateSetInsertion(candidateSet, outgoingMap[*setIt]);

						candidateSetInsertion(candidateSet, incomingMap[*setIt]);
					}
					// ----------------------------------------------------------

					// PAGERANK-----------------------------------------------------------

					map<string, double> pageRankMap;
					map<string, double>::iterator mapIt;

					for(setIt = candidateSet.begin(); setIt != candidateSet.end(); setIt++)
					{
						pageRankMap[*setIt] = 1.0/candidateSet.size();
					}

					double t = stepNumber;
					double eVal = restartProb;

					const double selfConstVal = eVal*(1.0/(candidateSet.size()));

					for(int i = 0; i < t; i++)
					{

						for(setIt = candidateSet.begin(); setIt != candidateSet.end(); setIt++)
						{
							double prob = 0.0;
							double duVar = outgoingMap[*setIt].size()+1;
							prob += (pageRankMap[*setIt])*(1.0/duVar);

							for(unsigned j = 0; j < incomingMap[*setIt].size(); j++)
							{
								duVar = outgoingMap[incomingMap[*setIt].at(j)].size()+1;
								prob += (pageRankMap[incomingMap[*setIt].at(j)])*(1.0/duVar);

							}

							prob = (1.0-eVal)*prob + selfConstVal;
							pageRankMap[*setIt] = prob;
						}
					}

					// END PAGERANK-----------------------------------------------------------

					// WRITE OUT -------------------------------------------------------
					vector< pair<string, double> > outVector;
					for(mapIt = pageRankMap.begin(); mapIt != pageRankMap.end(); ++mapIt)
					{
						string s = (*mapIt).first;
						double d = (*mapIt).second;
						outVector.push_back(make_pair(s, d));
					}
					sort(outVector.begin(), outVector.end(), descendingPageRank);

					ofile << candidateSet.size() << endl;
					for(unsigned e = 0; e < outVector.size(); e++)
					{
						ofile << outVector[e].first << endl;
					}
					// ------------------------------------------------------------------
					outVector.clear();
					pageRankMap.clear();

				}

			candidateSet.clear();
			intersectResult.clear();
			collect.str("");
			ss.str("");

		}
		else if(command == "OR")
		{
			// pull off "OR" command
			stringstream collect;
			fullQuery.erase(0,3);
			lowercaseWord(fullQuery);

			//ofile << "OR: " << endl;
			collect << fullQuery;
			while(collect >> storage)
			{
				storageVector.push_back(storage);
			}

			if(storageVector.size() > 0)
			{
				for(unsigned l = 0; l < storageVector.size(); l++)
				{
					for(unsigned p = 0; p < bigMap[storageVector.at(l)].size(); p++)
					{
						setOfPages.insert(bigMap[storageVector.at(l)].at(p));
					}
				}
			}

			vector<string> unionResult;
			set<string>::iterator itSet;

			for(itSet = setOfPages.begin(); itSet != setOfPages.end(); itSet++)
			{
				unionResult.push_back(*itSet);
			}

			// CANDIDATE--------------------------------------------
			candidateSetInsertion(candidateSet, unionResult);
			set<string>::iterator setIt;
			for(setIt = candidateSet.begin(); setIt != candidateSet.end(); setIt++)
			{
				candidateSetInsertion(candidateSet, outgoingMap[*setIt]);

				candidateSetInsertion(candidateSet, incomingMap[*setIt]);
			}
			// ----------------------------------------------------------

			// PAGERANK-----------------------------------------------------------

					map<string, double> pageRankMap;
					map<string, double>::iterator mapIt;

					for(setIt = candidateSet.begin(); setIt != candidateSet.end(); setIt++)
					{
						pageRankMap[*setIt] = 1.0/candidateSet.size();
					}

					double t = stepNumber;
					double eVal = restartProb;

					const double selfConstVal = eVal*(1.0/(candidateSet.size()));

					for(int i = 0; i < t; i++)
					{

						for(setIt = candidateSet.begin(); setIt != candidateSet.end(); setIt++)
						{
							double prob = 0.0;
							double duVar = outgoingMap[*setIt].size()+1;
							prob += (pageRankMap[*setIt])*(1.0/duVar);

							for(unsigned j = 0; j < incomingMap[*setIt].size(); j++)
							{
								duVar = outgoingMap[incomingMap[*setIt].at(j)].size()+1;
								prob += (pageRankMap[incomingMap[*setIt].at(j)])*(1.0/duVar);

							}

							prob = (1.0-eVal)*prob + selfConstVal;
							pageRankMap[*setIt] = prob;
						}
					}

			// END PAGERANK-----------------------------------------------------------

			// WRITE OUT -------------------------------------------------------
			vector< pair<string, double> > outVector;
			for(mapIt = pageRankMap.begin(); mapIt != pageRankMap.end(); ++mapIt)
			{
				string s = (*mapIt).first;
				double d = (*mapIt).second;
				outVector.push_back(make_pair(s, d));
			}
			sort(outVector.begin(), outVector.end(), descendingPageRank);

			ofile << candidateSet.size() << endl;
			for(unsigned e = 0; e < outVector.size(); e++)
			{
				ofile << outVector[e].first << endl;
			}
			// ------------------------------------------------------------------
			outVector.clear();
			pageRankMap.clear();
			candidateSet.clear();
			setOfPages.clear();
			unionResult.clear();
			collect.str("");
			ss.str("");
		}
		else if(command == "PRINT")
		{
			// pull off "PRINT" command
			stringstream collect;
			fullQuery.erase(0,6);
			lowercaseWord(fullQuery);

			//ofile << "PRINT: " << endl;
			collect << fullQuery;

			while(collect >> storage)
			{
				storageVector.push_back(storage);
				//cout << storage << endl;
			}


			for(unsigned k = 0; k < storageVector.size(); k++)
			{
				string hold = storageVector.at(k);

				ifile.open(hold.c_str());

				// print out the name of the requested file
				ofile << hold << endl;

				// print the content of the page without links
				while(!ifile.eof())
				{
					getline(ifile,fileLineContent);

					for(unsigned v = 0; v < fileLineContent.length(); v++)
					{
						// avoid the ( ) links, otherwise print
						if(fileLineContent.at(v) == '(')
						{
							while(fileLineContent.at(v) != ')')
							{
								v++;
							}
						}
						else
						{
							ofile << fileLineContent.at(v);
						}
					}
					ofile << endl;
				}

				ifile.close();
			}

			collect.str("");
			ss.str("");
		}
		else if(command == "INCOMING")
		{
			// pull off "INCOMING" command
			stringstream collect;
			fullQuery.erase(0,9);
			lowercaseWord(fullQuery);

			//ofile << "INCOMING: " << endl;

			vector<string> incomingResults;

			// for every file named, output the incoming links
			ofile << incomingMap[fullQuery].size() << endl;

			incomingResults = incomingMap[fullQuery];

			for(unsigned b = 0; b < incomingResults.size(); b++)
			{
				ofile << incomingResults.at(b) << endl;
			}

			fullQuery = "";
			incomingResults.clear();
			collect.str("");
			ss.str("");
		}
		else if(command == "OUTGOING")
		{
			// pull off "OUTGOING" command
			stringstream collect;
			fullQuery.erase(0,9);
			lowercaseWord(fullQuery);

			//ofile << "OUTGOING: " << endl;

			vector<string> outgoingResults;

			// for every file named, output the outgoing links
			ofile << outgoingMap[fullQuery].size() << endl;

			outgoingResults = outgoingMap[fullQuery];

			for(unsigned b = 0; b < outgoingResults.size(); b++)
			{
				ofile << outgoingResults.at(b) << endl;
			}


			fullQuery = "";
			outgoingResults.clear();
			collect.str("");
			ss.str("");
		}
		else
		{
			// error if command not given
			ofile << "Invalid query" << endl;
			collect.str("");
			ss.str("");
		}

		// clear the storage so other commands have space
		storageVector.clear();


	}

	ifile.close();
	ofile.close();

	return 0;
}






	/*
						for(unsigned j = 0; j < incomingMap[candidateSet].size(); j++)
						{
							double prob = probabilityVector.at(j)*ouotgoingMap[].size();
							prob += prob;
						}


					for(unsigned i = 0; i < candidateSet.size(); i++)
					{
						probabilityVector.push_back(1.0/candidateSet.size());
					}


						*/



					// self loop
					// make the map and start visits at 0

/*
					for(setIt = candidateSet.begin(); setIt != candidateSet.end(); setIt++)
					{
						//map creation
						fileToVisitCount[*setIt] = 0;

						//self loop creation
						cout << "CHECK: " << *setIt << endl;
						bool selfCheck = vectorFind(*setIt, outgoingMap[*setIt]);
						if(selfCheck == false)
						{
							// might permanently change it
							outgoingMap[*setIt].push_back(*setIt);
						}

					}

					int randomIndex = rand() % candidateSet.size();
					// the iterator moves to a random file
					map<string, double>::iterator mapFileIt = fileToVisitCount.begin();
					advance(mapFileIt, randomIndex);
					string fileRand = (*mapFileIt).first;

					cout << "RANDOM START: " << fileRand << endl;

					double eVal = restartProb;

					double t = stepNumber;

					for(double i = 0; i < t; i++)
					{
						//mapFileIt = fileToVisitCount.at(fileRand);
						//(*mapFileIt).second += 1;

						fileToVisitCount[fileRand] += 1;

						double rVal = (double(rand()) / double(RAND_MAX));
						cout << "r: " << rVal << endl;

						if(rVal < eVal)
						{
							randomIndex = rand() % candidateSet.size();
							map<string, double>::iterator mapFileIt = fileToVisitCount.begin();
							advance(mapFileIt, randomIndex);

							fileRand = (*mapFileIt).first;
							continue;
						}
						else
						{
							int randOutLinkInt = rand() % outgoingMap[fileRand].size();
							fileRand = outgoingMap[fileRand].at(randOutLinkInt);
						}
					}

					for(mapFileIt = fileToVisitCount.begin(); mapFileIt != fileToVisitCount.end(); ++mapFileIt)
					{
						cout << "FIRST: " << (*mapFileIt).first << ": " << "COUNT: " << (*mapFileIt).second << endl;
					}

					// divide all the visits by t to get the probability
					cout << "AFTER DIVIDING T" << endl;
					for(mapFileIt = fileToVisitCount.begin(); mapFileIt != fileToVisitCount.end(); ++mapFileIt)
					{
						(*mapFileIt).second = (*mapFileIt).second/t;
					}

					for(mapFileIt = fileToVisitCount.begin(); mapFileIt != fileToVisitCount.end(); ++mapFileIt)
					{
						cout << "FIRST: " << (*mapFileIt).first << ": " << "COUNT: " << (*mapFileIt).second << endl;
					}
*/
