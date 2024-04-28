#include <iostream>
#include <string>
#include <armadillo>
#include <cstring>

using namespace std;
using namespace arma;

struct eea {
    int r;
    int s;
    int t;
};

/**
 * @brief Computes the Extended Euclidean Algorithm (EEA) to find the greatest common divisor
 *        and Bézout coefficients of two integers.
 *
 * @param r0 The first integer.
 * @param r1 The second integer.
 * @return A struct containing the results of the EEA: r (gcd), s, and t (Bézout coefficients).
 */
eea compute_eea(int r0, int r1) {
    int s0 = 1;
    int t0 = 0;
    int s1 = 0;
    int t1 = 1;

    while (r1 > 0) {
        int q = r0 / r1;
        int r2 = r0 - q * r1;
        int s2 = s0 - q * s1;
        int t2 = t0 - q * t1;

        r0 = r1;
        r1 = r2;
        s0 = s1;
        s1 = s2;
        t0 = t1;
        t1 = t2;
    }

    eea result;
    result.r = r0;
    result.s = s0;
    result.t = t0;

    return result;

}

/**
 * @brief Calculates the multiplicative inverse of a modulo another number using the
 * Extended Euclidean Algorithm.
 *
 * @param a The number.
 * @param modular The modulo value.
 * @return The multiplicative inverse of a modulo modular.
 * @throws std::runtime_error if no inverse exists.
 */
int get_inverse(int a, int modular) {
    eea a_eea = compute_eea(modular, a);

    if (a_eea.r != 1) {
        throw std::runtime_error("No inverse exists");
    }

    return a_eea.t % modular;

}

/**
 * @brief Inverts the given key matrix for the Hill Cipher encryption.
 *
 * @param key_det The determinant of the key matrix.
 * @param key The key matrix.
 * @return The inverted key matrix.
 */
Mat<int> invert_ley_matrix(double key_det, mat key) {
    // first need to invert determinant of key matrix
    int int_det = int(key_det);
    int_det %= 26;
    int inverse_det = get_inverse(int_det, 26);

    mat trans_key = trans(key);

    // finding "core" values of matrix by finding 2x2 det
    // of all cells in the matrix
    Mat<int> inverse_key(3, 3);

    for (uword i = 0; i < 3; ++i) {
        for (uword j = 0; j < 3; ++j) {
            // Extract the corresponding 2x2 submatrix
            mat submatrix = trans_key.submat(0, 0, 2, 2);

            // Remove the i-th row
            submatrix.shed_row(i);

            // Remove the j-th column
            submatrix.shed_col(j);

            // Calculate the determinant of the submatrix
            double determinant = det(submatrix);

            if ((i + j) % 2 == 1) {
                determinant = -determinant;
            }


            int int_d = int(determinant);

            int_d %= 26;

            if (int_d < 0) {
                int_d += 26;
            }


            inverse_key.at(i, j) = int_d;
        }
    }

    for (uword i = 0; i < 3; ++i) {
        for (uword j = 0; j < 3; ++j) {
            int val = inverse_key(i, j);
            val = (val * inverse_det) % 26;

            if (val < 0) {
                val += 26;
            }

            inverse_key(i, j) = val;
        }
    }

    return inverse_key;


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
vector<Mat<int>> encrypt_hill_cipher(vector<Mat<int>> message, Mat<int> key) {
    vector<Mat<int>> encrypted;
    

    // Iterate through each matrix in the message vector
    for (const auto& matrix : message) {
        Mat<int> encrypted_matrix = key * matrix;

        for (uword i = 0; i < encrypted_matrix.n_rows; ++i) {
            for (uword j = 0; j < encrypted_matrix.n_cols; ++j) {
                encrypted_matrix(i, j) %= 26;
            }
        }


        // Add the encrypted matrix to the encrypted vector
        encrypted.push_back(encrypted_matrix);
    }

    return encrypted;
}

/**
 * Print out encrypted message
 *
 * @param encrypted a vector of vectors representation of the
 * encrypted messgae in groups of 3x1 matrices
*/
void print_encrypted(vector<Mat<int>> encrypted) {

    cout << "Encrypted Message: " << endl;

    for (const auto& matrix : encrypted) {

        for (uword i = 0; i < matrix.n_rows; ++i) {
            for (uword j = 0; j < matrix.n_cols; ++j) {
                int ascii = matrix(i, j);
                char character = static_cast<char>(ascii + 'A');
                cout << character;
            }
        }

    }

    cout << endl;
}

/**
 * Print out decrypted message
 *
 * @param encrypted a vector of vectors representation of the
 * decrypted messgae in groups of 3x1 matrices
*/
void print_decrypted(vector<Mat<int>> decrypted) {

    cout << "Decrypted Message: " << endl;

    for (const auto& matrix : decrypted) {

        for (uword i = 0; i < matrix.n_rows; ++i) {
            for (uword j = 0; j < matrix.n_cols; ++j) {
                int ascii = matrix(i, j);
                char character = static_cast<char>(ascii + 'A');
                cout << character;
            }
        }

    }

    cout << endl;
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
vector<Mat<int>> decrypt_hill_cipher(vector<Mat<int>> encrypted, Mat<int> inverse_key) {

    vector<Mat<int>> decrypted;


    // Iterate through each matrix in the message vector
    for (const auto& matrix : encrypted) {
        Mat<int> decrypted_matrix = inverse_key * matrix;

        for (uword i = 0; i < decrypted_matrix.n_rows; ++i) {
            for (uword j = 0; j < decrypted_matrix.n_cols; ++j) {
                decrypted_matrix(i, j) %= 26;
            }
        }

        // Add the encrypted matrix to the encrypted vector
        decrypted.push_back(decrypted_matrix);
    }

    return decrypted;
}


int main() {
    cout << "Enter your message: ";
    string message;
    getline(cin, message);
    // transform message into uppercase for simplicity
    transform(message.begin(), message.end(), message.begin(), toupper);

    message.erase(remove_if(message.begin(), message.end(), ::isspace), message.end());


    int message_len = message.length();
    int rem = message_len % 3;
    // pad message with As if message not divisible by 3
    if (rem != 0) {
        int to_add = 3 - rem;
        for (int i = 0; i < to_add; i++) {
            message.push_back('A');
        }
    }
    message_len = message.length();

    char* message_ints = new char[message_len];
    for (int i = 0; i < message_len; ++i) {
        message_ints[i] = (message[i] - 'A') % 26;
    }

    cout << "ASCII values of characters in the message:" << endl;
    for (int i = 0; i < message_len; ++i) {
        cout << static_cast<int>(message_ints[i]) << " ";
    }
    cout << endl;

    vector<Mat<int>> message_matrices;
    for (int i = 0; i < message_len; i += 3) {
        Mat<int> next(3, 1);
        for (int j = 0; j < 3; ++j) {
            next(j) = message_ints[i + j];
        }
        message_matrices.push_back(next);
    }

    for (size_t i = 0; i < message_matrices.size(); ++i) {
        cout << "Matrix of Message Portion " << i + 1 << ":" << endl;
        cout << message_matrices[i] << endl;
    }

    

    mat key(3, 3);
    Mat<int> key_int(3, 3);
    Mat<int> key_inv_int(3, 3);
    key.zeros();
    key_int.zeros();
    char* key_arr = new char[9 + 1];
    int key_ints[9 + 1];
    bool inverse = false;
    do {
        string init_key = "";
        cout << "Enter 9 Letters to be the Key (no spaces): ";
        cin >> init_key;

        int key_len = init_key.length();
        if (key_len != 9) {
            cout << "Key does not have a length of 9!" << endl;
            continue;
        }
        // transform to uppercase for simplicity
        transform(init_key.begin(), init_key.end(), init_key.begin(), toupper);

        for (int i = 0; i < 9; ++i) {
            key_arr[i] = init_key[i];
        }
        key_arr[9] = '\0';



        // Convert each character to its ASCII value modulo 26
        for (int i = 0; i < 9; ++i) {
            // Convert character to ASCII value
            int ascii_value = key_arr[i];
            // Apply modulo 26
            int mod_value = (ascii_value - 'A') % 26;
            key_ints[i] = mod_value;
        }

        // Convert key_ints to the key matrix
        int index = 0;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                key.at(i, j) = key_ints[index++];
            }
        }


        // check for inverse
        double detA = det(key);
        if (detA == 0) {
            cout << "This key does not have an inverse!" << endl;
            continue;
        }

        key_int = conv_to<Mat<int>>::from(key);
        key_inv_int = invert_ley_matrix(detA, key);
        inverse = true;

    } while (!inverse);

    cout << "Key Matrix " << endl;
    cout << key_int << endl;
    cout << "Key Inverse Matrix " << endl;
    cout << key_inv_int << endl;


    vector<Mat<int>> encrypted = encrypt_hill_cipher(message_matrices, key_int);

    print_encrypted(encrypted);

    vector<Mat<int>> decrypted = decrypt_hill_cipher(encrypted, key_inv_int);


    print_decrypted(decrypted);

    delete[] key_arr;

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