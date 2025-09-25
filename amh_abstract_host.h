#ifndef _AMH_ABSTRACT_HOST_H_
#define _AMH_ABSTRACT_HOST_H_
#include <stdint.h>
typedef struct Amh_Abstract_Host{
	char** abstract_host_list;
	uint32_t host_num;
	uint32_t host_size;
	char run_mode;
};
#endif