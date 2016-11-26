#pragma once
#include"define.h"
#include"BaseObject.h"
/*
	Class QNode được implement tương tự như trong mapEditor
*/
class QNode
{
public:
	QNode(int id, int level, RECT bound, QNode* parent);
	void insertObject(string name);
	RECT getBound();
	INT64 getId();
	int getLevel();
	QNode* getParent();
	void setParent(QNode* parent);
	QNode** getChilds();
	void setChilds(QNode* childs[4]);
	bool isLeaf();

	vector<string> getAllObject();
	
	static void loadChild(xml_node& node, QNode* parent);
	static QNode* loadQuadTree(const string path);
	static QNode* initNode(xml_node& node);
	/*
		Lấy danh sách các active Object từ bound
			@bound: vùng hình chữ nhật dể lấy danh sách object
			@botLeft: dùng tọa độ decac nếu true,top left nếu false
	*/
	vector<string> GetActiveObject(RECT bound, bool botLeft = false);
	void fetchActiveObject(RECT bound);
	static vector<string> ActiveObject;
	~QNode();
private:
	RECT _bound;
	INT64 _id;
	int _level;
	QNode* _parent;
	QNode* _childs[4];

	vector<string> _listObject;

};

