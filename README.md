# development_new_language
a new language made for stress relief.




hello, I'm just an ordinary university student.  
this is a project to create a new language for stress relief.  
updates will be irregular.  

since I'm not good at english, I might make mistakes in variable names and other parts,  
but I’d really appreciate it if you take a look anyway.  
## Overview

This program demonstrates the basic workflow of the **ANS language compiler front-end**.  
It currently performs **lexical analysis** and **AST construction**, but the full hash processing and later stages are still under development.

---

## `main.c`

```c
/*
  1) Entry point for lexing; begins tokenization.
  2) Sequentially outputs the lexed tokens.
  3) Creates the list used by the hash process.
  4) Builds the AST via the parser.
  5) Runs the hash process function.
  6) Prints the AST.
  7) Prints the AST (deprecated; use is discouraged).
  8–10) Clean up each structure (tokens, AST, hash list), then return.

  NOTE: At this stage, only AST construction is implemented.
*/

int main(void) {
  Ans_Lex_Token_List* token_list = start_ans_lex_main("ans_file_path"); // 1
  ans_token_list_call(token_list);                                      // 2
  Ans_Hash_Process_List* hash_list = gen_hash_process_list("std.ans");  // 3
  Ans_Ast_Nodes* ast_node = start_parser_ans_ast_main(token_list, hash_list); // 4
  start_hash_process_main(&ast_node, hash_list);                        // 5
  print_ast_ans_node(ast_node);                                         // 6

  normal_debug_ans_ast_print(ast_node, 0);                              // 7 (deprecated)
  delete_ans_token_list(&token_list);                                   // 8
  delete_ans_ast_node(ast_node);                                        // 9
  delete_hash_process_list(hash_list);                                  // 10

  return 0;
}
```

# ANS Compiler Front-End

A lightweight experimental **compiler front-end** for the ANS language.  
Currently supports **Lexical Analysis** and **AST Construction** phases.

---

##  Build & Run

### 1. Build

```bash
gcc main.c ext_alloc.c str_ext.c lexer_main_ans.c hash_process_ans_main.c parser_main_ans.c
```
## Example: Basic `obj` Declaration and `return`

Below is a simple example written in **ANS** syntax:

```ans
obj add(obj x, obj y) {
    return x + y;
}

obj var = add(x, y);
return var;
🔍 Explanation
## 1. obj Declaration

obj is a generic data type used in ANS.
It can hold various types (numbers, strings, or user-defined objects), similar to a dynamically typed variable in high-level languages.
```ans
obj var = add(x, y);
```

Here, var is declared as an obj and initialized with the result of the function add(x, y).

##2. Function return
```ans
return x + y;
```

This return is inside the function scope — it exits the add function and sends the computed value (x + y) back to the caller.

## 3. Top-Level return
```ans
return var;
```

At the top level (outside of any function),
return behaves as the program’s final output — it ends the program execution and returns the last evaluated value to the runtime environment.
This is similar to returning an exit value from main() in C.

## Summary
Concept	Description
obj	A flexible, dynamically typed object type
Function return	Returns a value from within a function scope
Top-level return	Ends program execution and returns a final result

## Note: In the current implementation,
the parser recognizes both function-scope and top-level return statements,
but runtime evaluation is still under development.
