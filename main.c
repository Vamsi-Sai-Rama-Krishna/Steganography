/* 
Name: Vamsi Krishna
Description: To Implement a Stegnography Project, in which Information(confidentitial/secret data) 
             can be securely transimited by encoding into an image with an encoded password, typically an image of ".bmp" extension.
Topics Covered: Bitwise Operators, Arrays, Strings, Functions, Pointers, Structures, Dynamic Memory Allocation(DMA),
                File I/0, Command Line Arguments(CLA), Preprocessor Derectives.             
Date: 06/01/2025.
*/

#include <stdio.h>
#include <string.h>
/* includes 'encode.h','decode.h', 'types.h' file by fetching from current directory */
#include "encode.h"    
#include "decode.h"
#include "types.h"

/* read command line arguments. */
int main(int argc, char *argv[])
{
    EncodeInfo encInfo;
    DecodeInfo decInfo;

    /* check command line arguments */
    if(argc<2)
    {
        printf("Encoding : ./a.out -e <.bmp file> <.txt file> [output file]\n");
        printf("Decoding : ./a.out -d <.bmp file> [output file]\n");
        return e_failure;
    }
    else if(( strcmp(argv[1],"-e") == 0) &&  argc < 4)
    {
        printf("Encoding : ./a.out -e <.bmp file> <.txt file> [output file]\n");
        return e_failure;
    }
    else if (( strcmp(argv[1],"-d") == 0 ) && argc <3)
    {
        printf("Decoding : ./a.out -d <.bmp file> [output file]\n");
        return e_failure;
    }
  
    /* check operation type whether encoding or decoding then proceed. */
    printf("INFO: Checking Operation Type..\n");
    Status option = check_operation_type(argv);

    /* if "-e" then encode. */
    if (option == e_encode)   
    {
        /* validate command line arguments entered for encoding. */
        if(read_and_validate_encode_args(argv, &encInfo) == e_success)
        {
            /* perform encoding */
            if(do_encoding(&encInfo) == e_success)
            {
                return e_success;
            }
            else
            {
                return e_failure;
            }
        }
        else
        {
            printf("Encoding : ./a.out -e <.bmp file> <.txt file> [output file]\n");
            return e_failure;
        }
    }
    /* if "-d" then decode */
    else if (option == e_decode)
    {   
        // validate command line arguments entered for decoding. 
        if(read_and_validate_decode_args(argv,&decInfo) == e_success)
        {
            // perform decode
            if(do_decoding(&decInfo) == e_success)
            {
                return e_success;
            }
            else
            {
                return e_failure;
            }
        }
        else
        {
            printf("Decoding : ./a.out -d <.bmp file> [output file]\n");
            return e_failure;
        }
    }
    else
    {
        return e_failure;
    }
}

/* Check operation type */
OperationType check_operation_type(char *argv[])
{
    if( strcmp(argv[1],"-e") == 0) 
    {
        printf("SUCCESS: Check Operation Function Completed!\n");
        printf("------Your Choosen Option is ENCODE------\n");
        return e_encode;
    }
    else if( strcmp(argv[1],"-d") == 0 )  
    { 
        printf("SUCCESS: Check Operation Function Completed!\n");
        printf("------Your Choosen Option is DECODE------\n");
        return e_decode;
    }
    else 
    {
        printf("ERROR : Your Operation Should Be Either -e or -d\n");
        printf("Encoding : ./a.out -e <.bmp file> <.txt file> [output file]\n");
        printf("Decoding : ./a.out -d <.bmp file> [output file]\n");
        return e_unsupported;
    }  
}