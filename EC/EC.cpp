// EC.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <stdlib.h>
#include <cmath>

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



ec_point point_addition(ec_point P, ec_point Q, ec_curve curve) {
    ec_point R;

    int x1 = P.x;
    int y1 = P.y;
    int x2 = Q.x;
    int y2 = Q.y;

    int den = x2 - x1;

    if (den == 0) {
        ec_point new_p;
        new_p.x = -1;
        new_p.y = -1;
        return new_p;
    }


    int inv_den = get_inverse(den, curve.p);

    int s = ((y2 - y1) * inv_den) % curve.p;

    ec_point new_p;

    int x3 = (static_cast<int>(pow(s, 2)) - x1 - x2) % curve.p;
    int y3 = (s * (x1 - x3) - y1) % curve.p;

    new_p.x = x3;
    new_p.y = y3;



    return new_p;
}

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

    new_p.x = x3;
    new_p.y = y3;



    return new_p;
}


ec_point int_mult_point(ec_point P, int mult, ec_curve curve) {
    ec_point R;


    R = point_doubling(P, curve);

    for (int i = 1; i < mult; i++) {
        if (P.x == R.x && P.y == R.y) {
            R = point_doubling(R, curve);
        }
        else {
            R = point_addition(R, P, curve);
        }
    }

    return R;

}

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

keys generate_keys(ec_curve curve, ec_point P) {
    srand(time(NULL));

    ec_point Q;

    int d;
    d = rand() % (curve.p - 1) + 2;

    Q = int_mult_point(P, d, curve);

    public_key pub_k;
    pub_k.Q = Q;
    private_key pr_k;
    pr_k.d = d;

    keys keys;

    keys.pub_k = pub_k;
    keys.pr_k = pr_k;

    return keys;
}



int main()
{
    ec_curve curve;
    curve.a = 0;
    curve.b = 7;
    curve.p = 17;

    std::cout << "Hello World!\n";
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
