//
///*
//   File:    gropurecexporter.h
//   Project: gross
//   Author:  Douwe Vos
//   Date:    Jun 7, 2016
//   e-mail:  dmvos2000(at)yahoo.com
//
//   Copyright (C) 2016 Douwe Vos.
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
//#ifndef EXPORT_GROPURECEXPORTER_H_
//#define EXPORT_GROPURECEXPORTER_H_
//
//#include "../model/gromodel.h"
//#include "../state/grostatemachine.h"
//#include "gropurechelper.h"
//#include <caterpillar.h>
//
//G_BEGIN_DECLS
//
//#define GRO_TYPE_PURE_C_EXPORTER              (gro_pure_c_exporter_get_type())
//#define GRO_PURE_C_EXPORTER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), gro_pure_c_exporter_get_type(), GroPureCExporter))
//#define GRO_PURE_C_EXPORTER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRO_TYPE_PURE_C_EXPORTER, GroPureCExporterClass))
//#define GRO_IS_PURE_C_EXPORTER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_PURE_C_EXPORTER))
//#define GRO_IS_PURE_C_EXPORTER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRO_TYPE_PURE_C_EXPORTER))
//#define GRO_PURE_C_EXPORTER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRO_TYPE_PURE_C_EXPORTER, GroPureCExporterClass))
//
//
//typedef struct _GroPureCExporter               GroPureCExporter;
//typedef struct _GroPureCExporterPrivate        GroPureCExporterPrivate;
//typedef struct _GroPureCExporterClass          GroPureCExporterClass;
//
//
//struct _GroPureCExporter {
//	GObject parent;
//};
//
//struct _GroPureCExporterClass {
//	GObjectClass parent_class;
//};
//
//
//GType gro_pure_c_exporter_get_type();
//
//GroPureCExporter *gro_pure_c_exporter_new();
//
//void gro_pure_c_exporter_do_export(GroPureCExporter *exporter, GroModel *model, GroStateMachine *state_machine);
//
//G_END_DECLS
//
//#endif /* EXPORT_GROPURECEXPORTER_H_ */
