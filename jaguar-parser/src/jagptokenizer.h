///*
//   File:    jagptokenizer.h
//   Project: jaguar-parser
//   Author:  Douwe Vos
//   Date:    May 29, 2017
//   e-mail:  dmvos2000(at)yahoo.com
//
//   Copyright (C) 2017 Douwe Vos.
//
//   This program is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 3 of the License, or
//   (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */
//
//#ifndef JAGPTOKENIZER_H_
//#define JAGPTOKENIZER_H_
//
//#include <caterpillar.h>
//
//G_BEGIN_DECLS
//
//#define JAGP_TYPE_TOKENIZER              (jagp_tokenizer_get_type())
//#define JAGP_TOKENIZER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_tokenizer_get_type(), JagPTokenizer))
//#define JAGP_TOKENIZER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_TOKENIZER, JagPTokenizerClass))
//#define JAGP_IS_TOKENIZER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_TOKENIZER))
//#define JAGP_IS_TOKENIZER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_TOKENIZER))
//#define JAGP_TOKENIZER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_TOKENIZER, JagPTokenizerClass))
//
//
//typedef struct _JagPTokenizer               JagPTokenizer;
//typedef struct _JagPTokenizerPrivate        JagPTokenizerPrivate;
//typedef struct _JagPTokenizerClass          JagPTokenizerClass;
//
//
//struct _JagPTokenizer {
//	GObject parent;
//};
//
//struct _JagPTokenizerClass {
//	GObjectClass parent_class;
//};
//
//
//GType jagp_tokenizer_get_type();
//
//JagPTokenizer *jagp_tokenizer_new();
//
//G_END_DECLS
//
//#endif /* JAGPTOKENIZER_H_ */
