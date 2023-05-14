#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

class node;
class Graph;

class node {
	int value;					// 섬을 구분하기 위한 value값

	node* red = NULL;			// 빨간색 다리에 연결되어 있는 섬의 주소
	node* yellow = NULL;		// 노란색 다리에 연결되어 있는 섬의 주소
	node* green = NULL;			// 초록색 다리에 연결되어 있는 섬의 주소
	node* blue = NULL;			// 파란색 다리에 연결되어 있는 섬의 주소
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
	node* start;	  			// 빨간모자가 출발하는 섬
	node* grandma;				// 할머니가 계시는 섬
	node* current;				// 빨간모자의 현재 위치
	node* wolf;					// 늑대 위치

	node* graph;				// 그래프
	int nodeSize;				// 섬의 수
	int linkSize;				// 다리의 수

	int* valuelist;				// valu의 값 순서 저장 (노드에 value값 주기 위함)
	//node flower;
public:
	Graph(int nodeSize) {
		while (nodeSize < 0) {
			cout << "올바르지 않은 섬의 개수입니다. 섬의 개수는 0 이상으로 설정하세요\n";
			cin >> nodeSize;
		}

		this->nodeSize = nodeSize + 2;
		graph = new node[this->nodeSize];
		if (!graph) { cout << "동적할당에 실패하였습니다.\n"; exit(1); }

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
	void makePath();			// 주어진 섬의 개수를 토대로 게임 필드 만들기
	void moveWolf();			// 늑대 이동
	int checkCrash();			// 늑대와 빨간모자사이의 거리 반환
	bool currentIsGrand();		// 할머니 댁 도착
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
	int linkNum;									// 연결할 다리의 개수
	int drawLink;									// 연결할 다리의 종류
	int nextValue;									// 연결할 섬의 value
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
		selectButton[num] = "빨간색 다리 ";
		num++;
	}
	if (wolf->getYellow()) {
		selectButton[num] = "노란색 다리 ";
		num++;
	}
	if (wolf->getGreen()) {
		selectButton[num] = "초록색 다리 ";
		num++;
	}
	if (wolf->getBlue()) {
		selectButton[num] = "파란색 다리 ";
		num++;
	}

	locate = rand() % num ;

	if (selectButton[locate] == "빨간색 다리 ") wolf = wolf->getRed();
	if (selectButton[locate] == "노란색 다리 ") wolf = wolf->getYellow();
	if (selectButton[locate] == "초록색 다리 ") wolf = wolf->getGreen();
	if (selectButton[locate] == "파란색 다리 ") wolf = wolf->getBlue();
}

int printScript(Graph graph, node a) {
	string selectButton[5];
	int num = 0;
	int select;
	int value;

	if (a.red) {
		cout << "빨간색 다리 ";
		selectButton[num] = "빨간색 다리 ";
		num++;
	}
	if (a.yellow) {
		cout << "노란색 다리 ";
		selectButton[num] = "노란색 다리 ";
		num++;
	}
	if (a.green) {
		cout << "초록색 다리 ";
		selectButton[num] = "초록색 다리 ";
		num++;
	}
	if (a.blue) {
		cout << "파란색 다리 ";
		selectButton[num] = "파란색 다리 ";
		num++;
	}

	cout << "가 보입니다.\n\n";
	for (int i = 0; i < num; i++) {
		cout << '\t' << i + 1 << ' ';
		cout << selectButton[i] << endl;
	}
	cout << "\n어디로 이동하시겠습니까? 번호를 입력하세요 : ";
	cin >> select;

	select--;
	cout << selectButton[select] << "를 건너갑니다.\n";

	if (selectButton[select] == "빨간색 다리 ") {
		value = a.red->getValue();
	}
	if (selectButton[select] == "노란색 다리 ") {
		value = a.yellow->getValue();
	}
	if (selectButton[select] == "초록색 다리 ") {
		value = a.green->getValue();
	}
	if (selectButton[select] == "파란색 다리 ") {
		value = a.blue->getValue();
	}
	if (value == graph.nodeSize) {
		cout << "바로 앞에 할머니 댁이 보입니다.\n";
		return 0;
	}
	cout << "\n빨간모자는 " << selectButton[select] << "를 건넌 후 " << value << "번 섬에 도착했습니다.\n";

	if (selectButton[select] == "빨간색 다리 ") return 1;
	if (selectButton[select] == "노란색 다리 ") return 2;
	if (selectButton[select] == "초록색 다리 ") return 3;
	if (selectButton[select] == "파란색 다리 ") return 4;
}

bool Graph::currentIsGrand() {
	if (current == grandma) return true;
	else return false;
}

void printtutorial() {
	cout << "할머니가 편찮으시다는 소식을 들은 빨간모자는 병문안을 가기로 마음먹었습니다.\n";
	cout << "하지만 할머니댁으로 가는 길이 순탄하지만은 않습니다.\n";
	cout << "늑대를 피해 빨간모자가 할머니 댁에 무사히 도착할 수 있도록 이동하세요.\n\n";

	cout << "할머니댁으로 가는 길은 여러 개의 섬으로 구성되어 있습니다.\n";
	cout << "할머니댁 까지 가는 길의 모든 섬의 개수를 입력합니다.\n";
	cout << "섬의 개수: ";
}

int main() {
	int nodeSize;
	int wolfselect;

	printtutorial();
	cin >> nodeSize;
	cout << "늑대를 추가하시겠습니까? (예 : 1, 아니요 : 2) : ";
	cin >> wolfselect;

	Graph graph(nodeSize);
	graph.makePath();

	int select;
	while (!graph.currentIsGrand()) {
		if (wolfselect == 1 && graph.getCurrent() == graph.getWolf()) {
			cout << "늑대에게 잡혔습니다! 게임을 종료합니다.";
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
	cout << "\n축하합니다!! 무사히 도착했습니다!!";
}