/*
 * catlockut.c
 *
 *  Created on: Aug 21, 2010
 *      Author: superman
 */

#include "catlockut.h"
#include "../src/catlib.h"
#include "../src/concurrent/catlock.h"

#include "../src/logging/catlogdefs.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#include "../src/logging/catlog.h"



void test_lock(CatTestContext *context) {

	CatLock *lock = cat_lock_new();

	cat_lock_lock(lock);

	cat_lock_wait_timed(lock, 2000l);

	cat_lock_unlock(lock);
	cat_unref_ptr(lock);
}
