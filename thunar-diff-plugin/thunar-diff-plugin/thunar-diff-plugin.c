/* $Id: thunar-diff-plugin.c,v 1.6 2013/07/20 14:01:31 setup Exp $ */
/*-
 * Copyright (c) 2013 Mingwang Xiao <eagle_xmw@aliyun.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <thunar-diff-plugin/tdp-provider.h>
#include <thunar-diff-plugin/tdp_preference.h>

static GType type_list[1];

G_MODULE_EXPORT void thunar_extension_initialize (ThunarxProviderPlugin *plugin)
{
    const gchar *mismatch;

    /* verify that the thunarx versions are compatible */
    mismatch = thunarx_check_version (THUNARX_MAJOR_VERSION, THUNARX_MINOR_VERSION, THUNARX_MICRO_VERSION);
    if (G_UNLIKELY (mismatch != NULL))
    {
        g_warning ("Version mismatch: %s", mismatch);
        return;
    }

    /* setup i18n support */
    bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
#ifdef HAVE_BIND_TEXTDOMAIN_CODESET
    bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
#endif
    textdomain (GETTEXT_PACKAGE);

#ifdef G_ENABLE_DEBUG
    g_message ("Initializing thunar-diff-plugin extension");
#endif

    /* register the types provided by this plugin */
    tdp_provider_register_type (plugin);

    /* setup the plugin provider type list */
    type_list[0] = TDP_TYPE_PROVIDER;
}

G_MODULE_EXPORT void thunar_extension_shutdown (void)
{
#ifdef G_ENABLE_DEBUG
    g_message ("Shutting down thunar-diff-plugin extension");
#endif
}

G_MODULE_EXPORT void thunar_extension_list_types (const GType **types, gint         *n_types)
{
    *types = type_list;
    *n_types = G_N_ELEMENTS (type_list);
}



