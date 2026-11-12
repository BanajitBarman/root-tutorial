// squares.C -- a macro that takes an argument
// Run with the default value:   root -l squares.C
// Run with your own value:      root -l 'squares.C(3)'

void squares(int n = 10)
{
   for (int i = 1; i <= n; i++) {
      std::cout << i << "^2 = " << i * i << std::endl;
   }
}
