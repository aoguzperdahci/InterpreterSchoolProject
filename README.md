# The BigAdd Language

BigAdd language is a small programming language that has been designed in order to add and subtract big integers programmatically and display the result on the screen. It reads the source file with the extension “.ba” performs lexical analyze and executes the code.

**Contents**
1. [Basics](#basics)
	- [Case sensitivity](#case-sensitivity)
	- [Whitespace and end of line](#whitespace-and-end-of-line)
	- [Comments](#comments)
	- [Data types](#data-types)
	- [Errors](#errors)
2. [Code structures](#code-structures) 
	 - [Declaration statement](#declaration-statement)
	 - [Assignment statement](#assignment-statement)
	 - [Addition statement](#addition-statement)
	 - [Subtraction statement](#subtraction-statement) 
	 - [Output statement](#output-statement)
	 - [Loop statement](#loop-statement)

## Basics

### Case sensitivity
BigAdd is case sensitive. 

    int a.
    move 5 to a.
    out a.{5}
    out A.{throws an Error: Undefined variable}

### Whitespace and end of line
Point (‘.’) is the end of line character. Lines can contain keywords, square brackets, variables and constants. Any number of spaces or comments can be used between these elements. A program line can be divided between multiple text lines also a text line can contain multiple program line.

    int a. move 5 to a.
       out 
    a
    .
    
    {Treated as:
    int a.
    move 5 to a.
    out a.}

### Comments
Comments are written between curly braces { } .

     {this is a comment}
     {and this is a
     comment too}

### Data types
There are two data type in this language;
- Integers: A integer is a signed whole number that is represented as a decimal data type. An integer can be as big as 100 decimal digits (this value can be set by changing the value of MAX_INT_CONST_LEN in the constant declarations section).
- Strings: A string is any sequence of characters between two quotation marks. Strings exist only as constants and are used in the out statements only.

      out "this is a negative number: ", -1.

### Errors

Interpreter can detect these errors below;
- File errors
- Invalid variable names
- Unrecognized characters
- Undefind variables  
- Integer overflow errors
- Invalid syntax
- Missing end of lines
- Unclosed comment or code blocks
- Not terminated strings
- Invalid values for loop statements

## Code Structures

### Declaration statement
The only valid variable type is integer. Variables must be declared before they are used. All variables are initialized to a default value of 0. All variables are global and static. Variable names must be no more than 20 characters long (this value can be set by changing the value of MAX_VARIABLE_NAME_LEN in the constant declarations section). Variable names start with a letter (alphabetical character) and continue with alphanumeric characters or an underscore character. Keywords cannot be used as varible name. 

    int myVar.
    int my_var2.

### Assignment statement
A constant value or another variable's value can be assign to a variable.

    move 5 to a. {assigns 5 to a}
    move a to b. {assigns a's value to b}

### Addition statement
A constant value or another variable's value can be add to a variable's value.

    add 5 to a. {performs a = a + 5}
    add a to b. {performs b = b + a}

### Subtraction statement
A constant value or another variable's value can be subtracted from a variable's value.

    sub 5 from a. {performs a = a - 5}
    sub a from b. {performs b = b - a}

### Output statement
A out statement can contain any number of strings, constant values, variables, and newline keywords. Each of these elements must be separated by a comma.

    out "today's profit is ", todaysProfit, " $", newline, "this month's profit is ", monthlyProfit, " $".

### Loop statement
A loop statement can iterate through a single line of code or a code block. A code block is a list of code lines between square brackets. Loop conrtol value can be a constant value or a variable. Loop control value must be a positive value. Loop conrtol value decreases at each iteration ends and when it reaches 0 program exits the loop. If loop control value is a variable it can be used and changed inside the loop therefore it can be used to determine the number of iterations.

 

    int i.
    int j.
    int total.
    move 4 to i.
	loop i times
	[
	 move i to j.
	 loop j times out "* ".
	 out newline.
	 add i to total.
	]
	out "i: ", i, newline.
	out "total: ",total.
	 
	Output:
	* * * *
	* * *
	* *
	*
	i: 0
	total: 10


