#ifndef _HASH_PROCESS_MODULE_HOST_H_
#define _HASH_PROCESS_MODULE_HOST_H_
#include "parser_main_amh.h"

#define hash_process_is_strap static inline
typedef struct Amh_Ast_Nodes Amh_Ast_Nodes;
typedef struct Module_Field_Node {
	Amh_Ast_Nodes* module_node;
	Amh_Ast_Nodes* top_module_node;
}Module_Field_Node;
hash_process_is_strap void* delete_amh_module_node(Module_Field_Node* src_ast_mod_node) {
	delete_ast_amh_node(src_ast_mod_node->module_node);
	delete_ast_amh_node(src_ast_mod_node->top_module_node);
	return NULL;
}
Amh_Ast_Nodes* merge_amh_module_node(Module_Field_Node* src_ast_mod_node, const char* src_abstract_host_name);
Amh_Ast_Nodes* decision_amh_module_node(Module_Field_Node* src_ast_mod_node, const char* src_abstract_host_name);

void build_amh_module_filed(Amh_Ast_Nodes* src_ast_node, Module_Field_Node* src_mod_field_node);
#endif // !_HASH_PROCESS_MODULE_HOST_H_
