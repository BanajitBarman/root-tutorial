## Files

| Path | What it is |
|------|------------|
| `ROOT_Tutorial_Manual.pdf` | the finished manual |
| `ROOT_Tutorial_Manual.md` | the same manual as one Markdown file (uses the images in `figures/`) |
| `chapters/*.md` | **the source of the manual: edit these** |
| `macros/chNN_*/` | the examples, one folder per chapter |
| `solutions/chNN/` | exercise solutions |
| `figures/` | every plot in the manual, made by the examples |
| `outputs/` | the saved terminal output of every example, pasted into the manual |
| `tools/` | scripts that run the examples and build the manual |

## Editing the manual

Edit the files in `chapters/`, not `ROOT_Tutorial_Manual.md`, which is regenerated
on every build. A chapter can include code and output with these marker lines:

```text
{{code: macros/ch03_histograms/hist_basic.C}}     the file, as a code block
{{output: macros/ch03_histograms/hist_basic}}     its saved output
```

The title page (author, date) is at the top of `chapters/00_front.md`.

## Rebuilding

```bash
python3 tools/run_examples.py        # runs every example: new outputs/ and figures/ (about 2 minutes)
python3 tools/build_manual.py --pdf  # makes ROOT_Tutorial_Manual.md and .pdf
tools/make_participant_zip.sh        # makes root-tutorial-participants.zip
```

These need ROOT, pandoc and a TeX installation with xelatex. The Python used for
the PyROOT examples is set at the top of `tools/run_examples.py`. It must be the
Python that ROOT was built with (check with `root-config --python-version`).
`run_examples.py ch05` runs only the examples whose path contains `ch05`.

`macros/ch10_project/cms_open_data.py` reads real CMS data over the internet and
takes about 2.5 minutes, so `run_examples.py` does not run it. Run it by hand if
you need a new copy of its plot.

## Notes for the tutorial days

- The mini-project's "hidden particle" is the J/ψ (with a small ψ(2S)). The
  answer is in Appendix A.10 and in `make_dimuon_toy.C`, so don't point
  participants there before they finish.
- Extra task D, and the CMS bonus, need internet access at the venue.
- The manual was tested with ROOT 6.40.04 on macOS (Homebrew). Every example
  uses standard ROOT 6 features and should also run on Linux, WSL and conda.
