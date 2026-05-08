#include <stdio.h>
#include "parser/parser.h"
#include "interpreter/evaluate.h"

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
    };

    int symbols_amount = 83;

    char* language_src = "language.k";
    char* language_regex = "(=?)$20|(>=)$21|(<=)$22|(>)$23|(<)$24|(/|/|)$25|(&&)$26|+$07|-$08|/*$09|//$10|/($11|/)$12|/[$16|/]$17|.$18|,$19|(0|[1-9][0-9]*)$13|((0|[1-9][0-9]*).[0-9][0-9]*)f$14|(\"([a-zA-Z0-9_][a-zA-Z0-9_]*)\")$27|(true)$28|(false)$29|(if)$35|(else)$36|(while)$37|(for)$38|(init)$39|(proc)$40|(return)$41|({)$42|(})$43|(;)$44|(<-)$45|(=)$46|(:)$47|(->)$48|(int)$49|(bool)$50|(float)$51|(string)$52|(void)$53|(break)$54|(continue)$55|(assign)$56|([a-zA-Z_][a-zA-Z0-9_]*)$15|(( |\n|\t|\r)( |\n|\t|\r)*)$01";

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

    calculate_tree(ast, false);

    printf("TREE DONE\n");
    fflush(stdout);

    // TO DO
    // TRY ARRAY AS ARGS
    // MODIFY LISTRING TO ACCEPT . and spaces
    // MAKETHE BUILTINS IN THE MAIN C FILE
}