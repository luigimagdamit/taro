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
if (true) {
	print "hello!!";
}
if (false) {
  print "you shouldn't see this";
} else {
  print "hello again!";
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
