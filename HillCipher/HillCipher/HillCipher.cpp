#include <iostream>
#include <string>
#include <armadillo>
#include <cstring>
#include "hill_c.h"

using namespace std;
using namespace arma;

/**
 * Convert the key matrix into a vector for future use
 *
 * @param key the key matrix to convert
 * @return a vector version of the key matrix
*/
vector<float> key_to_vector(float key[][3]) {
    vector<float> keyVector;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            keyVector.push_back(key[i][j]);
        }
    }

    return keyVector;
}

/**
 * Using matlab, check if the key has an inverse
 *
 * @param key the vector version of the key matrix
 * @return True if the key has a matrix or False otherwise
*/
extern bool has_inverse(std::vector<int> key) {

    return true;
}


/**
 * Encrypt the message using the Hill Cipher
 *
 * @param message a vector of vectors representation of the message grouped
 * by three characters together
 * @param key the vector version of the key matrix
 *
 * @return the encrypted message
*/
void encrypt_hill_cipher(vector<vector<int>> message, vector<int> key) {
}

/**
 * Using the inverse key matrix decrypt the encrypted messgae back into its
 * original state
 *
 * @param encrypted a vector of vectors representation of the
 * encrypted message grouped by three characters together
 * @param inverse_key the inverse representation of the key matrix
 *
 * @return the decrypted messgae in vector form
*/
void decrypt_hill_cipher(vector<vector<int>> encrpted, vector<int> inverse_key) {
}








int main() {
    cout << "Enter your message: ";
    string message;
    getline(cin, message);
    // transform message into uppercase for simplicity
    transform(message.begin(), message.end(), message.begin(), toupper);

    int message_len = message.length();
    int rem = message_len % 3;
    // pad message with As if message not divisible by 3
    if (rem != 0) {
        int to_add = 3 - rem;
        for (int i = 0; i < to_add; i++) {
            message.push_back('A');
        }
    }

   
    mat key(3, 3);
    key.zeros();

    /**
    float key[3][3];
    vector<vector<float>> key_vector;
    char* key_arr = new char[9 + 1];
    bool inverse = false;
    do {
        string init_key = "";
        cout << "Enter 9 Letters to be the Key (no spaces): ";
        cin >> init_key;

        int key_len = init_key.length();
        if (key_len != 9) {
            cout << "Key does not have a length of 9!\n";
            continue;
        }
        // transform to uppercase for simplicity
        transform(init_key.begin(), init_key.end(), init_key.begin(), toupper);

        strcpy(key_arr, init_key.c_str());



    } while (!inverse);
      
    delete[] key_arr;
    */


    return 0;

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
