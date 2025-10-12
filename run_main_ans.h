#ifndef _RUN_MAIN_ANS_H_
#define _RUN_MAIN_ANS_H_
#include "ir_main_ans.h"
#define run_ans_si_strap static inline
#define CONST_MEMORY true
#define NONE_CONST_MEMORY false
#define OBJECT_SEEK_ID 0x0
#define FUNC_SEEK_ID 0x1
typedef enum Ans_Standard_Type {
	E_Ans_Standard_None,
	E_Ans_Standard_Virtual,
	E_Ans_Standard_Bool,
	E_Ans_Standard_Char,
	E_Ans_Standard_Int,
	E_Ans_Standard_Float,
	E_Ans_Standard_Double,
	E_Ans_Standard_String,
	E_Ans_Standard_U_Binary,
}Ans_Standard_Type;
typedef struct Ans_Value_Memory {
	Ans_Standard_Type ans_s_type;
	union {

		void* mem_none;
		void* mem_virtual;
		char mem_char;
		bool mem_bool;
		int mem_int;
		float mem_float;
		double mem_double;
		char* mem_string;
		char* mem_u_binary;
	}ans_data;
}Ans_Value_Memory;
typedef struct Ans_Stack_Memory {
	Ans_Value_Memory *ans_memory;
	bool const_memory_check;
	struct Ans_Stack_Memory *next_stack;
}Ans_Stack_Memory;
typedef struct Func_Decl_Mem {
	Ans_Bir_Chain* start_func_adress;
	Ans_Bir_Chain* end_func_adress;
}Func_Decl_Mem;
typedef struct Ans_Symbol_Table {
	char* ans_name;
	uint32_t symbol_id;
	struct Ans_Symbol_Table* next_symbol_data;
}Ans_Symbol_Table;
typedef struct Ans_Symbol_Table_Datas {
	Ans_Symbol_Table* ans_func;
	Ans_Symbol_Table* ans_obj;

	Ans_Value_Memory* abs_object_mem;
	uint32_t object_num;
	Func_Decl_Mem* func_decl;
	uint32_t func_num;
	
	struct Ans_Symbol_Table_Datas* next_top_host;
	const char* host_name;
}Ans_Symbol_Table_Datas;
typedef struct Ans_Intermediate_Run_Host_Data {
	Ans_Symbol_Table_Datas* symbol_datas;
	Ans_Bir_Chain* abir_chain;
	Ans_Stack_Memory* ans_stack_memory;
	struct Ans_Intermediate_Run_Host_Data* next_host_data;
}Ans_Intermediate_Run_Host_Data;
struct Ans_Symbol_Class_Object;
run_ans_si_strap Ans_Bir_Chain* acc_irh_to_ans_bir_chain(Ans_Intermediate_Run_Host_Data* src_irh_data) {
	return src_irh_data->abir_chain;
}
void start_ini_run_program(Ans_Bir_Chain* src_abirs);
void main_run_abir_programs(Ans_Intermediate_Run_Host_Data* src_irh_data);
void run_abir_program_process_task(Ans_Intermediate_Run_Host_Data * src_irh_data);
void pop_value_to_none_const_memory(Ans_Intermediate_Run_Host_Data* src_irh_data, const char* src_pop_object_name);
void regist_symbol_table_to_object_symbol_table(Ans_Intermediate_Run_Host_Data* src_irh_data, const char* src_object_name);
Ans_Symbol_Table* seek_symbol_tables(Ans_Symbol_Table* src_symbol_table, const char* src_symbol_name);
void ans_virtual_write(Ans_Stack_Memory** src_stack_memory);
void gc_const_memory_value(Ans_Value_Memory* src_ans_memory);
void gc_discard_stack_memory(Ans_Stack_Memory** src_ans_stack_memory); 
void add_stack_memory(Ans_Stack_Memory** src_ans_stack_memory);

Ans_Stack_Memory* pop_tmp_stack_memory(Ans_Stack_Memory** src_stack_memory);
bool gen_and_push_new_stack_memory(Ans_Stack_Memory** src_stack_memory, Ans_Value_Memory* src_memory, bool src_const_mem_check);

void run_abir_const_push_stack_memory(Ans_Stack_Memory** ans_stack_memory, Ans_Bir_Chain* src_abir);
Ans_Intermediate_Run_Host_Data* gen_ans_irh_data(Ans_Bir_Chain* src_abir_chain);
void ans_get_virtual_stdout(Ans_Stack_Memory** src_stack_memory);
Ans_Value_Memory* gen_new_virtual_memory(const void* src_virtual_memory);
Ans_Value_Memory* gen_new_char_memory(const void* src_str_memory);
Ans_Value_Memory* gen_new_int_memory(const void* src_int_memory);
Ans_Value_Memory* gen_new_float_memory(const void* src_float_memory);
Ans_Value_Memory* gen_new_double_memory(const void* src_double_memory);
Ans_Value_Memory* gen_new_str_memory(const void* src_string_memory);
Ans_Value_Memory* gen_new_u_binary(const void* src_u_bin_memory);
#endif // !_RUN_MAIN_ANS_H_
