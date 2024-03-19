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

// List of all two digits primes to use as example
list<int> primes{ 11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,
                  79,83,89,97 };

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



}




keys generate_keys(int p, int q) {
    tuple <int, int> public_key;
    int private_key;

    // calculate necessary values to find public and private keys
    int n = p * q;
    int phi_n = (p - 1) * (q - 1);

    // populate public exponenet list
    list<int> e{};
    for (int i = 1; i < phi_n; i++) {
        e.push_back(i);
    }

    // copy e values into vector and shuffle it
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    vector<int> randomize_e_list(e.begin(), e.end());
    std::shuffle(randomize_e_list.begin(), randomize_e_list.end(),
        std::default_random_engine(seed));


    int public_e = e.front();
    for (auto it = e.begin(); it != e.end(); ++it) {
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


int main()
{
    cout << "Enter the following:\n"
        "0: Use two random two digit primes\n"
        "1: Use your own two prime numbers\n";
    int user_choice;
    cin >> user_choice;

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

    }

    return 0;
}