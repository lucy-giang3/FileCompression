#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "Helper.hpp"
#include "HCTree.hpp"

/* reads the header of the compressed file */
vector<int> read_header(FancyInputStream& input_file, int& num_symbols) {
    // read size of vector
    int freq_size = input_file.read_int();
    // read number of symbols
    num_symbols = input_file.read_int();
    // create a frequencies vector 
    vector<int> freqs(freq_size);
    // fill vector 
    for (int i = 0; i < freq_size; i++) {
        freqs[i] = input_file.read_int();
    }
    return freqs;
}

/* reconstructs the huffman tree using the information from the header */
void reconstruct_tree(HCTree& htree, FancyInputStream& input_file, int& num_symbols) {
    vector<int> freqs = read_header(input_file, num_symbols);
    htree.build(freqs);
}

/* uses a reconstructed tree to decode a file */
void decode_file(FancyInputStream& input_file, FancyOutputStream& output_file, HCTree& htree, int num_symbols) {
    for (int i = 0; i < num_symbols; i++) {
        output_file.write_byte(htree.decode(input_file));
    }
}

int main(int argc, char* argv[]) {
    // parse input for incorrect usage
    if (argc != 3) {
        std::cerr << "USAGE: " << argv[0] << " compressed_file output_file" << std::endl;
        return 1;
    }

    // assign input to variables
    const char* input_name = argv[1];
    const char* output_name = argv[2];

    // open input file for reading
    FancyInputStream input_file(input_name);
    if (!input_file.good()) {
       error("can't open input file");
    }

    // check for an empty input file 
    bool is_empty = true;
    while (input_file.read_byte() != EOF) {
        is_empty = false;
    }
    input_file.reset();
    if (is_empty) {
        // Create an empty output file
        FancyOutputStream output_file(output_name);
        if (!output_file.good()) {
            error("can't open output file");
        }
        return 0;
    }
    
    // reconstruct Huffman tree using the header 
    int num_symbols = 0;
    HCTree htree;
    reconstruct_tree(htree, input_file, num_symbols);

    // open output file for writing
    FancyOutputStream output_file(output_name);
    if (!output_file.good()) {
        error("can't open output file");
    }
    // decode symbols and write to output
    decode_file(input_file, output_file, htree, num_symbols);

    return 0;
}

