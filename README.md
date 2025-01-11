# LSB Image Steganography Project

## Overview
This project implements a **Least Significant Bit (LSB) Image Steganography** using the C programming language in which secret data is encoded in an `.bmp` image file with a passcode, which we can securely decode secret data from `.bmp` image files.
This project involves following concepts of c :
-  Command Line Arguments.
-  File Handling.
-  Strings.
-  Dyanamic Memory Allocation, Pointers.
-  Macros.
-  UDT (Structures, Enums, typedef).
-  Bitwise Operations.

---

## Operations
- **Encoding**: Embed/Encode secret data with a passcode into the least significant bits of a `.bmp` image file.
- **Decoding**: Retrieve the hidden data from a `.bmp` image file.
- **File Validation**: Ensure that the input image file is valid and meets the requirements for steganography.

---

## Project Structure
```
.
├── src
│   ├── main.c                # Entry point for the application.
│   ├── encode.c              # Functions for encoding data.
│   ├── decode.c              # Functions for decoding data.
│
├── include
│   ├── encode.h              # Header file contains all function prototypes,macros,structure for encoding.
│   ├── decode.h              # Header file contains all function prototypes,macros,structure for decoding
│   ├── types.h               # Header file contains macros and enums.
│
├── assets
│   ├── beautiful.bmp         # Sample input image.
│   ├── secret.txt            # Sample secret text.
```

---

### **For Encoding, format of passing Command Line Arguments**
```bash
./a.out -e <input_image.bmp> <secret_file.txt> <output_image.bmp>[optioanl]
```
- `input_image.bmp`: The original image to hide the data.
- `secret_file.txt`: The file containing the secret message.
- `output_image.bmp[optional]`: The image file with the hidden data. [if argument not passed, generates a "default_out_img.bmp"]

---

### **For Decoding, format of passing Command Line Arguments**
```bash
./a.out -d <encoded_image.bmp> <output_file.txt>[optional]
```
- `encoded_image.bmp`: The image containing the hidden data.
- `output_file.txt[optional]`: The file to save the retrieved secret message. [if argument not passed, generates a "default.txt" extracted data file.]

---

## Contact
- LinkedIn: http://www.linkedin.com/in/vamsisairamakrishna

