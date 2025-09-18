#include "amh_hash_process_main.h"

Run_Amh_Token_Type get_hash_type_token(Amh_Lex_Token_List* src_lex_list) {
	Run_Amh_Token_Type dst_hash_type = E_Amh_Type_NULL;

	const Amh_Lex_Token* tmp_now_amh_token = get_now_amh_token(src_lex_list);
	if (!tmp_now_amh_token)return dst_hash_type;

	if (tmp_now_amh_token->amh_token_type == E_Amh_Lex_Token_Type_Hash_Process) {
		if (simple_strcmp(tmp_now_amh_token->amh_token, "acc")) {
			dst_hash_type = E_Amh_Hash_Host_Acc;
		}
		else if (simple_strcmp(tmp_now_amh_token->amh_token, "class")) {
			dst_hash_type = E_Amh_Hash_Syntactic_Sugar_Class;
		}
		else if (simple_strcmp(tmp_now_amh_token->amh_token, "module")) {
			dst_hash_type = E_Amh_Hash_Syntactic_Sugar_Module;

		}
		else if (simple_strcmp(tmp_now_amh_token->amh_token, "run")) {
			dst_hash_type = E_Amh_Hash_Host_Run;
		}
		else if (simple_strcmp(tmp_now_amh_token->amh_token, "imp_run")) {
			dst_hash_type = E_Amh_Hash_Host_Imp_Run;

		}
		else dst_hash_type = (Run_Amh_Token_Type)get_host_extend_hash_type_token(tmp_now_amh_token->amh_token);
		consume_advance_list(src_lex_list);
	}
	return dst_hash_type;
}