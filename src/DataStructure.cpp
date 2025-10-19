#include "DataStructure.h"
#include <sstream>
#include <stdexcept>

DataStructure::DataStructure(int m) : num_fields(m) {
    if (m != 3) {
        throw std::invalid_argument("This implementation supports only m=3 fields");
    }
    fields.resize(m, 0);
}

int DataStructure::read(int field_index) {
    if (field_index < 0 || field_index >= num_fields) {
        throw std::out_of_range("Field index out of range");
    }
    
    if (field_index == 0) {
        std::lock_guard<std::mutex> lock(field0_mutex);
        return fields[field_index];
    } else if (field_index == 1) {
        std::lock_guard<std::mutex> lock(field1_mutex);
        return fields[field_index];
    } else {
        std::lock_guard<std::mutex> lock(field2_mutex);
        return fields[field_index];
    }
}

void DataStructure::write(int field_index, int value) {
    if (field_index < 0 || field_index >= num_fields) {
        throw std::out_of_range("Field index out of range");
    }
    
    if (field_index == 0) {
        std::lock_guard<std::mutex> lock(field0_mutex);
        fields[field_index] = value;
    } else if (field_index == 1) {
        std::lock_guard<std::mutex> lock(field1_mutex);
        fields[field_index] = value;
    } else {
        std::lock_guard<std::mutex> lock(field2_mutex);
        fields[field_index] = value;
    }
}

std::string DataStructure::toString() const {
    std::ostringstream oss;
    oss << "[";
    
    {
        std::lock_guard<std::mutex> lock(field0_mutex);
        oss << fields[0];
    }
    
    oss << ", ";
    
    {
        std::lock_guard<std::mutex> lock(field1_mutex);
        oss << fields[1];
    }
    
    oss << ", ";
    
    {
        std::lock_guard<std::mutex> lock(field2_mutex);
        oss << fields[2];
    }
    
    oss << "]";
    return oss.str();
}

DataStructure::operator std::string() const {
    return toString();
}
