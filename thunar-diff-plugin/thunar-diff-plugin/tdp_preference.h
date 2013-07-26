/* $Id: tdp_preference.h,v 1.12 2013/07/20 14:01:31 setup Exp $ */
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

#ifndef __TDP_PREFERENCE_H__
#define __TDP_PREFERENCE_H__

#include <locale.h>
#include <libintl.h>

G_BEGIN_DECLS;

#define _(str) gettext(str)
#define CFG_GROUPS              "common"
#define DEFAULT_PERFERENCE_INI  "thunar-diff-plugin.ini"
#define FUNC_HIDE               __attribute__ ((visibility ("hidden")))

typedef struct tagPerferenceInfo
{
    gchar* compare_edt;
    gchar* three_way_compare_edt;
    int keep_files_check:1;
    int select_left_check:1;
    int compare_to_left_check:1;
    int compare_two_items_check:1;
    int select_to_middle_check:1;
    int select_two_item_to_merge_check:1;
    int merge_to_left_check:1;
    int merge_to_left_middle_check:1;
    int merge_three_items_check:1;

    int select_left_item_type:2;
    int select_middle_item_type:2;
    gchar* select_left_item;
    gchar* select_middle_item;
}PerferenceInfo;

typedef struct tagPerferenceDlg
{
    GtkWidget *compare_edt;
    GtkWidget *three_way_compare_edt;
    GtkWidget *keep_files_check;
    GtkWidget *select_left_check;
    GtkWidget *compare_to_left_check;
    GtkWidget *compare_two_items_check;
    GtkWidget *select_to_middle_check;
    GtkWidget *select_two_item_to_merge_check;
    GtkWidget *merge_to_left_check;
    GtkWidget *merge_to_left_middle_check;
    GtkWidget *merge_three_items_check;
    PerferenceInfo* pstInfo;
    GtkWidget *pDialog;
}PerferenceDlg;

GtkWidget* create_edit_perf_win (GtkWindow *parent, PerferenceDlg *pDlg, PerferenceInfo *pInfo) FUNC_HIDE;

void load_perf_config_info(PerferenceInfo *pInfo, const gchar* str_perf_file) FUNC_HIDE;

void save_perf_config_info(const PerferenceInfo *pInfo, const gchar* str_perf_file) FUNC_HIDE;

#define is_check_item(pInfo , item)             (0 != ( pInfo )-> item )
#define is_exist_select_left_item(pInfo)        is_check_item(pInfo, select_left_item_type)
#define is_exist_select_middle_item(pInfo)      is_check_item(pInfo, select_middle_item_type)
#define get_select_left_item_type(pInfo)        ((pInfo)->select_left_item_type)
#define get_select_middle_item_type(pInfo)      ((pInfo)->select_middle_item_type)

G_END_DECLS;

#endif /* !__TDP_PREFERENCE_H__ */

