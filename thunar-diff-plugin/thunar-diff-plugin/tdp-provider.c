/* $Id: tdp-provider.c,v 1.27 2013/07/22 16:03:51 setup Exp $ */
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

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <string.h>
#include <thunarx/thunarx.h>
#include <thunar-diff-plugin/tdp-provider.h>
#include <thunar-diff-plugin/tdp_preference.h>

#if 0
/* use g_access() on win32 */
#if defined(G_OS_WIN32)
#include <glib/gstdio.h>
#else
#define g_access(filename, mode) access((filename), (mode))
#endif
#endif

/* nine item: GtkAction's name */
#define TDP_ACTION_NAME_SELECT_LEFT                     "Tdp::Select-Left"
#define TDP_ACTION_NAME_COMPARE_TO_LEFT                 "Tdp::Compare-To-Left"
#define TDP_ACTION_NAME_SELECT_MIDDLE                   "Tdp::Select-Middle"
#define TDP_ACTION_NAME_MERGE_TO_LEFT_MIDDLE            "Tdp::Merge-To-Left_Middle"
#define TDP_ACTION_NAME_COMPARE_TWO_ITEMS               "Tdp::Compare-Two-Items"
#define TDP_ACTION_NAME_SELECT_TWO_ITEMS_TO_MERGE       "Tdp::Select-Two-Items-To-Merge"
#define TDP_ACTION_NAME_MERGE_TO_LEFT                   "Tdp::Merge-To-Left"
#define TDP_ACTION_NAME_MERGE_THREE_ITEMS               "Tdp::Merge-Three-Items"
#define TDP_ACTION_NAME_EDIT_PREFERENCES                "Tdp::Edit-Preferences"

/* nine item: menu item display name */
#if 0
#define TDP_DISPLAY_NAME_SELECT_LEFT                    "Select _Left"
#define TDP_DISPLAY_NAME_COMPARE_TO_LEFT                "Compare _To: %10s"
#define TDP_DISPLAY_NAME_SELECT_MIDDLE                  "Select _Middle"
#define TDP_DISPLAY_NAME_MERGE_TO_LEFT_MIDDLE           "Merge _To: %10s & %10s"
#define TDP_DISPLAY_NAME_COMPARE_TWO_ITEMS              "Compare _Two Items"
#define TDP_DISPLAY_NAME_SELECT_TWO_ITEMS_TO_MERGE      "Select _Two Items To Merge"
#define TDP_DISPLAY_NAME_MERGE_TO_LEFT                  "Merge _To: %10s"
#define TDP_DISPLAY_NAME_MERGE_THREE_ITEMS              "Merge _Three Items"
#define TDP_DISPLAY_NAME_EDIT_PREFERENCES               "Edit _Preferences"
#endif

/* nine item: icon name */
#define TDP_ICON_NAME_SELECT_LEFT                       "tdp-select-left"
#define TDP_ICON_NAME_COMPARE_TO_LEFT                   "tdp-compare-to-left"
#define TDP_ICON_NAME_SELECT_MIDDLE                     "tdp-select-middle"
#define TDP_ICON_NAME_MERGE_TO_LEFT_MIDDLE              "tdp-merge-to-left-middle-items"
#define TDP_ICON_NAME_COMPARE_TWO_ITEMS                 "tdp-compare-two-items"
#define TDP_ICON_NAME_SELECT_TWO_ITEMS_TO_MERGE         "tdp-select-two-items-to-merge"
#define TDP_ICON_NAME_MERGE_TO_LEFT                     "tdp-merge-to-left-items"
#define TDP_ICON_NAME_MERGE_THREE_ITEMS                 "tdp-merge-three-items"
#define TDP_ICON_NAME_EDIT_PREFERENCES                  "tdp-edit-preferences"

#if 0
/* nine item: tips1 name */
#define TDP_TIPS_1_NAME_SELECT_LEFT                     "Select the file as compare left"
#define TDP_TIPS_1_NAME_COMPARE_TO_LEFT                 "Compare to the file"
#define TDP_TIPS_1_NAME_SELECT_MIDDLE                   "Select the file as merge middle"
#define TDP_TIPS_1_NAME_MERGE_TO_LEFT_MIDDLE            "Merge to the files"
#define TDP_TIPS_1_NAME_COMPARE_TWO_ITEMS               "Compare two files"
#define TDP_TIPS_1_NAME_SELECT_TWO_ITEMS_TO_MERGE       "Select two files as merge"
#define TDP_TIPS_1_NAME_MERGE_TO_LEFT                   "Merge two files to left file"
#define TDP_TIPS_1_NAME_MERGE_THREE_ITEMS               "Merge three files"
#define TDP_TIPS_1_NAME_EDIT_PREFERENCES                "Edit the preferences"

/* nine item: tips2 name */
#define TDP_TIPS_2_NAME_SELECT_LEFT                     "Select the folder as compare left"
#define TDP_TIPS_2_NAME_COMPARE_TO_LEFT                 "Compare to the folder"
#define TDP_TIPS_2_NAME_SELECT_MIDDLE                   "Select the folder as merge middle"
#define TDP_TIPS_2_NAME_MERGE_TO_LEFT_MIDDLE            "Merge to the folders"
#define TDP_TIPS_2_NAME_COMPARE_TWO_ITEMS               "Compare two folders"
#define TDP_TIPS_2_NAME_SELECT_TWO_ITEMS_TO_MERGE       "Select two folders as merge"
#define TDP_TIPS_2_NAME_MERGE_TO_LEFT                   "Merge two folders to left folder"
#define TDP_TIPS_2_NAME_MERGE_THREE_ITEMS               "Merge three folders"
#define TDP_TIPS_2_NAME_EDIT_PREFERENCES                "Edit the preferences"
#endif

/* local config info */
static PerferenceInfo m_stPerferenceInfo;

static void   tdp_provider_class_init           (TdpProviderClass         *klass);
static void   tdp_provider_menu_provider_init   (ThunarxMenuProviderIface *iface);
static void   tdp_provider_init                 (TdpProvider              *tdp_provider);
static void   tdp_provider_finalize             (GObject                  *object);
static GList *tdp_provider_get_file_actions     (ThunarxMenuProvider      *menu_provider, GtkWidget* window, GList* files);
static GList *tdp_provider_get_folder_actions   (ThunarxMenuProvider      *menu_provider, GtkWidget* window, ThunarxFileInfo* folders);

struct _TdpProviderClass
{
    GObjectClass __parent__;
};

struct _TdpProvider
{
    GObject         __parent__;

#if !GTK_CHECK_VERSION(2,9,0)
    /* GTK+ 2.9.0 and above provide an icon-name property
     * for GtkActions, so we don't need the icon factory.
     */
    GtkIconFactory *icon_factory;
#endif
};

typedef struct tagSelectInfo
{
    unsigned long type; /* 0: none, 1: file, 2: folder */
    gchar*  name;
} SelectInfo;

THUNARX_DEFINE_TYPE_WITH_CODE (TdpProvider,
                               tdp_provider,
                               G_TYPE_OBJECT,
                               THUNARX_IMPLEMENT_INTERFACE (THUNARX_TYPE_MENU_PROVIDER,
                               tdp_provider_menu_provider_init));


static void tdp_provider_class_init (TdpProviderClass *klass)
{
    GObjectClass *gobject_class;

    gobject_class = G_OBJECT_CLASS (klass);
    gobject_class->finalize = tdp_provider_finalize;
}

static void tdp_provider_menu_provider_init (ThunarxMenuProviderIface *iface)
{
    iface->get_file_actions   = &tdp_provider_get_file_actions;
    iface->get_folder_actions = &tdp_provider_get_folder_actions;
}

#if !GTK_CHECK_VERSION(2,9,0)
static void tdp_add_icon_set(GtkIconFactory *icon_factory, gchar* icon_name)
{
    GtkIconSet    *icon_set = NULL;
    GtkIconSource *icon_source = NULL;

    icon_set = gtk_icon_set_new ();
    icon_source = gtk_icon_source_new ();
    gtk_icon_source_set_icon_name (icon_source, icon_name);
    gtk_icon_set_add_source (icon_set, icon_source);
    gtk_icon_factory_add (icon_factory, icon_name, icon_set);
    gtk_icon_source_free (icon_source);
    gtk_icon_set_unref (icon_set);
}
#endif

static void tdp_provider_init (TdpProvider *tdp_provider)
{
#if !GTK_CHECK_VERSION(2,9,0)
    GtkIconSource *icon_source;
    GtkIconSet    *icon_set;

    /* setup our icon factory */
    tdp_provider->icon_factory = gtk_icon_factory_new ();
    gtk_icon_factory_add_default (tdp_provider->icon_factory);

    /* add the "tdp-select-left" stock icon */
    tdp_add_icon_set(tdp_provider->icon_factory, TDP_ICON_NAME_SELECT_LEFT);

    /* add the "tdp-compare-to-left" stock icon */
    tdp_add_icon_set(tdp_provider->icon_factory, TDP_ICON_NAME_COMPARE_TO_LEFT);

    /* add the "tdp-select-middle" stock icon */
    tdp_add_icon_set(tdp_provider->icon_factory, TDP_ICON_NAME_SELECT_MIDDLE);

    /* add the "tdp-merge-to-left-middle-items" stock icon */
    tdp_add_icon_set(tdp_provider->icon_factory, TDP_ICON_NAME_MERGE_TO_LEFT_MIDDLE);

    /* add the "tdp-compare-two-items" stock icon */
    tdp_add_icon_set(tdp_provider->icon_factory, TDP_ICON_NAME_COMPARE_TWO_ITEMS);

    /* add the "tdp-select-two-items-to-merge" stock icon */
    tdp_add_icon_set(tdp_provider->icon_factory, TDP_ICON_NAME_SELECT_TWO_ITEMS_TO_MERGE);

    /* add the "tdp-merge-to-left-items" stock icon */
    tdp_add_icon_set(tdp_provider->icon_factory, TDP_ICON_NAME_MERGE_TO_LEFT);

    /* add the "tdp-merge-three-items" stock icon */
    tdp_add_icon_set(tdp_provider->icon_factory, TDP_ICON_NAME_MERGE_THREE_ITEMS);

    /* add the "tdp-edit-preferences" stock icon */
    tdp_add_icon_set(tdp_provider->icon_factory, TDP_ICON_NAME_EDIT_PREFERENCES);
#endif /* !GTK_CHECK_VERSION(2,9,0) */

    /* load local config info */
    memset((void*)(&m_stPerferenceInfo), 0, sizeof(PerferenceInfo));
    /* set default data */
    m_stPerferenceInfo.select_left_check        = 1;
    m_stPerferenceInfo.compare_to_left_check    = 1;
    m_stPerferenceInfo.compare_two_items_check  = 1;
    /* load config data from config.ini */
    load_perf_config_info(&m_stPerferenceInfo, DEFAULT_PERFERENCE_INI);
    
#ifdef G_ENABLE_DEBUG
    if (NULL == m_stPerferenceInfo.compare_edt)
        g_message ("[config] compare_edt: ");
    else
        g_message ("[config] compare_edt: %s",                  m_stPerferenceInfo.compare_edt);
    if (NULL == m_stPerferenceInfo.three_way_compare_edt)
        g_message ("[config] three_way_compare_edt: ");
    else
        g_message ("[config] three_way_compare_edt: %s",        m_stPerferenceInfo.three_way_compare_edt);
    g_message ("[config] keep_files_check: %d",                 m_stPerferenceInfo.keep_files_check);
    g_message ("[config] select_left_check: %d",                m_stPerferenceInfo.select_left_check);
    g_message ("[config] compare_to_left_check: %d",            m_stPerferenceInfo.compare_to_left_check);
    g_message ("[config] compare_two_items_check: %d",          m_stPerferenceInfo.compare_two_items_check);
    g_message ("[config] select_to_middle_check: %d",           m_stPerferenceInfo.select_to_middle_check);
    g_message ("[config] select_two_item_to_merge_check: %d",   m_stPerferenceInfo.select_two_item_to_merge_check);
    g_message ("[config] merge_to_left_check: %d",              m_stPerferenceInfo.merge_to_left_check);
    g_message ("[config] merge_to_left_middle_check: %d",       m_stPerferenceInfo.merge_to_left_middle_check);
    g_message ("[config] merge_three_items_check: %d",          m_stPerferenceInfo.merge_three_items_check);
    
    g_message ("[config] select_left_item_type: %d",            m_stPerferenceInfo.select_left_item_type);
    if (NULL == m_stPerferenceInfo.select_left_item)
        g_message ("[config] select_left_item: ");
    else
        g_message ("[config] select_left_item: %s",             m_stPerferenceInfo.select_left_item);
    g_message ("[config] select_middle_item_type: %d",          m_stPerferenceInfo.select_middle_item_type);
    if (NULL == m_stPerferenceInfo.select_middle_item)
        g_message ("[config] select_middle_item: ");
    else
        g_message ("[config] select_middle_item: %s",           m_stPerferenceInfo.select_middle_item);
#endif
}

static void tdp_provider_finalize (GObject *object)
{
#if 0
    TdpProvider *tdp_provider = TDP_PROVIDER (object);
    GSource     *source;

    /* give up maintaince of any pending child watch */
    if (G_UNLIKELY (tdp_provider->child_watch_id != 0))
    {
        /* reset the callback function to g_spawn_close_pid() so the plugin can be
         * safely unloaded and the child will still not become a zombie afterwards.
         * This also resets the child_watch_id and child_watch_path properties.
         */
        source = g_main_context_find_source_by_id (NULL, tdp_provider->child_watch_id);
        g_source_set_callback (source, (GSourceFunc) g_spawn_close_pid, NULL, NULL);
    }
#endif    

#if !GTK_CHECK_VERSION(2,9,0)
    /* release our icon factory */
    gtk_icon_factory_remove_default (tdp_provider->icon_factory);
    g_object_unref (G_OBJECT (tdp_provider->icon_factory));
#endif

    /* free local config info */
    if (NULL != m_stPerferenceInfo.compare_edt)
    {
#ifdef G_ENABLE_DEBUG
        g_message ("try to free compare_edt");
#endif
        g_free(m_stPerferenceInfo.compare_edt);
        m_stPerferenceInfo.compare_edt = NULL;
    }
    if (NULL != m_stPerferenceInfo.three_way_compare_edt)
    {
#ifdef G_ENABLE_DEBUG
        g_message ("try to free three_way_compare_edt");
#endif
        g_free(m_stPerferenceInfo.three_way_compare_edt);
        m_stPerferenceInfo.three_way_compare_edt = NULL;
    }
    if (NULL != m_stPerferenceInfo.select_left_item)
    {
#ifdef G_ENABLE_DEBUG
        g_message ("try to free select_left_item");
#endif
        g_free(m_stPerferenceInfo.select_left_item);
        m_stPerferenceInfo.select_left_item = NULL;
        m_stPerferenceInfo.select_left_item_type = 0;
    }
    if (NULL != m_stPerferenceInfo.select_middle_item)
    {
#ifdef G_ENABLE_DEBUG
        g_message ("try to free select_middle_item");
#endif
        g_free(m_stPerferenceInfo.select_middle_item);
        m_stPerferenceInfo.select_middle_item = NULL;
        m_stPerferenceInfo.select_middle_item_type = 0;
    }

    (*G_OBJECT_CLASS (tdp_provider_parent_class)->finalize) (object);
}

static gchar* tdp_get_execute_cmd(const gchar* strCmd)
{
    if (NULL == strCmd)
    {
        g_warning ("can't find compare application!");
        return NULL;
    }
    
    /* check file is exist */
    if (0 != access(strCmd, R_OK))
    {
        g_warning ("can't open compare application!");
        return NULL;
    }
    gint len;
    gchar * strBuff;
    
    len = strlen(strCmd);
    strBuff = (gchar*)g_malloc(len + 1);
    memset(strBuff, 0, len + 1);
    strncpy(strBuff, strCmd, len);
    strBuff[len] = '\0';
    
    /* check file is desktop file */
#if 0 
    if ()
    {
    }
#endif
    
    /* check command can execute */
    if (0 != access(strBuff, X_OK))
    {
        g_warning ("compare application can't execute!");
        return NULL;
    }
    
    return strBuff;
}

static void   tdp_start_to_compare(gchar* left, gchar* right)
{
    GPid pid;
    gboolean result;
    gchar* strCommand;
    gchar* strBuff[4];
    
    strCommand = tdp_get_execute_cmd(m_stPerferenceInfo.compare_edt);
    if (NULL == strCommand)
    {
        g_warning ("can't find compare application!");
        return;
    }

#ifdef G_ENABLE_DEBUG
    g_message ("start to execute: %s %s %s", strCommand, left, right);
#endif

    strBuff[0] = strCommand;
    strBuff[1] = left;
    strBuff[2] = right;
    strBuff[3] = NULL;
    result = g_spawn_async(NULL, strBuff, NULL, 0, NULL, NULL, &pid, NULL);
    if (FALSE == result)
    {
        g_warning("execute fail!");
    }
    g_spawn_close_pid(pid);
    g_free(strCommand);
}

static void   tdp_start_to_merge(gchar* left, gchar* middle, gchar* right)
{
    GPid pid;
    gboolean result;
    gchar* strCommand;
    gchar* strBuff[5];
    
    strCommand = tdp_get_execute_cmd(m_stPerferenceInfo.three_way_compare_edt);
    if (NULL == strCommand)
    {
        g_warning ("can't find merge application!");
        return;
    }
    
#ifdef G_ENABLE_DEBUG
    g_message ("start to execute: %s %s %s %s", strCommand, left, middle, right);
#endif

    strBuff[0] = strCommand;
    strBuff[1] = left;
    strBuff[2] = middle;
    strBuff[3] = right;
    strBuff[4] = NULL;
    
    result = g_spawn_async(NULL, strBuff, NULL, 0, NULL, NULL, &pid, NULL);
    if (FALSE == result)
    {
        g_warning("execute fail!");
    }
    g_spawn_close_pid(pid);
    g_free(strCommand);
}

static void   tdp_clear_select_result()
{
    /* check if need clear data */
    if (!is_check_item(&m_stPerferenceInfo, keep_files_check))
    {
        if (NULL != m_stPerferenceInfo.select_left_item)
        {
            g_free(m_stPerferenceInfo.select_left_item);
        }
        if (NULL != m_stPerferenceInfo.select_middle_item)
        {
            g_free(m_stPerferenceInfo.select_middle_item);
        }
        m_stPerferenceInfo.select_left_item         = NULL;
        m_stPerferenceInfo.select_middle_item       = NULL;
        m_stPerferenceInfo.select_left_item_type    = 0;
        m_stPerferenceInfo.select_middle_item_type  = 0;
        save_perf_config_info(&m_stPerferenceInfo, DEFAULT_PERFERENCE_INI);
    }
}

/* check gtkaction associated data . */
static GList* tdp_check_associated_data(GtkAction *action, const gchar* act_name, gint item_num)
{
    gint      i;
    int       oldtype = 0;
    GList           *items;
    SelectInfo      *si = NULL;

    /* get the files associated with the action */
    items = (GList*)g_object_get_data (G_OBJECT (action), act_name);
    if (items == NULL)
    {
#ifdef G_ENABLE_DEBUG
        g_warning ("g_object_get_data return NULL\n");
#endif
        return NULL;
    }

    /* check item's number */
    if (item_num != g_list_length(items))
    {
#ifdef G_ENABLE_DEBUG
        g_warning ("GList length error: %d != %d\n", item_num, g_list_length(items));
#endif
        return NULL;
    }
    /* get data and check it */
    for(i = 0; i < item_num; i++)
    {
        si = g_list_nth_data(items, i);
        if (si == NULL)
        {
#ifdef G_ENABLE_DEBUG
            g_warning ("GList data is NULL, index = %d, max = %d\n", 
                        i, item_num);
#endif
            return NULL;
        }
        if (NULL == si->name)
        {
#ifdef G_ENABLE_DEBUG
            g_warning ("SelectInfo name is NULL, index = %d, max = %d\n",
                        i, item_num);
#endif
            return NULL;
        }
        /* the type must be file or folder */
        if ((1 != si->type)&&(2 != si->type))
        {
#ifdef G_ENABLE_DEBUG
            g_warning ("SelectInfo type error, index = %d, max = %d, type = %d\n", 
                        i, item_num, (int)(si->type));
#endif
            return NULL;
        }
        /*  first time, save current type */
        if (0 == oldtype)
        {
            oldtype = si->type;
        }
        /* check this type is equ old type */
        if (oldtype != si->type)
        {
#ifdef G_ENABLE_DEBUG
            g_warning ("SelectInfo type error, index = %d, max = %d, oldtype = %d, type = %d\n",
                        i, item_num, oldtype, (int)(si->type));
#endif
            return NULL;
        }
    }

    return items;
}

/* save left item into config info */
static void tdp_save_left_item(gchar* name, gint type)
{
    if (NULL != m_stPerferenceInfo.select_left_item)
    {
        g_free(m_stPerferenceInfo.select_left_item);
    }
    m_stPerferenceInfo.select_left_item      = name;
    m_stPerferenceInfo.select_left_item_type = type;
}

/* save middle item into config info */
static void tdp_save_middle_item(gchar* name, gint type)
{
    if (NULL != m_stPerferenceInfo.select_middle_item)
    {
        g_free(m_stPerferenceInfo.select_middle_item);
    }
    m_stPerferenceInfo.select_middle_item      = name;
    m_stPerferenceInfo.select_middle_item_type = type;
}

/* select one item, for compare or merge */
static void tdp_select_as_left (GtkAction *action, GtkWidget *window)
{
    GList           *items;
    SelectInfo      *si = NULL;

    /* check the files associated with the action */
    items = tdp_check_associated_data (action, TDP_ACTION_NAME_SELECT_LEFT, 1);
    if (items == NULL)
    {
#ifdef G_ENABLE_DEBUG
        g_warning ("check [Select as left] data fail\n");
#endif
        return;
    }
    si = g_list_nth_data(items, 0);
#ifdef G_ENABLE_DEBUG
    g_message ("selected name: [%s]", si->name);
#endif
    /* save left item */
    tdp_save_left_item(si->name, si->type);
    
    /* if select left item type is not equ select middle item */
    if (m_stPerferenceInfo.select_left_item_type != m_stPerferenceInfo.select_middle_item_type)
    {
        tdp_save_middle_item(NULL, 0);
    }

    /* set SelectInfo.name to NULL, */
    si->name = NULL;
    si->type = 0;
    
    /* save local config info */
    save_perf_config_info(&m_stPerferenceInfo, DEFAULT_PERFERENCE_INI);
}

/* select middle item, for merge, middle item must be same type to left item (file or folder) */
static void tdp_select_as_middle (GtkAction *action, GtkWidget *window)
{
    GList           *items;
    SelectInfo      *si = NULL;

    /* check the files associated with the action */
    items = tdp_check_associated_data (action, TDP_ACTION_NAME_SELECT_MIDDLE, 1);
    if (items == NULL)
    {
#ifdef G_ENABLE_DEBUG
        g_warning ("check [Select as middle] data fail\n");
#endif
        return;
    }
    si = g_list_nth_data(items, 0);

    /* if select left item type is not equ select middle item */
    if (m_stPerferenceInfo.select_left_item_type != si->type)
    {
#ifdef G_ENABLE_DEBUG
        g_warning ("[Select as middle] left type not equ select item type [%d, %d]\n", 
                    m_stPerferenceInfo.select_left_item_type, (int)(si->type));
#endif
        return;
    }
    /* save middle item */
    tdp_save_middle_item(si->name, si->type);

    /* set SelectInfo.name to NULL, */
    si->name = NULL;
    si->type = 0;

    /* save local config info */
    save_perf_config_info(&m_stPerferenceInfo, DEFAULT_PERFERENCE_INI);
}

/* select left and middle item, for merge */
static void tdp_select_two_items_to_merge (GtkAction *action, GtkWidget *window)
{
    GList           *items;
    SelectInfo      *si = NULL;

    /* check the files associated with the action */
    items = tdp_check_associated_data (action, TDP_ACTION_NAME_SELECT_TWO_ITEMS_TO_MERGE, 2);
    if (items == NULL)
    {
#ifdef G_ENABLE_DEBUG
        g_warning ("check [Select two items to merge] data fail\n");
#endif
        return;
    }
    si = g_list_nth_data(items, 0);
    /* save left item */
    tdp_save_left_item(si->name, si->type);

    /* set SelectInfo.name to NULL, */
    si->name = NULL;
    si->type = 0;

    si = g_list_nth_data(items, 1);
    /* save middle item */
    tdp_save_middle_item(si->name, si->type);

    /* set SelectInfo.name to NULL, */
    si->name = NULL;
    si->type = 0;

    /* save local config info */
    save_perf_config_info(&m_stPerferenceInfo, DEFAULT_PERFERENCE_INI);
}

/* compare to left, middle can exist, if compare op finished, left/middle item will be clean */
static void tdp_compare_to_left (GtkAction *action, GtkWidget *window)
{
    GList           *items;
    SelectInfo      *si = NULL;

    /* check the files associated with the action */
    items = tdp_check_associated_data (action, TDP_ACTION_NAME_COMPARE_TO_LEFT, 1);
    if (items == NULL)
    {
#ifdef G_ENABLE_DEBUG
        g_warning ("check [Compare to left] data fail\n");
#endif
        return;
    }
    si = g_list_nth_data(items, 0);

    /* if select left item type is not equ select middle item */
    if (m_stPerferenceInfo.select_left_item_type != si->type)
    {
#ifdef G_ENABLE_DEBUG
        g_warning ("[Compare to left] left type not equ select item type [%d, %d]\n",
                    (int)(m_stPerferenceInfo.select_left_item_type), (int)(si->type));
#endif
        return;
    }

    /* start to compare */
    tdp_start_to_compare(m_stPerferenceInfo.select_left_item, si->name);

    /* clear select result data */
    tdp_clear_select_result();
}

/* select two same type item, can direct to compare */
static void tdp_compare_two_items (GtkAction *action, GtkWidget *window)
{
    GList           *items;
    SelectInfo      *si1 = NULL;
    SelectInfo      *si2 = NULL;

    /* check the files associated with the action */
    items = tdp_check_associated_data (action, TDP_ACTION_NAME_COMPARE_TWO_ITEMS, 2);
    if (items == NULL)
    {
#ifdef G_ENABLE_DEBUG
        g_warning ("check [Compare two items] data fail\n");
#endif
        return;
    }
    si1 = g_list_nth_data(items, 0);
    si2 = g_list_nth_data(items, 1);

    /* start to compare */
    tdp_start_to_compare(si1->name, si2->name);

    /* clear select result data */
    tdp_clear_select_result();
}

/* select three same type item, can direct to merge */
static void tdp_merge_three_items (GtkAction *action, GtkWidget *window)
{
    GList           *items;
    SelectInfo      *si1 = NULL;
    SelectInfo      *si2 = NULL;
    SelectInfo      *si3 = NULL;

    /* check the files associated with the action */
    items = tdp_check_associated_data (action, TDP_ACTION_NAME_MERGE_THREE_ITEMS, 3);
    if (items == NULL)
    {
#ifdef G_ENABLE_DEBUG
        g_warning ("check [Merge three items] data fail\n");
#endif
        return;
    }
    si1 = g_list_nth_data(items, 0);
    si2 = g_list_nth_data(items, 1);
    si3 = g_list_nth_data(items, 2);

    /* start to merge */
    tdp_start_to_merge(si1->name, si2->name, si3->name);

    /* clear select result data */
    tdp_clear_select_result();
}

/* merge to left item, left item must be exist, if merge op finished, left item will be clean */
static void tdp_merge_to_left_item (GtkAction *action, GtkWidget *window)
{
    GList           *items;
    SelectInfo      *si1 = NULL;
    SelectInfo      *si2 = NULL;

    /* check the files associated with the action */
    items = tdp_check_associated_data (action, TDP_ACTION_NAME_MERGE_TO_LEFT, 2);
    if (items == NULL)
    {
#ifdef G_ENABLE_DEBUG
        g_warning ("check [Merge to left item] data fail\n");
#endif
        return;
    }
    si1 = g_list_nth_data(items, 0);
    si2 = g_list_nth_data(items, 1);
    /* if select left item type is not equ select middle item */
    if (m_stPerferenceInfo.select_left_item_type != si1->type)
    {
#ifdef G_ENABLE_DEBUG
        g_warning ("[Merge to left item] left type not equ select item type [%d, %d]\n",
                    m_stPerferenceInfo.select_left_item_type, (int)(si1->type));
#endif
        return;
    }

    /* start to merge */
    tdp_start_to_merge(m_stPerferenceInfo.select_left_item, si1->name, si2->name);

    /* clear select result data */
    tdp_clear_select_result();
}

/* merge to left/middle item, left/middle item must be exist, if merge op finished, left/middle item will be clean */
static void tdp_merge_to_left_middle_items (GtkAction *action, GtkWidget *window)
{
    GList           *items;
    SelectInfo      *si = NULL;

    /* check the files associated with the action */
    items = tdp_check_associated_data (action, TDP_ACTION_NAME_MERGE_TO_LEFT_MIDDLE, 1);
    if (items == NULL)
    {
#ifdef G_ENABLE_DEBUG
        g_warning ("check [Merge to left & middle items] data fail\n");
#endif
        return;
    }
    si = g_list_nth_data(items, 0);
    if (m_stPerferenceInfo.select_left_item_type != m_stPerferenceInfo.select_middle_item_type)
    {
#ifdef G_ENABLE_DEBUG
        g_warning ("[Merge to left item] left type not equ middle item type [%d, %d]\n", 
                    m_stPerferenceInfo.select_left_item_type, m_stPerferenceInfo.select_middle_item_type);
#endif
        return;
    }
    /* if select left item type is not equ select item */
    if (m_stPerferenceInfo.select_left_item_type != si->type)
    {
#ifdef G_ENABLE_DEBUG
        g_warning ("[Merge to left item] left type not equ select item type [%d, %d]\n",
                    m_stPerferenceInfo.select_left_item_type, (int)(si->type));
#endif
        return;
    }

    /* start to merge */
    tdp_start_to_merge(m_stPerferenceInfo.select_left_item, m_stPerferenceInfo.select_middle_item, si->name);

    /* clear select result data */
    tdp_clear_select_result();
}

/* edit the system preferences. */
static void tdp_edit_preferences (GtkAction *action, GtkWidget *window)
{
    PerferenceDlg dlg;
    
    GtkWidget* pDlg = create_edit_perf_win(GTK_WINDOW(window), &dlg, &m_stPerferenceInfo);
    gtk_widget_show_all (pDlg);
    gtk_dialog_run(GTK_DIALOG( pDlg ) );
    gtk_widget_destroy (pDlg);
    
#ifdef G_ENABLE_DEBUG
    g_message ("plugin func exit");
#endif

    /* write config info */
    save_perf_config_info(&m_stPerferenceInfo, DEFAULT_PERFERENCE_INI);
}

static GList* tdp_file_info_list_copy(GList* fileinfo_list)
{
    gint length;
    SelectInfo* tmp_si;
    GList* result = NULL;

    SelectInfo* si;
    GList* items = fileinfo_list;
#ifdef G_ENABLE_DEBUG
    g_message ("copy glist: 0x%08x", (int)fileinfo_list);
#endif

    /* get all items */
    while(items != NULL)
    {
        /* get data */
        tmp_si = NULL;
        si = (SelectInfo*)g_list_nth_data(items, 0);
        if (NULL != si)
        {
            tmp_si = g_malloc(sizeof(SelectInfo));
            /* name is not null */
            if (NULL != si->name)
            {
                length = strlen(si->name);
                tmp_si->name = g_malloc(length + 1);
                memset(tmp_si->name, 0, length + 1);
                strncpy(tmp_si->name, si->name, length);
            }
            tmp_si->type = si->type;
        }
        result = g_list_append(result, tmp_si);
        /* next item */
        items = g_list_next(items);
    }
#ifdef G_ENABLE_DEBUG
    g_message ("copy glist result: 0x%08x", (int)result);
#endif

    return result;
}

static void tdp_file_info_list_free(GList* fileinfo_list)
{
    SelectInfo* si;
    GList* items = fileinfo_list;
#ifdef G_ENABLE_DEBUG
    g_message ("free glist: 0x%08x", (int)fileinfo_list);
#endif

    /* get all items */
    while(items != NULL)
    {
        /* get data */
        si = g_list_nth_data(items, 0);
        if (NULL != si)
        {
            /* name is not null */
            if (NULL != si->name)
            {
                g_free(si->name);
            }
            g_free(si);
        }
        /* next item */
        items = g_list_next(items);
    }
    /* free this list */
    g_list_free(fileinfo_list);
}

typedef void (*TDP_CALLBACK)(GtkAction *action, GtkWidget *window);

static GtkAction* tdp_add_one_action(gchar* name,  gchar* lable, gchar* stock_id_or_icon_name, 
                    gchar* tooltip, TDP_CALLBACK pfCallback, GtkWidget* window, GList* fileinfo_list)
{
    GtkAction  *action = NULL;

#ifdef G_ENABLE_DEBUG
    g_message ("glist: 0x%08x", (int)fileinfo_list);
    g_message ("icon-name: %s", stock_id_or_icon_name);
#endif

    /* append the action */
    action = g_object_new (GTK_TYPE_ACTION,
                            "name", name, //"Tdp::extract-here",
                            "label", lable, //_("Extract _Here"),
#if !GTK_CHECK_VERSION(2,9,0)
                            "stock-id", stock_id_or_icon_name, //"tdp-extract",
#else
                            "icon-name", stock_id_or_icon_name, //"tdp-extract",
#endif
                            "tooltip", tooltip,
                             NULL);
    g_object_set_data_full(G_OBJECT(action), name,  
                            tdp_file_info_list_copy(fileinfo_list), 
                            (GDestroyNotify)tdp_file_info_list_free);
    g_signal_connect (G_OBJECT (action), "activate", G_CALLBACK (pfCallback), G_OBJECT(window));

    return action;
}

static GList *tdp_provider_get_folder_actions(ThunarxMenuProvider *menu_provider, GtkWidget* window, ThunarxFileInfo* folders)
{
    GList *menus;
    GList *folder_list = NULL;
    folder_list = g_list_append(folder_list, folders);
    menus = tdp_provider_get_file_actions(menu_provider, window, folder_list);
    g_list_free(folder_list);
    return menus;
}

/*
 * ref wine: PathCompactPathExW
 */

static gchar* tdp_path_compact_chars(const gchar* strSrc, gint max_len)
{
    gint len;
    gchar* strResult;
    gchar* strTmp,*strTmp2;

    if (NULL == strSrc)
    {
        return NULL;
    }

    if (max_len < 2)
    {
        return NULL;
    }
        
    strResult = (gchar*)g_malloc(max_len+1);
    if (NULL == strResult)
    {
        return NULL;
    }
    /* set all buff to zero */
    memset(strResult, 0, max_len+1);

    len = strlen(strSrc) + 1;
    if (len < max_len)
    {
        /* Don't need to compact */
        memcpy(strResult, strSrc, len);
        return strResult;
    }
    /* get filename pos */
    strTmp = strrchr(strSrc,'/');
    if ((strTmp == strSrc)&&(len == max_len))
    {
        /* if start with /, and filename len equ max_len, return filename */
        memcpy(strResult, strTmp+1, max_len);
        return strResult;
    }
    /* if a rel pathname or root file or root folder */
    if ((NULL == strTmp) || (strTmp == strSrc))
    {        
        /* no space to fit it */
        if (max_len <= 4)
        {
            strTmp2 = strResult;
            while(--max_len > 0)
                *strTmp2++ = '.';
            return strResult;
        }
        max_len -= 4;
        /* copy a part of filename, maybe some bug in multibyte */
        memcpy(strResult, strTmp+1, max_len);
        /* append some dot */
        strTmp2 = strResult+max_len;
        *strTmp2++ = '.';
        *strTmp2++ = '.';
        *strTmp2++ = '.';
        return strResult;
    }
    /* strlen(.../XXX) > max_len */
    if (strSrc + len - strTmp + 3 > max_len)
    {
        strTmp2 = strResult;
        /* no space to fit it */
        if (max_len <= 4)
        {
            while(--max_len > 0)
                *strTmp2++ = '.';
            return strResult;
        }
        /* Compact the file name */
        *strTmp2++ = '.';
        *strTmp2++ = '.';
        *strTmp2++ = '.';
        *strTmp2++ = '/';
        max_len -= 4;
        if (max_len <= 4)
        {
            while(--max_len > 0)
                *strTmp2++ = '.';
            return strResult;
        }
        max_len -= 4;
        /* copy a part of filename, maybe some bug in multibyte */
        memcpy(strTmp2, strTmp+1, max_len);
        /* append some dot */
        strTmp2 += max_len;
        *strTmp2++ = '.';
        *strTmp2++ = '.';
        *strTmp2++ = '.';
        return strResult;
    }
    
    /* /xxx/.../yyy */
    strTmp2 = strResult;
    len = max_len - ((strSrc + len) - strTmp) - 3;
    memcpy(strTmp2, strSrc, len);
    strTmp2 += len;
    *strTmp2++ = '.';
    *strTmp2++ = '.';
    *strTmp2++ = '.';
    strcpy(strTmp2, strTmp);
    return strResult;
}

static GList* tdp_provider_get_file_actions (ThunarxMenuProvider *menu_provider, GtkWidget *window, GList *files)
{
    GList              *scan;
    GtkAction          *action;
    gint                i = 0;
    gint                n_files = 0;
    gint                type = 0;
    GList              *actions = NULL;
    SelectInfo         *si[3] = {0};
    GList              *fileinfo_list = NULL;
    gchar              *strUri;
    gchar              *strLeft, *strMiddle;
    gchar              strInfo[1024] = {0};
    
    /* check files param */
    if (NULL == files)
    {
#ifdef G_ENABLE_DEBUG
        g_warning ("files is null\n");
#endif
        return NULL;
    }
    
    /* only process select one-three item */
    n_files = g_list_length(files);
    if (3 < n_files)
    {
#ifdef G_ENABLE_DEBUG
        g_message ("files item is upper three items[%d]\n", n_files);
#endif
        return NULL;
    }

    /* retrive all supplied items */
    i = 0;
    scan = files;
    while (scan != NULL)
    {
        /* malloc SelectInfo buff */
        si[i] = g_malloc(sizeof(SelectInfo));

        /* check if this item is a file or folders */
        if (thunarx_file_info_is_directory(scan->data))
        {
            si[i]->type = 2;
            type = 1;
        }
        else
        {
            si[i]->type = 1;
            type = 0;
        }
        /* get uri */
        strUri = thunarx_file_info_get_uri(scan->data);
        /* get full path name */
        si[i]->name = g_filename_from_uri(strUri, NULL, NULL);
        /* free uri buffer */
        g_free(strUri);
#ifdef G_ENABLE_DEBUG
        g_message ("orginal name: [%s]", si[i]->name);
#endif
        /* append it to fileinfo_list */
        fileinfo_list = g_list_append(fileinfo_list, si[i]);
        /* next item */
        scan = g_list_next(scan);
        i++;
    }

    /* select_left is always exists, must select one item */
    if (1 == n_files)
    {
        if (is_check_item(&m_stPerferenceInfo, select_left_check))
        {
            action = tdp_add_one_action(TDP_ACTION_NAME_SELECT_LEFT, _("Select as _left"), TDP_ICON_NAME_SELECT_LEFT,
                                        dngettext (GETTEXT_PACKAGE, "Select the file as compare left", "Select the folder as compare left", type),
                                        tdp_select_as_left, window, fileinfo_list);
            actions = g_list_append (actions, action);
        }

        /* compare to left need to check select left result is exist, must select one item, */
        if (is_exist_select_left_item(&m_stPerferenceInfo) && (si[0]->type == get_select_left_item_type(&m_stPerferenceInfo)) )
        {
            if (is_check_item(&m_stPerferenceInfo, compare_to_left_check))
            {
                //strLeft = tdp_get_strip_ten_chars(m_stPerferenceInfo.select_left_item);
                strLeft = tdp_path_compact_chars(m_stPerferenceInfo.select_left_item, 20);
                snprintf(strInfo, 1023, _("_Compare to left item: %s"), strLeft);
                strInfo[1023] = '\0';
                g_free(strLeft);
                action = tdp_add_one_action(TDP_ACTION_NAME_COMPARE_TO_LEFT, strInfo, TDP_ICON_NAME_COMPARE_TO_LEFT,
                                            dngettext (GETTEXT_PACKAGE, "Compare to the file", "Compare to the folder", type),
                                            tdp_compare_to_left, window, fileinfo_list);
                actions = g_list_append (actions, action);
            }

            if (is_check_item(&m_stPerferenceInfo, select_to_middle_check))
            {
                action = tdp_add_one_action(TDP_ACTION_NAME_SELECT_MIDDLE, _("_Select as middle"), TDP_ICON_NAME_SELECT_MIDDLE,
                                            dngettext (GETTEXT_PACKAGE, "Select the file as merge middle", "Select the folder as merge middle", type),
                                            tdp_select_as_middle, window, fileinfo_list);
                actions = g_list_append (actions, action);
            }

            /* merge to left/middle item need to check select left/middle result is exist, must select one item */
            if (is_exist_select_middle_item(&m_stPerferenceInfo) && (si[0]->type == get_select_middle_item_type(&m_stPerferenceInfo)) )
            {
                if (is_check_item(&m_stPerferenceInfo, merge_to_left_middle_check))
                {
                    //strLeft   = tdp_get_strip_ten_chars(m_stPerferenceInfo.select_left_item);
                    strLeft = tdp_path_compact_chars(m_stPerferenceInfo.select_left_item, 20);
                    //strMiddle = tdp_get_strip_ten_chars(m_stPerferenceInfo.select_middle_item);
                    strMiddle = tdp_path_compact_chars(m_stPerferenceInfo.select_middle_item, 20);
                    snprintf(strInfo, 1023, _("_Merge to left & middle item: %s & %s"), strLeft, strMiddle);
                    strInfo[1023] = '\0';
                    g_free(strLeft);
                    g_free(strMiddle);
                    action = tdp_add_one_action(TDP_ACTION_NAME_MERGE_TO_LEFT_MIDDLE, strInfo, TDP_ICON_NAME_MERGE_TO_LEFT_MIDDLE,
                                                dngettext (GETTEXT_PACKAGE, "Merge to the files", "Merge to the folders", type),
                                                tdp_merge_to_left_middle_items, window, fileinfo_list);
                    actions = g_list_append (actions, action);
                }
            }
        }
    }
    else if (2 == n_files)
    {
        if (si[0]->type == si[1]->type)
        {
            if (is_check_item(&m_stPerferenceInfo, compare_two_items_check))
            {
                /* compare two item only check two items select?!, must select two items */
                action = tdp_add_one_action(TDP_ACTION_NAME_COMPARE_TWO_ITEMS, _("_Compare two items"), TDP_ICON_NAME_COMPARE_TWO_ITEMS,
                                        dngettext (GETTEXT_PACKAGE, "Compare two files", "Compare two folders", type),
                                        tdp_compare_two_items, window, fileinfo_list);
                actions = g_list_append (actions, action);
            }

            if (is_check_item(&m_stPerferenceInfo, select_two_item_to_merge_check))
            {
                /* select two item as left/middle to merge, must select two items */
                action = tdp_add_one_action(TDP_ACTION_NAME_SELECT_TWO_ITEMS_TO_MERGE, _("_Select two items as left & middle items"), TDP_ICON_NAME_SELECT_TWO_ITEMS_TO_MERGE,
                                        dngettext (GETTEXT_PACKAGE, "Select two files as left & middle files", "Select two folders as left & middle folders", type),
                                        tdp_select_two_items_to_merge, window, fileinfo_list);
                actions = g_list_append (actions, action);
            }

            /* select_two_items_to_merge  only check two items select?!, must select two items */
            if (is_exist_select_left_item(&m_stPerferenceInfo) && (si[0]->type == get_select_left_item_type(&m_stPerferenceInfo)) )
            {
                if (is_check_item(&m_stPerferenceInfo, merge_to_left_check))
                {
                    //strLeft = tdp_get_strip_ten_chars(m_stPerferenceInfo.select_left_item);
                    strLeft = tdp_path_compact_chars(m_stPerferenceInfo.select_left_item, 20);
                    snprintf(strInfo, 1023, _("_Merge two items to left item: %s"), strLeft);
                    strInfo[1023] = '\0';
                    g_free(strLeft);
                    action = tdp_add_one_action(TDP_ACTION_NAME_MERGE_TO_LEFT, strInfo, TDP_ICON_NAME_MERGE_TO_LEFT,
                                                dngettext (GETTEXT_PACKAGE, "Merge two files to left file", "Merge two folders to left folder", type),
                                                tdp_merge_to_left_item, window, fileinfo_list);
                    actions = g_list_append (actions, action);
                }
            }
        }
    }
    else if (3 == n_files)
    {
        /* merge three item only check three items select?!, must select three items */
        if ((si[0]->type == si[1]->type) && (si[1]->type == si[2]->type))
        {
            if (is_check_item(&m_stPerferenceInfo, merge_three_items_check))
            {
                action = tdp_add_one_action(TDP_ACTION_NAME_MERGE_THREE_ITEMS, _("_Merge three items"), TDP_ICON_NAME_MERGE_THREE_ITEMS,
                                            dngettext (GETTEXT_PACKAGE, "Merge three files", "Merge three folders", type),
                                            tdp_merge_three_items, window, fileinfo_list);
                actions = g_list_append (actions, action);
            }
        }
    }
    action = tdp_add_one_action(TDP_ACTION_NAME_EDIT_PREFERENCES, _("Edit the _Preferences"), TDP_ICON_NAME_EDIT_PREFERENCES,
                                dngettext (GETTEXT_PACKAGE, "Edit the preferences", "Edit the preferences", type),
                                tdp_edit_preferences, window, fileinfo_list);
    actions = g_list_append (actions, action);
    
    tdp_file_info_list_free(fileinfo_list);

    return actions;
}

