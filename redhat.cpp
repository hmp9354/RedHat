#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

class node;
class Graph;

class node {
	int value;					// ���� �����ϱ� ���� value��

	node* red = NULL;			// ������ �ٸ��� ����Ǿ� �ִ� ���� �ּ�
	node* yellow = NULL;		// ����� �ٸ��� ����Ǿ� �ִ� ���� �ּ�
	node* green = NULL;			// �ʷϻ� �ٸ��� ����Ǿ� �ִ� ���� �ּ�
	node* blue = NULL;			// �Ķ��� �ٸ��� ����Ǿ� �ִ� ���� �ּ�
public:
	void setValue(int value) { this->value = value; }
	void setRed(node* next) { red = next; }
	void setYellow(node* next) { yellow = next; }
	void setGreen(node* next) { green = next; }
	void setBlue(node* next) { blue = next; }

	int getValue() { return value; }
	node* getRed() { return red; }
	node* getYellow() { return yellow; }
	node* getGreen() { return green; }
	node* getBlue() { return blue; }

	friend int printScript(Graph graph, node a);
};

class Graph {
	node* start;	  			// �������ڰ� ����ϴ� ��
	node* grandma;				// �ҸӴϰ� ��ô� ��
	node* current;				// ���������� ���� ��ġ
	node* wolf;					// ���� ��ġ

	node* graph;				// �׷���
	int nodeSize;				// ���� ��
	int linkSize;				// �ٸ��� ��

	int* valuelist;				// valu�� �� ���� ���� (��忡 value�� �ֱ� ����)
	//node flower;
public:
	Graph(int nodeSize) {
		while (nodeSize < 0) {
			cout << "�ùٸ��� ���� ���� �����Դϴ�. ���� ������ 0 �̻����� �����ϼ���\n";
			cin >> nodeSize;
		}

		this->nodeSize = nodeSize + 2;
		graph = new node[this->nodeSize];
		if (!graph) { cout << "�����Ҵ翡 �����Ͽ����ϴ�.\n"; exit(1); }

		current = &graph[0];
		start = &graph[0];
		start->setValue(0);
		grandma = &graph[this->nodeSize - 1];
		grandma->setValue(this->nodeSize - 1);
		srand((unsigned int)time(NULL));
	}
	void setCurrent(node* newnode) { current = newnode; }
	void setLinkSize();
	void setValuelist();

	node* getCurrent() { return current; }
	node* getWolf() { return wolf; }
	void makePath();			// �־��� ���� ������ ���� ���� �ʵ� �����
	void moveWolf();			// ���� �̵�
	int checkCrash();			// ����� �������ڻ����� �Ÿ� ��ȯ
	bool currentIsGrand();		// �ҸӴ� �� ����
	friend int printScript(Graph graph, node a);
};

void Graph::setLinkSize() {
	int maxLinksize = 0;
	int minLinksize = 0;

	if (nodeSize == 2) { return; }
	for (int i = 0; i < nodeSize - 2; i++) {
		maxLinksize += i;
	}

	maxLinksize += 2;
	minLinksize = nodeSize - 1;

	if (minLinksize > maxLinksize) linkSize = minLinksize;
	else {
		linkSize = minLinksize;
		while (minLinksize <= linkSize && linkSize <= maxLinksize) {
			linkSize = rand() % maxLinksize + 1;
		}
	}
}

void Graph::setValuelist() {
	valuelist = new int[nodeSize - 2];
	current->setValue(0);
	grandma->setValue(nodeSize);

	int i;
	for (i = 0; i < nodeSize - 2; i++) {
		valuelist[i] = rand() % (nodeSize - 2) + 1;
		for (int j = 0; j < i; j++) {
			if (valuelist[j] == valuelist[i]) {
				i--;
				break;
			}
		}
	}
}

void Graph::makePath() {
	int linkNum;									// ������ �ٸ��� ����
	int drawLink;									// ������ �ٸ��� ����
	int nextValue;									// ������ ���� value
	int size = linkSize;
	bool check = false;

	setLinkSize();
	setValuelist();

	int locate;
	locate = rand() % (nodeSize - 2) + 1;
	wolf = &graph[locate];

	for (int i = 1; i < nodeSize - 1; i++) {
		graph[i].setValue(valuelist[i - 1]);
	}

	for (int i = 0; i < nodeSize - 1; i++) {
		linkNum = rand() % 3 + 1;
		check = false;
		while (size - linkNum < size - i - 1) {
			linkNum = rand() % 3 + 1;
		}

		if (i == 0 || i == nodeSize - 2) {
			linkNum = 1;
		}

		for (int j = 0; j < linkNum; j++) {
			drawLink = rand() % 4;

			if (drawLink == 0 && (graph[i].getRed() == NULL)) {
				graph[i].setRed(&graph[i + 1]);
				graph[i + 1].setRed(&graph[i]);
				check = true;
			}
			if (drawLink == 1 && (graph[i].getYellow() == NULL)) {
				graph[i].setYellow(&graph[i + 1]);
				graph[i + 1].setYellow(&graph[i]);
				check = true;
			}
			if (drawLink == 2 && (graph[i].getGreen() == NULL)) {
				graph[i].setGreen(&graph[i + 1]);
				graph[i + 1].setGreen(&graph[i]);
				check = true;
			}
			if (drawLink == 3 && (graph[i].getBlue() == NULL)) {
				graph[i].setBlue(&graph[i + 1]);
				graph[i + 1].setBlue(&graph[i]);
				check = true;
			}
			if (check)size--;
			else if (check == false) {
				j--;
			}
		}
	}
}

void Graph::moveWolf() {
	int num = 0;
	string selectButton[4];
	int locate;

	if (wolf->getRed()) {		
		selectButton[num] = "������ �ٸ� ";
		num++;
	}
	if (wolf->getYellow()) {
		selectButton[num] = "����� �ٸ� ";
		num++;
	}
	if (wolf->getGreen()) {
		selectButton[num] = "�ʷϻ� �ٸ� ";
		num++;
	}
	if (wolf->getBlue()) {
		selectButton[num] = "�Ķ��� �ٸ� ";
		num++;
	}

	locate = rand() % num ;

	if (selectButton[locate] == "������ �ٸ� ") wolf = wolf->getRed();
	if (selectButton[locate] == "����� �ٸ� ") wolf = wolf->getYellow();
	if (selectButton[locate] == "�ʷϻ� �ٸ� ") wolf = wolf->getGreen();
	if (selectButton[locate] == "�Ķ��� �ٸ� ") wolf = wolf->getBlue();
}

int printScript(Graph graph, node a) {
	string selectButton[5];
	int num = 0;
	int select;
	int value;

	if (a.red) {
		cout << "������ �ٸ� ";
		selectButton[num] = "������ �ٸ� ";
		num++;
	}
	if (a.yellow) {
		cout << "����� �ٸ� ";
		selectButton[num] = "����� �ٸ� ";
		num++;
	}
	if (a.green) {
		cout << "�ʷϻ� �ٸ� ";
		selectButton[num] = "�ʷϻ� �ٸ� ";
		num++;
	}
	if (a.blue) {
		cout << "�Ķ��� �ٸ� ";
		selectButton[num] = "�Ķ��� �ٸ� ";
		num++;
	}

	cout << "�� ���Դϴ�.\n\n";
	for (int i = 0; i < num; i++) {
		cout << '\t' << i + 1 << ' ';
		cout << selectButton[i] << endl;
	}
	cout << "\n���� �̵��Ͻðڽ��ϱ�? ��ȣ�� �Է��ϼ��� : ";
	cin >> select;

	select--;
	cout << selectButton[select] << "�� �ǳʰ��ϴ�.\n";

	if (selectButton[select] == "������ �ٸ� ") {
		value = a.red->getValue();
	}
	if (selectButton[select] == "����� �ٸ� ") {
		value = a.yellow->getValue();
	}
	if (selectButton[select] == "�ʷϻ� �ٸ� ") {
		value = a.green->getValue();
	}
	if (selectButton[select] == "�Ķ��� �ٸ� ") {
		value = a.blue->getValue();
	}
	if (value == graph.nodeSize) {
		cout << "�ٷ� �տ� �ҸӴ� ���� ���Դϴ�.\n";
		return 0;
	}
	cout << "\n�������ڴ� " << selectButton[select] << "�� �ǳ� �� " << value << "�� ���� �����߽��ϴ�.\n";

	if (selectButton[select] == "������ �ٸ� ") return 1;
	if (selectButton[select] == "����� �ٸ� ") return 2;
	if (selectButton[select] == "�ʷϻ� �ٸ� ") return 3;
	if (selectButton[select] == "�Ķ��� �ٸ� ") return 4;
}

bool Graph::currentIsGrand() {
	if (current == grandma) return true;
	else return false;
}

void printtutorial() {
	cout << "�ҸӴϰ� �������ôٴ� �ҽ��� ���� �������ڴ� �������� ����� �����Ծ����ϴ�.\n";
	cout << "������ �ҸӴϴ����� ���� ���� ��ź�������� �ʽ��ϴ�.\n";
	cout << "���븦 ���� �������ڰ� �ҸӴ� �쿡 ������ ������ �� �ֵ��� �̵��ϼ���.\n\n";

	cout << "�ҸӴϴ����� ���� ���� ���� ���� ������ �����Ǿ� �ֽ��ϴ�.\n";
	cout << "�ҸӴϴ� ���� ���� ���� ��� ���� ������ �Է��մϴ�.\n";
	cout << "���� ����: ";
}

int main() {
	int nodeSize;
	int wolfselect;

	printtutorial();
	cin >> nodeSize;
	cout << "���븦 �߰��Ͻðڽ��ϱ�? (�� : 1, �ƴϿ� : 2) : ";
	cin >> wolfselect;

	Graph graph(nodeSize);
	graph.makePath();

	int select;
	while (!graph.currentIsGrand()) {
		if (wolfselect == 1 && graph.getCurrent() == graph.getWolf()) {
			cout << "���뿡�� �������ϴ�! ������ �����մϴ�.";
			return 0;
		}
		select = printScript(graph, *(graph.getCurrent()));
		if (select == 0) { break; }
		if (select == 1) {
			graph.setCurrent(graph.getCurrent()->getRed());
		}
		if (select == 2) {
			graph.setCurrent(graph.getCurrent()->getYellow());
		}
		if (select == 3) {
			graph.setCurrent(graph.getCurrent()->getGreen());
		}
		if (select == 4) {
			graph.setCurrent(graph.getCurrent()->getBlue());
		}
		if (wolfselect == 1) {
			graph.moveWolf();			
		}
	}
	cout << "\n�����մϴ�!! ������ �����߽��ϴ�!!";
}