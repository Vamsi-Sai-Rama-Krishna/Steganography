# Steganography

# LSB Image Steganography Project

## Overview
This project implements a **Least Significant Bit (LSB) Image Steganography System** using the C programming language. The application allows users to securely encode and decode secret messages into `.bmp` image files.
This project involves following concepts of c :
-  Command Line Arguments.
-  File Handling.
-  Strings.
-  Dyanamic Memory Allocation, Pointers.
-  Macros.
-  UDT (Structures, Enums, typedef).
-  Bitwise Operations.

---

## Features
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

### **Running the Encoder**
```bash
./steganography -e <input_image.bmp> <secret_file.txt> <output_image.bmp>
```
- `input_image.bmp`: The original image to hide the data.
- `secret_file.txt`: The file containing the secret message.
- `output_image.bmp`: The image file with the hidden data.

### **Running the Decoder**
```bash
./steganography -d <encoded_image.bmp> <output_file.txt>
```
- `encoded_image.bmp`: The image containing the hidden data.
- `output_file.txt`: The file to save the retrieved secret message.
