# Proposition-Evaluator
A command line program about propositional logic. Given a proposition, it evaluates the given and displays the truth table.

The code uses functional programming since I'm not really familiar with C++ OOP. Still messy and on testing stage that's why there are unnecessary codes. 


## Sample output

```
=================================
  PROPOSITIONAL LOGIC EVALUATOR  
              v3.0               
        Jonathan Pizarra         
=================================

Valid variables : p q r s t    
Valid brackets : (){}[]
Operators: 
 v  : Disjunction
 ^  : Conjunction
 -> : Implication
 <->: Bicondition
 o  : Exclusive Disjunction
 ~  : Negation

Enter proposition : ~p > q v (~q ^ r)
=========================================================
 Result : CONTINGENCY
 Truth Table: 
┌───┬───┬───┬────┬────┬─────────┬─────────┬────────┐
│ p │ q │ r │ ~p │ ~q │ A: ~q^r │ B: ~p>q │ C: BvA │ 
├───┼───┼───┼────┼────┼─────────┼─────────┼────────┤
│ 1 │ 1 │ 1 │ 0  │ 0  │    0    │    1    │   1    │
│ 1 │ 1 │ 0 │ 0  │ 0  │    0    │    1    │   1    │
│ 1 │ 0 │ 1 │ 0  │ 1  │    1    │    1    │   1    │
│ 1 │ 0 │ 0 │ 0  │ 1  │    0    │    1    │   1    │
│ 0 │ 1 │ 1 │ 1  │ 0  │    0    │    1    │   1    │
│ 0 │ 1 │ 0 │ 1  │ 0  │    0    │    1    │   1    │
│ 0 │ 0 │ 1 │ 1  │ 1  │    1    │    0    │   1    │
│ 0 │ 0 │ 0 │ 1  │ 1  │    0    │    0    │   0    │
└───┴───┴───┴────┴────┴─────────┴─────────┴────────┘
```
