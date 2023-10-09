# Taro
> Written with [StackEdit](https://stackedit.io/).
### Taro is a small, dynamically typed scripting language. 
#### Note: This project is still currently under construction

A bytecode interpreter and virtual machine for my Lua-inspired programming language that targets custom-made bytecode. Implements a Pratt-styled Precedence parser, implicit type assignment, and bytecode IR generation from a source file. Written in C.

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

print fav == "lemonade";
// yes, we love lemonade :)
```

Taro is designed to utilize the ease of readability of scripting languages similar to JavaScript, Lua, and Python while maintaining other traditional syntax  considerations such as curly braces for scoping, and semicolons. 

Planned for the future is a garbage collector to automatically manage heap-allocated objects such as strings, and arrays. Within the next update I will have control flow, which will make Taro turing complete. Further down, I will be implementing OOP functionality.

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

let favorite;

if (name == "Shravan" and bloodthirsty) {
  favorite = Games::MELEE;
}
else if (name == "Mark" or sleepy) {
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

### FizzBuzz
```js
let res = [];

for (let i = 0; i < 20; i = i + 1) {
  if (i % 3 == 0) {
    print "fizz";
    push(res, "fizz");
  }
  else if (i % 5 == 0) {
    print "buzz";
    push(res, "buzz");
  }
  else if (i % 3 == 0 and i % 5 == 0) {
    print "fizzbuzz";
    push(res, "fizzbuzz");
  }
  else {
    print i;
    push(res, i);
  }
}

print res;
```
