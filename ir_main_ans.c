#include "ir_main_ans.h"

const char* debug_abir_string[] = {
   Bulk_Gen_Ans_Bir_Instruction(CHANGE_TO_STR)
};
Ans_Bir_Chain* start_ans_ir_main(Ans_Ast_Nodes* src_ans_node) {
	Ans_Bir_Chain* dst_abir_instruction = NULL;
	Ans_Bir_Chain* tmp_abir_instruction = NULL;
	if (src_ans_node->token_type == E_Ans_Ast_Token_Type_Abstract_Host) {
		bool no_return_check = false;

		tmp_abir_instruction = dst_abir_instruction = gen_abir_chain(dst_abir_instruction, E_Abir_Instruction_Set_Abstract_Host, NULL, 0, E_Abir_C_Null);
		while (src_ans_node) {
			change_ast_node_to_abir_instruction(&tmp_abir_instruction, src_ans_node->right);
			if (src_ans_node->right && src_ans_node->right->token_type == E_Ans_Ast_Token_Type_Host_Return)no_return_check = true;

			src_ans_node = src_ans_node->left;
		}

		if (!no_return_check) {

			change_ast_node_to_abir_iden_or_literal(&tmp_abir_instruction, NULL);
			tmp_abir_instruction = gen_abir_chain(tmp_abir_instruction, E_Abir_Instruction_Abstract_Host_Return, NULL, 0, E_Abir_C_Null);
		}
		tmp_abir_instruction = gen_abir_chain(tmp_abir_instruction, E_Abir_Instruction_Abstract_Host_Return_Policy, NULL, 0, E_Abir_C_Null);

	}
	return dst_abir_instruction;
}
Ans_Ast_Nodes* change_ast_node_to_abir_instruction(Ans_Bir_Chain** src_air_chain, Ans_Ast_Nodes* src_ans_node) {
	if (!src_ans_node)return NULL;


	if (src_ans_node->token_type == E_Ans_Ast_Token_Type_Abstract_Host) {
		*src_air_chain = gen_abir_chain(*src_air_chain, E_Abir_Instruction_Set_Abstract_Host, NULL, 0, E_Abir_C_Null);
		bool no_return_check = false;
		while (src_ans_node) {
			change_ast_node_to_abir_instruction(src_air_chain, src_ans_node->right);
			if (src_ans_node->right && src_ans_node->right->token_type == E_Ans_Ast_Token_Type_Host_Return)no_return_check = true;
			src_ans_node = src_ans_node->left;
		}
		if (!no_return_check) {

			change_ast_node_to_abir_iden_or_literal(src_air_chain, NULL);
			*src_air_chain = gen_abir_chain(*src_air_chain, E_Abir_Instruction_Abstract_Host_Return, NULL, 0, E_Abir_C_Null);
		}
		*src_air_chain = gen_abir_chain(*src_air_chain, E_Abir_Instruction_Abstract_Host_Return_Policy, NULL, 0, E_Abir_C_Null);

		return NULL;
	}
	else if (src_ans_node->token_type == E_Ans_Ast_Token_Type_Iden) {
		void* push_op = get_ans_ast_nodes_op(src_ans_node);
		*src_air_chain = gen_abir_chain(*src_air_chain, E_Abir_Instruction_A_Push, push_op, ext_strlen_add_null((char*)push_op), E_Abir_C_Name);
		return src_ans_node;
	}
	else if (src_ans_node->token_type == E_Ans_Ast_Token_Type_Char_Value) {
		void* push_op = get_ans_ast_nodes_op(src_ans_node);
		*src_air_chain = gen_abir_chain(*src_air_chain, E_Abir_Instruction_A_Const_Push, push_op, sizeof(char), E_Abir_C_Char);
	}
	else if (src_ans_node->token_type == E_Ans_Ast_Token_Type_Int_Value) {
		void* push_op = get_ans_ast_nodes_op(src_ans_node);
		*src_air_chain = gen_abir_chain(*src_air_chain, E_Abir_Instruction_A_Const_Push, push_op, sizeof(int), E_Abir_C_Int);
	}


	else if (src_ans_node->token_type == E_Ans_Ast_Token_Type_Float_Value) {
		void* push_op = get_ans_ast_nodes_op(src_ans_node);
		*src_air_chain = gen_abir_chain(*src_air_chain, E_Abir_Instruction_A_Const_Push, push_op, sizeof(float), E_Abir_C_Float);
	}

	else if (src_ans_node->token_type == E_Ans_Ast_Token_Type_Double_Value) {
		void* push_op = get_ans_ast_nodes_op(src_ans_node);
		*src_air_chain = gen_abir_chain(*src_air_chain, E_Abir_Instruction_A_Const_Push, push_op, sizeof(double), E_Abir_C_Double);
	}

	else if (src_ans_node->token_type == E_Ans_Ast_Token_Type_Virtuals_Value) {
		void* push_op = get_ans_ast_nodes_op(src_ans_node);
		*src_air_chain = gen_abir_chain(*src_air_chain, E_Abir_Instruction_A_Const_Push, NULL, 0, E_Abir_C_Virtual);
	}
	else if (src_ans_node->token_type == E_Ans_Ast_Token_Type_String_Value) {
		void* push_op = get_ans_ast_nodes_op(src_ans_node);
		size_t str_size = ext_strlen_add_null((const char*)push_op);
		*src_air_chain = gen_abir_chain(*src_air_chain, E_Abir_Instruction_A_Const_Push, push_op, str_size, E_Abir_C_Str);
	}
	else if (src_ans_node->token_type == E_Ans_Ast_Token_Type_Var_Decl) {
		*src_air_chain = gen_abir_chain(*src_air_chain, E_Abir_Instruction_A_Alloc_Mem, src_ans_node->op, ext_strlen_add_null((char*)src_ans_node->op), E_Abir_C_Name);
		change_ast_node_to_abir_iden_or_literal(src_air_chain, src_ans_node->right);
		*src_air_chain = gen_abir_chain(*src_air_chain, E_Abir_Instruction_A_Pop, src_ans_node->op, ext_strlen_add_null(src_ans_node->op), E_Abir_C_Name);
		return src_ans_node;
	}
	else if (src_ans_node->token_type == E_Ans_Ast_Token_Type_Func_Decl) {
		*src_air_chain = gen_abir_chain(*src_air_chain, E_Abir_Instruction_A_Start_Func_Decl, src_ans_node->op, ext_strlen_add_null(src_ans_node->op), E_Abir_C_Name);
		change_ast_node_func_args(src_air_chain, src_ans_node->left);
		change_ast_node_to_abir_instruction(src_air_chain, src_ans_node->right);
		*src_air_chain = gen_abir_chain(*src_air_chain, E_Abir_Instruction_A_End_Func_Decl, src_ans_node->op, ext_strlen_add_null(src_ans_node->op), E_Abir_C_Name);

	}

	else if (src_ans_node->token_type == E_Ans_Ast_Token_Type_Loop) {

		change_ast_node_func_args(src_air_chain, src_ans_node->left);
		change_ast_node_to_abir_instruction(src_air_chain, src_ans_node->right);
	}
	else if (src_ans_node->token_type == E_Ans_Ast_Token_Type_Seq_Host_Break) {
		*src_air_chain = gen_abir_chain(*src_air_chain, E_Abir_Instruction_Set_Seq_Abstract_Host, NULL, 0, E_Abir_C_Null);
		while (src_ans_node) {
			change_ast_node_to_abir_instruction(src_air_chain, src_ans_node->right);
			src_ans_node = src_ans_node->left;
		}
		*src_air_chain = gen_abir_chain(*src_air_chain, E_Abir_Instruction_A_Goto_Seq_Abstract_Hpst, NULL, 0, E_Abir_C_Null);

	}
	else if (src_ans_node->token_type == E_Ans_Ast_Token_Type_Assigment) {

		Ans_Ast_Nodes* pop_left = change_ast_node_iden_pop(src_air_chain, src_ans_node->left);
		change_ast_node_to_abir_instruction(src_air_chain, src_ans_node->right);
		void* pop_op = get_ans_ast_nodes_op(pop_left);

		*src_air_chain = gen_abir_chain(*src_air_chain, E_Abir_Instruction_A_Pop, pop_op, ext_strlen_add_null(pop_op), E_Abir_C_Name);

	}
	else if (src_ans_node->token_type == E_Ans_Ast_Token_Type_Host_Return) {
		change_ast_node_to_abir_iden_or_literal(src_air_chain, src_ans_node->right);
		*src_air_chain = gen_abir_chain(*src_air_chain, E_Abir_Instruction_Abstract_Host_Return, NULL, 0, E_Abir_C_Null);

	}

	else if (src_ans_node->token_type == E_Ans_Ast_Token_Type_Seq_Host_Break) {
		change_ast_node_to_abir_iden_or_literal(src_air_chain, src_ans_node->right);
		*src_air_chain = gen_abir_chain(*src_air_chain, E_Abir_Instruction_Abstract_Seq_Host_Return, NULL, 0, E_Abir_C_Null);

	}
	else if (src_ans_node->token_type == E_Ans_Ast_Token_Type_Seq_All_Host_Break) {
		change_ast_node_to_abir_instruction(src_air_chain, src_ans_node->right);
		*src_air_chain = gen_abir_chain(*src_air_chain, E_Abir_Instruction_Abstract_Seq_Host_All_Return, NULL, 0, E_Abir_C_Null);

	}
	else if (src_ans_node->token_type == E_Ans_Ast_Token_Type_Add) {
		change_ast_node_to_abir_instruction(src_air_chain, src_ans_node->left);
		change_ast_node_to_abir_instruction(src_air_chain, src_ans_node->right);
		*src_air_chain = gen_abir_chain(*src_air_chain, E_Abir_Instruction_A_Add, NULL, 0, E_Abir_C_Null);
	}

	else if (src_ans_node->token_type == E_Ans_Ast_Token_Type_Add_Assigment) {
		Ans_Ast_Nodes* pop_left = change_ast_node_iden_pop(src_air_chain, src_ans_node->left);

		change_ast_node_to_abir_instruction(src_air_chain, pop_left);
		change_ast_node_to_abir_instruction(src_air_chain, src_ans_node->right);
		void* pop_op = get_ans_ast_nodes_op(pop_left);

		*src_air_chain = gen_abir_chain(*src_air_chain, E_Abir_Instruction_A_Add, NULL, 0, E_Abir_C_Null);
		*src_air_chain = gen_abir_chain(*src_air_chain, E_Abir_Instruction_A_Pop, pop_op, ext_strlen_add_null(pop_op), E_Abir_C_Name);
	}

	else if (src_ans_node->token_type == E_Ans_Ast_Token_Type_Sub_Assigment) {
		Ans_Ast_Nodes* pop_left = change_ast_node_iden_pop(src_air_chain, src_ans_node->left);

		change_ast_node_to_abir_instruction(src_air_chain, pop_left);
		change_ast_node_to_abir_instruction(src_air_chain, src_ans_node->right);
		void* pop_op = get_ans_ast_nodes_op(pop_left);

		*src_air_chain = gen_abir_chain(*src_air_chain, E_Abir_Instruction_A_Sub, NULL, 0, E_Abir_C_Null);
		*src_air_chain = gen_abir_chain(*src_air_chain, E_Abir_Instruction_A_Pop, pop_op, ext_strlen_add_null(pop_op), E_Abir_C_Name);
	}

	else if (src_ans_node->token_type == E_Ans_Ast_Token_Type_Mul_Assigment) {
		Ans_Ast_Nodes* pop_left = change_ast_node_iden_pop(src_air_chain, src_ans_node->left);

		change_ast_node_to_abir_instruction(src_air_chain, pop_left);
		change_ast_node_to_abir_instruction(src_air_chain, src_ans_node->right);
		void* pop_op = get_ans_ast_nodes_op(pop_left);

		*src_air_chain = gen_abir_chain(*src_air_chain, E_Abir_Instruction_A_Mul, NULL, 0, E_Abir_C_Null);
		*src_air_chain = gen_abir_chain(*src_air_chain, E_Abir_Instruction_A_Pop, pop_op, ext_strlen_add_null(pop_op), E_Abir_C_Name);
	}

	else if (src_ans_node->token_type == E_Ans_Ast_Token_Type_Mod_Assigment) {
		Ans_Ast_Nodes* pop_left = change_ast_node_iden_pop(src_air_chain, src_ans_node->left);

		change_ast_node_to_abir_instruction(src_air_chain, pop_left);
		change_ast_node_to_abir_instruction(src_air_chain, src_ans_node->right);
		void* pop_op = get_ans_ast_nodes_op(pop_left);

		*src_air_chain = gen_abir_chain(*src_air_chain, E_Abir_Instruction_A_Mod, NULL, 0, E_Abir_C_Null);
		*src_air_chain = gen_abir_chain(*src_air_chain, E_Abir_Instruction_A_Pop, pop_op, ext_strlen_add_null(pop_op), E_Abir_C_Name);
	}

	else if (src_ans_node->token_type == E_Ans_Ast_Token_Type_Div_Assigment) {
		Ans_Ast_Nodes* pop_left = change_ast_node_iden_pop(src_air_chain, src_ans_node->left);

		change_ast_node_to_abir_instruction(src_air_chain, pop_left);
		change_ast_node_to_abir_instruction(src_air_chain, src_ans_node->right);
		void* pop_op = get_ans_ast_nodes_op(pop_left);

		*src_air_chain = gen_abir_chain(*src_air_chain, E_Abir_Instruction_A_Div, NULL, 0, E_Abir_C_Null);
		*src_air_chain = gen_abir_chain(*src_air_chain, E_Abir_Instruction_A_Pop, pop_op, ext_strlen_add_null(pop_op), E_Abir_C_Name);
	}
	else if (src_ans_node->token_type == E_Ans_Ast_Token_Type_Sub) {
		change_ast_node_to_abir_instruction(src_air_chain, src_ans_node->left);
		change_ast_node_to_abir_instruction(src_air_chain, src_ans_node->right);
		*src_air_chain = gen_abir_chain(*src_air_chain, E_Abir_Instruction_A_Sub, NULL, 0, E_Abir_C_Null);
	}
	else if (src_ans_node->token_type == E_Ans_Ast_Token_Type_Mul) {
		change_ast_node_to_abir_instruction(src_air_chain, src_ans_node->left);
		change_ast_node_to_abir_instruction(src_air_chain, src_ans_node->right);
		*src_air_chain = gen_abir_chain(*src_air_chain, E_Abir_Instruction_A_Mul, NULL, 0, E_Abir_C_Null);
	}
	else if (src_ans_node->token_type == E_Ans_Ast_Token_Type_Div) {
		change_ast_node_to_abir_instruction(src_air_chain, src_ans_node->left);
		change_ast_node_to_abir_instruction(src_air_chain, src_ans_node->right);
		*src_air_chain = gen_abir_chain(*src_air_chain, E_Abir_Instruction_A_Div, NULL, 0, E_Abir_C_Null);
	}
	else if (src_ans_node->token_type == E_Ans_Ast_Token_Type_Mod) {
		change_ast_node_to_abir_instruction(src_air_chain, src_ans_node->left);
		change_ast_node_to_abir_instruction(src_air_chain, src_ans_node->right);
		*src_air_chain = gen_abir_chain(*src_air_chain, E_Abir_Instruction_A_Mod, NULL, 0, E_Abir_C_Null);
	}
	else if (src_ans_node->token_type == E_Ans_Ast_Token_Type_Not_Logic) {
		change_ast_node_to_abir_instruction(src_air_chain, src_ans_node->left);
		change_ast_node_to_abir_instruction(src_air_chain, src_ans_node->right);
		*src_air_chain = gen_abir_chain(*src_air_chain, E_Abir_Instruction_A_Logic_Not, NULL, 0, E_Abir_C_Null);
	}

	else if (src_ans_node->token_type == E_Ans_Ast_Token_Type_Func_Use) {
		Ans_Ast_Nodes* pop_left = change_ast_node_iden_pop(src_air_chain, src_ans_node->left);
		change_ast_node_to_abir_instruction(src_air_chain, src_ans_node->right);

		const char* pop_op = (const char*)get_ans_ast_nodes_op(pop_left);
		*src_air_chain = gen_abir_chain(*src_air_chain, get_function_instruction_type(pop_op), pop_op, ext_strlen_add_null(pop_op), E_Abir_C_Name);
	}
	else if (src_ans_node->token_type == E_Ans_Ast_Token_Type_Func_Args) {
		change_ast_node_to_abir_instruction(src_air_chain, src_ans_node->left);
		change_ast_node_to_abir_instruction(src_air_chain, src_ans_node->right);
	}
	else if (src_ans_node->token_type == E_Ans_Ast_Token_Type_Expr_Pare) {
		change_ast_node_to_abir_instruction(src_air_chain, src_ans_node->left);
		change_ast_node_to_abir_instruction(src_air_chain, src_ans_node->right);
	}
	else if (src_ans_node->token_type == E_Ans_Ast_Token_Type_Expr_Statement) {
		change_ast_node_to_abir_instruction(src_air_chain, src_ans_node->left);
		change_ast_node_to_abir_instruction(src_air_chain, src_ans_node->right);
		if ((*src_air_chain)->abir_instruction != E_Abir_Instruction_A_Pop) {
			*src_air_chain = gen_abir_chain(*src_air_chain, E_Abir_Instruction_A_Discard, NULL, 0, E_Abir_C_Null);

		}
	}
	return NULL;
}
Ans_Abir_Instruction get_function_instruction_type(const char* func_name) {
	if (simple_strcmp(func_name, "____virtual__write"))return E_Abir_Instruction_A_Virtual_Write;
	if (simple_strcmp(func_name, "____virtual__read"))return E_Abir_Instruction_A_Virtual_Read;
	if (simple_strcmp(func_name, "____get__virtual__stdout"))return E_Abir_Instruction_A_Virtual_Stdout;
	else return E_Abir_Instruction_A_Move_Program_Host;
}
Ans_Ast_Nodes* change_ast_node_func_args(Ans_Bir_Chain** src_air_chain, Ans_Ast_Nodes* src_ans_node) {

	while (src_ans_node) {
		if (src_ans_node->op) {
			*src_air_chain = gen_abir_chain(*src_air_chain, E_Abir_Instruction_A_Args_Alloc_Mem, src_ans_node->op, ext_strlen_add_null((char*)src_ans_node->op), E_Abir_C_Name);
		}
		src_ans_node = src_ans_node->left;
	}
	return NULL;
}
Ans_Ast_Nodes* change_ast_node_iden_pop(Ans_Bir_Chain** src_air_chain, Ans_Ast_Nodes* src_ans_node) {
	if (!src_ans_node)return NULL;
	if (src_ans_node->token_type == E_Ans_Ast_Token_Type_Iden) return src_ans_node;
	else if (src_ans_node->token_type == E_Ans_Ast_Token_Type_Var_Decl) {

		*src_air_chain = gen_abir_chain(*src_air_chain, E_Abir_Instruction_A_Alloc_Mem, src_ans_node->op, ext_strlen_add_null((char*)src_ans_node->op), E_Abir_C_Name);
		return src_ans_node;
	}
	else change_ast_node_to_abir_instruction(src_air_chain, src_ans_node);
	return NULL;
}
Ans_Ast_Nodes* change_ast_node_to_abir_iden_or_literal(Ans_Bir_Chain** src_air_chain, Ans_Ast_Nodes* src_ans_node) {
	if (!src_ans_node) {
		*src_air_chain = gen_abir_chain(*src_air_chain, E_Abir_Instruction_A_Const_Push, NULL, 0, E_Abir_C_Virtual);
		return NULL;
	}

	if (src_ans_node->token_type == E_Ans_Ast_Token_Type_Var_Decl) {
		*src_air_chain = gen_abir_chain(*src_air_chain, E_Abir_Instruction_A_Alloc_Mem, src_ans_node->op, ext_strlen_add_null((char*)src_ans_node->op), E_Abir_C_Name);
		return src_ans_node;
	}

	else if (src_ans_node->token_type == E_Ans_Ast_Token_Type_Iden) {
		void* push_op = get_ans_ast_nodes_op(src_ans_node);
		*src_air_chain = gen_abir_chain(*src_air_chain, E_Abir_Instruction_A_Push, push_op, ext_strlen_add_null((char*)push_op), E_Abir_C_Name);
		return src_ans_node;
	}
	change_ast_node_to_abir_instruction(src_air_chain, src_ans_node);
	return NULL;
}