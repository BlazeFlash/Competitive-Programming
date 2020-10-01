#include <bits/stdc++.h>
#define col 5
#define _s 30

using namespace std;

int init[20], fin[20], a = 0, b = 0;
string init_dfa[_s], fin_dfa[_s];
int _a = 0, _b = 0;

void initialise(int nfa_table[][col]) {
    for (int i = 0;i < 1000;i++) {
        for (int j = 0;j < col;j++) {
            nfa_table[i][j] = -1;
        }
    }
}

void print_initial_final() {
    cout << "initial state/s is/are :- ";
    for (int i = 0;i < a;i++)
        cout << init[i] << " ";
    cout << endl;
    cout << "final state/s is/are :- ";
    for (int i = 0;i < b;i++)
        cout << fin[i] << " ";
    cout << endl;
}

void print_initial_final_dfa() {
    cout << "initial state/s is/are :- ";
    for (int i = 0;i < _a;i++)
        cout << init_dfa[i] << " ";
    cout << endl;
    cout << "final state/s is/are :- ";
    for (int i = 0;i < _b;i++)
        cout << fin_dfa[i] << " ";
    cout << endl << endl;
    for (int i = 0;i < 60;i++)
        cout << "-";
    cout << endl << endl;
}

void reduce_fin(int x) {
    for (int i = x; i < b - 1; i++)
        fin[i] = fin[i + 1];
    b -= 1;
}

/*
 preprocessor function simplifies the expression, as we have no symbol for concatenation
 operation. preprocessor function provide one.
*/

string preprocessor(string s) {
    string pre;
    int l = s.length();
    pre = "(";
    for (int i = 0;i < l;i++) {
        pre += s[i];
        if (s[i] >= 97 && s[i] <= 122 && s[i + 1] >= 97 && s[i + 1] <= 122) {
            pre += '.';
        }
        else if (s[i] == ')' && s[i + 1] == '(') {
            pre += '.';
        }
        else if (s[i] >= 97 && s[i] <= 122 && s[i + 1] == '(') {
            pre += '.';
        }
        else if (s[i] == ')' && s[i + 1] >= 97 && s[i + 1] <= 122) {
            pre += '.';
        }
        else if (s[i] == '*' && (s[i + 1] == '(' || (s[i + 1] >= 97 && s[i + 1] < 122))) {
            pre += '.';
        }
    }
    pre += ')';
    return pre;
}

/*
    converts given regular expression into it's corresponding
    postfix notation
*/
int priority(char c) {
    switch (c) {
    case '*': return 3;
    case '.': return 2;
    case '+': return 1;
    }
    return 0;
}
string postfix(string re)
{
    string post = "";
    stack<char> op;
    char c;
    for (long i = 0; i < re.size(); i++)
    {
        switch (re[i])
        {
        case 'a':
        case 'b':
            post += re[i]; break;
        case '(':
            op.push(re[i]); break;
        case ')':
            while (op.top() != '(') {
                post += op.top();
                op.pop();
            }
            op.pop();
            break;
        default:
            while (!op.empty()) {
                c = op.top();
                if (priority(c) >= priority(re[i])) {
                    post += op.top();
                    op.pop();
                }
                else break;
            }
            op.push(re[i]);
        }
        //cout<<re[i]<<' '<<postfix<<endl;
    }
    while (!op.empty())
    {
        post += op.top();
        op.pop();
    }
    return post;
}

/*
    this function convert's given postfix of reg expression
    into it's corresponding nfa
*/

int reg_nfa(string s, int nfa_table[][col]) {
    int l = s.length();
    int states = 1;
    int m, n, j, counter;
    for (int i = 0;i < l;i++) {
        char x = s[i];
        switch (x) {
        case 'a': nfa_table[states][0] = states;
            init[a] = states;
            a += 1;
            states += 1;
            nfa_table[states - 1][1] = states;
            fin[b] = states;
            b += 1;
            nfa_table[states][0] = states;
            states += 1;
            break;
        case 'b': nfa_table[states][0] = states;
            init[a] = states;
            a += 1;
            states += 1;
            nfa_table[states - 1][2] = states;
            fin[b] = states;
            b += 1;
            nfa_table[states][0] = states;
            states += 1;
            break;
        case '.': m = fin[b - 2];
            n = init[a - 1];
            nfa_table[m][3] = n;
            reduce_fin(b - 2);  //reduces final state
            a -= 1;           //reduces initial states
            break;
        case '+': for (j = a - 1, counter = 0;counter < 2;counter++) {
            m = init[j - counter];
            nfa_table[states][3 + counter] = m;
        }
                a = a - 2;
                init[a] = states;
                a += 1;
                nfa_table[states][0] = states;
                states += 1;
                for (j = b - 1, counter = 0;counter < 2;counter++) {
                    m = fin[j - counter];
                    nfa_table[m][3] = states;
                }
                b = b - 2;
                fin[b] = states;
                b += 1;
                nfa_table[states][0] = states;
                states += 1;
                break;
        case '*': m = init[a - 1];
            nfa_table[states][3] = m;
            nfa_table[states][0] = states;
            init[a - 1] = states;

            states += 1;
            n = fin[b - 1];
            nfa_table[n][3] = m;
            nfa_table[n][4] = states;
            nfa_table[states - 1][4] = states;
            fin[b - 1] = states;
            nfa_table[states][0] = states;
            states += 1;
            break;
        }
    }
    return states;
}

void print_nfa_table(int nfa_table[][col], int states) {
    cout << endl;
    for (int i = 0;i < 60;i++)
        cout << "*";
    cout << endl << endl;
    cout << setw(43) << "TRANSITION TABLE FOR NFA" << endl << endl;
    cout << setw(10) << "States" << setw(10) << "a" << setw(10) << "b" << setw(10) << "e" << setw(10) << "e" << endl;
    for (int i = 0;i < 60;i++)
        cout << "-";
    cout << endl;
    for (int i = 1;i < states;i++) {
        for (int j = 0;j < col;j++) {
            if (nfa_table[i][j] == -1)
                cout << setw(10) << "--";
            else
                cout << setw(10) << nfa_table[i][j];
        }
        cout << endl;
    }

    cout << endl;
    for (int i = 0;i < 60;i++)
        cout << "*";
    cout << endl;
    print_initial_final();
}

void print_dfa_table(string dfa_tab[][3], int state) {
    cout << endl << endl;
    for (int i = 0;i < 60;i++)
        cout << "*";
    cout << endl << endl;
    cout << setw(43) << "TRANSITION TABLE FOR DFA" << endl << endl;
    cout << setw(10) << "States" << setw(10) << "a" << setw(10) << "b" << endl;
    for (int i = 0;i < 60;i++)
        cout << "-";
    cout << endl;
    for (int i = 0;i < state;i++) {
        for (int j = 0;j < 3;j++) {
            cout << setw(10) << dfa_tab[i][j];
        }
        cout << endl;
    }
    cout << endl;
    for (int i = 0;i < 60;i++)
        cout << "*";
    cout << endl;
    print_initial_final_dfa();
}

void print_mdfa_table(string dfa_tab[][3], int state) {
    cout << endl << endl;
    for (int i = 0;i < 60;i++)
        cout << "*";
    cout << endl << endl;
    cout << setw(43) << "TRANSITION TABLE FOR MINIMISED-DFA" << endl << endl;
    cout << setw(10) << "States" << setw(10) << "a" << setw(10) << "b" << endl;
    for (int i = 0;i < 60;i++)
        cout << "-";
    cout << endl;
    for (int i = 0;i < state;i++) {
        for (int j = 0;j < 3;j++) {
            cout << setw(10) << dfa_tab[i][j];
        }
        cout << endl;
    }
    cout << endl;
    for (int i = 0;i < 60;i++)
        cout << "*";
    cout << endl;
    print_initial_final_dfa();
}

vector <int> eclosure(int nfa_table[][col], int x) {
    stack <int> s;
    map <int, int> m;
    vector <int> v;
    int y;

    s.push(x);
    m[x] = 1;

    while (!s.empty()) {
        y = s.top();
        s.pop();
        if (nfa_table[y][3] == -1)
            continue;
        else {
            s.push(nfa_table[y][3]);
            m[nfa_table[y][3]] = 1;

            if (nfa_table[y][4] == -1)
                continue;
            else {
                s.push(nfa_table[y][4]);
                m[nfa_table[y][4]] == -1;
            }
        }
    }

    map <int, int> ::iterator itr;
    itr = m.begin();

    while (itr != m.end()) {
        v.push_back(itr->first);
        itr++;
    }
    return v;
}

long long int map_it(vector <int> v, int y) {
    long long int x = 0, m = 1;
    while (y--) {
        m *= 10;
    }

    vector<int> ::iterator it = v.begin();
    while (it != v.end()) {
        x += *it * m;
        m /= 10;
        it += 1;
    }
    return x / 10;
}

/*
    The function state_name() is giving a new name to all new dfa state.
*/
string state_name(int i) {
    char s = 'q';
    string p;
    p += s;
    if (i == 0) {
        p += '0';
        return p;
    }
    int a[100];
    int j = 0;
    while (i > 0) {
        int x = i % 10;
        a[j] = x;
        j += 1;
        i = i / 10;
    }
    for (int i = j - 1;i >= 0;i--) {
        int x = a[i];
        switch (x) {
        case 0: p += '0';
            break;
        case 1: p += '1';
            break;
        case 2: p += '2';
            break;
        case 3: p += '3';
            break;
        case 4: p += '4';
            break;
        case 5: p += '5';
            break;
        case 6: p += '6';
            break;
        case 7: p += '7';
            break;
        case 8: p += '8';
            break;
        case 9: p += '9';
            break;

        }
    }
    return p;
}

void check_init(vector <int> v, string s) {
    for (int i = 0;i < v.size();i++) {
        if (v[i] == init[0]) {
            init_dfa[_a] = s;
            _a += 1;
        }
    }
}

void check_fin(vector <int> v, string s) {
    for (int i = 0;i < v.size();i++) {
        if (v[i] == fin[0]) {
            fin_dfa[_b] = s;
            _b += 1;
        }
    }
}

bool check_valid(string word) {
    int len = word.length();
    int i = 0;
    for (i = 0;i < len;i++) {
        if (word[i] == 'a' || word[i] == 'b')
            continue;
        else
            return false;

    }

    if (i == len)
        return true;
    return false;
}

int nfa_dfa(int nfa_table[][col], int states, string dfa_tab[][3]) {
    bool flag[states];
    memset(flag, true, sizeof(flag)); //to make sure all states E-closure found

    int state = 0, j = 0;
    map <vector<int>, string> map_state;
    vector <int> v, v1, v2, v3, v4;

    v = eclosure(nfa_table, init[0]);
    flag[init[a]] = false;

    map_state[v] = state_name(j++);
    check_init(v, map_state[v]);  //to check whether current state is initial or not
    check_fin(v, map_state[v]);

    /* cout<<endl<<map_state[v]<<"represents :- ";
                 for(int i=0;i<v.size();i++)
                     cout<<v[i]<<" ";
                 cout<<endl; */

    stack < vector<int> > st;
    st.push(v);
    // transition of e-closure to over input symbol a
    int counter = 0;
    while (true) {

        while (!st.empty()) {
            vector <int> v;
            v = st.top();
            st.pop();
            counter += 1;
            dfa_tab[state][0] = map_state[v];       //find transition of a state over input symbol 'a' and 'b'

            for (int i = 0;i < v.size();i++) {
                flag[v[i]] = false;
                int temp = nfa_table[v[i]][1];      //over input symbol a
                int temp1 = nfa_table[v[i]][2];     //over input symbol b
                if (temp >= 0)
                    v1.push_back(temp);
                if (temp1 >= 0)
                    v3.push_back(temp1);
            }

            map <int, int> map_temp, map_temp1;  //to remove duplicate state
            map <int, int> ::iterator it;

            for (int i = 0;i < v1.size();i++) {
                v2 = eclosure(nfa_table, v1[i]);
                for (int j = 0;j < v2.size();j++) {
                    map_temp[v2[j]] = 1;
                }
                v2.clear();
            }

            for (int i = 0;i < v3.size();i++) {
                v4 = eclosure(nfa_table, v3[i]);
                for (int j = 0;j < v4.size();j++) {
                    map_temp1[v4[j]] = 1;
                }
                v4.clear();
            }


            for (it = map_temp.begin(); it != map_temp.end(); it++) {
                v2.push_back(it->first);
                flag[it->first] = false;
            }

            for (it = map_temp1.begin(); it != map_temp1.end(); it++) {
                v4.push_back(it->first);
                flag[it->first] = false;
            }

            if (v2.empty()) {
                dfa_tab[state][1] = "--";
            }
            else {
                string t = map_state[v2];
                char flag1 = t[0];
                if (flag1 == 'q') {
                    dfa_tab[state][1] = map_state[v2];   //checking v2 has already been mapped or not
                }
                else {
                    dfa_tab[state][1] = state_name(j++);
                    map_state[v2] = dfa_tab[state][1];
                    check_init(v2, map_state[v2]);
                    check_fin(v2, map_state[v2]);
                    /*-     cout<<endl<<map_state[v2]<<" represents :- ";
                         for(int i=0;i<v2.size();i++)
                             cout<<v2[i]<<" ";
                         cout<<endl; */
                    st.push(v2);                //not mapped state will be pushed into stack
                }
            }

            if (v4.empty()) {
                dfa_tab[state][2] = "--";
            }
            else {
                string t = map_state[v4];
                char flag1 = t[0];
                if (flag1 == 'q') {
                    dfa_tab[state][2] = map_state[v4];
                }
                else {
                    dfa_tab[state][2] = state_name(j++);
                    map_state[v4] = dfa_tab[state][2];
                    check_init(v4, map_state[v4]);
                    check_fin(v4, map_state[v4]);
                    /* cout<<endl<<map_state[v4]<<" represents :- ";
                     for(int i=0;i<v4.size();i++)
                         cout<<v4[i]<<" ";
                     cout<<endl; */
                    st.push(v4);
                }
            }
            v1.clear();
            v2.clear();
            v3.clear();
            v4.clear();
            state += 1;
        }

        int k = 1;
        for (k = 1;k < states;k++) {
            if (flag[k]) {
                v = eclosure(nfa_table, k);
                map_state[v] = state_name(j++);
                check_init(v, map_state[v]);
                check_fin(v, map_state[v]);
                cout << endl << map_state[v] << " represents :- ";
                for (int i = 0;i < v.size();i++)
                    cout << v[i] << " ";
                cout << endl;
                st.push(v);
                break;
            }
        }

        if (k == states)
            break;

    }

    print_dfa_table(dfa_tab, state);     //function to print dfa table

    return state;


}
bool check_same(string dfa[][3], set<string> p[], int state, int nstate, string x, string y)
{
    int x1, y1;
    for (int i = 0;i < state;i++)
    {
        if (dfa[i][0] == x)
            x1 = i;
        else if (dfa[i][0] == y)
            y1 = i;
    }
    int c = 0;
    for (int i = 0;i < nstate;i++)
    {
        if (p[i].count(dfa[x1][1]) == 1 && p[i].count(dfa[y1][1]) == 1)
            c++;
        if (p[i].count(dfa[x1][2]) == 1 && p[i].count(dfa[y1][2]) == 1)
            c++;
    }
    if (c == 2)
        return true;
    return false;
}
int min_dfa(string dfa_tab[][3], int state, string mdfa[][3])
{
    set<string> p[state + 1];
    int nstate = 2, temp = 2;

    for (int i = 0;i < _b;i++)
        p[1].insert(fin_dfa[i]);
    for (int i = 0;i < state;i++)
    {
        if (p[1].find(dfa_tab[i][0]) == p[1].end())
            p[0].insert(dfa_tab[i][0]);
    }
    int i;
    bool change;
    while (true)
    {
        nstate = temp;
        for (i = 0;i <= nstate;i++)
        {
            if (p[i].size() <= 1)
                continue;

            change = false;

            string x = *(p[i].begin());
            for (auto y = p[i].begin();y != p[i].end();y++)
            {
                if (x == *y)
                    continue;
                if (!check_same(dfa_tab, p, state, temp, x, *y))
                {
                    //cout<<*y<<endl;
                    change = true;
                    p[temp].insert(*y);
                    //p[i].erase(y);
                }
            }

            if (change == true)
            {
                for (auto y = p[temp].begin();y != p[temp].end();y++)
                {
                    if (p[i].count(*y) == 1)
                        p[i].erase(*y);
                }
                temp++;
            }

        }
        /*cout<<temp<<"\n";
        for(int i=0;i<temp;i++)
    {
        for(auto y=p[i].begin();y!=p[i].end();y++)
        cout<<*y<<" ";
        cout<<"\n";
    }*/
        if (temp == nstate)
            break;

    }
    bool visit[nstate];
    for (int i = 0;i < nstate;i++)
    {
        visit[i] = false;
        if (p[i].count(init_dfa[0]) == 1)
            visit[i] = true;
    }
    set<string> d;
    for (int i = 0;i < nstate;i++)
    {
        if (p[i].count(init_dfa[0]) == 1)
        {
            mdfa[0][0] = *(p[i].begin());
            d.insert(*(p[i].begin()));
            init_dfa[0] = *(p[i].begin());
            break;
        }
        if (p[i].count(fin_dfa[0]) == 1)
        {
            fin_dfa[0] = *(p[i].begin());
        }
    }
    for (int i = 0;i < nstate;i++)
    {
        string x, y;
        for (int j = 0;j < state;j++)
        {
            if (dfa_tab[j][0] == mdfa[i][0])
            {
                x = dfa_tab[j][1];
                y = dfa_tab[j][2];
                break;
            }
        }
        for (int j = 0;j < nstate;j++)
        {
            if (p[j].count(x) == 1)
            {
                if (visit[j] == true)
                {
                    mdfa[i][1] = *(p[j].begin());
                }
                else {
                    mdfa[i][1] = *(p[j].begin());
                    mdfa[i + 1][0] = *(p[j].begin());
                    visit[j] = true;
                }
            }
            if (p[j].count(y) == 1)
            {
                if (visit[j] == true)
                {
                    mdfa[i][2] = *(p[j].begin());
                }
                else {
                    mdfa[i][2] = *(p[j].begin());
                    mdfa[i + 1][0] = *(p[j].begin());
                    visit[j] = true;
                }
            }
        }

    }
    return nstate;
}
void simulator(string dfa_tab[][3], string word, int state, string pos) {
    int idx1 = 0, idx2, idx3 = 0, c = 0;
    for (int i = pos.size() - 1;i >= 0;i--)
    {
        if (pos[i] == '*')
            idx1 = i;
        if (pos[i] == 'a')
            idx3 = i;
        if (pos[i] == 'b')
        {
            c++;
            idx2 = i;
            if (idx2 < idx1 || idx2 < idx3)
            {
                cout << "Not Accepted";
                return;
            }
            else
            {

                while (i - 2 >= 0)
                {
                    //cout<<pos[i-2];
                    if (pos[i - 2] == 'b' && c == 1)
                        c++;
                    else if (pos[i - 2] == 'a' && c == 2)
                    {
                        cout << "Accept";
                        return;
                    }
                    else {
                        cout << "Not Accepted";
                        return;
                    }
                    i -= 2;
                }
            }
        }
    }
    cout << "Not Accepted";
    return;
    for (int i = 0;i < _b;i++)
        if (init_dfa[0] == fin_dfa[i])
        {
            cout << "Not Accepted";
            return;
        }
    int len = word.length();
    string temp = init_dfa[0];
    bool check = check_valid(word);     //makes sure that word need to be simulated only consists of 'a' and 'b'

    if (!check) {
        temp = "--";
    }

    int i = 0;
    for (i = 0;i < len;i++) {
        if (temp == "--") {
            cout << endl << "Not Accepted" << endl << endl;
            break;
        }
        else {
            int _xtate;
            int j = 0;

            for (j = 0;j < state;j++) {       //to find the index over which transition need to be performed.
                if (temp == dfa_tab[j][0])
                    break;
            }

            if (word[i] == 'a') {
                temp = dfa_tab[j][1];
            }
            else if (word[i] == 'b') {
                temp = dfa_tab[j][2];
            }
        }
    }

    if (i == len) {
        int j = 0;
        for (j = 0;j < _b;j++) {
            if (temp == fin_dfa[j]) {
                cout << endl << "Accept" << endl << endl;
                break;
            }
        }
        if (j == _b) {
            cout << endl << "Not Accepted" << endl << endl;
        }
    }/*else{
        cout<<endl<<"String does not belong to given regular expression."<<endl<<endl;
    }*/

}


int main() {
    /*  In nfa_table :-
        0th column represents states
        1st column represents states over input 'a'
        2nd column represents states over input 'b'
        3rd column represents states over input 'e'(epsilon)
        -1 in states columns represent no state changes over that input.
     */
    int nfa_table[1000][col];
    initialise(nfa_table);
    int states = 0;
    cout << "Enter a regular expression :-  " << endl << "e.g. (a+b)*abb" << endl;
    string s, pos;
    cin >> s;

    s = preprocessor(s);
    cout << "after preprocessed " << s;
    cout << endl;

    pos = postfix(s);
    cout << "postfix " << pos << endl;

    states = reg_nfa(pos, nfa_table);
    print_nfa_table(nfa_table, states);

    string dfa_tab[states][3];
    int dfa_state = 0;
    dfa_state = nfa_dfa(nfa_table, states, dfa_tab);

    string mdfa[dfa_state][3];
    dfa_state = min_dfa(dfa_tab, dfa_state, mdfa);
    print_mdfa_table(mdfa, dfa_state);

    string word = "abb";
    cout << "check whether regular expression ends with abb or not." << endl;

    simulator(mdfa, word, dfa_state, s);

    return 0;
}