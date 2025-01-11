#include<stdio.h>
#include<string.h>
#include "decode.h"
#include "types.h"
#include<stdlib.h>

/* Function Defintions for Decoding */

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    //1. check for source file.
    //check for ".bmp" in the entered file name.
    char *bmp_substring = strstr(argv[2],".bmp");
    if(bmp_substring != NULL)
    {
        //not equal NULL means substring found.so, compare for trailing
        if(strcmp(bmp_substring,".bmp") == e_success)
        {
            //now copy this name to source_image_fname
            decInfo->encoded_img_fname=argv[2];
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

    //2. check for output file name.
    //check any argument is passed or not if not consider it as "default" 
    if(argv[3] == NULL)
    {
        strcpy(decInfo->decoded_file_fname,"default");
        printf("INFO: Output File not Mentioned, Creating a \"Default\" output Image File\n");
    }
    else
    {
        //no matter what the argument is even with the an extension copy to "decoded_file_fname".
        strcpy(decInfo->decoded_file_fname,argv[3]);
        printf("INFO: DONE..!");
    }
    printf("SUCCESS : Read and Validate Function Successfull\n");

    return e_success;
}

Status open_encoded_img_file(DecodeInfo *decInfo)
{
    printf("INFO: Opening \"%s\" Image..\n",decInfo->encoded_img_fname);
   
    // open encoded image & store it in file pointer.
    decInfo->fptr_encoded_img = fopen(decInfo->encoded_img_fname,"r");
    
    // error handling.
    if(decInfo->fptr_encoded_img == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->encoded_img_fname);
        return e_failure;
    }
    printf("INFO: DONE.\n");

    return e_success;
} 

Status header_skipper(DecodeInfo *decInfo)
{
    printf("INFO: Skipping Header..\n");
    //to skip header point file pointer to 54 using fseek function.
    fseek(decInfo->fptr_encoded_img, 54 , SEEK_SET);

    if(ftell(decInfo->fptr_encoded_img) != 54)
    {
        return e_failure;
    }
    printf("INFO: DONE.\n");
    
    return e_success;
}

Status decode_magic_string_len(DecodeInfo *decInfo)
{
    printf("INFO: Decoding Magic String..\n");

    //reading magic string(password) from user.
    decInfo->user_magic_string = calloc(100,1);
    printf("Enter MAGIC STRING (Password) : ");
    scanf(" %s",decInfo->user_magic_string);

    decInfo->user_magic_str_len = strlen(decInfo->user_magic_string);

    printf("INFO: Checking Magic String Length..\n");
    uint length = decode_integer_from_lsb(decInfo);
    
    //store in decInfo structure
    decInfo->decoded_magic_str_len = length;

    if(decInfo->user_magic_str_len != length )
    {
        printf("INFO: Entered Magic String Length is not Matching!\n");
        return e_failure;
    }

    printf("INFO: DONE.\n");
    return e_success;
}

uint decode_integer_from_lsb(DecodeInfo *decInfo)
{
    char integerbytes[32];
    fread(integerbytes,32,1,decInfo->fptr_encoded_img);
    int length=0;
    //decoding length
    for(int i=0; i<32; i++)
    {
       length = length | ((integerbytes[i] & 1)<<i);
    }
    return length;
}

Status decode_magic_string_data(DecodeInfo *decInfo)
{
    printf("INFO: Decoding Magic String Data..\n");
    //allocate memory dynamically.
    decInfo->DECODED_MAGIC_STRING = (char*)calloc(decInfo->decoded_magic_str_len+1,sizeof(char));

    for(int i=0; i<decInfo->decoded_magic_str_len; i++)
    {
        decInfo->DECODED_MAGIC_STRING[i] = decode_data_from_lsb(decInfo->DECODED_MAGIC_STRING[i] , decInfo);
    }

    if(strcmp(decInfo->DECODED_MAGIC_STRING , decInfo->user_magic_string) == e_success)
    {
        printf("INFO: DONE.\n");
        return e_success;
    }
    return e_failure;
}

char decode_data_from_lsb(char ch,DecodeInfo *decInfo)
{
    char characterbytes[8];
    // read 8 bytes to decode 8 bits -> 1byte => 1 character and return.
    fread(characterbytes,8,1,decInfo->fptr_encoded_img);
    for(int i=0;i<8;i++)
    {
        ch = ch | ((characterbytes[i] & 1)<<i);
    }

    return ch;
}

Status decode_extn_len(DecodeInfo *decInfo)
{
    printf("INFO: Decoding Secret File Extension Size..\n");
    //Decode extension length and store into decInfo structure.
    decInfo->extn_length = decode_integer_from_lsb(decInfo);
    
    printf("INFO: DONE.\n");
    return e_success;
}

Status decode_extn_data(DecodeInfo *decInfo)
{
    printf("INFO: Decoding Extension of Secret File..\n");
    //allocate memory dynamically for length of extension times
    decInfo->extension = (char*)calloc(decInfo->extn_length+1,sizeof(char));   

    //decoding extn data.
    for(int i=0; i<decInfo->extn_length; i++)
    {
        decInfo->extension[i] = decode_data_from_lsb(decInfo->extension[i] , decInfo); 
    } 
    printf("INFO: Done.\n");

    return e_success;
}

Status decode_secret_data_len(DecodeInfo *decInfo)
{
    printf("INFO: Decoding Secret File Size..\n");
    //call decode_integer_from_lsb which returns length.
    decInfo->secret_file_len = decode_integer_from_lsb(decInfo);
    printf("INFO: DONE.\n");

    return e_success;
}

Status decode_secret_data(DecodeInfo *decInfo)
{
    printf("Info: Decoding Secret Data..\n");
    //dynamically allocate memory for storing secret file data
    decInfo->secret_file_data = (char*)calloc(decInfo->secret_file_len+1,sizeof(char)); 
    
    //pass each byte of allocated memory for secret data to character decoding function.
    for(int i=0; i<decInfo->secret_file_len; i++)
    {
        decInfo->secret_file_data[i]  = decode_data_from_lsb(decInfo->secret_file_data[i],decInfo);
    }
    printf("INFO: DONE..!\n");

    return e_success;
}

Status open_ouput_file_and_write(DecodeInfo *decInfo)
{
    //concatenate the output file name with the retrived extension.
    strcpy( decInfo->decoded_file_fname, strcat(decInfo->decoded_file_fname,decInfo->extension) );

    printf("INFO: Opening output File..\n");
    decInfo->fptr_decoded_file = fopen(decInfo->decoded_file_fname,"w");
    
    //Error Handling
    if(decInfo->fptr_decoded_file == NULL)
    {
        printf("Opening output File Failed!\n");
        return e_failure;
    }
    printf("INFO: Generating \"%s\" File..\n",decInfo->decoded_file_fname);
    
    //writing secret data to output file. 
    fwrite(decInfo->secret_file_data,decInfo->secret_file_len,1,decInfo->fptr_decoded_file);
    printf("INFO: DONE.\n");
    
    //free the memory allocated dynamically.
    free(decInfo->DECODED_MAGIC_STRING);
    free(decInfo->extension);
    free(decInfo->secret_file_data);
    
    //close file pointers.
    fclose(decInfo->fptr_encoded_img);
    fclose(decInfo->fptr_decoded_file);

    return e_success;  
}

Status do_decoding(DecodeInfo *decInfo)
{
    //1. open enocoded image(stego_image.bmp)
    if(open_encoded_img_file(decInfo) == e_success)
    {   
        printf("## Decoding Procedure Started ##\n");
        //2. skip header
        if(header_skipper(decInfo) == e_success)
        {
            //3.decode length of magic string
            //to do that read read magic string from user.
            if(decode_magic_string_len(decInfo) == e_success)
            {
                //4.decode magic string based on length of user entered magic string.
                //store decoded magic string int an array.
                if(decode_magic_string_data(decInfo) == e_success)
                {
                    //5.Decode extension length of secret file
                    if(decode_extn_len(decInfo) == e_success)
                    {
                        //6. Decode Extension of secret file.
                        if(decode_extn_data(decInfo) == e_success)
                        {
                            //7.Decode sceret file length/size.
                            if(decode_secret_data_len(decInfo) == e_success)
                            {
                                //8.Decode Secret File Data.
                                if(decode_secret_data(decInfo) == e_success)
                                {
                                    /* 9. open output file pointer to store retrived data.
                                         open output file by concatinating extension to output file name entered
                                         and write decoded secret data to it. */
                                    if(open_ouput_file_and_write(decInfo) == e_success)
                                    {
                                        printf("\n## Decoding Done Sucessfully ##\n\n");
                                        return e_success;
                                    }
                                    else
                                    {
                                        printf("\n## Decoding Failed ##\n\n");
                                        return e_failure;
                                    }
                                }
                                else
                                {
                                    printf("INFO: Decoding Secret File Data Failed\n");
                                    return e_failure;
                                }

                            }
                            else
                            {
                                printf("INFO: Decoding Secret File Length/Size Failed\n");
                                return e_failure;
                            }
                        }
                        else
                        {
                            printf("Info: Decoding File Extension Failed\n");
                            return e_failure;
                        }
                    }
                    else
                    {
                        printf("INFO: Decoding File Extension Length Failed\n");
                        return e_failure;
                    }
                }
                else
                {
                    printf("INFO: ERROR, Entered Magic String(Password) is Incorrect\n");
                    return e_failure;
                }
            }
            else
            {
                printf("INFO: Decoding Magic String Length Failed\n");
                return e_failure;
            }
        }
        else
        {
            printf("INFO: Header Skip Failed..\n");
            return e_failure;
        }
    }
    else
    {
        printf("INFO: Opening Encoded Image Failed\n");
        return e_failure;
    } 
}