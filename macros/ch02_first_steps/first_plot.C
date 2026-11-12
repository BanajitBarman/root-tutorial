// first_plot.C -- draw a mathematical function and save it as a picture
// Run with:   root -l first_plot.C

void first_plot()
{
   // A canvas is the window you draw on: name, title, width, height (pixels)
   auto c = new TCanvas("c", "My first plot", 800, 600);

   // A 1D function: name, formula, x-min, x-max
   auto f = new TF1("f", "sin(x)/x", -10, 10);

   // Title and axis titles in one string, separated by ';'
   f->SetTitle("My first ROOT plot;x;sin(x)/x");
   f->SetLineColor(kBlue);
   f->SetLineWidth(3);
   f->Draw();

   c->SaveAs("first_plot.png");   // also try .pdf, .jpg, .svg, .root
}
