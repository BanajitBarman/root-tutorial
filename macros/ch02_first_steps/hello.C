// hello.C -- your first ROOT macro
// Run it from a terminal with:   root -l hello.C
// The function name (hello) must be the same as the file name (hello.C).

void hello()
{
   std::cout << "Hello from ROOT " << gROOT->GetVersion() << "!" << std::endl;

   for (int i = 1; i <= 5; i++) {
      std::cout << i << " squared is " << i * i << std::endl;
   }
}
