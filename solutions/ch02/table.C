// Solution to Exercise 2.1 -- a table of square roots and logarithms
void table()
{
   printf("%4s %10s %10s\n", "x", "sqrt(x)", "log(x)");
   for (int x = 1; x <= 10; x++) {
      printf("%4d %10.4f %10.4f\n", x, sqrt(x), log(x));   // log = natural log
   }
}
