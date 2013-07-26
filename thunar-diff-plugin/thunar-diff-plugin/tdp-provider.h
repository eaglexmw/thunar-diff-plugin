/* $Id: tdp-provider.h,v 1.3 2013/06/04 14:40:43 setup Exp $ */
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

#ifndef __TDP_PROVIDER_H__
#define __TDP_PROVIDER_H__

#include <locale.h>
#include <libintl.h>
#include <thunarx/thunarx.h>

G_BEGIN_DECLS;

typedef struct _TdpProviderClass TdpProviderClass;
typedef struct _TdpProvider      TdpProvider;

#define TDP_TYPE_PROVIDER             (tdp_provider_get_type ())
#define TDP_PROVIDER(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), TDP_TYPE_PROVIDER, TdpProvider))
#define TDP_PROVIDER_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), TDP_TYPE_PROVIDER, TdpProviderClass))
#define TDP_IS_PROVIDER(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TDP_TYPE_PROVIDER))
#define TDP_IS_PROVIDER_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), TDP_TYPE_PROVIDER))
#define TDP_PROVIDER_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), TDP_TYPE_PROVIDER, TdpProviderClass))

GType tdp_provider_get_type      (void) G_GNUC_CONST G_GNUC_INTERNAL;
void  tdp_provider_register_type (ThunarxProviderPlugin *plugin) G_GNUC_INTERNAL;

#define _(str) gettext(str)

G_END_DECLS;

#endif /* !__TAP_PROVIDER_H__ */
