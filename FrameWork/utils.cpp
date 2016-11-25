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

// check trong Decac
bool isRectangleIntersected(RECT rect1, RECT rect2) {
	float left = rect1.left - rect2.right;
	float top = rect1.top - rect2.bottom;
	float right = rect1.right - rect2.left;
	float bottom = rect1.bottom - rect2.top;

	//  Chồng lên nhau khi :
	//  left < 0 && right > 0 && top > 0 && bottom < 0
	//
	if (left > 0 || right < 0 || top < 0 || bottom > 0)
		return false;

	return true;
}
