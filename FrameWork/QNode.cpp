#include "QNode.h"

vector<string> QNode::ActiveObject;

QNode::QNode(int id, int level, RECT bound, QNode* parent)
{
	this->_id = id;
	this->_level = level;
	this->_bound = bound;
	this->_parent = parent;
	for (int i = 0; i < 4; i++)
		_childs[i] = NULL;
}

void QNode::insertObject(string name)
{
	this->_listObject.push_back(name);
}

RECT QNode::getBound()
{
	return _bound;
}

INT64 QNode::getId()
{
	return _id;
}

int QNode::getLevel()
{
	return _level;
}

QNode* QNode::getParent()
{
	return _parent;
}

void QNode::setParent(QNode* parent)
{
	if (_parent != parent)
		_parent = parent;
}

QNode** QNode::getChilds()
{
	return _childs; 
}

void QNode::setChilds(QNode* childs[4])
{
	for (int i = 0; i < 4; i++)
	{
		this->_childs[i] = childs[i];
	}
}

bool QNode::isLeaf()
{
	if (this->_childs[0] == NULL)
		return true;
	return false;
}

vector<string> QNode::getAllObject()
{
	return _listObject;
}

QNode* QNode::loadQuadTree(const string path)
{
	QNode* node = nullptr;
	pugi::xml_document doc;

	//Mở file và đọc
	xml_parse_result result = doc.load_file(path.data(), pugi::parse_default | pugi::parse_pi);
	if (result == false)
	{
		return nullptr;
	}

	pugi::xml_node rootxml = doc.first_child();
	node = initNode(rootxml);
	loadChild(rootxml, node);
	return node;
}

QNode* QNode::initNode(xml_node& node)
{
	RECT bound;
	int id = node.attribute("Id").as_int();
	int level = node.attribute("Level").as_int();
	bound.top = node.attribute("Y").as_int();
	bound.left = node.attribute("X").as_int();
	bound.right = bound.left + node.attribute("Width").as_int();
	bound.bottom = bound.top + node.attribute("Height").as_int();
	QNode* qnode = new QNode(id, level, bound, NULL);
	return qnode;
}

void QNode::loadChild(xml_node& node, QNode* parent)
{
	QNode* childs[4] = { NULL };
	//Lấy danh sách các node con trong xml file từ node
	auto childNodes = node.children();
	string nodeName;
	int childIndex = 0;
	//Với mỗi node xml trong danh sách
	for each (xml_node childNode in childNodes)
	{
		//Với nodeName là Object thì insert object vào _listObject của node
		//Nếu là QNode thì đệ quy xuống để init node và addOBject
		nodeName = childNode.name();
		if (nodeName == "Objects")
		{
			string text = childNode.text().as_string();
			auto objectName = splitString(text, ' ');
			for each (string name in objectName)
			{
				parent->insertObject(name);
			}
		}
		else if (nodeName == "QNode")
		{
			QNode* qnode = initNode(childNode);
			qnode->setParent(parent);
			loadChild(childNode, qnode);
			childs[childIndex] = qnode;
			childIndex++;
		}
	}
	parent->setChilds(childs); 
}


vector<string> QNode::GetActiveObject(RECT bound, bool botLeft = false)
{
	if (botLeft)
	{
		//Đổi từ top-left sang bot-left
		bound.bottom = WINDOW_HEIGHT - bound.bottom;
		bound.top = WINDOW_HEIGHT - bound.top;
	}

	QNode::ActiveObject.clear();
	this->fetchActiveObject(bound);
	return QNode::ActiveObject;
}

//Lấy danh sách các đối tượng mà node của nó giao với hình chữ nhật bound,thông thường bound là khung màn hình
void QNode::fetchActiveObject(RECT bound)
{
	//Đầu tiên cần kiểm tra xem vùng bound ta cần lấy object
	//Có nằm trong,giao,hoặc chứa bound của QNode hay ko
	if (isContains(this->_bound, bound) || isRectangleIntersected(this->_bound, bound) || isContains(bound, this->_bound))
	{
		//Nếu là node lá thì tiến hành kiểm tra chèn object vào 
		//static ActiveObject nếu ko thì bắt đầu kiểm tra con của nó
		if (this->isLeaf() == true)
		{
			//Duyệt hết các obj trong node hiện tại
			for (string obj : _listObject)
			{
				//Duyệt hết các phần từ trong ActiveObject list,nếu chưa có obj của node hiện tại thì
				//add vào
				auto it = std::find(ActiveObject.begin(), ActiveObject.end(), obj);
				if (it == ActiveObject.end() || it._Ptr == nullptr)
				{
					ActiveObject.push_back(obj);
				}
			}
		}
		//Nếu node hieenjt ại không phải là node lá,thì duyệt xuống 
		//các node con của nó để thực hiện việc add ActiveObject
		else
		{
			for (int i = 0; i < 4; i++)
			{
				_childs[i]->fetchActiveObject(bound);
			}
		}
	}
}

