#include <iostream>
#include <fstream>
#include <cstdio>
#define N 50
using namespace std;

int construct_optimal_bst(int root[][N], int i, int j) {
	static int d = 0;
	int parent = root[i - 1][j - 1];
	int lchild, rchild;
	if (i <= parent - 1) {
		lchild = construct_optimal_bst(root, i, parent - 1);
		cout << 'k' << lchild << " Ϊ " << 'k' << parent << " ������" << endl;
	}
	else 
		cout << 'd' << d++ << " Ϊ " << 'k' << parent << " ������" << endl;
	
	if (parent + 1 <= j) {
		rchild = construct_optimal_bst(root, parent + 1, j);
		cout << 'k' << rchild << " Ϊ " << 'k' << parent << " ���Һ���" << endl;
	}
	else 
		cout << 'd' << d++ << " Ϊ " << 'k' << parent << " ���Һ���" << endl;

	return parent;

}
int main(){
	std::ifstream cin("lab3_test.txt");
	if (0 == cin.is_open()) {
		cout << "�Ҳ����ļ�lab3_test.txt!" << endl;
		getchar();
		return 0;
	}
	int n;
	cin >> n;
	int root[N][N] = { 0 };
	for (int i = 0; i < n; i++)
		for (int j = i; j < n; j++)
			cin >> root[i][j];
	cout << 'k' << root[0][n-1] << "Ϊ��" << endl;
	construct_optimal_bst(root, 1, n);
	getchar();
	return 0;
}