---
title: "ROOT for Beginners"
subtitle: "Participant manual for the two-day hands-on tutorial"
author: "Banajit Barman"
date: "November 2026 — written for ROOT 6.40"
---

# About this manual

This manual goes with a two-day, hands-on tutorial on **ROOT**, the data-analysis
framework developed at CERN. It is written for complete beginners: you do not need to
know ROOT, and you only need a little programming experience.

Everything that we do in the tutorial is written here, step by step. Every example
has been run, and every plot you see in this manual was made by the example code next
to it. You can therefore use the manual in three ways:

1. **Before the tutorial** — install ROOT (Chapter 1) and read Chapter 2.
2. **During the tutorial** — follow along, run the examples, and do the exercises.
3. **After the tutorial** — as a reference when you start your own analysis.

## Before you come

Please do these three things **before Day 1**. Installing software takes time, and
we want to spend the tutorial on ROOT, not on installation problems.

1. Install ROOT on your laptop, following **Chapter 1**.
2. Download the tutorial files and run the two check scripts in
   `macros/ch01_install` (Section 1.9). If both print
   `Everything is fine. You are ready for the tutorial.`, you are ready.
3. Read **Chapter 2** (First steps). It is short.

If something does not work, write to the organisers before the tutorial
and include the error message you see.

## The two days at a glance

| Time          | Day 1: ROOT basics                    | Day 2: Working with data                   |
|---------------|----------------------------------------|---------------------------------------------|
| 09:30 – 10:00 | Welcome, what is ROOT                  | **Ch. 6** ROOT files                         |
| 10:00 – 11:00 | **Ch. 2** The prompt and macros        | **Ch. 7** Trees (TTree)                      |
| 11:00 – 11:15 | *Break*                                | *Break*                                     |
| 11:15 – 12:45 | **Ch. 3** Histograms                   | **Ch. 7** Trees (cont.), **Ch. 8** PyROOT    |
| 12:45 – 13:45 | *Lunch*                                | *Lunch*                                     |
| 13:45 – 15:00 | **Ch. 4** Graphs and functions         | **Ch. 9** RDataFrame                         |
| 15:00 – 15:15 | *Break*                                | *Break*                                     |
| 15:15 – 16:45 | **Ch. 5** Fitting                      | **Ch. 10** Mini-project                      |
| 16:45 – 17:15 | Recap and questions                    | Results, wrap-up, feedback                   |

Each session starts with a short demonstration (about 20 minutes). The rest of the
time you work on the examples and exercises yourself, with helpers around you.

## How to read this manual

**Code you type into a file** is shown in a box with colours, with the file name in
the first comment line:

```cpp
// hello.C -- your first ROOT macro
void hello()
{
   std::cout << "Hello!" << std::endl;
}
```

**Commands you type in a terminal** (a Linux/macOS terminal, or the Ubuntu window
on Windows) look like this:

```bash
root -l hello.C
```

**What the computer prints back** is shown in a grey box:

```text
Processing hello.C...
Hello!
```

**Lines typed at the ROOT prompt** start with `root [0]`, `root [1]`, ... You type only
the part after the prompt:

```text
root [0] sqrt(2)
(double) 1.4142136
```

> **Tip:** Boxes like this one contain tips, warnings and short explanations. They are
> worth reading — most of them describe a mistake that beginners make very often.

## The tutorial files

All examples and exercise solutions are in the folder `root-tutorial`, which you get
from the organisers. It looks like this:

```text
root-tutorial/
├── ROOT_Tutorial_Manual.pdf     this manual
├── macros/                      all examples, one folder per chapter
│   ├── ch01_install/            installation check
│   ├── ch02_first_steps/
│   ├── ch03_histograms/
│   ├── ch04_graphs_functions/
│   ├── ch05_fitting/
│   ├── ch06_files/
│   ├── ch07_trees/
│   ├── ch08_pyroot/
│   ├── ch09_rdataframe/
│   └── ch10_project/
└── solutions/                   solutions to the exercises
```

Always run an example **from inside its own folder**. Some examples read a file that
is in the same folder, and they will not find it if you start them from somewhere else:

```bash
cd root-tutorial/macros/ch03_histograms
root -l hist_basic.C
```

> **Tip:** Some examples use files made by earlier examples. Chapter 7 writes
> `toy_tree.root`, and Chapters 8 and 9 read it. If an example says that a file
> does not exist, run the example that makes it first. Each example says at the
> top which file it needs.
