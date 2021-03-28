#include "QBits.h"

static long N(3);// number of propositions
static long M(4);// number of clauses in conjunctive normal form
static long prop[] = {0,1,6,0};// bit flags to negate or not
static long mask[] = {3,5,6,5};// bit flags to include in clauses or not

void SAT(const QBits&q, long l=0)
// oracle for Grover interation in SAT problem
// l = flags of control qubits
// reference:
//   E.R.Johnston et al, "Programming Quantum Computer" fig10.9
{
    long i,j,t;
    QBits q1(M);// work space qubits
    // assume q1 has been set to zero
    for(i=0; i<M; i++) {
        t = q(mask[i]^prop[i]);
        XNot(t,l);
        XNot(q1[i], l|q(mask[i]));
        XNot(q1[i]|t, l);
    }
    ZFlip(q1,l);
    for(i=M-1; i>=0; i--) {
        t = q(mask[i]^prop[i]);
        XNot(q1[i]|t, l);
        XNot(q1[i], l|q(mask[i]));
        XNot(t,l);
    }
}

#include<iostream>

main() {
    long i,m,s;
    srand(100);
    for(i=0; i<10; i++) {
        m = QCount(SAT,N);
        std::cout << "number of solutions: " << m;
        if(m) {
            s = Grover(SAT,N,m);
            std::cout << ", solution: " << s;
        }
        std::cout << std::endl;
    }
}