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

string formatScoreString(int num, string str)
{
	string strFormatted;

	int length = str.length();

	for (int i = 0; i < num - length; i++)
	{
		strFormatted += "0";
	}

	strFormatted += str;

	return strFormatted;
}