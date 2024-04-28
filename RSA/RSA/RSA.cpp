// RSA.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdlib.h>
#include <stdbool.h>
#include <numeric>
#include <algorithm>
#include <list>
#include <vector>
#include <random>
#include <chrono>
using namespace std;

struct keys {
    tuple <int, int> public_key;
    int private_key;
};

struct eea {
    int r;
    int s;
    int t;
};


// List of all two digits primes to use as example
list<int> primes{ 11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,
                  79,83,89,97 };

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
 * @brief Retrieves a prime number from a predefined list based on the provided index.
 *
 * @param index The index of the prime number to retrieve.
 * @return The prime number at the specified index.
 */
int get_random_prime(int index) {

    auto it = primes.begin();
    advance(it, index);

    return *it;
}

/**
 * @brief Checks if a number has a multiplicative inverse modulo another number.
 *
 * @param a The number.
 * @param modular The modulo value.
 * @return True if a has an inverse modulo modular, false otherwise.
 */
bool has_inverse(int a, int modular) {
    return std::gcd(a, modular) == 1;
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
    if (!has_inverse(a, modular)) {
        throw std::runtime_error("No inverse exists");
    }

    eea a_eea = compute_eea(modular, a);

    if (a_eea.r != 1) {
        throw std::runtime_error("No inverse exists");
    }

    return a_eea.t % modular;

}

/**
 * @brief Generates public and private keys for RSA encryption and decryption.
 *
 * @param p The first prime number.
 * @param q The second prime number.
 * @return A struct containing the generated public and private keys.
 */
keys generate_keys(int p, int q) {
    tuple <int, int> public_key;
    int private_key;

    // calculate necessary values to find public and private keys
    int n = p * q;
    int phi_n = (p - 1) * (q - 1);

    // populate public exponenet list
    vector<int> e;
    for (int i = 2; i < phi_n; i++) {
        e.push_back(i);
    }

    // copy e values into vector and shuffle it
    unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
    vector<int> randomize_e_list(e.begin(), e.end());
    std::shuffle(randomize_e_list.begin(), randomize_e_list.end(),
        std::default_random_engine(seed));


    int public_e = randomize_e_list.front();
    for (auto it = randomize_e_list.begin(); it != randomize_e_list.end(); ++it) {
        public_e = *it;
        if (gcd(public_e, phi_n) == 1) {
            break;
        }

    }

    public_key = make_tuple(n, public_e);
    private_key = get_inverse(public_e, phi_n);

    keys result = { public_key, private_key };

    return result;
}

/**
 * @brief Encrypts a plaintext using RSA encryption.
 *
 * @param n The modulus.
 * @param e The public exponent.
 * @param x The plaintext to encrypt.
 * @return The ciphertext.
 */
int rsa_encryption(int n, int e, int x) {

    long long result = 1;
    long long base = x;
    long long exponent = e;

    // Perform modular exponentiation
    while (exponent > 0) {
        if (exponent % 2 == 1) {
            result = (result * base) % n;
        }
        base = (base * base) % n;
        exponent /= 2;           
    }

    return static_cast<int>(result);
}

/**
 * @brief Decrypts a ciphertext using RSA decryption.
 *
 * @param d The private exponent.
 * @param n The modulus.
 * @param y The ciphertext to decrypt.
 * @return The decrypted plaintext.
 */
int rsa_decryption(int d, int n, int y) {


    long long result = 1; 
    long long base = y;  
    long long exponent = d;

    // Perform modular exponentiation
    while (exponent > 0) {
        if (exponent % 2 == 1) {
            result = (result * base) % n; // Update result modulo n at each step
        }
        base = (base * base) % n; // Square base and update modulo n
        exponent /= 2;            // Divide exponent by 2 for each iteration
    }

    return static_cast<int>(result);
}


int main()
{
    keys user_keys;

    int p = 3;
    int q = 11;
    int e = 3;
    int d = 7;
    int n = p * q;


    cout << "Enter your message to encrypt:\n";
    int plaintext;
    cin >> plaintext;

    int ciphertext;


    ciphertext = rsa_encryption(n, e, plaintext);

    cout << "Ciphertext: " << ciphertext << endl;

    plaintext = rsa_decryption(d, n, ciphertext);

    cout << "Plaintext: " << plaintext << endl;




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
