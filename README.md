![](https://github.com/fbgencer/fbgc/blob/master/fbgc_small_logo.png)

- Written in C for small memory environments as a calculator and interpreted programming language. However, ![fbgc] is getting powerful to become a programming language or a scientific calculation tool on computers.

##### History
- It started as a calculator using [Shunting Yard algorithm](https://en.wikipedia.org/wiki/Shunting-yard_algorithm) for ![fbgc] parser. Later it growed and it shows a capacity to become a programming language. Since then it has been improved and also it's under development.
- Inspired by Python, Matlab, and of course C. Yet it tries to differ somehow from these languages while keeping their good but changing features that is thought as useful.

#### Features
- Flexible and Dynamically typed
- Matrices are internal C arrays for fast and reliable computation.
- Easy to write and learn
- Easy to extend with a C/C++ libraries

Write your first program:

```ruby
	print('Hello World')
```

## Build

 To build ![fbgc], simply run:

    $ make

## Run

![fbgc] allows you to define multiple types of entries. Such as integers, doubles, strings, tuples and matrices. You don't need to specify types as in the case of statically typed languages.

Basic types:
```ruby
	:> This is a line comment
	x = 5 :> integer
	x = 3.14159 :> double
	x = 'Hey this is a string' :> string
	x = (1,2,1.23,('Hey i am tuple!')) :> tuple
	x = [1,2,3 ; 4,5,6] :> 2x3 matrix
```

Absolute operator:
number objects return positive of that number under abs operator
complex object returns 2-d distance
string, tuple and matrix objects return their length
```ruby

|'fbgc'| :> gives 4

|('a','fbgc',1,2,3)| :> gives 5

|3+4j| :> gives 5.0

|-3| :> gives 3

```

Function definition:
```ruby
sum = fun(a,b)
	return a+b
end
```


Conditional structures:
```ruby
if(a == 5 & y == 7)
	print('This is an if structure')
end

if(x == 5)
	print('Inside if : ',x)
elif(x == 6)
	print('Inside elif  :',x)
else
	print('Inside else: ',x)
end

i = 0
while(i<5)
	print(i)
end
```

In fbgc, for loop allow you to write fast code !
You can create a for loop just giving sequence or range 
```ruby
:>prints 0,1,2,3,4,5,6,7,8,9
for(i = 0:10)
	print(i)
end

:>prints 0,2,4,6,8
for(i = 0:2:10)
	print(i)
end

:>prints 'f','b','g','c'
for (i = 'fbgc')
	print(i)
end

:>|obj| operator gives you the length of the object
s = 'Hello world'
for (i = 0 : |x| )
	print(x[i])
end
```



## Future Work

  - ◦ Documentation
  - ✔ Lexer, parser and interpreter
  - ◦ Meaningful lexical, grammar and program errors
  - ✔ Internal memory handling mechanism
  - ✔ Integer computation
  - ◦ Internal libraries for all object types
  - ✔ C internal API
  - ◦ Garbage collection
  - ◦ Optimizations

![fbgc] is looking forward to be improved. If you are interested in please make pull request. It is under development.


[fbgc]: http://chart.apis.google.com/chart?cht=tx&chl=f_{b}^{g}c
