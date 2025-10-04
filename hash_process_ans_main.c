#include "hash_process_ans_main.h"
#include "lexer_main_ans.h"
#include "parser_main_ans.h"
bool standard_hash_process_ans(Ans_Hash_Process_List* src_hash_process_list, Ans_Lex_Token_List* src_token_list) {
	Ans_Lex_Token* now_lex_token = peek_ans_lex_token(src_token_list);
	if (!now_lex_token)return false;
	if (now_lex_token->ans_token_type == E_Ans_Lex_Token_Type_Hash_Process) {
		if (ans_token_list_expect_token_str(src_token_list, "module")) {
			if (src_hash_process_list->file_modularization_process & HASH_PROCESS_STANDARD_ID) {
			src_hash_process_list->file_modularization_process = HASH_PROCESS_MODULE_ID;
		}
			return false;
		}
		else if (ans_token_list_expect_token_str(src_token_list, "class")) {
			if (src_hash_process_list->file_modularization_process & HASH_PROCESS_STANDARD_ID)src_hash_process_list->file_modularization_process = HASH_PROCESS_CLASS_ID;
			return false;
		}
		else consume_ans_lex_token(src_token_list);/*not found hash process cosume*/
		return false;
	}
	return true;
}
void start_hash_process_main(Ans_Ast_Nodes** src_ast_node, Ans_Hash_Process_List* src_hash_process_list) {
	if (src_hash_process_list->file_modularization_process & HASH_PROCESS_MODULE_ID) {
		Ans_Ast_Nodes* tmp_result_module = change_to_module_hash_process(*src_ast_node, src_hash_process_list->abstract_host_name);
		if(tmp_result_module)*src_ast_node = tmp_result_module;

	}
	else if (src_hash_process_list->file_depency_process & HASH_PROCESS_CLASS_ID) {

	}
}
Ans_Ast_Nodes* delete_chain_ans_ast_build(Ans_Ast_Nodes* src_ast_node) {
	while (src_ast_node) {
		Ans_Ast_Nodes* now_node = src_ast_node->left;
		if (src_ast_node->op)free(src_ast_node->op);
		free(src_ast_node);
		src_ast_node = now_node;
	}
	return NULL;
}

Ans_Ast_Nodes* delete_module_only_build(Ans_Ast_Nodes* src_ast_filed_node, Ans_Ast_Nodes* src_ast_host_node, Ans_Ast_Nodes* src_ast_toplevel_node, Ans_Ast_Nodes* src_ast_class_node) {
	delete_chain_ans_ast_build(src_ast_filed_node);
	delete_chain_ans_ast_build(src_ast_host_node);
	delete_chain_ans_ast_build(src_ast_toplevel_node);
	delete_chain_ans_ast_build(src_ast_class_node);
	return NULL;
}
Ans_Ast_Nodes* change_to_module_hash_process(Ans_Ast_Nodes* src_ast_node, const char* module_name) {
	if (!src_ast_node)return NULL;
	Ans_Ast_Nodes* public_decl_node = NULL;
	Ans_Ast_Nodes* tmp_public_decl_node = NULL;
	Ans_Ast_Nodes* toplevel_statement_node = NULL;
	Ans_Ast_Nodes* tmp_toplevel_statement_node = NULL;
	if (src_ast_node->token_type == E_Ans_Ast_Token_Type_Abstract_Host) {
		for (Ans_Ast_Nodes* abs_ans = src_ast_node; abs_ans; abs_ans = abs_ans->left) {
			Ans_Ast_Nodes* decl_node = change_to_module_hash_process_helper(abs_ans->right);
			if (decl_node) {
				/*decl all public*/
				set_up_ans_ast_type(abs_ans, E_Ans_Ast_Token_Type_Class_Filed_Public);

				if (!public_decl_node)tmp_public_decl_node = public_decl_node = abs_ans;
				else {
					public_decl_node->left = abs_ans;
					public_decl_node = abs_ans;
				}
			}
			else {
				if (!toplevel_statement_node)tmp_toplevel_statement_node = toplevel_statement_node = abs_ans;
				else {
					toplevel_statement_node->left = abs_ans;
					toplevel_statement_node = abs_ans;
				}
			}
		}
	}
	if (public_decl_node && public_decl_node->left)public_decl_node->left = NULL;
	if (toplevel_statement_node && toplevel_statement_node->left) toplevel_statement_node->left = NULL;

	/*toplevel function*/
	Ans_Ast_Nodes* toplevel_func_decl = set_up_ans_ast(NULL, tmp_toplevel_statement_node, E_Ans_Ast_Token_Type_Func_Decl, module_name, ext_strlen_add_null(module_name));
	if (!toplevel_func_decl)return NULL;
	/*mod filed decl*/
	Ans_Ast_Nodes* mod_filed_decl = set_up_ans_ast(NULL, toplevel_func_decl, E_Ans_Ast_Token_Type_Class_Filed_Public, NULL, 0);
	if (!mod_filed_decl) {
		free(toplevel_func_decl->op);

		free(toplevel_func_decl);
		return NULL;
	}
	mod_filed_decl->left = tmp_public_decl_node;
	Ans_Ast_Nodes* mod_node = set_up_ans_ast(mod_filed_decl, NULL, E_Ans_Ast_Token_Type_Class_Decl, module_name, ext_strlen_add_null(module_name));
	if (!mod_node) {
		free(mod_filed_decl);
		free(toplevel_func_decl->op);

		free(toplevel_func_decl);
		return NULL;
	}
	Ans_Ast_Nodes* decision_module_absract_node = set_up_ans_ast(NULL, mod_node, E_Ans_Ast_Token_Type_Abstract_Host, NULL, 0);
	if (!decision_module_absract_node) {

		free(mod_filed_decl);
		free(toplevel_func_decl->op);

		free(toplevel_func_decl);
		free(mod_node->op);
		free(mod_node);
	}
	return decision_module_absract_node;
}
Ans_Ast_Nodes* change_to_module_hash_process_helper(Ans_Ast_Nodes* src_ast_node) {
	if (!src_ast_node)return NULL;
	if (src_ast_node->token_type == E_Ans_Ast_Token_Type_Var_Decl
		|| src_ast_node->token_type == E_Ans_Ast_Token_Type_Func_Decl
		) {
		return src_ast_node;
	}
	else {
		/*decl is	 only left.*/
		Ans_Ast_Nodes* ret_node = change_to_module_hash_process_helper(src_ast_node->left);
		if (!ret_node)ret_node = change_to_module_hash_process_helper(src_ast_node->right);
		return ret_node;
	}
}