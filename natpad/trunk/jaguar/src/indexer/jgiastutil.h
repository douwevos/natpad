/*
   File:    jgiastutil.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Jun 16, 2014
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2014 Douwe Vos.

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

#ifndef JGIASTUTIL_H_
#define JGIASTUTIL_H_

#include "../parser/jagasttoken.h"
#include "model/jgitokenrange.h"
#include <caterpillar.h>


JgiTokenRange *jgi_ast_util_extract_name_root(CatArrayWo *e_token_array, int token_index);

void jgi_ast_util_dump_reverse(JagAstToken *token, JagAstToken *token_root_opt);

#endif /* JGIASTUTIL_H_ */
