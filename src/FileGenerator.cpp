#include "FileGenerator.h"
#include <fstream>
#include <random>
#include <stdexcept>

void FileGenerator::generateFile(const std::string& filename, 
                                const OperationFrequencies& frequencies,
                                int num_fields,
                                int num_operations) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    
    double total_frequency = frequencies.string_frequency;
    for (size_t i = 0; i < frequencies.read_frequencies.size(); ++i) {
        total_frequency += frequencies.read_frequencies[i] + frequencies.write_frequencies[i];
    }

    std::uniform_real_distribution<> dis(0.0, total_frequency);
    std::uniform_int_distribution<> value_dis(1, 100);

    for (int i = 0; i < num_operations; ++i) {
        double rand_val = dis(gen);
        double cumulative = 0.0;

        cumulative += frequencies.string_frequency;
        if (rand_val < cumulative) {
            file << "string\n";
            continue;
        }

        bool found = false;
        for (int field = 0; field < num_fields; ++field) {
            cumulative += frequencies.read_frequencies[field];
            if (rand_val < cumulative) {
                file << "read " << field << "\n";
                found = true;
                break;
            }

            cumulative += frequencies.write_frequencies[field];
            if (rand_val < cumulative) {
                file << "write " << field << " " << value_dis(gen) << "\n";
                found = true;
                break;
            }
        }
    }

    file.close();
}

FileGenerator::OperationFrequencies FileGenerator::getVariant8Frequencies() {
    OperationFrequencies frequencies;
    
    // Field 0: read 20%, write 10%
    frequencies.read_frequencies.push_back(20.0);
    frequencies.write_frequencies.push_back(10.0);
    
    // Field 1: read 25%, write 10%
    frequencies.read_frequencies.push_back(25.0);
    frequencies.write_frequencies.push_back(10.0);
    
    // Field 2: read 20%, write 10%
    frequencies.read_frequencies.push_back(20.0);
    frequencies.write_frequencies.push_back(10.0);
    
    // String: 5%
    frequencies.string_frequency = 5.0;
    
    return frequencies;
}

FileGenerator::OperationFrequencies FileGenerator::getEqualFrequencies(int num_fields) {
    OperationFrequencies frequencies;
    double frequency_per_op = 100.0 / (2 * num_fields + 1);
    
    for (int i = 0; i < num_fields; ++i) {
        frequencies.read_frequencies.push_back(frequency_per_op);
        frequencies.write_frequencies.push_back(frequency_per_op);
    }
    frequencies.string_frequency = frequency_per_op;
    
    return frequencies;
}

FileGenerator::OperationFrequencies FileGenerator::getInverseFrequencies() {
    // Inverse of variant 8
    OperationFrequencies frequencies;
    
    // Field 0: read 10%, write 20%
    frequencies.read_frequencies.push_back(10.0);
    frequencies.write_frequencies.push_back(20.0);
    
    // Field 1: read 10%, write 25%
    frequencies.read_frequencies.push_back(10.0);
    frequencies.write_frequencies.push_back(25.0);
    
    // Field 2: read 10%, write 20%
    frequencies.read_frequencies.push_back(10.0);
    frequencies.write_frequencies.push_back(20.0);
    
    // String: 5%
    frequencies.string_frequency = 5.0;
    
    return frequencies;
}
