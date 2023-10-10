# Taro
> Written with [StackEdit](https://stackedit.io/).
### Taro is a small, portable, dynamically typed scripting language. 
#### Note: This project is still currently under construction

A bytecode interpreter and virtual machine for my Lua/Python-inspired programming language that targets custom-made bytecode. Implements a Pratt-styled Precedence parser, explicit types,  control flow, and more, alongside bytecode IR generation from a source file. Written in C.

```js
let name = "taro";
let age = 400;
// variable assignment with 'let' keyword

let Games = enum{
  VALORANT,
	TEKKEN,
	SF6
};

let currentlyPlaying = Games::VALORANT;
// native support of enum types!

print "my name is " + name;
// print keyword. taro also features string concatenation

let drinks = ["tea", "coffee", "boba"];
print drinks;
// taro also features native list types and operations!

push(drinks, "lemonade");
let fav = drinks[3];

print "my name is " + name + " and i love " + fav;

if (fav is "lemonade") print "when life gives you lemons...";
else print "awkward..";

print fav == "lemonade";
// yes, we love lemonade :)
```

Taro is designed to utilize the ease of readability of scripting languages similar to JavaScript, Lua, and Python while maintaining other traditional syntax  considerations such as curly braces for scoping, and semicolons. 

# Variable Declaration
```js
let cat = "meow";
let dog = "woof";
// string declarations

let pets = 3;
// number declaration
```
# Types
### Numbers

Similarly to its predecessor, **taro** utilizes floating point numbers to represent digital values.
```js
let smol = 0.01;
let big = 1000;
// numbers are internally stored as floats, and support decimal numbers.

let bigMinus = -big;
// numeric negation is also supported
```

### Strings
```js
let name = "taro";
// string declaration

let name = name + " is my name!";
// string concatenation is supported

let a = "a";
let b = "b";

print a == b;
// string equality comparison is also included natively
```

### Lists
```js
let books = ["chainsaw man", "berserk", "one piece"];
let nums = [1, 2, 3];

let matrix = [
  [1, 2, 3],
  [4, 5, 6],
  [7, 8, 9]
];
// storage of lists within lists allows for matrix structures
print matrix[2][2];

let mix = [1, "1"];
// lists allow for multiple types to be stored in the same list
```

### Enums
```js
let Genres = enum {
  ROCK,
  INDIE,
  CLASSICAL	
};
// native enum structures allow for custom data types and pattern matching!

let mostPlayed = Genres::ROCK;
```

### Scope

```js
let a = 123;

{
  let a = 321;
  print a;
  // outputs 321. variable shadowing is allowed here!
}
print a;
```

### Control Flow

```js
let Games = enum {
  MELEE,
  MARIOKART,
  BG3
};

let name = "Shravan";
let bloodthirsty = true;
let sleepy = true;
// boolean values take on values of either [true, false, nil]

let favorite;
// you can also instantiate a variable without declaring a value

if (name is "Shravan" and bloodthirsty) {
// you can use python style `is` for equality, or good old `==`
  favorite = Games::MELEE;
}
else if (name is "Mark" or sleepy) {
  favorite = Games::MARIOKART;
}
else {
  favorite = Games::BG3;
}

```
### While Loops
```js
let count = 0;

let homies = [];
while (count < 10) {
  count = count + 1;
  push(homies, count);
  print count;
}
// the usual C-style while loops

count = 0;
while (count < 10) {
  print homies[count];
  count = count + 1;
}
print homies;
```
### For Loops 
```js
let nums = [];

for (let i = 0; i < 10; i = i + 1) {
  push(nums, i);
}
print nums;
```

### Nested For Loops and Matrix Iteration
```js
let a = [
  [951, 626, 909],
  [323, 301, 805],
  [505, 999, 911]
];

for (let i = 0; i < 3; i = i + 1) {
  for (let j = 0; j < 3; j = j + 1) {
    print a[j][i];
    // column wise iteration
  }
}
print a;
```
### Fibonacci Number :)
```js
let first = 0;
let second = 1;

let length = 7;
let count = 0;

let fibs = [];
while (count < length) {
  push(fibs, first);
  let temp = second;
  print first + second;
  second = first + second;
  first = temp;
  count = count + 1;
}

print fibs;
```

### Pretty Fizzbuzz and Single Line Control Flow
```js
let res = [];

for (let i = 1; i < 20; i = i + 1) {
  if (i % 3 == 0 and i % 5 == 0) push(res, "fizzbuzz");
  else if (i % 3 == 0) push(res, "fizz");
  else if (i % 5 == 0) push(res, "buzz");
  else push(res, i);
}
print res;
```
