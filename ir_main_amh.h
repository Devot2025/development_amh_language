#ifndef _IR_MAIN_AMH_H_
#define _IR_MAIN_AMH_H_
#include "parser_main_amh.h"
typedef enum Amh_Intermediate_Run_Host_Code_Ope {
	E_Amh_Irh_Add,
	E_Amh_Irh_Sub,
	E_Amh_Irh_Push,
	E_Amh_Irh_Pop,
	E_Amh_Irh_Get_Filed,
	E_Amh_Irh_Set_Filed,
	E_Amh_Irh_Goto_Program,
	E_Amh_Irh_Define_
}Amh_Intermediate_Run_Host_Code_Ope;
typedef struct Amh_Line_Code {
	Amh_Line_Code* next_line_code;
	Amh_Intermediate_Run_Host_Code_Ope irh_ope_code;
	void* code_adress;
}Amh_Line_Code;
void build_amh_ast_to_amh_line_code(Amh_Ast_Nodes * src_abstract_host_node) {
	if (!src_abstract_host_node)return;
	if (!src_abstract_host_node->op)return;
	switch (*(Run_Amh_Token_Type*)src_abstract_host_node->op) {
	case E_Amh_Type_Plus:
		build_amh_ast_to_amh_line_code(src_abstract_host_node->left);
		build_amh_ast_to_amh_line_code(src_abstract_host_node->right);
		break;
	case E_Amh_Type_Int:
		break;
	case E_Amh_Type_Func_Def:
		build_amh_ast_to_amh_line_code(src_abstract_host_node->left);
		build_amh_ast_to_amh_line_code(src_abstract_host_node->right);
	case E_Amh_Type_Men_Acc:
		build_amh_ast_to_amh_line_code(src_abstract_host_node->left);
		build_amh_ast_to_amh_line_code(src_abstract_host_node->right);
		break;
	}
	if (ast_op_memcmp_token_type(src_abstract_host_node, E_Amh_Type_Plus)) {

	}
}
void start_build_amh_ast_to_amh_line_code(Amh_Abstract_Host_Node* src_abstract_host_node) {
	if (!src_abstract_host_node) return;
	Amh_Abstract_Host_Node* tmp_node = src_abstract_host_node;
	while (tmp_node) {


		tmp_node = tmp_node->amh_block_next;
	}
}
#endif // !_IR_MAIN_AMH_H_
