#ifndef _AMH_HASH_PROCESS_MAIN_H_
#define _AMH_HASH_PROCESS_MAIN_H_
#include "lexer_main_amh.h"
#include "hash_process_module_host.h"


typedef enum Run_Amh_Token_Type Run_Amh_Token_Type;

static inline unsigned int get_host_extend_hash_type_token(const char* src_token_str) { return 0; }
Run_Amh_Token_Type get_hash_type_token(Amh_Lex_Token_List* src_lex_list);

#endif // !_AMH_HASH_PROCESS_MAIN_H_
