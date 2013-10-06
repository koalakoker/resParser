#include "hfloat.h"

#define ROUND MPFR_RNDN
#define PRECISION 256

hfloat::hfloat()
{
    mpfr_init2(value,PRECISION);
    setNan();
}

hfloat::hfloat(QString str)
{
    mpfr_init2(value,PRECISION);
    mpfr_set_str(value, str.toLocal8Bit().data(), 10, ROUND);
}

hfloat::hfloat(const hfloat &val)
{
    mpfr_init2(value,PRECISION);
    mpfr_set(this->value,val.value,ROUND);
}

hfloat::~hfloat()
{
    mpfr_clear(value);
}

QString hfloat::toString(QString format)
{
    QString retVal;
    char out[200];
    if (format.toLower().contains("x"))
    {
        sprintf(out,format.toLocal8Bit().data(),mpfr_get_si(value,ROUND));
        retVal = QString("0x%1").arg(QString(out));
    }
    else
    {
        mpfr_sprintf(out,format.toLocal8Bit().data(),value);
        retVal = QString(out);
    }
    return retVal;
}

float hfloat::toFloat()
{
    return mpfr_get_flt(this->value,ROUND);
}

bool hfloat::isNan(void)
{
    return (mpfr_nan_p(this->value) != 0);
}

bool hfloat::isNumber(void)
{
    return (mpfr_number_p(this->value) != 0);
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

hfloat hfloat::cbrt(const hfloat a)
{
    hfloat retVal;
    mpfr_cbrt(retVal.value,a.value,ROUND);
    return retVal;
}

hfloat hfloat::pow(const hfloat a,const hfloat b)
{
    hfloat retVal;
    mpfr_pow(retVal.value,a.value,b.value,ROUND);
    return retVal;
}

hfloat hfloat::abs(const hfloat a)
{
    hfloat retVal;
    mpfr_abs(retVal.value,a.value,ROUND);
    return retVal;
}

hfloat hfloat::log(const hfloat a)
{
    hfloat retVal;
    mpfr_log(retVal.value,a.value,ROUND);
    return retVal;
}

hfloat hfloat::log2(const hfloat a)
{
    hfloat retVal;
    mpfr_log2(retVal.value,a.value,ROUND);
    return retVal;
}

hfloat hfloat::log10(const hfloat a)
{
    hfloat retVal;
    mpfr_log10(retVal.value,a.value,ROUND);
    return retVal;
}

hfloat hfloat::exp(const hfloat a)
{
    hfloat retVal;
    mpfr_exp(retVal.value,a.value,ROUND);
    return retVal;
}

hfloat hfloat::exp2(const hfloat a)
{
    hfloat retVal;
    mpfr_exp2(retVal.value,a.value,ROUND);
    return retVal;
}

hfloat hfloat::exp10(const hfloat a)
{
    hfloat retVal;
    mpfr_exp10(retVal.value,a.value,ROUND);
    return retVal;
}

hfloat hfloat::sin(const hfloat a)
{
    hfloat retVal;
    mpfr_sin(retVal.value,a.value,ROUND);
    return retVal;
}

hfloat hfloat::cos(const hfloat a)
{
    hfloat retVal;
    mpfr_cos(retVal.value,a.value,ROUND);
    return retVal;
}

hfloat hfloat::tan(const hfloat a)
{
    hfloat retVal;
    mpfr_tan(retVal.value,a.value,ROUND);
    return retVal;
}

hfloat hfloat::sec(const hfloat a)
{
    hfloat retVal;
    mpfr_sec(retVal.value,a.value,ROUND);
    return retVal;
}

hfloat hfloat::csc(const hfloat a)
{
    hfloat retVal;
    mpfr_csc(retVal.value,a.value,ROUND);
    return retVal;
}

hfloat hfloat::cot(const hfloat a)
{
    hfloat retVal;
    mpfr_cot(retVal.value,a.value,ROUND);
    return retVal;
}

hfloat hfloat::acos(const hfloat a)
{
    hfloat retVal;
    mpfr_acos(retVal.value,a.value,ROUND);
    return retVal;
}

hfloat hfloat::asin(const hfloat a)
{
    hfloat retVal;
    mpfr_asin(retVal.value,a.value,ROUND);
    return retVal;
}

hfloat hfloat::atan(const hfloat a)
{
    hfloat retVal;
    mpfr_atan(retVal.value,a.value,ROUND);
    return retVal;
}

hfloat hfloat::atan2(const hfloat a,const hfloat b)
{
    hfloat retVal;
    mpfr_atan2(retVal.value,a.value,b.value,ROUND);
    return retVal;
}

hfloat hfloat::cosh(const hfloat a)
{
    hfloat retVal;
    mpfr_cosh(retVal.value,a.value,ROUND);
    return retVal;
}

hfloat hfloat::sinh(const hfloat a)
{
    hfloat retVal;
    mpfr_sinh(retVal.value,a.value,ROUND);
    return retVal;
}

hfloat hfloat::tanh(const hfloat a)
{
    hfloat retVal;
    mpfr_tanh(retVal.value,a.value,ROUND);
    return retVal;
}

hfloat hfloat::sech(const hfloat a)
{
    hfloat retVal;
    mpfr_sech(retVal.value,a.value,ROUND);
    return retVal;
}

hfloat hfloat::csch(const hfloat a)
{
    hfloat retVal;
    mpfr_csch(retVal.value,a.value,ROUND);
    return retVal;
}

hfloat hfloat::coth(const hfloat a)
{
    hfloat retVal;
    mpfr_coth(retVal.value,a.value,ROUND);
    return retVal;
}

hfloat hfloat::acosh(const hfloat a)
{
    hfloat retVal;
    mpfr_acosh(retVal.value,a.value,ROUND);
    return retVal;
}

hfloat hfloat::asinh(const hfloat a)
{
    hfloat retVal;
    mpfr_asinh(retVal.value,a.value,ROUND);
    return retVal;
}

hfloat hfloat::atanh(const hfloat a)
{
    hfloat retVal;
    mpfr_atanh(retVal.value,a.value,ROUND);
    return retVal;
}
