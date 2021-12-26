#include <libwebsockets.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>

#include "server.h"

pthread_t ws_thread;

struct msg {
        void *payload; /* is malloc'd */
        size_t len;
};

/* one of these is created for each client connecting to us */

struct per_session_data__minimal {
        struct per_session_data__minimal *pss_list;
        struct lws *wsi;
        int last; /* the last message number we sent */
};

/* one of these is created for each vhost our protocol is used with */

struct per_vhost_data__minimal {
        struct lws_context *context;
        struct lws_vhost *vhost;
        const struct lws_protocols *protocol;

        struct per_session_data__minimal *pss_list; /* linked-list of live pss*/

        struct msg amsg; /* the one pending message... */
        int current; /* the current message number we are caching */
};

/* destroys the message when everyone has had a copy of it */

static void
__minimal_destroy_message(void *_msg)
{
        struct msg *msg = _msg;

        free(msg->payload);
        msg->payload = NULL;
        msg->len = 0;
}

static int
callback_lws_libusb(struct lws *wsi, enum lws_callback_reasons reason,
                        void *user, void *in, size_t len)
{
        struct per_session_data__minimal *pss =
                        (struct per_session_data__minimal *)user;
        struct per_vhost_data__minimal *vhd =
                        (struct per_vhost_data__minimal *)
                        lws_protocol_vh_priv_get(lws_get_vhost(wsi),
                                        lws_get_protocol(wsi));
        int m;

        switch (reason) {
        case LWS_CALLBACK_PROTOCOL_INIT:
                vhd = lws_protocol_vh_priv_zalloc(lws_get_vhost(wsi),
                                lws_get_protocol(wsi),
                                sizeof(struct per_vhost_data__minimal));
                vhd->context = lws_get_context(wsi);
                vhd->protocol = lws_get_protocol(wsi);
                vhd->vhost = lws_get_vhost(wsi);
                break;

        case LWS_CALLBACK_ESTABLISHED:
                /* add ourselves to the list of live pss held in the vhd */
                lws_ll_fwd_insert(pss, pss_list, vhd->pss_list);
                pss->wsi = wsi;
                pss->last = vhd->current;
                break;

        case LWS_CALLBACK_CLOSED:
                /* remove our closing pss from the list of live pss */
                lws_ll_fwd_remove(struct per_session_data__minimal, pss_list,
                                  pss, vhd->pss_list);
                break;

        case LWS_CALLBACK_SERVER_WRITEABLE:
                if (!vhd->amsg.payload)
                        break;

                if (pss->last == vhd->current)
                        break;

                /* notice we allowed for LWS_PRE in the payload already */
                m = lws_write(wsi, ((unsigned char *)vhd->amsg.payload) +
                              LWS_PRE, vhd->amsg.len, LWS_WRITE_TEXT);
                if (m < (int)vhd->amsg.len) {
                        lwsl_err("ERROR %d writing to ws\n", m);
                        return -1;
                }

                pss->last = vhd->current;
                break;

        case LWS_CALLBACK_RECEIVE:
                if (vhd->amsg.payload)
                        __minimal_destroy_message(&vhd->amsg);

                vhd->amsg.len = len;
                /* notice we over-allocate by LWS_PRE */
                vhd->amsg.payload = malloc(LWS_PRE + len);
                if (!vhd->amsg.payload) {
                        lwsl_user("OOM: dropping\n");
                        break;
                }

                memcpy((char *)vhd->amsg.payload + LWS_PRE, in, len);
                vhd->current++;

                /*
                 * let everybody know we want to write something on them
                 * as soon as they are ready
                 */
                lws_start_foreach_llp(struct per_session_data__minimal **,
                                      ppss, vhd->pss_list) {
                        lws_callback_on_writable((*ppss)->wsi);
                } lws_end_foreach_llp(ppss, pss_list);
                break;

        default:
                break;
        }

        return 0;
}

static struct lws_protocols protocols[] = {
        { "http", lws_callback_http_dummy, 0, 0, 0, NULL, 0},
        { "lws-libusb", callback_lws_libusb, sizeof(struct per_session_data__minimal), 128, 0, NULL, 0 },
        LWS_PROTOCOL_LIST_TERM
};

static const struct lws_http_mount mount = {
        /* .mount_next */		NULL,		/* linked-list "next" */
        /* .mountpoint */		"/",		/* mountpoint URL */
        /* .origin */			"./webapp",  /* serve from dir */
        /* .def */			"index.html",	/* default filename */
        /* .protocol */			NULL,
        /* .cgienv */			NULL,
        /* .extra_mimetypes */		NULL,
        /* .interpret */		NULL,
        /* .cgi_timeout */		0,
        /* .cache_max_age */		0,
        /* .auth_mask */		0,
        /* .cache_reusable */		0,
        /* .cache_revalidate */		0,
        /* .cache_intermediaries */	0,
        /* .origin_protocol */		LWSMPRO_FILE,	/* files in a dir */
        /* .mountpoint_len */		1,		/* char count */
        /* .basic_auth_login_file */	NULL,
};

void * ws_thread_func(struct lws_context *context)
{
        int n = 0;

        while (n >= 0) {
                n = lws_service(context, 0);
        }

        return NULL;
}

int ws_init()
{
        struct lws_context_creation_info info;
        struct lws_context *context;
        const char *p;
        int n = 0, logs = LLL_USER | LLL_ERR | LLL_WARN | LLL_NOTICE;
         
        memset(&info, 0, sizeof info); /* otherwise uninitialized garbage */
        info.port = 7681;
        info.mounts = &mount;
        info.protocols = protocols;
        info.vhost_name = "localhost";

        info.options =
                LWS_SERVER_OPTION_HTTP_HEADERS_SECURITY_BEST_PRACTICES_ENFORCE;

/*
#if defined(LWS_WITH_TLS)
        lwsl_user("Server using TLS\n");
        info.options |= LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;
        info.ssl_cert_filepath = "localhost-100y.cert";
        info.ssl_private_key_filepath = "localhost-100y.key";
#endif
*/
        /*
        if (lws_cmdline_option(argc, argv, "-h"))
                info.options |= LWS_SERVER_OPTION_VHOST_UPG_STRICT_HOST_CHECK;

        if (lws_cmdline_option(argc, argv, "-v"))
                info.retry_and_idle_policy = &retry;
        */
       
        context = lws_create_context(&info);
        if (!context) {
                lwsl_err("lws init failed\n");
                return 1;
        }

        pthread_create(&ws_thread, NULL, 
                (void * (*)(void *))ws_thread_func,
                (void *)context);

        return 0;
}

void ws_shutdown()
{
        pthread_cancel(ws_thread);
}
