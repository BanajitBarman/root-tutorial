# 1 Installing ROOT

## 1.1 What is ROOT?

ROOT is a free, open-source software framework for data analysis. It was created at
CERN and is used by almost every experiment in particle and nuclear physics,
including all the LHC experiments (ALICE, ATLAS, CMS and LHCb). With ROOT you can:

- make **histograms** and **graphs**, and draw publication-quality plots;
- **fit** functions to data and get the parameters with their errors;
- **store** very large amounts of data (billions of collision events) in ROOT files;
- **analyse** those data quickly, on one laptop or on many computers at once.

You can use ROOT in two languages:

- **C++** — ROOT includes a C++ *interpreter* called Cling, so you can type C++
  commands and run C++ files (called *macros*) without compiling them first.
- **Python** — the same ROOT classes are available in Python through **PyROOT**.

On Day 1 we use C++ macros; on Day 2 we also use Python. You do not need to be an
expert in either language — Section 2.6 explains the little C++ you need.

This manual was written for **ROOT 6.40**. The latest stable release when it was
written is **ROOT 6.40.04** (27 August 2026). Any ROOT 6.36 or newer will work for
almost everything here.

## 1.2 Which installation method should I use?

There are many ways to install ROOT. For this tutorial we recommend the following:

| Your computer        | Recommended method                                  | Section |
|----------------------|-----------------------------------------------------|---------|
| Linux (any)          | **conda** (Miniforge)                               | 1.3     |
| macOS                | **conda** (Miniforge), or **Homebrew**              | 1.3, 1.5 |
| Windows 10 / 11      | **WSL** (Linux inside Windows), then conda          | 1.6     |
| No admin rights, or nothing works | Docker, or a CERN computer (lxplus/SWAN) | 1.7, 1.8 |

Why conda? It works the same way on Linux and macOS, it does not need administrator
rights, it installs ROOT together with a matching Python and Jupyter, and it does not
interfere with anything else on your computer. If all participants use the same
method, helpers can also fix problems faster.

> **Disk space and time:** a conda installation of ROOT needs about 2–3 GB of disk
> space and 10–20 minutes on a good internet connection. Please do it at home, not at
> the venue.

## 1.3 Linux and macOS: conda (recommended)

**Step 1 — Install Miniforge.** Miniforge is a small installer for conda that uses the
community package collection *conda-forge*, where ROOT lives. Open a terminal and type:

```bash
curl -L -O "https://github.com/conda-forge/miniforge/releases/latest/download/Miniforge3-$(uname)-$(uname -m).sh"
bash Miniforge3-$(uname)-$(uname -m).sh
```

The installer asks a few questions. Press Enter to read the licence, type `yes` to
accept it, press Enter to accept the default install location, and type `yes` when it
asks whether to initialise conda. Then **close the terminal and open a new one**.

> **Already have Anaconda or Miniconda?** You can use it instead of Miniforge.
> Skip Step 1 and go to Step 2.

**Step 2 — Create an environment with ROOT.** An *environment* is a separate folder
with its own set of programs. We make one called `root-tutorial` with ROOT, Jupyter
and NumPy in it:

```bash
conda config --set channel_priority strict
conda create -c conda-forge --name root-tutorial root jupyterlab numpy
```

conda shows a list of packages and asks `Proceed ([y]/n)?`. Type `y`. The download
takes a while.

> **Note:** The first command (`channel_priority strict`) is recommended by the ROOT
> team; without it you can get conflicts between packages on some systems.

**Step 3 — Activate the environment.**

```bash
conda activate root-tutorial
```

Your prompt now starts with `(root-tutorial)`. **You must do this in every new
terminal** before you use ROOT. To check that ROOT is there:

```bash
root-config --version
```

```text
6.40.04
```

Now go to Section 1.9 to test your installation.

## 1.4 Linux: other methods

You only need these if you do not want to use conda.

**Package managers.** Several Linux distributions have ROOT packages. They are made by
the community, not by the ROOT team, and are sometimes an older version:

```bash
# Fedora (the second line adds Python and Jupyter support)
sudo dnf install root
sudo dnf install root python3-root root-notebook

# Arch Linux
sudo pacman -S root

# Gentoo
sudo emerge sci-physics/root
```

**Ubuntu snap.** On Ubuntu (and other distributions with *snap*):

```bash
sudo snap install root-framework
snap run root-framework
```

The snap has its own Python. To use ROOT from Python, start Python with the command
`pyroot` instead of `python`.

> **Ubuntu and Debian** do not have an official ROOT package (`apt install root`
> does not work). The ROOT team says one is being prepared. Until then, use conda,
> the snap, or the pre-compiled binary below.

**Pre-compiled binary.** The ROOT team provides ready-made packages for several Linux
versions. For Ubuntu 24.04:

```bash
# 1. Install the system libraries that ROOT needs
sudo apt install binutils cmake dpkg-dev g++ gcc libssl-dev git libx11-dev \
    libxext-dev libxft-dev libxpm-dev python3 libtbb-dev libvdt-dev libgif-dev

# 2. Download and unpack ROOT (about 400 MB)
wget https://root.cern/download/root_v6.40.04.Linux-ubuntu24.04-x86_64-gcc13.3.tar.gz
tar -xzf root_v6.40.04.Linux-ubuntu24.04-x86_64-gcc13.3.tar.gz

# 3. Set up the environment (this creates the "root" command)
source root/bin/thisroot.sh
```

For Ubuntu 22.04 the file is `root_v6.40.04.Linux-ubuntu22.04-x86_64-gcc11.4.tar.gz`.
Files for other distributions (AlmaLinux, Debian, Fedora, Ubuntu 26.04) are listed on
the release page: <https://root.cern/releases/release-64004/>.

Step 3 must be repeated in every new terminal. To do it automatically, add it to the
end of your `~/.bashrc` file, with the full path:

```bash
echo 'source $HOME/root/bin/thisroot.sh' >> ~/.bashrc
```

## 1.5 macOS: Homebrew

If you already use Homebrew (<https://brew.sh>), this is the quickest way:

```bash
xcode-select --install     # Apple command-line tools, if not yet installed
brew install root
```

Check it:

```bash
root-config --version
```

> **Python with Homebrew ROOT.** Homebrew's ROOT works only with *one* Python
> version: the one it was built with. Ask ROOT which one:
>
> ```bash
> root-config --python-version
> ```
>
> If it prints `3.14.7`, use Python 3.14. Homebrew installs it, but it may not be the
> `python3` you get by default. Run it with its full path:
>
> ```bash
> $(brew --prefix python@3.14)/bin/python3.14 check_pyroot.py
> ```
>
> If you use a different Python, `import ROOT` fails with
> `ROOT was built for Python 3.14.7, but you are running Python 3.9.6`.
> For Jupyter notebooks with Homebrew ROOT, conda (Section 1.3) is simpler.

## 1.6 Windows: WSL (recommended)

ROOT is mainly developed for Linux and macOS. On Windows, the easiest way is
**WSL** (*Windows Subsystem for Linux*). It runs a real Ubuntu Linux inside Windows,
and ROOT windows open on your Windows desktop like normal windows.

**Step 1 — Install WSL and Ubuntu.** Open **PowerShell as Administrator** (right-click
the Start button → *Terminal (Admin)* or *Windows PowerShell (Admin)*) and type:

```text
wsl --install
```

Restart the computer when asked. After the restart an **Ubuntu** window opens and
asks you to choose a Linux user name and password. Remember the password — Linux asks
for it when you install things with `sudo`.

**Step 2 — Install ROOT inside Ubuntu.** In the Ubuntu window, follow the conda
instructions of Section 1.3 exactly. They are the same as for Linux.

**Step 3 — Install a small package for graphics** (in the Ubuntu window):

```bash
sudo apt update
sudo apt install libgl1 libglu1-mesa
```

From now on, open the **Ubuntu** app from the Start menu whenever you want to use
ROOT, and type `conda activate root-tutorial`.

> **Where are my Windows files?** Inside Ubuntu, your Windows `C:` drive is at
> `/mnt/c`. For example, your Windows Downloads folder is
> `/mnt/c/Users/<your Windows name>/Downloads`. ROOT runs faster on files inside the
> Linux home folder, so copy the tutorial files there:
>
> ```bash
> cp -r /mnt/c/Users/<your Windows name>/Downloads/root-tutorial ~/
> cd ~/root-tutorial
> ```

> **No windows appear?** Windows 11 shows Linux windows out of the box. On Windows 10,
> first update WSL with `wsl --update` (in PowerShell). If plots still do not appear,
> you can still do the whole tutorial: every example also saves its plot as a PNG
> file, which you can open from Windows.

### Native Windows installer (alternative)

ROOT also has a native Windows version. On the install page it is still marked as a
*beta*, and it needs a matching Visual Studio, so we recommend WSL. If you still want it:

1. Install **Visual Studio** (the free *Community* edition is fine). In the Visual
   Studio Installer, select the workload **"Desktop development with C++"**.
   You need the same Visual Studio version that ROOT was built with: the `vc18` in the
   file name below. The release page says which version this is.
2. If you want PyROOT, install **Python 3.14** (64-bit) from <https://www.python.org>.
   The `python314` in the file name means that ROOT needs exactly this Python version.
3. Download and run the installer
   `root_v6.40.04.win64.python314.vc18.exe` from
   <https://root.cern/releases/release-64004/>. It sets up everything for you.
4. Start ROOT by double-clicking its icon, or from the
   **x64 Native Tools Command Prompt for VS** (in the Start menu):

```text
cd %USERPROFILE%
root
```

> **Avoid spaces in folder names on Windows.** Do not unpack or install ROOT in a
> folder whose name contains a space (like `C:\Program Files`), and keep your own
> macros in folders without spaces too.

## 1.7 Docker

If you know Docker, the ROOT team provides ready-made images:

```bash
docker run -it rootproject/root:latest
```

To work on files in your current folder, mount it into the container:

```bash
docker run -it -v "$PWD":/work -w /work rootproject/root:latest
```

Graphics windows do not open from Docker without extra setup, but the examples in this
manual all save their plots as PNG files, so you can follow the tutorial this way.

## 1.8 If you have a CERN account

- **lxplus**: log in with `ssh -X <username>@lxplus.cern.ch` and type `root`. The
  latest stable ROOT is installed there.
- **SWAN** (<https://swan.cern.ch>): Jupyter notebooks with ROOT, in your web browser.
  Nothing to install.

## 1.9 Test your installation

Download the tutorial files and go to the installation-check folder. (On conda,
activate the environment first.)

```bash
conda activate root-tutorial          # only if you use conda
cd root-tutorial/macros/ch01_install
```

**Test 1 — ROOT with C++.** The file `check_root.C`:

{{code: macros/ch01_install/check_root.C}}

Run it:

```bash
root -l check_root.C
```

A window with a histogram opens, and the terminal prints:

{{output: macros/ch01_install/check_root}}

![The plot made by `check_root.C`. If you see this, ROOT works.](figures/check_root.png)

The ROOT prompt `root [1]` is now waiting for you. Type `.q` and press Enter to quit.

**Test 2 — ROOT with Python (PyROOT).** The file `check_pyroot.py`:

{{code: macros/ch01_install/check_pyroot.py}}

Run it:

```bash
python check_pyroot.py
```

You should see something like:

{{output: macros/ch01_install/check_pyroot}}

If both tests print `Everything is fine`, you are ready. Well done!

## 1.10 A text editor

You write macros in a text editor (not in Word!). Any of these is fine:

- **Visual Studio Code** (<https://code.visualstudio.com>), free, for all systems.
  Install the *C/C++* and *Python* extensions. With WSL, install the *WSL* extension
  too, then type `code .` in the Ubuntu window to open the current folder.
- Any editor you already like: gedit, Kate, nano, vim, emacs, Sublime Text, ...

## 1.11 Installation problems

| What you see | What it means and what to do |
|---|---|
| `root: command not found` | ROOT is not set up in this terminal. With conda: `conda activate root-tutorial`. With a binary: `source root/bin/thisroot.sh`. |
| `conda: command not found` | Close the terminal and open a new one after installing Miniforge. If it still fails, run the installer again and answer `yes` to "initialise conda". |
| `ModuleNotFoundError: No module named 'ROOT'` | You run a Python that does not know about ROOT. With conda: activate the environment. With Homebrew: use the Python of Section 1.5. |
| `ROOT was built for Python 3.14.7, but you are running Python 3.9.6` | Same as above: use the Python version that ROOT was built with. |
| The ROOT window never appears | On WSL see Section 1.6; over `ssh` use `ssh -X`. The PNG files are still made, so you can continue. |
| conda takes forever to "solve" | Make sure you ran `conda config --set channel_priority strict` and use Miniforge. |

If you are stuck, write to the organisers **before** the tutorial with: your operating
system, the method you used, the exact command you typed, and the full error message.
