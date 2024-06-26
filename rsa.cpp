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


int get_random_prime(int index) {

    auto it = primes.begin();
    advance(it, index);

    return *it;
}

bool has_inverse(int a, int modular) {
    return std::gcd(a, modular) == 1;
}


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

int rsa_encryption(int n, int e, int x) {

    int encrypt = (int) pow(x, e) % n;
    if (encrypt < 0) {
        while(encrypt < 0) {
             encrypt += n;
        }
    }

    return encrypt;
}

int rsa_decryption(int d, int n, int y) {

    int decrypt = (int) pow(y, d) % n;
     if (decrypt < 0) {
        while(decrypt < 0) {
             decrypt += n;
        }
    }

    return decrypt;
}


int main()
{
    keys user_keys;
    /**
    cout << "Enter the following:\n"
        "0: Use two random two digit primes\n"
        "1: Use your own two prime numbers\n";
    int user_choice;
    cin >> user_choice;

    keys user_keys;

    if (user_choice == 0) {
        srand(time(0));

        int prime_count = primes.size();
        int rand_index1 = rand() % prime_count;
        int rand1 = get_random_prime(rand_index1);

        int rand_index2;

        do {
           rand_index2 = rand() % prime_count;
        } while (rand_index2 == rand_index1);

        int rand2 = get_random_prime(rand_index2);

        cout << "Random prime 1: " << rand1 << endl;
        cout << "Random prime 2: " << rand2 << endl;

        user_keys = generate_keys(rand1, rand2);

    }
    */

    int p = 3;
    int q = 11;
    int e = 3;
    int d = 7;
    int n = p * q;
    /*
    user_keys = generate_keys(p, q);



    // TODO get user inputted values and check if usable

    cout << "Public Key: (" << get<0>(user_keys.public_key) 
        << ", " << get<1>(user_keys.public_key) << ")" << endl;
    cout << "Private Key: " << user_keys.private_key << endl;
    */


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