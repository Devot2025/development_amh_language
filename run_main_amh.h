#ifndef _RUN_MAIN_AMH_H_
#define _RUN_MAIN_AMH_H_
#include "parser_main_amh.h"
#define run_main_amh_is_strap static inline
#define SEEK_TABLE_SYMBOL_FUNCTION_NUMBER 0
#define SEEK_TABLE_SYMBOL_CLASS_NUMBER 1
#define SEEK_TABLE_SYMBOL_OBJECT_NUMBER 2
typedef enum Amh_Run_Mode {
	E_Amh_Normal_Run,
	E_Amh_Normal_Run_Err_Write,
	E_Amh_Imp_Run,
	E_Amh_Imp_Run_Call,
}Amh_Run_Mode;
typedef struct Abstract_Host_Name {
	const char* now_host_name;/*file_name == abstract host*/
	Amh_Run_Mode host_run_mode;
}Abstract_Host_Name;

typedef enum Amh_Standard_Type{
	E_Amh_Standard_None,
	E_Amh_Standard_Virtuals,
	E_Amh_Standard_Virtuals_Empty,
	E_Amh_Standard_Char,
	E_Amh_Standard_Int,
	E_Amh_Standard_Float,
	E_Amh_Standard_Double,
	E_Amh_Standard_String,
	E_Amh_Standard_Array,
	E_Amh_Standard_Class_Object,
}Amh_Standard_Type;

struct Amh_Symbol_Class_Object;
typedef struct Amh_Standard_Value_Mem {
	Amh_Standard_Type amh_standard_type;
	union {
		char c_value;
		int i_value;
		float f_value;
		double d_value;
		char* str;
		void* array;
		void* virtuals;
		struct Amh_Symbol_Class_Object* object;
	}amh_value_store;
}Amh_Standard_Value_Mem;

typedef Amh_Standard_Value_Mem(* amh_base_func) (Amh_Standard_Value_Mem* args, int argc, void* base_context);



typedef struct Amh_Iden_Table_Store {
	uint32_t iden_index;
	Amh_Standard_Value_Mem* object_mem;
}Amh_Iden_Table_Store;


typedef enum Amh_Symbol_Iden_Type{
	E_Amh_Symbol_Func_Desc,
	E_Amh_Symbol_Class_Desc,
	E_Amh_Symbol_Instance_Objects,
}Amh_Symbol_Iden_Type;
typedef struct Amh_Symbol_Store {
	uint32_t now_symbol_num;
	Amh_Standard_Value_Mem* objects_mem;
}Amh_Symbol_Store;
typedef struct Amh_Symbol_Table {
	char* name;
	uint32_t symbol_id;
	struct Amh_Symbol_Table* next_symbol;
}Amh_Symbol_Table;

typedef struct Amh_Symbol_Table_Base {
	Amh_Symbol_Table* func_desc;
	Amh_Symbol_Table* class_desc;
	Amh_Symbol_Table* objects;
	uint32_t func_desc_symbol_num;
	uint32_t class_desc_symbol_num;
	uint32_t object_symbol_num;
	void** func_descs_mem;
	struct Amh_Class_Desc* class_descs_mem;
}Amh_Symbol_Table_Base;

typedef struct Amh_Class_Desc {
	Amh_Symbol_Table_Base filed_symbol_table;
	Amh_Standard_Value_Mem* objects_mem;
}Amh_Class_Desc;

typedef struct Amh_Symbol_Class_Object {
	Amh_Class_Desc* instance_base_class;
	Amh_Standard_Value_Mem* objects_mem;
}Amh_Symbol_Class_Object;

typedef struct Amh_Symbol_Table_Datas {
	Amh_Standard_Value_Mem* objects_mem;
	Amh_Symbol_Table_Base symbol_table;
	struct Amh_Symbol_Table_Datas* next_top_nest;
}Amh_Symbol_Table_Datas;

typedef struct Amh_Intermediate_Run_Host_Data {
	Abstract_Host_Name abstract_host_name;
	/*ast node*/
	Amh_Ast_Nodes* amh_ast_node;
	/*synbol datas */
	Amh_Symbol_Table_Datas* symbol_table_datas;
	/*top abstract host code*/
	struct Amh_Intermediate_Run_Host_Data* parent_host;
}Amh_Intermediate_Run_Host_Data;

run_main_amh_is_strap Amh_Symbol_Table* acc_symbol_function(Amh_Intermediate_Run_Host_Data* src_sys_run_host) {
	return NULL;//src_sys_run_host->symbol_table_datas->func_desc;
}

run_main_amh_is_strap Amh_Symbol_Table_Datas* acc_symbol_tables(Amh_Intermediate_Run_Host_Data* src_sys_run_host) {
	return src_sys_run_host->symbol_table_datas;
}
Amh_Symbol_Table* get_registered_function_desc(Amh_Intermediate_Run_Host_Data* src_sys_run_host, const char* src_iden);
Amh_Class_Desc* regist_new_class_desc_symbol(Amh_Symbol_Table_Base* src_symbol_table, const char* src_iden);
run_main_amh_is_strap Amh_Intermediate_Run_Host_Data* gen_run_host_data() {return smart_calloc(Amh_Intermediate_Run_Host_Data, 1);}
void regist_to_new_function_desc_symbol(Amh_Symbol_Table_Base* src_symbol_table, const char* src_iden, void* function_ctx);
void start_amh_run(Amh_Ast_Nodes * src_amh_nodes);
void regist_object_to_symbol_table(Amh_Symbol_Table_Base* src_symbol_table, Amh_Standard_Value_Mem** dst_value_mem, const char* src_iden, Amh_Standard_Value_Mem src_value);
Amh_Standard_Value_Mem instance_class_desc_to_object(Amh_Class_Desc* src_class_desc);
void set_class_desc_to_member_class_instnace_symbol(Amh_Class_Desc* src_class_desc, const char* src_iden, Amh_Class_Desc* base_instance_class_objects);
Amh_Symbol_Table* seek_to_table_symbols(Amh_Symbol_Table_Base* src_symbol_table, const char* src_iden, int symbol_table_id);
run_main_amh_is_strap void set_class_desc_to_method_symbol_desc(Amh_Class_Desc* src_class_desc, const char* src_iden, void* function_ctx) {
	if (!src_class_desc)return;
	regist_to_new_function_desc_symbol(&src_class_desc->filed_symbol_table, src_iden, function_ctx);
}
Amh_Standard_Value_Mem gen_standard_v_em();
#endif // !_RUN_MAIN_AMH_H_
