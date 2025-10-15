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
		else if (ans_token_list_expect_token_str(src_token_list, "extend")) {

		}
		else if (ans_token_list_expect_token_str(src_token_list, "module_base")) {
			if (src_hash_process_list->file_modularization_process & HASH_PROCESS_STANDARD_ID)src_hash_process_list->file_modularization_process = HASH_PROCESS_MODULE_BASE_ID;
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
		if (tmp_result_module)*src_ast_node = tmp_result_module;
	}
	else if (src_hash_process_list->file_modularization_process & HASH_PROCESS_CLASS_ID) {
		Ans_Ast_Nodes* tmp_result_module = change_to_class_hash_process(*src_ast_node, src_hash_process_list->abstract_host_name);
		if (tmp_result_module)*src_ast_node = tmp_result_module;
	}

	else if (src_hash_process_list->file_modularization_process & HASH_PROCESS_MODULE_BASE_ID) {
		Ans_Ast_Nodes* tmp_result_module = change_to_module_base_hash_process(*src_ast_node, src_hash_process_list->abstract_host_name);
		*src_ast_node = tmp_result_module;
	}
}


Ans_Ast_Nodes* get_to_none_value_of_class_field_node(Ans_Ast_Nodes* src_ans_node) {
	if (src_ans_node) {
		if (src_ans_node->token_type == E_Ans_Ast_Token_Type_None_Value) {
			if (src_ans_node->right) {
				switch (src_ans_node->right->token_type) {
				case E_Ans_Ast_Token_Type_Class_Field_Public:
				case E_Ans_Ast_Token_Type_Class_Field_Private:
				case E_Ans_Ast_Token_Type_Class_Field_Protected:
					return src_ans_node;
				default:
					break;
				}
			}
		}
	}
	return NULL;
}

Ans_Ast_Nodes* get_to_block_node(Ans_Ast_Nodes* src_ans_node) {
	if (src_ans_node->token_type == E_Ans_Ast_Token_Type_Block) return src_ans_node;
	return NULL;
}


Ans_Ast_Nodes* change_to_class_hash_process(Ans_Ast_Nodes* src_ast_node, const char* class_name) {

	Ans_Class_Hash_Nodes ans_class_hash_nodes = {
		.private_decl_node = NULL,
		.tmp_private_decl_node = NULL,
		.protected_decl_node = NULL,
		.tmp_protected_decl_node = NULL,
		.public_decl_node = NULL,
		.tmp_public_decl_node = NULL,
		.toplevel_statement_node = NULL,
		.toplevel_statement_node = NULL
	};

	if (src_ast_node->token_type == E_Ans_Ast_Token_Type_Abstract_Host) {
		for (Ans_Ast_Nodes* abs_ans = src_ast_node, * tmp_ast_nodes = src_ast_node; abs_ans; abs_ans = abs_ans->left) {
			
			Ans_Ast_Nodes* ans_right = NULL;

			if ((ans_right = get_to_none_value_of_class_field_node(abs_ans->right))) {

				ans_right = ans_right->right;
				abs_ans->right->right = NULL;

				while (ans_right) {
					Ans_Ast_Nodes* next_ans_left = ans_right->left;
					if (ans_right->token_type == E_Ans_Ast_Token_Type_Class_Field_Public) {
						if (!ans_class_hash_nodes.public_decl_node)ans_class_hash_nodes.public_decl_node = ans_class_hash_nodes.tmp_public_decl_node = ans_right;
						else {
							ans_class_hash_nodes.public_decl_node->left = ans_right;
							ans_class_hash_nodes.public_decl_node = ans_right;
						}
					}
					else if (ans_right->token_type == E_Ans_Ast_Token_Type_Class_Field_Private) {
						if (!ans_class_hash_nodes.private_decl_node)ans_class_hash_nodes.private_decl_node = ans_class_hash_nodes.tmp_private_decl_node = ans_right;
						else {
							ans_class_hash_nodes.private_decl_node->left = ans_right;
							ans_class_hash_nodes.private_decl_node = ans_right;
						}
					}
					else if (ans_right->token_type == E_Ans_Ast_Token_Type_Class_Field_Protected) {
						if (!ans_class_hash_nodes.protected_decl_node)ans_class_hash_nodes.protected_decl_node = ans_class_hash_nodes.tmp_protected_decl_node = ans_right;
						else {
							ans_class_hash_nodes.protected_decl_node->left = ans_right;
							ans_class_hash_nodes.protected_decl_node = ans_right;
						}
					}
					else assert(false);
					ans_right->left = NULL;
					ans_right = next_ans_left;
				}
			}

			else if ((ans_right = get_to_block_node(abs_ans->right))) {
				
				change_to_class_hash_process_block(ans_right, &ans_class_hash_nodes);
			}
			else if ((ans_right = change_to_module_hash_process_helper(abs_ans->right))) {
				Ans_Ast_Nodes* new_right = set_up_ans_ast(NULL, abs_ans->right, E_Ans_Ast_Token_Type_Class_Field_Public, NULL, 0);
				if (!new_right)break;
				if (!ans_class_hash_nodes.public_decl_node)ans_class_hash_nodes.public_decl_node = ans_class_hash_nodes.tmp_public_decl_node = new_right;
				else {
					ans_class_hash_nodes.public_decl_node->left = new_right;
					ans_class_hash_nodes.public_decl_node = new_right;
				}
				abs_ans->right = NULL;
			}
			else {
				Ans_Ast_Nodes* new_right = set_up_ans_ast(NULL, abs_ans->right, E_Ans_Ast_Token_Type_Abstract_Host, NULL, 0);
				if (!new_right)break;

				if (!ans_class_hash_nodes.toplevel_statement_node)ans_class_hash_nodes.toplevel_statement_node = ans_class_hash_nodes.tmp_toplevel_statement_node = new_right;
				else {
					ans_class_hash_nodes.toplevel_statement_node->left = new_right;
					ans_class_hash_nodes.toplevel_statement_node = new_right;
				}
				abs_ans->right = NULL;

			}
		}

	}


	Ans_Ast_Nodes* final_field_node = NULL;
	Ans_Ast_Nodes* tmp_final_node = NULL;
	Ans_Ast_Nodes* toplevel_func_decl = set_up_ans_ast(NULL, ans_class_hash_nodes.tmp_toplevel_statement_node, E_Ans_Ast_Token_Type_Func_Decl, "mod", ext_strlen_add_null("mod"));
	
	if (!toplevel_func_decl) {
		delete_ans_ast_node(ans_class_hash_nodes.tmp_private_decl_node);
		delete_ans_ast_node(ans_class_hash_nodes.tmp_public_decl_node);
		delete_ans_ast_node(ans_class_hash_nodes.tmp_protected_decl_node);
		delete_ans_ast_node(ans_class_hash_nodes.tmp_toplevel_statement_node);
		return NULL;
	}
	/*mod filed decl*/
	Ans_Ast_Nodes* mod_filed_decl = set_up_ans_ast(NULL, toplevel_func_decl, E_Ans_Ast_Token_Type_Class_Field_Public, NULL, 0);
	if (!mod_filed_decl) {

		delete_ans_ast_node(ans_class_hash_nodes.tmp_private_decl_node);
		delete_ans_ast_node(ans_class_hash_nodes.tmp_public_decl_node);
		delete_ans_ast_node(ans_class_hash_nodes.tmp_protected_decl_node);
		delete_ans_ast_node(ans_class_hash_nodes.tmp_toplevel_statement_node);
		free(toplevel_func_decl->op);

		free(toplevel_func_decl);
		return NULL;
	}
	mod_filed_decl->left = ans_class_hash_nodes.tmp_public_decl_node;
	ans_class_hash_nodes.tmp_public_decl_node = mod_filed_decl;
	if (ans_class_hash_nodes.tmp_public_decl_node) {
		build_field_chain(&final_field_node, &tmp_final_node, ans_class_hash_nodes.public_decl_node, ans_class_hash_nodes.tmp_public_decl_node);
	}

	if (ans_class_hash_nodes.tmp_private_decl_node) {
		build_fields_chain(&final_field_node, &tmp_final_node, ans_class_hash_nodes.private_decl_node, ans_class_hash_nodes.tmp_private_decl_node);
	}
	if (ans_class_hash_nodes.tmp_protected_decl_node) {
		build_fields_chain(&final_field_node, &tmp_final_node, ans_class_hash_nodes.protected_decl_node, ans_class_hash_nodes.tmp_protected_decl_node);
	}

	Ans_Ast_Nodes* class_desc_node = set_up_ans_ast(NULL, tmp_final_node, E_Ans_Ast_Token_Type_Class_Decl, class_name, ext_strlen_add_null(class_name));
	if (!class_desc_node)return NULL;
	Ans_Ast_Nodes* decision_class_abstract_node = set_up_ans_ast(NULL, class_desc_node, E_Ans_Ast_Token_Type_Abstract_Host, NULL, 0);
	if (!decision_class_abstract_node) {

		delete_ans_ast_node(ans_class_hash_nodes.tmp_private_decl_node);
		delete_ans_ast_node(ans_class_hash_nodes.tmp_public_decl_node);
		delete_ans_ast_node(ans_class_hash_nodes.tmp_protected_decl_node);
		delete_ans_ast_node(ans_class_hash_nodes.tmp_toplevel_statement_node);
		free(class_desc_node->op);
		free(class_desc_node);
		return NULL;
	}

	delete_ans_ast_node(src_ast_node);
	return decision_class_abstract_node;
}
void change_to_class_hash_process_block(Ans_Ast_Nodes* src_ast_node, Ans_Class_Hash_Nodes* src_ans_class_hash_node) {
	Ans_Ast_Nodes* public_abs_ans_node = NULL;
	if (src_ast_node->token_type == E_Ans_Ast_Token_Type_Abstract_Host || src_ast_node->token_type == E_Ans_Ast_Token_Type_Seq_Abstract_Host || src_ast_node->token_type == E_Ans_Ast_Token_Type_None_Host) {
		for (Ans_Ast_Nodes* abs_ans = src_ast_node; abs_ans; abs_ans = abs_ans->left) {
			Ans_Ast_Nodes* ans_right;
			if ((ans_right = get_to_none_value_of_class_field_node(abs_ans->right))) {

				ans_right = ans_right->right;
				abs_ans->right->right = NULL;
				while (ans_right) {
					Ans_Ast_Nodes* next_ans_left = ans_right->left;
					if (ans_right->token_type == E_Ans_Ast_Token_Type_Class_Field_Public) {
						if (!src_ans_class_hash_node->public_decl_node)src_ans_class_hash_node->public_decl_node = src_ans_class_hash_node->tmp_public_decl_node = ans_right;
						else {
							src_ans_class_hash_node->public_decl_node->left = ans_right;
							src_ans_class_hash_node->public_decl_node = ans_right;
						}
					}
					else if (ans_right->token_type == E_Ans_Ast_Token_Type_Class_Field_Private) {
						if (!src_ans_class_hash_node->private_decl_node)src_ans_class_hash_node->private_decl_node = src_ans_class_hash_node->tmp_private_decl_node = ans_right;
						else {
							src_ans_class_hash_node->private_decl_node->left = ans_right;
							src_ans_class_hash_node->private_decl_node = ans_right;
						}
					}
					else  if (ans_right->token_type == E_Ans_Ast_Token_Type_Class_Field_Protected) {
						if (!src_ans_class_hash_node->protected_decl_node)src_ans_class_hash_node->protected_decl_node = src_ans_class_hash_node->tmp_protected_decl_node = ans_right;
						else {
							src_ans_class_hash_node->protected_decl_node->left = ans_right;
							src_ans_class_hash_node->protected_decl_node = ans_right;
						}
					}
					else assert(false);
					ans_right->left = NULL;
					ans_right = next_ans_left;
				}

			}
			else if ((ans_right = get_to_block_node(abs_ans->right))) {
				change_to_class_hash_process_block(ans_right, src_ans_class_hash_node);
			}
			else if ((ans_right = change_to_module_hash_process_helper(abs_ans->right))) {
				Ans_Ast_Nodes* new_right = set_up_ans_ast(NULL, abs_ans->right, E_Ans_Ast_Token_Type_Class_Field_Public, NULL, 0);
				if (!new_right)break;
				if (!src_ans_class_hash_node->public_decl_node)src_ans_class_hash_node->public_decl_node = src_ans_class_hash_node->tmp_public_decl_node = new_right;
				else {
					src_ans_class_hash_node->public_decl_node->left = new_right;
					src_ans_class_hash_node->public_decl_node = new_right;
				}
				abs_ans->right = NULL;
			}
			else {
				Ans_Ast_Nodes* new_right = set_up_ans_ast(NULL, abs_ans->right, E_Ans_Ast_Token_Type_Abstract_Host, NULL, 0);
				if (!new_right)break;
				if (!src_ans_class_hash_node->toplevel_statement_node)src_ans_class_hash_node->toplevel_statement_node = src_ans_class_hash_node->tmp_toplevel_statement_node = new_right;
				else {
					src_ans_class_hash_node->toplevel_statement_node->left = new_right;
					src_ans_class_hash_node->toplevel_statement_node = new_right;
				}
				abs_ans->right = NULL;

			}
		}
	}
}
Ans_Ast_Nodes* change_to_module_base_hash_process(Ans_Ast_Nodes* src_ast_node, const char* module_base_name) {

	if (!src_ast_node)return NULL;
	Ans_Ast_Nodes* toplevel_decl_node = NULL;
	Ans_Ast_Nodes* tmp_toplevel_decl_node = NULL;
	Ans_Ast_Nodes* toplevel_statement_node = NULL;
	Ans_Ast_Nodes* tmp_toplevel_statement_node = NULL;
	if (src_ast_node->token_type == E_Ans_Ast_Token_Type_Abstract_Host) {
		for (Ans_Ast_Nodes* abs_ans = src_ast_node; abs_ans; abs_ans = abs_ans->left) {
			Ans_Ast_Nodes* decl_node = change_to_module_hash_process_helper(abs_ans->right);
			if (decl_node) {
				/*decl all public*/
				if (decl_node->token_type == E_Ans_Ast_Token_Type_Var_Decl) {
					decl_node->token_type = E_Ans_Ast_Token_Type_Iden;
					Ans_Ast_Nodes* new_decl_node = set_up_ans_ast(NULL, NULL, E_Ans_Ast_Token_Type_Var_Decl, decl_node->op, ext_strlen_add_null(decl_node->op));
					if (!new_decl_node)break;
					Ans_Ast_Nodes* new_decl_expr_node = set_up_ans_ast(new_decl_node, NULL, E_Ans_Ast_Token_Type_Expr_Statement, decl_node->op, ext_strlen_add_null(decl_node->op));
					if (!new_decl_expr_node) {
						delete_ans_ast_node(new_decl_node);
						break;
					}
					Ans_Ast_Nodes* new_ah_decl_node = set_up_ans_ast(NULL, new_decl_expr_node, E_Ans_Ast_Token_Type_Abstract_Host, decl_node->op, ext_strlen_add_null(decl_node->op));
					if (!new_ah_decl_node) {
						delete_ans_ast_node(new_decl_expr_node);
						break;
					}

					if (!toplevel_statement_node)toplevel_statement_node = tmp_toplevel_statement_node = abs_ans;
					else {
						toplevel_statement_node->left = abs_ans;
						toplevel_statement_node = abs_ans;
					}

					if (!toplevel_decl_node)tmp_toplevel_decl_node = toplevel_decl_node = new_ah_decl_node;
					else {
						toplevel_decl_node->left = new_ah_decl_node;
						toplevel_decl_node = new_ah_decl_node;
					}
				}
				else {

					if (!toplevel_decl_node)tmp_toplevel_decl_node = toplevel_decl_node = abs_ans;
					else {
						toplevel_decl_node->left = abs_ans;
						toplevel_decl_node = abs_ans;
					}
				}
			}
			else {
				if (!toplevel_statement_node)toplevel_statement_node = tmp_toplevel_statement_node = abs_ans;
				else {
					toplevel_statement_node->left = abs_ans;
					toplevel_statement_node = abs_ans;
				}
			}
		}
	}

	if (toplevel_decl_node && toplevel_decl_node->left)toplevel_decl_node->left = NULL;
	if (toplevel_statement_node && toplevel_statement_node->left) toplevel_statement_node->left = NULL;
	/*toplevel function*/
	Ans_Ast_Nodes* toplevel_func_decl = set_up_ans_ast(NULL, tmp_toplevel_statement_node, E_Ans_Ast_Token_Type_Func_Decl, module_base_name, ext_strlen_add_null(module_base_name));
	if (!toplevel_func_decl) {
		delete_ans_ast_node(tmp_toplevel_decl_node);
		delete_ans_ast_node(tmp_toplevel_statement_node);
		return NULL;
	}
	/*mod filed decl*/
	Ans_Ast_Nodes* decision_module_absract_node = set_up_ans_ast(tmp_toplevel_decl_node, toplevel_func_decl, E_Ans_Ast_Token_Type_Abstract_Host, NULL, 0);
	if (!decision_module_absract_node) {

		delete_ans_ast_node(toplevel_func_decl);
	}
	return decision_module_absract_node;
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
				set_up_ans_ast_type(abs_ans, E_Ans_Ast_Token_Type_Class_Field_Public);

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
	Ans_Ast_Nodes* mod_filed_decl = set_up_ans_ast(NULL, toplevel_func_decl, E_Ans_Ast_Token_Type_Class_Field_Public, NULL, 0);
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
	/*look for the decl node in the ast*/
	if (!src_ast_node)return NULL;
	if (
		src_ast_node->token_type == E_Ans_Ast_Token_Type_Var_Decl ||
		src_ast_node->token_type == E_Ans_Ast_Token_Type_Func_Decl ||
		src_ast_node->token_type == E_Ans_Ast_Token_Type_Class_Decl

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