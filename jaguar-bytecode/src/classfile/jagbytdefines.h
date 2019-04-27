/*
   File:    jagbytdefines.h
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Mar 7, 2012
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2012 Douwe Vos.

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

#ifndef JAGBYTDEFINES_H_
#define JAGBYTDEFINES_H_

#define JAG_ACC_PUBLIC        0x0001
#define JAG_ACC_PRIVATE       0x0002
#define JAG_ACC_PROTECTED     0x0004
#define JAG_ACC_STATIC        0x0008
#define JAG_ACC_FINAL         0x0010
#define JAG_ACC_SUPER         0x0020
#define JAG_ACC_SYNCHRONIZED  0x0020
#define JAG_ACC_VOLATILE      0x0040
#define JAG_ACC_TRANSIENT     0x0080
#define JAG_ACC_NATIVE        0x0100
#define JAG_ACC_INTERFACE     0x0200
#define JAG_ACC_ABSTRACT      0x0400
#define JAG_ACC_STRICT        0x0800
#define JAG_ACC_SYNTHETIC     0x1000
#define JAG_ACC_ANNOTATION    0x2000
#define JAG_ACC_ENUM          0x4000

#endif /* JAGBYTDEFINES_H_ */
