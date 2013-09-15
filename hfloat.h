#ifndef HFLOAT_H
#define HFLOAT_H

#include "gmp.h"
#include "mpfr.h"
#include "QString"

class hfloat
{
private:
    mpfr_t value;
public:
    ~hfloat();
    hfloat();
    hfloat(QString str);
    hfloat(const hfloat& val);

    QString toString(void);
    bool isNan(void);
    void setNan(void);

    hfloat& operator=(hfloat a);
    hfloat operator+(const hfloat& a) const;
    hfloat operator-(const hfloat& a) const;
    hfloat operator*(const hfloat& a) const;
    hfloat operator/(const hfloat& a) const;

    static hfloat sqrt(const hfloat a);
    static hfloat sqr(const hfloat a);
    static hfloat cbrt(const hfloat a);
    static hfloat pow(const hfloat a, const hfloat b);
    static hfloat abs(const hfloat a);
    static hfloat log(const hfloat a);
    static hfloat log2(const hfloat a);
    static hfloat log10(const hfloat a);
    static hfloat exp(const hfloat a);
    static hfloat exp2(const hfloat a);
    static hfloat exp10(const hfloat a);
    static hfloat sin(const hfloat a);
    static hfloat cos(const hfloat a);
    static hfloat tan(const hfloat a);
    static hfloat sec(const hfloat a);
    static hfloat csc(const hfloat a);
    static hfloat cot(const hfloat a);
    static hfloat acos(const hfloat a);
    static hfloat asin(const hfloat a);
    static hfloat atan(const hfloat a);
    static hfloat atan2(const hfloat a, const hfloat b);
    static hfloat cosh(const hfloat a);
    static hfloat sinh(const hfloat a);
    static hfloat tanh(const hfloat a);
    static hfloat sech(const hfloat a);
    static hfloat csch(const hfloat a);
    static hfloat coth(const hfloat a);
    static hfloat acosh(const hfloat a);
    static hfloat asinh(const hfloat a);
    static hfloat atanh(const hfloat a);
};

#endif // HFLOAT_H
