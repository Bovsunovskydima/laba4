// for lounch .\build\Release\thread_safe_demo.exe or rebuild the project
#include "DataStructure.h"
#include "FileGenerator.h"
#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <chrono>
#include <sstream>

struct Operation {
    std::string type;
    int field;
    int value;
};

std::vector<Operation> readOperationsFromFile(const std::string& filename) {
    std::vector<Operation> operations;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Cannot open file: " << filename << std::endl;
        return operations;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        Operation op;
        iss >> op.type;
        
        if (op.type == "read") {
            iss >> op.field;
        } else if (op.type == "write") {
            iss >> op.field >> op.value;
        }
        
        operations.push_back(op);
    }
    
    file.close();
    return operations;
}

void executeOperations(DataStructure& ds, const std::vector<Operation>& operations) {
    for (const auto& op : operations) {
        if (op.type == "read") {
            ds.read(op.field);
        } else if (op.type == "write") {
            ds.write(op.field, op.value);
        } else if (op.type == "string") {
            std::string result = ds.toString();
        }
    }
}

void runTest(const std::string& test_name, 
             const std::vector<std::string>& filenames,
             int num_fields,
             int num_threads) {
    std::cout << "\n=== " << test_name << " (" << num_threads << " thread(s)) ===" << std::endl;
    
    DataStructure ds(num_fields);
    std::vector<std::vector<Operation>> all_operations;
    
    for (const auto& filename : filenames) {
        all_operations.push_back(readOperationsFromFile(filename));
        std::cout << "Loaded " << all_operations.back().size() 
                  << " operations from " << filename << std::endl;
    }
    
    auto start = std::chrono::high_resolution_clock::now();
    
    if (num_threads == 1) {
        for (const auto& ops : all_operations) {
            executeOperations(ds, ops);
        }
    } else {
        std::vector<std::thread> threads;
        for (size_t i = 0; i < all_operations.size() && i < static_cast<size_t>(num_threads); ++i) {
            threads.emplace_back(executeOperations, std::ref(ds), std::cref(all_operations[i]));
        }
        
        for (auto& t : threads) {
            t.join();
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "Execution time: " << duration.count() << " ms" << std::endl;
    std::cout << "Final state: " << std::string(ds) << std::endl;
}

int main() {
    const int NUM_FIELDS = 3;
    const int NUM_OPERATIONS = 100000;
    
    std::cout << "Generating test files..." << std::endl;
    
    auto frequencies_a = FileGenerator::getVariant8Frequencies();
    FileGenerator::generateFile("res/scenario_a_thread1.txt", frequencies_a, NUM_FIELDS, NUM_OPERATIONS);
    FileGenerator::generateFile("res/scenario_a_thread2.txt", frequencies_a, NUM_FIELDS, NUM_OPERATIONS);
    FileGenerator::generateFile("res/scenario_a_thread3.txt", frequencies_a, NUM_FIELDS, NUM_OPERATIONS);
    
    auto frequencies_b = FileGenerator::getEqualFrequencies(NUM_FIELDS);
    FileGenerator::generateFile("res/scenario_b_thread1.txt", frequencies_b, NUM_FIELDS, NUM_OPERATIONS);
    FileGenerator::generateFile("res/scenario_b_thread2.txt", frequencies_b, NUM_FIELDS, NUM_OPERATIONS);
    FileGenerator::generateFile("res/scenario_b_thread3.txt", frequencies_b, NUM_FIELDS, NUM_OPERATIONS);
    
    auto frequencies_c = FileGenerator::getInverseFrequencies();
    FileGenerator::generateFile("res/scenario_c_thread1.txt", frequencies_c, NUM_FIELDS, NUM_OPERATIONS);
    FileGenerator::generateFile("res/scenario_c_thread2.txt", frequencies_c, NUM_FIELDS, NUM_OPERATIONS);
    FileGenerator::generateFile("res/scenario_c_thread3.txt", frequencies_c, NUM_FIELDS, NUM_OPERATIONS);
    
    std::cout << "Files generated successfully!\n" << std::endl;
    
    runTest("Scenario A (Variant 8 frequencies) - 1 thread", 
            {"res/scenario_a_thread1.txt"}, NUM_FIELDS, 1);
    runTest("Scenario A (Variant 8 frequencies) - 2 threads", 
            {"res/scenario_a_thread1.txt", "res/scenario_a_thread2.txt"}, NUM_FIELDS, 2);
    runTest("Scenario A (Variant 8 frequencies) - 3 threads", 
            {"res/scenario_a_thread1.txt", "res/scenario_a_thread2.txt", "res/scenario_a_thread3.txt"}, 
            NUM_FIELDS, 3);
    
    runTest("Scenario B (Equal frequencies) - 1 thread", 
            {"res/scenario_b_thread1.txt"}, NUM_FIELDS, 1);
    runTest("Scenario B (Equal frequencies) - 2 threads", 
            {"res/scenario_b_thread1.txt", "res/scenario_b_thread2.txt"}, NUM_FIELDS, 2);
    runTest("Scenario B (Equal frequencies) - 3 threads", 
            {"res/scenario_b_thread1.txt", "res/scenario_b_thread2.txt", "res/scenario_b_thread3.txt"}, 
            NUM_FIELDS, 3);
    
    runTest("Scenario C (Inverse frequencies) - 1 thread", 
            {"res/scenario_c_thread1.txt"}, NUM_FIELDS, 1);
    runTest("Scenario C (Inverse frequencies) - 2 threads", 
            {"res/scenario_c_thread1.txt", "res/scenario_c_thread2.txt"}, NUM_FIELDS, 2);
    runTest("Scenario C (Inverse frequencies) - 3 threads", 
            {"res/scenario_c_thread1.txt", "res/scenario_c_thread2.txt", "res/scenario_c_thread3.txt"}, 
            NUM_FIELDS, 3);
    
    return 0;
}
