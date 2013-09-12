#ifndef HFLOAT_H
#define HFLOAT_H

#include "gmp.h"
#include "QString"

class hfloat
{
private:
    mpf_t value;
public:
    ~hfloat();
    hfloat();
    hfloat(QString str);

    QString toString(void);

    hfloat& operator=(hfloat a);
    hfloat operator+(const hfloat& a) const;
    hfloat operator-(const hfloat& a) const;
    hfloat operator*(const hfloat& a) const;
    hfloat operator/(const hfloat& a) const;
};

#endif // HFLOAT_H
