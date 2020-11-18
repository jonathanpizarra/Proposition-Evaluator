#include <iostream>
#include <vector>
#include <math.h>
#include "parser.cpp"

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
//               : October 12, 2020 9:10 am. Missing logic. Should not evaluate similar expression that was already evaluated
//                 eg. : (p->q)^(p->q), the expression p->q should only be evaluated once.
//                 It should be replaced by A^A because they are just the same.
//        v3.0 - : October 12, 2020 7:12pm. Fixed the missing logic. Now fully functioning but expecting minor logical errors.
//               : October 12, 2020 11:54 pm. commit changes to github repo.
//                 Bug: Problem arises if the table exceeds 31 columns. Exceeds the 26 capital letters. 





void print_table(){
    // increase cmd screen width buffer size if the proposition is too long.
    cout << "\n\n Truth Table: \n > : Shorthand for ->\n < : Shorthand for <->\n\n";
    int ts = table.size();
    int x,y,z;
    int len[ts];
    int left,right;

    for(y=0; y<vs.size(); y++){
        len[y] = (vs[y][0] == vs[y][1]) ? vs[y][0].length()+1 : len[y] = vs[y][0].length() + vs[y][1].length() + 3;
    }

    cout << char(218);
    for(x=0; x<vs.size(); x++){
        for(y=0; y<len[x]+1; y++){
            cout << char(196); // horizontal bar symbol
        }
        cout << ((x == vs.size()-1) ? char(191) : char(194)); // cross symbol
    }

    cout << "\n" << char(179) << " ";
    for(y=0; y<vs.size(); y++){
        if(vs[y][0] == vs[y][1]){
            cout << vs[y][0] << " " << char(179) << " ";
        }else{
            cout << vs[y][0] << ": " << vs[y][1] << " " << char(179) << " ";
        }
    }
    cout << "\n" << char(195);

    for(x=0; x<vs.size(); x++){
        for(y=0; y<len[x]+1; y++){
            cout << char(196); // horizontal bar symbol
        }
        cout << ((x == vs.size()-1) ? char(180) : char(197)); // cross symbol
    }
    cout << "\n";
    for(x = 0; x < cols; x++){
        cout << char(179);
        for(y = 0; y < ts; y++){
            left = right = len[y]/2;
            if( (len[y]/2)*2 != len[y] ){
                right++;
            }
            for(z=0; z<left; z++) cout << " ";
            cout << table[y][x];
            for(z=0; z<right; z++) cout << " ";
            cout << char(179); // vertical bar symbol
        }
        cout << "\n";
    }
    cout << char(192);

    for(x=0; x<vs.size(); x++){
        for(y=0; y<len[x]+1; y++){
            cout << char(196); // horizontal bar symbol
        }
        cout << ((x == vs.size()-1) ? char(217) : char(193)); // cross symbol
    }

}

bool checkIfValidChars(string p){
    string vals = "pqrsto^v(){}[]<->~";

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
        }
    }
    if(stacc.size() == 0){
        return true;
    }else{
        return false;
    }
}


void introduction(){
    cout << "\n=================================";
    cout << "\n  PROPOSITIONAL LOGIC EVALUATOR  ";
    cout << "\n              v3.0               ";
    cout << "\n        Jonathan Pizarra         ";
    cout << "\n=================================";
    cout << "\n\nValid variables : p q r s t    ";
    cout << "\nValid brackets : (){}[]";
    cout << "\nOperators: ";
    cout << "\n v  : Disjunction";
    cout << "\n ^  : Conjunction";
    cout << "\n -> : Implication";
    cout << "\n <->: Bicondition";
    cout << "\n o  : Exclusive Disjunction";
    cout << "\n ~  : Negation";
    cout << "\n\nEnter proposition : ";
}

int main(){


/*  v / V   = disjunction
    ^       = conjunction
    ->      = implication
    <->     = bicondition
    o / O   = exclusive disjunction
    p, q, r, s, t, P, Q, R, S, T = variables
    //// p ^ q -> r ^ s == should be evaluated from left to right
    ~       = (tilde) negation operator

*/
    introduction();

     string prop = "((p v r) ^ (q -> p -> r)) -> (s <-> t o ((q v r) o (r -> (p <-> t))) o s <-> t)";
    // prop = "{[ ( p -> q ) ^ ( r -> s ) ] ^ ( p v r ) } -> (q v s )";
    // prop = "[ ( ~p -> q ) ^ ( p -> r) ] -> (q -> r)";
    // prop = "~p > q v (~q ^ r)";
    // prop = "~p > ~q";
     prop = "~q v s -> ~{[ ( p -> ~q ) ^ ( r -> s -> ~p v ~t) ] ^ ~( p v r ) } -> (~q v s ) o (p v r) ^ (~q v s) v [t ^ p ^ (~r -> ~p)]";
     prop = "~(p->q ^ (p->q))";
    int n = prop.length();
    int x, y; //// loop/counter variable

    cout << prop;
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

////// CHECK FOR INVALID CHARS
    if(!checkIfValidChars(prop)){
        cout << "\nInvalid proposition. Invalid chars found\n";
        return 0;
    }else{
        //cout << "\nValid characters\n";
    }

////// CHECK FOR BALANCE PARENTHESES
    if(!checkIfBalance(prop)){
        cout << "\nParentheses not balanced\n";
        return 0;
    }else{
        //cout << "\nBalance Parentheses\n";
    }

/////// CHECK IF VALID EXPRESSION  ex : (p (v q) )vv r is invalid
    if(!checkIfValidExp(prop)){
        cout << "\nINvalid expression\n";
        return 0;
    }else{
        //cout << "\nValid expression\n";
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
    //cout << "\nSIMPLIFIED :" << prop << ":\n";

//// GET THE VARIABLES USED
    string vars = "pqrst", var;
    char v;
    for(unsigned int x = 0; x < vars.length(); x++){
        v = vars[x]; // char
        for(int y = 0; y < n; y++){
            if(prop[y] == v){
                var = prop.substr(y,1);
                vs.push_back({var, var});
                v_count++;
                break;
            }
        }
    }

    int vl = vs.size();
    cols = pow( 2.0,double(vl) );
    // ADD VARIABLE COLUMNS IN TABLE
    for(x=0;x<vl;x++)table.push_back(vector<int>(cols));

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
    get_negations(prop);
    get_group(prop);

    string prop2 = prop;
    evaluator(&prop2);

    ////// PRINT THE TABLE
    print_table();

    return 0;
}
