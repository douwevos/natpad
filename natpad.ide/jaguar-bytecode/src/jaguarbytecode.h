/*
   File:    jaguarbytecode.h
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Oct 28, 2012
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


#ifndef JAGUARBYTECODE_H_
#define JAGUARBYTECODE_H_

#include "jagbytecodereader.h"
#include "classfile/jagbytcatch.h"
#include "classfile/jagbytclassfile.h"
#include "classfile/jagbytfield.h"
#include "classfile/jagbytfieldlist.h"
#include "classfile/jagbytfinally.h"
#include "classfile/jagbyticatchorfinally.h"
#include "classfile/jagbytinterfacelist.h"
#include "classfile/jagbytmethodlist.h"
#include "classfile/jagbytmethod.h"
#include "classfile/jagbytmethodheader.h"
#include "classfile/jagbytname.h"
#include "classfile/jagbyttrycatch.h"
#include "classfile/jagbyttrycatchlist.h"
#include "classfile/bytecode/jagbytmnemonicblock.h"
#include "classfile/bytecode/jagbytimnemonicblock.h"
#include "classfile/bytecode/jagbytimnemonic.h"

#include "classfile/bytecode/mnemonic/jagbytoparraylength.h"
#include "classfile/bytecode/mnemonic/jagbytopcheckcast.h"
#include "classfile/bytecode/mnemonic/jagbytopcompareint.h"
#include "classfile/bytecode/mnemonic/jagbytopcomparereference.h"
#include "classfile/bytecode/mnemonic/jagbytopduplicate.h"
#include "classfile/bytecode/mnemonic/jagbytopget.h"
#include "classfile/bytecode/mnemonic/jagbytopgoto.h"
#include "classfile/bytecode/mnemonic/jagbytopincrement.h"
#include "classfile/bytecode/mnemonic/jagbytopinvoke.h"
#include "classfile/bytecode/mnemonic/jagbytoploadarray.h"
#include "classfile/bytecode/mnemonic/jagbytoploadconstant.h"
#include "classfile/bytecode/mnemonic/jagbytoploadfastindex.h"
#include "classfile/bytecode/mnemonic/jagbytoploadindex.h"
#include "classfile/bytecode/mnemonic/jagbytoploadnull.h"
#include "classfile/bytecode/mnemonic/jagbytopmathbasic.h"
#include "classfile/bytecode/mnemonic/jagbytopmathlogic.h"
#include "classfile/bytecode/mnemonic/jagbytopmonitor.h"
#include "classfile/bytecode/mnemonic/jagbytopmultianewarray.h"
#include "classfile/bytecode/mnemonic/jagbytopnew.h"
#include "classfile/bytecode/mnemonic/jagbytopnewarray.h"
#include "classfile/bytecode/mnemonic/jagbytoppush.h"
#include "classfile/bytecode/mnemonic/jagbytoppushindex.h"
#include "classfile/bytecode/mnemonic/jagbytopput.h"
#include "classfile/bytecode/mnemonic/jagbytopreturn.h"
#include "classfile/bytecode/mnemonic/jagbytopstorearray.h"
#include "classfile/bytecode/mnemonic/jagbytopstorefastindex.h"
#include "classfile/bytecode/mnemonic/jagbytopstoreindex.h"
#include "classfile/bytecode/mnemonic/jagbytoptableswitch.h"
#include "classfile/bytecode/mnemonic/jagbytopthrow.h"
#include "classfile/bytecode/mnemonic/jagbytopvalueconvert.h"

#include "classfile/label/jagbytlabelrepository.h"
#include "classfile/attribute/jagbytattributemap.h"
#include "classfile/attribute/jagbytattributelocalvariabletable.h"
#include "classfile/attribute/jagbytattributelinenumbertable.h"
#include "ast/formater/jagastwriter.h"
#include "ast/jagastargument.h"
#include "ast/jagastargumentlist.h"
#include "ast/jagastarrayaccessexpression.h"
#include "ast/jagastarraylengthexpression.h"
#include "ast/jagastassignment.h"
#include "ast/jagastblock.h"
#include "ast/jagastbreakstatement.h"
#include "ast/jagastcastexpression.h"
#include "ast/jagastcatchblock.h"
#include "ast/jagastdeclarationstatement.h"
#include "ast/jagastdeclarationtype.h"
#include "ast/jagastexpressionstatement.h"
#include "ast/jagastfieldaccessexpression.h"
#include "ast/jagastfielddeclaration.h"
#include "ast/jagastfinallyblock.h"
#include "ast/jagastforloopstatement.h"
#include "ast/jagastiastnode.h"
#include "ast/jagasticonditionalexpression.h"
#include "ast/jagastidentifier.h"
#include "ast/jagastiexpression.h"
#include "ast/jagastifthenelse.h"
#include "ast/jagastimportstatement.h"
#include "ast/jagastinfixexpression.h"
#include "ast/jagastinstanceofexpression.h"
#include "ast/jagastinvocationexpression.h"
#include "ast/jagastistatement.h"
#include "ast/jagastliteral.h"
#include "ast/jagastlogicalexpression.h"
#include "ast/jagastmethod.h"
#include "ast/jagastmethodheader.h"
#include "ast/jagastmodifiers.h"
#include "ast/jagastnewexpression.h"
#include "ast/jagastpackagestatement.h"
#include "ast/jagastpostfixexpression.h"
#include "ast/jagastprefixexpression.h"
#include "ast/jagastquotedstring.h"
#include "ast/jagastrelationalexpression.h"
#include "ast/jagastreturnstatement.h"
#include "ast/jagastsynchronizedblock.h"
#include "ast/jagastthrowstatement.h"
#include "ast/jagasttp.h"
#include "ast/jagasttpheader.h"
#include "ast/jagasttrycatchfinallystatement.h"
#include "ast/jagastunaryexpression.h"
#include "ast/jagastwhilestatement.h"
#include "ast/jagastwrappedconditionalexpression.h"


#endif /* JAGUARBYTECODE_H_ */
