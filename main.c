#include <stdio.h>
#include "parser/parser.h"
#include "interpreter/evaluate.h"

int print_primitive(Variable var_print, bool only_length){
    switch (prim_type(var_print)) {
        case VAR_TYPE_INT:
            if(only_length){
                return snprintf(NULL, 0, "%d\n", prim_int(var_print));
            }
            else{
                return printf("%d\n", prim_int(var_print));
            }
            
        case VAR_TYPE_FLOAT:
            if(only_length){
                return snprintf(NULL, 0, "%.2f\n", prim_float(var_print));
            }
            else{
                return printf("%.2f\n", prim_float(var_print));
            }
        case VAR_TYPE_BOOL:
            if(only_length){
                return snprintf(NULL, 0, "%s\n", prim_bool(var_print) ? "true" : "false");
            }
            else{
                return printf("%s\n", prim_bool(var_print) ? "true" : "false");
            } 
        case VAR_TYPE_STRING:
            if(only_length){
                return snprintf(NULL, 0, "%s\n", prim_str(var_print) ? prim_str(var_print) : "null");
            }
            else{
                return printf("%s\n", prim_str(var_print) ? prim_str(var_print) : "null");
            }
        default:
            printf("Unsuported type for print function %d\n", prim_type(var_print));
            assert(false);
            break;
    }
}

void* print_execute(void* manager_void, ...){
    init_symbols(manager_void);
    var_fetch(generic_print);

    decl_return(int_pass);
    
    if(var_is_prim(generic_print)){
        print_primitive(generic_print, false);
    }
    else if(var_is_ptr(generic_print)){
        assert(false);
    }
    else if(var_is_arr(generic_print)){
        //int* sizes = arr_size(generic_print);
        //int ndims = arr_ndims(generic_print);

        assert(false);
    }
    else{
        assert(false);
    }

    end_symbols(int_pass);
}

void* to_int_execute(void* manager_void, ...){
    init_symbols(manager_void);
    var_fetch(unconverted);

    assert(var_is_prim(unconverted));

    decl_return(int_pass);
    init_return(int_pass);

    switch (prim_type(unconverted)) {
        case VAR_TYPE_INT: {
            set_return(int_pass, unconverted);
            break;
        }
        case VAR_TYPE_FLOAT: {
            int int_value = (int) prim_float(unconverted);
            make_int(float_to_int, int_value);
            set_return(int_pass, float_to_int);
            break;
        }
        case VAR_TYPE_BOOL: {
            int int_value = (int) prim_bool(unconverted);
            make_int(bool_to_int, int_value);
            set_return(int_pass, bool_to_int);
            break;
        }
        case VAR_TYPE_STRING: {
            char* str_value = prim_str(unconverted);
            char* endptr;
            int int_value = (int) strtol(str_value, &endptr, 10);

            if (endptr == str_value || *endptr != '\0') {
                printf("Cannot convert string \"%s\" to int\n", str_value);
                assert(false);
            }

            make_int(string_to_int, int_value);
            set_return(int_pass, string_to_int);
            break;
        }
        default: {
            printf("Unsuported type %d for int conversion\n", prim_type(unconverted));
            assert(false);
            break;
        }
    }

    end_symbols(int_pass);
}

void* to_float_execute(void* manager_void, ...){
    init_symbols(manager_void);
    var_fetch(unconverted);

    assert(var_is_prim(unconverted));

    decl_return(float_pass);
    init_return(float_pass);

    switch (prim_type(unconverted)) {
        case VAR_TYPE_FLOAT: {
            set_return(float_pass, unconverted);
            break;
        }
        case VAR_TYPE_INT: {
            float float_value = (float) prim_int(unconverted);
            make_float(int_to_float, float_value);
            set_return(float_pass, int_to_float);
            break;
        }
        case VAR_TYPE_BOOL: {
            float float_value = (float) prim_bool(unconverted);
            make_float(bool_to_float, float_value);
            set_return(float_pass, bool_to_float);
            break;
        }
        case VAR_TYPE_STRING: {
            char* str_value = prim_str(unconverted);
            char* endptr;
            float float_value = strtof(str_value, &endptr);

            if (endptr == str_value || *endptr != '\0') {
                printf("Cannot convert string \"%s\" to float\n", str_value);
                assert(false);
            }

            make_float(string_to_float, float_value);
            set_return(float_pass, string_to_float);
            break;
        }
        default: {
            printf("Unsuported type %d for float conversion\n", prim_type(unconverted));
            assert(false);
            break;
        }
    }

    end_symbols(float_pass);
}

void* to_string_execute(void* manager_void, ...){
    init_symbols(manager_void);
    var_fetch(unconverted);

    assert(var_is_prim(unconverted));

    decl_return(string_pass);
    init_return(string_pass);

    switch (prim_type(unconverted)) {
        case VAR_TYPE_FLOAT: {
            int string_length = snprintf(NULL, 0, "%.7g", prim_float(unconverted)) + 1;
            char* string_value = local_storage_get(string_length*sizeof(char));
            snprintf(string_value, string_length, "%.7g", prim_float(unconverted));
            make_string(float_to_string, string_value);
            set_return(string_pass, float_to_string);
            break;
        }
        case VAR_TYPE_INT: {
            int string_length = snprintf(NULL, 0, "%d", prim_int(unconverted)) + 1;
            char* string_value = local_storage_get(string_length*sizeof(char));
            snprintf(string_value, string_length, "%d", prim_int(unconverted));
            make_string(int_to_string, string_value);
            set_return(string_pass, int_to_string);
            break;
        }
        case VAR_TYPE_BOOL: {
            bool value_bool = prim_bool(unconverted);
            char* string_value;
            if(value_bool){
                string_value = "true";
            }
            else{
                string_value = "false";
            }
            make_string(bool_to_string, string_value);
            set_return(string_pass, bool_to_string);
            break;
        }
        case VAR_TYPE_STRING: {
            set_return(string_pass, unconverted);
            break;
        }
        default: {
            printf("Unsuported type %d for float conversion\n", prim_type(unconverted));
            assert(false);
            break;
        }
    }

    end_symbols(string_pass);
}

void* input_execute(void* manager_void, ...){
    init_symbols(manager_void);

    decl_return(read_pass);
    init_return(read_pass);

    char buffer[MAX_INPUT_BUILTIN];
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';

    int input_size = strnlen(buffer, MAX_INPUT_BUILTIN);

    char* str_copy = local_storage_get((input_size+1) * sizeof(char));
    strncpy(str_copy, buffer, input_size);

    make_string(input_string, str_copy);
    set_return(read_pass, input_string);

    end_symbols(read_pass);
}

void print_define(SymbolsManager* manager){
    Argument argument;
    argument.mode = PARAMETER_MODE;
    argument.name = "generic_print";
    argument.argtype = GENERIC_MACRO_TYPE;

    external_define(manager, "print", VOID_MACRO_TYPE, print_execute, 1, argument);
}

void to_int_define(SymbolsManager* manager){
    Argument argument;
    argument.mode = PARAMETER_MODE;
    argument.name = "unconverted";
    argument.argtype = GENERIC_MACRO_TYPE;

    external_define(manager, "toint", INT_MACRO_TYPE, to_int_execute, 1, argument);
}

void to_float_define(SymbolsManager* manager){
    Argument argument;
    argument.mode = PARAMETER_MODE;
    argument.name = "unconverted";
    argument.argtype = GENERIC_MACRO_TYPE;

    external_define(manager, "tofloat", FLOAT_MACRO_TYPE, to_float_execute, 1, argument);
}

void to_string_define(SymbolsManager* manager){
    Argument argument;
    argument.mode = PARAMETER_MODE;
    argument.name = "unconverted";
    argument.argtype = GENERIC_MACRO_TYPE;

    external_define(manager, "tostr", STRING_MACRO_TYPE, to_string_execute, 1, argument);
}

void input_define(SymbolsManager* manager){
    external_define(manager, "read", STRING_MACRO_TYPE, input_execute, 0);
}

int main(){
    Pair mapping[] = {
        {"End",             0},
        {"Epsilon",         1},
        {"Goal",            2},
        {"Expr",            3},
        {"Eval",            4},
        {"Term",            5},
        {"Factor",          6},
        {"+",               7},
        {"-",               8},
        {"*",               9},
        {"/",               10},
        {"(",               11},
        {")",               12},
        {"integer",         13},
        {"floating",        14},
        {"name",            15},
        {"[",               16},
        {"]",               17},
        {".",               18},
        {",",               19},
        {"=?",              20},
        {">=",              21},
        {"<=",              22},
        {">",               23},
        {"<",               24},
        {"||",              25},
        {"&&",              26},
        {"litstring",       27},
        {"true",            28},
        {"false",           29},
        {"Access",          30},
        {"AccessBase",      31},
        {"RelOp",           32},
        {"Args",            33},
        {"ArgList",         34},
        {"if",              35}, 
        {"else",            36},
        {"while",           37},
        {"for",             38},
        {"init",            39},
        {"proc",            40},
        {"return",          41},
        {"{",               42},
        {"}",               43},
        {";",               44},
        {"<-",              45},
        {"=",               46},
        {":",               47},
        {"->",              48},
        {"int",             49},
        {"bool",            50},
        {"float",           51},
        {"string",          52},
        {"void",            53},
        {"break",           54},
        {"continue",        55},
        {"assign",          56},
        {"Program",         57},
        {"Block",           58},
        {"CompStat",        59},
        {"UnitStat",        60},
        {"ControlStat",     61},
        {"Stat",            62},
        {"CondStat",        63},
        {"LoopStat",        64},
        {"While",           65},
        {"For",             66},
        {"Declaration",     67},
        {"ProcDeclaration", 68},
        {"Assignment",      69},
        {"VarType",         70},
        {"Primitive",       71},
        {"Jump",            72},
        {"Params",          73},
        {"ParamsList",      74},
        {"SingleParam",     75},
        {"Identifier",      76},
        {"ArrSize",         77},
        {"Instantiation",   78},
        {"ArrInst",         79},
        {"StorageAssign",   80},
        {"Relation",        81},
        {"LogAnd",          82},
        {"Negative",        83},
    };

    int symbols_amount = 84;

    char* language_src = "language.k";
    char* language_regex = "(=?)$20|(>=)$21|(<=)$22|(>)$23|(<)$24|(/|/|)$25|(&&)$26|+$07|-$08|/*$09|//$10|/($11|/)$12|/[$16|/]$17|.$18|,$19|(0|[1-9][0-9]*)$13|((0|[1-9][0-9]*).[0-9][0-9]*)f$14|((\"([a-zA-Z0-9. ][a-zA-Z0-9. ]*)\")|\"\")$27|(true)$28|(false)$29|(if)$35|(else)$36|(while)$37|(for)$38|(init)$39|(proc)$40|(return)$41|({)$42|(})$43|(;)$44|(<-)$45|(=)$46|(:)$47|(->)$48|(int)$49|(bool)$50|(float)$51|(string)$52|(void)$53|(break)$54|(continue)$55|(assign)$56|([a-zA-Z_][a-zA-Z0-9_]*)$15|(( |\n|\t|\r)( |\n|\t|\r)*)$01";

    char* rules_src = "grammar.k.specs";
    char* rules_regex = "(([a-zA-Z/(/)/*///-/[/]+=?><.;{},:/|&])([a-zA-Z/(/)/*///-/[/]+=?><.;{},:/|&])*)$02|///|$03|(//->)$04|//;$05|(//%%//)$06|(@sh)$07|(@ap)$08|(@mn)$09|(@bx)$10|(@vl)$11|(-/$(0|[1-9][0-9]*))$12|(-#)$13|((<([a-zA-Z_])([a-zA-Z_])*)>)$14|(/[(0|[1-9][0-9]*)/])$15|(( |\n|\t|\r)( |\n|\t|\r)*)$01";

    int ignore_categories_language[] = {1};
    int ignore_categories_rules[] = {1};

    char* lexer_dir = "static/lexer";
    char* parser_dir = "static/parser";

    bool generate_parsing_tables = false;
    bool generate_lexing_tables = false;

    bool debug = true;

    TreeManager ast = parse_pipeline(
        language_src, 
        language_regex, 
        rules_src, 
        rules_regex, 
        ignore_categories_language, 
        ignore_categories_rules, 
        mapping, symbols_amount, 
        lexer_dir, parser_dir, 
        generate_parsing_tables, 
        generate_lexing_tables, 
        debug
    );

    printf("PARSE DONE\n");
    fflush(stdout);

    calculate_tree(ast, false, 5, 
        print_define, 
        to_int_define, 
        to_float_define, 
        to_string_define, 
        input_define
    );

    printf("TREE DONE\n");
    fflush(stdout);

    // TO DO
    // TRY ARRAY AS ARGS
    // MODIFY LISTRING TO ACCEPT . and spaces
    // MAKETHE BUILTINS IN THE MAIN C FILE
}