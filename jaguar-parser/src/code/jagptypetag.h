/*
   File:    jagptypetag.h
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    May 7, 2017
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2017 Douwe Vos.

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

#ifndef CODE_JAGPTYPETAG_H_
#define CODE_JAGPTYPETAG_H_

typedef enum _JagPTypeTag JagPTypeTag;

enum _JagPTypeTag {
    /** The tag of the basic type `byte'.
     */
    JAGP_TT_BYTE,

    /** The tag of the basic type `char'.
     */
	JAGP_TT_CHAR,

    /** The tag of the basic type `short'.
     */
	JAGP_TT_SHORT,

    /** The tag of the basic type `long'.
     */
	JAGP_TT_LONG,

    /** The tag of the basic type `float'.
     */
	JAGP_TT_FLOAT,
    /** The tag of the basic type `int'.
     */
	JAGP_TT_INT,
    /** The tag of the basic type `double'.
     */
	JAGP_TT_DOUBLE,
    /** The tag of the basic type `boolean'.
     */
	JAGP_TT_BOOLEAN,

    /** The tag of the type `void'.
     */
	JAGP_TT_VOID,

    /** The tag of all class and interface types.
     */
	JAGP_TT_CLASS,

    /** The tag of all array types.
     */
	JAGP_TT_ARRAY,

    /** The tag of all (monomorphic) method types.
     */
	JAGP_TT_METHOD,

    /** The tag of all package "types".
     */
	JAGP_TT_PACKAGE,

    /** The tag of all module "types".
     */
	JAGP_TT_MODULE,

    /** The tag of all (source-level) type variables.
     */
	JAGP_TT_TYPEVAR,

    /** The tag of all type arguments.
     */
	JAGP_TT_WILDCARD,

    /** The tag of all polymorphic (method-) types.
     */
	JAGP_TT_FORALL,

    /** The tag of deferred expression types in method context
     */
	JAGP_TT_DEFERRED,

    /** The tag of the bottom type {@code <null>}.
     */
	JAGP_TT_BOT,

    /** The tag of a missing type.
     */
	JAGP_TT_NONE,

    /** The tag of the error type.
     */
	JAGP_TT_ERROR,

    /** The tag of an unknown type
     */
	JAGP_TT_UNKNOWN,

    /** The tag of all instantiatable type variables.
     */
	JAGP_TT_UNDETVAR,

    /** Pseudo-types, these are special tags
     */
	JAGP_TT_UNINITIALIZED_THIS,

	JAGP_TT_UNINITIALIZED_OBJECT
};



#endif /* CODE_JAGPTYPETAG_H_ */
