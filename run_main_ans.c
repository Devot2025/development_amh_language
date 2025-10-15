#include "run_main_ans.h"

void start_ini_run_program(Ans_Bir_Chain * src_abirs) {
	if (!src_abirs)return;
	Ans_Intermediate_Run_Host_Data* ans_irh_data = gen_ans_irh_data(src_abirs);
	ans_irh_data->symbol_datas = smart_calloc(Ans_Symbol_Table_Datas, 1);
	main_run_abir_programs(ans_irh_data);
}
/**
* $winホストを設定
* winホストがスタート
* TEST.ansをロード
* ホストを設定する
* TESTを実行する
* 上のホストに返す
* winの実行
**/
void main_run_abir_programs(Ans_Intermediate_Run_Host_Data* src_irh_data) {

	while (src_irh_data->abir_chain) {
		run_abir_program_process_task(src_irh_data);
		src_irh_data->abir_chain = src_irh_data->abir_chain->next_bir;
	}
}

void run_abir_program_process_task(Ans_Intermediate_Run_Host_Data * src_irh_data) {
	Ans_Abir_Instruction abir_instruction = src_irh_data->abir_chain->abir_instruction;
	printf("%-20s", debug_abir_string[src_irh_data->abir_chain->abir_instruction] + AST_IR_STR_OUT_POS);
	char break_stack[256];
	if (!fgets(break_stack, GET_ARRAY_SIZE(break_stack), stdin))return;
	if(abir_instruction == E_Abir_Instruction_A_Pop){
		pop_value_to_none_const_memory(src_irh_data, src_irh_data->abir_chain->a_data.v_data);
	}
	else if (abir_instruction == E_Abir_Instruction_A_Virtual_Write) {
		ans_virtual_write(&src_irh_data->ans_stack_memory);
	}
	else if (abir_instruction == E_Abir_Instruction_A_Virtual_Stdout) {
		ans_get_virtual_stdout(&src_irh_data->ans_stack_memory);
	}
	else if (abir_instruction == E_Abir_Instruction_A_Push) {
	}
	else if (abir_instruction == E_Abir_Instruction_A_Const_Push) {
		run_abir_const_push_stack_memory(&src_irh_data->ans_stack_memory, src_irh_data->abir_chain);

	}
	else if (abir_instruction == E_Abir_Instruction_A_Add) {
		add_stack_memory(&src_irh_data->ans_stack_memory);

	}
	else if (abir_instruction == E_Abir_Instruction_A_Discard) {
		gc_discard_stack_memory(&src_irh_data->ans_stack_memory);
	}
	else if (abir_instruction == E_Abir_Instruction_A_Alloc_Mem) {
		regist_symbol_table_to_object_symbol_table(src_irh_data, (const char *)src_irh_data->abir_chain->a_data.v_data);

	}
	else if (abir_instruction == E_Abir_Instruction_A_Start_Func_Decl) {

	}
	else if (abir_instruction == E_Abir_Instruction_A_End_Func_Decl) {

	}
	else if (abir_instruction == E_Abir_Instruction_Set_Abstract_Host) {

	}
}
void pop_value_to_none_const_memory(Ans_Intermediate_Run_Host_Data * src_irh_data, const char * src_pop_object_name) {

	Ans_Symbol_Table* symbol_table = seek_symbol_tables(src_irh_data->symbol_datas->ans_obj, src_pop_object_name);
	if (!symbol_table) {
		return;
	}
	Ans_Stack_Memory* tmp_stack_memory = pop_tmp_stack_memory(&src_irh_data->ans_stack_memory);

	Ans_Value_Memory* object_memory = &src_irh_data->symbol_datas->abs_object_mem[symbol_table->symbol_id];
	if (object_memory->ans_s_type == E_Ans_Standard_Virtual) {
		/*deep memory*/
		object_memory->ans_data.mem_virtual = tmp_stack_memory->ans_memory->ans_data.mem_virtual;

	}
	if (object_memory->ans_s_type == E_Ans_Standard_Char) {
		object_memory->ans_data.mem_char = tmp_stack_memory->ans_memory->ans_data.mem_char;
	}
	else if (object_memory->ans_s_type == E_Ans_Standard_Int) {
		object_memory->ans_data.mem_int = tmp_stack_memory->ans_memory->ans_data.mem_int;
	}
	else if (object_memory->ans_s_type == E_Ans_Standard_Float) {
		object_memory->ans_data.mem_float = tmp_stack_memory->ans_memory->ans_data.mem_float;
	}
	else if (object_memory->ans_s_type == E_Ans_Standard_Double) {
		object_memory->ans_data.mem_double = tmp_stack_memory->ans_memory->ans_data.mem_double;
	}
	else if (object_memory->ans_s_type == E_Ans_Standard_String) {
		object_memory->ans_data.mem_string = simple_strdup(tmp_stack_memory->ans_memory->ans_data.mem_string);
	}
	else if (object_memory->ans_s_type == E_Ans_Standard_U_Binary) {
		object_memory->ans_data.mem_u_binary = tmp_stack_memory->ans_memory->ans_data.mem_u_binary;
	}
	gc_discard_stack_memory(&tmp_stack_memory);
}
Ans_Stack_Memory* pop_tmp_stack_memory(Ans_Stack_Memory** src_stack_memory) {

	if (!src_stack_memory || !*src_stack_memory)return NULL;
	
	Ans_Stack_Memory* dst_tmp_stack_memory = (*src_stack_memory);
	(*src_stack_memory) = (*src_stack_memory)->next_stack;
	dst_tmp_stack_memory->next_stack = NULL;
	return dst_tmp_stack_memory;
}
void regist_symbol_table_to_object_symbol_table(Ans_Intermediate_Run_Host_Data* src_irh_data, const char* src_object_name) {
	if (!src_irh_data->symbol_datas) {
		assert(false);
		return;
	}
	if (seek_symbol_tables(src_irh_data->symbol_datas->ans_obj, src_object_name)) {
		return;
	}
	Ans_Symbol_Table* new_ans_symbol_table = smart_malloc(Ans_Symbol_Table, 1);
	if (!new_ans_symbol_table)return;
	new_ans_symbol_table->ans_name = simple_strdup(src_object_name);
	new_ans_symbol_table->symbol_id = src_irh_data->symbol_datas->object_num;
	new_ans_symbol_table->next_symbol_data = src_irh_data->symbol_datas->ans_obj;
	Ans_Value_Memory* new_value_mem = smart_realloc(Ans_Value_Memory, src_irh_data->symbol_datas->abs_object_mem, src_irh_data->symbol_datas->object_num + 1);
	if (!new_value_mem) {
		free(new_ans_symbol_table->ans_name);
		free(new_ans_symbol_table);
		return;
	}
	src_irh_data->symbol_datas->ans_obj = new_ans_symbol_table;
	src_irh_data->symbol_datas->abs_object_mem = new_value_mem;
	src_irh_data->symbol_datas->object_num++;

}
Ans_Symbol_Table* seek_symbol_tables(Ans_Symbol_Table* src_symbol_table, const char * src_symbol_name) {
	if (!src_symbol_name)return NULL;
	while (src_symbol_table) {
		if (simple_strcmp(src_symbol_table->ans_name, src_symbol_name))return src_symbol_table;
		src_symbol_table = src_symbol_table->next_symbol_data;
	}
	return NULL;
}
void add_stack_memory(Ans_Stack_Memory** src_ans_stack_memory) {
	Ans_Stack_Memory* pop_stack = pop_tmp_stack_memory(src_ans_stack_memory);;
	Ans_Stack_Memory* pop_stack_2 = pop_tmp_stack_memory(src_ans_stack_memory);

	if (!pop_stack || !pop_stack_2)return;
	if (pop_stack->ans_memory->ans_s_type == pop_stack_2->ans_memory->ans_s_type) {
		Ans_Value_Memory* new_value_mem = NULL;
		new_value_mem = smart_calloc(Ans_Value_Memory, 1);
		if (!new_value_mem)return;
		if (pop_stack->ans_memory->ans_s_type == E_Ans_Standard_Char) {
			new_value_mem->ans_data.mem_char = pop_stack->ans_memory->ans_data.mem_char + pop_stack_2->ans_memory->ans_data.mem_char;
		}
		else if (pop_stack->ans_memory->ans_s_type == E_Ans_Standard_Int) {
			new_value_mem->ans_data.mem_int = pop_stack->ans_memory->ans_data.mem_int + pop_stack_2->ans_memory->ans_data.mem_int;
		}
		else if (pop_stack->ans_memory->ans_s_type == E_Ans_Standard_Float) {
			new_value_mem->ans_data.mem_float = pop_stack->ans_memory->ans_data.mem_float + pop_stack_2->ans_memory->ans_data.mem_float;
		}
		else if (pop_stack->ans_memory->ans_s_type == E_Ans_Standard_Double) {
			new_value_mem->ans_data.mem_double = pop_stack->ans_memory->ans_data.mem_double + pop_stack_2->ans_memory->ans_data.mem_double;
		}
		else if (pop_stack->ans_memory->ans_s_type == E_Ans_Standard_Virtual) {
			new_value_mem->ans_s_type = E_Ans_Standard_Virtual;
		}
		else if (pop_stack->ans_memory->ans_s_type == E_Ans_Standard_String) {
			
			size_t str_len_1 = strlen(pop_stack->ans_memory->ans_data.mem_string);
			size_t str_len_2 = strlen(pop_stack_2->ans_memory->ans_data.mem_string);
			size_t all_str_len = str_len_1 + str_len_2 + 1;
			new_value_mem->ans_data.mem_string = smart_malloc(char, all_str_len);
			if (!new_value_mem->ans_data.mem_string)return;
			memcpy(new_value_mem->ans_data.mem_string, pop_stack->ans_memory->ans_data.mem_string, str_len_1);
			memcpy(new_value_mem->ans_data.mem_string + str_len_1, pop_stack->ans_memory->ans_data.mem_string, str_len_2);
			new_value_mem->ans_data.mem_string[all_str_len] = '\0';
		}

		gc_discard_stack_memory(&pop_stack);
		gc_discard_stack_memory(&pop_stack_2);
		gen_and_push_new_stack_memory(src_ans_stack_memory, new_value_mem, CONST_MEMORY);
	}
	else {

		Ans_Stack_Memory* strength_memory = pop_stack->ans_memory->ans_s_type > pop_stack_2->ans_memory->ans_s_type ? pop_stack : pop_stack_2;
		Ans_Stack_Memory* weekness_memory = pop_stack->ans_memory->ans_s_type > pop_stack_2->ans_memory->ans_s_type ? pop_stack_2 : pop_stack;
		Ans_Value_Memory* new_value_mem = smart_calloc(Ans_Value_Memory, 1);
		if (!new_value_mem)return;
		new_value_mem->ans_s_type = strength_memory->ans_memory->ans_s_type;

		if (strength_memory->ans_memory->ans_s_type == E_Ans_Standard_Char) {
			new_value_mem->ans_data.mem_char = strength_memory->ans_memory->ans_data.mem_char;
		}
		else if (strength_memory->ans_memory->ans_s_type == E_Ans_Standard_Int) {
			if (weekness_memory->ans_memory->ans_s_type == E_Ans_Standard_Char)
				new_value_mem->ans_data.mem_int = strength_memory->ans_memory->ans_data.mem_int + (int)weekness_memory->ans_memory->ans_data.mem_char;
			else if(weekness_memory->ans_memory->ans_s_type == E_Ans_Standard_Virtual)
				new_value_mem->ans_data.mem_int = strength_memory->ans_memory->ans_data.mem_int;
		}
		else if (strength_memory->ans_memory->ans_s_type == E_Ans_Standard_Float) {
			if (weekness_memory->ans_memory->ans_s_type == E_Ans_Standard_Char)
				new_value_mem->ans_data.mem_float = strength_memory->ans_memory->ans_data.mem_float + (float)weekness_memory->ans_memory->ans_data.mem_char;
			else if (weekness_memory->ans_memory->ans_s_type == E_Ans_Standard_Int)
				new_value_mem->ans_data.mem_float = strength_memory->ans_memory->ans_data.mem_float + (float)weekness_memory->ans_memory->ans_data.mem_int;
			else if (weekness_memory->ans_memory->ans_s_type == E_Ans_Standard_Virtual)
				new_value_mem->ans_data.mem_float = strength_memory->ans_memory->ans_data.mem_float;
		}
		else if (strength_memory->ans_memory->ans_s_type == E_Ans_Standard_Double) {
			if (weekness_memory->ans_memory->ans_s_type == E_Ans_Standard_Char)
				new_value_mem->ans_data.mem_double = strength_memory->ans_memory->ans_data.mem_double + (double)weekness_memory->ans_memory->ans_data.mem_char;
			else if (weekness_memory->ans_memory->ans_s_type == E_Ans_Standard_Int)
				new_value_mem->ans_data.mem_double = strength_memory->ans_memory->ans_data.mem_double + (double)weekness_memory->ans_memory->ans_data.mem_int;
			else if (weekness_memory->ans_memory->ans_s_type == E_Ans_Standard_Float)
				new_value_mem->ans_data.mem_double = strength_memory->ans_memory->ans_data.mem_double + (double)weekness_memory->ans_memory->ans_data.mem_float;
			else if (weekness_memory->ans_memory->ans_s_type == E_Ans_Standard_Virtual)
				new_value_mem->ans_data.mem_double = strength_memory->ans_memory->ans_data.mem_double;
		}
		else if (strength_memory->ans_memory->ans_s_type == E_Ans_Standard_String) {

			if (strength_memory == pop_stack) {

				if (weekness_memory->ans_memory->ans_s_type == E_Ans_Standard_Char) {
					new_value_mem->ans_data.mem_string = marge_value_and_str_to_str(pop_stack->ans_memory->ans_data.mem_string, &pop_stack_2->ans_memory->ans_data.mem_char, 0x0);
				}
				else if (weekness_memory->ans_memory->ans_s_type == E_Ans_Standard_Int) {
					new_value_mem->ans_data.mem_string = marge_value_and_str_to_str(pop_stack->ans_memory->ans_data.mem_string, &pop_stack_2->ans_memory->ans_data.mem_int, 0x1);

				}

				else if (weekness_memory->ans_memory->ans_s_type == E_Ans_Standard_Float) {
					new_value_mem->ans_data.mem_string = marge_value_and_str_to_str(pop_stack->ans_memory->ans_data.mem_string, &pop_stack_2->ans_memory->ans_data.mem_float, 0x2);

				}
				else if (weekness_memory->ans_memory->ans_s_type == E_Ans_Standard_Double) {
					new_value_mem->ans_data.mem_string = marge_value_and_str_to_str(pop_stack->ans_memory->ans_data.mem_string, &pop_stack_2->ans_memory->ans_data.mem_double, 0x3);

				}
			}
			else {

				if (weekness_memory->ans_memory->ans_s_type == E_Ans_Standard_Char) {
					new_value_mem->ans_data.mem_string = marge_str_and_value_to_str(pop_stack_2->ans_memory->ans_data.mem_string, &pop_stack->ans_memory->ans_data.mem_char, 0x0);
				}
				else if (weekness_memory->ans_memory->ans_s_type == E_Ans_Standard_Int) {

					new_value_mem->ans_data.mem_string = marge_str_and_value_to_str(pop_stack_2->ans_memory->ans_data.mem_string, &pop_stack->ans_memory->ans_data.mem_int, 0x1);
				}

				else if (weekness_memory->ans_memory->ans_s_type == E_Ans_Standard_Float) {
					new_value_mem->ans_data.mem_string = marge_str_and_value_to_str(pop_stack_2->ans_memory->ans_data.mem_string, &pop_stack->ans_memory->ans_data.mem_float, 0x2);

				}
				else if (weekness_memory->ans_memory->ans_s_type == E_Ans_Standard_Double) {
					new_value_mem->ans_data.mem_string = marge_str_and_value_to_str(pop_stack_2->ans_memory->ans_data.mem_string, &pop_stack->ans_memory->ans_data.mem_double, 0x3);

				}
			}


		}
		gc_discard_stack_memory(&pop_stack);
		gc_discard_stack_memory(&pop_stack_2);
		gen_and_push_new_stack_memory(src_ans_stack_memory, new_value_mem, CONST_MEMORY);
	}
}
void ans_get_virtual_stdout(Ans_Stack_Memory** src_stack_memory) {
	if(!src_stack_memory || *src_stack_memory) assert(false);
	Ans_Value_Memory* new_virtual_mem = gen_new_virtual_memory(stdout);
	gen_and_push_new_stack_memory(src_stack_memory, new_virtual_mem, NONE_CONST_MEMORY);
}
void ans_virtual_write(Ans_Stack_Memory** src_stack_memory) {
	/*args = stdout, out_string, -> stack memory null*/
	Ans_Stack_Memory* local_memory_src_str = pop_tmp_stack_memory(src_stack_memory);
	Ans_Stack_Memory* local_memory_src_stdout = pop_tmp_stack_memory(src_stack_memory);
	Ans_Stack_Memory* end_stack_memory = pop_tmp_stack_memory(src_stack_memory);

	int ret_stack_mem_int = 0;
	if (local_memory_src_stdout && local_memory_src_str && !end_stack_memory) {

		FILE* v_write_ptr = (FILE*)local_memory_src_stdout->ans_memory->ans_data.mem_virtual;
		if (local_memory_src_str->ans_memory->ans_s_type == E_Ans_Standard_String) {
			const char* out_string = local_memory_src_str->ans_memory->ans_data.mem_string;
			size_t str_num = simple_strlen(out_string);
			size_t ret = fwrite(out_string, str_num, sizeof(char), stdout);
			ret_stack_mem_int = ret != str_num ? 0 : 1;
		}
	}
	else {
		assert(false);
	}
	gc_discard_stack_memory(&local_memory_src_str);
	gc_discard_stack_memory(&local_memory_src_stdout);

	Ans_Value_Memory* new_int_memory = gen_new_int_memory(&ret_stack_mem_int);
	gen_and_push_new_stack_memory(src_stack_memory, new_int_memory, CONST_MEMORY);
}
Ans_Value_Memory* get_gc_discard_stack_memory(Ans_Stack_Memory ** src_stack_memory) {
	Ans_Value_Memory* dst_memory = (*src_stack_memory)->ans_memory;
	Ans_Stack_Memory* pop_stack_memory = (*src_stack_memory)->next_stack;

	return dst_memory;
}
Ans_Value_Memory* deep_copy_stack_memory(Ans_Stack_Memory* src_stack_memory) {
	return NULL;
}
void stack_memory_abs_checker(Ans_Stack_Memory* src_ans_stack_memory) {
	if (!src_ans_stack_memory)assert(false);
	Ans_Stack_Memory* pop_stack = src_ans_stack_memory->next_stack;
	Ans_Stack_Memory* pop_stack_2 = src_ans_stack_memory;
	if (!pop_stack)assert(false);
	if (pop_stack->ans_memory->ans_s_type < pop_stack_2->ans_memory->ans_s_type) {

	}
	else if (pop_stack->ans_memory->ans_s_type > pop_stack_2->ans_memory->ans_s_type) {

	}
	else return;
}
void gc_discard_stack_memory(Ans_Stack_Memory ** src_ans_stack_memory) {
	if (!src_ans_stack_memory)return;
	Ans_Stack_Memory* next_stack = (*src_ans_stack_memory)->next_stack;
	if ((*src_ans_stack_memory)->const_memory_check & CONST_MEMORY) gc_const_memory_value((*src_ans_stack_memory)->ans_memory);
	free(*src_ans_stack_memory);
	*src_ans_stack_memory = next_stack;
}
void gc_const_memory_value(Ans_Value_Memory* src_ans_memory) {
	if (!src_ans_memory)return;
	switch (src_ans_memory->ans_s_type) {
	case E_Ans_Standard_Int:
	case E_Ans_Standard_Char:
	case E_Ans_Standard_Float:
	case E_Ans_Standard_Double:
	case E_Ans_Standard_Virtual:
		free(src_ans_memory);
		break;
	case E_Ans_Standard_String:
		free(src_ans_memory->ans_data.mem_string);
		free(src_ans_memory);

		break;
	default:
		free(src_ans_memory);
	}
}
bool gen_and_push_new_stack_memory(Ans_Stack_Memory ** src_stack_memory, Ans_Value_Memory * src_memory, bool src_const_mem_check) {
	if (!src_stack_memory)return false;
	Ans_Stack_Memory* dst_memory = smart_malloc(Ans_Stack_Memory, 1);
	if (!dst_memory)return false;
	dst_memory->ans_memory = src_memory;
	dst_memory->const_memory_check = src_const_mem_check;
	dst_memory->next_stack = (*src_stack_memory);
	(*src_stack_memory) = dst_memory;
	return true;
}
Ans_Intermediate_Run_Host_Data* gen_ans_irh_data(Ans_Bir_Chain * src_abir_chain) {
	Ans_Intermediate_Run_Host_Data* ans_irh_data = smart_calloc(Ans_Intermediate_Run_Host_Data, 1);
	if (!ans_irh_data)return NULL;
	ans_irh_data->abir_chain = src_abir_chain;


	return ans_irh_data;
}
void run_abir_const_push_stack_memory(Ans_Stack_Memory ** ans_stack_memory, Ans_Bir_Chain * src_abir) {
	if (src_abir->a_data.abir_cdata_type == E_Abir_C_Char) {
		Ans_Value_Memory* char_value_memory = gen_new_char_memory(src_abir->a_data.v_data);
		gen_and_push_new_stack_memory(ans_stack_memory, char_value_memory, CONST_MEMORY);
	}
	if (src_abir->a_data.abir_cdata_type == E_Abir_C_Int) {
		Ans_Value_Memory* int_value_memory = gen_new_int_memory(src_abir->a_data.v_data);
		gen_and_push_new_stack_memory(ans_stack_memory, int_value_memory, CONST_MEMORY);
	}
	if (src_abir->a_data.abir_cdata_type == E_Abir_C_Float) {
		Ans_Value_Memory* float_value_memory = gen_new_float_memory(src_abir->a_data.v_data);
		gen_and_push_new_stack_memory(ans_stack_memory, float_value_memory, CONST_MEMORY);
	}
	if (src_abir->a_data.abir_cdata_type == E_Abir_C_Double) {
		Ans_Value_Memory* double_value_memory = gen_new_double_memory(src_abir->a_data.v_data);
		gen_and_push_new_stack_memory(ans_stack_memory, double_value_memory, CONST_MEMORY);
	}
	else if (src_abir->a_data.abir_cdata_type == E_Abir_C_Str) {
		Ans_Value_Memory* str_value_memory = gen_new_str_memory(src_abir->a_data.v_data);
		gen_and_push_new_stack_memory(ans_stack_memory, str_value_memory, CONST_MEMORY);
	}
}
Ans_Value_Memory* gen_new_virtual_memory(void* src_str_virtual_memory) {
	Ans_Value_Memory* char_memory = smart_malloc(Ans_Value_Memory, 1);
	if (!char_memory)return NULL;
	char_memory->ans_data.mem_virtual = src_str_virtual_memory;
	char_memory->ans_s_type = E_Ans_Standard_Virtual;
	return char_memory;
}
Ans_Value_Memory* gen_new_char_memory(void* src_str_memory) {

	Ans_Value_Memory* char_memory = smart_malloc(Ans_Value_Memory, 1);
	if (!char_memory)return NULL;
	char_memory->ans_data.mem_char = *(char*)(char_memory);
	char_memory->ans_s_type = E_Ans_Standard_Char;
	return char_memory;
}
Ans_Value_Memory* gen_new_int_memory(void* src_int_memory) {
	Ans_Value_Memory* int_memory = smart_malloc(Ans_Value_Memory, 1);
	if (!int_memory)return NULL;
	int_memory->ans_data.mem_int = *(int*)(src_int_memory);
	int_memory->ans_s_type = E_Ans_Standard_Int;
	return int_memory;
}

Ans_Value_Memory* gen_new_float_memory(void* src_float_memory) {
	Ans_Value_Memory* float_memory = smart_malloc(Ans_Value_Memory, 1);
	if (!float_memory)return NULL;
	float_memory->ans_data.mem_float = *(float*)(src_float_memory);
	float_memory->ans_s_type = E_Ans_Standard_Float;
	return float_memory;
}
Ans_Value_Memory* gen_new_double_memory(void* src_double_memory) {
	Ans_Value_Memory* double_memory = smart_malloc(Ans_Value_Memory, 1);
	if (!double_memory)return NULL;
	double_memory->ans_data.mem_double = *(double*)(src_double_memory);
	double_memory->ans_s_type = E_Ans_Standard_Double;
	return double_memory;
}


Ans_Value_Memory* gen_new_str_memory(void* src_string_memory) {

	Ans_Value_Memory* str_memory = smart_malloc(Ans_Value_Memory, 1);
	if (!str_memory)return NULL;
	str_memory->ans_data.mem_string = simple_strdup(src_string_memory);
	str_memory->ans_s_type = E_Ans_Standard_String;
	return str_memory;
}

Ans_Value_Memory* gen_new_u_binary(void* src_u_bin_memory) {

	Ans_Value_Memory* u_bin_memory = smart_malloc(Ans_Value_Memory, 1);
	if (!u_bin_memory)return NULL;
	u_bin_memory->ans_data.mem_u_binary = src_u_bin_memory;
	u_bin_memory->ans_s_type = E_Ans_Standard_U_Binary;
	return u_bin_memory;
}