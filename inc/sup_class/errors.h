#include <stdexcept>
#include <string>

class FileException : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class ParseException : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class MemoryException : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};