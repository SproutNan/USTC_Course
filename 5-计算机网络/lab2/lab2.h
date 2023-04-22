#include <string>
#include <vector>

#define MAX_CLIENT_NUMBER 32
#define MAX_SINGLE_MESSAGE_LENGTH 1025 // 缓冲区
#define EPOLL_SIZE MAX_CLIENT_NUMBER

typedef std::pair<bool, int> Client;
typedef struct timeval timev_t;
typedef struct epoll_event ep_evt;

// 按delimiter分割：copied from ta's lab2
std::pair<std::vector<std::string>, int> split(char *str, const std::string &delimiter)
{
    std::string s = str;
    std::vector<std::string> res;
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos)
    {
        token = s.substr(0, pos);
        res.push_back(token);
        s = s.substr(pos + delimiter.length());
    }
    if (s.length())
    {
        res.push_back(s);
    }
    return std::pair<std::vector<std::string>, int>(res, res.size());
}

