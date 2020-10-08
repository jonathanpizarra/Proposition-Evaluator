#include <iostream>
#include <vector>
#include <math.h>
#define np string::npos

using namespace std;

// Proposition Evaluator
// Created by Jonathan Pizarra
// Date started : March 24, 2019 Sunday 4:55PM.
// Date finished : June 1, 2019 Saturday 9:24PM.(Code logic complete and working).
//                 June 1, 2019 Saturday 9:29PM - Bug found after testing long proposition expression.
//       v.2.2.0 - June 2, 2019 Sunday 11:31AM - Finally fixed the bug. But realized that I wasn't able to add the negation (~) operator in the algorithm... :((
//               : October 8, 2020 Thursday 5:25 pm PST - After more than a year, I finally have time to revisit and complete this code.
//                 I will upload this to github first before making changes.

int operant, cols;
vector< vector<int> > table; // truth table
vector<string> vs; // variables in input prop
vector< vector<int> > groups; // braces group
int group[2] = {-1,-1};
char caps = 'A'; // letters for simplifying prop expression
string sub;
int v_count = 0;

    string ops = "v^o<->";
    string openings = "{[(";
    string closings = "]})";
    string vars = "pqrst";

bool checkIfValidChars(string p){
    string vals = "pqrsto^v(){}[]<->";

    for(unsigned int x = 0; x < p.length(); x++){
        if(p[x] == ' '){
            continue;
        }
        if(vals.find(p[x]) == string::npos){
            return false;
        }
    }
    return true;

}

bool checkIfBalance(string p){
    vector< char > stacc;
    string pars = "(){}[]";

    for(unsigned int x = 0; x < p.length(); x++){
        char q = p[x];
        if(pars.find(q) != string::npos){
            if(q == '(' || q == '{' || q == '['){
                stacc.push_back(q);
            }else{
                if(q == ')' && stacc.back() == '('){
                    stacc.pop_back();
                }else if(q == '}' && stacc.back() == '{'){
                    stacc.pop_back();
                }else if(q == ']' && stacc.back() == '['){
                    stacc.pop_back();
                }else{
                    return false;
                }
            }
        //    cout << "\nxxxstack : " << stacc.size() << endl;
        }
    } // end of FOR LOOP

   // cout << "\nstacc : " << (stacc.size()) << endl;
    if(stacc.size() == 0){
        return true;
    }else{
        return false;
    }
}

bool checkIfValidExp(string p){
    cout << "\nChecking your expression...\n";
    char c1, c2;
    int n = p.length(), x;
//closings...openings...vars...ops.. -moved to global scope

    for(x = 0; x < n-1; x++){
        c1 = p[x];
        c2 = p[x+1];
        if(x == 0 && (ops.find(c1) != np || closings.find(c1) != np) ){
            cout << "\nInvalid first char\n";
            return false;
        }

        if(openings.find(c1) != np && (closings.find(c2) != np || ops.find(c2) != np) ){
            cout << "\nInvalid char after opening brace/bracket/parenthesis\n";
            if(closings.find(c2) != np){
                cout << "Empty braces:" << c1 << c2;
            }
            return false;
        }
        if(closings.find(c1) != np && (vars.find(c2) != np || openings.find(c2) != np) ){
            cout << "\nInvalid char after closing brace/bracket/parenthesis\n";
            return false;
        }
        if(vars.find(c1) != np ){
            if(c2 != 'v' && c2 != '^' && c2 != 'o'){
                if(c2 == '-' && (x+2 < n && p[x+2] != '>') ){
                    cout << "\nInvalid char after minus\n";
                    return false;
                }else if(c2 == '-' && (x+2 < n && p[x+2] == '>') ){
                    operant = 4;
                    //x++;
                    continue;
                }
                if(c2 == '<' && ( ( x+2 < n && p[x+2] != '-') || (x+3 < n && p[x+3] != '>') ) ){
                    cout << "\nInvalid chars after < symbol, \n";
                    return false;
                }else if( c2 == '<' && ( ( x+2 < n && p[x+2] != '-') || (x+3 < n && p[x+3] != '>') ) ){
                    operant = 5;
                    //x+=2;
                    continue;
                }
            }
            if(vars.find(c2) != np){
                cout << "\nInvalid:variable after a variable. missing operator between\n";
                return false;
            }else if(openings.find(c2) != np){
                cout << "\nInvalid : missing operator between variable and opening brace.\n";
                return false;
            }
        } // end of if - vars.find(c1)

        if(c1 == 'v' || c1 == '^' || c1 == 'o'){
            operant = c1 == 'v' ? 1:c1 == '^'? 2 : 3;

            if(closings.find(c2) != np || ops.find(c2) != np){
                cout << "\nInvalid char after v/^/o operator\n";
                return false;
            }
        }else if(c1 == '-'){
          //  cout << "\nEncountered a minus char.\n";
            if(c2 != '>'){
                cout << "\nInvalid char after (-) minus. (>) is missing.\n";
                return false;
            }else if( (c2 == '>') && (ops.find(p[x+2]) != np || closings.find(p[x+2]) != np ) ){
                cout << "\nInvalid char after -> operator:" << p[x+2] <<": \n";
                return false;
            }else{
                //cout << "\nValid char after -> operator\n";
                operant = 4;
                x++;
            }
        }else if(c1 == '<'){
           // cout << "\nEncountered (<) less than.\n";
            if(c2 != '-' || (x+2 < n && p[x+2] != '>') ){
                cout << "\nInvalid chars after (<) symbol.\n";
                return false;
            }else if( c2 == '-' || (x+2 < n && p[x+2] == '>') ){
                if( x+3 < n && (ops.find(p[x+3]) != np || closings.find(p[x+3]) != np ) ){
                    cout << "\nInvalid char after <-> operator.\n";
                    return false;
                }else{
                    operant = 5;
                    x+=2;
                }
            }
        }

    }//end of for loop


    return true;
}

void get_group(string p){
    //cout << "\nstart of get_group";
    groups.clear();
    string b = "(){}[]";
    vector< int > poss;
    vector< char > braces;
    for(unsigned int x = 0; x < p.length(); x++){
        if(b.find(p[x]) != np){
            if(p[x] == '(' || p[x] == '{' || p[x] == '['){
                poss.push_back(x);
                braces.push_back(p[x]);
            }else{
                if( (p[x] == ')' && braces.back() == '(') || (p[x] == '}' && braces.back() == '{') || (p[x] == ']' && braces.back() == '[') ){
                    groups.push_back({poss.back(), signed(x)});
                    braces.pop_back();
                    poss.pop_back();
                }
            }
        }
    }

    if(groups.size() == 0){
        group[0] = -1;
        group[1] = -1;
    }else{
        group[0] = groups[0][0];
        group[1] = groups[0][1];
    }

    for(unsigned int y = 0; y < groups.size(); y++){
           // cout << "\ny = " << groups.size();
        if(groups[y][0] > group[0] && groups[y][1] < group[1]){
            group[0] = groups[y][0];
            group[1] = groups[y][1];

        }
    }
    //cout << "\n ggg="<< group[0];

//cout << "\ng[0] = " << group[0] << " .. g[1] = " << group[1];

} // end of get_group

int get_pos(char p){
    //cout << "\np : " << p;
    for(unsigned int x = 0; x < vs.size(); x++){
          //  cout << "\nlooping.." << vs[x][0];
        if(vs[x][0] == p){
            return x;
        }
    }
    return -1;
}

int evaluator(string *p){
    char p1, p2, p3;
    int v1, v2;
    int ts;
    string extra = "a";
if(groups.size() != 0){
    do{
        sub = (*p).substr(group[0]+1, group[1] - group[0]-1);
        //cout << "\nSub : " << sub << " .. g[0]=" << group[0] << " .. g[1]=" << group[1] << " .. gsize=" << groups.size();
        do{
            p1 = sub[0];
            p2 = sub[1];
            p3 = sub[2];
            extra = "a";
            extra[0] = caps;
            extra += sub.substr(0,3);
           // cout << "\nextra : " << extra;
            v1 = vars.find(p1);
            if(v1 == np){
                v1 = get_pos(p1);
            }
            v2 = vars.find(p3);
            if(v2 == np){
                v2 = get_pos(p3);
            }
            ts = table.size();
            table.push_back(vector<int>(cols));
            vs.push_back(extra);

            // EVALUATION OF PROPOSITION BASED ON p2 (OPERATOR)
            for(int y = 0; y < cols; y++){
                //    cout << "\ny:" << y;
                if(p2 == '^'){ // CONJUNCTION
                    table[ts][y] = int (table[v1][y] && table[v2][y]);
                }else if(p2 == 'v'){ ////DISJUNCTION
                    table[ts][y] = (table[v1][y] || table[v2][y]);
                }else if(p2 == '<'){ // BICONDITION
                    table[ts][y] = int (table[v1][y] == table[v2][y]);
                }else if(p2 == 'o'){ // EXCLUSIVE DISJUNCTION
                    table[ts][y] = int (table[v1][y] != table[v2][y]);
                  //  cout << "\nxclusive disj:v1:" << table[v1][y] << "   v2:" << table[v2][y] << "\n";
                }else if(p2 == '>'){ // IMPLICATION
                    table[ts][y] = int ((table[v1][y] == 1 && table[v2][y] == 0)? 0:1);
                }
            } // end of cols loop
            //cout << "\nmidSub : " << sub << " .. P : " << *p << " .. gsize=" << groups.size();

            sub[0] = caps;
            if(openings.find((*p)[group[0]]) != np){
                (*p).erase(group[0],1);
            }
            (*p)[group[0]] = caps;
            sub.erase(1,2);
            (*p).erase(group[0]+1, 2); //
            caps++;
//cout << "\n extra : " << extra << " .. P : " << *p << " .. sub = " << sub;
        }while(sub.length() != 1);
//cout << "\nbefore = " << *p ;
        (*p).erase(group[0]+1,1);
        //cout << "\n.....after = " << *p;
        get_group(*p);
//cout << "\nendSub : " << sub << " .. g[0]=" << group[0] << " .. g[1]=" << group[1] << " .. gsize=" << groups.size();

    }while(group[0] >= 0 && group[1] > 0); // end of groups loop


}else{
    //cout << "\nerror ....";
}

    while((*p).length() != 1){
/////....
            p1 = (*p)[0];
            p2 = (*p)[1];
            p3 = (*p)[2];
            extra = "a";
            extra[0] = caps;
            extra += (*p).substr(0,3);
            cout << "\nNew xtra = " << *p;
            v1 = vars.find(p1);
            if(v1 == np){
                v1 = v_count + (int(p1) - 65);
            }
            v2 = vars.find(p3);
            if(v2 == np){
                v2 = v_count + (int(p3) - 65);
            }
            ts = table.size();
            table.push_back(vector<int>(cols));
            vs.push_back(extra);
            // EVALUATION OF PROPOSITION BASED ON p2 (OPERATOR)
            for(int y = 0; y < cols; y++){
                if(p2 == '^'){ // CONJUNCTION
                    table[ts][y] = int (table[v1][y] && table[v2][y]);
                }else if(p2 == 'v'){ ////DISJUNCTION
                    table[ts][y] = (table[v1][y] || table[v2][y]);
                }else if(p2 == '<'){ // BICONDITION
                    table[ts][y] = int (table[v1][y] == table[v2][y]);
                }else if(p2 == 'o'){ // EXCLUSIVE DISJUNCTION
                    table[ts][y] = int (table[v1][y] != table[v2][y]);
                  //  cout << "\nxclusive disj:v1:" << table[v1][y] << "   v2:" << table[v2][y] << "\n";
                }else if(p2 == '>'){ // IMPLICATION
                    table[ts][y] = int ((table[v1][y] == 1 && table[v2][y] == 0)? 0:1);
                }
            } // end of cols loop
            (*p)[0] = caps;
            (*p).erase(1, 2);
            caps++;
////.....
    }
  //  cout << "\nppP : " << (*p);
    return 0;
} // END OF EVALUATOR FUNCTION

int main()
{
/*  v / V   = disjunction
    ^       = conjunction
    ->      = implication
    <->     = bicondition
    o / O   = exclusive disjunction
    p, q, r, s, t, P, Q, R, S, T = variables
    //// p ^ q -> r ^ s == should be evaluated from left to right

*/
   // string prop = "((p v r) ^ (q -> p -> r)) -> (s <-> t o ((q v r) o (r -> (p <-> t))) o s <-> t)";
    // string prop = "{[ ( p -> q ) ^ ( r -> s ) ] ^ ( p v r ) } -> (q v s )";
     string prop = "[ ( p -> q ) ^ ( p -> r) ] -> (q -> r)";

    int n = prop.length();
    int x, y; //// loop/counter variable

////// TO LOWER CASE
    for(x = 0; x < n; x++){
        prop[x] = tolower(prop[x]);
    }
///// REMOVE SPACES
    for(y = 0; y < n; y++){
        if(prop[y] == ' '){
            prop.erase(y,1);
            y--;
        }
    }
    cout << "\nno spaces : " << prop << ":end\n";

////// CHECK FOR INVALID CHARS
    if(!checkIfValidChars(prop)){
        cout << "\nInvalid proposition. Invalid chars found\n";
        return 0;
    }else{
        cout << "\nValid characters\n";
    }

////// CHECK FOR BALANCE PARENTHESES
    if(!checkIfBalance(prop)){
        cout << "\nParentheses not balanced\n";
        return 0;
    }else{
        cout << "\nBalance Parentheses\n";
    }
/////// CHECK IF VALID EXPRESSION  ex : (p (v q) )vv r is invalid

    if(!checkIfValidExp(prop)){
        cout << "\nINvalid expression\n";
        return 0;
    }else{
        cout << "\nValid expression\n";
    }
/////// SIMPLIFY <-> AND -> INTO < AND > RESPECTIVELY
    for(x = 0; x < n; x++){
        if(prop[x] == '<'){
            prop.erase(x+1,2);
            n-=2;
        }else if(prop[x] == '-'){
            prop.erase(x,1);
            n--;
        }
    }
    cout << "\nSIMPLIFIED :" << prop << ":\n";

     //// get variables
    string vars = "pqrst";
//    vector<string> vs;
    char v;
    string extra = "a";
    for(unsigned int x = 0; x < vars.length(); x++){
        v = vars[x];//char
        for(int y = 0; y < n; y++){
            if(prop[y] == v){
                extra[0] = v;
                vs.push_back(extra);
                v_count++;
                break;
            }
        }
    }

    //cout << "\nvars : " << vs[0] << endl;

    int vl = vs.size();
    cols = pow( 2.0,double(vl) );
    for(x=0;x<vl;x++)table.push_back(vector<int>(cols));
    //cout << "\npow:" << (pow(2.0,double(vl))) << ":\n";
    ///// SETTING INITIAL TRUTH TABLE VALUES FOR THE VARIABLES
    bool b = false;
    int valcount = cols/2;
    for(x = 0; x < vl; x++){
        for(y = 0; y < cols; y++){
            if(y%valcount == 0){
                b = !b;
            }
            table[x][y] = int(b);
        }
        valcount /= 2;
    }
   // cout << "\nbefore get_group";
    get_group(prop);

    for(unsigned int x = 0; x < groups.size(); x++){
       // cout << "\n x[" << x << "][0]=" << groups[x][0] << "..x[" << x << "][1]=" << groups[x][1] << endl;
    }

    string prop2 = prop;
    evaluator(&prop2);

    ////// PRINT THE TABLE
    cout << "\n\ntable: \n\n";
    vl = table.size();
    for(x = 0; x < vl; x++){
        cout << vs[x] << " :\t";
        for(y = 0; y < cols; y++){
            cout << table[x][y] << "  " ;
        }
        cout << "\n";
    }

    return 0;
}
