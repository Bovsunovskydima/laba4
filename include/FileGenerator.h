#ifndef FILEGENERATOR_H
#define FILEGENERATOR_H

#include <string>
#include <vector>

class FileGenerator {
public:
    struct OperationFrequencies {
        std::vector<double> read_frequencies;
        std::vector<double> write_frequencies;
        double string_frequency;
    };

    static void generateFile(const std::string& filename, 
                            const OperationFrequencies& frequencies,
                            int num_fields,
                            int num_operations);
    
    static OperationFrequencies getVariant8Frequencies();
    static OperationFrequencies getEqualFrequencies(int num_fields);
    static OperationFrequencies getInverseFrequencies();
};

#endif