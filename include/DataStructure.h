#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

#include <string>
#include <mutex>
#include <vector>

class DataStructure {
private:
    std::vector<int> fields;
    
    mutable std::mutex field0_mutex;
    mutable std::mutex field1_mutex;
    mutable std::mutex field2_mutex;
    
    int num_fields;

public:
    explicit DataStructure(int m);
    ~DataStructure() = default;

    int read(int field_index);
    void write(int field_index, int value);
    std::string toString() const;
    operator std::string() const;
};

#endif