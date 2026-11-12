// unnamed.C -- an "unnamed" macro: just a block of code in { }
// Run with:   root -l unnamed.C
{
   double x = 3.0;
   std::cout << "x       = " << x << std::endl;
   std::cout << "sqrt(x) = " << sqrt(x) << std::endl;
   std::cout << "x^3     = " << pow(x, 3) << std::endl;
}
