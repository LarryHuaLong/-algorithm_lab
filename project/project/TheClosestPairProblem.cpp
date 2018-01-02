#include <iostream>
#include <fstream>
#include <cstdio>
#include <list>
#include <math.h>
#include <algorithm>

using namespace std;

enum side {
	side_default,
	side_left,
	side_right
};

class node {
public:
	node() {};
	node(double x, double y) {
		this->x = x;
		this->y = y;
		node_side = side_default;
		node_index = -1;
	}
	int node_index;
	side node_side;
	double x;
	double y;
};

class line {
public:
	line(node node1, node node2) {
		this->node1 = node1;
		this->node2 = node2;
		distance = sqrt((node1.x - node2.x)*(node1.x - node2.x)
			+ (node1.y - node2.y)*(node1.y - node2.y));
	}
	
	double	distance;
	node	node1;
	node	node2;
};

bool CompNodeByX(node node1, node node2) {
	return node1.x < node2.x;
}

bool CompNodeByY(node node1, node node2) {
	return node1.y < node2.y;
}

std::list<line>* findclosest(std::list<node>* P, std::list<node>* Q) {
	std::list<node>::iterator IterQ;
	//如果只有两个点则返回这两个点的线
	if (P->size() ==  2){
		std::list<line>* pveclines = new std::list<line>;
		line pnewline(P->front(),P->back());
		pveclines->push_back(pnewline);
		return pveclines;
	}
	//如果只有三个点则返回这三个点的最近点对
	if (P->size() == 3) {
		std::list<line>* pveclines = new std::list<line>;
		std::list<node>::iterator IterP0 = P->begin();
		std::list<node>::iterator IterP1 = ++(P->begin());
		std::list<node>::iterator IterP2 = --(P->end());
		line pnewline1(*IterP0, *IterP1);
		line pnewline2(*IterP1, *IterP2);
		line pnewline3(*IterP0, *IterP2);
		pveclines->push_back(pnewline1);
		pveclines->push_back(pnewline2);
		pveclines->push_back(pnewline3);
		double destance = pnewline1.distance;//假定line1最短
		if (pnewline2.distance < destance) 
			destance = pnewline2.distance;
		if (pnewline3.distance < destance)
			destance = pnewline3.distance;
		//pveclines->remove_if([](line line) { return line.distance > destance; });
		std::list<line>::iterator Iter = pveclines->begin();
		while(Iter != pveclines->end()){
			if (Iter->distance > destance)
				pveclines->erase(Iter++);
			else
				Iter++;
		}
		if (pveclines->back().distance > destance) {
			pveclines->pop_back();
		}
		return pveclines;
	}
	//否则将P表分成两个部分，分治
	std::list<node> Pl, Pr, Ql, Qr;
	//P分组
	int i = 0;
	for (auto elem : *P) {//P中点分组
		if (i < P->size() / 2) 
			Pl.push_back(elem);
		else 
			Pr.push_back(elem);
		i++;
	}
	//Q分组到Ql和Qr中,并标记点的side
	IterQ = Q->begin();
	while (IterQ != Q->end()) {
		if (IterQ->node_index <= Pl.back().node_index) {
			IterQ->node_side = side_left;
			Ql.push_back(*IterQ);
		}
		else {
			IterQ->node_side = side_right;
			Qr.push_back(*IterQ);
		}
		IterQ++;
	}
	
	std::list<line>* pvecL = findclosest(&Pl, &Ql);
	std::list<line>* pvecR = findclosest(&Pr, &Qr);

	double delta;
	std::list<line> vecLineTemp;//暂存目前最近点对
	if(pvecL->front().distance < pvecR->front().distance){
		delta = pvecL->front().distance;
		for (auto elem : *pvecL)
			vecLineTemp.push_back(elem);
		//释放pvecR中的边
		pvecR->clear();
	}
	else if (pvecL->front().distance > pvecR->front().distance) {
		delta = pvecR->front().distance;
		for (auto elem : *pvecR)
			vecLineTemp.push_back(elem);
		//释放pvecL中的边
		pvecL->clear();
	}
	else {
		delta = pvecL->front().distance;
		for (auto elem : *pvecL)
			vecLineTemp.push_back(elem);
		for (auto elem : *pvecR)
			vecLineTemp.push_back(elem);
	}
	//删除Q中不在带内的点 
	IterQ = Q->begin();
	while (IterQ != Q->end()) {
		if ((Pl.back().x - (*IterQ).x )> delta || ((*IterQ).x - (Pr.front().x) > delta))
			Q->erase(IterQ++);
		else
			IterQ++;
	}
	//找Δy≤delta 的点对
	std::list<line> pvecC;//暂存中间的最近点对
	std::list<node>::iterator IterQi = Q->begin();
	for (; IterQi != Q->end(); IterQi++) {
		std::list<node>::iterator IterQj = (++IterQi)--;
		for (; IterQj != Q->end(); IterQj++) {
			if ((IterQi->y - IterQj->y) > delta)
				break;
			line newline(*IterQi, *IterQj);
			if (newline.distance < delta) {
				delta = newline.distance;
				pvecC.clear();//清空表
				pvecC.push_back(newline);
			}
			else if (newline.distance == delta) {
				if (newline.node1.node_side == newline.node2.node_side)
					continue;//如果Qi和Qj在同一边则不加入pvecC
				else
					pvecC.push_back(newline);
			}
		}
	}
	std::list<line>* pveclines = new std::list<line>;
	if (pvecC.size() == 0) {
		for (auto elem : vecLineTemp)
			pveclines->push_back(elem);
	}
	else if (vecLineTemp.front().distance < pvecC.front().distance) {
		for (auto elem : vecLineTemp)
			pveclines->push_back(elem);
		//释放pvecC中的边
		pvecC.clear();
	}
	else if (vecLineTemp.front().distance > pvecC.front().distance) {
		for (auto elem:pvecC)
			pveclines->push_back(elem);
		//释放vecLineTemp中的边
		vecLineTemp.clear();
	}
	else {
		for (auto elem : vecLineTemp)
			pveclines->push_back(elem);
		for (auto elem : pvecC)
			pveclines->push_back(elem);
	}
	return pveclines;
}

int main(){

	std::ifstream cin("lab1_test.txt");
	if (0 == cin.is_open()) {
		cout << "找不到文件lab1_test.txt!" << endl;
		getchar();
		return 0;
	}

	std::list<node> Px;
	std::list<node> Qy;
	//输入点集
	int n = 0;
	cin >> n;
	for (int i = 0; i < n; i++) {
		char c;
		double x, y;
		cin >> c >> x >> c >> y >> c;
		Px.push_back(node(x, y));
	}
	//预排序
	Px.sort(CompNodeByX);
	int i = 0;
	std::list<node>::iterator IterNode = Px.begin();
	while(IterNode != Px.end()){
		IterNode->node_index = i++;
		Qy.push_back(*IterNode);
		IterNode++;
	}
	Qy.sort(CompNodeByY);
	//查找最近点对
	std::list<line>* closestline = findclosest(&Px, &Qy);
	//输出最近点对
	std::list<line>::iterator IterLine = closestline->begin();
	for (; IterLine != closestline->end(); IterLine++) {
		cout << "(" << IterLine->node1.x << ", " << IterLine->node1.y << ") --> ";
		cout << "(" << IterLine->node2.x << ", " << IterLine->node2.y << ")";
		cout << " distance = " << IterLine->distance << endl;
	}
	getchar();
	return 0;
}