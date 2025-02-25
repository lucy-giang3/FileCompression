#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_map>
#include "HCTree.hpp"
#include "Helper.hpp"

using namespace std;

void write_header(const vector<int>& freqs, FancyOutputStream& output_file, int num_symbols) {
    // write size of vector 
    output_file.write_int(freqs.size());
    // write number of symbols
    output_file.write_int(num_symbols);
    // traverse and write each frequency into the vector
    for (int freq : freqs) {
        output_file.write_int(freq);
    }
}

int main(int argc, char* argv[]) {
    // parse input for incorrect usage
    if (argc != 3) {
        std::cerr << "USAGE: " << argv[0] << " input_file compressed_file" << std::endl;
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

    // read input file and count bytes
    std::vector<int> freqs(256, 0); // 256
    int byte_count;
    bool is_input_empty = true;
    while ((byte_count = input_file.read_byte()) != EOF) {
        ++freqs[byte_count];
        is_input_empty = false;
    }

    // count number of symbols 
    int num_symbols = 0;
    for (int freq : freqs) {
        num_symbols += freq;
    }
    
    // check for an empty input file 
    if (is_input_empty) {
        // Create an empty output file
        FancyOutputStream output_file(output_name);
        if (!output_file.good()) {
            error("can't create output file");
        }

        return 0;
    }

    // use byte count to construct a Huffman tree
    HCTree htree;
    htree.build(freqs);

    // open output file for writing
    FancyOutputStream output_file(output_name);
    if (!output_file.good()) {
        error("can't open output file");
    }
    
    // write the header into output file
    write_header(freqs, output_file, num_symbols);

    // move back to the beginning of the input file
    input_file.reset();

    // write everencoded symbol to output file
    int symbol;
    while ((symbol = input_file.read_byte()) != EOF) {
        htree.encode(symbol, output_file);
    }

    return 0;
}
