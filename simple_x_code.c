#include <stdio.h>
#include "lexser_main_amh.h"
int main(void) {
	Amh_Lex_Token_List * tmp = start_read_amh_lex_main("test.amh");
	amh_token_list_call(tmp);
	delete_amh_token_list(&tmp);
	return 0;

}
