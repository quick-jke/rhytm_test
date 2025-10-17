#ifndef INCLUDE_SIGN_PAIR
#define INCLUDE_SIGN_PAIR
#include <optional>
#include <string>

struct SignPair {
    std::optional<char> first;
    std::optional<char> second;

    bool isValid() const;

    std::string to_string() const;
};


#endif
