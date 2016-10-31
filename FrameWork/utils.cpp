#include"utils.h"

vector<string> splitString(const string &input, char seperate)
{
	vector<string> output;
	stringstream ss(input);

	string item;

	while (getline(ss, item, seperate))
	{
		output.push_back(item);
	}

	return output;
}