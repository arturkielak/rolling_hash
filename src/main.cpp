#include <iostream>
#include "signature_builder.h"
#include "delta_builder.h"
#include "serializable_types.h"

void usage() {
    using namespace std;
    std::cout << "Usage:\n"
                 "\trolling_hash -s old_file signature_file\n" \
                "\trolling_hash -d signature_file new_file delta_file\n";
}

int main(int argc, char **argv) {
    if (argc != 4 && argc != 5) {
        usage();
        return EXIT_FAILURE;
    }
    if (std::string(argv[1]) == "-s") {
        File old_file(argv[2], std::ios_base::in);
        File signature_file(argv[3], std::ios_base::out);
        Signatures signatures = SignatureBuilder::build(old_file);
        signatures.serialize(signature_file);
    } else if (std::string(argv[1]) == "-d") {
        File signatures_file(argv[2], std::ios_base::in);
        File new_file(argv[3], std::ios_base::in);
        File delta_file(argv[4], std::ios_base::out);
        Signatures signatures;
        signatures.deserialize(signatures_file);
        Deltas deltas = DeltaBuilder::build(signatures, new_file);
        deltas.serialize(delta_file);
    } else {
        //TODO patch command -p
        usage();
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
