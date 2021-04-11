#include <rtthread.h>
#include <rtdevice.h>
#include <stdio.h>
#include <stdlib.h>
#include <pb_encode.h>
#include <pb_decode.h>
#include "amessage.pb.h"
#include <dfs_posix.h>

static void nanopb_decode_from_file(int argc,char *argv[])
{
    /* This is the buffer where we will store our message. */
    int ret;
    struct stat buf;
    ret = stat("/amessage.onnx", &buf);
    if(ret == 0)
    {
        rt_kprintf("amessage.onnx file size = %d\n", buf.st_size);
    }
    else
    {
        rt_kprintf("amessage.onnx file not fonud\n");
    }

    size_t message_length = buf.st_size;
    uint8_t buffer[128];
    bool status;

    int fd = open("/amessage.onnx", O_RDONLY);
    if (fd>= 0)
    {
        int size = read(fd, buffer, message_length);
        close(fd);
        rt_kprintf("Read from file amessage.onnx \n");
        if (size < 0)
            return ;
    }

    {
        /* Allocate space for the decoded message. */
        AMessage amessage = AMessage_init_zero;
        
        /* Create a stream that reads from the buffer. */
        pb_istream_t stream = pb_istream_from_buffer(buffer, message_length);
        
        /* Now we are ready to decode the message. */
        status = pb_decode(&stream, AMessage_fields, &amessage);
        
        /* Check for errors... */
        if (!status)
        {
            rt_kprintf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
            return ;
        }
        
        /* Print the data contained in the message. */
        rt_kprintf("amessage.aa = %d\n", (int)amessage.aa);
        rt_kprintf("amessage.bb = %d\n", (int)amessage.bb);
    }

    return;
}
MSH_CMD_EXPORT(nanopb_decode_from_file, nanopb decode from file);
