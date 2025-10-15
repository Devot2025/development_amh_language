#include "parser_main_ans.h"
#include <math.h>
#include <float.h>
void build_ans_ast_field_node(Ans_Ast_Nodes** src_tmp_field_node, Ans_Ast_Nodes** src_field_node, Ans_Ast_Token_Type src_field_type, Ans_Lex_Token_List* src_ans_token_list) {
	Ans_Ast_Nodes* filed_decl_node = set_up_ans_ast(NULL, build_ans_ast_class_field_decl(src_ans_token_list), src_field_type, NULL, 0);
	if (!*src_field_node)*src_tmp_field_node = *src_field_node = filed_decl_node;
	else {
		(*src_field_node)->left = filed_decl_node;
		*src_field_node = filed_decl_node;
	}
}
Ans_Field_Type_Cache* gen_and_chain_ans_field_cache(Ans_Field_Type_Cache * src_field_cache, Ans_Ast_Token_Type src_field_type) {
	Ans_Field_Type_Cache* new_field_type_cache = smart_calloc(Ans_Field_Type_Cache, 1);
	if (!new_field_type_cache)return src_field_cache;
	new_field_type_cache->field_type = src_field_type;
	new_field_type_cache->next_field = src_field_cache;
	return new_field_type_cache;
}

bool check_to_only_expr_keyword(Ans_Ast_Nodes* src_ast_node) {
	if (!src_ast_node)return false;
	if (src_ast_node->token_type == E_Ans_Ast_Token_Type_Loop) return true;
	return false;
}
void decision_final_accurate_binary_type(Ans_Ast_Nodes* src_ans_ast_node, const char* src_str) {
	const char* tmp_src_str = src_str;
	bool float_check = false;
	while (*src_str) if (*(src_str++) == '.') { float_check = true; break; }
	src_str = tmp_src_str;
	typedef union tmp_value_datas {
		float float_data;
		double double_data;
		int int_data;
	}tmp_value_datas;
	tmp_value_datas tmp_val_data;

	if (float_check) {
		tmp_val_data.double_data = strtod(src_str, NULL);
		if (isfinite(tmp_val_data.double_data) && fabs(tmp_val_data.double_data) <= FLT_MAX) {
			/*float check*/
			tmp_val_data.float_data = (float)tmp_val_data.double_data;
			set_up_ast_node_op_datas(src_ans_ast_node, E_Ans_Ast_Token_Type_Float_Value, &tmp_val_data.float_data, sizeof(float));
		}
		else {
			/*double check*/
			set_up_ast_node_op_datas(src_ans_ast_node, E_Ans_Ast_Token_Type_Double_Value, &tmp_val_data.double_data, sizeof(double));
		}
	}
	else {
		/* change to python-style infinite int*/
		tmp_val_data.int_data = (int)strtol(src_str, NULL, 0);
		set_up_ast_node_op_datas(src_ans_ast_node, E_Ans_Ast_Token_Type_Int_Value, &tmp_val_data.int_data, sizeof(int));
	}
}

void decision_final_accurate_specail_value(Ans_Ast_Nodes* src_node, const char* src_now_token_str) {
	if (simple_strcmp(src_now_token_str, "none")) set_up_ast_node_op_datas(src_node, E_Ans_Ast_Token_Type_None_Value, NULL, 0);
	else if (simple_strcmp(src_now_token_str, "_______none")) set_up_ast_node_op_datas(src_node, E_Ans_Ast_Token_Type_None_Value, NULL, 0);
	else if (simple_strcmp(src_now_token_str, "v_em")) set_up_ast_node_op_datas(src_node, E_Ans_Ast_Token_Type_Virtual_Empty_Value, NULL, 0);
	else set_up_ast_node_op_datas(src_node, E_Ans_Ast_Token_Type_None_Value, "error : not exit specail value", ext_strlen_add_null("error : not exit specail value"));
}
Ans_Ast_Token_Type get_match_ans_lex_ast_multi_type(const Ans_Ast_Token_Type* dst_token_type, const char** src_match_lex_str_list, Ans_Lex_Token_Type src_ast_lex_token_type,Ans_Lex_Token_List* src_ans_token_list, uint32_t src_match_list_size) {
	for (uint32_t i = 0; i < src_match_list_size; ++i) if (ans_token_list_expect_token_data(src_ans_token_list, src_ast_lex_token_type,*(src_match_lex_str_list + i)))return *(dst_token_type + i);
	return E_Ans_Ast_Token_Type_Null;
}
Ans_Ast_Token_Type expect_add_sub_type(Ans_Lex_Token_List* src_ans_token_list) {
	const Ans_Ast_Token_Type dst_add_sub_type_list[] = { E_Ans_Ast_Token_Type_Add, E_Ans_Ast_Token_Type_Sub };
	const char* dst_add_sub_str_list[] = { "+", "-" };

	return get_match_ans_lex_ast_multi_type(dst_add_sub_type_list, dst_add_sub_str_list, E_Ans_Lex_Token_Type_Operator, src_ans_token_list, GET_ARRAY_SIZE(dst_add_sub_str_list));
}

Ans_Ast_Token_Type expect_mul_div_mod_type(Ans_Lex_Token_List* src_ans_token_list) {
	const Ans_Ast_Token_Type dst_add_sub_type_list[] = { E_Ans_Ast_Token_Type_Mul, E_Ans_Ast_Token_Type_Div, E_Ans_Ast_Token_Type_Mod };
	const char* dst_add_sub_str_list[] = { "*", "/", "%" };
	return get_match_ans_lex_ast_multi_type(dst_add_sub_type_list, dst_add_sub_str_list, E_Ans_Lex_Token_Type_Operator, src_ans_token_list, GET_ARRAY_SIZE(dst_add_sub_str_list));
}

Ans_Ast_Token_Type expect_assigment_type(Ans_Lex_Token_List* src_ans_token_list) {
	const Ans_Ast_Token_Type dst_add_sub_type_list[] = { E_Ans_Ast_Token_Type_Assigment,E_Ans_Ast_Token_Type_Add_Assigment, E_Ans_Ast_Token_Type_Sub_Assigment, E_Ans_Ast_Token_Type_Mul, E_Ans_Ast_Token_Type_Div_Assigment, E_Ans_Ast_Token_Type_Mod_Assigment };
	const char* dst_add_sub_str_list[] = { "=" ,"+=", "-=", "/=", "*=", "%=" };
	return get_match_ans_lex_ast_multi_type(dst_add_sub_type_list, dst_add_sub_str_list, E_Ans_Lex_Token_Type_Operator, src_ans_token_list, GET_ARRAY_SIZE(dst_add_sub_str_list));
}

Ans_Ast_Token_Type expect_equals_type(Ans_Lex_Token_List* src_ans_token_list) {
	const Ans_Ast_Token_Type dst_add_sub_type_list[] = { E_Ans_Ast_Token_Type_Assigment,E_Ans_Ast_Token_Type_Add_Assigment, E_Ans_Ast_Token_Type_Sub_Assigment, E_Ans_Ast_Token_Type_Mul, E_Ans_Ast_Token_Type_Div_Assigment, E_Ans_Ast_Token_Type_Mod_Assigment };
	const char* dst_add_sub_str_list[] = { "==" ,"!=" };
	return get_match_ans_lex_ast_multi_type(dst_add_sub_type_list, dst_add_sub_str_list, E_Ans_Lex_Token_Type_Operator, src_ans_token_list, GET_ARRAY_SIZE(dst_add_sub_str_list));
}

Ans_Ast_Token_Type expect_relation_type(Ans_Lex_Token_List* src_ans_token_list) {
	const Ans_Ast_Token_Type dst_add_sub_type_list[] = { E_Ans_Ast_Token_Type_Less, E_Ans_Ast_Token_Type_Greater, E_Ans_Ast_Token_Type_Less_Equal, E_Ans_Ast_Token_Type_Greater_Equal };
	const char* dst_add_sub_str_list[] = { "<" ,">", "<=", ">=" };
	return get_match_ans_lex_ast_multi_type(dst_add_sub_type_list, dst_add_sub_str_list, E_Ans_Lex_Token_Type_Operator, src_ans_token_list, GET_ARRAY_SIZE(dst_add_sub_str_list));
}
Ans_Ast_Token_Type expect_shift_type(Ans_Lex_Token_List* src_ans_token_list) {
	const Ans_Ast_Token_Type dst_add_sub_type_list[] = { E_Ans_Ast_Token_Type_Left_Shift, E_Ans_Ast_Token_Type_Right_Shift };
	const char* dst_add_sub_str_list[] = { "<<" ,">>" };
	return get_match_ans_lex_ast_multi_type(dst_add_sub_type_list, dst_add_sub_str_list, E_Ans_Lex_Token_Type_Operator, src_ans_token_list, GET_ARRAY_SIZE(dst_add_sub_str_list));
}
Ans_Ast_Token_Type expect_back_ope_type(Ans_Lex_Token_List* src_ans_token_list) {
	const Ans_Ast_Token_Type dst_add_sub_type_list[] = { E_Ans_Ast_Token_Type_Men_Acc, E_Ans_Ast_Token_Type_Spe_Men_Acc, E_Ans_Ast_Token_Type_Iden_Changer };
	const char* dst_add_sub_str_list[] = { ".", ":", "::" };
	return get_match_ans_lex_ast_multi_type(dst_add_sub_type_list, dst_add_sub_str_list, E_Ans_Lex_Token_Type_Operator, src_ans_token_list, GET_ARRAY_SIZE(dst_add_sub_str_list));
}
Ans_Ast_Nodes* start_parser_ans_ast_main(Ans_Lex_Token_List* src_ans_token_list, Ans_Hash_Process_List* src_hash_list) {
	if (!src_ans_token_list || !src_hash_list)return NULL;
	return build_ans_ast_abstract_node(src_ans_token_list, src_hash_list);
}
Ans_Ast_Nodes* build_ans_ast_abstract_node(Ans_Lex_Token_List* src_ans_token_list, Ans_Hash_Process_List* src_hash_list) {
	Ans_Ast_Nodes* ans_block = NULL;
	Ans_Ast_Nodes* tmp_ans_block = NULL;
	src_ans_token_list->stack_token_list_index = 0;
	while (src_ans_token_list->stack_token_list_index < src_ans_token_list->stack_token_list_size) {
		if (!standard_hash_process_ans(src_hash_list, src_ans_token_list)) {
			continue;
		}
		Ans_Ast_Nodes* ans_left = smart_calloc(Ans_Ast_Nodes, 1);
		if (!ans_left)break;

		Ans_Ast_Nodes* ans_right = build_ans_ast_statement(src_ans_token_list);

		ans_left->right = ans_right;
		set_up_ast_node_op_datas(ans_left, E_Ans_Ast_Token_Type_Abstract_Host, NULL, 0);
		if (!ans_block) tmp_ans_block = ans_block = ans_left;
		else {
			ans_block->left = ans_left;
			ans_block = ans_left;
		}
	}
	return tmp_ans_block;
}
Ans_Ast_Nodes* build_ans_ast_statement(Ans_Lex_Token_List* src_ans_token_list) {
	Ans_Ast_Nodes* ans_left;
	if ((ans_left = build_ans_ast_block(src_ans_token_list)))return ans_left;
	if (ans_token_list_expect_token_str(src_ans_token_list, ";")) return build_ans_ast_null_expr_statement();
	if ((ans_left = build_ans_ast_keyword(src_ans_token_list)))return ans_left;
	if ((ans_left = build_ans_ast_toplevel_field(src_ans_token_list)))return ans_left;
	ans_left = build_ans_ast_expr_comma(src_ans_token_list);
	if (check_to_only_expr_keyword(ans_left)) return build_ans_ast_keyword_expr_statement(src_ans_token_list, ans_left);
	return build_ans_ast_expr_statement(src_ans_token_list, ans_left);
}
Ans_Ast_Nodes* build_ans_ast_keyword_expr_statement(Ans_Lex_Token_List* src_ans_token_list, Ans_Ast_Nodes * src_ans_node) {
	if (ans_token_list_expect_token_data(src_ans_token_list, E_Ans_Lex_Token_Type_Punchcute, ";")) {
		Ans_Ast_Nodes* ans_left;
		ans_left = set_up_ans_ast(src_ans_node, NULL, E_Ans_Ast_Token_Type_Expr_Statement, NULL, 0);
		if (ans_left)src_ans_node = ans_left;
	}
	return src_ans_node;
}
Ans_Ast_Nodes* build_ans_ast_null_expr_statement() {
	Ans_Ast_Nodes* ans_left;

	ans_left = smart_calloc(Ans_Ast_Nodes, 1);

	if (!ans_left)return NULL;
	set_up_ast_node_op_datas(ans_left, E_Ans_Ast_Token_Type_Expr_Statement, NULL, 0);
	return ans_left;
}
Ans_Ast_Nodes* build_ans_ast_keyword(Ans_Lex_Token_List* src_ans_token_list) {
	if (ans_token_list_expect_token_data(src_ans_token_list, E_Ans_Lex_Token_Type_Keyword, "obj")) {
		return build_ans_ast_iden_decl(src_ans_token_list);
	}
	else if (ans_token_list_expect_token_data(src_ans_token_list, E_Ans_Lex_Token_Type_Keyword,"if")) {
		return build_ans_ast_if_else(src_ans_token_list);
	}
	else if (ans_token_list_expect_token_data(src_ans_token_list, E_Ans_Lex_Token_Type_Keyword, "return")) {

		return build_ans_ast_host_return_and_breaks(src_ans_token_list, E_Ans_Ast_Token_Type_Host_Return);
	}

	else if(ans_token_list_expect_token_data(src_ans_token_list, E_Ans_Lex_Token_Type_Keyword, "break")) {
		return build_ans_ast_host_return_and_breaks(src_ans_token_list, E_Ans_Ast_Token_Type_Seq_Host_Break);
	}
	else if (ans_token_list_expect_token_data(src_ans_token_list, E_Ans_Lex_Token_Type_Keyword, "break_a")) {
		return build_ans_ast_host_return_and_breaks(src_ans_token_list, E_Ans_Ast_Token_Type_Seq_All_Host_Break);
	}
	else if (ans_token_list_expect_token_data(src_ans_token_list, E_Ans_Lex_Token_Type_Keyword, "cls")) {
		return build_ans_ast_class(src_ans_token_list);
	}

	return NULL;
}
Ans_Ast_Nodes* build_ans_ast_toplevel_field(Ans_Lex_Token_List* src_ans_token_list) {
	Ans_Ast_Nodes* field_node = NULL;
	if (ans_token_list_expect_token_str(src_ans_token_list, "pbl")) {
		if (ans_token_list_expect_token_str(src_ans_token_list, "{")) {
			field_node = build_ans_ast_class_field(src_ans_token_list, E_Ans_Ast_Token_Type_Class_Field_Public, "error : expect to after public field");
		}
		else {
			Ans_Ast_Nodes* tmp_field_node = build_ans_ast_class_field_decl(src_ans_token_list);
			Ans_Ast_Nodes* field_node = set_up_ans_ast(NULL, NULL, E_Ans_Ast_Token_Type_Class_Field_Public, NULL, 0);
			if (!field_node) return tmp_field_node;
		}
	}
	else if (ans_token_list_expect_token_str(src_ans_token_list, "+{")) {
		field_node = build_ans_ast_class_field(src_ans_token_list, E_Ans_Ast_Token_Type_Class_Field_Public, "error : expect to after public field");
	}
	else if (ans_token_list_expect_token_str(src_ans_token_list, "prv")) {
		if (ans_token_list_expect_token_str(src_ans_token_list, "{")) {
			field_node = build_ans_ast_class_field(src_ans_token_list, E_Ans_Ast_Token_Type_Class_Field_Private, "error : expect to after protected");
		}
		else {
			Ans_Ast_Nodes* tmp_field_node = build_ans_ast_class_field_decl(src_ans_token_list);
			Ans_Ast_Nodes* field_node = set_up_ans_ast(NULL, NULL, E_Ans_Ast_Token_Type_Class_Field_Private, NULL, 0);
			if (!field_node) return tmp_field_node;
		}
	}

	else if (ans_token_list_expect_token_str(src_ans_token_list, "-{")) {
		field_node = build_ans_ast_class_field(src_ans_token_list, E_Ans_Ast_Token_Type_Class_Field_Private, "error : expect to after protected");
	}
	else if (ans_token_list_expect_token_str(src_ans_token_list, "prt")) {

		if (ans_token_list_expect_token_str(src_ans_token_list, "{")) {
			field_node = build_ans_ast_class_field(src_ans_token_list, E_Ans_Ast_Token_Type_Class_Field_Protected, "error : expect to after protected");
		}
		else {
			Ans_Ast_Nodes* tmp_field_node = build_ans_ast_class_field_decl(src_ans_token_list);
			Ans_Ast_Nodes* field_node = set_up_ans_ast(NULL, NULL, E_Ans_Ast_Token_Type_Class_Field_Protected, NULL, 0);
			if (!field_node) return tmp_field_node;
		}
	}
	else if (ans_token_list_expect_token_str(src_ans_token_list, "%{")) {
		field_node = build_ans_ast_class_field(src_ans_token_list, E_Ans_Ast_Token_Type_Class_Field_Protected, "error : expect to after protected");
	}
	else return NULL;

	Ans_Ast_Nodes* ans_left = set_up_ans_ast(NULL, field_node, E_Ans_Ast_Token_Type_None_Value, "error : expect to class hash process", ext_strlen_add_null("error : expect to class hash process"));
	if (!ans_left)return field_node;
	return ans_left;
}
Ans_Ast_Nodes* build_ans_ast_block(Ans_Lex_Token_List* src_ans_token_list) {
	if (ans_token_list_expect_token_data(src_ans_token_list, E_Ans_Lex_Token_Type_Punchcute,"{")) {
		int end_check;
		Ans_Ast_Nodes* ans_right = build_ans_ast_block_process(&end_check, src_ans_token_list, E_Ans_Ast_Token_Type_None_Host);
		if (!ans_right)return ans_right;
		Ans_Ast_Nodes* ans_left = set_up_ans_ast(NULL, ans_right, E_Ans_Ast_Token_Type_Block, NULL, 0);
		if (!ans_left)return ans_right;
		if (end_check == 0) set_up_ast_node_op_datas(ans_left, E_Ans_Ast_Token_Type_None_Value, "error : extpect to '}' after block.", ext_strlen_add_null("error : extpect to '}' after block."));
		return ans_left;
	}
	return NULL;
}
int end_code_ans_ast_block(Ans_Lex_Token_List* src_ans_token_list) {
	if (ans_token_list_expect_token_data(src_ans_token_list, E_Ans_Lex_Token_Type_Punchcute,"}"))return -1;
	if (src_ans_token_list->stack_token_list_index >= src_ans_token_list->stack_token_list_size)return 0;
	return 1;
}
Ans_Ast_Nodes* build_ans_ast_block_process(int* end_code, Ans_Lex_Token_List* src_ans_token_list, Ans_Ast_Token_Type src_token_type) {
	Ans_Ast_Nodes* ans_block = NULL;
	Ans_Ast_Nodes* tmp_ans_block = NULL;
	while ((*end_code = end_code_ans_ast_block(src_ans_token_list)) > 0) {
		Ans_Ast_Nodes* ans_left = smart_calloc(Ans_Ast_Nodes, 1);
		if (!ans_left) { *end_code = 0; break; }
		Ans_Ast_Nodes* ans_right = build_ans_ast_statement(src_ans_token_list);

		ans_left->right = ans_right;
		set_up_ast_node_op_datas(ans_left, src_token_type, NULL, 0);
		if (!ans_block) {
			tmp_ans_block = ans_block = ans_left;
		}
		else {
			ans_block->left = ans_left;
			ans_block = ans_left;
		}
	}
	return tmp_ans_block;
}
Ans_Ast_Nodes* build_ans_ast_if_else(Ans_Lex_Token_List* src_ans_token_list) {
	Ans_Ast_Nodes* ans_right = set_up_ans_ast(NULL, NULL, E_Ans_Ast_Token_Type_If, NULL, 0);
	if (!ans_right)return ans_right;
	if (ans_token_list_expect_token_data(src_ans_token_list, E_Ans_Lex_Token_Type_Punchcute, "(")) {
		Ans_Ast_Nodes* ans_left = build_ans_ast_expr_comma(src_ans_token_list);
		if (!ans_left)return ans_right;
		ans_right->left = ans_left;
		if (ans_token_list_expect_token_data(src_ans_token_list, E_Ans_Lex_Token_Type_Punchcute, ")")) {
			if (ans_token_list_expect_token_data(src_ans_token_list, E_Ans_Lex_Token_Type_Punchcute, "{")) {
				int end_code = 0;
				Ans_Ast_Nodes* ans_if_statement = build_ans_ast_block_process(&end_code, src_ans_token_list, E_Ans_Ast_Token_Type_None_Host);
				ans_right->right = ans_if_statement;
				const char* if_err_str = "error : expect to '}' after if.";
				if (end_code == 0)
					set_up_ast_node_op_datas(ans_right, E_Ans_Ast_Token_Type_None_Value, if_err_str, ext_strlen_add_null(if_err_str));
				else ans_right = build_ans_ast_else_if(ans_right, src_ans_token_list);
			}
			else {
				Ans_Ast_Nodes* ans_if_statement = build_ans_ast_statement(src_ans_token_list);
				ans_right->right = ans_if_statement;
				ans_right = build_ans_ast_else_if(ans_right, src_ans_token_list);

				return ans_right;
			}
		}
		else{
			const char* if_err_str = "error : expect to ')' after if.";
			set_up_ast_node_op_datas(ans_right, E_Ans_Ast_Token_Type_None_Value, if_err_str, ext_strlen_add_null(if_err_str));
		}
	}
	else {
		const char* if_err_str = "error : expect to '(' after if.";
		set_up_ast_node_op_datas(ans_right, E_Ans_Ast_Token_Type_None_Value, if_err_str, ext_strlen_add_null(if_err_str));
	}
	return ans_right;
}
Ans_Ast_Nodes* build_ans_ast_else_if(Ans_Ast_Nodes* src_ans_node, Ans_Lex_Token_List * src_ans_token_list) {
	int else_end_check = 0;
	Ans_Ast_Nodes* ans_if_chain = set_up_ans_ast(NULL, src_ans_node, E_Ans_Ast_Token_Type_If_Else_Chain, NULL, 0);
	Ans_Ast_Nodes* tmp_ans_if_chain = ans_if_chain;
	if (!ans_if_chain)return src_ans_node;
	while (ans_token_list_expect_token_data(src_ans_token_list, E_Ans_Lex_Token_Type_Keyword, "else")) {
		Ans_Ast_Nodes* ans_else_if_chain = set_up_ans_ast(NULL, NULL, E_Ans_Ast_Token_Type_If_Else_Chain, NULL, 0);
		if (!ans_else_if_chain)break;
		if (ans_token_list_expect_token_data(src_ans_token_list, E_Ans_Lex_Token_Type_Keyword, "if")) {
			Ans_Ast_Nodes* ans_left = set_up_ans_ast(NULL, NULL, E_Ans_Ast_Token_Type_Else_If, NULL, 0);
			if (!ans_left) {

				ans_if_chain->left = ans_else_if_chain;
				ans_if_chain = ans_else_if_chain;
				break;
			}
			ans_else_if_chain->right = ans_left;
			if (ans_token_list_expect_token_data(src_ans_token_list, E_Ans_Lex_Token_Type_Punchcute, "(")) {
				ans_left->left = build_ans_ast_expr_comma(src_ans_token_list);
				if (ans_token_list_expect_token_data(src_ans_token_list, E_Ans_Lex_Token_Type_Punchcute, ")")) {

					if (ans_token_list_expect_token_data(src_ans_token_list, E_Ans_Lex_Token_Type_Punchcute, "{")) {
						int end_code = 0;
						Ans_Ast_Nodes* ans_if_statement = build_ans_ast_block_process(&end_code, src_ans_token_list, E_Ans_Ast_Token_Type_None_Host);
						ans_left->right = ans_if_statement;
						const char* if_err_str = "error : expect to '}' after else if.";
						if (end_code == 0)
							set_up_ast_node_op_datas(ans_left, E_Ans_Ast_Token_Type_None_Value, if_err_str, ext_strlen_add_null(if_err_str));
					}
					else{
						Ans_Ast_Nodes* ans_if_statement = build_ans_ast_statement(src_ans_token_list);
						ans_left->right = ans_if_statement;
					}
				}
				else {
					const char* err_else_if_str = "error : expect to ')' after else if.";
					set_up_ast_node_op_datas(ans_left, E_Ans_Ast_Token_Type_None_Value, err_else_if_str, ext_strlen_add_null(err_else_if_str));

				}

			}
			else {
				const char* err_else_if = "error : expect to '(' after else if.";
				set_up_ast_node_op_datas(ans_left, E_Ans_Ast_Token_Type_None_Value, err_else_if, ext_strlen_add_null(err_else_if));

			}

			ans_if_chain->left = ans_else_if_chain;
			ans_if_chain = ans_else_if_chain;

		}
		else {
			Ans_Ast_Nodes* ans_left = set_up_ans_ast(NULL, NULL, E_Ans_Ast_Token_Type_Else, NULL, 0);
			if (!ans_left) {

				ans_if_chain->left = ans_else_if_chain;
				ans_if_chain = ans_else_if_chain;
				break;
			}
			if (ans_token_list_expect_token_data(src_ans_token_list, E_Ans_Lex_Token_Type_Punchcute, "{")) {
				int end_code = 0;
				Ans_Ast_Nodes* ans_if_statement = build_ans_ast_block_process(&end_code, src_ans_token_list, E_Ans_Ast_Token_Type_None_Host);
				ans_left->right = ans_if_statement;
				const char* if_err_str = "error : expect to '}' after else if.";
				if (end_code == 0)
					set_up_ast_node_op_datas(ans_left, E_Ans_Ast_Token_Type_None_Value, if_err_str, ext_strlen_add_null(if_err_str));
			}
			else {
				Ans_Ast_Nodes* ans_if_statement = build_ans_ast_statement(src_ans_token_list);
				ans_left->right = ans_if_statement;
			}
			ans_else_if_chain->right = ans_left;


			ans_if_chain->left = ans_else_if_chain;
			ans_if_chain = ans_else_if_chain;
			break;
		}
	}
	return tmp_ans_if_chain;
}
Ans_Ast_Nodes* build_ans_ast_iden_decl(Ans_Lex_Token_List* src_ans_token_list) {
	Ans_Ast_Nodes* ans_left = set_up_ans_ast(NULL, NULL, E_Ans_Ast_Token_Type_Var_Decl, NULL, 0);
	if (ans_token_list_expect_token_data(src_ans_token_list, E_Ans_Lex_Token_Type_Punchcute,";")) {
		Ans_Ast_Nodes* ans_right = set_up_ans_ast(ans_left, NULL, E_Ans_Ast_Token_Type_Expr_Statement, NULL, 0);
		return ans_right ? ans_right : ans_left;
	}

	Ans_Lex_Token* now_token_list = peek_ans_lex_token(src_ans_token_list);
	const char* decl_err = "";
	if (now_token_list->ans_token_type != E_Ans_Lex_Token_Type_Iden) {
		decl_err = "error : expect to iden after obj.";
		set_up_ast_node_op_datas(ans_left, E_Ans_Ast_Token_Type_None_Value, decl_err, ext_strlen_add_null(decl_err));
		return ans_left;
	}
	else {
		set_up_ast_node_op_datas(ans_left, E_Ans_Ast_Token_Type_Var_Decl, now_token_list->ans_token_str, ext_strlen_add_null(now_token_list->ans_token_str));;
	}
	consume_ans_lex_token(src_ans_token_list);
	if (ans_token_list_expect_token_data(src_ans_token_list, E_Ans_Lex_Token_Type_Operator,"=")) {
		Ans_Ast_Nodes* ans_right = set_up_ans_ast(NULL, NULL, E_Ans_Ast_Token_Type_Assigment, NULL, 0);
		if (!ans_right)return ans_left;
		ans_right->left = ans_left;
		ans_right->right = build_ans_ast_expr_comma(src_ans_token_list);
		return build_ans_ast_expr_statement(src_ans_token_list, ans_right);
	}
	else if (ans_token_list_expect_token_data(src_ans_token_list, E_Ans_Lex_Token_Type_Punchcute,"(")) {
		ans_left->token_type = E_Ans_Ast_Token_Type_Func_Decl;
		Ans_Ast_Nodes* ans_args;
		int end_code = 0;
		if (ans_token_list_expect_token_data(src_ans_token_list, E_Ans_Lex_Token_Type_Punchcute,")")) {
			ans_args = set_up_ans_ast(NULL, NULL, E_Ans_Ast_Token_Type_Func_Decl_Args, NULL, 0);
			ans_left->left = ans_args;
		}
		else {
			ans_args = build_ans_ast_args_decl(&end_code, src_ans_token_list);

			decl_err = "error : expect to ')' after func def";
			ans_left->left = ans_args;

			if (!ans_token_list_expect_token_data(src_ans_token_list, E_Ans_Lex_Token_Type_Punchcute,")")) {
				set_up_ast_node_op_datas(ans_left, E_Ans_Ast_Token_Type_None_Value, decl_err, ext_strlen_add_null(decl_err));
				return ans_left;
			}
		}
		if (end_code == 0) {
			if (ans_token_list_expect_token_data(src_ans_token_list, E_Ans_Lex_Token_Type_Punchcute,"{")) {
				decl_err = "error : expect to '}' after func def";
				Ans_Ast_Nodes* block_node = build_ans_ast_block_process(&end_code, src_ans_token_list, E_Ans_Ast_Token_Type_Abstract_Host);
				if (end_code == 0) set_up_ast_node_op_datas(ans_left, E_Ans_Ast_Token_Type_None_Value, decl_err, ext_strlen_add_null(decl_err));
				if(!block_node) {
					block_node = set_up_ans_ast(NULL, NULL, E_Ans_Ast_Token_Type_Abstract_Host, NULL, 0);
				}
				ans_left->right = block_node;
			}
			else set_up_ast_node_op_datas(ans_left, E_Ans_Ast_Token_Type_None_Value, decl_err, ext_strlen_add_null(decl_err));
		}
		return ans_left;
	}
	else {
		return build_ans_ast_expr_statement(src_ans_token_list, ans_left);
	}
}
Ans_Ast_Nodes* build_ans_ast_args_decl(int* end_code, Ans_Lex_Token_List* src_ans_token_list) {
	*end_code = 0;
	Ans_Ast_Nodes* ans_args = NULL;
	Ans_Ast_Nodes* tmp_ans_args = NULL;
	const char* args_err = "error : expect to obj after iden.";
	do {
		Ans_Ast_Nodes* ans_left = set_up_ans_ast(NULL, NULL, E_Ans_Ast_Token_Type_Func_Decl_Args, NULL, 0);
		if (!ans_left) {
			*end_code = 1;
			break;
		}
		if (!ans_token_list_expect_token_data(src_ans_token_list, E_Ans_Lex_Token_Type_Keyword,"obj")) {
			set_up_ast_node_op_datas(ans_left, E_Ans_Ast_Token_Type_None_Value, args_err, ext_strlen_add_null(args_err));
			*end_code = 1;

			if (!ans_args)tmp_ans_args = ans_left;
			else ans_args->left = ans_left;
			break;
		}
		Ans_Lex_Token* now_token_type = peek_ans_lex_token(src_ans_token_list);
		if (!now_token_type) {
			set_up_ans_ast(ans_left, NULL, E_Ans_Ast_Token_Type_None_Value, args_err, ext_strlen_add_null(args_err));
			*end_code = 1;

			if (!ans_args)tmp_ans_args = ans_left;
			else ans_args->left = ans_left;
			break;
		}
		if (now_token_type->ans_token_type == E_Ans_Lex_Token_Type_Iden) {
			set_up_ast_node_op_datas(ans_left, E_Ans_Ast_Token_Type_Func_Decl_Args, now_token_type->ans_token_str, ext_strlen_add_null(now_token_type->ans_token_str));
			consume_ans_lex_token(src_ans_token_list);
			if (!ans_args)tmp_ans_args = ans_args = ans_left;
			else {
				ans_args->left = ans_left;
				ans_args = ans_left;
			}
		}
		else {
			set_up_ans_ast(ans_left, NULL, E_Ans_Ast_Token_Type_None_Value, args_err, ext_strlen_add_null(args_err));
			*end_code = 1;
			if (!ans_args)tmp_ans_args = ans_left;
			else ans_args->left = ans_left;
			break;
		}
	} while (ans_token_list_expect_token_data(src_ans_token_list, E_Ans_Lex_Token_Type_Operator,","));

	return tmp_ans_args;
}
Ans_Ast_Nodes* build_ans_ast_class(Ans_Lex_Token_List* src_ans_token_list) {
	Ans_Lex_Token* now_lex_token = peek_ans_lex_token(src_ans_token_list);
	Ans_Ast_Nodes* ans_left = set_up_ans_ast(NULL, NULL, E_Ans_Ast_Token_Type_Class_Decl, NULL, 0);
	const char* err_class_decl = "error : expect to iden after class decl.";
	if (now_lex_token->ans_token_type == E_Ans_Lex_Token_Type_Iden) {
		set_up_ans_ast_data(ans_left, now_lex_token->ans_token_str, ext_strlen_add_null(now_lex_token->ans_token_str));
		consume_ans_lex_token(src_ans_token_list);

		if (ans_token_list_expect_token_data(src_ans_token_list, E_Ans_Lex_Token_Type_Punchcute,"{")) {
			ans_left->right = build_ans_ast_class_field(src_ans_token_list, E_Ans_Ast_Token_Type_Class_Field_Public, "error : expect to '}' after class fields");
		}
		else {
			err_class_decl = "error : expect to '{' after class decl";
			set_up_ast_node_op_datas(ans_left, E_Ans_Ast_Token_Type_None_Value, err_class_decl, ext_strlen_add_null(err_class_decl));

		}
	}
	else {
		set_up_ast_node_op_datas(ans_left, E_Ans_Ast_Token_Type_None_Value, err_class_decl, ext_strlen_add_null(err_class_decl));
	}
	return ans_left;
}
Ans_Ast_Nodes* build_ans_ast_class_field(Ans_Lex_Token_List* src_ans_token_list, Ans_Ast_Token_Type src_null_field_type, const char * src_class_field_err) {
	int end_code = 0;
	Ans_Ast_Nodes* public_field_node = NULL;
	Ans_Ast_Nodes* tmp_public_field_node = NULL;
	Ans_Ast_Nodes* private_field_node = NULL;
	Ans_Ast_Nodes* tmp_private_field_node = NULL;
	Ans_Ast_Nodes* protected_field_node = NULL;
	Ans_Ast_Nodes* tmp_protected_field_node = NULL;
	Ans_Field_Type_Cache* field_type_cache = NULL;
	while ((end_code = end_code_ans_ast_block(src_ans_token_list)) != 0) {
		if (end_code < 0) 	if (!field_type_cache)break; else { field_type_cache = delete_now_field_cache(field_type_cache); continue; }

		if (ans_token_list_expect_token_str(src_ans_token_list, "pbl")) {
			if (ans_token_list_expect_token_str(src_ans_token_list, "{")) {
				field_type_cache = gen_and_chain_ans_field_cache(field_type_cache, E_Ans_Ast_Token_Type_Class_Field_Public);
			}
			else {
				build_ans_ast_field_node(&tmp_public_field_node, &public_field_node, E_Ans_Ast_Token_Type_Class_Field_Public, src_ans_token_list);
			}
		}

		else if (ans_token_list_expect_token_str(src_ans_token_list, "+{")) {
			field_type_cache = gen_and_chain_ans_field_cache(field_type_cache, E_Ans_Ast_Token_Type_Class_Field_Public);
		}
		else if (ans_token_list_expect_token_str(src_ans_token_list, "prv")) {
			if (ans_token_list_expect_token_str(src_ans_token_list, "{")) {
				field_type_cache = gen_and_chain_ans_field_cache(field_type_cache, E_Ans_Ast_Token_Type_Class_Field_Private);

			}
			else {
				build_ans_ast_field_node(&tmp_private_field_node, &private_field_node, E_Ans_Ast_Token_Type_Class_Field_Private, src_ans_token_list);
			}
		}
		else if (ans_token_list_expect_token_str(src_ans_token_list, "-{")) {
			field_type_cache = gen_and_chain_ans_field_cache(field_type_cache, E_Ans_Ast_Token_Type_Class_Field_Private);
		}
		else if (ans_token_list_expect_token_str(src_ans_token_list, "prt")) {
			if (ans_token_list_expect_token_str(src_ans_token_list, "{")) {
				field_type_cache = gen_and_chain_ans_field_cache(field_type_cache, E_Ans_Ast_Token_Type_Class_Field_Protected);

			}
			else {
				build_ans_ast_field_node(&tmp_protected_field_node, &protected_field_node, E_Ans_Ast_Token_Type_Class_Field_Protected, src_ans_token_list);
			}
		}
		else if (ans_token_list_expect_token_str(src_ans_token_list, "%{")) {
			field_type_cache = gen_and_chain_ans_field_cache(field_type_cache, E_Ans_Ast_Token_Type_Class_Field_Protected);
		}
		else if (ans_token_list_expect_token_str(src_ans_token_list, "{")) {
			field_type_cache = gen_and_chain_ans_field_cache(field_type_cache, E_Ans_Ast_Token_Type_Block);
		}
		else {
			Ans_Ast_Token_Type field_type = get_field_cache_type(field_type_cache, src_null_field_type);
			if (field_type == E_Ans_Ast_Token_Type_Class_Field_Public)
				build_ans_ast_field_node(&tmp_public_field_node, &public_field_node, field_type, src_ans_token_list);
			else if (field_type == E_Ans_Ast_Token_Type_Class_Field_Private)
				build_ans_ast_field_node(&tmp_private_field_node, &private_field_node, field_type, src_ans_token_list);
			else if (field_type == E_Ans_Ast_Token_Type_Class_Field_Protected)
				build_ans_ast_field_node(&tmp_protected_field_node, &protected_field_node, field_type, src_ans_token_list);
			else assert(false);
		}
	}

	Ans_Ast_Nodes* final_field_node = NULL;
	Ans_Ast_Nodes* tmp_final_node = NULL;
	if (public_field_node) {
		build_field_chain(&final_field_node, &tmp_final_node, public_field_node, tmp_public_field_node);
	}

	if (private_field_node) {
		build_fields_chain(&final_field_node, &tmp_final_node, private_field_node, tmp_private_field_node);
	}
	if (protected_field_node) {
		build_fields_chain(&final_field_node, &tmp_final_node, protected_field_node, tmp_protected_field_node);
	}
	if (end_code == 0) {
		const char* decl_err = !field_type_cache
			? src_class_field_err
			: get_class_field_err_str(field_type_cache->field_type);
		Ans_Ast_Nodes* err_class_node =
			set_up_ans_ast(NULL, NULL, E_Ans_Ast_Token_Type_None_Value,
				decl_err, ext_strlen_add_null(decl_err));
		if (err_class_node) {
			err_class_node->right = tmp_final_node;
			tmp_final_node = err_class_node;
		}
	}
	while (field_type_cache) field_type_cache = delete_now_field_cache(field_type_cache);
	return tmp_final_node;
}
Ans_Ast_Nodes* build_ans_ast_class_field_decl(Ans_Lex_Token_List* src_ans_token_list) {
	Ans_Ast_Nodes* field_node = NULL;
	if (ans_token_list_expect_token_data(src_ans_token_list, E_Ans_Lex_Token_Type_Keyword,"obj")) field_node = build_ans_ast_iden_decl(src_ans_token_list);
	else if (ans_token_list_expect_token_data(src_ans_token_list, E_Ans_Lex_Token_Type_Punchcute,";")) field_node = build_ans_ast_null_expr_statement();
	else if (ans_token_list_expect_token_data(src_ans_token_list, E_Ans_Lex_Token_Type_Keyword,"cls"))field_node = build_ans_ast_class(src_ans_token_list);
	else {
		consume_ans_lex_token(src_ans_token_list);
		field_node = build_ans_ast_err_node(NULL, NULL, "error : expect to decl in filed node");
	}
	return field_node;
}
Ans_Ast_Nodes* build_ans_ast_host_return_and_breaks(Ans_Lex_Token_List* src_ans_token_list, Ans_Ast_Token_Type src_token_type) {
	Ans_Ast_Nodes* ans_left = set_up_ans_ast(NULL, NULL, src_token_type, NULL, 0);
	if (!ans_left)return ans_left;
	if (ans_token_list_expect_token_data(src_ans_token_list, E_Ans_Lex_Token_Type_Punchcute,";"))return ans_left;
	ans_left->right = build_ans_ast_expr_comma(src_ans_token_list);
	return build_ans_ast_expr_statement_err(src_ans_token_list, ans_left);
}

Ans_Ast_Nodes* build_ans_ast_expr_statement_err(Ans_Lex_Token_List* src_ans_token_list, Ans_Ast_Nodes* src_left) {
	if (!ans_token_list_expect_token_data(src_ans_token_list, E_Ans_Lex_Token_Type_Punchcute,";")) {
		const char* expr_statement_err_base = "error : expect to ';' after ";
		const Ans_Lex_Token* now_lex_token = peek_ans_lex_token(src_ans_token_list);
		const char* now_token_str = now_lex_token ? now_lex_token->ans_token_str : "end code";
		size_t err_len = strlen(expr_statement_err_base);
		size_t token_len = ext_strlen_add_null(now_token_str);
		char* expr_statement_err = smart_calloc(char, token_len + err_len);
		if (expr_statement_err) {
			memcpy(expr_statement_err, expr_statement_err_base, err_len);
			memcpy(expr_statement_err + err_len, now_token_str, token_len);
			set_up_ast_node_op_datas(src_left, E_Ans_Ast_Token_Type_None_Value, expr_statement_err, token_len + err_len);
			free(expr_statement_err);
		}
	}
	return src_left;
}
Ans_Ast_Nodes* build_ans_ast_expr_statement(Ans_Lex_Token_List* src_ans_token_list, Ans_Ast_Nodes* src_left) {
	Ans_Ast_Nodes* ans_left = set_up_ans_ast(src_left, NULL, E_Ans_Ast_Token_Type_Expr_Statement, NULL, 0);
	if (!ans_left)return ans_left;
	return build_ans_ast_expr_statement_err(src_ans_token_list, ans_left);
}
Ans_Ast_Nodes* build_ans_ast_expr_comma(Ans_Lex_Token_List* src_ans_token_list) {
	Ans_Ast_Nodes* ans_left = build_ans_ast_assigment(src_ans_token_list);
	while (ans_token_list_expect_token_data(src_ans_token_list, E_Ans_Lex_Token_Type_Operator,",")) {
		Ans_Ast_Nodes* ans_right = smart_calloc(Ans_Ast_Nodes, 1);
		if (!ans_right)break;
		ans_right->left = ans_left;
		set_up_ast_node_op_datas(ans_right, E_Ans_Ast_Token_Type_Expr_Comma, NULL, 0);
		ans_right->right = build_ans_ast_assigment(src_ans_token_list);
		ans_left = ans_right;
	}
	return ans_left;
}
Ans_Ast_Nodes* build_ans_ast_assigment(Ans_Lex_Token_List* src_ans_token_list) {
	Ans_Ast_Nodes* ans_left = build_ans_ast_bit_or(src_ans_token_list);
	Ans_Ast_Token_Type tmp_type;
	while ((tmp_type = expect_assigment_type(src_ans_token_list))) {
		Ans_Ast_Nodes* ans_right = smart_calloc(Ans_Ast_Nodes, 1);
		if (!ans_right)break;
		ans_right->left = ans_left;

		set_up_ast_node_op_datas(ans_right, tmp_type, NULL, 0);
		ans_right->right = build_ans_ast_assigment(src_ans_token_list);
		ans_left = ans_right;
	}
	return ans_left;
}

Ans_Ast_Nodes* build_ans_ast_bit_or(Ans_Lex_Token_List* src_ans_token_list) {
	Ans_Ast_Nodes* ans_left = build_ans_ast_bit_xor(src_ans_token_list);
	while (ans_token_list_expect_token_data(src_ans_token_list, E_Ans_Lex_Token_Type_Operator,"|")) {
		Ans_Ast_Nodes* ans_right = smart_calloc(Ans_Ast_Nodes, 1);
		if (!ans_right) break;
		ans_right->left = ans_left;
		set_up_ast_node_op_datas(ans_right, E_Ans_Ast_Token_Type_Or, NULL, 0);
		ans_right->right = build_ans_ast_bit_xor(src_ans_token_list);
		ans_left = ans_right;
	}
	return ans_left;
}
Ans_Ast_Nodes* build_ans_ast_bit_xor(Ans_Lex_Token_List* src_ans_token_list) {
	Ans_Ast_Nodes* ans_left = build_ans_ast_bit_and(src_ans_token_list);
	while (ans_token_list_expect_token_data(src_ans_token_list, E_Ans_Lex_Token_Type_Operator,"^")) {
		Ans_Ast_Nodes* ans_right = smart_calloc(Ans_Ast_Nodes, 1);
		if (!ans_right) break;
		ans_right->left = ans_left;
		set_up_ast_node_op_datas(ans_right, E_Ans_Ast_Token_Type_Xor, NULL, 0);
		ans_right->right = build_ans_ast_bit_and(src_ans_token_list);
		ans_left = ans_right;
	}
	return ans_left;
}
Ans_Ast_Nodes* build_ans_ast_bit_and(Ans_Lex_Token_List* src_ans_token_list) {
	Ans_Ast_Nodes* ans_left = build_ans_ast_equlas(src_ans_token_list);
	while (ans_token_list_expect_token_data(src_ans_token_list, E_Ans_Lex_Token_Type_Operator,"&")) {
		Ans_Ast_Nodes* ans_right = smart_calloc(Ans_Ast_Nodes, 1);
		if (!ans_right) break;
		ans_right->left = ans_left;
		set_up_ast_node_op_datas(ans_right, E_Ans_Ast_Token_Type_And, NULL, 0);
		ans_right->right = build_ans_ast_equlas(src_ans_token_list);
		ans_left = ans_right;
	}
	return ans_left;
}

Ans_Ast_Nodes* build_ans_ast_equlas(Ans_Lex_Token_List* src_ans_token_list) {
	Ans_Ast_Nodes* ans_left = build_ans_ast_relation(src_ans_token_list);
	Ans_Ast_Token_Type tmp_type;
	while ((tmp_type = expect_equals_type(src_ans_token_list))) {
		Ans_Ast_Nodes* ans_right = smart_calloc(Ans_Ast_Nodes, 1);
		if (!ans_right)break;
		ans_right->left = ans_left;
		set_up_ast_node_op_datas(ans_right, tmp_type, NULL, 0);
		ans_right->right = build_ans_ast_relation(src_ans_token_list);
		ans_left = ans_right;
	}
	return ans_left;
}

Ans_Ast_Nodes* build_ans_ast_relation(Ans_Lex_Token_List* src_ans_token_list) {
	Ans_Ast_Nodes* ans_left = build_ans_ast_shift(src_ans_token_list);
	Ans_Ast_Token_Type tmp_type;
	while ((tmp_type = expect_relation_type(src_ans_token_list))) {
		Ans_Ast_Nodes* ans_right = smart_calloc(Ans_Ast_Nodes, 1);
		if (!ans_right)break;
		ans_right->left = ans_left;
		set_up_ast_node_op_datas(ans_right, tmp_type, NULL, 0);
		ans_right->right = build_ans_ast_shift(src_ans_token_list);
		ans_left = ans_right;
	}
	return ans_left;
}
Ans_Ast_Nodes* build_ans_ast_shift(Ans_Lex_Token_List* src) {
	Ans_Ast_Nodes* ans_left = build_ans_ast_add_sub(src);
	Ans_Ast_Token_Type tmp_type;
	while ((tmp_type = expect_shift_type(src))) {
		Ans_Ast_Nodes* ans_right = smart_calloc(Ans_Ast_Nodes, 1);
		if (!ans_right) break;
		ans_right->left = ans_left;
		set_up_ast_node_op_datas(ans_right, tmp_type, NULL, 0);
		ans_right->right = build_ans_ast_add_sub(src);
		ans_left = ans_right;
	}
	return ans_left;
}

Ans_Ast_Nodes* build_ans_ast_add_sub(Ans_Lex_Token_List* src_ans_token_list) {
	Ans_Ast_Nodes* ans_left = build_ans_ast_mul_div_mod(src_ans_token_list);
	Ans_Ast_Token_Type tmp_type;
	while ((tmp_type = expect_add_sub_type(src_ans_token_list))) {
		Ans_Ast_Nodes* ans_right = smart_calloc(Ans_Ast_Nodes, 1);
		if (!ans_right)break;
		ans_right->left = ans_left;
		set_up_ast_node_op_datas(ans_right, tmp_type, NULL, 0);
		ans_right->right = build_ans_ast_mul_div_mod(src_ans_token_list);
		ans_left = ans_right;
	}
	return ans_left;
}
Ans_Ast_Nodes* build_ans_ast_mul_div_mod(Ans_Lex_Token_List* src_ans_token_list) {
	Ans_Ast_Nodes* ans_left = build_ans_ast_sign_symbol(src_ans_token_list);
	Ans_Ast_Token_Type tmp_type;
	while ((tmp_type = expect_mul_div_mod_type(src_ans_token_list))) {
		Ans_Ast_Nodes* ans_right = smart_calloc(Ans_Ast_Nodes, 1);
		if (!ans_right)break;
		ans_right->left = ans_left;
		set_up_ast_node_op_datas(ans_right, tmp_type, NULL, 0);
		ans_right->right = build_ans_ast_sign_symbol(src_ans_token_list);
		ans_left = ans_right;
	}

	return ans_left;
}
Ans_Ast_Nodes* build_ans_ast_sign_symbol(Ans_Lex_Token_List* src_ans_token_list) {

	if (ans_token_list_expect_token_data(src_ans_token_list,E_Ans_Lex_Token_Type_Operator,"*")) {
		Ans_Lex_Token* now_lex_token = peek_ans_lex_token(src_ans_token_list);
		if (!now_lex_token)return NULL;
		if (now_lex_token->ans_token_type == E_Ans_Lex_Token_Type_Iden) {
			Ans_Ast_Nodes* ans_left = set_up_ans_ast(NULL, NULL, E_Ans_Ast_Token_Type_Class_Instance, now_lex_token->ans_token_str, ext_strlen_add_null(now_lex_token->ans_token_str));
			consume_ans_lex_token(src_ans_token_list);
			if (!ans_left)return NULL;
			if (ans_token_list_expect_token_data(src_ans_token_list, E_Ans_Lex_Token_Type_Punchcute, "(")) {
				printf("debug out\n");
				if (ans_token_list_expect_token_data(src_ans_token_list, E_Ans_Lex_Token_Type_Punchcute, ")")) {
					return ans_left;
				}
				ans_left->right = build_ans_ast_function_args(src_ans_token_list);
				if (ans_token_list_expect_token_data(src_ans_token_list, E_Ans_Lex_Token_Type_Punchcute, ")")) {
					return ans_left;
				}
				else {
					const char* err_innstance_class = "error : expect to ')' after instance class.";
					set_up_ast_node_op_datas(ans_left, E_Ans_Ast_Token_Type_None_Value, err_innstance_class, ext_strlen_add_null(err_innstance_class));
					return ans_left;
				}
			}
			else return ans_left;
		}
		else {
			const char* err_instance_class = "error : expect to class name after '*'";
			Ans_Ast_Nodes* ans_left = set_up_ans_ast(NULL, NULL, E_Ans_Ast_Token_Type_None_Value, err_instance_class, ext_strlen_add_null(err_instance_class));
			return ans_left;
		}
	}
	else if (ans_token_list_expect_token_data(src_ans_token_list, E_Ans_Lex_Token_Type_Operator,"-")) {
		Ans_Ast_Nodes* ans_left = smart_calloc(Ans_Ast_Nodes, 1);
		if (!ans_left)return NULL;
		Ans_Ast_Nodes* ans_right = build_ans_ast_sign_symbol(src_ans_token_list);
		set_up_ast_node_op_datas(ans_left, E_Ans_Ast_Token_Type_Sub, NULL, 0);
		ans_left->right = ans_right;
		return ans_left;
	}

	else if (ans_token_list_expect_token_data(src_ans_token_list, E_Ans_Lex_Token_Type_Operator,"~")) {
		Ans_Ast_Nodes* ans_left = smart_calloc(Ans_Ast_Nodes, 1);
		if (!ans_left)return NULL;
		Ans_Ast_Nodes* ans_right = build_ans_ast_sign_symbol(src_ans_token_list);
		set_up_ast_node_op_datas(ans_left, E_Ans_Ast_Token_Type_Not, NULL, 0);
		ans_left->right = ans_right;
		return ans_left;
	}

	else if (ans_token_list_expect_token_data(src_ans_token_list, E_Ans_Lex_Token_Type_Operator,"!")) {
		Ans_Ast_Nodes* ans_left = smart_calloc(Ans_Ast_Nodes, 1);
		if (!ans_left)return NULL;
		Ans_Ast_Nodes* ans_right = build_ans_ast_sign_symbol(src_ans_token_list);
		set_up_ast_node_op_datas(ans_left, E_Ans_Ast_Token_Type_Not_Logic, NULL, 0);
		ans_left->right = ans_right;
		return ans_left;
	}

	else if (ans_token_list_expect_token_data(src_ans_token_list, E_Ans_Lex_Token_Type_Operator, "...")) {
		Ans_Ast_Nodes* ans_left = smart_calloc(Ans_Ast_Nodes, 1);
		if (!ans_left)return NULL;
		Ans_Ast_Nodes* ans_right = build_ans_ast_back_ope(src_ans_token_list);
		set_up_ast_node_op_datas(ans_left, E_Ans_Ast_Token_Type_Stack_F, NULL, 0);
		ans_left->right = ans_right;
		return ans_left;
	}
	else return build_ans_ast_back_ope(src_ans_token_list);
}

Ans_Ast_Nodes* build_ans_ast_back_ope(Ans_Lex_Token_List* src_ans_token_list) {
	Ans_Ast_Nodes* ans_left = build_ans_ast_function(src_ans_token_list);
	Ans_Ast_Token_Type tmp_type;
	while ((tmp_type = expect_back_ope_type(src_ans_token_list))) {
		Ans_Ast_Nodes* ans_right = smart_calloc(Ans_Ast_Nodes, 1);
		if (!ans_right)break;
		ans_right->left = ans_left;
		set_up_ast_node_op_datas(ans_right, tmp_type, NULL, 0);
		ans_right->right = build_ans_ast_function(src_ans_token_list);
		ans_left = ans_right;
	}
	return ans_left;
}
Ans_Ast_Nodes* build_ans_ast_function_args(Ans_Lex_Token_List* src_ans_token_list) {
	Ans_Ast_Nodes* ans_left = build_ans_ast_assigment(src_ans_token_list);
	Ans_Ast_Nodes* ans_args = set_up_ans_ast(NULL, ans_left, E_Ans_Ast_Token_Type_Func_Args, NULL, 0);
	if (!ans_args)return ans_args;

	while (ans_token_list_expect_token_data(src_ans_token_list, E_Ans_Lex_Token_Type_Operator,",")) {
		Ans_Ast_Nodes* ans_right = smart_calloc(Ans_Ast_Nodes, 1);
		if (!ans_right)break;
		ans_right->left = ans_args;
		set_up_ast_node_op_datas(ans_right, E_Ans_Ast_Token_Type_Func_Args, NULL, 0);
		ans_right->right = build_ans_ast_assigment(src_ans_token_list);
		ans_args = ans_right;
	}

	return ans_args;
}
Ans_Ast_Nodes* build_ans_ast_function(Ans_Lex_Token_List* src_ans_token_list) {
	Ans_Ast_Nodes* ans_left = build_ans_ast_expr_keyword(src_ans_token_list);
	while (ans_token_list_expect_token_data(src_ans_token_list, E_Ans_Lex_Token_Type_Punchcute,"(")) {
		Ans_Ast_Nodes* ans_right = smart_calloc(Ans_Ast_Nodes, 1);
		if (ans_right) {
			ans_right->left = ans_left;
			if (ans_token_list_expect_token_data(src_ans_token_list, E_Ans_Lex_Token_Type_Punchcute,  ")")) {
				ans_right->right = set_up_ans_ast(NULL, NULL, E_Ans_Ast_Token_Type_Func_Args, NULL, 0);
				set_up_ast_node_op_datas(ans_right, E_Ans_Ast_Token_Type_Func_Use, NULL, 0);
			}
			else {
				ans_right->right = build_ans_ast_function_args(src_ans_token_list);
				if (ans_token_list_expect_token_data(src_ans_token_list, E_Ans_Lex_Token_Type_Punchcute, ")"))
					set_up_ast_node_op_datas(ans_right, E_Ans_Ast_Token_Type_Func_Use, NULL, 0);
				else
					set_up_ast_node_op_datas(ans_right, E_Ans_Ast_Token_Type_None_Value, "error : expect to ')' after func use", ext_strlen_add_null("error : expect to ')' after func use"));
			}
			ans_left = ans_right;
		}
	}
	return ans_left;
}

Ans_Ast_Nodes* build_ans_ast_expr_keyword(Ans_Lex_Token_List* src_ans_token_list) {

	if (ans_token_list_expect_token_data(src_ans_token_list, E_Ans_Lex_Token_Type_Keyword, "loop")) {
		Ans_Ast_Nodes* ans_left = set_up_ans_ast(NULL, NULL, E_Ans_Ast_Token_Type_Loop, NULL, 0);
		if (!ans_left)return ans_left;
		if (ans_token_list_expect_token_data(src_ans_token_list, E_Ans_Lex_Token_Type_Punchcute, "{")) {
			int end_check = 0;
			Ans_Ast_Nodes* ans_right = build_ans_ast_block_process(&end_check, src_ans_token_list, E_Ans_Ast_Token_Type_Seq_Abstract_Host);
			const char* loop_err_str = "error : expect to '}' after loop.";
			if (end_check == 0)set_up_ans_ast_data(ans_left, loop_err_str, ext_strlen_add_null(loop_err_str));
			ans_left->right = ans_right;
			return ans_left;
		}
		return ans_left;
	}
	return build_ans_ast_pare(src_ans_token_list);
}
Ans_Ast_Nodes* build_ans_ast_pare(Ans_Lex_Token_List* src_ans_token_list) {

	if (ans_token_list_expect_token_data(src_ans_token_list, E_Ans_Lex_Token_Type_Punchcute,"(")) {
		Ans_Ast_Nodes* ans_left = smart_calloc(Ans_Ast_Nodes, 1);
		if (!ans_left)return NULL;
		ans_left->right = build_ans_ast_expr_comma(src_ans_token_list);
		if (ans_token_list_expect_token_data(src_ans_token_list, E_Ans_Lex_Token_Type_Punchcute,")")) set_up_ast_node_op_datas(ans_left, E_Ans_Ast_Token_Type_Expr_Pare, NULL, 0);
		else set_up_ast_node_op_datas(ans_left, E_Ans_Ast_Token_Type_None_Value, "error : expect to ')'", ext_strlen_add_null("error : expect to ')'"));
		return ans_left;
	}
	return build_ans_ast_basic_token(src_ans_token_list);
}

Ans_Ast_Nodes* build_ans_ast_basic_token(Ans_Lex_Token_List* src_ans_token_list) {
	Ans_Lex_Token* now_token_list = peek_ans_lex_token(src_ans_token_list);
	if (!now_token_list)return NULL;
	Ans_Ast_Nodes* ans_left = smart_calloc(Ans_Ast_Nodes, 1);
	if (!ans_left)return NULL;
	const char* error_str = "error : expect to operator.";
	switch (now_token_list->ans_token_type) {
	case E_Ans_Lex_Token_Type_Iden:
		set_up_ast_node_op_datas(ans_left, E_Ans_Ast_Token_Type_Iden, now_token_list->ans_token_str, ext_strlen_add_null(now_token_list->ans_token_str));
		break;
	case E_Ans_Lex_Token_Type_Value_Literal:
		decision_final_accurate_binary_type(ans_left, now_token_list->ans_token_str);
		break;
	case E_Ans_Lex_Token_Type_Special_Literal:
		decision_final_accurate_specail_value(ans_left, now_token_list->ans_token_str);

		break;
	case E_Ans_Lex_Token_Type_Char_Literal:
		set_up_ast_node_op_datas(ans_left, E_Ans_Ast_Token_Type_Char_Value, now_token_list->ans_token_str, sizeof(char));
	case E_Ans_Lex_Token_Type_Str_Literal:
		set_up_ast_node_op_datas(ans_left, E_Ans_Ast_Token_Type_String_Value, now_token_list->ans_token_str, ext_strlen_add_null(now_token_list->ans_token_str));
		break;
	case E_Ans_Lex_Token_Type_Keyword:
		if (simple_strcmp(now_token_list->ans_token_str, "fld")) set_up_ast_node_op_datas(ans_left, E_Ans_Ast_Token_Type_Fld_Iden, NULL, 0);
		break;
	default:
		set_up_ast_node_op_datas(ans_left, E_Ans_Ast_Token_Type_None_Value, error_str, ext_strlen_add_null(error_str));
		break;
	}
	consume_ans_lex_token(src_ans_token_list);

	return ans_left;
}