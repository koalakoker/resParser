#include "QString"

typedef long int Int64;

class Resistor
{
private:
	 Int64 resValue;
public:
	Resistor(void);
    Int64 Value(void);
    void setValue(Int64 value);
    QString ValueStr(void);
    void setValueStr(QString value);
    QString ToString();
    Int64 FromStringToValue(QString str);
    static int* GetE12Values(void);
    static QString E12ValuesToString(void);
    static int* GetE24Values(void);
    static QString E24ValuesToString(void);
    static bool IsE12(Int64 val);
    static bool IsE24(Int64 val);
    static bool IsE(Int64 val,int* E_Values,int nValues);
    static int GetTwoDigit(Int64 val);
    static int GetOrder(Int64 val);
    static Int64 ToNearE12(Int64 val);
};
