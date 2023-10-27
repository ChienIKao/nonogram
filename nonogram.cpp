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
set<short> propagate(vector<vector<int>> &G, vector<vector<int>> &d, string &status);
string FP1(vector<vector<int>> &G, vector<vector<int>> &d);
string backtracking(vector<vector<int>> &G, vector<vector<int>> &d);

ifstream fin;
ofstream fout;

int main() {

    fin.open("./test.txt"); 
    fout.open("./output.txt");

    string cs;
    while(!fin.fail() && fin >> cs) {
        vector<vector<int>> row(SIZE);
        vector<vector<int>> col(SIZE);   

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

        // string status = backtracking(G, d);

        long start = clock(); 
        string status = FP1(G, d);
        long end = clock();

        fout << "status: " << status << "\n";
        fout << "time: " << (double)(end - start) / CLOCKS_PER_SEC << " sec\n";
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

set<short> propagate(vector<vector<int>> &G, vector<vector<int>> &d, string &status) {
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
    while (!ListG.empty()) {

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
        
        // 6.
        int idx = L[0];
        if (!fix(sr, d[idx], SIZE + 1, d[idx].size() - 1)) {
            status = "CONFLICT";
            return PIG;
        }

        // 7.
        string sr2 = paint(sr, d[idx], SIZE + 1, d[idx].size() - 1);

        // 8. 
        set<short> pi;  
        for (int j = 1; j <= SIZE; j++) {

            if (sr2[j] != 'u' && sr[j] == 'u') {

                vector<int> tmp;
                if (idx >= SIZE) { // row
                    int r = idx - SIZE, c = j - 1;

                    pi.insert(100 * (r) + (c));

                    if (sr2[j] == '1') G[r][c] = 1;
                    else G[r][c] = 0;

                    // 9.
                    tmp.push_back(c);
                    for (int k = 0; k < SIZE; k++) tmp.push_back(G[k][c]);
                } else { // col
                    int r = j - 1, c = idx;

                    pi.insert(100 * (r) + c);
                    
                    if (sr2[j] == '1') G[r][c] = 1;
                    else G[r][c] = 0;

                    // 9.
                    tmp.push_back(r + SIZE);
                    for (int k = 0; k < SIZE; k++) tmp.push_back(G[r][k]);
                }

                if (!findList(ListG, tmp)) ListG.push(tmp);
            }
        }

        // 10.
        // collect all painted cells in this propagate
        PIG.insert(pi.begin(), pi.end());
    }

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (G[i][j] == -1) {
                status = "INCOMPLETE";
                return PIG;
            }
        }
    }

    status = "SOLVED";
    return PIG;
}

void copy(vector<vector<int>> &a, vector<vector<int>> &b) {
    for (int i = 0; i < a.size(); i++) {
        for (int j = 0; j < a[i].size(); j++) {
            a[i][j] = b[i][j];
        }
    }
}

string probe(short p, vector<vector<int>> &G, vector<vector<int>> &d) {

    int r = p / 100, c = p % 100;

    // 1.
    string s0;
    vector<vector<int>> GP0(G);
    GP0[r][c] = 0;
    set<short> PI0 = propagate(GP0, d, s0);

    // 2.
    string s1;
    vector<vector<int>> GP1(G);
    GP1[r][c] = 1;
    set<short> PI1 = propagate(GP1, d, s1);

    // 3. 
    if (s0 == "CONFLICT" && s1 == "CONFLICT") return "CONFLICT"; // status G

    // 4.
    if (s0 == "CONFLICT") {
        if (!PI1.empty()) {
            copy(G, GP1); // 11.
            // return "PAINTED";
        } else return "INCOMPLETE"; // 12.
    } else if (s1 == "CONFLICT") {
        if (!PI0.empty()) {
            copy(G, GP0); // 11.
            // return "PAINTED";
        } else return "INCOMPLETE"; // 12.
    } else {
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (GP0[i][j] == GP1[i][j]) G[i][j] = GP0[i][j];
            }
        }
        // return "PAINTED";
    }

    // cout << "G: \n";
    // for (int i = 0; i < SIZE; i++) {
    //     for (int j = 0; j < SIZE; j++) cout << G[i][j] << " ";
    //     cout << "\n";
    // }
    // cout << "GP0: \n";
    // for (int i = 0; i < SIZE; i++) {
    //     for (int j = 0; j < SIZE; j++) cout << GP0[i][j] << " ";
    //     cout << "\n";
    // }
    // cout << "GP1: \n";
    // for (int i = 0; i < SIZE; i++) {
    //     for (int j = 0; j < SIZE; j++) cout << GP1[i][j] << " ";
    //     cout << "\n";
    // }
    return "PAINTED";
}

string FP1(vector<vector<int>> &G, vector<vector<int>> &d) {

    set<short> PI;
    string status = "";

    // 2. 
    // Repeat
    int i = 1;
    do {   
        // 3.
        // Propagate(G)
        PI = propagate(G, d, status);

        // 4.
        if (status == "CONFLICT" || status == "SOLVED") return status;

        // 6.
        // cout << "try: " << i++ << " times\n";
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (G[i][j] == -1) {
                    int p = 100 * i + j;
                    status = probe(p, G, d);
                    // cout << "p = " << p << " status = " << status << " G[i][j] = " << G[i][j] << "\n";
                    if (status == "CONFLICT" || status == "SOLVED") return status;
                    if (status == "PAINTED") break;
                }
            }
            if (status == "PAINTED") break;
        }

        // for (short pi : PI) {
        //     cout << pi << " ";
        // }
        // cout << "\n";
        
    } while (!PI.empty());

    return status;
}

string backtracking(vector<vector<int>> &G, vector<vector<int>> &d) {
    string status = FP1(G, d);

    if (status == "CONFLICT" || status == "SOLVED") return status;

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (G[i][j] == -1) {
                vector<vector<int>> GP0(G);
                vector<vector<int>> GP1(G);
                GP0[i][j] = 0;
                GP1[i][j] = 1;

                copy(G, GP0);
                backtracking(GP0, d);

                copy(G, GP1);
                backtracking(GP1, d);
            }
        }
    }
    return status;
}