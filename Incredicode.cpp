#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>

//const std::string allowed = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ,./<>?;:\\\"'!@#$%^&*()-_=+`~|0123456789 []{}ÇüéâäàåçêëèïîìÄÅÉæÆôöòûùÿÖÜø£€Ø×ƒáíóúñÑÁÂÀĞÊËÈÍÎÏÌÓßÔÒõÕµşŞÚÛÙıİ";
std::string allowed = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ,./<>?;:\\\"'!@#$%^&*()-_=+`~|0123456789 []{}";

int key;
std::string keystr;
const int keyprecisionprime = 31; // must be prime
const int maxkeylength = 10000;
const int srandexponent = 2;

double mod(double a, double b) { return a - b * floor(a / b); }

bool is_number(const std::string& s) {
    return !s.empty() && find_if(s.begin(),
        s.end(), [](unsigned char c) { return !isdigit(c); }) == s.end();
}
int keygen(std::string s) {
    int result = 0;
    for (char c : s)
        result = result * keyprecisionprime + c;
    return mod(result, maxkeylength);
}
std::string shuffle(std::string str)
{
    // Seed the random number generator with a fixed value
    std::mt19937 rng;
    rng.seed(pow(key, srandexponent));
    // Shuffle the std::string using the Fisher-Yates algorithm
    for (size_t i = str.size() - 1; i > 0; --i) {
        int j = mod(rng(), (i + 1));
        std::swap(str[i], str[j]);
    }

    return str;
}

std::string unshuffle(std::string str)
{
    // Seed the random number generator with a fixed value
    std::mt19937 rng;
    rng.seed(pow(key, srandexponent));

    // Create a vector to store the indices of the characters in the shuffled std::string
    std::vector<int> indices(str.size());
    for (int i = 0; i < str.size(); ++i)
        indices[i] = i;

    // Shuffle the indices using the Fisher-Yates algorithm
    for (size_t i = str.size() - 1; i > 0; --i) {
        int j = mod(rng(), (i + 1));
        std::swap(indices[i], indices[j]);
    }

    // Use the shuffled indices to restore the original order of the characters
    std::string unshuffled(str.size(), ' ');
    for (int i = 0; i < str.size(); ++i)
        unshuffled[indices[i]] = str[i];

    return unshuffled;
}

// This function takes a message and a block size as input and returns the padded message
std::string addPadding(const std::string& message, int blockSize) {
    std::string padded = message;
    for (int i = 0; i < blockSize; i++)
        padded += ' ';
    return padded;
}

// This function takes a padded message as input and returns the original message without padding
std::string removePadding(const std::string& padded, int blockSize) {
    std::string message = padded;
    for (int i = 0; i < blockSize; i++)
        if (message.back() == ' ')
            message.pop_back();
    return message;
}

std::string applyXor(const std::string& message, const std::string& key) {
    std::string encrypted;
    encrypted.reserve(message.size());

    for (std::size_t i = 0; i < message.size(); ++i) {
        char c = message[i] ^ key[i % key.size()];
        if (allowed.find(c) == std::string::npos)
            c = allowed[c % allowed.size()];
        encrypted += c;
    }

    return encrypted;
}


std::string XOR_Encryption(const std::string& toBeEncrypted, const std::string& sKey)
{
    std::string sEncrypted(toBeEncrypted);
    unsigned int iKey(sKey.length()), iIn(toBeEncrypted.length()), x(0);
    for (unsigned int i = 0; i < iIn; i++)
    {
        sEncrypted[i] = toBeEncrypted[i] ^ sKey[x];
        if (++x == iKey) { x = 0; }
    }

    return sEncrypted;
}

void stream2hex(const std::string str, std::string& hexstr, bool capital = false)
{
    hexstr.resize(str.size() * 2);
    const size_t a = capital ? 'A' - 1 : 'a' - 1;

    for (size_t i = 0, c = str[0] & 0xFF; i < hexstr.size(); c = str[i / 2] & 0xFF)
    {
        hexstr[i++] = c > 0x9F ? (c / 16 - 9) | a : c / 16 | '0';
        hexstr[i++] = (c & 0xF) > 9 ? (c % 16 - 9) | a : c % 16 | '0';
    }
}

void hex2stream(const std::string hexstr, std::string& str)
{
    str.resize((hexstr.size() + 1) / 2);

    for (size_t i = 0, j = 0; i < str.size(); i++, j++)
    {
        str[i] = (hexstr[j] & '@' ? hexstr[j] + 9 : hexstr[j]) << 4, j++;
        str[i] |= (hexstr[j] & '@' ? hexstr[j] + 9 : hexstr[j]) & 0xF;
    }
}

std::string XorEncrypt(const std::string& message, const std::string& key) {
    std::string encrypted_message = XOR_Encryption(message, key);
    stream2hex(encrypted_message, encrypted_message, true);
    return encrypted_message;
}

std::string XorDecrypt(const std::string& message, const std::string& key) {
    std::string decryptedMessage = message;
    hex2stream(decryptedMessage, decryptedMessage);
    decryptedMessage = XOR_Encryption(decryptedMessage, key);
    return decryptedMessage;
}

std::string encrypt(const std::string& message, bool XOR = true) {
    //std::string padded = addPadding(message, blockSize);
    std::string shuffled = shuffle(message);
    std::string xored = shuffled;
    if (XOR)
        xored = XorEncrypt(shuffled, keystr);
    return xored;
}

std::string decrypt(const std::string& encrypted, bool XOR = true) {
    std::string unxored = encrypted;
    if (XOR)
        unxored = XorDecrypt(encrypted, keystr);
    std::string unshuffled = unshuffle(unxored);
    //std::string unpadded = removePadding(unshuffled, blockSize);
    return unshuffled;
}

std::string encodeThis(const std::string& s, bool XOR = true) {
    // Apply encryption and character shifting
    std::string encrypted = encrypt(s, XOR);
    std::string final;
    for (int i = 0; i < encrypted.length(); i++) {
        // Find the index of the character in the allowed string
        int index = allowed.find(encrypted[i]);
        if (index == -1) {
            return "ERROR: Invalid input.";
        }
        // Shift the index by the key and wrap it around the length of the allowed string
        index = (index + (i + 2 + key * (i + 1))) % allowed.length();
        final += allowed[index];
    }
    for (int i = 1; i < final.length(); i++) {
        int oldindex = allowed.find(final[i - 1]);
        int index = allowed.find(final[i]);
        index = (index + oldindex) % allowed.length();
        final[i] = allowed[index];
    }
    for (int i = final.length() - 2; i >= 0; i--) {
        int oldindex = allowed.find(final[i + 1]);
        int index = allowed.find(final[i]);
        index = (index + oldindex) % allowed.length();
        final[i] = allowed[index];
    }
    // Shuffle the encrypted message
    return shuffle(final);
}

std::string decodeThis(std::string s, bool XOR = true) {
    s = unshuffle(s);
    std::string olds = s;
    for (int i = s.length() - 2; i >= 0; i--) {
        int oldindex = allowed.find(olds[i + 1]);
        int index = allowed.find(olds[i]);
        int nindex = mod((index - oldindex), allowed.length());
        s[i] = allowed[nindex];
    }
    olds = s;
    for (int i = 1; i < s.length(); i++) {
        int oldindex = allowed.find(olds[i - 1]);
        int index = allowed.find(olds[i]);
        index = mod((index - oldindex), allowed.length());
        s[i] = allowed[index];
    }
    std::string final;
    for (int i = 0; i < s.length(); i++) {
        int found = allowed.find(s[i]);
        if (found == -1) {
            return "ERROR: Invalid input.";
        }
        found = mod(found - (i + 2 + key * (i + 1)), allowed.length());
        final += allowed[found];
    }
    return decrypt(final, XOR);
}

int main() {
    std::cout << "string: ";
    std::string string;
    std::getline(std::cin, string);
    std::cout << "givenkey: ";
    std::string givenkey;
    std::cin >> givenkey;
    std::cout << "encode: ";
    std::string encode;
    std::cin >> encode;
    std::cout << "xored: ";
    std::string xored;
    std::cin >> xored;
    std::cout << "onlyletters: ";
    std::string onlyletters;
    std::cin >> onlyletters;
    if (onlyletters == "onlyletters")
        allowed = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    keystr = givenkey;
    keystr = (keystr == "") ? "KEYGEN" : keystr;
    key = keygen(keystr);
    if (encode == "encode")
        std::cout << encodeThis(string, (xored == "xor")) << std::endl;
    else
        std::cout << decodeThis(string, (xored == "xor")) << std::endl;
    std::cin.get();
    return 0;
}