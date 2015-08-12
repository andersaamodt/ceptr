/**
 * @ingroup def
 *
 * @{
 * @file sys_defs.c
 * @brief definition of system semantic elements for bootstrapping
 *
 * @copyright Copyright (C) 2013-2015, The MetaCurrency Project (Eric Harris-Braun, Arthur Brock, et. al).  This file is part of the Ceptr platform and is released under the terms of the license contained in the file LICENSE (GPLv3).
 */

#include "tree.h"
#include "def.h"

#define sT(name,num,...) name = _d_define_structure(G_sys_defs.structures,"" #name "",SYS_CONTEXT,num,__VA_ARGS__)
#define sY(name,str) name = _d_declare_symbol(G_sys_defs.symbols,str,"" #name "",SYS_CONTEXT)
#define sP(name,intention,in,out) name = _d_code_process(G_sys_defs.processes,0,"" #name "",intention,in,out,SYS_CONTEXT)
#define sYt(name,str) name = _d_declare_symbol(G_sys_defs.symbols,str,"" #name "",TEST_CONTEXT)

Symbol NULL_SYMBOL = {0,SEM_TYPE_SYMBOL,0};
Symbol DEFINITIONS = {SYS_CONTEXT,SEM_TYPE_SYMBOL,DEFINITIONS_ID};
Symbol STRUCTURES = {SYS_CONTEXT,SEM_TYPE_SYMBOL,STRUCTURES_ID};
Symbol STRUCTURE_DEFINITION = {SYS_CONTEXT,SEM_TYPE_SYMBOL,STRUCTURE_DEFINITION_ID};
Symbol STRUCTURE_LABEL = {SYS_CONTEXT,SEM_TYPE_SYMBOL,STRUCTURE_LABEL_ID};
Symbol STRUCTURE_PARTS = {SYS_CONTEXT,SEM_TYPE_SYMBOL,STRUCTURE_PARTS_ID};
Symbol STRUCTURE_PART = {SYS_CONTEXT,SEM_TYPE_SYMBOL,STRUCTURE_PART_ID};
Symbol SYMBOLS = {SYS_CONTEXT,SEM_TYPE_SYMBOL,SYMBOLS_ID};
Symbol PROCESSES = {SYS_CONTEXT,SEM_TYPE_SYMBOL,PROCESSES_ID};
Symbol SYMBOL_DECLARATION = {SYS_CONTEXT,SEM_TYPE_SYMBOL,SYMBOL_DECLARATION_ID};
Symbol SYMBOL_STRUCTURE = {SYS_CONTEXT,SEM_TYPE_SYMBOL,SYMBOL_STRUCTURE_ID};
Symbol SYMBOL_LABEL = {SYS_CONTEXT,SEM_TYPE_SYMBOL,SYMBOL_LABEL_ID};

Structure NULL_STRUCTURE = {0,SEM_TYPE_STRUCTURE,0};
Structure BIT = {SYS_CONTEXT,SEM_TYPE_STRUCTURE,BIT_ID};
Structure INTEGER = {SYS_CONTEXT,SEM_TYPE_STRUCTURE,INTEGER_ID};
Structure FLOAT = {SYS_CONTEXT,SEM_TYPE_STRUCTURE,FLOAT_ID};
Structure CHAR = {SYS_CONTEXT,SEM_TYPE_STRUCTURE,CHAR_ID};
Structure CSTRING = {SYS_CONTEXT,SEM_TYPE_STRUCTURE,CSTRING_ID};
Structure SYMBOL = {SYS_CONTEXT,SEM_TYPE_STRUCTURE,SYMBOL_ID};
Structure ENUM = {SYS_CONTEXT,SEM_TYPE_STRUCTURE,ENUM_ID};
Structure TREE_PATH = {SYS_CONTEXT,SEM_TYPE_STRUCTURE,TREE_PATH_ID};
Structure XADDR = {SYS_CONTEXT,SEM_TYPE_STRUCTURE,XADDR_ID};
Structure LIST = {SYS_CONTEXT,SEM_TYPE_STRUCTURE,LIST_ID};
Structure SURFACE = {SYS_CONTEXT,SEM_TYPE_STRUCTURE,SURFACE_ID};
Structure TREE = {SYS_CONTEXT,SEM_TYPE_STRUCTURE,TREE_ID};
Structure RECEPTOR = {SYS_CONTEXT,SEM_TYPE_STRUCTURE,RECEPTOR_ID};
Structure PROCESS = {SYS_CONTEXT,SEM_TYPE_STRUCTURE,PROCESS_ID};
Structure STRUCTURE = {SYS_CONTEXT,SEM_TYPE_STRUCTURE,STRUCTURE_ID};
Structure SCAPE = {SYS_CONTEXT,SEM_TYPE_STRUCTURE,SCAPE_ID};
Structure PROTOCOL = {SYS_CONTEXT,SEM_TYPE_STRUCTURE,PROTOCOL_ID};
Structure SEMTREX = {SYS_CONTEXT,SEM_TYPE_STRUCTURE,SEMTREX_ID};
Structure STREAM = {SYS_CONTEXT,SEM_TYPE_STRUCTURE,STREAM_ID};

Process NULL_PROCESS = {0,SEM_TYPE_PROCESS,0};
Process NOOP = {SYS_CONTEXT,SEM_TYPE_PROCESS,NOOP_ID};
Process IF = {SYS_CONTEXT,SEM_TYPE_PROCESS,IF_ID};
Process ADD_INT = {SYS_CONTEXT,SEM_TYPE_PROCESS,ADD_INT_ID};
Process SUB_INT = {SYS_CONTEXT,SEM_TYPE_PROCESS,SUB_INT_ID};
Process MULT_INT = {SYS_CONTEXT,SEM_TYPE_PROCESS,MULT_INT_ID};
Process DIV_INT = {SYS_CONTEXT,SEM_TYPE_PROCESS,DIV_INT_ID};
Process MOD_INT = {SYS_CONTEXT,SEM_TYPE_PROCESS,MOD_INT_ID};
Process EQ_INT = {SYS_CONTEXT,SEM_TYPE_PROCESS,EQ_INT_ID};
Process LT_INT = {SYS_CONTEXT,SEM_TYPE_PROCESS,LT_INT_ID};
Process GT_INT = {SYS_CONTEXT,SEM_TYPE_PROCESS,GT_INT_ID};
Process LTE_INT = {SYS_CONTEXT,SEM_TYPE_PROCESS,LTE_INT_ID};
Process GTE_INT = {SYS_CONTEXT,SEM_TYPE_PROCESS,GTE_INT_ID};
Process CONCAT_STR = {SYS_CONTEXT,SEM_TYPE_PROCESS,CONCAT_STR_ID};
Process RESPOND = {SYS_CONTEXT,SEM_TYPE_PROCESS,RESPOND_ID};
Process SEND = {SYS_CONTEXT,SEM_TYPE_PROCESS,SEND_ID};
Process EXPECT_ACT = {SYS_CONTEXT,SEM_TYPE_PROCESS,EXPECT_ACT_ID};
Process QUOTE = {SYS_CONTEXT,SEM_TYPE_PROCESS,QUOTE_ID};
Process INTERPOLATE_FROM_MATCH = {SYS_CONTEXT,SEM_TYPE_PROCESS,INTERPOLATE_FROM_MATCH_ID};
Process RAISE = {SYS_CONTEXT,SEM_TYPE_PROCESS,RAISE_ID};
Process READ_STREAM = {SYS_CONTEXT,SEM_TYPE_PROCESS,READ_STREAM_ID};
Process WRITE_STREAM = {SYS_CONTEXT,SEM_TYPE_PROCESS,WRITE_STREAM_ID};

void def_sys() {
    G_sys_root = _t_new_root(DEFINITIONS);
    G_sys_defs.symbols = _t_newr(G_sys_root,SYMBOLS);
    G_sys_defs.structures = _t_newr(G_sys_root,STRUCTURES);
    G_sys_defs.processes = _t_newr(G_sys_root,PROCESSES);

    sT(BIT,1,NULL_SYMBOL);
    sT(INTEGER,1,NULL_SYMBOL);
    sT(FLOAT,1,NULL_SYMBOL);
    sT(CHAR,1,NULL_SYMBOL);
    sT(CSTRING,1,NULL_SYMBOL);
    sT(SYMBOL,1,NULL_SYMBOL);
    sT(ENUM,1,NULL_SYMBOL);
    sT(TREE_PATH,1,NULL_SYMBOL);
    sT(XADDR,1,NULL_SYMBOL);
    sT(LIST,1,NULL_SYMBOL);
    sT(SURFACE,1,NULL_SYMBOL);
    sT(TREE,1,NULL_SYMBOL);
    sT(RECEPTOR,1,NULL_SYMBOL);
    sT(PROCESS,1,NULL_SYMBOL);
    sT(STRUCTURE,1,NULL_SYMBOL);
    sT(SCAPE,1,NULL_SYMBOL);
    sT(SEMTREX,1,NULL_SYMBOL);
    sT(STREAM,1,NULL_SYMBOL);

    sY(DEFINITIONS,LIST);
    sY(STRUCTURES,LIST);
    sY(STRUCTURE_DEFINITION,TREE);
    sY(STRUCTURE_LABEL,CSTRING);
    sY(STRUCTURE_PARTS,LIST);
    sY(STRUCTURE_PART,SYMBOL);
    sY(SYMBOLS,LIST);
    sY(SYMBOL_DECLARATION,TREE);
    sY(SYMBOL_STRUCTURE,STRUCTURE);
    sY(SYMBOL_LABEL,CSTRING);

    sY(BOOLEAN,BIT);
    sY(SEMTREX_MATCH_PATH,TREE_PATH);
    sY(SEMTREX_SYMBOL_LITERAL,TREE);
    sY(SEMTREX_SYMBOL_LITERAL_NOT,TREE);
    sY(SEMTREX_SYMBOL_SET,LIST);
    sY(SEMTREX_SYMBOL,SYMBOL);
    sY(SEMTREX_SEQUENCE,LIST);
    sY(SEMTREX_OR,SEMTREX); //@todo, this really should be something like SEMTREX_PAIR
    sY(SEMTREX_NOT,SEMTREX);
    sY(SEMTREX_SYMBOL_ANY,NULL_STRUCTURE);
    sY(SEMTREX_ZERO_OR_MORE,SEMTREX);
    sY(SEMTREX_ONE_OR_MORE,SEMTREX);
    sY(SEMTREX_ZERO_OR_ONE,SEMTREX);
    sY(SEMTREX_VALUE_LITERAL,TREE);
    sY(SEMTREX_VALUE_LITERAL_NOT,TREE);
    sY(SEMTREX_VALUE_SET,LIST);
    sY(SEMTREX_GROUP,SYMBOL);
    sY(SEMTREX_DESCEND,NULL_STRUCTURE);
    sY(SEMTREX_WALK,SEMTREX);
    sY(SEMTREX_MATCH,INTEGER);
    sY(SEMTREX_MATCH_CURSOR,NULL_STRUCTURE);
    sY(SEMTREX_MATCH_RESULTS,NULL_STRUCTURE);
    sY(SEMTREX_MATCH_SYMBOL,SYMBOL);
    sY(SEMTREX_MATCH_SIBLINGS_COUNT,INTEGER);

    sY(ASCII_CHAR,CHAR);
    sY(ASCII_CHARS,LIST);

    sY(RECEPTOR_XADDR,XADDR);
    sY(FLUX,LIST);
    sY(SCAPE_SPEC,TREE);
    sY(ASPECTS,LIST);
    sY(ASPECT_DEF,TREE);
    sY(ASPECT_TYPE,BIT);
    sY(CARRIER,INTEGER); //@todo fix to proper structure when we understand this  currently CARRIER is c pointer, but we have it set to an INTEGER so it prints in the tree debug dump
    sY(ASPECT,INTEGER);
    sY(SIGNALS,LIST);
    sY(SIGNAL,TREE);
    sY(ENVELOPE,LIST);
    sY(BODY,TREE);
    sY(LISTENERS,LIST);
    sY(LISTENER,SYMBOL);
    sY(EXPECTATION,SEMTREX);
    sY(ACTION,PROCESS);
    sY(INTERPOLATE_SYMBOL,SYMBOL);
    sY(PROCESSES,LIST);
    sY(PROCESS_CODING,TREE);
    sY(PROCESS_NAME,CSTRING);
    sY(PROCESS_INTENTION,CSTRING);
    sY(INPUT,LIST);
    sY(INPUT_SIGNATURE,TREE);
    sY(INPUT_LABEL,CSTRING);
    sY(SIGNATURE_STRUCTURE,STRUCTURE);
    sY(OUTPUT_SIGNATURE,NULL_STRUCTURE);
    sY(RUN_TREE,TREE);
    sY(PARAM_REF,TREE_PATH);
    sY(PARAMS,LIST);
    sY(RESULT_SYMBOL,SYMBOL);
    sY(REDUCTION_ERROR_SYMBOL,SYMBOL);

    sY(PROTOCOLS,LIST);
    sY(SEQUENCES,LIST);
    //    sY(SEQUENCE,LIST); // @todo this should really be a structure be we don't have optional elements yet....
    sY(STEP_SYMBOL,SYMBOL);
    sY(STEPS,LIST);
    sT(SEQUENCE,1,STEP_SYMBOL); // this really should specify one or more step_symbols
    sT(PROTOCOL_STEP,2,EXPECTATION,ACTION);
    sY(INTERACTIONS,LIST);
    sY(INTERACTION,LIST); // @todo this should really be a structure be we don't have optional elements yet....
    sY(STEP,CSTRING);
    sY(RESPONSE_STEPS,LIST);
    sT(PROTOCOL,2,STEPS,SEQUENCES);
    sY(SCAPES,LIST);
    sY(VM_HOST_RECEPTOR,RECEPTOR);
    sY(CLOCK_RECEPTOR,RECEPTOR);
    sY(COMPOSITORY,RECEPTOR);
    sY(MANIFEST,TREE);
    sY(MANIFEST_PAIR,LIST);
    sY(MANIFEST_LABEL,CSTRING);
    sY(MANIFEST_SPEC,SYMBOL);
    sY(RECEPTOR_PACKAGE,TREE);
    sY(RECEPTOR_IDENTIFIER,INTEGER);
    sY(INSTALLED_RECEPTOR,TREE);
    sY(ACTIVE_RECEPTORS,LIST);
    sY(PENDING_SIGNALS,LIST);
    sY(BINDINGS,TREE);
    sY(BINDING_PAIR,LIST);

    sY(YEAR,INTEGER);
    sY(MONTH,INTEGER);
    sY(DAY,INTEGER);
    sY(HOUR,INTEGER);
    sY(MINUTE,INTEGER);
    sY(SECOND,INTEGER);
    sT(DATE,3,YEAR,MONTH,DAY);
    sT(TIME,3,HOUR,MINUTE,SECOND);
    sY(TODAY,DATE);
    sY(NOW,TIME);
    sT(TIMESTAMP,2,TODAY,NOW);
    sY(TICK,TIMESTAMP);

    sY(ERROR_LOCATION,TREE_PATH);
    sT(REDUCTION_ERROR,1,ERROR_LOCATION);
    sY(ZERO_DIVIDE_ERR,REDUCTION_ERROR);
    sY(TOO_FEW_PARAMS_ERR,REDUCTION_ERROR);
    sY(TOO_MANY_PARAMS_ERR,REDUCTION_ERROR);
    sY(SIGNATURE_MISMATCH_ERR,REDUCTION_ERROR);
    sY(NOT_A_PROCESS_ERR,REDUCTION_ERROR);
    sY(NOT_IN_SIGNAL_CONTEXT_ERR,REDUCTION_ERROR);
    sY(INCOMPATIBLE_TYPE_ERR,REDUCTION_ERROR);
    sY(UNIX_ERRNO_ERR,REDUCTION_ERROR);

    sP(NOOP,"no-op",0,0);
    sP(IF,"if",0,0);
    sP(ADD_INT,"addition",0,0);
    sP(SUB_INT,"subtract",0,0);
    sP(MULT_INT,"multiply",0,0);
    sP(DIV_INT,"divide",0,0);
    sP(MOD_INT,"modulo",0,0);
    sP(EQ_INT,"test equality",0,0);
    sP(LT_INT,"test less than",0,0);
    sP(GT_INT,"test greater than",0,0);
    sP(LTE_INT,"test less than or equal",0,0);
    sP(GTE_INT,"test greater than or equal",0,0);
    sP(CONCAT_STR,"concatinate strings",0,0);
    sP(RESPOND,"respond to signal",0,0);
    sP(SEND,"send a signal",0,0);
    sP(EXPECT_ACT,"block run tree until expectation met and then act",0,0);
    sP(QUOTE,"quote a process so that it can be passed as a value",0,0);
    sP(INTERPOLATE_FROM_MATCH,"interploate match result in place",0,0);
    sP(RAISE,"raise reduction error",0,0);
    sP(READ_STREAM,"read from a unix stream",0,0);
    sP(WRITE_STREAM,"write to a unix stream",0,0);

    sYt(TEST_INT_SYMBOL,INTEGER);
    sYt(TEST_INT_SYMBOL2,INTEGER);
    sYt(TEST_FLOAT_SYMBOL,FLOAT);
    sYt(TEST_STR_SYMBOL,CSTRING);
    sYt(TEST_TREE_SYMBOL,TREE);
    sYt(TEST_TREE_SYMBOL2,TREE);
    sYt(TEST_STREAM_SYMBOL,STREAM);
    sYt(TEST_NAME_SYMBOL,CSTRING);
    sYt(TEST_RECEPTOR_SYMBOL,RECEPTOR);
    sYt(TEST_ALPHABETIZE_SCAPE_SYMBOL,SCAPE);

    sY(STX_TOKENS,LIST);
    sY(STX_SL,NULL_STRUCTURE);
    sY(STX_OP,NULL_STRUCTURE);
    sY(STX_CP,NULL_STRUCTURE);
    sY(STX_SET,NULL_STRUCTURE);
    sY(STX_OS,NULL_STRUCTURE);
    sY(STX_CS,NULL_STRUCTURE);
    sY(STX_LABEL,CSTRING);
    sY(STX_OG,CSTRING);
    sY(STX_CG,NULL_STRUCTURE);
    sY(STX_EQ,CSTRING);
    sY(STX_NEQ,CSTRING);
    sY(STX_WALK,NULL_STRUCTURE);
    sY(STX_STAR,NULL_STRUCTURE);
    sY(STX_PLUS,NULL_STRUCTURE);
    sY(STX_Q,NULL_STRUCTURE);
    sY(STX_OR,NULL_STRUCTURE);
    sY(STX_COMMA,NULL_STRUCTURE);
    sY(STX_EXCEPT,CSTRING);
    sY(STX_NOT,NULL_STRUCTURE);
    sY(STX_VAL_S,CSTRING);
    sY(STX_VAL_C,CHAR);
    sY(STX_VAL_I,INTEGER);
    sY(STX_VAL_F,FLOAT);

    sY(STX_SIBS,LIST);
    sY(STX_CHILD,TREE);
    sY(STX_POSTFIX,TREE);

    sY(TREE_DELTA_PATH,TREE_PATH);
    sY(TREE_DELTA_VALUE,TREE);
    sY(TREE_DELTA_COUNT,INTEGER);
    sT(TREE_DELTA,3,TREE_DELTA_PATH,TREE_DELTA_VALUE,TREE_DELTA_COUNT);
    sY(TREE_DELTA_ADD,TREE_DELTA);
    sY(TREE_DELTA_REPLACE,TREE_DELTA);



}
void sys_free() {
    _t_free(G_sys_root);
}
