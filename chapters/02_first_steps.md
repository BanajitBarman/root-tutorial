# 2 First steps: the ROOT prompt and macros

In this chapter you start ROOT, use it as a calculator, write your first *macro* (a
small program), and make your first plot. You will also learn the small amount of C++
that you need for the rest of the tutorial.

Examples for this chapter are in `macros/ch02_first_steps`.

## 2.1 Starting and quitting ROOT

Open a terminal (remember `conda activate root-tutorial` if you use conda) and type:

```bash
root -l
```

The `-l` option means "do not show the welcome banner". You now see the ROOT prompt:

```text
root [0]
```

ROOT is waiting for a command. The number in brackets counts your commands.
Commands that start with a dot are special ROOT commands. The most important ones:

| Command        | What it does                                   |
|----------------|------------------------------------------------|
| `.q`           | quit ROOT                                      |
| `.x file.C`    | run (execute) the macro `file.C`               |
| `.L file.C`    | load the macro `file.C` without running it     |
| `.ls`          | list the objects in the current folder/file    |
| `.!` *command* | run a shell command, e.g. `.! ls`              |
| `.help`        | list all dot commands                          |

Two keyboard tricks save a lot of typing:

- **Up / Down arrows** bring back your previous commands.
- **Tab** completes names. Type `TMath::Si` and press Tab to see all functions that
  start with `Si`. After an object, `h->Get` + Tab lists all its methods that start
  with `Get`.

## 2.2 ROOT as a calculator

At the prompt you can type any C++ expression and ROOT prints the result, with its
type in brackets:

```text
root [0] 1 + 2
(int) 3
root [1] sqrt(2)
(double) 1.4142136
root [2] TMath::Pi()
(double) 3.1415927
root [3] double r = 2.5
(double) 2.5000000
root [4] TMath::Pi() * r * r
(double) 19.634954
root [5] int a = 7, b = 2
(int) 2
root [6] a / b
(int) 3
```

Look at the last line: 7 / 2 gives **3**, not 3.5! When both numbers are integers
(`int`), C++ does *integer division* and throws away the fraction. Write `7.0 / 2` or
`a / 2.0` to get 3.5. This is one of the most common mistakes in C++.

> **Tip:** If you end a line with a semicolon `;`, ROOT does not print the result.
> This is useful for long calculations: `double x = 3.0;`

`TMath` contains many mathematical functions and constants: `TMath::Pi()`,
`TMath::Exp(x)`, `TMath::Gaus(x, mean, sigma)`, `TMath::Prob(chi2, ndf)`, and the
normal C/C++ functions `sqrt`, `pow`, `exp`, `log`, `sin`, `cos`, `atan2` all work too.

## 2.3 Your first macro

Typing everything at the prompt is fine for quick tests, but for real work you write
your commands in a file, called a **macro**. Create a file `hello.C` in a text editor:

{{code: macros/ch02_first_steps/hello.C}}

There are three ways to run it. From the terminal:

```bash
root -l hello.C
```

From the ROOT prompt:

```text
root [0] .x hello.C
```

Or from the terminal, quitting ROOT automatically at the end (`-q`):

```bash
root -l -q hello.C
```

In all cases you see:

{{output: macros/ch02_first_steps/hello}}

> **The golden rule of macros:** the file `hello.C` must contain a function called
> `hello()`. ROOT runs the function that has the **same name as the file**. If the names
> do not match, ROOT only prints a warning and does not run anything:
>
> ```text
> warning: Failed to call `hello()` to execute the macro.
> Add this function or rename the macro. Falling back to `.L`.
> ```

The useful command-line options are:

| Option | Meaning |
|--------|---------|
| `-l`   | no welcome banner |
| `-q`   | quit after running the macros |
| `-b`   | *batch mode*: no graphics windows (plots are still saved to files) — useful on remote computers and for long jobs |

You can combine them: `root -l -b -q hello.C`.

## 2.4 Macros with arguments

A macro function can take arguments, with default values:

{{code: macros/ch02_first_steps/squares.C}}

```bash
root -l -q squares.C          # uses the default n = 10
root -l -q 'squares.C(3)'     # n = 3; note the quotes around the whole thing
```

From the ROOT prompt: `.x squares.C(3)`. With `n = 10` the output is:

{{output: macros/ch02_first_steps/squares}}

> **Why the quotes?** Without them, the terminal tries to understand the brackets
> itself and gives an error. Strings inside need double quotes, so use single quotes
> outside: `root -l 'myMacro.C("data.root", 5)'`.

## 2.5 Loading macros and unnamed macros

`.L` loads a macro without running it. After that, you can call its functions as often
as you like:

```text
root [0] .L squares.C
root [1] squares(2)
1^2 = 1
2^2 = 4
root [2] squares(4)
1^2 = 1
2^2 = 4
3^2 = 9
4^2 = 16
```

A file can contain several functions; after `.L` all of them are available.

There is also a second kind of macro, the **unnamed macro**: just a block of code
inside `{ }`, with no function name:

{{code: macros/ch02_first_steps/unnamed.C}}

{{output: macros/ch02_first_steps/unnamed}}

Unnamed macros are handy for quick tests, but they cannot take arguments and cannot
be compiled. In this tutorial we use named macros.

## 2.6 The C++ you need for ROOT

You do not need to be a C++ expert to use ROOT. This macro shows almost everything
we use in the tutorial:

{{code: macros/ch02_first_steps/cpp_basics.C}}

Output:

{{output: macros/ch02_first_steps/cpp_basics}}

The important points:

1. **Every variable has a type**: `int` (whole number), `double` (real number),
   `bool` (true/false), `TString` or `std::string` (text). In ROOT you often also see
   `Int_t`, `Double_t`, `Float_t`, ... which are the same types with ROOT names.
2. **Statements end with a semicolon** `;`. Blocks of code are inside `{ }`.
3. **Integer division**: `1/2` is `0`. Use `1./2` or `1.0/2`.
4. **Loops**: `for (int i = 0; i < n; i++) { ... }` runs the block for i = 0, 1, ..., n-1.
5. **Arrays start at index 0**: the first element of `masses` is `masses[0]`.
6. **Objects and pointers.** ROOT is built of *classes* (`TH1D`, `TF1`, `TCanvas`, ...).
   `new TH1D(...)` creates an object and gives you a *pointer* to it. You call the
   object's functions (*methods*) with an arrow: `h->Fill(3.5)`. `auto` lets C++ work
   out the type for you: `auto h = new TH1D(...)` is the same as
   `TH1D *h = new TH1D(...)`.
7. **Comments** start with `//` and go to the end of the line.

> **Why `new`?** An object made with `new` lives until you delete it, so a histogram or
> canvas made with `new` is still there after the macro has finished, and you can keep
> looking at it and working with it. An object made without `new`
> (`TH1D h("h", ...)`) is deleted when the macro ends, and its plot disappears from the
> screen. **For histograms, graphs, functions and canvases, always use `new` in macros.**

## 2.7 Your first plot

{{code: macros/ch02_first_steps/first_plot.C}}

```bash
root -l first_plot.C
```

{{output!: macros/ch02_first_steps/first_plot}}

![The output of `first_plot.C`: the function sin(x)/x.](figures/first_plot.png)

Three objects work together here, and you will see them again and again:

- a **canvas** (`TCanvas`) — the window, or the page, that you draw on;
- the thing you draw — here a **function** (`TF1`);
- `Draw()`, which puts it on the current canvas.

The title string `"My first ROOT plot;x;sin(x)/x"` sets three things at once, separated
by semicolons: the plot title, the x-axis title, and the y-axis title. This works for
histograms and graphs too.

**The canvas is interactive.** Try this in the window that opened:

- Move the mouse over the curve or the axes: the status bar at the bottom shows what is
  under the mouse. (If you do not see it, use *View → Event Statusbar*.)
- **Zoom**: click on the x axis and drag along it. To zoom out again, right-click
  the axis and choose *UnZoom*.
- **Right-click** on any object (the curve, an axis, the title) to get a menu of
  everything you can do with it: change colour, line width, axis range, and more.
- *View → Editor* opens a side panel to change styles with the mouse.
- *File → Save* saves the canvas as PNG, PDF, and other formats.

## 2.8 Saving plots

`SaveAs` chooses the format from the file extension:

| Command | Result |
|---------|--------|
| `c->SaveAs("plot.png")` | picture (good for slides and web pages) |
| `c->SaveAs("plot.pdf")` | vector graphics: sharp at any size (best for papers and theses) |
| `c->SaveAs("plot.svg")` | vector graphics for web pages and editing |
| `c->SaveAs("plot.root")` | the canvas itself; open it later and keep editing |
| `c->SaveAs("plot.C")` | a macro that redraws the canvas |

## 2.9 Compiled macros

So far ROOT has *interpreted* our macros: it read and ran them line by line. You can
also ask ROOT to **compile** a macro into fast machine code first, by adding a `+`
after the name. This uses ROOT's built-in compiler tool, called **ACLiC**.

Compiled code runs much faster (important for loops over millions of events), and the
compiler finds more mistakes. But a compiled macro must be correct C++: it must
`#include` the header file of every class and function it uses, like any C++ program.

{{code: macros/ch02_first_steps/hello_compiled.C}}

```bash
root -l -q hello_compiled.C+
```

```text
Processing hello_compiled.C+...
Info in <TMacOSXSystem::ACLiC>: creating shared library .../ch02_first_steps/./hello_compiled_C.so
Hello from ROOT 6.40.04!
sqrt(6 * sum) = 3.14159
```

ROOT made a library file `hello_compiled_C.so` (and a few other files) next to the
macro. The next time, it uses that file directly, unless you changed the macro. Use
`++` to force a fresh compilation.

If you try to compile `hello.C`, which has no `#include` lines, the compiler complains:

```text
././hello.C:10:12: error: no member named 'cout' in namespace 'std'
      std::cout << i << " squared is " << i * i << std::endl;
      ~~~~~^
```

The fix is to add `#include <iostream>` (for `std::cout`) and `#include "TROOT.h"`
(for `gROOT`) at the top. The header for a ROOT class is its name plus `.h`:
`TH1D.h`, `TCanvas.h`, `TF1.h`, ...

> **Tip:** In this tutorial you can always run macros the normal (interpreted) way.
> Compiling is something you will want later, for long analysis jobs.

## 2.10 The ROOT object browser

ROOT has a graphical **browser** to look at files, histograms and trees:

```text
root [0] new TBrowser
```

or, from the terminal, to open a particular file:

```bash
rootbrowse my_results.root
```

In recent ROOT versions the browser opens inside a web-browser window. Double-click
a ROOT file to open it, and double-click a histogram to draw it. If you prefer the
older look, start ROOT with `root --web=off`. We will use the browser in Chapter 6.

## 2.11 Getting help

- **Tab completion** at the prompt (Section 2.1) shows the methods of any object.
- **The reference guide** has a page for every class, with all methods and many
  examples: <https://root.cern/doc/master/>. Search for a class name, for example
  `TH1` (<https://root.cern/doc/master/classTH1.html>).
- **The ROOT manual**: <https://root.cern/manual/>.
- **The ROOT forum**: <https://root-forum.cern.ch>. Search it first: most beginner
  questions have already been answered.
- **ROOT's own tutorials**: hundreds of small examples. Find the folder with
  `root-config --tutdir`, or browse them online at
  <https://root.cern/doc/master/group__Tutorials.html>.

## Key points

- Start ROOT with `root -l`, quit with `.q`.
- A macro `name.C` must contain a function `name()`. Run it with `root -l name.C` or
  `.x name.C`.
- `-b` (no graphics) and `-q` (quit at the end) are useful for running macros in a script.
- Use `new` to create histograms, functions, graphs and canvases in macros.
- Watch out for integer division: `1/2 == 0`.
- `c->SaveAs("file.png")` or `"file.pdf"` saves your plot.

## Exercises

**Exercise 2.1 — A table.** Write a macro `table.C` that prints a table with three
columns: x, √x and ln(x), for x = 1, 2, ..., 10.
*Hint:* `printf("%4d %10.4f\n", i, value);` prints an integer in 4 characters and a real
number with 4 decimals in 10 characters. `log(x)` is the natural logarithm.

**Exercise 2.2 — Your own function plot.** Change `first_plot.C` so that it draws the
damped oscillation e^(−x/3) cos(x) for x from 0 to 15, as a red line, with the axis
titles "Time t (s)" and "Amplitude". Save it both as PNG and as PDF, and open the two
files to compare them (zoom in on the PDF!).
*Hint:* in a formula, write e^(−x/3) as `exp(-x/3)`.

Solutions are in Appendix A and in the folder `solutions/ch02`.
