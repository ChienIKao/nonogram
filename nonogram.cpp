#include <bits/stdc++.h>
#define SIZE 25

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
string propagate(vector<vector<int>> &G, vector<vector<int>> &d);

ifstream fin;
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
            col[i].push_back(num);
            if (symbol == '\n') i++;
        }
        i = 0;
        while(i < SIZE) {
            fin >> num;
            fin.get(symbol);
            row[i].push_back(num);
            if (symbol == '\n') i++;
        }

        vector<vector<int>> d;
        d.insert(d.end(), col.begin(), col.end());
        d.insert(d.end(), row.begin(), row.end());
        vector<vector<int>> G(SIZE, vector<int>(SIZE, -1));

        // vector<int> test = {3};
        // cout << fix("0uuu", test, 4, 0) << endl;
        // cout << paint("0uuu", test, 4, 0) << endl;

        string status = propagate(G, d);

        fout << "status: " << status << "\n";
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
        for (int k = 1; k <= d[j]; k++) dj = dj + "1";
        string tmp = s.substr(i - d[j] - 1, d[j] + 1);
        for (int k = 0; k < d[j] + 1; k++) {
            if (tmp[k] != dj[k] && tmp[k] != 'u') return false;
        }
        return fix(s, d, i - d[j] - 1, j - 1);
    }
    return false;
}

bool fix(string s, vector<int> &d, int i, int j) {
    if (i == 0 && j == -1) return true;
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

bool findList(queue<vector<int>> &ListG, vector<int> target) {
    bool find = false;
    queue<vector<int>> q(ListG);
    while (!q.empty()) {
        vector<int> l = q.front();
        q.pop();

        for (int i = 0; i < l.size(); i++) {
            if (l[i] == target[i]) find = true;
            else {
                find = false;
                continue;
            }
        }
    }

    return find;
}

string propagate(vector<vector<int>> &G, vector<vector<int>> &d) {
    // 1.
    // PI(G) <-- NULL
    // PI(G) is a set of updated pixels
    set<short> PIG;

    // 2.
    // Put all rows and columns of G into List(G)
    queue<vector<int>> ListG;
    for (int i = 0; i < SIZE; i++) {
        vector<int> list = {i}; // column index
        for (int j = 0; j < SIZE; j++) {
            list.push_back(G[j][i]);
        }
        ListG.push(list);
    } 

    for (int i = 0; i < SIZE; i++) {
        vector<int> list = {i + SIZE}; // row index
        for (int j = 0; j < SIZE; j++) {
            list.push_back(G[i][j]);
        }
        ListG.push(list);
    }

    // 3.
    int i = 1;
    while (!ListG.empty()) {
        // cout << "step " << i++ << endl;

        // 4.
        // Retrieve one line L from ListG
        vector<int> L = ListG.front();
        ListG.pop();

        // 5.
        string sr = "0";
        for (int j = 1; j <= SIZE; j++) {
            if (L[j] == 1) sr += '1';
            else if (L[j] == 0) sr += '0';
            else sr += 'u';
        }
        
        // cout << "sr " << sr << endl;
        // 6.
        int idx = L[0];
        if (!fix(sr, d[idx], SIZE + 1, d[idx].size() - 1)) return "CONFLICT";

        // 7.
        string sr2 = paint(sr, d[idx], SIZE + 1, d[idx].size() - 1);
        // cout << "sr2: " << sr2 << endl;
        // 8. 
        set<short> pi;  
        for (int j = 1; j <= SIZE; j++) {

            if (sr2[j] != 'u' && sr[j] == 'u') {

                vector<int> tmp;
                if (idx >= SIZE) { // col
                    pi.insert(100 * (idx - SIZE) + (j - 1));

                    if (sr2[j] == '1') G[idx - SIZE][j - 1] = 1;
                    else G[idx - SIZE][j - 1] = 0;

                    // 9.
                    tmp.push_back(j - 1);
                    for (int k = 0; k < SIZE; k++) tmp.push_back(G[k][j - 1]);
                } else { // row 
                    pi.insert(100 * (j - 1) + idx);
                    
                    if (sr2[j] == '1') G[j - 1][idx] = 1;
                    else G[j - 1][idx] = 0;

                    // 9.
                    tmp.push_back(j - 1 + SIZE);
                    for (int k = 0; k < SIZE; k++) tmp.push_back(G[j - 1][k]);
                }

                if (!findList(ListG, tmp)) ListG.push(tmp);
            }
        }

        // for (int m = 0; m < SIZE; m++) {
        //     for (int n = 0; n < SIZE; n++) {
        //         cout << G[m][n] << " ";
        //     }
        //     cout << endl;
        // }

        // 10.
        // collect all painted cells in this propagate
        PIG.insert(pi.begin(), pi.end());

        // cout << "PIG: ";
        // for (auto s : PIG) {
        //     cout << s << " ";
        // }
        // cout << endl << endl;
    }

    // for (int i = 0; i < 2 * SIZE; i++) {
    //     string sr = "0";
    //     for (int j = 0; j < SIZE; j++) {
    //         if (G[i][j] == 1) sr += '1';
    //         else if (G[i][j] == 0) sr += '0';
    //         else sr += 'u';
    //     } 

    //     if (!fix(sr, d[i], SIZE, d[i].size())) return "CONFLICT";

    //     sr = paint(sr, d[i], SIZE, d[i].size());
    //     if (i < SIZE) { 
    //         for (int j = 0; j < SIZE; j++) {
    //             if (sr[j] == '1') G[i][j] = 1;
    //             else if (sr[j] == '0') G[i][j] = 0;
    //             else G[i][j] = -1;
    //         }
    //     } else {
    //         for (int j = 0; j < SIZE; j++) {
    //             if (sr[j] == '1') G[j][i - SIZE] = 1;
    //             else if (sr[j] == '0') G[j][i - SIZE] = 0;
    //             else G[j][i - SIZE] = -1;
    //         }
    //     }
    // }

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (G[i][j] == -1) return "INCOMPLETE";
        }
    }
    return "SOLVED";
}
