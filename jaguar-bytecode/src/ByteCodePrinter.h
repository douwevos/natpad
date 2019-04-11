/*
 * ByteCodePrinter.h
 *
 *  Created on: Apr 10, 2019
 *      Author: dvos
 */

#ifndef CLASSFILE_BYTECODEPRINTER_H_
#define CLASSFILE_BYTECODEPRINTER_H_

#include <glib-object.h>
G_BEGIN_DECLS
#include "classfile/jagbytclassfile.h"
G_END_DECLS

#ifdef __cplusplus

class ByteCodePrinter {
public:
	ByteCodePrinter();
	virtual ~ByteCodePrinter();
	char *print(JagBytClassfile *classfile);
};

#endif

G_BEGIN_DECLS


struct _JagByteCodePrinter {};

typedef struct _JagByteCodePrinter JagByteCodePrinter;

JagByteCodePrinter *jag_byte_code_printer_new();

char *jag_byte_code_printer_print(JagByteCodePrinter *printer, JagBytClassfile *classfile);

G_END_DECLS


#endif /* CLASSFILE_BYTECODEPRINTER_H_ */
