// hello_compiled.C -- a macro that can also be COMPILED (ACLiC)
// Run interpreted:   root -l hello_compiled.C
// Run compiled:      root -l hello_compiled.C+      (note the + at the end)
//
// A compiled macro must #include the headers of everything it uses,
// exactly like a normal C++ program.
#include <iostream>
#include <cmath>
#include "TROOT.h"

void hello_compiled()
{
   std::cout << "Hello from ROOT " << gROOT->GetVersion() << "!" << std::endl;

   double sum = 0;
   for (int i = 1; i <= 10000000; i++) {
      sum += 1.0 / (double(i) * i);     // converges to pi^2 / 6
   }
   std::cout << "sqrt(6 * sum) = " << std::sqrt(6 * sum) << std::endl;
}
