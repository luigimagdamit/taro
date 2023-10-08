
# Taro
> Written with [StackEdit](https://stackedit.io/).
### Taro is a small, dynamically typed scripting language. 
#### Note: This project is still currently under construction

A bytecode interpreter and virtual machine for my Lua & Python inspired programming language that targets custom-made bytecode. Implements a Pratt-styled Precedence parser, implicit type assignment, and bytecode IR generation from a source file. Written in C.

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
Taro is designed to utilize the ease of readability of scripting languages similar to JavaScript and Python while maintaining other traditional syntax  considerations such as curly braces for scoping, and semicolons. 

Planned for the future is a garbage collector to automatically manage heap-allocated objects such as strings, and arrays. Within the next update I will have control flow, which will make Taro turing complete. Further down, I will be implementing OOP functionality.


