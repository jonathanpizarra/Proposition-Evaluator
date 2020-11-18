#include <iostream>
#define np string::npos

using namespace std;

// Nov 18, 2020 9:35pm PST
// separated this long method in a separate file 
// for easier access
// for easier adding of functionality
// will try to optimize the code as well

int operant, cols;
string ops = "v^o<->";
string openings = "{[(";
string closings = "]})";
string vars = "pqrst";

bool checkIfValidExp(string p){
    char c1, c2;
    int n = p.length(), x;
//closings...openings...vars...ops.. -moved to global scope

// I FEEL LIKE ALL THE LOGIC HERE CAN STILL BE SIMPLIFIED.
// TODO : ADD A METHOD THAT DISPLAYS WHERE EXACTLY IN THE STRING THE ERROR IS.

/* additional logic for negation operator
    ~ can't come after a variable.
    ~ can't go between brackets like   )~(    )~)    (~)     but can go  (~(
    ~ can't go consecutively like : ~~

*/

    for(x = 0; x < n-1; x++){
        c1 = p[x];
        c2 = p[x+1];

        if(x == 0 && (ops.find(c1) != np || closings.find(c1) != np) ){
            if(c1 != '~'){
                cout << "\nInvalid first char : " << c1 << "\n";
                return false;
            }
        }

        if(openings.find(c1) != np ){
            if(ops.find(c2) != np){
                cout << "\nOperator not allowed after opening bracket:" << x << "," << x+1 <<":\n";
                return false;
            }else if(closings.find(c2) != np){
                cout << "\nEmpty braces not allowed. " << c1 << c2;
                return false;
            }else if(c2 == '~'){
                continue;
            }
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
            if(c2 != '>'){
                cout << "\nInvalid char after (-) minus. (>) is missing.\n";
                return false;
            }else if( (c2 == '>') && (ops.find(p[x+2]) != np || closings.find(p[x+2]) != np ) ){
                cout << "\nInvalid char after -> operator: " << p[x+2] <<" \n";
                return false;
            }else{
                operant = 4;
                x++;
            }
        }else if(c1 == '<'){
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
        }else if(c1 == '~'){
            /* additional logic for negation operator
                ~ can't come after a variable.
                ~ can't go between brackets like   )~(    )~)    (~)     but can go  (~(
                ~ can't go consecutively like : ~~
            */
            if(c2 == '~'){
                cout << "\nInvalid consecutive negation operators";
                return false;
            }else if(closings.find(c2) != np){
                cout << "\nInvalid negation operator at the end of closing bracket";
                return false;
            }else if( x > 0 && openings.find(c2) != np && closings.find(p[x-1]) != np){
                cout << "\nNegation operator can't be between closing and opening brackets";
                return false;
            }
        }
    }//end of for loop
    return true;
}


