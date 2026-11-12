# 7 Trees: storing and reading event data

A histogram stores a summary of the data. But in an experiment you want to keep **every
event**, with all its measured quantities, so that you can decide later which cuts to
apply and which plots to make. ROOT stores such data in a **tree** (class `TTree`).

Examples for this chapter are in `macros/ch07_trees`. Run `tree_write.C` first: it
creates the file `toy_tree.root` that all the other examples (also in Chapters 8 and 9)
read.

## 7.1 What is a tree?

Think of a tree as a big table:

- each **row** is one *entry* — usually one collision event;
- each **column** is a *branch* — one measured quantity, like the energy.

| eventNumber | energy | nHits | x | y | hitEnergy |
|---|---|---|---|---|---|
| 0 | 17.49 | 2 | 9.88 | −1.56 | {11.06, 6.44} |
| 1 | 0.02 | 1 | −5.28 | 17.10 | {0.02} |
| 2 | 18.94 | 11 | 3.39 | 11.29 | {1.47, 0.94, 0.21, 2.05, ...} |
| ... | ... | ... | ... | ... | ... |

A branch can hold a single number per event, or an **array** whose length changes from
event to event (like `hitEnergy`: one value per detector hit). Trees can have billions of
rows and hundreds of columns. ROOT stores each column separately and compressed, so
when you read only two columns, only those two are read from disk. This makes trees very
fast for analysis.

## 7.2 Writing a tree

Our example is a toy calorimeter: for each event we store the total energy, the impact
position (x, y), the number of hits, and the energy of each hit.

{{code: macros/ch07_trees/tree_write.C}}

{{output: macros/ch07_trees/tree_write}}

The recipe is always the same:

1. **Make variables** that hold the values of one event.
2. **Make one branch per variable** with `tree->Branch("name", &variable)`. The `&`
   gives the *address* of the variable, so that the tree knows where to read the value
   from when you call `Fill`.
3. **In the event loop**, set the variables, then call `tree->Fill()`. `Fill` copies the
   current values of all variables into a new row.
4. **Write** the tree to the file, and close the file.

`std::vector<double>` is C++'s array that can grow; `push_back` adds an element and
`clear()` empties it. Do not forget to `clear()` it at the start of every event!

> **The older way to make branches.** In many existing programs you will see a third
> argument, a *leaf list*, that states the type: `tree->Branch("energy", &energy,
> "energy/D")`. The letter after `/` is the type: `I` int, `F` float, `D` double,
> `L` 64-bit integer, `O` bool. Both ways give the same result for simple numbers.

## 7.3 Looking at a tree

{{code: macros/ch07_trees/tree_inspect.C}}

{{output: macros/ch07_trees/tree_inspect}}

`tree->Print()` lists every branch with its type (`/I` int, `/D` double, or a class name
like `vector<double>`) and its size. `tree->Scan("columns", "cut", "", n)` prints a table.

> **Watch out:** the last argument of `Scan` is the number of *entries to look at*, not
> the number of rows to print. With the cut `nHits > 10`, ROOT looks at the first 10
> events and prints only the 2 that pass.

## 7.4 Quick plots: `TTree::Draw`

`TTree::Draw` makes a histogram from a tree in one line. It is the fastest way to look
at your data:

{{code: macros/ch07_trees/tree_draw.C}}

{{output: macros/ch07_trees/tree_draw}}

![Four quick plots made with `TTree::Draw`.](figures/tree_draw.png)

The syntax:

| You write | You get |
|-----------|---------|
| `tree->Draw("energy")` | histogram of `energy`, binning chosen automatically |
| `tree->Draw("energy", "nHits > 20")` | only events that pass the cut |
| `tree->Draw("energy>>hE(50,0,100)")` | the histogram is called `hE`, with 50 bins from 0 to 100 |
| `tree->Draw("nHits:energy", "", "COLZ")` | 2D: **y first**, then x |
| `tree->Draw("sqrt(x*x + y*y)")` | any formula of the branches |
| `tree->Draw("energy", "", "goff")` | fill the histogram but do not draw it |
| `tree->GetEntries("energy > 50")` | count the events that pass a cut |

Cuts use C++ syntax: `&&` (and), `||` (or), `!` (not), for example
`"energy > 10 && nHits < 30"`. A cut can also be a *weight*:
`tree->Draw("energy", "w * (nHits > 5)")` fills each event with weight `w`.

After `Draw("...>>hE(...)")`, you get the histogram back with
`(TH1F *)gDirectory->Get("hE")` to change its title or style, or to use it further.

For array branches like `hitEnergy`, `Draw("hitEnergy")` fills **one entry per element**
(every hit of every event). There are also special functions that work on the whole array
of one event: `Length$(hitEnergy)` (number of elements), `Sum$(hitEnergy)`,
`Max$(hitEnergy)`, `Min$(hitEnergy)`.

## 7.5 Reading a tree in a loop

`TTree::Draw` is great for quick plots, but sometimes you need full control: complicated
calculations, several histograms at once, counting. Then you loop over the events
yourself:

{{code: macros/ch07_trees/tree_loop.C}}

{{output: macros/ch07_trees/tree_loop}}

![Histograms filled in an event loop.](figures/tree_loop.png)

It is the mirror image of writing: you make variables, connect them to the branches
with `SetBranchAddress("name", &variable)`, and `tree->GetEntry(i)` fills them with the
values of event `i`.

> **The variable type must match the branch type exactly.** The branch `energy` was
> written as a `double`. If you connect it to a `float`, you get:
>
> ```text
> Error in <TTree::SetBranchAddress>: The pointer type given "Float_t" (5) does not
> correspond to the type needed "Double_t" (8) by the branch: energy
> ```
>
> and the values you read are garbage. Use `tree->Print()` to check the types.

`Long64_t` is ROOT's name for a 64-bit integer; use it to count entries, because trees can
have more than 2 billion of them.

## 7.6 TTreeReader: a safer loop

`TTreeReader` is the modern way to write the same loop. It checks the types for you and
makes array branches easy:

{{code: macros/ch07_trees/tree_reader.C}}

{{output: macros/ch07_trees/tree_reader}}

- `TTreeReaderValue<double> energy(reader, "energy")` connects to a single-value branch.
  Use `*energy` to get the value.
- `TTreeReaderArray<double> hitEnergy(reader, "hitEnergy")` connects to an array branch.
  You can loop over it, or use `hitEnergy[i]` and `hitEnergy.GetSize()`.
- `while (reader.Next())` loops over all entries.

With the wrong type, `TTreeReader` gives a clear message instead of garbage:

```text
Error in <TTreeReaderValueBase::CreateProxy()>: The branch energy contains data of
type double. It cannot be accessed by a TTreeReaderValue<float>
```

## 7.7 Many files: TChain

Data are usually split into many files with the same tree inside. A `TChain` makes them
look like one big tree:

```cpp
TChain chain("events");          // the name of the tree inside the files
chain.Add("run1.root");
chain.Add("run2.root");
chain.Add("data/run*.root");     // wildcards work too
std::cout << chain.GetEntries() << std::endl;
chain.Draw("energy");            // everything that works for a TTree works here
```

## 7.8 Which method should I use?

| Method | Use it for |
|--------|-----------|
| `TTree::Draw`, `Scan`, `GetEntries(cut)` | quick looks and checks |
| Loop with `TTreeReader` (or `SetBranchAddress`) | full control in C++; old code uses `SetBranchAddress` |
| `RDataFrame` (Chapter 9) | **the recommended way for analyses** — short, fast, and works in Python |

## Key points

- A tree is a table: rows are events (entries), columns are branches.
- Write: `Branch("name", &var)`, set the variables, `Fill()`, then `Write()`.
- Look: `Print()` for the structure, `Scan()` for values, `Draw()` for quick plots.
- `Draw("y:x")` puts y first. `Draw("expr>>h(n,a,b)")` names and bins the histogram.
- In a loop, the variable type must match the branch type exactly. `TTreeReader` checks
  this for you.
- `TChain` treats many files as one tree.

## Exercises

**Exercise 7.1 — Compare selections.** Using `toy_tree.root` and `TTree::Draw` with the
`"goff"` option, make two histograms of `energy`: one for all events, and one for events
with more than 20 hits. Normalise both to unit area and draw them on the same canvas with
a legend. Why does the second one have no events at low energy?

**Exercise 7.2 — Count in a loop.** Write a loop with `SetBranchAddress` that counts the
events with energy above 50 GeV *and* a distance r = √(x² + y²) less than 10 cm. Check
your answer with a single `tree->GetEntries("...")` call.

Solutions are in Appendix A and in `solutions/ch07`.
