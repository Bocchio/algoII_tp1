#ifndef DICTIONARY_HPP__
#define DICTIONARY_HPP__

#include <string>

using std::string;

template <typename T>
class Dictionary {
    static const int FREE = -1;
    static const size_t DEFAULT_SIZE = 20;
 public:
    size_t size;
    T *table;
    long *hashes;
    Dictionary() {
        size = DEFAULT_SIZE;
        table = new T[size];
        hashes = new long[size];
        for (size_t i = 0; i < size; i++) {
            hashes[i] = FREE;
        }
    }

    void append(string key, const T& value) {
        long hash = Dictionary::hash(key);
        size_t pos = hash%size;

        for (; pos < size; pos++) {
            if (hashes[pos] == hash)  // unsafe?
                return;
            hashes[pos] = hash;
            table[pos] = value;
        }
    }

    T& operator[](string key) const {
        long hash = Dictionary::hash(key);
        size_t pos = hash%size;

        for (; hashes[pos%size] != FREE; pos++) {
            if (hashes[pos%size] == hash)
                return table[pos%size];
        }

        // Qué devolver acá? posibilidad excepción
    }

    static long hash(string str) {
        long hash = 5381;

        for (size_t i = 0; i < str.length(); i++)
            hash = ((hash << 5) + hash) + str[i];  // hash * 33 + str[i]

        return hash;
    }
};

#endif  // DICTIONARY_HPP_

