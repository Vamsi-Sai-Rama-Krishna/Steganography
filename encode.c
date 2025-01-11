#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"

/* Function Definitions for Encoding*/

/* To validate the command line argumnets for encoding */
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{  
    printf("INFO: Opening Required Files..\n");
    //1.argv[2] is valid file or not 
    //check "bmp" substring in source file. 
    //also compare to conform there is no trailing character at the end.

    char *bmp_substring = strstr(argv[2],".bmp");
    if(strstr(argv[2],".bmp") != NULL)
    {
        //not equal NULL means substring found. so,compare for trailing
        if(strcmp(strstr(argv[2],".bmp"),".bmp") == e_success)
        {   
            //now copy this name to source_image_fname
            strcpy(encInfo->src_image_fname,argv[2]);
            //2.check secret file extension and compare 
            // char *txt_substring = strstr(argv[3],".");
            if(strstr(argv[3],".") != NULL)
            {
                //not equal NULL means substring found.so, compare for trailing
                //now copy this name to secret_fname
                strcpy(encInfo->secret_fname,argv[3]);
            }
            else
            {
                return e_failure;
            }
        }
        else
        {
            return e_failure;
        }
    }
    else
    {
        return e_failure;
    }

    //3.if argv[4] is NULL then there is no argument passed for "output image file"
    if(argv[4] == NULL)
    {
        printf("INfo: Output File is not Mentioned. Creating \"Default_img.bmp\" as default.\n");
        strcpy(encInfo->stego_image_fname,"default_img.bmp");
    }
    else
    {
        //check if user passed output file name with bmp or not
        char *bmp_output_substring = strstr(argv[4],".bmp");
        //not equal NULL means substring found.so, compare for trailing
        if(bmp_output_substring != NULL && (strcmp(bmp_output_substring,".bmp") == e_success))
        {
            //printf("INFO: Valid Output Image File");
            //now copy this name to stego_image_fname
            strcpy(encInfo->stego_image_fname,argv[4]);
        }
        else
        {
            //printf("INFO: Output Image File with Improper Extension\n");
            return e_failure;
        } 
    }
    return e_success;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file \"%s\"\n", encInfo->src_image_fname);

    	return e_failure;
    }
    printf("INFO: Opened \"%s\"\n",encInfo->src_image_fname);

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file \"%s\"\n", encInfo->secret_fname);

    	return e_failure;
    }
    printf("INFO: Opened \"%s\"\n",encInfo->secret_fname);

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file \"%s\"\n", encInfo->stego_image_fname);

    	return e_failure;
    }
    printf("INFO: Opened \"%s\"\n",encInfo->stego_image_fname);
    
    // No failure return e_success
    printf("INFO: DONE.\n");
    return e_success;
}

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);

    // Return image capacity
    return width * height * 3;
}

uint get_file_size(FILE *fptr)
{
    fseek(fptr,0,SEEK_END); // brings offset to EOF
    return ftell(fptr);     //gives the position of offset which is equal to file_size; 
}

Status check_capacity(EncodeInfo *encInfo)
{
    //read magic string from user.
    printf("Enter ""MAGIC STRING"": ");
    scanf(" %s",encInfo->MAGIC_STRING);
    printf("INFO: Checking Capacity..\n");
    printf("INFO: Checking for \"%s\" file size..\n",encInfo->secret_fname);

    //secret file size
    // get file size of (secret file.) using function --> secret_file_size.
    uint secret_file_size = get_file_size(encInfo -> fptr_secret);

    if(secret_file_size == 0)
    {
        printf("INFO: \"%s\" is empty\n",encInfo->secret_fname);
        return e_failure;
    }
    else
    {
        printf("INFO: DONE. \"%s\" Not Empty\n",encInfo->secret_fname); 
    }  

    //source file size
    // get file size of source file(image file.) using function --> get_image_size_for_bmp.
    printf("INFO: Checking for \"%s\" File Capacity to Handle \"%s\"\n",encInfo->src_image_fname,encInfo->secret_fname);
    uint img_size = get_image_size_for_bmp(encInfo->fptr_src_image);

    encInfo->secret_file_extn = strstr(encInfo->secret_fname,".");
    if(encInfo->secret_file_extn == NULL)
    {
        printf("INFO: Couldn't find Extension of \"%s\"\n",encInfo->secret_fname);
        return e_failure;
    }

    /*
        required_size = (4 * 8) + (4 * 8) + (4 * 8) + for storing length i.e integer = 4 bytes * 8 =>32 bits
                        (magic string len * 8) +    for storing every character in magic string.
                        (secret file extn len * 8) +    for storing every character in secret file extn.
                        (secret file len * 8)      for storing every character in secret file.
    */

    // caliculate for required size.
    uint required_size = (4 + 4 + 4 + strlen(encInfo->MAGIC_STRING) + strlen(encInfo->secret_file_extn) + secret_file_size)*8;    

    printf("INFO: DONE.\n");
    if( required_size < img_size) //if the required size is less than image file size then we can do encoding
    {
        printf("INFO: File \"%s\" can handle capacity of \"%s\"\n",encInfo->src_image_fname,encInfo->secret_fname);
	    return e_success;
    }
    else
    {
        printf("INFO: File \"%s\" can't handle capacity of \"%s\"\n",encInfo->src_image_fname,encInfo->secret_fname);
	    return e_failure;
    }
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_stego_image)
{
    printf("INFO: Copying Header (54 bytes)\n");
    char header[54];
        //point the offset to start using rewind function.
        //because "fptr_src_image" is pointing to EOF as of previous operation.
        rewind(fptr_src_image);    

        //read 54 bytes at 1 time from the source file(fptr_src_image).
        fread(header, 54 , 1, fptr_src_image);  

        //write 54 bytes at 1 time to the output file(fptr_stego_image).
        fwrite(header, 54, 1, fptr_stego_image); 
        
        //check offset value whether pointing to the same point/index if not return failure.
        if(ftell(fptr_src_image) != ftell(fptr_stego_image))
        {
            return e_failure;
        }
        printf("INFO: DONE.\n");
        return e_success;       //return e_success at the end.
}

Status encode_magic_string_len(long len, EncodeInfo *encInfo)
{
    //call encode_integer_to_lsb function
    encode_integer_to_lsb(len, encInfo);

    if(ftell(encInfo->fptr_src_image) != ftell(encInfo->fptr_stego_image))
    {
        return e_failure;
    }
;
    return e_success;
}

Status encode_integer_to_lsb(long len, EncodeInfo *encInfo)
{
    //read 32 bytes from source image and store it in an array
    char integerbytes[32];
    fread(integerbytes,32,1,encInfo->fptr_src_image);

    for(int i=0;i<32;i++)
    {
	    integerbytes[i] = (integerbytes[i] & (~1) ) | ((len & (1 << i)) >> i);  
    }

    //write those 32-bits from integerbytes[32] array to file stream of outputfile(encoded image)
    fwrite(integerbytes,32,1,encInfo->fptr_stego_image);

    return e_success;
}

Status encode_magic_string( char *magic_string, EncodeInfo *encInfo)
{
    printf("INFO: Encoding Magic String Signature..\n");
    
    //call encode_character_to_lsb function.
    encode_string_to_lsb(magic_string, encInfo);
    
    if(ftell(encInfo->fptr_src_image) != ftell(encInfo->fptr_stego_image))
    {
        return e_failure;
    }

    printf("INFO: DONE.\n"); 
    return e_success;
}

Status encode_string_to_lsb(char *data, EncodeInfo *encInfo)
{
    //array to store bytes of data read from source file.
    char characterbytes[8];   
    //run loop for length of data pointer times.
    //out loop indicates for one character inner loop runs 8 times --> 8-bit times
    for(int i=0; i<strlen(data); i++)
    {
        //read 8 bytes from source file
        fread(characterbytes, 8 , 1 , encInfo->fptr_src_image);
        //again run loop for each character conatain 8 bits = 1byte 
        for(int j=0;j<8;j++)
        {
            characterbytes[j] = characterbytes[j] & (~1) |  ((data[i] & (1<<j)) >> j);    //replace the lsb with our data string of each character. 
        }
        //write those 8 bytes to output file after encoding.
        fwrite(characterbytes, 8 , 1 , encInfo->fptr_stego_image);
    }
    return e_success;
}

Status encode_secret_file_extn_len(const char *file_extn, EncodeInfo *encInfo)  
{
    //call encode integer to lsb function.
    encode_integer_to_lsb(strlen(file_extn), encInfo);
    
    if(ftell(encInfo->fptr_src_image) != ftell(encInfo->fptr_stego_image))
    {
        return e_failure;
    }

    return e_success;
}

Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    printf("INFO: Encoding \"%s\" File Extension..\n",encInfo->secret_fname);
    encode_string_to_lsb(encInfo->secret_file_extn,encInfo);
    
    if(ftell(encInfo->fptr_src_image) != ftell(encInfo->fptr_stego_image))
    {
        return e_failure;
    }

    printf("INFO: DONE.\n");
    return e_success;
}

Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    encode_integer_to_lsb(file_size,encInfo);

    if(ftell(encInfo->fptr_src_image) != ftell(encInfo->fptr_stego_image))
    {
        return e_failure;
    }

    return e_success;
}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    printf("INFO: Encoding \"%s\" File Data..\n",encInfo->secret_fname);
    int total_bytes = get_file_size(encInfo -> fptr_secret);
    char secret_data[total_bytes];
    // Move file pointer to the start position.
	fseek(encInfo -> fptr_secret,0,SEEK_SET); 

    // reading data of size of file at one time.
    fread( secret_data, total_bytes , 1 ,encInfo -> fptr_secret ); 

    //call function to encode secret data.
    encode_string_to_lsb(secret_data, encInfo);

    if(ftell(encInfo->fptr_src_image) != ftell(encInfo->fptr_stego_image))
    {
        return e_failure;
    }
	
    printf("INFO: DONE.\n");
    return e_success;
}

Status copy_remaining_img_data(FILE *fptr_src_image, FILE *fptr_stego_image)
{
    printf("INFO: Copying Left Over Data..\n");
    char ch;
    //read one time one byte to ch variable (8 bits/1 byte). 
    while(fread(&ch, 1, 1, fptr_src_image)>0)   
    {
        //write to output file i.e stego_image.
        fwrite(&ch, 1, 1, fptr_stego_image);  
    }

    if(ftell(fptr_src_image) != ftell(fptr_stego_image))
    {
        return e_failure;
    }

    printf("INFO: DONE.\n");
    return e_success;
}

Status do_encoding(EncodeInfo *encInfo)
{
    //open required file
    if(open_files(encInfo) == e_success)
    {
        printf("## Encoding Procedure Started ##\n");
        //check capacity
        if(check_capacity(encInfo) == e_success)    //if true.
        {
            // copy_bmp_header
            if(copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_success)     //if true.
            {
                //encode magic string len(int) (beacuse it tells how many next bytes should to fetch while decoding).
                if(encode_magic_string_len(strlen(encInfo->MAGIC_STRING),encInfo) == e_success)        //if true.
                {
                    // encode magic string(char)
                    if( encode_magic_string(encInfo->MAGIC_STRING, encInfo) == e_success)
                    {
                        //encode secret file extension length(int)
                        if( encode_secret_file_extn_len(encInfo->secret_file_extn, encInfo) == e_success)
                        {
                            //encode secret file extension.
                            if(encode_secret_file_extn(encInfo->secret_file_extn,  encInfo) == e_success )
                            {
                                //encode secret file size.(int)
                                if(encode_secret_file_size(get_file_size(encInfo -> fptr_secret), encInfo) == e_success)
                                {
                                    //encode secret file data.
                                    if( encode_secret_file_data(encInfo) == e_success)
                                    {
                                        //copy remaining data.
                                        if(copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
                                        {
                                            printf("\n## Encoding done sucessfully !!... ##\n\n");
                                            return e_success;
                                        }
                                        else
                                        {
                                            printf("\n## Encoding Failed !!... ##\n");
                                            return e_failure;
                                        }
                                    }
                                    else
                                    {
                                        printf("INFO: ERROR, Encoding Secret File Data Failed\n");
                                        return e_failure;
                                    }
                                }
                                else
                                {
                                    printf("INFO: ERROR, Encoding Secret File Size Failed\n");
                                    return e_failure;
                                }
                            }
                            else
                            {
                                printf("INFO: ERROR, Encoding Secret File Extension Failed\n");
                                return e_failure;
                            }
                        }
                        else
                        {
                            printf("INFO: ERROR, Encoding Secret File Extension Length Failed\n");
                            return e_failure;
                        }
                    }
                    else
                    {
                        printf("INFO: ERROR, Encoding Magic String Failed\n");
                        return e_failure;
                    }
                }
                else
                {
                    printf("INFO: ERROR, Encoding Magic String Length Failed\n");
                    return e_failure;
                }
            }
            else 
            {
                printf("INFO: ERROR, Coding Header Of Source File Failed\n");
                return e_failure;
            } 
        }
        else
        {
            printf("INFO: %s can't handle capacity of %s\n",encInfo->src_image_fname,encInfo->secret_fname);
            return e_failure;
        }
    }
    else
    {
        printf("INFO: ERROR, ""open_files"" function failed\n");
        return e_failure;
    }
}