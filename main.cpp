#include <vector>
#include <stdint.h>
#include <list>
#include <tuple>

#include "function.h"

int main() {
    print_ip(static_cast<uint8_t>(-1));
    print_ip(uint16_t{0});
    print_ip(uint32_t{2130706433});
    print_ip(uint64_t{8875824491850138409});  
    print_ip(std::string("Hello, World!"));
    print_ip(std::vector<int>{100, 200, 300, 400});
    print_ip(std::list<short>{400, 300, 200, 100});
    print_ip(std::make_tuple(123,456, 789, 0));

    return 0;
}