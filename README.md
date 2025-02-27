# FileCompression  

## Overview  
FileCompression is a Huffman coding-based file compression and decompression tool. It encodes files into a compressed binary format and is also capable of reconstructing them back to their original form. The program efficiently reduces file sizes by encoding frequently occurring characters with shorter bit sequences.  

## Features  
- Implements **Huffman Coding** for lossless file compression.  
- Custom **FancyInputStream** and **FancyOutputStream** classes for bit-level operations.  
- Stores and reconstructs Huffman trees for efficient decoding.  
- Handles both empty and non-empty files gracefully.  

## Usage
./compressor input_file compressed_file
./decompressor compressed_file output_file

