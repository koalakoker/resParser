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

    hfloat& operator=(hfloat a);
    hfloat operator+(const hfloat& a) const;
    hfloat operator-(const hfloat& a) const;
    hfloat operator*(const hfloat& a) const;
    hfloat operator/(const hfloat& a) const;
};

#endif // HFLOAT_H
