#pragma once
#include<string>
#include<vector>
#include<sstream>
#include <d3dx9.h>

using namespace std;

vector<string> splitString(const string &input, char seperate);

string formatScoreString(int num,string str);

/*
Kiểm tra hai hình chữ nhật có chồng lên nhau không.
rect1, rect2: hai hình chữ nhật cần kiểm tra.
return: true nếu có chồng lên nhau, ngược lại là false
*/
bool isRectangleIntersected(RECT rect1, RECT rect2);