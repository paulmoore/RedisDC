// http://paulmoore.mit-license.org

#include "rdc_sem.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>

static void rdc_sem_create(rdc_sem_t *rdc_sem, unsigned int initial);

static void rdc_sem_free(rdc_sem_t *rdc_sem);

rdc_sem_t *rdc_sem_init(const char *name, unsigned int initial, const char *ip, int port)
{
	redisContext *redis = redisConnect(ip, port);
	rdc_sem_t *rdc_sem = NULL;
	if (redis) {
		if (redis->err) {
			// TODO: handle error
			redisFree(redis);
		} else {
			rdc_sem = (rdc_sem_t *)malloc(sizeof(rdc_sem_t));
			rdc_sem->redis = redis;
			rdc_sem->key = (char *)malloc(sizeof(char) * (strlen(RDC_SEM_KEY_PREFIX) + strlen(name) + 1));
			sprintf(rdc_sem->key, "%s%s", RDC_SEM_KEY_PREFIX, name);
			// TODO: check that rdc_sem_create was successful
			rdc_sem_create(rdc_sem, initial);
		}
	} else {
		// TODO: handle error
	}
	return rdc_sem;
}

void rdc_sem_signal(rdc_sem_t *rdc_sem)
{
	// TODO: argument check
	redisReply *reply = (redisReply *)redisCommand(rdc_sem->redis, "LPUSH %s 1", rdc_sem->key);
	if (reply) {
		freeReplyObject(reply);
	} else {
		// TODO: handle error
	}
}

void rdc_sem_signal_n(rdc_sem_t *rdc_sem, unsigned int n)
{
	// TODO: argument check
	redisAppendCommand(rdc_sem->redis, "MULTI");
	unsigned int i;
	for (i = 0; i < n; i++) {
		redisAppendCommand(rdc_sem->redis, "LPUSH %s 1", rdc_sem->key);
	}
	redisAppendCommand(rdc_sem->redis, "EXEC");
	for (i = 0; i < n + 2; i++) {
		if (redisGetReply(rdc_sem->redis, NULL) == REDIS_OK) {
			// TODO: check the reply object for error
		} else {
			// TODO: handle the error, recreate connection
		}
	}
}

void rdc_sem_wait(rdc_sem_t *rdc_sem)
{
	rdc_sem_wait_timeout(rdc_sem, 0);
}

void rdc_sem_wait_timeout(rdc_sem_t *rdc_sem, unsigned int timeout)
{
	redisReply *reply = (redisReply *)redisCommand(rdc_sem->redis, "BLPOP %s %u", rdc_sem->key, timeout);
	if (reply) {
		freeReplyObject(reply);
	} else {
		// TODO: handle error
	}
}

void rdc_sem_destroy(rdc_sem_t *rdc_sem) {
	// TODO: argument check
	redisReply *reply = (redisReply *)redisCommand(rdc_sem->redis, "DEL %s", rdc_sem->key);
	// TODO: handle error
	freeReplyObject(reply);
	rdc_sem_free(rdc_sem);
}

void rdc_sem_create(rdc_sem_t *rdc_sem, unsigned int initial)
{
	// TODO: argument check
	// TODO: check if the semaphore was already created
	rdc_sem_signal_n(rdc_sem, initial);
}

void rdc_sem_free(rdc_sem_t *rdc_sem)
{
	// TODO: argument check
	redisFree(rdc_sem->redis);
	free(rdc_sem->key);
	free(rdc_sem);
}
