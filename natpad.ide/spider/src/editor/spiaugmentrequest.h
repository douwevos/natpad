/*
   File:    spiaugmentrequest.h
   Project: spider
   Author:  Douwe Vos
   Date:    Dec 28, 2016
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2016 Douwe Vos.

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

#ifndef EDITOR_SPIAUGMENTREQUEST_H_
#define EDITOR_SPIAUGMENTREQUEST_H_

#include "spiscannerfactory.h"
#include <caterpillar.h>
#include <dragonfly.h>
#include <chameleon.h>
#include <grossruntime.h>

G_BEGIN_DECLS

#define SPI_TYPE_AUGMENT_REQUEST              (spi_augment_request_get_type())
#define SPI_AUGMENT_REQUEST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), spi_augment_request_get_type(), SpiAugmentRequest))
#define SPI_AUGMENT_REQUEST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), SPI_TYPE_AUGMENT_REQUEST, SpiAugmentRequestClass))
#define SPI_IS_AUGMENT_REQUEST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SPI_TYPE_AUGMENT_REQUEST))
#define SPI_IS_AUGMENT_REQUEST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), SPI_TYPE_AUGMENT_REQUEST))
#define SPI_AUGMENT_REQUEST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), SPI_TYPE_AUGMENT_REQUEST, SpiAugmentRequestClass))


typedef struct _SpiAugmentRequest               SpiAugmentRequest;
typedef struct _SpiAugmentRequestPrivate        SpiAugmentRequestPrivate;
typedef struct _SpiAugmentRequestClass          SpiAugmentRequestClass;


struct _SpiAugmentRequest {
	DraAugmentRequest parent;
};

struct _SpiAugmentRequestClass {
	DraAugmentRequestClass parent_class;
};


GType spi_augment_request_get_type();

SpiAugmentRequest *spi_augment_request_new(ChaDocument *document, ChaRevisionWo *a_revision, CatStringWo *slot_key,
		GroRunModel *model, GroRunITokenFactory *token_factory, SpiScannerFactory *scanner_factory);

G_END_DECLS

#endif /* EDITOR_SPIAUGMENTREQUEST_H_ */
