#include <bits/stdc++.h>
#define SIZE 5 

using namespace std;

bool fix(string s, vector<int> &d, int i, int j);
bool fix0(string s, vector<int> &d, int i, int j);
bool fix1(string s, vector<int> &d, int i, int j);
char merge_c(char sk, char tk);
string merge(string s, string t);
string paint0(string s, vector<int> &d, int i, int j);
string paint1(string s, vector<int> &d, int i, int j);
string _paint(string s, vector<int> &d, int i, int j);
string paint(string s, vector<int> &d, int i, int j);
vector<vector<int>> propagate(vector<vector<int>> &d);

fstream fin;
ofstream fout;

int main() {

    fin.open("./test.txt"); 
    fout.open("./output.txt");

    string cs;
    vector<vector<int>> row(SIZE);
    vector<vector<int>> col(SIZE);            
    while(!fin.fail() && fin >> cs) {
        cs = cs.substr(1);
        fout << "Case: " << cs << "\n";

        int num, i = 0;
        char symbol;
        while (i < SIZE) {
            fin >> num;
            fin.get(symbol);
            row[i].push_back(num);
            if (symbol == '\n') i++;
        }
        i = 0;
        while(i < SIZE) {
            fin >> num;
            fin.get(symbol);
            col[i].push_back(num);
            if (symbol == '\n') i++;
        }

        vector<vector<int>> d(row);
        d.insert(d.end(), col.begin(), col.end());

        // vector<int> v = {3};
        // cout << fix0("u0u1uu", v, 6, 0);

        vector<vector<int>> G = propagate(d);
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (G[i][j] == 1) fout << "1\t";
                else if (G[i][j] == 0) fout << "0\t";
                else fout << "u\t";
            }
            fout << "\n";
        }
    }

    fin.close();
    fout.close();
    return 0;
}

bool fix0(string s, vector<int> &d, int i, int j) {
    if (s[i - 1] == '0' || s[i - 1] == 'u') return fix(s, d, i - 1, j);
    return false;
}

bool fix1(string s, vector<int> &d, int i, int j) {
    if (j >= 0 && i >= d[j] + 1) {
        string dj = "0";
        for (int i = 1; i <= d[j]; i++) dj = dj + "1";
        string tmp = s.substr(i - d[j] - 1, d[j] + 1);
        for (int i = 0; i < d[j] + 1; i++) {
            if (tmp[i] != dj[i] && tmp[i] != 'u') return false;
        }
        return fix(s, d, i - d[j] - 1, j - 1);
    }
    return false;
}

bool fix(string s, vector<int> &d, int i, int j) {
    if (i == 0 && j <= -1) return true;
    if (i == 0 && j >= 1) return false;
    return fix0(s, d, i, j) || fix1(s, d, i, j);
}

char merge_c(char sk, char tk) {
    if (sk == '0' && tk == '0') return '0';
    else if (sk == '1' && tk == '1') return '1';
    else return 'u';
}

string merge(string s, string t) {
    string m = "";
    for (int k = 0; k < s.size(); k++) {
        m += merge_c(s[k], t[k]);
    }
    return m;
}

string paint0(string s, vector<int> &d, int i, int j) {
    return paint(s, d, i - 1, j) + "0";
}

string paint1(string s, vector<int> &d, int i, int j) {
    string dj = "0";
    for (int i = 1; i <= d[j]; i++) dj = dj + "1";
    return paint(s, d, i - d[j] - 1, j - 1) + dj;
}

string _paint(string s, vector<int> &d, int i, int j) {
    if (fix0(s, d, i, j) && !fix1(s, d, i, j)) return paint0(s, d, i, j);
    if (!fix0(s, d, i, j) && fix1(s, d, i, j)) return paint1(s, d, i, j);
    return merge(paint0(s, d, i, j), paint1(s, d, i, j));
}

string paint(string s, vector<int> &d, int i, int j) {
    if (i == 0) return "";
    return _paint(s, d, i, j);
}

vector<vector<int>> propagate(vector<vector<int>> &d) {

    vector<vector<int>> G(SIZE, vector<int>(SIZE, -1));

    for (int i = 0; i < 2 * SIZE; i++) {
        string sr = "";
        for (int j = 0; j < SIZE; j++) {
            if (G[i][j] == 1) sr += '1';
            else if (G[i][j] == 0) sr += '0';
            else sr += 'u';
        } 

        if (!fix(sr, d[i], SIZE, d[i].size() - 1)) {
            fout << "CONFLICT!\n";
            return G;
        }

        sr = paint(sr, d[i], SIZE, d[i].size() - 1);
        if (i < SIZE) { 
            for (int j = 0; j < SIZE; j++) {
                if (sr[j] == '1') G[i][j] = 1;
                else if (sr[j] == '0') G[i][j] = 0;
                else G[i][j] = -1;
            }
        } else {
            for (int j = 0; j < SIZE; j++) {
                if (sr[j] == '1') G[j][i - SIZE] = 1;
                else if (sr[j] == '0') G[j][i - SIZE] = 0;
                else G[j][i - SIZE] = -1;
            }
        }
    }

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (G[i][j] == -1) {
                cout << "Incomplete\n";
                return G;
            }
        }
    }
    cout << "Solved\n";
    return G;
}