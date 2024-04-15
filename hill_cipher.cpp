#include <iostream>
#include <string>
#include <vector>
#include "C:\Program Files\MATLAB\R2024a\extern\include"

using namespace std;
using namespace matlab::engine;


extern void has_inverse(float key[][3]){
    std::unique_ptr<MATLABEngine> matlabPtr = startMATLAB();

    std::vector<float> keyVector;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            keyVector.push_back(key[i][j]);
        }
    }

    // Initialize 3x3 matlab data arra
    matlab::data::ArrayFactory factory;
    Array key_arr = factory.createArray<float>({3,3},
    keyVector.cbegin(), keyVector.cend());


}

void hill_cipher(string message, float key[][3]){
}


int main() {
    cout << "Enter your message: ";
    string message;
    cin >> message;

    const int message_l = message.length();

    vector<int> message_ascii(message_l);

    for (int i = 0; i < message_l; ++i) {
        message_ascii[i] = static_cast<int>(message[i]);
    }

    float key[3][3];

    cout <<"Enter 3x3 matrix for key (should have inverse):\n";
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++) {
            float value;
            cin >> value;
            key[i][j] = value;
        }
    }

    return 0;
}