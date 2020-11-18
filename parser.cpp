#include <iostream>
#include <vector>
#include "validation.cpp"

using namespace std;


vector< vector<int> > table; // truth table
vector< vector<string> > vs; // variables in input prop
vector< vector<int> > groups; // braces group
vector< int > negations; // positions of negation operators

int group[2] = {-1,-1};
char caps = 'A'; // letters for simplifying prop expression
string sub;
int v_count = 0; // length of vs


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


int get_duplicate(string p){
    // GET POSSIBLE DUPLICATE OF EXPRESSION IN vs list
    for(unsigned int x=0; x<vs.size(); x++){
        if(vs[x][1] == p){
            return x;
        }
    }
    return -1;
}

int get_pos(string p){
    // GET THE POSITION OF SHORTENER VARIABLE IN vs list
    for(unsigned int x = 0; x < vs.size(); x++){
        if(vs[x][0] == p){
            return x;
        }
    }
    return -1;
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
