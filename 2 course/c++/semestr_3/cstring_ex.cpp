#include <cstring>
#include <iostream>

int find_last(const char *word){
    int idx = 0;
    for(; word[idx] != '\0'; ++idx);

    return idx - 1; 
}

void reverse_word(const char *text, const char *word, char *dest) {
    std::strcpy(dest, text);
    int last = find_last(word);
    for (int i = 0; ;){
        char *first = std::strstr(dest + i, word);
        for(int j = last; j >= 0; --j){
            *first++ = word[j];
            
        }
    }
}

int main() {
    char const *text = "Hello, world!";
    char const *word = "world";
    char res[1024];
    reverse_word(text, word, res);
    std::cout << res << std::endl;
    return 0;
}