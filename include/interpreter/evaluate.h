#ifndef EVALUATE_H
#define EVALUATE_H

#include <stdio.h>
#include <stdlib.h>

#include "utils/ast.h"
#include "utils/dynarray.h"
#include "interpreter/symbols.h"

#define INT_MACRO_TYPE (Type){.kind = KIND_PRIMITIVE, .primitive_tag = VAR_TYPE_INT}
#define FLOAT_MACRO_TYPE (Type){.kind = KIND_PRIMITIVE, .primitive_tag = VAR_TYPE_FLOAT}
#define BOOL_MACRO_TYPE (Type){.kind = KIND_PRIMITIVE, .primitive_tag = VAR_TYPE_BOOL}
#define STRING_MACRO_TYPE (Type){.kind = KIND_PRIMITIVE, .primitive_tag = VAR_TYPE_STRING}
#define VOID_MACRO_TYPE (Type){.kind = KIND_PRIMITIVE, .primitive_tag = VAR_TYPE_VOID}
#define GENERIC_MACRO_TYPE (Type){.kind = KIND_PRIMITIVE, .primitive_tag = VAR_TYPE_GENERIC}
#define ANY_MACRO_TYPE (Type){.kind = KIND_ANY}

#define init_symbols(ptr_name) \
    va_list args; \
    va_start(args, ptr_name); \
    Arena* arena = va_arg(args, Arena*); \
    SymbolsManager* manager = (SymbolsManager*) ptr_name 

#define end_symbols(name) \
    va_end(args); \
    return name

#define var_fetch(name) \
    Variable name = { \
        .storage = *getAttributeIdentifier(manager, #name, "value"), \
        .vtype = *((Type*) (getAttributeIdentifier(manager, #name, "type")->value_ptr)) \
    }

#define decl_return(name) EvalPass* name = NULL
#define init_return(name) name = arena_get(arena, sizeof(EvalPass))
#define set_return(name, var) \
    _Static_assert(_Generic((var), Variable: 1, default: 0), "set_return requires a Variable"); \
    *name = (EvalPass) { \
        .tag = SIGNAL_TYPE, \
        .as_signal = (Signal){.signal = SIGNAL_RETURN, .variable = var} \
    }

#define local_storage_get(arena_size) arena_get(arena, arena_size)

#define var_is_prim(name) ((name).vtype.kind == KIND_PRIMITIVE)
#define var_is_ptr(name) ((name).vtype.kind == KIND_POINTER)
#define var_is_arr(name) ((name).vtype.kind == KIND_ARRAY)

#define prim_type(name) ((name).vtype.primitive_tag)
#define arr_type(name) ((name).vtype.array.elem_type)

#define arr_size(name) ((name).vtype.array.sizes)
#define arr_ndims(name) ((name).vtype.array.ndims)

#define prim_int(name) ((name).storage.value_int)
#define prim_float(name) ((name).storage.value_float)
#define prim_bool(name) ((name).storage.value_bool)
#define prim_str(name) ((name).storage.value_string)

#define make_int(name, value) \
    _Static_assert(_Generic((value), int: 1, default: 0), "make_int requires an int"); \
    Variable name = (Variable) { \
        .vtype = INT_MACRO_TYPE, \
        .storage = (VValue){.vv_tag = VV_INT, .value_int = value} \
    }

#define make_float(name, value) \
    _Static_assert(_Generic((value), float: 1, default: 0), "make_float requires a float"); \
    Variable name = (Variable) { \
        .vtype = FLOAT_MACRO_TYPE, \
        .storage = (VValue){.vv_tag = VV_FLOAT, .value_float = value} \
    }

#define make_string(name, value) \
    _Static_assert(_Generic((value), char*: 1, default: 0), "make_string requires a char*"); \
    Variable name = (Variable) { \
        .vtype = STRING_MACRO_TYPE, \
        .storage = (VValue){.vv_tag = VV_STRING, .value_string = value} \
    }

#define EPSILON_FLOAT_COMP 0.0001f

#define MAX_INPUT_BUILTIN 256

typedef enum {
    LOP_EQ,
    LOP_GTE,
    LOP_LTE,
    LOP_GT,
    LOP_LT,
} LopTag;

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
EvalPass refactor_access_to_var(SymbolsManager* manager, EvalPass packed_expr, bool unpack_ptr);
EvalPass refactor_var_to_access(SymbolsManager* manager, Arena* current_arena, EvalPass unpacked_expr);
EvalPass evaluate(SymbolsManager* manager, ASTNode* node, Arena* current_arena);

void* print_execute(void* manager_void, ...);
void print_define(SymbolsManager* manager);
void external_define(SymbolsManager* manager, char* func_name, Type return_type, void* (*external_func)(void*, ...), int args_amount, ...);

SymbolsManager* create_symbols_manager(bool production);
void calculate_tree(TreeManager tree_manager, bool production, int amount_builtin, ...);

#endif