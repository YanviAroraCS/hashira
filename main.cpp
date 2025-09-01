#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "json.hpp" // Make sure json.hpp is in same folder

using json = nlohmann::json;

// Manual base conversion: string -> long long
long long convertToDecimal(const std::string& value, int base) {
    long long result = 0;
    for (size_t i = 0; i < value.size(); i++) {
        char c = value[i];
        int digit;
        if (c >= '0' && c <= '9') digit = c - '0';
        else if (c >= 'a' && c <= 'z') digit = 10 + (c - 'a');
        else if (c >= 'A' && c <= 'Z') digit = 10 + (c - 'A');
        else {
            std::cerr << "Invalid digit: " << c << std::endl;
            exit(1);
        }
        if (digit >= base) {
            std::cerr << "Digit " << c << " not valid in base " << base << std::endl;
            exit(1);
        }
        result = result * base + digit;
    }
    return result;
}

int main() {
    // Read entire JSON from stdin
    json j;
    std::cin >> j;

    int n = j["keys"]["n"];
    int k = j["keys"]["k"];

    // Collect roots
    std::vector<long long> roots;
    for (json::iterator it = j.begin(); it != j.end(); ++it) {
        if (it.key() == "keys") continue;
        int base = std::stoi(it.value()["base"].get<std::string>());
        std::string numStr = it.value()["value"].get<std::string>();
        roots.push_back(convertToDecimal(numStr, base));
    }

    if (roots.size() < (size_t)k) {
        std::cerr << "Not enough roots to determine polynomial" << std::endl;
        return 1;
    }

    // Since it's a quadratic: F(x) = x^2 + bx + c
    // If r1 and r2 are roots: sum = -b , product = c
    long long r1 = roots[0];
    long long r2 = roots[1];
    long long c = r1 * r2;

    std::cout << c << std::endl;
    return 0;
}
