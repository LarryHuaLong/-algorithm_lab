#include <iostream>
#include <fstream>
#include <cstdio>
#define N_MAX  10
#define INFINITE  0x3fffffff
#define NIL 0

using namespace std;

void print_path(int (*Pi)[N_MAX][N_MAX], int i, int j) {
	int k = (*Pi)[i][j];
	if (k == i + 1) {
		cout << k << "->" << j + 1;
		return;
	}
	print_path(Pi, i, k-1);
	cout << "->" << j + 1;
	return;
}

int main() {
	std::ifstream cin("lab4_test.txt");
	if(0 == cin.is_open()){
		cout << "找不到文件lab4_test.txt!" << endl;
		getchar();
		return 0;
	}
	int n, m;
	cin >> n >> m;
	int D[N_MAX + 1][N_MAX][N_MAX], Pi[N_MAX + 1][N_MAX][N_MAX];
	//初始化辅助数组
	for (int k = 0; k < n + 1; k++)
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++) {
				D[k][i][j] = i == j ? 0 : INFINITE;//如果i == j,则D[k][i][j] = 0,否则初始化为无穷大
				Pi[k][i][j] = NIL;
			}
	//D[0] = W,将边的权值直接输入到D[0]
	for(int k = 0;k<m;k++){
		int i, j, w;
		cin >> i >> j >> w;
		D[0][i][j] = w;
		Pi[0][i][j] = i+1;
	}
	//计算D[k]和Pi[k]
	for (int k = 1; k < n + 1; k++)
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++) {
				if (i == j)
					continue;
				int d_ij = D[k - 1][i][j];
				int d_ik_kj = D[k - 1][i][k-1] + D[k - 1][k-1][j];
				D[k][i][j] = d_ij < d_ik_kj ? d_ij : d_ik_kj;
				Pi[k][i][j] = d_ij <= d_ik_kj ? Pi[k - 1][i][j] : Pi[k - 1][k - 1][j];
			}
	//根据Pi[k]构造路径并输出路径权值
	for(int i = 0;i<n;i++)
		for(int j = 0; j<n;j++){
			if(i == j)
				continue;
			cout << i + 1 << "-->" << j + 1 << " weight = " << D[n][i][j] << "\tpath: ";
			print_path(&Pi[n], i, j);
			cout << endl;
		}
	getchar();
	return 0;
}

