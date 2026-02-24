#include "constructor_utils.hpp"
#include "tree.hpp" 
#include <stdexcept>


bool constructorUtils::parseEdgeLine(const std::string& line, int& value, int& left, int& right) 
{
    std::istringstream iss(line);
    if (!(iss >> value >> left >> right)) 
    {
        return false;
    }
    return true;
}