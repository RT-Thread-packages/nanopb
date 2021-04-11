#include <rtthread.h>
#include <rtdevice.h>
#include <stdio.h>
#include <stdlib.h>
#include <pb_encode.h>
#include <pb_decode.h>
#include "amessage.pb.h"

static void nanopb_encode_decode(int argc,char *argv[])
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
    }
    
    /* Now we could transmit the message over network, store it in a file or
     * wrap it to a pigeon's leg.
     */

    /* But because we are lazy, we will just decode it immediately. */
    
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
MSH_CMD_EXPORT(nanopb_encode_decode, nanopb encode test);
