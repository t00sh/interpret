interpret
=========

Basic interpreter for EVAL language. (totally invented :))

### USAGE
interpret < file

### EVAL language

The language is executed line per line.

##### Loop
while(E)
	E
end

##### Test
if(E)
	E
end

##### Variable declaration + affectation
V = E

##### Function call
foo(E)

##### Comments
Starting by #
eg:
a = a+b # A comment

##### Grammar
E = expression
F = function
R = float
o = operator
V = variable

E --> if(E)
E --> while(E)
E --> end
E --> (E)
E --> F(E)
E --> V=E
E --> R
E --> V
E --> EoE

o --> [+ - / ^ * % > < <= >= == !=]{1}
V --> [A-Za-z]+
F --> [A-Za-z]+
R --> [-+]{0,1}[0-9.]+

##### Operators
* a = b -> true if a is egual b
* a > b -> true if a greater than b
* a < b -> true if a lower than b
* a ~ b -> true if a not egual to b
* a + b -> a add b
* a - b -> a sub b
* a * b -> a mul b
* a % b -> a mod b
* a / b -> a div b
* a ^ b -> a power b

##### Operators priority
* =, >, >, <, ~          ===> 1
* +,-                    ===> 2
* *,/,%                  ===> 3
* ^                      ===> 4

You can use the () for change the priority.

##### Predefined functions
print(E) ===> print the value of the expression
abs(E)   ===> calc the absolute value of the expression
sqrt(E)  ===> function sqrt
cos(E)   ===> function cos

### EXAMPLES
You can read test.txt, it's a basic program written in EVAL.
It print all prime numbers between 1 and 1000.


### TODO
- The ELSE structure : if(E) E  else  E  end
- Functions definitions : def:f(x) E end
- Multi-parameters functions
- Better errors management
- Multi-chars operators
- Multi-types (dynamic) : double, int, string, list
- Very big numbers.
