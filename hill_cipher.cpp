#include <iostream>
#include <string>
#include "hill_c.h"
#include "C:\Program Files\MATLAB\R2024a\extern\include\matrix.h"
#include "C:\Program Files\MATLAB\R2024a\extern\include\MatlabEngine.hpp"

using namespace std;
using namespace matlab::engine;

/**
 * Convert the key matrix into a vector for future use
 * 
 * @param key the key matrix to convert
 * @return a vector version of the key matrix
*/
vector<int> key_to_vector(float key[][3]) {
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
extern bool has_inverse(std::vector<int> key){
    std::unique_ptr<MATLABEngine> matlabPtr = startMATLAB();

    // Initialize 3x3 matlab data arra
    matlab::data::ArrayFactory factory;
    Array key_arr = factory.createArray<float>({3,3},
    key.cbegin(), key.cend());

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
vector<vector<int>> encrypt_hill_cipher(vector<vector<int>> message, vector<int> key){

    return NULL;
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
vector<vector<int>> decrypt_hill_cipher(vector<vector<int>> encrpted, vector<int> inverse_key) {
    return NULL;
}





vector<vector<int>> create_message_vector(const string& message){
    vector<vector<int>> message_vectors;
    const int message_l = message.length();
    const int cols_needed = ceil(static_cast<double>(message_l) / 3);

    for (int i = 0; i < cols_needed; i++){
        vector<int> curr_group(3,0);
        for (int j = 0; j < 3; ++j) {
            int index = i * 3 + j;
            if (index < message_l) {
                curr_group[j] = static_cast<int>(message[index] - 'A') % 26;
            }
            else {
                break;
            }
        }
        messageGroups.push_back(group);
    }

    return message_vectors;
}


int main() {
    cout << "Enter your message: ";
    string message;
    cin >> message;

    vector<vector<int>> message_matrix = create_message_vector(&message);

    float key[3][3];
    vector<vector<int>> key_vector;
    bool inverse = false;
    do{
        cout <<"Enter 3x3 matrix for key (should have inverse):\n";
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++) {
                float value;
                cin >> value;
                key[i][j] = static_cast<int>(std::fabs(value)) % 26;
            }
        }
        key_vector = key_to_vector(key);

        inverse = has_inverse(key_vector);
        if (!inverse) {
            cout << "Key does not have an inverse. 
            Please enter a different key.\n";
        }
    } while(!inverse);


    vector<vector<int>> encrypted = encrypt_hill_cipher(message_matrix, 
                                                        key_vector);

    vector<vector<int>> decrypted = decrypt_hill_cipher(encrypted, inverse);

    return 0;
}