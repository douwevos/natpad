/*
   File:    chapagewoprivate.h
   Project: chameleon
   Author:  Douwe Vos
   Date:    Feb 28, 2015
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2015 Douwe Vos.

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

#ifndef CHAPAGEWOPRIVATE_H_
#define CHAPAGEWOPRIVATE_H_

#include "chapagewo.h"

void cha_page_wo_construct(ChaPageWo *page, gboolean editable);

//ChaPageWo *cha_page_wo_new_with(CatArrayWo *c_line_list, int version);

CatArrayWo *cha_page_wo_get_internal_list(ChaPageWo *page);

int cha_page_wo_get_enrichment_count(ChaPageWo *page);
CatLock *cha_page_wo_get_enrichment_lock(ChaPageWo *page);

gboolean cha_page_wo_write_single_line(ChaPageWo *page, ChaWriteReq *write_req, const char *txt_data, int txt_len, ChaLineEnd line_end);

#endif /* CHAPAGEWOPRIVATE_H_ */
