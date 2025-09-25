#include "amh_host_pripro_manage.h"
#include "str_ext.h"
typedef enum Standard_Pripro_ID {
	E_Amh_Pripro_None,
	E_Amh_Pripro_Class,
	E_Amh_Pripro_Module,
	E_Amh_Pripro_Base,
	E_Amh_Pripro_Extends,
	E_Amh_Pripro_Package,
	E_Amh_Pripro_Acc,
	E_Amh_Pripro_Alb,
	E_Amh_Pripro_Run,
	E_Amh_Pripro_Imp_Run,
	E_Amh_Pripro_Imp_Run_Call,
}Standard_Pripri_ID;
enum Extends_Pripro_ID;



const char* standard_file_optional_pripro[] = {
	"class", "module", "module_base", "extends", NULL
};
const char* standard_dependence_pripro[] = {
	"package", "acc", "alb", NULL
};
const char* standard_run_pripro[] = {
	"run", "run_wcall", "imp_run", "imp_run_call", NULL
};
Standard_Pripri_ID standard_pripro_check(const char * src_pripro_str) {
	/*file_oprional*/
	const char** pripro_list_ptr = standard_file_optional_pripro;
	Standard_Pripri_ID ret_pripro_id = E_Amh_Pripro_None;
	while (pripro_list_ptr[ret_pripro_id]) {

	}
}
enum Extends_Pripro_ID extends_pripro_chack(const char* src_pripro_str) { return  0; }
const char * amh_class_pripro(const char * host_name) {
	return NULL;// strdup(host_name);
}
void amh_pripro_process() {

}
void amh_pripro_manage(const char *src_pripro_str) {
	if (!src_pripro_str);

}
