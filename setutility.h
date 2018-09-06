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
