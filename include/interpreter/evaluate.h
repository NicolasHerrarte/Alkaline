#ifndef EVALUATE_H
#define EVALUATE_H

#include <stdio.h>
#include <stdlib.h>


#include "utils/ast.h"
#include "utils/dynarray.h"
#include "interpreter/symbols.h"

void print_vvalue(VValue* v);
void print_type(Type tp);
void print_eval_pass(EvalPass ep);

bool type_equals(Type a, Type b);
void type_modify(Type* type_ptr, Type type_from);
size_t iterations_per_type(Type* type);

EvalPass eval_comparison(EvalPass left, EvalPass right, int op);
EvalPass eval_arithmetic(EvalPass left, EvalPass right, char op);
VValue var_deep_copy(VValue val, Type type, Arena* dest_arena);
Variable unpack_pointer(Variable variable);
EvalPass refactor_access_to_var(SymbolsManager* manager, EvalPass packed_expr);
EvalPass refactor_var_to_access(SymbolsManager* manager, Arena* current_arena, EvalPass unpacked_expr);
EvalPass evaluate(SymbolsManager* manager, ASTNode* node, Arena* current_arena);

void print_execute(void* manager_void, ...);
void print_define(SymbolsManager* manager);

SymbolsManager* create_symbols_manager(bool production);
void calculate_tree(TreeManager tree_manager, bool production);

#endif