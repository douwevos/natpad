/*
   File:    caterpillartest.c
   Project: Natpad
   Author:  Douwe Vos
   Date:    Aug 19, 2010
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2008 Douwe Vos.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */


#include <glib-object.h>
#include "cattestcontext.h"
#include "catregexput.h"
#include "catbitarraywout.h"
#include "catarraywout.h"
#include "catstringstatic.h"
#include "catstringwout.h"
#include "catlockut.h"
#include "catfixedhashmaput.h"
#include "cathashmaput.h"
#include "../src/catlib.h"
#include "../src/profiling/catstacktracer.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv) {
	printf("starting caterpillar-test\n");
	CatTestContext context;
	test_string(&context);
	test_regexp(&context);
	test_bit_array(&context);
	test_array_wo(&context);
	test_lock(&context);
//	test_hash_map(&context);
//	test_fixed_hash_map(&context);



#ifdef CAT_ENABLE_STACK_TRACING
	CatStackTracer *tracer = cat_stack_tracer_get_or_create();
	CatStackTrace *trace = cat_stack_tracer_create_trace(tracer, -1, -1);
	cat_stack_trace_dump(trace);
#endif

	cat_ref_report();
	cat_ref_summary();
	cat_ref_cleanup();

	cat_ref_entry_report();


	printf("done\n");
	return 0;
}
