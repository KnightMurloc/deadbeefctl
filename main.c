//
// Created by victor on 05.07.2022.
//

#include <mqueue.h>
#include <string.h>

#define MAX_LEN 256

int main(int argc, char* argv[]){

    if(argc == 1){
        return 0;
    }

    struct mq_attr attr;

    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_LEN;
    attr.mq_curmsgs = 0;


    mqd_t qq = mq_open("/deadbeefctl", O_RDWR | O_CREAT, 0660, &attr);

    for (int i = 1; i < argc; ++i) {
        mq_send(qq, argv[i],strlen(argv[i]),0);
    }

    mq_close(qq);

    return 0;
}
