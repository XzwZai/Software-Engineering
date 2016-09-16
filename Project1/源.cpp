#include<stdio.h>
#include<fstream>
#include<iostream>
#define MAX 4000
#define maxnum 500
#pragma warning (disable : 4996) 
using namespace std;

struct station {
	char name[30];
	char linename[30];
	int no;
	station *path;
};

struct Line {
	char name[30];
	int sta_num = 0;
	station *stas[100];
};

struct city {
	int line_num = 0;
	int sta_num = 0;
	Line lines[100];
	station stas[500];
	int link[500][500];
	int link_c[500][500];
};
city bj;
Line line;
station sta;

void Dijkstra(int n, int v, int *dist, int *prev, int c[maxnum][maxnum])
{
	bool s[maxnum];
	for (int i = 0; i < n; ++i)
	{
		dist[i] = c[v][i];
		s[i] = 0;
		if (dist[i] == MAX)
			prev[i] = 0;
		else
			prev[i] = v;
	}
	dist[v] = 0;
	s[v] = 1;
	for (int i = 1; i < n; ++i)
	{
		int tmp = MAX;
		int u = v;
		for (int j = 0; j < n; ++j) {
			if ((!s[j]) && dist[j] < tmp) {
				u = j;
				tmp = dist[j];
			}
		}
		s[u] = 1;
		for (int j = 0; j < n; ++j) {
			if ((!s[j]) && c[u][j] < MAX) {
				int newdist = dist[u] + c[u][j];
				if (newdist < dist[j]) {
					dist[j] = newdist;
					prev[j] = u;
				}
			}
		}
	}
}

void searchPath(int *prev, int v, int u)
{
	int que[maxnum];
	int num;
	int tot = 1;
	char linename[30] = " ";
	que[tot] = u;
	tot++;
	int tmp = prev[u];
	while (tmp != v)
	{
		que[tot] = tmp;
		tot++;
		tmp = prev[tmp];
	}
	que[tot] = v;
	num = tot;
	for (int i = tot; i > 1; --i) {
		if (strcmp(bj.stas[que[i]].linename, bj.stas[que[i-1]].linename)){
			num--;
		}
	}
	cout << num << endl;
	for (int i = tot; i >= 1; --i) {
		if (!strcmp(linename, " ")) {
			strcpy(linename, bj.stas[que[i]].linename);

		}
		else if (!strcmp(linename, bj.stas[que[i]].linename)) {
		}
		else {
			if (i != 1) {
				strcpy(linename, bj.stas[que[i]].linename);
				cout << "换乘地铁" << bj.stas[que[i]].linename;
				continue;
			}
			continue;
		}
		cout << endl << bj.stas[que[i]].name;
	}
}
int main(int argc,char *argv[])
{
	char staname1[30];
	char staname2[30];
	char buffer[500];
	int dist[500], distbest = -1;
	char *part;
	int i, j, no1 = 0, no2 = 0,tag,type = -1,lastno;
	int prev[500],prevbest[500];
	int exit1 = 0, exit2 = 0;
	ifstream in("F:\\beijing-subway.txt");
	if (!in.is_open()) {
		cout << "fail to read map\n" << endl;
		return 0;
	}
	for (i = 0; i < 500; i++) {
		for (j = 0; j < 500; j++) {
			bj.link[i][j] = MAX;
			bj.link_c[i][j] = MAX;
		}
	}
	while (!in.eof()) {
		in.getline(buffer, 500);
		part = strtok(buffer, " ");
		if (part == NULL) {
			continue;
		}
		strcpy(line.name, part);
		lastno = -1;
		part = strtok(NULL, " ");
		if (part == NULL) {
			cout << "wrong map" << endl;
			return 0;
		}
		if (!strcmp(part, "单向")) {
			part = strtok(NULL, " ");
			while (part != NULL) {
				//cout << part << endl;
				for (i = 0; i < line.sta_num; i++) {
					if (!strcmp(part, line.stas[i]->name)) {
						bj.link[lastno][line.stas[i]->no] = 2;
						bj.link_c[lastno][line.stas[i]->no] = 2;
						lastno = line.stas[i]->no;
						break;
					}
				}
				if (i != line.sta_num) {
					part = strtok(NULL, " ");
					continue;
				}
				for (i = 0; i < bj.sta_num; i++) {
					if (!strcmp(part, bj.stas[i].name)) {
						bj.link[bj.sta_num][i] = 1;
						bj.link[i][bj.sta_num] = 1;
						bj.link_c[bj.sta_num][i] = 100;
						bj.link_c[i][bj.sta_num] = 100;
					}
				}
				strcpy(sta.name, part);
				strcpy(sta.linename, line.name);
				sta.no = bj.sta_num;
				bj.stas[bj.sta_num++] = sta;
				line.stas[line.sta_num++] = &bj.stas[bj.sta_num - 1];

				if (line.sta_num > 1) {
					bj.link[lastno][line.stas[line.sta_num - 1]->no] = 2;
					bj.link_c[lastno][line.stas[line.sta_num - 1]->no] = 2;
				}
				lastno = line.stas[line.sta_num - 1]->no;
				part = strtok(NULL, " ");
			}
		}
		while (part != NULL) {
			//cout << part << endl;
			for (i = 0; i < line.sta_num; i++) {
				if (!strcmp(part, line.stas[i]->name)) {
					bj.link[lastno][line.stas[i]->no] = 2;
					bj.link[line.stas[i]->no][lastno] = 2;
					bj.link_c[lastno][line.stas[i]->no] = 2;
					bj.link_c[line.stas[i]->no][lastno] = 2;
					lastno = line.stas[i]->no;
					break;
				}
			}
			if (i != line.sta_num) {
				part = strtok(NULL, " ");
				continue;
			}
			for (i = 0; i < bj.sta_num; i++) {
				if (!strcmp(part, bj.stas[i].name)) {
					bj.link[bj.sta_num][i] = 1;
					bj.link[i][bj.sta_num] = 1;
					bj.link_c[bj.sta_num][i] = 100;
					bj.link_c[i][bj.sta_num] = 100;
				}
			}
			strcpy(sta.name, part);
			strcpy(sta.linename, line.name);
			sta.no = bj.sta_num;
			bj.stas[bj.sta_num++] = sta;
			line.stas[line.sta_num++] = &bj.stas[bj.sta_num - 1];
			
			if (line.sta_num > 1) {
				bj.link[line.stas[line.sta_num - 1]->no][lastno] = 2;
				bj.link[lastno][line.stas[line.sta_num - 1]->no] = 2;
				bj.link_c[line.stas[line.sta_num - 1]->no][lastno] = 2;
				bj.link_c[lastno][line.stas[line.sta_num - 1]->no] = 2;
			}
			lastno = line.stas[line.sta_num - 1]->no;
			part = strtok(NULL, " ");
		}
		bj.lines[bj.line_num++] = line;
		line.sta_num = 0;
	}
	//cout << endl << bj.sta_num << endl;
	/*for (i = 0; i < bj.line_num; i++) {
		cout << "线路 " << bj.lines[i].name << endl;
		for (j = 0; j < bj.lines[i].sta_num; j++) {
			cout << "站点 " << bj.lines[i].stas[j]->name << '\t';
		}
		cout << endl;
	}*/
	//cout << "end" << endl;
	if (argc == 1) {
		while (cin >> staname1) {
			for (i = 0; i < bj.line_num; i++) {
				if (strcmp(bj.lines[i].name, staname1) == 0) {
					for (j = 0; j < bj.lines[i].sta_num; j++) {
						cout << bj.lines[i].stas[j]->name << '\t';
					}
					break;
				}
			}
			cout << endl;
			if (i == bj.line_num) {
				cout << "没这个站" << endl;
			}
		}
	}
	else {
		if (!strcmp(argv[1], "-b")) {
			type = 1;
			//cout << argv[1] << argv[2] << endl;
		}
		else if (!strcmp(argv[1], "-c")) {
			type = 2;
		}
		strcpy(staname1,argv[2]);
		strcpy(staname2,argv[3]);
		for (i = 0; i < bj.sta_num; i++) {
			if (!strcmp(bj.stas[i].name, staname1)) {
				exit1 = 1;
				if (type == 1) {
					Dijkstra(bj.sta_num, i, dist, prev, bj.link);
					//cout << "type == 1" << endl;
				}
				else if (type == 2) {
					Dijkstra(bj.sta_num, i, dist, prev, bj.link_c);
					//cout << "type == 2" << endl;
				}
				tag = 1;
				for (j = 0; j < bj.sta_num; j++) {
					if (!strcmp(bj.stas[j].name, staname2)) {
						exit2 = 1;
						if (distbest < 0 || dist[j] < distbest) {	
							distbest = dist[j];
							no2 = j;
							no1 = i;
							if (tag) {
								for (int m = 0; m < bj.sta_num; m++) {
									prevbest[m] = prev[m];
								}
								tag = 0;
							}
						}
					}
				}
				if (exit2 == 0) {
					cout << "没" << staname2 << "这个站" << endl;
					return 0;
				}
			}
		}
		/*if (bj.link[no1][no2] == 1) {
			cout << "linked" << endl;
		}
		for (i = 0; i < bj.sta_num; i++) {
			cout << dist[i] << '\t';
		}*/
		if (exit1 == 0) {
			cout << "没" << staname1 << "这个站" << endl;
			return 0;
		}
		//cout << endl << dist[no2] << endl;
		searchPath(prevbest, no1, no2);
	}
	return 0;
}