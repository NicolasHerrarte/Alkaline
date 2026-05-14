# Alkaline

An interpreted programming language library written entirely in C without the use of frameworks.

## Motivation

This project started as a curiosity to demystify one of the most used abstractions. Over time it became much more than a toy project. Programming languages are the backbone of computer science and many modern systems, thus I could not call myself a true computer scientist without building my own from scratch.

## Methodology

The front-end of Alkaline consists of three parts: a lexer, a parser, and the interpreter. Each is its own separated module, assuming the previous is capable of executing correctly.

## Paradigm

Alkaline is a statically typed imperative language following the procedural paradigm. Programs are structured as sequences of statements with procedures as the main unit of abstraction. It features C family control flow with `if`/`else`, `while`, and `for` loops, along with standard arithmetic and logical expressions using a familiar operator precedence hierarchy. Variables must be explicitly declared with a type, and arrays require manual allocation. All values are passed by value, meaning everything, including arrays, is deep copied when passed to procedures or assigned. The language uses a few distinctive syntactic choices: `init` for variable declarations, `proc` with arrow return types for functions, and `=?` for equality, giving it a slightly unique flavor while remaining recognizable to anyone familiar with C like languages. Overall it's a minimal, straightforward procedural language well suited for learning about compilation and language design, while still being capable of holding its own ground for most basic programming tasks.

### Limitations

The language lacks support for object oriented programming, offering no structs, classes, or inheritance, which means there is no way to define custom composite data types or model hierarchical relationships. It also has no closures, lambdas, or first class functions, so procedures cannot be passed around as values or created on the fly. Generics and polymorphism are absent, limiting code reuse to manual duplication. All code must reside in a single program as there is no module or import system, and there is no built in error handling mechanism such as try/catch or exceptions. Despite these omissions, the language remains focused and intentional in its simplicity, prioritizing clarity over feature richness.

### Vision

The goal is to make a programming language that strongly enforces the basics and makes them intuitive. Every value will be passed by value unless stated otherwise, in which case the passed value becomes a *view* of the original data. This is not to be confused with pointers, which hold the actual piece of memory. If possible there will be a clear distinction between values, views, and pointers.

### Future

| Feature | Difficulty |
|---|---|
| Terminal version with arguments to interpret files | 1 |
| More binary operators like `!` | 2 |
| More operator types like `**` or `%` | 2 |
| Attributes like `array.length` | 3 |
| Generics | 3 (semi-implemented) |
| More utilities like sets or linked lists | 4 |
| Pointers | 4 (semi-implemented) |
| Meaningful error reporting in lexing, parsing, and evaluating | 5 — **PRIORITY** |
| Object oriented programming: structs, classes | 6 |
| Error recovery | 7 |
| Heap memory and garbage collector | 7 |
| First class functions and lambdas | 7 |
| Views of structures | 7 |
| Compiled version | 9 |

Any help is greatly appreciated.

## Showcase

### Fibonacci

```
proc fibonacci(n : int) -> int {
    if(n <= 1){
        return n;
    }
    return fibonacci(n - 1) + fibonacci(n - 2);
}

print("Fibonacci Calculator");
print("Type a number or exit");

init string cmd <- read();

while((cmd =? "exit") =? false){
    init int number_input <- toint(cmd);
    print(fibonacci(number_input));
    print("Type a number or exit");
    cmd = read();
}

print("Program Finished");
```

**Output:**
```
Fibonacci Calculator
Type a number or exit
8
21
10
55
exit
Program Finished
```

### Matrix Multiplication

```
proc matmul(a : int[][], b : int[][]) -> int[][] {
    init int[][] result <- assign int[3][3];

    for(init int i <- 0; i < 3; i = i + 1){
        for(init int j <- 0; j < 3; j = j + 1){
            init int sum <- 0;
            for(init int k <- 0; k < 3; k = k + 1){
                sum = sum + a[i][k] * b[k][j];
            }
            result[i][j] = sum;
        }
    }

    return result;
}

init int[][] a <- assign int[3][3];
init int[][] b <- assign int[3][3];

for(init int i <- 0; i < 3; i = i + 1){
    for(init int j <- 0; j < 3; j = j + 1){
        a[i][j] = i + j;
        b[i][j] = i * j;
    }
}

init int[][] c <- matmul(a, b);

for(init int i <- 0; i < 3; i = i + 1){
    for(init int j <- 0; j < 3; j = j + 1){
        print(tostr(c[i][j]));
    }
}
```

**Output:**
```
0
5
10
0
8
16
0
11
22
```

## Architecture

### Lexing

The process utilizes Thompson construction to break down a regex sequence into a usable nondeterministic finite automaton (NFA). The NFA is then converted into a deterministic finite automaton (DFA) by means of subset construction. The resulting tables are stored as a text file, which can then be loaded back without the necessity to recalculate. Any stream can then be tokenized using the tables and a double buffer maximal munch scanner to lex sequences into a (word, category) pair.

### Parsing

#### DSL

The grammar specification process uses the mentioned lexing library to break down the parsing instructions. These are fed into a hard-coded finite automaton to generate a *gramatika* data structure holding everything needed for LR(1) table generation and AST creation. After the process, the DSL data structure should contain the correct terminal symbols (T), nonterminal symbols (NT), grammar rules, and AST rules.

#### Parser

With the given DSL specifications, the required sets are generated: FIRST (containing possible lookahead symbols), canonical construction using one lookahead symbol, and LR(1) tables. These can be exported as well to prevent runtime calculations. The parsing loop, in partnership with the tables and DSL AST specifications, concludes with an independent arena-allocated AST.

### Evaluating

Using the AST, the evaluator runs a recursive `evaluate()` function which handles each node case and outputs a generic evaluator pass. It relies on the symbols table to open, close, and clean scopes containing fundamental variable information.

## Features

- **Fully independent architecture** — each component is self-reliant
- **Highly adaptable** — grammar can be written and modified without code intervention
- **Modular integration** — each module can be rewritten or refined without affecting unrelated components
- **Memory safety** — every component creates a copy of the most error-prone memory allocations like strings
- **Function wrapping** — given its high level of abstraction, C function wrappers are easily implemented into working features

## Usage

### Installation

```bash
git clone https://github.com/NicolasHerrarte/Alkaline.git
cd Alkaline
gcc -g -Iinclude src/lexer/*.c src/parser/*.c src/interpreter/*.c src/utils/*.c main.c -o build/interpreter.exe
./build/interpreter.exe
```

Or copy the C files and make sure to include the header files. Make sure to create the appropriate directories you specify.

### Running the Interpreter

Two main functions are required to execute the interpreter: one to build the AST, the other to run it.

#### Building the AST

```c
TreeManager ast = parse_pipeline(
    language_src,                  // source code
    language_regex,                // code lexing regex
    rules_src,                     // DSL source
    rules_regex,                   // DSL lexing regex
    ignore_categories_language,    // which code lexing categories to ignore
    ignore_categories_rules,       // which DSL lexing categories to ignore
    mapping,                       // pairs mapping of every symbol in DSL to each category
    symbols_amount,                // amount of symbols
    lexer_dir,                     // lexing directory
    parser_dir,                    // parsing directory
    generate_parsing_tables,       // create and load parsing tables (true) or only load (false)
    generate_lexing_tables,        // create and load lexing tables (true) or only load (false)
    debug                          // show debug messages in terminal
);
```

#### Evaluating the AST

```c
calculate_tree(
    ast,                   // the AST
    production,            // affects whether hash maps and scopes are destroyed,
                           // preventing visualization of symbol tables after execution
    number_definitions,    // number of definitions
    definitions...         // function pointers
);
```

### Code Example

```c
int main(){
    Pair mapping[] = {
        {"End",       0},
        {"Epsilon",   1},
        {"Goal",      2},
        {"Expr",      3},
        // ...
    };

    int ignore_spaces[] = {1};

    TreeManager ast = parse_pipeline(
        "language.k",
        "(=?)$20|(>=)$21|(<=)$22|(>)$23|(<)$24...",
        "grammar.k.specs",
        "(([a-zA-Z/(/)/*///-/[/]+=?><.;{},:/|&])...",
        ignore_spaces,
        ignore_spaces,
        mapping,
        84,
        "static/lexer",
        "static/parser",
        false,
        false,
        false
    );

    calculate_tree(ast, false, 5,
        print_define,
        to_int_define,
        to_float_define,
        to_string_define,
        input_define
    );
}
```

## DSL Reference

### Structure

```
Name /-> Reduction Rules /%%/ @command -$Arguments <ASTName> /;
```

Every rule follows this structure: first the name of the subject of the reduction followed by `/->`, then the predicate of the reduction. This can include other nonterminal symbols or terminal symbols. These names must be added in the mapping with their respective categories. The symbol `/%%/` terminates the grammar section and introduces the AST command. To avoid writing the same name repeatedly a convenience command can be used `/|` to indicate the use of the last recorded subject name.

### AST Commands

#### `@sh` — Shift

Shifts a node up in the tree, avoiding the creation of unnecessary nodes. Requires one argument: the index of the child to be shifted. Does not require a node tag.

```
Block /-> { CompStat } /%%/ @sh -$1 /;
```

To create a block (the inside of any scope), it is unnecessary to create a designated node. It is more convenient to simply shift the compounded statement to take the place of `Block`.

#### `@mn` — Make Node

Creates a node in the tree. This is the fundamental operation in building the tree. It uses children in the predicate to build a new tree node with a given tag for differentiation. Requires a node tag.

```
Eval /-> Eval + Term /%%/ @mn -$0 -$2 <Sum> /;
```

Creates a summation node using the left and right expressions.

#### `@ap` — Append

A special case for creating siblings instead of depth. Appends the children of the destination with the source. The first argument is the destination, the second is the source. Requires exactly two arguments and does not require a node tag.

```
CompStat /-> CompStat UnitStat /%%/ @ap -$0 -$1 /;
```

Appends a compounded statement with a singular statement instead of nesting nodes.

#### `@bx` — Box

A type of leaf node where label values are converted into actual tree members that can be processed. There are six types:

| Type | Index |  Examples |
|---|---|---|
| `integer` | `[0]` | `0`, `1`, `2` |
| `float` | `[1]` | `0.0`, `3.14` |
| `litstring` | `[2]` | `"hello"`, `"goodbye"` |
| `bool (true)` | `[3]` | `true` |
| `bool (false)` | `[4]` | `false` |
| `identifier` | `[5]` | `x`, `y`, `var_one` |

```
Factor /-> float /%%/ @bx [1] /;
```

If a boxing step is not applied, the AST will contain labels, which is forbidden. Labels are only scaffolding and should not be present in the final AST.

#### `@vl` — Value

A boxing of type integer which can have a numeric category.

```
RelOp /-> =? /%%/ @vl [1] /;
```

Creates an integer box with value 1.

### Mapping

```c
Pair mapping[] = {
    {"End",     0},
    {"Epsilon", 1},
    {"Goal",    2},
    {"Expr",    3},
    {"Eval",    4},
    {"Term",    5},
    {"Factor",  6},
    {"+",       7},
    {"-",       8},
    {"*",       9},
    {"/",      10},
    {"(",      11},
    {")",      12},
    // ...
};
```

Every mapping should match either a terminal or nonterminal symbol in the DSL. The amount of symbols needs to match those in the DSL file. If any symbol is repeated, not used, has its category out of bounds, or there is a hole in the categories (e.g. there is no category 3 when the category amount is 10), any of the previous are bound to cause a segmentation fault or undefined behavior due to the nature of the containers holding them.

### Regex

The regex used is still primitive, only supporting the main operations and some quality of life preprocessing.

**Supported operations** (given `a` and `b`):

| Operation | Syntax | Description |
|---|---|---|
| Concatenation | `ab` | Matches `a` followed by `b` |
| Alternation | `a\|b` | Matches `a` or `b` |
| Closure | `a*` | Matches zero or more `a` |
| Parenthesis | `a(a\|b)` | Matches `a` followed by `a` or `b` |
| Category | `a$01` | `a` will be represented as integer 1 |
| Literal | `a/\|` | `a` followed by literal `\|` |

**Preprocessing** introduces `[]`. Everything inside brackets becomes automatically an alternation: `[abcd]` becomes `(a|b|c|d)`. You can use the `a-b` operator to include a range from the initial symbol `a` to the final symbol `b`. This only works for numbers and letters:

- All numbers: `[0-9]`
- All lowercase: `[a-z]`
- All uppercase: `[A-Z]`
- All letters: `[a-zA-Z]`
- Terminators: `[.,;:]`

Literal characters are also supported inside brackets.

To build a suitable lexer, you will most likely need to combine many categories and regex. Alternation `|` with parentheses `()` and category specification `$` can be used to achieve this:

```
(abc)$01|(efg)$02|([0-9])$03
```

For the sample stream `abcefgefg6`, this would produce the following tokens:

```
(abc, 1)(efg, 2)(efg, 2)(6, 3)
```

## Function Embedding

Function embedding is divided into two steps: definition and execution.

### Definition

Call the following function inside a wrapper to create a definition:

```c
external_define(manager, function_name, type, execute_function, number_of_arguments, arguments...);
```

Every argument needs to be set to `PARAMETER_MODE` and include a name and a type.

```c
void print_define(SymbolsManager* manager){
    Argument argument;
    argument.mode = PARAMETER_MODE;
    argument.name = "generic_print";
    argument.argtype = GENERIC_MACRO_TYPE;

    external_define(manager, "print", VOID_MACRO_TYPE, print_execute, 1, argument);
}
```

### Execution

The function signature needs to be `void* f_execute(void* manager_name, ...)`. A set of macros is provided to simplify the process:

| Macro | Description |
|---|---|
| `init_symbols(manager_name)` | Must be included; initializes everything needed |
| `end_symbols(manager_name)` | Signals end of function |
| `var_fetch(var_name)` | Fetches an argument variable |
| `decl_return(return_name)` | Must be included; the name needs to match |
| `init_return(return_name)` | Signals the return will not be void |
| `set_return(return_name, variable)` | Sets a return to a variable |
| `var_is_prim` / `var_is_arr` / `var_is_ptr` | Checks the type of a variable |
| `prim_type` / `arr_type` | Gets the primitive type of a primitive or array |
| `prim_int` / `prim_float` / `prim_bool` / `prim_str` | Unpacks value to the respective primitive type |
| `make_int` / `make_float` / `make_bool` / `make_str` | Creates a variable of the specified type |

```c
void* print_execute(void* manager_void, ...){
    init_symbols(manager_void);
    var_fetch(generic_print);

    decl_return(int_pass);

    if(var_is_prim(generic_print)){
        print_primitive(generic_print, false);
    }
    else{
        assert(false);
    }

    end_symbols(int_pass);
}
```

## Symbols Table

The evaluator uses a 2D hash-scoped symbols table implementation, which allows for effortless adjustment of storage. A lookup includes the identifier and the attribute key (`"value"`, `"key"`, `"parameters"`, etc.). Scopes can be managed using a stack with initialize, reset, and finalize scope operations. The actual hashing storage and every singular table (except for the dictionaries) are stored in the arena parameter of the symbols manager, not to be confused with `global_arena`. The amount of attributes can be adjusted at initialization.

Every storage unit is a `VValue` structure capable of containing many primitive types (this can be adjusted as well). These are stacked in an array and can be retrieved using `getIdentifierStorage`. The function `getAttributeIdentifier` goes a step further and automatically fetches the attribute. Attributes can also be fetched given a storage of `VValue` pointer using `getAttributeStorage`. Using the setter functions, these values can all be redefined at runtime. If anything is desired to outlast the current scope, it can be stored in the global arena.

# Utilities

All secondary modules used in this project were developed for the same purpose, but outside use cases are possible.

## Allocator

Linked arena allocator that expands the list of linked arenas once storage is full. If the size of a memory slice requested is greater than the specified size, a new arena will be linked with exactly the storage needed for allocation, preventing any undefined behavior. The downside being it can hide a code flaw at runtime.

## Hash

Implements a linked bucket list for the specified storage, preventing any collisions from causing data loss. When deleting, a hole tracker is maintained to prevent misalignment of node storage identifiers and actual object positioning. It has many uses: it can either be used with or without keys. In the first scenario, any structure can be hashed simulating set behavior if a suitable hash function and hash comparison are provided (these need to meet certain requirements which can be seen in the dedicated Hash repository). In the second scenario, the key is the one being hashed and the object stored. A `dynadict` macro utility is provided with predefined string hashing functions, operating similarly to a Python dictionary.

## AST

Dedicated library implementing a left-child right-sibling (LCRS) representation, allowing for easy sibling manipulation. It uses an arena for efficient node allocation and deallocation. It includes every box and value case in the DSL and can be modified at convenience for more boxing types.

## Subset

Implements a bit array to simulate sets with a known universe size. Includes set operations like union, add, remove, membership checks, and more.

## Dynarray

This project is the backbone of Alkaline. Thanks to [eignnx](https://github.com/eignnx/dynarray) for such a simple and understandable implementation of Python-like arrays in C.











