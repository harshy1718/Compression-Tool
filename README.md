# Huffman Coding Project

## Overview
This project implements Huffman coding, a lossless data compression algorithm. It includes functionalities to compress a text file into a smaller binary file and decompress a binary file back to its original text form. The program features an interactive menu for easy selection between compression and decompression operations.

## Features
- **File Compression:** Compress text files to reduce storage size.
- **File Decompression:** Decompress binary files back to their original text form.
- **Interactive Menu:** User-friendly menu to select between compression and decompression operations.
- **Efficient Encoding and Decoding:** Utilizes a priority queue and binary tree for optimal performance.
- **File Handling:** Read and write files in binary format for effective compression.

## Getting Started

### Prerequisites
- C++ compiler (e.g., g++)
- Git (for version control)

### Compiling the Program
1. Clone the repository:
    ```sh
    git clone https://github.com/yourusername/HuffmanCoding.git
    cd HuffmanCoding
    ```
2. Compile the code:
    ```sh
    g++ -o huffman huffman.cpp
    ```

### Running the Program
1. Execute the program:
    ```sh
    ./huffman
    ```

2. Follow the on-screen instructions to compress or decompress files.

### Example
#### Compressing a File
1. Select the option to compress a file.
2. Provide the path to the input text file.
3. Provide the path to save the compressed binary file.

#### Decompressing a File
1. Select the option to decompress a file.
2. Provide the path to the compressed binary file.
3. Provide the path to save the decompressed text file.

## Project Structure
- `huffman.cpp`: The main source code file containing the implementation of Huffman coding, including compression and decompression functionalities.
- `README.md`: This file, providing an overview and instructions for the project.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments
- The Huffman coding algorithm was invented by David A. Huffman while he was a Ph.D. student at MIT.
