/*
   File:    jagpflags.h
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    May 2, 2017
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

#ifndef CODE_JAGPFLAGS_H_
#define CODE_JAGPFLAGS_H_

/* Standard Java flags.
 */
#define JAGP_FLAG_PUBLIC         1
#define JAGP_FLAG_PRIVATE        1<<1
#define JAGP_FLAG_PROTECTED      1<<2
#define JAGP_FLAG_STATIC         1<<3
#define JAGP_FLAG_FINAL          1<<4
#define JAGP_FLAG_SYNCHRONIZED   1<<5
#define JAGP_FLAG_VOLATILE       1<<6
#define JAGP_FLAG_TRANSIENT      1<<7
#define JAGP_FLAG_NATIVE         1<<8
#define JAGP_FLAG_INTERFACE      1<<9
#define JAGP_FLAG_ABSTRACT       1<<10
#define JAGP_FLAG_STRICTFP       1<<11

/* Flag that marks a symbol synthetic, added in classfile v49.0. */
#define JAGP_FLAG_SYNTHETIC      1<<12

/** Flag that marks attribute interfaces, added in classfile v49.0. */
#define JAGP_FLAG_ANNOTATION     1<<13

/** An enumeration type or an enumeration constant, added in
 *  classfile v49.0. */
#define JAGP_FLAG_ENUM           1<<14

/** Added in SE8, represents constructs implicitly declared in source. */
#define JAGP_FLAG_MANDATED       1<<15

#define JAGP_FLAG_StandardFlags   0x0fff

// Because the following access flags are overloaded with other
// bit positions, we translate them when reading and writing class
// files into unique bits positions: ACC_SYNTHETIC <-> SYNTHETIC,
// for example.
#define JAGP_FLAG_ACC_SUPER      0x0020
#define JAGP_FLAG_ACC_BRIDGE     0x0040
#define JAGP_FLAG_ACC_VARARGS    0x0080
#define JAGP_FLAG_ACC_MODULE     0x8000

/*****************************************
 * Internal compiler flags (no bits in the lower 16).
 *****************************************/

/** Flag is set if symbol is deprecated.
 */
#define JAGP_FLAG_DEPRECATED     1<<17

/** Flag is set for a variable symbol if the variable's definition
 *  has an initializer part.
 */
#define JAGP_FLAG_HASINIT            1<<18

/** Flag is set for compiler-generated anonymous method symbols
 *  that `own' an initializer block.
 */
#define JAGP_FLAG_BLOCK              1<<20

/** Flag bit 21 is available. (used earlier to tag compiler-generated abstract methods that implement
 *  an interface method (Miranda methods)).
 */

/** Flag is set for nested classes that do not access instance members
 *  or `this' of an outer class and therefore don't need to be passed
 *  a this$n reference.  This value is currently set only for anonymous
 *  classes in superclass constructor calls and only for pre 1.4 targets.
 *  todo: use this value for optimizing away this$n parameters in
 *  other cases.
 */
#define JAGP_FLAG_NOOUTERTHIS    1<<22

/** Flag is set for package symbols if a package has a member or
 *  directory and therefore exists.
 */
#define JAGP_FLAG_EXISTS             1<<23

/** Flag is set for compiler-generated compound classes
 *  representing multiple variable bounds
 */
#define JAGP_FLAG_COMPOUND       1<<24

/** Flag is set for class symbols if a class file was found for this class.
 */
#define JAGP_FLAG_CLASS_SEEN     1<<25

/** Flag is set for class symbols if a source file was found for this
 *  class.
 */
#define JAGP_FLAG_SOURCE_SEEN    1<<26

/* State flags (are reset during compilation).
 */

/** Flag for class symbols is set and later re-set as a lock in
 *  Enter to detect cycles in the superclass/superinterface
 *  relations.  Similarly for constructor call cycle detection in
 *  Attr.
 */
#define JAGP_FLAG_LOCKED             1<<27

/** Flag for class symbols is set and later re-set to indicate that a class
 *  has been entered but has not yet been attributed.
 */
#define JAGP_FLAG_UNATTRIBUTED   1<<28

/** Flag for synthesized default constructors of anonymous classes.
 */
#define JAGP_FLAG_ANONCONSTR     1<<29

/** Flag for class symbols to indicate it has been checked and found
 *  acyclic.
 */
#define JAGP_FLAG_ACYCLIC            1<<30

/** Flag that marks bridge methods.
 */
#define JAGP_FLAG_BRIDGE            1L<<31

/** Flag that marks formal parameters.
 */
#define JAGP_FLAG_PARAMETER     1L<<33

/** Flag that marks varargs methods.
 */
#define JAGP_FLAG_VARARGS     1L<<34

/** Flag for annotation type symbols to indicate it has been
 *  checked and found acyclic.
 */
#define JAGP_FLAG_ACYCLIC_ANN        1L<<35

/** Flag that marks a generated default constructor.
 */
#define JAGP_FLAG_GENERATEDCONSTR     1L<<36

/** Flag that marks a hypothetical method that need not really be
 *  generated in the binary, but is present in the symbol table to
 *  simplify checking for erasure clashes - also used for 292 poly sig methods.
 */
#define JAGP_FLAG_HYPOTHETICAL     1L<<37

/**
 * Flag that marks an internal proprietary class.
 */
#define JAGP_FLAG_PROPRIETARY   1L<<38

/**
 * Flag that marks a multi-catch parameter.
 */
#define JAGP_FLAG_UNION   1L<<39

// Flag bit (1L << 40) is available.

/**
 * Flag that marks an 'effectively final' local variable.
 */
#define JAGP_FLAG_EFFECTIVELY_FINAL   1L<<41

/**
 * Flag that marks non-override equivalent methods with the same signature.
 */
#define JAGP_FLAG_CLASH   1L<<42

/**
 * Flag that marks either a default method or an interface containing default methods.
 */
#define JAGP_FLAG_DEFAULT   1L<<43

/**
 * Flag that marks class as auxiliary, ie a non-public class following
 * the public class in a source file, that could block implicit compilation.
 */
#define JAGP_FLAG_AUXILIARY   1L<<44

/**
 * Flag that marks that a symbol is not available in the current profile
 */
#define JAGP_FLAG_NOT_IN_PROFILE   1L<<45

/**
 * Flag that indicates that an override error has been detected by Check.
 */
#define JAGP_FLAG_BAD_OVERRIDE   1L<<45

/**
 * Flag that indicates a signature polymorphic method (292).
 */
#define JAGP_FLAG_SIGNATURE_POLYMORPHIC   1L<<46

/**
 * Flag that indicates that an inference variable is used in a 'throws' clause.
 */
#define JAGP_FLAG_THROWS   1L<<47

/**
 * Flag that marks potentially ambiguous overloads
 */
#define JAGP_FLAG_POTENTIALLY_AMBIGUOUS   1L<<48

/**
 * Flag that marks a synthetic method body for a lambda expression
 */
#define JAGP_FLAG_LAMBDA_METHOD   1L<<49

/**
 * Flag to control recursion in TransTypes
 */
#define JAGP_FLAG_TYPE_TRANSLATED   1L<<50

/**
 * Flag to indicate class symbol is for module-info
 */
#define JAGP_FLAG_MODULE   1L<<51

/**
 * Flag to indicate the given ModuleSymbol is an automatic module.
 */
#define JAGP_FLAG_AUTOMATIC_MODULE   1L<<52

/**
 * Flag to indicate the given ModuleSymbol is a system module.
 */
#define JAGP_FLAG_SYSTEM_MODULE   1L<<53


#define JAGP_FLAG_AccessFlags           JAGP_FLAG_PUBLIC | JAGP_FLAG_PROTECTED | JAGP_FLAG_PRIVATE
#define JAGP_FLAG_LocalClassFlags       JAGP_FLAG_FINAL | JAGP_FLAG_ABSTRACT | JAGP_FLAG_STRICTFP | JAGP_FLAG_ENUM | JAGP_FLAG_SYNTHETIC
#define JAGP_FLAG_MemberClassFlags      JAGP_FLAG_LocalClassFlags | JAGP_FLAG_INTERFACE | JAGP_FLAG_AccessFlags
#define JAGP_FLAG_ClassFlags            JAGP_FLAG_LocalClassFlags | JAGP_FLAG_INTERFACE | JAGP_FLAG_PUBLIC | JAGP_FLAG_ANNOTATION
#define JAGP_FLAG_InterfaceVarFlags     JAGP_FLAG_FINAL | JAGP_FLAG_STATIC | JAGP_FLAG_PUBLIC
#define JAGP_FLAG_VarFlags              JAGP_FLAG_AccessFlags | JAGP_FLAG_FINAL | JAGP_FLAG_STATIC | JAGP_FLAG_VOLATILE | JAGP_FLAG_TRANSIENT | JAGP_FLAG_ENUM
#define JAGP_FLAG_ConstructorFlags      JAGP_FLAG_AccessFlags
#define JAGP_FLAG_InterfaceMethodFlags  JAGP_FLAG_ABSTRACT | JAGP_FLAG_PUBLIC
#define JAGP_FLAG_MethodFlags           JAGP_FLAG_AccessFlags | JAGP_FLAG_ABSTRACT | JAGP_FLAG_STATIC | JAGP_FLAG_NATIVE | JAGP_FLAG_SYNCHRONIZED | JAGP_FLAG_FINAL | JAGP_FLAG_STRICTFP


#define JAGP_FLAG_ExtendedStandardFlags       JAGP_FLAG_StandardFlags | JAGP_FLAG_DEFAULT
#define JAGP_FLAG_ModifierFlags               (JAGP_FLAG_StandardFlags & ~JAGP_FLAG_INTERFACE) | JAGP_FLAG_DEFAULT
#define JAGP_FLAG_InterfaceMethodMask         JAGP_FLAG_ABSTRACT | JAGP_FLAG_PRIVATE | JAGP_FLAG_STATIC | JAGP_FLAG_PUBLIC | JAGP_FLAG_STRICTFP | JAGP_FLAG_DEFAULT
#define JAGP_FLAG_AnnotationTypeElementMask   JAGP_FLAG_ABSTRACT | JAGP_FLAG_PUBLIC
#define JAGP_FLAG_LocalVarFlags               JAGP_FLAG_FINAL | JAGP_FLAG_PARAMETER
#define JAGP_FLAG_ReceiverParamFlags          JAGP_FLAG_PARAMETER;

#endif /* CODE_JAGPFLAGS_H_ */
