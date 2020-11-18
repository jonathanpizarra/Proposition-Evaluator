#include <iostream>
#include <vector>
#include <math.h>
#include "validation.cpp"

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


vector< vector<int> > table; // truth table
vector< vector<string> > vs; // variables in input prop
vector< vector<int> > groups; // braces group
vector< int > negations; // positions of negation operators
int group[2] = {-1,-1};
char caps = 'A'; // letters for simplifying prop expression
string sub;
int v_count = 0; // length of vs


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


void get_group(string p){
    // GET THE POSITIONS OF BRACKET GROUPS eg. : (p>q) gets 0 and 4
    groups.clear();
    string b = "(){}[]";
    vector< int > poss; // current character position
    vector< char > braces; //
    for(unsigned int x = 0; x < p.length(); x++){
        if(b.find(p[x]) != np){
            if(p[x] == '(' || p[x] == '{' || p[x] == '['){
                poss.push_back(x);
                braces.push_back(p[x]);
            }else{
                if( (p[x] == ')' && braces.back() == '(') || (p[x] == '}' && braces.back() == '{') || (p[x] == ']' && braces.back() == '[') ){
                    groups.push_back({poss.back(), signed(x)}); // gets the positions of the start and end of group
                    braces.pop_back();
                    poss.pop_back();
                }
            }
        }
    }

    if(groups.size() == 0){ // setting default
        group[0] = -1;
        group[1] = -1;
    }else{
        group[0] = groups[0][0];
        group[1] = groups[0][1];
    }

    for(unsigned int y = 0; y < groups.size(); y++){
           // getting the innermost group first
        if(groups[y][0] > group[0] && groups[y][1] < group[1]){
            group[0] = groups[y][0];
            group[1] = groups[y][1];

        }
    }

} // end of get_group

int get_pos(string p){
    // GET THE POSITION OF SHORTENER VARIABLE IN vs list
    for(unsigned int x = 0; x < vs.size(); x++){
        if(vs[x][0] == p){
            return x;
        }
    }
    return -1;
}

int get_duplicate(string p){
    // GET POSSIBLE DUPLICATE OF EXPRESSION IN vs list
    for(unsigned int x=0; x<vs.size(); x++){
        if(vs[x][1] == p){
            return x;
        }
    }
    return -1;
}

void get_negations(string p){
    bool duplicate;
    negations.clear();
    for(unsigned int x=0; x<p.length()-1; x++){
        if(p[x] == '~' && get_pos(p.substr(x+1,1)) != -1 ){
            duplicate = false;
            for(unsigned int y=0; y<negations.size(); y++){
                if( p.substr(negations[y],2) == p.substr(x,2)){
                    duplicate = true;
                    break;
                }
            }
            for(unsigned int y=0; y<vs.size(); y++){
                if(p.substr(x, 2) == vs[y][0]){
                    duplicate = true;
                    break;
                }
            }
            if(!duplicate){
                negations.push_back(x);
            }
        }
    }
}

int evaluator(string *p){
    string p1, p2, p3; // p1 is the first variable, p2 is the operator, p3 it the second variable
    int v1, v2; // positions of p1 and p3 in the truth table
    int ts; // current size of truth table
    int sublen; // current length of expression substring
    string extra = "a", extrasub = ""; // values to be pushed in reference array
    int duplicate; // if there are duplicate expresions

    for(unsigned int x=0; x<negations.size(); x++){ // evaluate negations first
        int neg = negations[x];

        if( unsigned(neg+1) < (*p).length() && vars.find((*p).substr(neg+1,1)[0] != np ) ){ // if there is a variable after negations operator
            p1 = "" + (*p).substr(neg+1, 1);
            v1 = get_pos(p1);
            ts = table.size();
            table.push_back(vector<int>(cols));
            vs.push_back({"~" + p1, "~" + p1});

            for(int y=0; y<cols; y++){
                table[ts][y] = int (!( bool (table[v1][y]) ));
            }
        }
    }

    if(groups.size() != 0){
        do{
            sub = (*p).substr(group[0]+1, group[1] - group[0]-1); // extract group
            do{
                sublen = 3;
                if(sub[0] == '~'){
                    p1 = "~" + sub.substr(1,1);
                    p2 = "" + sub.substr(2,1);
                    sublen++;
                    if(sub[3] == '~'){
                        p3 = "~" + sub.substr(4,1);
                        sublen++;
                    }else{
                        p3 = "" + sub.substr(3,1);
                    }
                }else{
                    p1 = "" + sub.substr(0,1);
                    p2 = "" + sub.substr(1,1);
                    if(sub[2] == '~'){
                        p3 = "~" + sub.substr(3,1);
                        sublen++;
                    }else{
                        p3 = "" + sub.substr(2,1);
                    }
                }

                extra = "a";
                extra[0] = caps;
                extrasub = p1 + p2 + p3;
                duplicate  = get_duplicate(extrasub);

                if(duplicate == -1){
                    v1 = get_pos(p1);
                    v2 = get_pos(p3);

                    ts = table.size();
                    table.push_back(vector<int>(cols));
                    vs.push_back({extra, extrasub});

                    // EVALUATION OF PROPOSITION BASED ON p2 (OPERATOR)
                    for(int y = 0; y < cols; y++){
                        if(p2 == "^"){ // CONJUNCTION
                            table[ts][y] = int (table[v1][y] && table[v2][y]);
                        }else if(p2 == "v"){ ////DISJUNCTION
                            table[ts][y] = (table[v1][y] || table[v2][y]);
                        }else if(p2 == "<"){ // BICONDITION
                            table[ts][y] = int (table[v1][y] == table[v2][y]);
                        }else if(p2 == "o"){ // EXCLUSIVE DISJUNCTION
                            table[ts][y] = int (table[v1][y] != table[v2][y]);
                        }else if(p2 == ">"){ // IMPLICATION
                            table[ts][y] = int ((table[v1][y] == 1 && table[v2][y] == 0)? 0:1);
                        }
                    } // end of y<cols loop

                    if(openings.find((*p)[group[0]]) != np){
                        (*p).erase(group[0],1); // remove the opening bracket of group. I think this should be outside of this loop
                    }

                    sub[0] = caps; // simplify evaluated expression into a letter.
                    (*p)[group[0]] = caps; // simplify evaluated expression into a letter.

                    sub.erase(1,sublen-1); // remove evaluated expression.
                    (*p).erase(group[0]+1, sublen-1); // remove evaluated expression.

                    caps++;
                }else{
                    if(openings.find((*p)[group[0]]) != np){
                        (*p).erase(group[0],1); // remove the opening bracket of group. I think this should be outside of this loop
                    }

                    sub[0] = vs[duplicate][0][0]; // simplify evaluated expression into a letter.
                    (*p)[group[0]] = vs[duplicate][0][0]; // simplify evaluated expression into a letter.
                    sub.erase(1,sublen-1); // remove evaluated expression.
                    (*p).erase(group[0]+1, sublen-1); // remove evaluated expression.
                }

            }while(sub.length() != 1);
            (*p).erase(group[0]+1,1); // remove closing bracket of group;
            // GET THE GROUPS AGAIN BECAUSE THE POSITIONS ALREADY CHANGED.
            get_negations(*p);
            get_group(*p);

            for(unsigned int x=0; x<negations.size(); x++){ // evaluate negations again if there are any
                int neg = negations[x];
                if( unsigned(neg+1) < (*p).length() && vars.find((*p).substr(neg+1,1)[0] != np )){ // if there is a variable after negations operator
                    p1 = "" + (*p).substr(neg+1, 1);
                    v1 = get_pos(p1);
                    ts = table.size();
                    table.push_back(vector<int>(cols));
                    vs.push_back({"~" + p1, "~" + p1});

                    for(int y=0; y<cols; y++){
                        table[ts][y] = int (!( bool (table[v1][y]) ));
                    }
                }
            }

        }while(group[0] >= 0 && group[1] > 0); // end of groups loop

    }else{
        // else is not needed
    }

    while((*p).length() > 1){
/////.... NOW LOOPING THROUGH THE SIMPLIFIED GROUPLESS PROPOSITION
        if((*p).length() == 2){ // if the remaining expression is in the form '~p';
            if(get_pos(*p) != -1){
                break; // negation already evaluated
            }else{
                get_negations(*p); // get negation one last time.
                for(unsigned int x=0; x<negations.size(); x++){ // evaluate negations first
                    int neg = negations[x];
                    p1 = "" + (*p).substr(neg+1, 1);
                    v1 = get_pos(p1);
                    ts = table.size();
                    table.push_back(vector<int>(cols));
                    vs.push_back({"~" + p1, "~" + p1});

                    for(int y=0; y<cols; y++){
                        table[ts][y] = int (!( bool (table[v1][y]) ));
                    }
                }
                break;
            }
        }
        sublen = 3;
        if((*p).substr(0,1) == "~"){
            p1 = (*p).substr(0,2);
            p2 = (*p).substr(2,1);
            sublen++;
            if((*p).substr(3,1) == "~"){
                p3 = (*p).substr(3,2);
                sublen++;
            }else{
                p3 = (*p).substr(3,1);
            }
        }else{
            p1 = (*p).substr(0,1);
            p2 = (*p).substr(1,1);
            if((*p).substr(2,1) == "~"){
                p3 = (*p).substr(2,2);
                sublen++;
            }else{
                p3 = (*p).substr(2,1);
            }
        }

         extra = "a";
        extra[0] = caps;
        extrasub = (*p).substr(0,sublen);
        duplicate = get_duplicate(extrasub);

        if(duplicate == -1){
            v1 = get_pos(p1);
            v2 = get_pos(p3);

            ts = table.size();
            table.push_back(vector<int>(cols));
            vs.push_back({extra, extrasub});
            // EVALUATION OF PROPOSITION BASED ON p2 (OPERATOR)
            for(int y = 0; y < cols; y++){
                if(p2 == "^"){ // CONJUNCTION
                    table[ts][y] = int (table[v1][y] && table[v2][y]);
                }else if(p2 == "v"){ ////DISJUNCTION
                    table[ts][y] = (table[v1][y] || table[v2][y]);
                }else if(p2 == "<"){ // BICONDITION
                    table[ts][y] = int (table[v1][y] == table[v2][y]);
                }else if(p2 == "o"){ // EXCLUSIVE DISJUNCTION
                    table[ts][y] = int (table[v1][y] != table[v2][y]);
                  //  cout << "\nxclusive disj:v1:" << table[v1][y] << "   v2:" << table[v2][y] << "\n";
                }else if(p2 == ">"){ // IMPLICATION
                    table[ts][y] = int ((table[v1][y] == 1 && table[v2][y] == 0)? 0:1);
                }
            } // end of cols loop
            (*p)[0] = caps;
            (*p).erase(1, sublen-1);
            caps++;
        }else{
            (*p)[0] = vs[duplicate][0][0];
            (*p).erase(1, sublen-1);
        }
////.....
    }
    return 0;
} // END OF EVALUATOR FUNCTION

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
