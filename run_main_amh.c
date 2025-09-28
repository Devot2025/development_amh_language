#include "run_main_amh.h"
Amh_Intermediate_Run_Host_Data* booting_amh_intermediate_host_data(Amh_Ast_Nodes * src_amh_nodes) {
	Amh_Intermediate_Run_Host_Data* dst_intermediate_run_host_data = smart_calloc(Amh_Intermediate_Run_Host_Data, 1);
	if (!dst_intermediate_run_host_data)return NULL;
	dst_intermediate_run_host_data->amh_ast_node = src_amh_nodes;
	return dst_intermediate_run_host_data;
}
void start_amh_run(Amh_Ast_Nodes * src_amh_nodes) {
	Amh_Intermediate_Run_Host_Data* amh_intermediate_run_host_data = booting_amh_intermediate_host_data(src_amh_nodes);
}
void amh_runtime_run(Amh_Intermediate_Run_Host_Data * src_intermediate_run_host_data) {
	if (!src_intermediate_run_host_data->amh_ast_node)return;
	if (ast_op_memcmp_token_type(src_intermediate_run_host_data->amh_ast_node, E_Amh_Type_Iden)) {

	}
	else if (ast_op_memcmp_token_type(src_intermediate_run_host_data->amh_ast_node, E_Amh_Type_Statement)) {
		Amh_Ast_Nodes* tmp_ast_node = src_intermediate_run_host_data->amh_ast_node;
		src_intermediate_run_host_data->amh_ast_node = tmp_ast_node->left;
		amh_runtime_run(src_intermediate_run_host_data);
		src_intermediate_run_host_data->amh_ast_node = tmp_ast_node->right;
		amh_runtime_run(src_intermediate_run_host_data);

	}
	else return;
}
void amh_run_iden_type(Amh_Intermediate_Run_Host_Data * src_intermediate_run_host_data) {

}
void regist_standard_io_synbol_table(){

}
void standard_io_synbol_table(Amh_Intermediate_Run_Host_Data * src_intermediate_run_host_data) {
}
void amh_putsln(const char* src_str) {
	printf(src_str);
	putchar('\n');
}
void amh_print(const char* src_str, int i) {
	fprintf(stdout, src_str);

}
void regist_symbol_sys_out_putln(Amh_Intermediate_Run_Host_Data* src_sys_run_host_data) {
	/*sys.out.putsln("hello world!")*/
	/*std.putln("hello world!");*/
	const char* amh_system = "sys";
	const char* amh_system_out = "out";
	const char* amh_io_putln = "putsln";
	const char* amh_write = "_______amh_write";
	
	regist_to_new_function_desc_symbol(&src_sys_run_host_data->symbol_table_datas->symbol_table, amh_write, printf);
	
	Amh_Class_Desc * tmp_new_out_class = regist_new_class_desc_symbol(&src_sys_run_host_data->symbol_table_datas->symbol_table, amh_system_out);

	if (!tmp_new_out_class)return;

	set_class_desc_to_method_symbol_desc(tmp_new_out_class, amh_io_putln, amh_putsln);
	Amh_Class_Desc* tmp_new_sys_class = regist_new_class_desc_symbol(&src_sys_run_host_data->symbol_table_datas->symbol_table, amh_system);
	/*クラス定義と変数は判別できる。(但し、同名クラス2軒以上は不可)*/
	set_class_desc_to_member_class_instnace_symbol(tmp_new_sys_class, amh_system_out, tmp_new_out_class);
	Amh_Standard_Value_Mem value_mem = instance_class_desc_to_object(tmp_new_sys_class);
	regist_object_to_symbol_table(&src_sys_run_host_data->symbol_table_datas->symbol_table, &src_sys_run_host_data->symbol_table_datas->objects_mem, amh_system, value_mem);

}
void regist_object_to_symbol_table(Amh_Symbol_Table_Base* src_symbol_table, Amh_Standard_Value_Mem ** dst_value_mem, const char* src_iden, Amh_Standard_Value_Mem src_value) {
	if (!src_symbol_table || !dst_value_mem || !src_iden)return;
	if (seek_to_table_symbols(src_symbol_table, src_iden, SEEK_TABLE_SYMBOL_OBJECT_NUMBER))return;
	Amh_Symbol_Table* new_object_symbol = smart_calloc(Amh_Symbol_Table, 1);
	if (!new_object_symbol)return;
	new_object_symbol->name = simple_strdup(src_iden);
	if (!new_object_symbol->name) {
		free(new_object_symbol);
		return;
	}
	Amh_Standard_Value_Mem * new_value_mem = smart_realloc(Amh_Standard_Value_Mem, (*dst_value_mem), src_symbol_table->object_symbol_num + 1);
	if (!new_value_mem) {
		free(new_object_symbol->name);
		free(new_object_symbol);

		return;
	}
	new_object_symbol->symbol_id = src_symbol_table->object_symbol_num;           
	new_object_symbol->next_symbol = src_symbol_table->objects;                     
	src_symbol_table->objects = new_object_symbol;
	(*dst_value_mem) = new_value_mem;
	(*((*dst_value_mem) + src_symbol_table->object_symbol_num)) = src_value;
	src_symbol_table->object_symbol_num++;
}
void set_class_desc_to_member_class_instnace_symbol(Amh_Class_Desc * src_class_desc, const char * src_iden, Amh_Class_Desc * base_instance_class_objects) {
	if (!src_class_desc)return;
	if (seek_to_table_symbols(&src_class_desc->filed_symbol_table, src_iden, SEEK_TABLE_SYMBOL_OBJECT_NUMBER)) {
		return;
	}
	Amh_Symbol_Table* new_objects_table = smart_calloc(Amh_Symbol_Table, 1);
	/*don't found class desc*/
	if (!new_objects_table)return;
	new_objects_table->name = simple_strdup(src_iden);
	if (!new_objects_table->name) {
		free(new_objects_table);
		return;
	}
	Amh_Standard_Value_Mem* ext_filed_objects_mem = smart_realloc(Amh_Standard_Value_Mem, src_class_desc->objects_mem, src_class_desc->filed_symbol_table.object_symbol_num + 1);
	if (!ext_filed_objects_mem) {
		free(new_objects_table->name);
		free(new_objects_table);
		return;
	}
	src_class_desc->objects_mem = ext_filed_objects_mem;
	src_class_desc->objects_mem[src_class_desc->filed_symbol_table.object_symbol_num].amh_standard_type = E_Amh_Standard_Class_Object;
	src_class_desc->objects_mem[src_class_desc->filed_symbol_table.object_symbol_num].amh_value_store.object = smart_calloc(Amh_Symbol_Class_Object, 1);
	/*desc default is null*/
	if (src_class_desc->objects_mem[src_class_desc->filed_symbol_table.object_symbol_num].amh_value_store.object)
	src_class_desc->objects_mem[src_class_desc->filed_symbol_table.object_symbol_num].amh_value_store.object->instance_base_class = base_instance_class_objects;
	new_objects_table->symbol_id = src_class_desc->filed_symbol_table.object_symbol_num;
	new_objects_table->next_symbol = src_class_desc->filed_symbol_table.objects;
	src_class_desc->filed_symbol_table.objects = new_objects_table;
	src_class_desc->filed_symbol_table.object_symbol_num++;
}
/*
* class A{
*    i = 1:
* }
* class B{
*     a = new A;
* }
* b = new B;
* b.a.i = 1;<--ここでAを生成したいと考える
*/
Amh_Standard_Value_Mem instance_class_desc_to_object(Amh_Class_Desc* src_class_desc) {
	Amh_Standard_Value_Mem amh_object_mem = { 0 };
	if (!src_class_desc)return amh_object_mem;

	amh_object_mem.amh_standard_type = E_Amh_Standard_Class_Object;
	amh_object_mem.amh_value_store.object = smart_calloc(Amh_Symbol_Class_Object, 1);
	if (!amh_object_mem.amh_value_store.object)return amh_object_mem;
	amh_object_mem.amh_value_store.object->instance_base_class = src_class_desc;

	amh_object_mem.amh_value_store.object->objects_mem = smart_calloc(Amh_Standard_Value_Mem, src_class_desc->filed_symbol_table.object_symbol_num);
	if (!amh_object_mem.amh_value_store.object->objects_mem)return amh_object_mem;
	
	for (uint32_t i = 0; i < src_class_desc->filed_symbol_table.object_symbol_num; ++i) {
		switch (src_class_desc->objects_mem[i].amh_standard_type) {
		case E_Amh_Standard_Class_Object:
			amh_object_mem.amh_value_store.object->objects_mem[i].amh_standard_type = E_Amh_Standard_Class_Object;

			if (src_class_desc->objects_mem[i].amh_value_store.object) {
				amh_object_mem.amh_value_store.object->objects_mem[i].amh_value_store.object = smart_calloc(Amh_Symbol_Class_Object, 1);
				if (amh_object_mem.amh_value_store.object->objects_mem[i].amh_value_store.object) {
					amh_object_mem.amh_value_store.object->objects_mem[i].amh_value_store.object->instance_base_class = src_class_desc->objects_mem[i].amh_value_store.object->instance_base_class;
				}
			}
			break;
		default:
			break;
		}
	}
	return amh_object_mem;
}
void test_instance_to_objects_symbol(Amh_Symbol_Table_Base* src_symbol_table) {

}
Amh_Symbol_Table* seek_to_table_symbols(Amh_Symbol_Table_Base* src_symbol_table, const char* src_iden, int symbol_table_id) {
	Amh_Symbol_Table* virtual_symbol_table;
	if (symbol_table_id == SEEK_TABLE_SYMBOL_FUNCTION_NUMBER)virtual_symbol_table = src_symbol_table->func_desc;
	else if (symbol_table_id == SEEK_TABLE_SYMBOL_CLASS_NUMBER)virtual_symbol_table = src_symbol_table->class_desc;
	else virtual_symbol_table = src_symbol_table->objects;
	while (virtual_symbol_table) {
		if (simple_strcmp(src_iden, virtual_symbol_table->name))return virtual_symbol_table;
		virtual_symbol_table = virtual_symbol_table->next_symbol;
	}
	return NULL;
}
Amh_Symbol_Table* get_registered_function_desc(Amh_Intermediate_Run_Host_Data * src_sys_run_host, const char * src_iden) {
	Amh_Symbol_Table_Datas* now_symbol_tables = acc_symbol_tables(src_sys_run_host);
	while (now_symbol_tables) {
		Amh_Symbol_Table* function_symbol_table = seek_to_table_symbols(&now_symbol_tables->symbol_table, src_iden, SEEK_TABLE_SYMBOL_FUNCTION_NUMBER);

		if (function_symbol_table)return function_symbol_table;
		now_symbol_tables = now_symbol_tables->next_top_nest;
	}
	return NULL;
}
void set_instance_class_object(Amh_Symbol_Class_Object* instance_mem, Amh_Class_Desc* src_class_desc) {
	instance_mem->instance_base_class = src_class_desc;
	for (uint32_t i = 0; i < src_class_desc->filed_symbol_table.object_symbol_num; ++i) {

	}
}
void regist_to_new_objects_symbol(Amh_Symbol_Table_Base* src_symbol_table, Amh_Symbol_Class_Object* src_class_symbol_table, const char* src_iden) {
	Amh_Symbol_Table* objects_symbol_table = seek_to_table_symbols(src_symbol_table, src_iden, SEEK_TABLE_SYMBOL_OBJECT_NUMBER);
	if (objects_symbol_table) {
		/*error*/
		return;
	}
	
}

Amh_Class_Desc * regist_new_class_desc_symbol(Amh_Symbol_Table_Base* src_symbol_table, const char* src_iden) {
	/*shadowing ok*/
	Amh_Symbol_Table* class_symbol_table = seek_to_table_symbols(src_symbol_table, src_iden, SEEK_TABLE_SYMBOL_CLASS_NUMBER);
	if (class_symbol_table) {
		/*error*/
		return NULL;
	}
	/*don't found function desc*/
	/*gen new class desc symbol table*/
	Amh_Symbol_Table* new_class_desc_table = smart_calloc(Amh_Symbol_Table, 1);

	if (!new_class_desc_table)return NULL;
	/*alloc and dup class name*/
	new_class_desc_table->name = simple_strdup(src_iden);
	if (!new_class_desc_table->name) {
		free(new_class_desc_table);
		return NULL;
	}
	/*extends to class desc data list*/
	Amh_Class_Desc * new_class_desc_mem = smart_realloc(Amh_Class_Desc, src_symbol_table->class_descs_mem, src_symbol_table->class_desc_symbol_num + 1);
	if (!new_class_desc_mem) {
		free(new_class_desc_table->name);
		free(new_class_desc_table);
		return NULL;
	}
	src_symbol_table->class_descs_mem = new_class_desc_mem;
	/*regist to symbol id*/
	new_class_desc_table->symbol_id = src_symbol_table->class_desc_symbol_num;
	/*now symbol in next symbol*/
	new_class_desc_table->next_symbol = src_symbol_table->class_desc;
	src_symbol_table->class_desc = new_class_desc_table;
	/*increment id*/
	src_symbol_table->class_desc_symbol_num++;

	return &src_symbol_table->class_descs_mem[src_symbol_table->class_desc->symbol_id];
}
void regist_to_new_objects_desc_symbol(Amh_Symbol_Table_Base* src_symbol_table, const char* src_iden, Amh_Standard_Value_Mem* src_value_mem) {

}
void regist_to_new_function_desc_symbol(Amh_Symbol_Table_Base * src_symbol_table, const char* src_iden, void * function_ctx) {
	/*shadowing OK*/
	Amh_Symbol_Table* function_symbol_table = seek_to_table_symbols(src_symbol_table, src_iden, SEEK_TABLE_SYMBOL_FUNCTION_NUMBER);
	if (function_symbol_table) {
		/*error*/
		return;
	}
	/*don't found function desc*/
	Amh_Symbol_Table* new_function_desc_table = smart_calloc(Amh_Symbol_Table, 1);

	if (!new_function_desc_table)return;
	new_function_desc_table->name = simple_strdup(src_iden);
	if (!new_function_desc_table->name) {
		free(new_function_desc_table);
		return;
	}
	void ** new_function_desc_mem = smart_realloc(void*, src_symbol_table->func_descs_mem, src_symbol_table->func_desc_symbol_num + 1);
	if (!new_function_desc_mem) {
		free(new_function_desc_table->name);
		free(new_function_desc_table);
		return;
	}
	src_symbol_table->func_descs_mem = new_function_desc_mem;
	
	new_function_desc_table->symbol_id = src_symbol_table->func_desc_symbol_num;
	new_function_desc_table->next_symbol = src_symbol_table->func_desc;

	src_symbol_table->func_desc = new_function_desc_table;
	src_symbol_table->func_descs_mem[src_symbol_table->func_desc_symbol_num] = function_ctx;
	src_symbol_table->func_desc_symbol_num++;
}


void regist_to_new_iden_symbol(Amh_Intermediate_Run_Host_Data * src_sys_run_host, const char * src_iden) {

}
Amh_Standard_Value_Mem amh_standard_putln(Amh_Standard_Value_Mem * args, int argc, void * amh_comtext) {
	if (args->amh_standard_type != E_Amh_Standard_String)return gen_standard_v_em();
	/*only string and argc is 1*/
	printf("%s", args->amh_value_store.str);
	putchar('\n');
	return gen_standard_v_em();
}
Amh_Standard_Value_Mem gen_standard_v_em() {
	Amh_Standard_Value_Mem v_em_value;
	v_em_value.amh_standard_type = E_Amh_Standard_Virtuals_Empty;
	v_em_value.amh_value_store.virtuals = NULL;
	return v_em_value;

}
void regist_amh_method_to_object() {

}
