#include <inttypes.h>

imaxdiv_t imaxdiv(intmax_t numerator, intmax_t denominator) {
    imaxdiv_t ret;

    ret.quot = numerator / denominator;
    ret.rem  = numerator % denominator;
    if (numerator >= 0 && ret.rem < 0) {
        ret.quot++;
        ret.rem -= denominator;
    }

    return ret;
}

intmax_t imaxabs(intmax_t number) {
	return number > 0 ? number : -number;
}
