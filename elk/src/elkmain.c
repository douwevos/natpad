///*
// * elkmain.c
// *
// *  Created on: Mar 15, 2010
// */
//
//#include <stdio.h>
//#include <errno.h>
//#include <string.h>
//#include <sys/stat.h>
//
//
//#include <caterpillar.h>
//#include "elkmain.h"
//#include "elkiservice.h"
//#include "elkarguments.h"
//
//#include <logging/catlogdefs.h>
//#define CAT_LOG_LEVEL CAT_LOG_WARN
//#define CAT_LOG_CLAZZ "ElkMain"
//#include <logging/catlog.h>
//
//G_DEFINE_TYPE (ElkMain, elk_main, G_TYPE_OBJECT)
//
//static void _dispose(GObject *object);
//
//static void elk_main_class_init(ElkMainClass *clazz) {
//	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
//	object_class->dispose = _dispose;
//}
//
//static void elk_main_init(ElkMain *instance) {
//}
//
//static void _dispose(GObject *object) {
//	cat_log_detail("dispose:%p", object);
////	ElkMain *instance = ELK_MAIN(object);
//	cat_log_detail("end-dispose:%p", object);
//}
//
//
//
////ElkMain *elk_main_new(int argc, char **argv) {
////	ElkMain *result = g_object_new(ELK_TYPE_MAIN, NULL);
////	result->top_window = NULL;
////	result->argc = argc;
////	result->argv = argv;
////	result->elk_configuration_service = elk_configuration_service_new();
////	result->configuration_container = (ConFigurationContainer *) result->elk_configuration_service;
////	elk_configuration_service_load(result->elk_configuration_service);
////	return result;
////}
//
//
//
//
