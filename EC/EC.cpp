// EC.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <stdbool.h>
#include <ctime>

using namespace std;

struct ec_curve {
    int a;
    int b;
    int p;
};

struct ec_point {
    int x;
    int y;
};

struct eea {
    int r;
    int s;
    int t;
};

struct public_key {
    ec_point Q;
};

struct private_key {
    int d;
};


struct keys {
    public_key pub_k;
    private_key pr_k;
};

struct encrypted {
    ec_point C1;
    ec_point C2;
};

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


int get_inverse(int a, int modular) {
    eea a_eea = compute_eea(modular, a);

    if (a_eea.r != 1) {
        throw std::runtime_error("No inverse exists");
    }

    return a_eea.t % modular;

}


/**
 * Performs point addition on two different points P and Q in an elliptic curve
 *
 * @param P: An elliptic curve point represented as (x, y) coordinates.
 * @param Q: Differnt elliptic curve point represented as (x, y) coordinates.
 * @param curve: Parameters of the elliptic curve
 * @return: The result of the point addition operation, which is a new elliptic curve point.
 *          If the operation results in a point at infinity, (-1, -1) is returned.
 */
ec_point point_addition(ec_point P, ec_point Q, ec_curve curve) {
    
    int x1 = P.x;
    int y1 = P.y;
    int x2 = Q.x;
    int y2 = Q.y;

    int den = x2 - x1;

    // checking if point at infinity is reached
    if (den == 0) {
        ec_point new_p;
        new_p.x = -1;
        new_p.y = -1;
        return new_p;
    }

    if (den < 0) {
        den += curve.p;
    }


    int inv_den = get_inverse(den, curve.p);

    int s = ((y2 - y1) * inv_den) % curve.p;

    ec_point new_p;

    int x3 = (static_cast<int>(pow(s, 2)) - x1 - x2) % curve.p;
    int y3 = (s * (x1 - x3) - y1) % curve.p;

    if (x3 < 0) {
        while (x3 < 0) {
            x3 += curve.p;
        }
    }

    if (y3 < 0) {
        while (y3 < 0) {
            y3 += curve.p;
        }
    }

    new_p.x = x3;
    new_p.y = y3;



    return new_p;
}

/**
 * Performs point doubling on a point P in an elliptic curve
 *
 * @param P: An elliptic curve point represented as (x, y) coordinates.
 * @param curve: Parameters of the elliptic curve
 * @return: The result of the point doubling operation, which is a new elliptic curve point.
 *          If the operation results in a point at infinity, (-1, -1) is returned.
 */
ec_point point_doubling(ec_point P, ec_curve curve) {
    int s;

    int den = 2 * P.y;

    if (den == 0) {
        ec_point new_p;
        new_p.x = -1;
        new_p.y = -1;
        return new_p;
    }

    int inv_den = get_inverse(den, curve.p);

    s = ((3 * pow(P.x, 2)) + curve.a) * inv_den;

    s %= curve.p;

    ec_point new_p;

    int x3 = (static_cast<int>(pow(s, 2)) - P.x - P.x) % curve.p;
    int y3 = (s * (P.x - x3) - P.y) % curve.p;

    if (x3 < 0) {
        while (x3 < 0) {
            x3 += curve.p;
        }
    }

    if (y3 < 0) {
        while (y3 < 0) {
            y3 += curve.p;
        }
    }

    new_p.x = x3;
    new_p.y = y3;



    return new_p;
}


/**
 * Performs scalar multiplication of a point P on an elliptic curve by an integer multiplier.
 *
 * @param P: An elliptic curve point represented as (x, y) coordinates.
 * @param mult: The integer multiplier for scalar multiplication.
 * @param curve: Parameters of the elliptic curve
 * @return: The result of scalar multiplication, which is a new elliptic curve point.
 *          If the operation results in a point at infinity, (-1, -1) is returned.
 */
ec_point int_mult_point(ec_point P, int mult, ec_curve curve) {
    ec_point R;


    R = point_doubling(P, curve);

    for (int i = 1; i < mult-1; i++) {
        if (P.x == R.x && P.y == R.y) {
            R = point_doubling(R, curve);
        }
        else if(R.x == -1 && R.y == -1) {
            R.x = P.x;
            R.y = P.y;
        }
        else {
            R = point_addition(R, P, curve);
        }
    }

    return R;

}

/**
 * Computes the additive inverse of an elliptic curve point P.
 *
 * @param P: An elliptic curve point represented as (x, y) coordinates.
 * @param curve: Parameters of the elliptic curve
 * @return: The additive inverse of the input point P, where the y-coordinate is negated.
 */
ec_point point_inverse(ec_point P, ec_curve curve) {
    P.y = -P.y;

    P.y %= curve.p;

    if (P.y < 0) {
        while (P.y < 0) {
            P.y += curve.p;
        }
    }

    return P;
}


/**
 * Generates public and private keys for elliptic curve cryptography.
 *
 * @param curve: Parameters of the elliptic curve
 * @param P: The base point on the elliptic curve.
 * @return: A structure containing the generated public and private keys.
 */
keys generate_keys(ec_curve curve, ec_point P) {
    srand(time(nullptr));

    ec_point Q;
    int d;
    do {
        d = (rand() % (curve.p - 1)) + 1;
        Q = int_mult_point(P, d, curve);
    } while (Q.x == -1 and Q.y == -1);

    public_key pub_k;
    pub_k.Q = Q;
    private_key pr_k;
    pr_k.d = d;

    keys keys;

    keys.pub_k = pub_k;
    keys.pr_k = pr_k;

    return keys;
}

/**
 * Performs encryption of a message using elliptic curve cryptography.
 *
 * @param curve: Parameters of the elliptic curve including the prime modulus 'p'.
 * @param P: The base point on the elliptic curve.
 * @param Q: The public key point.
 * @param M: The message point to be encrypted.
 * @return: A structure containing the encrypted message.
 */
encrypted encryption(ec_curve curve, ec_point P, ec_point Q, ec_point M) {
    ec_point C1;
    ec_point C2;

    int k;
    k = rand() % (curve.p - 1) + 2;

    // first point of encrypted message
    C1 = int_mult_point(P, k, curve);

    // second point of encrypted message
    C2 = int_mult_point(Q, k, curve);
    C2 = point_addition(C2, M, curve);

    encrypted points;
    points.C1 = C1;
    points.C2 = C2;

    return points;
}

/**
 * Performs decryption of an encrypted message using elliptic curve cryptography.
 *
 * @param curve: Parameters of the elliptic curve including the prime modulus 'p'.
 * @param C1: The first part of the encrypted message.
 * @param C2: The second part of the encrypted message.
 * @param d: The private key used for decryption.
 * @return: The decrypted message point.
 */
ec_point decryption(ec_curve curve, ec_point C1, ec_point C2, int d) {
    ec_point M;

    ec_point mult_c1 = int_mult_point(C1, d, curve);
    mult_c1 = point_inverse(mult_c1, curve);

    M = point_addition(C2, mult_c1, curve);
    return M;
}

bool euler_criterion(int a, int p) {
    int exp = (p - 1) / 2;

    int check = static_cast<int>(pow(a, exp)) % p;

    if (check == 1) {
        return true;
    }
    else {
        return false;
    }
}

int find_non_square(int n, int p) {
    int a;
    for (a = 2; a < p; a++) {
        int b = (static_cast<int>(pow(a, 2)) - n) % p;
        if (b < 0) {
            while (b < 0) {
                b += p;
            }
        }

        bool has_res = euler_criterion(b, p);
        if (!has_res) {
            return a;
        }
    }

    return -1;
}


ec_point rand_gen_point(ec_curve curve) {

    ec_point point;

    point.x = -1;
    point.y = -1;

    int x;
    int right;
    int a;

    while (true) {
        x = rand() % curve.p;
        right = (static_cast<int>(pow(x, 3)) + (curve.a * x) + curve.b) % curve.p;
        bool has_res = euler_criterion(right, curve.p);

        if (!has_res) {
            continue;
        }

        a = find_non_square(right, curve.p);
        if (a != -1) {
            break;
        }
    }

    return point;

}



int main()
{
    ec_curve curve;
    curve.a = 0;
    curve.b = 7;
    curve.p = 17;

    cout << "Elliptic Curve: " << endl;
    cout << "y^2 = x^3 + " << curve.a << "x + " << curve.b << " (mod " << curve.p << ")" << endl;

    ec_point point;
    point.x = 8;
    point.y = 3;

    cout << "Point of Reference: " << endl;
    cout << "(" << point.x << ", " << point.y << ")" << endl;


    keys k = generate_keys(curve, point);
    cout << "Public Key: " << endl;
    cout << "Q = (" << k.pub_k.Q.x << ", " << k.pub_k.Q.y << ")" << endl;

    cout << "Private Key:" << endl;
    cout << "d = " << k.pr_k.d << endl;



    ec_point message;
    message.x = 1;
    message.y = 12;

    cout << "Message Point:" << endl;
    cout << "(" << message.x << ", " << message.y << ")" << endl;

    encrypted e_mes = encryption(curve, point, k.pub_k.Q, message);

    cout << "Encrypted Points:" << endl;
    cout << "C1: (" << e_mes.C1.x << ", " << e_mes.C1.y << ")" << endl;
    cout << "C2: (" << e_mes.C2.x << ", " << e_mes.C2.y << ")" << endl;

    ec_point decrypted = decryption(curve, e_mes.C1, e_mes.C2, k.pr_k.d);
    cout << "Decrypted Point:" << endl;
    cout << "(" << decrypted.x << ", " << decrypted.y << ")" << endl;

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
