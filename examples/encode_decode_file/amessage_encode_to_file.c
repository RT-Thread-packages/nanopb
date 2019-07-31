#include <rtthread.h>
#include <rtdevice.h>
#include <stdio.h>
#include <stdlib.h>
#include <pb_encode.h>
#include <pb_decode.h>
#include "amessage.pb.h"
#include <dfs_posix.h>

static void nanopb_encode_to_file(int argc,char *argv[])
{
    /* This is the buffer where we will store our message. */
    uint8_t buffer[128];
    size_t message_length;
    bool status;
    
    /* Encode our message */
    {
        /* Allocate space on the stack to store the message data.
         *
         * Nanopb generates simple struct definitions for all the messages.
         * - check out the contents of simple.pb.h!
         * It is a good idea to always initialize your structures
         * so that you do not have garbage data from RAM in there.
         */
        AMessage amessage = AMessage_init_zero;
        
        /* Create a stream that will write to our buffer. */
        pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
        
        if (argc != 3)
        {
            rt_kprintf("usage: amessage aa bb\n");
            return;
        }
        amessage.aa = atoi(argv[1]);
        amessage.bb = atoi(argv[2]);
        
        /* Now we are ready to encode the message! */
        status = pb_encode(&stream, AMessage_fields, &amessage);
        message_length = stream.bytes_written;
        
        /* Then just check for any errors.. */
        if (!status)
        {
            rt_kprintf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
            return;
        }

        rt_kprintf("Writing %d serialized bytes\n", message_length); // See the length of message  
        int fd = open("/amessage.onnx", O_WRONLY | O_CREAT | O_TRUNC);
        if (fd>= 0)
        {
            write(fd, buffer, message_length);
            close(fd);
            rt_kprintf("Write done.\n");
        }
    }

    return;
}
MSH_CMD_EXPORT(nanopb_encode_to_file, nanopb encode to file);
