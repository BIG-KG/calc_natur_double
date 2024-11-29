#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double  getG();
double  getE();
double  getT();
double  getP();
double  getN();
double  getS();

int main()
{
    printf("ans = %lf\n", getG());
}

int p = 0;
char s[100] = "((25*10*(3*(25-10*2)+1))-3990)^3$";

double  getP()
{
    if(s[p] == '(')
    {
        p++;
        double  val = getE();
        if(s[p] != ')') assert(0);
        p++;
        return val;
    }

    else
    {
        return getN();
    }
 
}

double  getE()
{
    //printf("start E\n");
    double  val = getT();
    double  val1 = 0;

    while (s[p] == '+' || s[p] == '-')
    {
        int op = s[p];
        p++;
        double val1 = getT();

        if (op == '+') val += val1;
        else           val -= val1;
    }

    return val;    
}


double  getG()
{
    //printf("start G");
    double  val = getE();
    if(s[p] != '$')  assert(0);
    return val;
}

double  getN()
{
    //printf("start N\n");
    double  val = 0;
    int startPos = p;
    while ('0' <= s[p] && s[p] <= '9')
    {
        val = val * 10 + (double)(s[p] - '0');
        p++;
    }
    if (startPos == p) assert(0);

    return val;
}

double  getT()
{
    //printf("start T\n");
    double  val = getS();
    double  val1 = 0;

    while (s[p] == '*' || s[p] == '/')
    {
        int op = s[p];
        p++;
        double  val1 = getS();

        if (op == '*') val *= val1;
        else           val /= val1;
    }

    return val; 
}

double  getS()
{
    double  val  = getP();
    double  val1 = 0;
    if(s[p] == '^')
    {
        p++;
        val1 = getP();

        return pow(val, val1);
    }

    return val;
}


