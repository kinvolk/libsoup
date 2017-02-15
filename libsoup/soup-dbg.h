/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */

#include <stdio.h>

#include <glib.h>

#include "soup-message-queue.h"
#include "soup-message.h"
#include "soup-uri.h"

#ifndef SOUP_DBG_H
#define SOUP_DBG_H 1

G_BEGIN_DECLS

const gchar * soup_dbg_message_queue_item_state_to_string (SoupMessageQueueItemState state);

#define SOUP_DBG_FILTER_SB(msg)						\
	if (!msg ||							\
	    !soup_message_get_uri (msg) ||				\
	    !g_str_has_suffix (soup_uri_get_path(soup_message_get_uri (msg)), "/superblock")) \
		break

#define SOUP_DBG_PRINTF(...)						\
	G_STMT_START {							\
		gchar *sdp_str = g_strdup_printf (__VA_ARGS__);		\
		printf ("\nfunction: %s\nline: %d\nthread: %p\n%s\n",	\
			G_STRFUNC,					\
			__LINE__,					\
			g_thread_self (),				\
			sdp_str);					\
		g_free (sdp_str);					\
	} G_STMT_END

#define SOUP_DBG_MSG_PRINTF(msg, ...)					\
	G_STMT_START {							\
		gchar *sdmp_uristr;					\
		gchar *sdmp_str;					\
		SoupURI *sdmp_uri = soup_message_get_uri (msg);		\
		/*SOUP_DBG_FILTER_SB (msg);*/				\
		if (sdmp_uri) {						\
			sdmp_uristr = soup_uri_to_string (sdmp_uri, FALSE); \
		} else {						\
			sdmp_uristr = g_strdup ("<NONE>");		\
		}							\
		sdmp_str = g_strdup_printf (__VA_ARGS__);		\
		SOUP_DBG_PRINTF ("uri: %s\nmsg: %p\n%s\n",		\
				 sdmp_uristr,				\
				 msg,					\
				 sdmp_str);				\
		g_free (sdmp_str);					\
		g_free (sdmp_uristr);					\
	} G_STMT_END

#define SOUP_DBG_ITEM_DUMP(item, new_state)				\
	SOUP_DBG_MSG_PRINTF (item->msg,					\
			     "item: %p\nold: %s\nnew: %s\nasync pending: %s\n",	\
			     item,					\
			     soup_dbg_message_queue_item_state_to_string (item->state), \
			     soup_dbg_message_queue_item_state_to_string (new_state), \
			     item->async_pending ? "true" : "false")

G_END_DECLS

#endif /* SOUP_DBG_H */
