#include <vector>
#include <string>
using namespace std;
static inline auto split(string s, string delimiter) -> vector<string>{
    vector<string> lines;
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        lines.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    lines.push_back(s);
    return lines;
}
