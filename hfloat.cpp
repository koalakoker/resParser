#include "hfloat.h"

hfloat::hfloat()
{
    mpf_init2(value,256);
}

hfloat::hfloat(QString str)
{
    mpf_init2(value,256);
    mpf_set_str(value, str.toLocal8Bit().data(), 10);
}

hfloat::~hfloat()
{
    //mpf_clear(value);
}

QString hfloat::toString(void)
{
    char out[200];
    gmp_sprintf(out,"%.32Fg",value);
    QString retVal = QString("%1").arg(QString(out));
    return retVal;
}

hfloat hfloat::operator+(const hfloat& a) const
{
    hfloat result;
    mpf_add(result.value, this->value, a.value);
    return result;
}

hfloat hfloat::operator-(const hfloat& a) const
{
    hfloat result;
    mpf_sub(result.value, this->value, a.value);
    return result;
}

hfloat hfloat::operator*(const hfloat& a) const
{
    hfloat result;
    mpf_mul(result.value, this->value, a.value);
    return result;
}

hfloat hfloat::operator/(const hfloat& a) const
{
    hfloat result;
    mpf_div(result.value, this->value, a.value);
    return result;
}

hfloat& hfloat::operator=(hfloat a)
{
    mpf_set(this->value,a.value);
    return *this;
}
