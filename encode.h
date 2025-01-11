#ifndef ENCODE_H       //checks for macro definition
#define ENCODE_H    //defines this macro if not defined.

#include<stdlib.h>
#include "types.h" // Contains user defined types


/* 
 * Structure to store information required for
 *      MAGIC STRING.
 *      secret file extension.
 *      source image --> name,file pointer to source image file.
 *      secret file  --> name,file pointer to secret file.
 *      encoded file --> name,file pointer to encoded file.
 */

typedef struct _EncodeInfo
{
    /* Magic String */
    char MAGIC_STRING[20];

    /* for storing secret file extension */
    char *secret_file_extn;

    /* Source Image info */
    char src_image_fname[100];
    FILE *fptr_src_image;

    /* Secret File Info */
    char secret_fname[200];
    FILE *fptr_secret;

    /* Stego Image Info */
    char stego_image_fname[200];
    FILE *fptr_stego_image;

} EncodeInfo;


/* Encoding Function prototype */

/* Read and validate Encode args from argv */
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo);

/* Check operation type */
OperationType check_operation_type(char *argv[]);

/* Get File pointers for i/p and o/p files */
Status open_files(EncodeInfo *encInfo);

/* check capacity */
Status check_capacity(EncodeInfo *encInfo);

//sub-function for check capacity
/* Get image size */
uint get_image_size_for_bmp(FILE *fptr_image);

//sub-function for check capacity
/* Get file size */
uint get_file_size(FILE *fptr);

/* Copy bmp image header */
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_stego_image);

/* Store Magic String length */
Status encode_magic_string_len(long len, EncodeInfo *encInfo);

/* Store Magic String data */
Status encode_magic_string(char *magic_string, EncodeInfo *encInfo);

/* Encode secret file extenstion length */
Status encode_secret_file_extn_len(const char *file_extn, EncodeInfo *encInfo);

/* Encode secret file extenstion */
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo);

/* Encode secret file size */
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo);


/* Copy remaining image bytes from src to stego image after encoding */
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest);

/* Encode a integer (4-bytes) into LSB of image data array */
Status encode_integer_to_lsb(long len, EncodeInfo *encInfo);

//sub---functions
/* Encode a string (1-byte) into LSB of image data array */
Status encode_string_to_lsb(char *data, EncodeInfo *encInfo);

/* Encode secret file data*/
Status encode_secret_file_data(EncodeInfo *encInfo);

/*Perform Encoding*/
Status do_encoding(EncodeInfo *encInfo);


#endif
