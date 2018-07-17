# x_language
design programming languege 


### Release
### data types
- boolean true|false   
- integer int32_t   
- float   double    
- string  string
### variable 
- var x : int 
- val x : int 
### function
- fn id () {}  
- fn id ( int x) {}  
### control flow 
#### if expression
- if () {} else {}
- if () {} elsif () {} else {}
##### loop expression
- while () {}
- for  () {}
- foreach () {} 



#### operator and operator priority
- () Grouping operator
- . member access form an struct
- Symbol inversion  
- * / %
- + -
- << >>
- < <= >= > Bitwise shift left  Bitwise shift right
- == != Comparison equal-to    Comparison not-equal-to
- &  Bitwise AND
- ^  Bitwise exclusive OR
- |  Bitwise inclusive (normal) OR
- && Logical AND
- || Logical OR
- += -= *= /= %= &= ^= |= <<= >>=
- =
####  symbols 
- => (pat => expr) part of match arm syntax  
- ; (...;)statement
- -> (fn() -> type, lamdba()->type) function and closure return type





### comments 
- // comments 
- /\*comments\*/

### collections
- vector


### key words 
- break - exit a loop immediately  
- continue - continue to the next loop iteration
- else -  fallback for if control flow constructs
- elsif - fallback for if contril flow constructs  
- false - boolean false literal  
- fn - function definition and function pointer type  
- for - iterator loop  
- if - conditional branching  
- lambda - lambda function definition
- match - pattern matching
- pub   - public data , method, function
- return - return from function 
- use - import sysbols into scope
- struct  - data struct
- true - boolean true literal
- val - denotes immutability in references, raw pointers, and pattern bindings 
- var - denotes mutability in references, raw pointers, and pattern bindings 
- while - iterator loop 
