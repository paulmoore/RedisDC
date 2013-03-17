// http://paulmoore.mit-license.org

#ifndef __RDC_SEM_H__
#define __RDC_SEM_H__

#include "hiredis.h"

#define RDC_SEM_KEY_PREFIX "rdc:sem:"

typedef struct {
	char *key;
	redisContext *redis;
} rdc_sem_t;

/**
 * Creates a new semaphore.
 *
 * @param name The name of this semaphore, must be unique from other semaphores in use.
 * @param initial The initial value of the semaphore.
 * @param ip The host of the Redis server to connect to.
 * @param port The port of the Redis server to connect to.
 * @return Returns a new semaphore object, or NULL on error.
 */
rdc_sem_t *rdc_sem_init(const char *name, unsigned int initial, const char *ip, int port);

/**
 * Signals the semaphore once.
 * Will unblock one node waiting on this semaphore, if any.
 *
 * @param rdc_sem The semaphore object to signal.
 */
void rdc_sem_signal(rdc_sem_t *rdc_sem);

/**
 * Signals the sempahore multiple times.
 * This operation is atomic.
 *
 * @param rdc_sem The semaphore object to signal.
 * @param n The number of times to signal the semaphore.
 */
void rdc_sem_signal_n(rdc_sem_t *rdc_sem, unsigned int n);

/**
 * Waits on the semaphore.
 * The result of this operation may cause it to block.
 * In that case, it will not unblock until the semaphore receives a signal from another node.
 * It is not guarenteed that this node will be the first to be unblocked.
 *
 * @param rdc_sem The semaphore object to wait on.
 */
void rdc_sem_wait(rdc_sem_t *rdc_sem);

/**
 * Waits on the semaphore until the semaphore is signaled and this node is chosen, or the operation times out.
 * See rdc_sem_wait().
 *
 * @param rdc_sem The semaphore object to wait on.
 * @param timeout The timeout, in seconds. If 0, the operation will not timeout if it is blocked.
 */
void rdc_sem_wait_timeout(rdc_sem_t *rdc_sem, unsigned int timeout);

/**
 * Destroys the semaphore.
 *
 * @param rdc_sem The semaphore object to destroy.
 */
void rdc_sem_destroy(rdc_sem_t *rdc_sem);

#endif
