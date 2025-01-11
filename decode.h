#ifndef DECODE_H           //if macro not defined then.
#define DECODE_H        //defines macro

#include"types.h"       //contains user defined data types.

typedef struct _DecodeInfo
{
    /* User enterd Magic String */
    char *user_magic_string;
    uint user_magic_str_len;

    /*Extension Length and Extension */
    char *extension;
    uint extn_length;

    /* Secret File */
    char *secret_file_data;
    uint secret_file_len;


    /* Magic String */
    char *DECODED_MAGIC_STRING;
    uint decoded_magic_str_len;

    /* Encoded image */
    char *encoded_img_fname;
    FILE *fptr_encoded_img;

    /* Decoded data */
    //char *decoded_file_fname;
    char decoded_file_fname[200];
    FILE *fptr_decoded_file;

}DecodeInfo;


/* Decoding Function prototypes */

/* To validate the command line argumentss for decoding*/
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Get File pointers for Encoded Image(i/p file) 
    opens only encoded image pointer */
Status open_encoded_img_file(DecodeInfo *decInfo);

/* To skip header of the image b/c it is not required data */
Status header_skipper(DecodeInfo *decInfo);

/* Decodes magic string length */
Status decode_magic_string_len(DecodeInfo *decInfo);

/* Decodes magic string data */
Status decode_magic_string_data(DecodeInfo *decInfo);

/* Decodes file extension length */
Status decode_extn_len(DecodeInfo *decInfo);

/* Decodes extension data */
Status decode_extn_data(DecodeInfo *decInfo);

/* Decodes secret data length */
Status decode_secret_data_len(DecodeInfo *decInfo);

/* Decodes secret data */
Status decode_secret_data(DecodeInfo *decInfo);

// subfunctions for finding length and character from lsb of each byte.
/* Decodes or extracts intger(length) from lsb of encoded image */
uint decode_integer_from_lsb( DecodeInfo *decInfo);

/* Decodes or extracts actual data to from lsb of encoded image */
char decode_data_from_lsb(char ch, DecodeInfo *decInfo);

/* Open a new File pointer based on retrived extn from */
Status open_ouput_file_and_write(DecodeInfo *decInfo);

/* Actual Decoding happens here, all functions above are called from this function */
Status do_decoding(DecodeInfo *decInfo);

#endif