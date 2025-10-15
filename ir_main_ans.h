#ifndef _IR_MAIN_ANS_H_
#define _IR_MAIN_ANS_H_
#include "parser_main_ans.h"
#define ans_ir_si_strap static inline
#define AST_IR_STR_OUT_POS 19
#define Bulk_Gen_Ans_Bir_Instruction(To_Do_Macro_Func)\
To_Do_Macro_Func(E_Abir_Instruction_Null),\
To_Do_Macro_Func(E_Abir_Instruction_Acc_Process),\
To_Do_Macro_Func(E_Abir_Instruction_Abstract_Host_Return),\
To_Do_Macro_Func(E_Abir_Instruction_Abstract_Host_Return_Policy),\
To_Do_Macro_Func(E_Abir_Instruction_Abstract_Seq_Host_Return),\
To_Do_Macro_Func(E_Abir_Instruction_Abstract_Seq_Host_All_Return),\
To_Do_Macro_Func(E_Abir_Instruction_A_Absorute_Check),\
To_Do_Macro_Func(E_Abir_Instruction_A_Push),\
To_Do_Macro_Func(E_Abir_Instruction_A_Pop),\
To_Do_Macro_Func(E_Abir_Instruction_A_Discard),\
To_Do_Macro_Func(E_Abir_Instruction_A_Const_Push), \
To_Do_Macro_Func(E_Abir_Instruction_A_Const_Pop), \
To_Do_Macro_Func(E_Abir_Instruction_Set_Abstract_Host),\
To_Do_Macro_Func(E_Abir_Instruction_Set_Seq_Abstract_Host),\
To_Do_Macro_Func(E_Abir_Instruction_A_Set_Start_Loop),\
To_Do_Macro_Func(E_Abir_Instruction_A_Iteration_Loop),\
To_Do_Macro_Func(E_Abir_Instruction_A_Add),\
To_Do_Macro_Func(E_Abir_Instruction_A_Sub),\
To_Do_Macro_Func(E_Abir_Instruction_A_Mul),\
To_Do_Macro_Func(E_Abir_Instruction_A_Div),\
To_Do_Macro_Func(E_Abir_Instruction_A_Mod),\
To_Do_Macro_Func(E_Abir_Instruction_A_Logic_Not),\
To_Do_Macro_Func(E_Abir_Instruction_A_A),\
To_Do_Macro_Func(E_Abir_Instruction_Set),\
To_Do_Macro_Func(E_Abir_Instruction_),\
To_Do_Macro_Func(E_Abir_Instruction_A),\
To_Do_Macro_Func(E_Abir_Instruction_A_Virtual_Write), \
To_Do_Macro_Func(E_Abir_Instruction_A_Virtual_Read), \
To_Do_Macro_Func(E_Abir_Instruction_A_Virtual_Stdout), \
To_Do_Macro_Func(E_Abir_Instruction_A_Virtual_Stdin), \
To_Do_Macro_Func(E_Abir_Instruction_A_Virtual_Stderr), \
To_Do_Macro_Func(E_Abir_Instruction_A_End_Func_Decl), \
To_Do_Macro_Func(E_Abir_Instruction_A_Move_Program_Host), \
To_Do_Macro_Func(E_Abir_Instruction_A_Args_Alloc_Mem),\
To_Do_Macro_Func(E_Abir_Instruction_A_Alloc_Mem),\
To_Do_Macro_Func(E_Abir_Instruction_A_Start_Func_Decl),\
To_Do_Macro_Func(E_Abir_Instruction_A_Goto_Seq_Abstract_Hpst),

typedef enum Ans_Abir_Instrcution {
	Bulk_Gen_Ans_Bir_Instruction(CHANGE_TO_ENUM)
}Ans_Abir_Instruction;

extern const char* debug_abir_string[];

typedef enum Abir_Const_Data_Type {
	E_Abir_C_Null,
	E_Abir_C_Virtual,
	E_Abir_C_None,
	E_Abir_C_Name,
	E_Abir_C_Int,
	E_Abir_C_Char,
	E_Abir_C_Float,
	E_Abir_C_Double,
	E_Abir_C_Str,
}Abir_Const_Data_Type;
typedef struct Abir_Data {
	Abir_Const_Data_Type abir_cdata_type;
	void* v_data;
}Abir_Data;
typedef struct Ans_Bir_Chain {
	Ans_Abir_Instruction abir_instruction;
	Abir_Data a_data;
	struct Ans_Bir_Chain* next_bir;
}Ans_Bir_Chain;
ans_ir_si_strap Ans_Bir_Chain* gen_abir_chain(Ans_Bir_Chain* now_abir, Ans_Abir_Instruction src_abir_instruction, void* src_data, size_t src_data_size, Abir_Const_Data_Type src_c_data_type) {
	Ans_Bir_Chain* dst_abir_chain = smart_calloc(Ans_Bir_Chain, 1);
	if (!dst_abir_chain)return now_abir;
	dst_abir_chain->a_data.abir_cdata_type = src_c_data_type;
	dst_abir_chain->a_data.v_data = dupalloc(src_data, src_data_size);
	dst_abir_chain->abir_instruction = src_abir_instruction;
	if (now_abir)now_abir->next_bir = dst_abir_chain;
	return dst_abir_chain;
}
ans_ir_si_strap void print_debug_abir_instructions(Ans_Bir_Chain* src_ans_chain) {
	printf("\x1b[34m");

	printf("%-20s", debug_abir_string[src_ans_chain->abir_instruction] + AST_IR_STR_OUT_POS);
	printf("\x1b[0m");

	if (src_ans_chain->a_data.abir_cdata_type == E_Abir_C_Char) {
		const char out_char = *(const char*)src_ans_chain->a_data.v_data;
		printf("\x1b[35m");

		char quoted_buf[8];
		if (out_char == '\n')
			snprintf(quoted_buf, sizeof(quoted_buf), "\"%s\"", "\\n");
		else
			snprintf(quoted_buf, sizeof(quoted_buf), "\"%c\"", out_char);
		printf(" %20s", quoted_buf);
	}
	else if (src_ans_chain->a_data.abir_cdata_type == E_Abir_C_Int) {
		printf("\x1b[35m");

		printf(" %20d", *(const int*)src_ans_chain->a_data.v_data);
	}
	else if (src_ans_chain->a_data.abir_cdata_type == E_Abir_C_Float) {
		printf("\x1b[35m");

		printf(" %20f", *(const float*)src_ans_chain->a_data.v_data);
	}
	else if (src_ans_chain->a_data.abir_cdata_type == E_Abir_C_Double) {
		printf("\x1b[35m");

		printf(" %20lf", *(const double*)src_ans_chain->a_data.v_data);
	}
	else if (src_ans_chain->a_data.abir_cdata_type == E_Abir_C_Str) {
		printf("\x1b[33m");

		const char* out_str = (const char*)src_ans_chain->a_data.v_data;

		uint32_t str_size = ext_strlen_add_null(out_str);
		uint32_t tmp_str_size = str_size + 3;

		char* final_out_str = smart_malloc(char, tmp_str_size);
		if (final_out_str) {
			snprintf(final_out_str, tmp_str_size, "\"%s\"", out_str);

			printf(" %20s", final_out_str);
		}

		free(final_out_str);
	}

	else if (src_ans_chain->a_data.abir_cdata_type == E_Abir_C_Name) {
		printf("\x1b[31m");

		printf(" %20s", (const char*)src_ans_chain->a_data.v_data);
	}
	else if (src_ans_chain->a_data.abir_cdata_type == E_Abir_C_None) {
		printf("\x1b[34m");

		printf(" none[%20s]", (const char*)src_ans_chain->a_data.v_data);
	}
	else if (src_ans_chain->a_data.abir_cdata_type == E_Abir_C_Virtual) {
		printf("\x1b[35m");

		printf(" %20s", "virtual empty");
	}
	else if (src_ans_chain->a_data.abir_cdata_type == E_Abir_C_Null)printf("%20s", "");
	else assert(false);
	printf("\x1b[0m");
}
ans_ir_si_strap Ans_Bir_Chain* delete_ans_bir_chain(Ans_Bir_Chain* src_ans_chain) {
	while (src_ans_chain) {
		Ans_Bir_Chain* next_chain = src_ans_chain->next_bir;
		free(src_ans_chain->a_data.v_data);
		free(src_ans_chain);
		src_ans_chain = next_chain;
	}
	return NULL;
}
ans_ir_si_strap void start_print_abir_insruction(Ans_Bir_Chain* src_start_chain, bool break_check) {
	while (src_start_chain) {
		print_debug_abir_instructions(src_start_chain);
		if (break_check) {
			printf("			; ");
			char break_stack[256];
			if (!fgets(break_stack, GET_ARRAY_SIZE(break_stack), stdin))break;
		}
		else putchar('\n');

		src_start_chain = src_start_chain->next_bir;
	}
}

Ans_Bir_Chain* start_ans_ir_main(Ans_Ast_Nodes* src_ans_node);
Ans_Abir_Instruction get_function_instruction_type(const char* func_name);
Ans_Ast_Nodes* change_ast_node_to_abir_instruction(Ans_Bir_Chain** src_air_chain, Ans_Ast_Nodes* src_ans_node);
Ans_Ast_Nodes* change_ast_node_func_args(Ans_Bir_Chain** src_air_chain, Ans_Ast_Nodes* src_ans_node);
Ans_Ast_Nodes* change_ast_node_iden_pop(Ans_Bir_Chain** src_air_chain, Ans_Ast_Nodes* src_ans_node);
Ans_Ast_Nodes* change_ast_node_to_abir_iden_or_literal(Ans_Bir_Chain** src_air_chain, Ans_Ast_Nodes* src_ans_node);
#endif // !_IR_MAIN_ANS_H_
