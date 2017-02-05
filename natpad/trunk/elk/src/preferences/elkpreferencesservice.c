/*
   File:    elkpreferencesservice.c
   Project: elk
   Author:  Douwe Vos
   Date:    Sep 22, 2014
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

#include "elkpreferencesservice.h"
#include "elkdefaultmapper.h"
#include "elkchaprefsextractor.h"
#include "ui/elkpreferencespanelfactory.h"
#include <shoveler.h>
#include <dragonfly.h>

#ifdef G_OS_WIN32
#include <userenv.h>
#endif

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ElkPreferencesService"
#include <logging/catlog.h>

struct _ElkPreferencesServicePrivate {
	ElkPreferencesContainer *container;
	CowPanelModel *panel_model;
	VipCdProvider *vip_cd_provider;

	ElkChaPrefsExtractor *chameleon_prefs_accessor;

	ElkPreferencesPanelFactory *panel_factory;
	DraPreferencesPanelFactory *panel_factory2;


	CatWeakList *mapper_list;
	ElkDefaultMapper *default_mapper;

	VipPath *shared_config_path;
	VipPath *user_config_path;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ElkPreferencesService, elk_preferences_service, G_TYPE_OBJECT,
		G_ADD_PRIVATE(ElkPreferencesService)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_setup_paths(ElkPreferencesService *service);

static void elk_preferences_service_class_init(ElkPreferencesServiceClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void elk_preferences_service_init(ElkPreferencesService *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ElkPreferencesService *instance = ELK_PREFERENCES_SERVICE(object);
	ElkPreferencesServicePrivate *priv = elk_preferences_service_get_instance_private(instance);
	cat_unref_ptr(priv->container);
	cat_unref_ptr(priv->panel_model);
	cat_unref_ptr(priv->vip_cd_provider);
	cat_unref_ptr(priv->panel_factory);
	cat_unref_ptr(priv->panel_factory2);
	cat_unref_ptr(priv->mapper_list);
	cat_unref_ptr(priv->default_mapper);
	cat_unref_ptr(priv->shared_config_path);
	cat_unref_ptr(priv->user_config_path);
	cat_unref_ptr(priv->chameleon_prefs_accessor);
	G_OBJECT_CLASS(elk_preferences_service_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(elk_preferences_service_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ElkPreferencesService *elk_preferences_service_new(VipCdProvider *vip_cd_provider) {
	ElkPreferencesService *result = g_object_new(ELK_TYPE_PREFERENCES_SERVICE, NULL);
	cat_ref_anounce(result);
	ElkPreferencesServicePrivate *priv = elk_preferences_service_get_instance_private(result);
	priv->container = elk_preferences_container_new();
	priv->panel_model = cow_panel_model_new();
	priv->vip_cd_provider = cat_ref_ptr(vip_cd_provider);
	priv->panel_factory = elk_preferences_panel_factory_new();
	cow_panel_model_add_factory(priv->panel_model, (CowIPanelFactory *) priv->panel_factory);

	priv->chameleon_prefs_accessor = elk_cha_prefs_extractor_new();
	priv->panel_factory2 = dra_preferences_panel_factory_new((CowIEntryAccessor *) priv->chameleon_prefs_accessor);
	cow_panel_model_add_factory(priv->panel_model, (CowIPanelFactory *) priv->panel_factory2);


	priv->mapper_list = cat_weak_list_new();
	priv->default_mapper = elk_default_mapper_new();
	cat_weak_list_append(priv->mapper_list, (GObject *) priv->default_mapper);

	l_setup_paths(result);
	return result;
}

void elk_preferences_service_set_spell_helper(ElkPreferencesService *service, DraSpellHelper *spell_helper) {
	ElkPreferencesServicePrivate *priv = elk_preferences_service_get_instance_private(service);
	dra_preferences_panel_factory_set_spell_helper(priv->panel_factory2, spell_helper);
}


ElkPreferencesContainer *elk_preferences_service_get_container(ElkPreferencesService *service) {
	ElkPreferencesServicePrivate *priv = elk_preferences_service_get_instance_private(service);
	return priv->container;
}

CowPanelModel *elk_preferences_service_get_panel_model(ElkPreferencesService *service) {
	ElkPreferencesServicePrivate *priv = elk_preferences_service_get_instance_private(service);
	return priv->panel_model;
}

CowIEntryAccessor *elk_preferences_services_get_chameleon_prefs_accessor(ElkPreferencesService *service) {
	ElkPreferencesServicePrivate *priv = elk_preferences_service_get_instance_private(service);
	return (CowIEntryAccessor *) priv->chameleon_prefs_accessor;
}



ElkPreferencesWo *elk_preferences_service_load(ElkPreferencesService *service) {
	ElkPreferencesServicePrivate *priv = elk_preferences_service_get_instance_private(service);

	ElkPreferencesWo *e_new_prefs = NULL;

	const char *home_dir = g_get_home_dir();
	cat_log_debug("home_dir=%s", home_dir);
	if (home_dir) {
		GFile *file = g_file_new_for_path(home_dir);
		if (file) {
			cat_log_debug("file=%p", file);
			cat_log_debug("file=%s", g_file_get_path(file));
			GFile *file_dot_natpad = g_file_get_child(file, ".natpad");
			if (file_dot_natpad) {
				cat_log_debug("file=%p", file_dot_natpad);
				cat_log_debug("file=%s", g_file_get_path(file_dot_natpad));
				GFile *file_natpad_sht = g_file_get_child(file_dot_natpad, "natpad-config2.sht");
				cat_log_debug("file=%s", g_file_get_path(file_natpad_sht));
				GFileQueryInfoFlags flags = G_FILE_QUERY_INFO_NONE;
				GFileInfo *file_info = g_file_query_info(file_natpad_sht, "standard::*", flags, NULL, NULL);
				cat_log_debug("file_info=%p", file_info);
				if (file_info) {
					uint64_t length = g_file_info_get_attribute_uint64(file_info, G_FILE_ATTRIBUTE_STANDARD_SIZE);
					cat_log_debug("length=%ld", length);

					char *buf = g_malloc0((gsize) length+1);

					GFileInputStream *stream = g_file_read(file_natpad_sht, NULL, NULL);


					gboolean all_read = g_input_stream_read_all((GInputStream *) stream, buf, (gsize) length, NULL, NULL, NULL);
					cat_log_debug("all_read=%d", all_read);
					if (all_read) {
						const CatStringWo *a_textd = cat_string_wo_new_data_len(buf, (int) length);
						cat_log_debug("text=%o", a_textd);
						CatStringInputStream *pstream = cat_string_input_stream_new(a_textd);
						cat_unref_ptr(a_textd);
						ShoParser *parser = sho_parser_new();
						ShoModel *model = sho_parser_parse(parser, (CatIInputStream *) pstream);
						cat_unref_ptr(pstream);
						cat_log_debug("model=%p", model);
						if (model==NULL) {
							model = sho_model_new();
						}
						cat_log_on_debug({
							sho_model_dump(model);
						});
						elk_preferences_container_set_model(priv->container, model);

						cat_log_indent_level++;
						e_new_prefs = elk_preferences_wo_new();
						cat_log_debug("mapping new prefs: e_new_prefs=%p", e_new_prefs);
						CatIIterator *iter = cat_weak_list_iterator(priv->mapper_list);
						while(cat_iiterator_has_next(iter)) {
							ElkIPreferencesMapper *mapper = (ElkIPreferencesMapper *) cat_iiterator_next(iter);
							elk_ipreferences_mapper_model_to_prefs(mapper, model, e_new_prefs);
						}
						cat_unref_ptr(iter);

						e_new_prefs = elk_preferences_wo_anchor(e_new_prefs, 0);
						elk_preferences_container_set(priv->container, e_new_prefs);
						cat_log_debug("finished mapping prefs: e_new_prefs=%p", e_new_prefs);
						cat_unref(e_new_prefs);
						cat_log_indent_level--;
						cat_unref_ptr(parser);
						cat_unref_ptr(model);
					}

					g_input_stream_close((GInputStream *) stream, NULL, NULL);
				}
			}
		}
	}
	return e_new_prefs;
}

void elk_preferences_service_save(ElkPreferencesService *service) {
	ElkPreferencesServicePrivate *priv = elk_preferences_service_get_instance_private(service);

	ElkPreferencesWo *prefs = elk_preferences_container_get(priv->container);
	cat_log_on_debug({
		cat_log_debug("prefs=%p", prefs);
		elk_preferences_wo_dump(prefs);
	});
	if (prefs==NULL) {
		return;
	}

	ShoModel *model = sho_model_new();
	CatIIterator *iter = cat_weak_list_iterator(priv->mapper_list);
	while(cat_iiterator_has_next(iter)) {
		ElkIPreferencesMapper *mapper = (ElkIPreferencesMapper *) cat_iiterator_next(iter);
		elk_ipreferences_mapper_prefs_to_model(mapper, prefs, model);
	}
	cat_unref_ptr(iter);

	ShoSerializer *serializer = sho_serializer_new(model);
	CatStringWo *model_as_text = sho_serialize_to_char_array(serializer);
	cat_unref_ptr(model);


	const char *home_dir = g_get_home_dir();
	if (home_dir) {
		GFile *file = g_file_new_for_path(home_dir);
		if (file) {
			cat_log_debug("file=%p", file);
			cat_log_debug("file=%s", g_file_get_path(file));
			GFile *file_dot_natpad = g_file_get_child(file, ".natpad");

			if (!g_file_query_exists(file_dot_natpad, NULL)) {
				g_file_make_directory(file_dot_natpad, NULL, NULL);
			}
			GFile *file_natpad_sht = g_file_get_child(file_dot_natpad, "natpad-config2.sht");

			VARIABLE_IS_NOT_USED gboolean result = g_file_replace_contents(file_natpad_sht, cat_string_wo_getchars(model_as_text), cat_string_wo_length(model_as_text), NULL, FALSE, G_FILE_CREATE_REPLACE_DESTINATION, NULL, NULL, NULL);
			cat_log_debug("result=%d", result);
		}
	}
	cat_unref_ptr(model_as_text);
	cat_unref_ptr(serializer);
}



void elk_preferences_services_add_mapper(ElkPreferencesService *service, ElkIPreferencesMapper *mapper) {
	ElkPreferencesServicePrivate *priv = elk_preferences_service_get_instance_private(service);
	cat_log_debug("mapper=%o", mapper);
	if (cat_weak_list_append_once(priv->mapper_list, (GObject *) mapper)) {
		ShoModel *model = elk_preferences_container_get_model(priv->container);
		cat_log_debug("model=%o", model);
		if (model==NULL) {
			return;
		}
		cat_log_indent_level++;
		cat_log_debug("adding mapper: mapper=%o", mapper);
		ElkPreferencesWo *e_prefs = (ElkPreferencesWo *) cow_container_get_configuration((CowContainer *) priv->container);
		cat_log_debug("e_prefs=%p", e_prefs);
		e_prefs = elk_preferences_wo_create_editable(e_prefs);
		cat_log_debug("e_prefs=%o", e_prefs);
		if (e_prefs) {
			elk_ipreferences_mapper_model_to_prefs(mapper, model, e_prefs);
			e_prefs = elk_preferences_wo_anchor(e_prefs, 0);
			elk_preferences_container_set(priv->container, e_prefs);
			cat_unref_ptr(e_prefs);
		}
		cat_log_debug("done adding mapper: mapper=%o", mapper);
		cat_log_indent_level--;
	}

}



VipPath *elk_preferences_services_get_shared_config_map(ElkPreferencesService *service) {
	ElkPreferencesServicePrivate *priv = elk_preferences_service_get_instance_private(service);
	return priv->shared_config_path;
}

VipPath *elk_preferences_services_get_user_config_map(ElkPreferencesService *service) {
	ElkPreferencesServicePrivate *priv = elk_preferences_service_get_instance_private(service);
	return priv->user_config_path;
}



#ifdef G_OS_UNIX

static CatS l_s_txt_shared_config_path = CAT_S_DEF("/usr/share/natpad/");

#endif



static void l_setup_paths(ElkPreferencesService *service) {
	ElkPreferencesServicePrivate *priv = elk_preferences_service_get_instance_private(service);
#ifdef G_OS_WIN32
	cat_log_debug("get home dir");
	char szHomeDirBuf[MAX_PATH] = { 0 };

	HANDLE hToken = 0;
	OpenProcessToken( GetCurrentProcess(), TOKEN_QUERY, &hToken);

	DWORD BufSize = MAX_PATH;
	GetUserProfileDirectory( hToken, szHomeDirBuf, &BufSize );


	const char *bpath = getenv("windir");
	CatStringWo *shared_config_dir = cat_string_wo_new_with(bpath);
	cat_string_wo_append_chars(shared_config_dir, "\\Natpad\\");

//   ExpandEnvironmentStringsForUserA(hToken, "%windir%\\Natpad\\", (LPWSTR)szHomeDirBuf, MAX_PATH);
//   CatStringWo *shared_config_dir = cat_string_wo_new_with(szHomeDirBuf);
	priv->shared_config_path = vip_path_new(shared_config_dir);
	cat_unref_ptr(shared_config_dir);

	CloseHandle( hToken );
	cat_log_debug("ending setup-path");
#else

	priv->shared_config_path = vip_path_new(CAT_S(l_s_txt_shared_config_path));

#endif

	cat_log_debug("shared_config_path=%o", priv->shared_config_path);

	priv->user_config_path = NULL;

	const char *home_dir = g_get_home_dir();
	if (home_dir) {
		GFile *file = g_file_new_for_path(home_dir);
		if (file) {
			cat_log_debug("file=%p : %s", file, g_file_get_path(file));
			GFile *file_dot_natpad = g_file_get_child(file, ".natpad");
			if (file_dot_natpad) {
				cat_log_debug("file=%p : %s", file_dot_natpad, g_file_get_path(file_dot_natpad));

				priv->user_config_path = vip_path_new(cat_string_wo_new_with(g_file_get_path(file_dot_natpad)));
			}
		}
	}

	cat_log_debug("user_config_path=%o", priv->user_config_path);

}



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ElkPreferencesService *instance = ELK_PREFERENCES_SERVICE(self);
	ElkPreferencesServicePrivate *priv = elk_preferences_service_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p user_config_path=%o]", iname, self, priv->user_config_path);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

