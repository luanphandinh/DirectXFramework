#pragma once
#include<string>
#include<vector>
#include<sstream>
#include <d3dx9.h>

using namespace std;

vector<string> splitString(const string &input, char seperate);

string formatScoreString(int num,string str);


bool isRectangleIntersectedInTopLeft(RECT rect1, RECT rect2);

/*
Kiểm tra hai hình chữ nhật có chồng lên nhau không.
rect1, rect2: hai hình chữ nhật cần kiểm tra.
return: true nếu có chồng lên nhau, ngược lại là false
*/
bool isRectangleIntersectedInDescartes(RECT rect1, RECT rect2);

bool isContains(RECT rect1, RECT rect2);