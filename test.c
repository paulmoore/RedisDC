// http://paulmoore.mit-license.org

#include "redisdc.h"

int main(int argc, char **argv) {
	printf("[main] entering\n");
	rdc_sem_t *rdc_sem = rdc_sem_init("test", 3, "127.0.0.1", 6379);
	printf("[main] rdc_sem_init returned\n");
	if (rdc_sem) {
		printf("[main] rdc_sem init ok\n");
		rdc_sem_wait(rdc_sem);
		printf("[main] rdc_sem wait 1 ok\n");
		rdc_sem_wait(rdc_sem);
		printf("[main] rdc_sem wait 2 ok\n");
		rdc_sem_wait(rdc_sem);
		printf("[main] rdc_sem wait 3 ok\n");
		rdc_sem_wait(rdc_sem);
		printf("[main] rdc_sem wait 4 ok\n");
		rdc_sem_destroy(rdc_sem);
		return 0;
	}
	printf("[main] rdc_sem init error\n");
	return 1;
}
