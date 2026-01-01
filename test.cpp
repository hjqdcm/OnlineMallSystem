#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
void buffer_overflow_vuln(const char* input) {
    char buffer[32];
    strcpy(buffer, input);  
    
    std::cout << "Buffer contains: " << buffer << std::endl;
}

void heap_overflow_vuln(const char* input) {
    int len = strlen(input);
    

    char* heap_buf = new char[16];

    strcpy(heap_buf, input); 
    
    std::cout << "Heap buffer: " << heap_buf << std::endl;
    delete[] heap_buf;
}

void integer_overflow_vuln(const char* input) {
    unsigned short size;
    
    if (strlen(input) >= 2) {
        memcpy(&size, input, 2);
    
        unsigned short new_size = size * 10;
        
        char* buf = new char[new_size];

        strcpy(buf, input); 
        
        std::cout << "Allocated " << new_size << " bytes" << std::endl;
        
        delete[] buf;
    }
}

void divide_by_zero_vuln(const char* input) {
    int divisor = input[0];

    int result = 100 / divisor;
    
    std::cout << "100 / " << divisor << " = " << result << std::endl;
}

void off_by_one_vuln(const char* input) {
    char buffer[40];
    int len = strlen(input);

    for (int i = 0; i <= 40; i++) {
        if (i < len)
            buffer[i] = input[i];
        else
            buffer[i] = '\0';
    }
    
    std::cout << "Result: " << buffer << std::endl;
}

int main(int argc, char* argv[]) {
    char input[4096];
    ssize_t bytes_read = read(STDIN_FILENO, input, sizeof(input) - 1);
    
    if (bytes_read <= 0) {
        std::cerr << "Failed to read input" << std::endl;
        return 1;
    }
    
    input[bytes_read] = '\0';
    
    std::cout << "Input length: " << bytes_read << std::endl;
    
    std::cout << "\n=== Triggering buffer overflow ===" << std::endl;
    buffer_overflow_vuln(input);
    
    std::cout << "\n=== Triggering heap overflow ===" << std::endl;
    heap_overflow_vuln(input);
    
    std::cout << "\n=== Triggering integer overflow ===" << std::endl;
    if (bytes_read >= 2) {
        integer_overflow_vuln(input);
    }
    
    std::cout << "\n=== Triggering off-by-one ===" << std::endl;
    off_by_one_vuln(input);

    std::cout << "\n=== Triggering divide by zero ===" << std::endl;
    if (input[0] != 0) {
        divide_by_zero_vuln(input);
    }
    
    return 0;
}