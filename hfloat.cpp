#include "hfloat.h"

#define ROUND MPFR_RNDN

hfloat::hfloat()
{
    mpfr_init2(value,128);
    setNan();
}

hfloat::hfloat(QString str)
{
    mpfr_init2(value,128);
    mpfr_set_str(value, str.toLocal8Bit().data(), 10, ROUND);
}

hfloat::hfloat(const hfloat &val)
{
    mpfr_init2(value,128);
    mpfr_set(this->value,val.value,ROUND);
}

hfloat::~hfloat()
{
    mpfr_clear(value);
}

QString hfloat::toString(void)
{
    char out[200];
    mpfr_sprintf(out,"%.32Rg",value);
    QString retVal = QString("%1").arg(QString(out));
    return retVal;
}

bool hfloat::isNan(void)
{
    return (mpfr_nan_p(this->value) != 0);
}

void hfloat::setNan(void)
{
    mpfr_set_nan(this->value);
}

hfloat hfloat::operator+(const hfloat& a) const
{
    hfloat result;
    mpfr_add(result.value, this->value, a.value,ROUND);
    return result;
}

hfloat hfloat::operator-(const hfloat& a) const
{
    hfloat result;
    mpfr_sub(result.value, this->value, a.value,ROUND);
    return result;
}

hfloat hfloat::operator*(const hfloat& a) const
{
    hfloat result;
    mpfr_mul(result.value, this->value, a.value,ROUND);
    return result;
}

hfloat hfloat::operator/(const hfloat& a) const
{
    hfloat result;
    mpfr_div(result.value, this->value, a.value,ROUND);
    return result;
}

hfloat& hfloat::operator=(hfloat a)
{
    mpfr_set(this->value,a.value,ROUND);
    return *this;
}

hfloat hfloat::sqrt(const hfloat a)
{
    hfloat retVal;
    mpfr_sqrt(retVal.value,a.value,ROUND);
    return retVal;
}

hfloat hfloat::sqr(const hfloat a)
{
    hfloat retVal;
    mpfr_sqr(retVal.value,a.value,ROUND);
    return retVal;
}
