#include <deadbeef/deadbeef.h>
#include <gtk/gtk.h>
#include <deadbeef/gtkui_api.h>
#include <mqueue.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#define MAX_LEN 256

static DB_functions_t *deadbeef;
static ddb_gtkui_t *gtkui_plugin;
static struct DB_plugin_s *overlay_plugin;
static mqd_t qq;
static char msg[MAX_LEN];
static pthread_t thread;

static void* loop(void *pVoid){
    struct mq_attr attr;

    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_LEN;
    attr.mq_curmsgs = 0;

    while (1){
        memset(msg,0,sizeof(msg));
        mq_receive(qq, msg, MAX_LEN, NULL);

        if(strcmp(msg,"stop") == 0){
            printf("stop\n");
            break;
        }

        if(strcmp(msg, "toggle_window") == 0){
            gtkui_plugin->mainwin_toggle_visible();
        }

        if(strcmp(msg, "show_overlay") == 0){
            if(overlay_plugin)
                overlay_plugin->get_actions(NULL)->callback(NULL,NULL);
        }

        printf("msg: %s\n", msg);
    }
    return NULL;
}

static int start(){
    gtkui_plugin = (ddb_gtkui_t *) deadbeef->plug_get_for_id ("gtkui3_1");
    overlay_plugin = deadbeef->plug_get_for_id ("overlay");
    if (!gtkui_plugin) {
        return -1;
    }

    if(!overlay_plugin){
        printf("overlay plugin not found\n");
    }

    struct mq_attr attr;

    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_LEN;
    attr.mq_curmsgs = 0;

    qq = mq_open("/deadbeefctl", O_RDWR | O_CREAT, 0660, &attr);

    pthread_create(&thread,NULL,loop,NULL);

    return 0;
}

static int stop(){
    mq_send(qq,"stop",strlen("stop"),0);

    pthread_join(thread,NULL);

    mq_close(qq);

    mq_unlink("/deadbeefctl");

    return 0;
}

static DB_misc_t plugin = {
        .plugin = {
                .api_vmajor = 1,
                .api_vminor = 10,
                .id = NULL,
                .name = "DeadBeefCtl",
                .descr = "control deadbeef via cmd",
                .copyright = "Murloc Knight",
                .website = "https://github.com/KnightMurloc/DeadBeef-X11-Overlay-Plugin-",
                .command = NULL,
                .start = start,
                .stop = stop,
                .connect = NULL,
                .disconnect = NULL,
                .exec_cmdline = NULL,
                .get_actions = NULL,
                .message = NULL,
        }
};

extern DB_plugin_t *deadbeefctl_load(DB_functions_t *api) {
    deadbeef = api;
    return &plugin.plugin;
}