#include <iostream>
#include <cstring>
#include <string>

char *strcat_r(char *dest, int bufsz,const char *src){ 
    int new_buff_size = strlen(dest) + strlen(src) + 1;
    if (new_buff_size > bufsz)
        
        dest = static_cast<char *>(realloc(dest, new_buff_size));
    strcat(dest, src);
    return dest;
}

int main() {
    size_t BUFSIZE = 12; 
    char *buffer = new char[BUFSIZE];
    buffer[0] = '\0';
    strcat(buffer, "Hello");
    const char *str = ", world!";
    char * res = strcat_r(buffer, BUFSIZE, str);
    std::cout << res << std::endl;
    delete [] res;
    return 0;
}