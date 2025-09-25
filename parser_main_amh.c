#include "parser_main_amh.h"
#include "amh_hash_process_main.h"
#include <math.h>
#include <float.h>

const char* debug_type_string[] = { TOKEN_TYPE_LIST(GEN_ENUM_STRING) };

void* ast_op_memdup(Run_Amh_Token_Type src_token_type, const void* dat, size_t src_size) {

	typedef union ___Block {
		char* m_block;
		void* s_block;
		size_t i_block;
	}___Block;

	size_t cap = sizeof(Run_Amh_Token_Type) + src_size;

	___Block new_op_block;

	new_op_block.s_block = calloc(cap, 1);

	if (!new_op_block.s_block)return NULL;

	memcpy(new_op_block.s_block, &src_token_type, sizeof(Run_Amh_Token_Type));

	void* tmp_block = new_op_block.s_block;

	new_op_block.m_block += sizeof(Run_Amh_Token_Type);
	if (dat && src_size)memcpy(new_op_block.s_block, dat, src_size);

	return tmp_block;
}

void* ast_op_iden_dup(const Amh_Ast_Nodes* src_ast_node) {

	const char* tmp_token_iden_str = acc_iden_token_string(src_ast_node);
	size_t iden_str_len = strlen(tmp_token_iden_str) + 1;
	return ast_op_memdup(E_Amh_Type_Iden, tmp_token_iden_str, iden_str_len);
}
void* decision_final_accurate_binary_type(const char* src_str) {
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

			return ast_op_memdup(E_Amh_Type_Float, &tmp_val_data.float_data, sizeof(float));
		}
		else {
			/*double check*/
			return ast_op_memdup(E_Amh_Type_Double, &tmp_val_data.double_data, sizeof(double));

		}

	}
	else {
		/* change to python-style infinite int*/
		tmp_val_data.int_data = (int)strtol(src_str, NULL, 0);
		return ast_op_memdup(E_Amh_Type_Int, &tmp_val_data.int_data, sizeof(int));// ast_op_memdup(E_Amh_Type_Int, NULL, 0);
	}
}
void delete_ast_amh_node(Amh_Ast_Nodes * src_amh_ast) {
	if(!src_amh_ast)return;
	delete_ast_amh_node(src_amh_ast->left);

	delete_ast_amh_node(src_amh_ast->right);
	free(src_amh_ast->op);
	free(src_amh_ast);
	src_amh_ast = NULL;
}


void print_ast_amh_node(Amh_Ast_Nodes* src_amh_ast, uint32_t src_node_indent) {
	if (!src_amh_ast)return;
	if (!src_amh_ast->op)return;

	if (*(Run_Amh_Token_Type*)(src_amh_ast->op) == E_Amh_Type_Abstract_Host) {

		for (uint32_t i = 0; i < src_node_indent; ++i)putchar('.');
		printf("type => %s", debug_type_string[*(Run_Amh_Token_Type*)(src_amh_ast->op)]);
		putchar('\n');

		while (src_amh_ast) {
			print_ast_amh_node(src_amh_ast->right, src_node_indent + 1);
			src_amh_ast = src_amh_ast->left;
		}

	}
	else {
		print_ast_amh_node(src_amh_ast->left, src_node_indent + 1);

		if (*(Run_Amh_Token_Type*)(src_amh_ast->op) != E_Amh_Type_NULL) {

			for (uint32_t i = 0; i < src_node_indent; ++i)putchar('.');
			printf("type => %s", debug_type_string[*(Run_Amh_Token_Type*)(src_amh_ast->op)]);
			if (ast_op_memcmp_token_type(src_amh_ast, E_Amh_Type_Iden))
				printf("(\x1b[31m%s\x1b[0m)", acc_iden_token_string(src_amh_ast));

			putchar('\n');
		}

		print_ast_amh_node(src_amh_ast->right, src_node_indent + 1);
	}
}

typedef struct Amh_Iden_Id {
	uintptr_t regist_num;
	char* regist_id;
	char** regist_str;
}Amh_Iden_Id;
void* get_regist_amh_var_id(Amh_Iden_Id* src_amh_iden_id, const char* iden_token) {
	if (src_amh_iden_id->regist_id[src_amh_iden_id->regist_num] >= CHAR_MAX) {
		src_amh_iden_id->regist_num++;
		char* tmp_amh_id = smart_realloc(char, &src_amh_iden_id->regist_id[src_amh_iden_id->regist_num], src_amh_iden_id->regist_num);
	}
	return NULL;
}
uintptr_t get_test_mode_amh_var_id(Amh_Iden_Id* src_amh_iden_id, const char* src_iden) {
	/*
	for (uintptr_t i = 0; i < src_amh_iden_id->regist_num; ++i)
		if (simple_strcmp(&src_amh_iden_id->regist_str[i], src_iden))return i;
		*/
	return 0;
}

Run_Amh_Token_Type multi_amh_token_expect(Amh_Lex_Token_List* src_lex_list, const char** src_token_str_list, const Run_Amh_Token_Type* src_token_type_list, uintptr_t src_list_size) {
	uintptr_t i = 0;
	for (i = 0; i < src_list_size; ++i) {
		if (amh_token_list_expect_str(src_lex_list, src_token_str_list[i]))return src_token_type_list[i];
	}
	return E_Amh_Type_NULL;
}
Run_Amh_Token_Type assigments_amh_token_expect(Amh_Lex_Token_List* src_lex_list) {
	const char* assigments_token_list[] = {
		"=", "+=", "-=", "*=", "/="
	};
	const Run_Amh_Token_Type assigments_token_type_list[] = {
		E_Amh_Type_Assignment, E_Amh_Type_Plus_Assignment,
		E_Amh_Type_Minus_Assignment, E_Amh_Type_Mul_Assignment,
		E_Amh_Type_Div_Assignment
	};
	return multi_amh_token_expect(src_lex_list, assigments_token_list, assigments_token_type_list, GET_ARRAY_SIZE(assigments_token_list));
}

Run_Amh_Token_Type add_sub_amh_token_expect(Amh_Lex_Token_List* src_lex_list) {
	const char* add_sub_token_list[] = {
		"+", "-"
	};
	const Run_Amh_Token_Type assigments_token_type_list[] = {
		E_Amh_Type_Plus, E_Amh_Type_Minus
	};
	return multi_amh_token_expect(src_lex_list, add_sub_token_list, assigments_token_type_list, GET_ARRAY_SIZE(add_sub_token_list));
}

Run_Amh_Token_Type mul_div_amh_token_expect(Amh_Lex_Token_List* src_lex_list) {
	const char* mul_div_token_list[] = {
		"/", "*"
	};
	const Run_Amh_Token_Type assigments_token_type_list[] = {
		E_Amh_Type_Mul, E_Amh_Type_Div
	};
	return multi_amh_token_expect(src_lex_list, mul_div_token_list, assigments_token_type_list, GET_ARRAY_SIZE(mul_div_token_list));
}

Run_Amh_Token_Type back_ope_amh_token_expect(Amh_Lex_Token_List* src_lex_list) {
	const char* back_ope_token_list[] = {
		"::", "."
	};
	const Run_Amh_Token_Type assigments_token_type_list[] = {
		E_Amh_Type_Var_Change, E_Amh_Type_Men_Acc
	};
	return multi_amh_token_expect(src_lex_list, back_ope_token_list, assigments_token_type_list, GET_ARRAY_SIZE(back_ope_token_list));
}

void ast_op_type_memcpy(void* src_op, Run_Amh_Token_Type src_amh_token_type) {
	memcpy(src_op, &src_amh_token_type, sizeof(Run_Amh_Token_Type));
}
Amh_Ast_Nodes* start_amh_parser_main(Amh_Lex_Token_List* src_lex_list){
	if (!src_lex_list->stack_token_list_size) return NULL;
	if (!src_lex_list->stack_token_list) return NULL;
	/*start amh parser -----> */
	return build_ast_abstract_amh_host(src_lex_list);
}
Amh_Abstract_Host_Ast_Chain* build_ast_amh_abstract_host_node(Amh_Lex_Token_List* src_lex_list) {
	src_lex_list->stack_token_list_index = 0;
	Amh_Abstract_Host_Ast_Chain* dst_abstract_node = NULL;
	Amh_Abstract_Host_Ast_Chain* tmp_abstract_node = NULL;
	while (src_lex_list->stack_token_list_index < src_lex_list->stack_token_list_size) {
		Amh_Ast_Nodes* left = build_ast_amh_block(src_lex_list);

		if (!left || ast_op_memcmp_token_type(left, E_Amh_Type_Unknown)) {
			consume_advance_list(src_lex_list);
		}

		Amh_Abstract_Host_Ast_Chain* new_abstract_node = smart_calloc(Amh_Abstract_Host_Ast_Chain, 1);
		if (!new_abstract_node)break;
		new_abstract_node->amh_block = left;
		if (!dst_abstract_node) {
			dst_abstract_node = new_abstract_node;
			tmp_abstract_node = dst_abstract_node;
		}
		else{
			dst_abstract_node->amh_block_next = new_abstract_node;
			dst_abstract_node = new_abstract_node;
		}
	}
	return dst_abstract_node = tmp_abstract_node;
}
Amh_Ast_Nodes * build_ast_abstract_amh_host(Amh_Lex_Token_List * src_lex_list) {
	/*reset pos*/

	src_lex_list->stack_token_list_index = 0;
	
	Run_Amh_Token_Type hash_process_type = E_Amh_Type_NULL;

	Amh_Ast_Nodes* amh_chain_node = NULL;
	Amh_Ast_Nodes* tmp_amh_chain_node = NULL;
	while (src_lex_list->stack_token_list_index < src_lex_list->stack_token_list_size) {
		//hash_process_type = get_hash_type_token(src_lex_list);
		Amh_Ast_Nodes* amh_next = smart_calloc(Amh_Ast_Nodes, 1);
		if (!amh_next)  break; 
		Amh_Ast_Nodes* amh_node = build_ast_amh_block_recursive(src_lex_list);
		if (!amh_node) { free(amh_next); break; }
		if (ast_op_memcmp_token_type(amh_node, E_Amh_Type_Unknown)) consume_advance_list(src_lex_list);

		if (!amh_chain_node) {
			/*begin, left chain null node*/
			amh_next->left = amh_chain_node;
			amh_next->op = ast_op_memdup(E_Amh_Type_Abstract_Host, NULL, 0);
			amh_next->right = amh_node;
			tmp_amh_chain_node = amh_next;
			amh_chain_node = amh_next;
		}
		else {
			amh_next->op = ast_op_memdup(E_Amh_Type_Abstract_Host, NULL, 0);
			amh_next->right = amh_node;
			/*decision before node.*/
			amh_chain_node->left = amh_next;
			amh_chain_node = amh_next;
		}
	}

	return tmp_amh_chain_node;
}
Amh_Ast_Nodes* build_ast_amh_block_recursive(Amh_Lex_Token_List* src_lex_list) {
	Amh_Ast_Nodes* amh_left = build_ast_amh_comma(src_lex_list);
	if (amh_token_list_expect_str(src_lex_list, "{")) {

		Amh_Ast_Nodes* amh_right = smart_calloc(Amh_Ast_Nodes, 1);
		if (amh_right) {
			amh_right->left = amh_left;
			Amh_Ast_Nodes* amh_chain_node = NULL;
			Amh_Ast_Nodes* tmp_amh_chain_node = NULL;
			int block_end_check = 0;
			while ((block_end_check = amh_token_block_process_check(src_lex_list)) > 0) {
				Amh_Ast_Nodes* amh_next = smart_calloc(Amh_Ast_Nodes, 1);
				if (!amh_next) { block_end_check = 0; break; }
				Amh_Ast_Nodes * amh_node = build_ast_amh_block_recursive(src_lex_list);
				if (!amh_node) { block_end_check = 0; free(amh_next); break; }
				if (ast_op_memcmp_token_type(amh_node, E_Amh_Type_Unknown)) consume_advance_list(src_lex_list);

				if (!amh_chain_node) {
					/*begin, left chain null node*/
					amh_next->left = amh_chain_node;
					amh_next->op = ast_op_memdup(E_Amh_Type_Abstract_Host, NULL, 0);
					amh_next->right = amh_node;
					tmp_amh_chain_node = amh_next;
					amh_chain_node = amh_next;
				}
				else {
					amh_next->op = ast_op_memdup(E_Amh_Type_Abstract_Host, NULL, 0);
					amh_next->right = amh_node;
					/*decision before node.*/
					amh_chain_node->left = amh_next;
					amh_chain_node = amh_next;
				}
			}
			amh_right->right = tmp_amh_chain_node;

			if (ast_op_memcmp_token_type(amh_left, E_Amh_Type_Func_Use)) {
				const char* function_err = "error: expected '}' after function def.";

				ast_op_type_memcpy(amh_left->op, E_Amh_Type_Func);
				if (block_end_check == -1)
					amh_right->op = ast_op_memdup(E_Amh_Type_Func_Def, NULL, 0);
				else
					amh_right->op = ast_op_memdup(E_Amh_Type_None, function_err, strlen(function_err) + 1);
			}
			else {
				const char* block_err = "error: expected '}' after block.";
				if (block_end_check == -1)
					amh_right->op = ast_op_memdup(E_Amh_Type_Block, NULL, 0);
				else
					amh_right->op = ast_op_memdup(E_Amh_Type_None, block_err, strlen(block_err) + 1);
			}
			amh_left = amh_right;

		}
	}
	/*
	* 
	*/
	else if (amh_token_list_expect_str(src_lex_list, ";")) {

		Amh_Ast_Nodes* amh_right = smart_calloc(Amh_Ast_Nodes, 1);
		if (amh_right) {
			amh_right->op = ast_op_memdup(E_Amh_Type_Statement, NULL, 0);
			amh_right->left = amh_left;
			amh_left = amh_right;
		}
	}
	else {
		if (ast_op_memcmp_token_type(amh_left, E_Amh_Type_NULL))
			ast_op_type_memcpy(amh_left->op, E_Amh_Type_Unknown);

	}
	return amh_left;
}
Amh_Ast_Nodes* build_ast_amh_block(Amh_Lex_Token_List* src_lex_list) {
	Amh_Ast_Nodes * amh_left = build_ast_amh_next_iden(src_lex_list);

	if (amh_token_list_expect_str(src_lex_list, "{")) {

		Amh_Ast_Nodes* amh_right = smart_calloc(Amh_Ast_Nodes, 1);
		if (amh_right) {
			amh_right->left = amh_left;
			amh_right->right = build_ast_amh_next_iden(src_lex_list);
			if (ast_op_memcmp_token_type(amh_left, E_Amh_Type_Func_Use)) {
				const char* function_err = "error: expected '}' after function definition";

				ast_op_type_memcpy(amh_left->op, E_Amh_Type_Func);
				if (amh_token_list_expect_str(src_lex_list, "}"))
					amh_right->op = ast_op_memdup(E_Amh_Type_Func_Def, NULL, 0);
				else
					amh_right->op = ast_op_memdup(E_Amh_Type_None, function_err, strlen(function_err) + 1);

			}
			else if (ast_op_memcmp_token_type(amh_left, E_Amh_Type_Statement) && amh_left->right) {
				if (ast_op_memcmp_token_type(amh_left->right, E_Amh_Type_Func_Use)) {

					const char* function_err = "error: expected '}' after function definition.";

					ast_op_type_memcpy(amh_left->right->op, E_Amh_Type_Func);
					if (amh_token_list_expect_str(src_lex_list, "}"))
						amh_right->op = ast_op_memdup(E_Amh_Type_Func_Def, NULL, 0);
					else
						amh_right->op = ast_op_memdup(E_Amh_Type_None, function_err, strlen(function_err) + 1);
				}
				else {
					const char* function_err = "error: expected ';' after expression.";

					amh_right->op = ast_op_memdup(E_Amh_Type_None, function_err, strlen(function_err) + 1);
				}

			}
			else {
				const char* block_err = "error: expected '}' after block.";

				if (amh_token_list_expect_str(src_lex_list, "}"))
					amh_right->op = ast_op_memdup(E_Amh_Type_Block, NULL, 0);
				else
					amh_right->op = ast_op_memdup(E_Amh_Type_None, block_err, strlen(block_err) + 1);
			}
			amh_left = amh_right;

		}
	}
	else {
		if(ast_op_memcmp_token_type(amh_left, E_Amh_Type_NULL))
			ast_op_type_memcpy(amh_left->op, E_Amh_Type_Unknown);

	}
	return amh_left;
}
Amh_Ast_Nodes* build_ast_amh_next_iden(Amh_Lex_Token_List* src_lex_list) {
	Amh_Ast_Nodes* amh_left = build_ast_amh_comma(src_lex_list);

	while (amh_token_list_expect_str(src_lex_list, ";")) {
		Amh_Ast_Nodes* amh_right = smart_calloc(Amh_Ast_Nodes, 1);
		if (!amh_right)break;
		amh_right->left = amh_left;
		amh_right->op = ast_op_memdup(E_Amh_Type_Statement, NULL, 0);
		amh_right->right = build_ast_amh_comma(src_lex_list);
		amh_left = amh_right;

	}

	return amh_left;
}

Amh_Ast_Nodes* build_ast_amh_comma(Amh_Lex_Token_List* src_lex_list) {
	Amh_Ast_Nodes* amh_left = build_ast_amh_assigment(src_lex_list);
	while (amh_token_list_expect_str(src_lex_list, ",")) {
		Amh_Ast_Nodes* amh_right = smart_calloc(Amh_Ast_Nodes, 1);
		if (!amh_right)break;
		amh_right->left = amh_left;
		amh_right->op = ast_op_memdup(E_Amh_Type_Comma, NULL, 0);
		amh_right->right = build_ast_amh_assigment(src_lex_list);
		amh_left = amh_right;
	}

	return amh_left;
}
Amh_Ast_Nodes* build_ast_amh_assigment(Amh_Lex_Token_List* src_lex_list) {
	Amh_Ast_Nodes* amh_left = build_ast_amh_add_sub(src_lex_list);
	Run_Amh_Token_Type tmp_amh_type;
	if (tmp_amh_type = assigments_amh_token_expect(src_lex_list)) {
		Amh_Ast_Nodes* amh_right = smart_calloc(Amh_Ast_Nodes, 1);
		if (amh_right) {
			amh_right->left = amh_left;
			amh_right->op = ast_op_memdup(tmp_amh_type, NULL, 0);
			amh_right->right = build_ast_amh_assigment(src_lex_list);
			amh_left = amh_right;
		}
	}
	return amh_left;
}
Amh_Ast_Nodes* build_ast_amh_add_sub(Amh_Lex_Token_List* src_lex_list) {
	Amh_Ast_Nodes* amh_left = build_ast_amh_mul_div(src_lex_list);
	Run_Amh_Token_Type tmp_amh_type;
	while (tmp_amh_type = add_sub_amh_token_expect(src_lex_list)) {
		Amh_Ast_Nodes* amh_right = smart_calloc(Amh_Ast_Nodes, 1);
		if (!amh_right)break;
		amh_right->left = amh_left;
		amh_right->op = ast_op_memdup(tmp_amh_type, NULL, 0);
		amh_right->right = build_ast_amh_mul_div(src_lex_list);
		amh_left = amh_right;
	}
	return amh_left;
}

Amh_Ast_Nodes* build_ast_amh_mul_div(Amh_Lex_Token_List* src_lex_list) {
	Amh_Ast_Nodes* amh_left = build_ast_amh_back_ope(src_lex_list);
	Run_Amh_Token_Type tmp_amh_type;
	while (tmp_amh_type = mul_div_amh_token_expect(src_lex_list)) {
		Amh_Ast_Nodes* amh_right = smart_calloc(Amh_Ast_Nodes, 1);
		if (!amh_right)break;
		amh_right->left = amh_left;
		amh_right->op = ast_op_memdup(tmp_amh_type, NULL, 0);
		amh_right->right = build_ast_amh_back_ope(src_lex_list);
		amh_left = amh_right;
	}
	return amh_left;
}

Amh_Ast_Nodes* build_ast_amh_back_ope(Amh_Lex_Token_List* src_lex_list) {
	Amh_Ast_Nodes* amh_left = build_ast_amh_use_function(src_lex_list);
	Run_Amh_Token_Type tmp_amh_type;
	while (tmp_amh_type = back_ope_amh_token_expect(src_lex_list)) {
		Amh_Ast_Nodes* amh_right = smart_calloc(Amh_Ast_Nodes, 1);
		if (!amh_right)break;
		amh_right->left = amh_left;
		amh_right->op = ast_op_memdup(tmp_amh_type, NULL, 0);
		amh_right->right = build_ast_amh_use_function(src_lex_list);
		amh_left = amh_right;
	}
	return amh_left;
}

Amh_Ast_Nodes* build_ast_amh_use_function(Amh_Lex_Token_List* src_lex_list) {
	Amh_Ast_Nodes* amh_left = build_ast_amh_pare(src_lex_list);
	while((ast_op_memcmp_token_type(amh_left, E_Amh_Type_Iden)) && amh_token_list_expect_str(src_lex_list, "(")) {
		Amh_Ast_Nodes* amh_right = smart_calloc(Amh_Ast_Nodes, 1);
		if (!amh_right) break;
		amh_right->left = amh_left;
		amh_right->right = build_ast_amh_comma(src_lex_list);
		const char* func_err_str = "error : expect to ')'";

		if (amh_token_list_expect_str(src_lex_list, ")")) amh_right->op = ast_op_memdup(E_Amh_Type_Func_Use, NULL, 0);
		/*none is err*/
		else {
			amh_right->op = ast_op_memdup(E_Amh_Type_None, func_err_str, strlen(func_err_str) + 1);
		}
		amh_left = amh_right;
	}
	return amh_left;
}

Amh_Ast_Nodes* build_ast_amh_pare(Amh_Lex_Token_List* src_lex_list) {
	if (amh_token_list_expect_str(src_lex_list, "(")) {
		Amh_Ast_Nodes* amh_right = smart_calloc(Amh_Ast_Nodes, 1);
		if (amh_right) {
			amh_right->right = build_ast_amh_comma(src_lex_list);
			const char* pare_err_str = "error : expect to ')'";
			if (amh_token_list_expect_str(src_lex_list, ")")) amh_right->op = ast_op_memdup(E_Amh_Type_Pare, NULL, 0);
			/*none is err*/
			else amh_right->op = ast_op_memdup(E_Amh_Type_None, pare_err_str, strlen(pare_err_str) + 1);
		}
		return amh_right;
	}
	return build_ast_amh_basic_token(src_lex_list);
}
Amh_Ast_Nodes* build_ast_amh_basic_token(Amh_Lex_Token_List* src_lex_list) {
	Amh_Ast_Nodes* amh_left = smart_calloc(Amh_Ast_Nodes, 1);
	if (!amh_left) return NULL;

	const Amh_Lex_Token * now_token = get_now_amh_token(src_lex_list);
	if (!now_token) {
		amh_left->op = ast_op_memdup(E_Amh_Type_NULL, NULL, 0);
		return amh_left;
	}
	uintptr_t lex_data_len;
	const char* basic_token_err;
	switch (now_token->amh_token_type) {
	case E_Amh_Lex_Token_Type_Iden:
		/*
		* which should the variable name be, binary or str?
		*/
		lex_data_len = strlen(now_token->amh_token);
		amh_left->op = ast_op_memdup(E_Amh_Type_Iden, now_token->amh_token, lex_data_len + 1);
		consume_advance_list(src_lex_list);

		break;
	case E_Amh_Lex_Token_Type_Bin_Literal:
		amh_left->op = decision_final_accurate_binary_type(now_token->amh_token);
		consume_advance_list(src_lex_list);

		break;
	case E_Amh_Lex_Token_Type_D_Str_Literal:
		lex_data_len = strlen(now_token->amh_token);
		amh_left->op = ast_op_memdup(E_Amh_Type_String, now_token->amh_token, lex_data_len + 1);
		consume_advance_list(src_lex_list);

		break;
	case E_Amh_Lex_Token_Type_Specail_Literal:
		if (simple_strcmp("v_em", now_token->amh_token)) {
			amh_left->op = ast_op_memdup(E_Amh_Type_Virtual_Empty, NULL, 0);
		}
		else {
			/*normal run */
			lex_data_len = strlen(now_token->amh_token);

			basic_token_err = "error : normal run not consider to none literal";
			amh_left->op = ast_op_memdup(E_Amh_Type_None, basic_token_err, strlen(basic_token_err) + 1);
				/*none error
				amh_left->op = ast_op_memdup(E_Amh_Type_None, NULL, 0);
			*/

		}
		consume_advance_list(src_lex_list);
		break;
	case E_Amh_Lex_Hash_Process:

		break;
	default:
		amh_left->op = ast_op_memdup(E_Amh_Type_NULL, NULL, 0);
	}
	return amh_left;
}