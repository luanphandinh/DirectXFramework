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

bool isRectangleIntersectedInTopLeft(RECT rect1, RECT rect2) {
	float left = rect1.left - rect2.right;
	float top = rect1.bottom - rect2.top;
	float right = rect1.right - rect2.left;
	float bottom = rect1.top - rect2.bottom;

	//  Chồng lên nhau khi :
	//  left < 0 && right > 0 && top > 0 && bottom < 0
	//  
	if (left > 0 || right < 0 || top < 0 || bottom > 0)
		return false;

	return true;
}

// check trong Decac
bool isRectangleIntersectedInDescartes(RECT rect1, RECT rect2) {
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


bool isContains(RECT rect1, RECT rect2)
{
	float left = rect1.left - rect2.left;
	float right = rect1.right - rect2.left;
	float bottom = rect1.bottom - rect2.bottom;
	float top = rect1.top - rect2.top;

	//Nếu rect2 nằm trong rect1
	//Thì left và right hoặc top và bottom phải 1 cái dương 1 cái âm
	//Nếu cả 2 đều âm hoặc cả 2 đều dương thì rect2 nằm ngoài rect1
	if (left * right > 0)
	{
		return false;
	}
	if (top * bottom > 0)
	{
		return false;
	}
	return true;
}
