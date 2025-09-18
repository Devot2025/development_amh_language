#include "hash_process_module_host.h"
Amh_Ast_Nodes* start_build_amh_module(Amh_Ast_Nodes* src_ast_node, const char * src_abstract_host_name) {
	if (!src_abstract_host_name)return NULL;
	Module_Field_Node amh_module = { .module_node = NULL, .top_module_node = NULL };

	build_amh_module_filed(src_ast_node, &amh_module);
	return decision_amh_module_node(&amh_module, src_abstract_host_name);
}
Amh_Ast_Nodes* merge_amh_module_node(Module_Field_Node * src_ast_mod_node, const char * src_abstract_host_name) {
	Amh_Ast_Nodes* amh_left = smart_calloc(Amh_Ast_Nodes, 1);
	Amh_Ast_Nodes* amh_right = smart_calloc(Amh_Ast_Nodes, 1);
	Amh_Ast_Nodes* amh_host = smart_calloc(Amh_Ast_Nodes, 1);
	Amh_Ast_Nodes* amh_mod_host = smart_calloc(Amh_Ast_Nodes, 1);

	if (!amh_left || !amh_right || !amh_host || amh_mod_host) return NULL;
	size_t host_str_len = strlen(src_abstract_host_name) + 1;
	amh_left->op = ast_op_memdup(E_Amh_Type_Iden, src_abstract_host_name, host_str_len);

	amh_right->left = amh_left;
	amh_right->op = ast_op_memdup(E_Amh_Type_Func_Def, NULL, 0);
	amh_right->right = src_ast_mod_node->top_module_node;

	amh_host->left = amh_right;
	amh_host->op = ast_op_memdup(E_Amh_Type_Abstract_Host, NULL, 0);
	amh_host->right = src_ast_mod_node->module_node;

	return amh_host;
}
Amh_Ast_Nodes* decision_amh_module_node(Module_Field_Node* src_ast_mod_node, const char* src_abstract_host_name) {
	Amh_Ast_Nodes* module_left = merge_amh_module_node(src_ast_mod_node, src_abstract_host_name);

	if (!module_left) return delete_amh_module_node(src_ast_mod_node);
	
	Amh_Ast_Nodes* amh_left = smart_calloc(Amh_Ast_Nodes, 1);
	Amh_Ast_Nodes* amh_right = smart_calloc(Amh_Ast_Nodes, 1);

	if (!amh_left || !amh_right) return delete_amh_module_node(src_ast_mod_node);
	
	size_t host_str_len = strlen(src_abstract_host_name) + 1;
	
	amh_left->op = ast_op_memdup(E_Amh_Type_Iden, src_abstract_host_name, host_str_len);
	
	
	amh_right->left = amh_left;
	
	amh_right->op = ast_op_memdup(E_Amh_Type_Class_Def, NULL, 0);
	amh_right->right = module_left;
	return amh_right;
}
void build_amh_module_filed(Amh_Ast_Nodes* src_ast_node, Module_Field_Node* src_mod_field_node) {
	if (!src_ast_node)return;
	if (ast_op_memcmp_token_type(src_ast_node, E_Amh_Type_Men_Acc))return;
	if (ast_op_memcmp_token_type(src_ast_node, E_Amh_Type_Func_Use))return;
	build_amh_module_filed(src_ast_node->left, src_mod_field_node);
	build_amh_module_filed(src_ast_node->right, src_mod_field_node);

	if (ast_op_memcmp_token_type(src_ast_node, E_Amh_Type_Iden)) {
		if (!src_mod_field_node->module_node) {
			src_mod_field_node->module_node = smart_calloc(Amh_Ast_Nodes, 1);
			if (!src_mod_field_node->module_node)return;
			src_mod_field_node->module_node->op = ast_op_iden_dup(src_ast_node);
		}
		else {
			Amh_Ast_Nodes* amh_right = smart_calloc(Amh_Ast_Nodes, 1);

			if (!amh_right)return;
			amh_right->op = ast_op_iden_dup(src_ast_node);
			src_mod_field_node->module_node->right = amh_right;

		}

		Amh_Ast_Nodes* amh_left = smart_calloc(Amh_Ast_Nodes, 1);
		if (!amh_left)return;
		amh_left->left = src_mod_field_node->module_node;
		amh_left->op = ast_op_memdup(E_Amh_Type_Statement, NULL, 0);
		amh_left->right = NULL;
		src_mod_field_node->module_node = amh_left;
	}
	else if (ast_op_memcmp_token_type(src_ast_node, E_Amh_Type_Statement)) {
		if (!src_mod_field_node->top_module_node) {
			src_mod_field_node->top_module_node = src_ast_node;
		}
		else {
			src_ast_node->left = src_mod_field_node->top_module_node;
			src_mod_field_node->top_module_node = src_ast_node;
		}
	}
}