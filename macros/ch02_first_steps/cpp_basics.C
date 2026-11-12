// cpp_basics.C -- the small amount of C++ you need for ROOT
// Run with:   root -l cpp_basics.C

// A helper function: takes a number, returns a number
double kineticEnergy(double mass, double speed)
{
   return 0.5 * mass * speed * speed;
}

void cpp_basics()
{
   // 1. Variables have a type
   int    nEvents = 1000;        // whole numbers
   double energy  = 13.6;        // real numbers
   bool   isGood  = true;        // true / false
   TString name   = "electron";  // text (ROOT's string class)

   std::cout << name << ": E = " << energy << " eV, N = " << nEvents
             << ", good = " << isGood << std::endl;

   // 2. Careful: integer division throws away the fraction!
   std::cout << "1/2   = " << 1 / 2   << "   (integer division)" << std::endl;
   std::cout << "1./2  = " << 1. / 2  << " (real division)"     << std::endl;

   // 3. if / else
   if (energy > 10) {
      std::cout << "High energy" << std::endl;
   } else {
      std::cout << "Low energy" << std::endl;
   }

   // 4. for loop
   double sum = 0;
   for (int i = 1; i <= 100; i++) {
      sum += i;
   }
   std::cout << "Sum of 1..100 = " << sum << std::endl;

   // 5. Arrays and vectors
   double masses[3] = {0.000511, 0.10566, 1.77686};   // e, mu, tau in GeV
   std::vector<TString> names = {"e", "mu", "tau"};
   for (int i = 0; i < 3; i++) {
      std::cout << names[i] << " mass = " << masses[i] << " GeV" << std::endl;
   }

   // 6. Calling a function
   std::cout << "Kinetic energy = " << kineticEnergy(2.0, 3.0) << " J" << std::endl;

   // 7. Objects and pointers: "new" creates an object that lives on after
   //    the macro ends; "->" calls a function (method) of the object
   auto h = new TH1D("h", "Example", 10, 0, 10);
   h->Fill(3.5);
   std::cout << "Histogram entries: " << h->GetEntries() << std::endl;
}
