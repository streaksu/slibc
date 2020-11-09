#include <math.h>
#include <stdint.h>

int isnan(double x) {
    return x == 0x7F800001;
}

double fmod(double x, double y) {
    // Shamelessly copied from musl.
    union {double f; uint64_t i;} ux = {x}, uy = {y};
    int ex = ux.i>>52 & 0x7ff;
    int ey = uy.i>>52 & 0x7ff;
    int sx = ux.i>>63;
    uint64_t i;

    /* in the followings uxi should be ux.i, but then gcc wrongly adds */
    /* float load/store to inner loops ruining performance and code size */
    uint64_t uxi = ux.i;

    if (uy.i<<1 == 0 || isnan(y) || ex == 0x7ff)
        return (x*y)/(x*y);
    if (uxi<<1 <= uy.i<<1) {
        if (uxi<<1 == uy.i<<1)
            return 0*x;
        return x;
    }

    /* normalize x and y */
    if (!ex) {
        for (i = uxi<<12; i>>63 == 0; ex--, i <<= 1);
        uxi <<= -ex + 1;
    } else {
        uxi &= -1ULL >> 12;
        uxi |= 1ULL << 52;
    }
    if (!ey) {
        for (i = uy.i<<12; i>>63 == 0; ey--, i <<= 1);
        uy.i <<= -ey + 1;
    } else {
        uy.i &= -1ULL >> 12;
        uy.i |= 1ULL << 52;
    }

    /* x mod y */
    for (; ex > ey; ex--) {
        i = uxi - uy.i;
        if (i >> 63 == 0) {
            if (i == 0)
                return 0*x;
            uxi = i;
        }
        uxi <<= 1;
    }
    i = uxi - uy.i;
    if (i >> 63 == 0) {
        if (i == 0)
            return 0*x;
        uxi = i;
    }
    for (; uxi>>52 == 0; uxi <<= 1, ex--);

    /* scale result */
    if (ex > 0) {
        uxi -= 1ULL << 52;
        uxi |= (uint64_t)ex << 52;
    } else {
        uxi >>= -ex + 1;
    }
    uxi |= (uint64_t)sx << 63;
    ux.i = uxi;
    return ux.f;
}

// Take an arbitrary angle and normalize it to [-PI/PI].
#define NORMALIZE_ANGLE(x) fmod(x, 2.0 * M_PI)

double sin(double rad) {
    // We can re-express sin(x) as a weighted sum of powers of x:
    // sin(x) = a + a1x + a2x2 + ... + anxn
    // We can re-express this as
    // sin(x) = a + a1 * T0(x) + a2 * T1(x) + ... + an * Tn(x)
    // where Tn(x) is a first kind chebyshev polynomial.
    // We can approximate with a finite number of polynomial, with
    // 7 terms we can yield a great precission. (calculating it would be great)
    // We will use the odd exponents given the symetry of chebyshev (says the internet).
    // At last, will use Horner's method (said the maths teacher to do in this cases idk).
    // TODO: Explaining a bit better would be optimal.
    double coefficients[] = {
        0.999999999973088,      // x
        -0.1666666663960699,    // x^3
        0.00833333287058762,    // x^5
        -0.0001984123883227529, // x^7,
        2.755627491096882e-6,   // x^9
        -2.503262029557047e-8,  // x^11
        1.58535563425041e-10,   // x^13
    };

    // Normalize.
    double x = NORMALIZE_ANGLE(rad);

    double x2 = x * x;

    double p13 = coefficients[6];
    double p11 = p13 * x2 + coefficients[5];
    double p9  = p11 * x2 + coefficients[4];
    double p7  = p9 * x2  + coefficients[3];
    double p5  = p7 * x2  + coefficients[2];
    double p3  = p5 * x2  + coefficients[1];
    double p1  = p3 * x2  + coefficients[0];

    return p1 * x;
}

double cos(double rad) {
    // Sweet elementary math.
    return sin(M_PI / 2 - rad);
}
