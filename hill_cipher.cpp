#include <iostream>
#include <string>
#include <vector>

using namespace std;


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