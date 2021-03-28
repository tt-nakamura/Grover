#include "QBits.h"

static double PI(atan(1)*4);

void mirror(const QBits& q, long l)
// reflection about the mean
// l = flags of control qubits
// reference: Nielsen and Chuang, ex6.2, 6.6
{
    Hadamar(q,l);
    XNot(q,l);
    ZFlip(q,l);
    XNot(q,l);
    Hadamar(q,l);
    ZFlip(l);// flip overall sign
}

void (*oracle)(const QBits&, long);

void GroverIteration(const QBits& q, long l=0)
{
    oracle(q,l);
    mirror(q,l);
}

long QCount(void o(const QBits&, long), long n, long t)
// Quantum Counting
// input:
//   o = oracle (Nielsen and Chuang, \S6.1.1)
//   n = number of search space qubits
//   t = number of qubits for phase estimation
// return:
//   number of solutions marked by the oracle
// reference: Nielsen and Chuang, section 6.3
{
    long m,T,N(1<<n);
    if(t<=0) t=n+1;
    T = 1L<<t;
    QBits q(n);
    clear(q);//???
    Hadamar(q);
    oracle = o;
    m = PhaseEst(GroverIteration,q,t);
    clear(q);// clear q before destruct
    if(m > T/2) m=T-m;
    m = (long)round(N*pow(sin(PI*m/T),2));
    return m;
}

long Grover(void o(const QBits&, long), long n, long r)
// Quantum search algorithm
// input:
//   o = oracle (Nielsen and Chuang, \S6.1.1)
//   n = number of search space qubits
//   r = number of solutions to search for
// return:
//   one of solutions marked by the oracle
// reference: Nielsen and Chuang, section 6.1
{
    long i,N(1<<n),R(floor(PI/2/acos(1-r*2./N)));
    QBits q(n);
    Hadamar(q);
    oracle = o;
    for(i=0; i<R; i++) GroverIteration(q);
    i = measure(q);
    XNot(q(i));// clear q before destruct
    return i;
}