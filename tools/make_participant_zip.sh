#!/bin/bash
# Make root-tutorial-participants.zip: the manual, the examples and the solutions,
# without any generated files (plots, ROOT files, compiled libraries).
set -e
cd "$(dirname "$0")/.."
STAGE=$(mktemp -d)
mkdir -p "$STAGE/root-tutorial"
cp ROOT_Tutorial_Manual.pdf "$STAGE/root-tutorial/"
rsync -a --exclude '*.png' --exclude '*.pdf' --exclude '*.root' --exclude '*.so' \
      --exclude '*.d' --exclude '*.pcm' --exclude '__pycache__' --exclude '.DS_Store' \
      macros solutions "$STAGE/root-tutorial/"
rm -f root-tutorial-participants.zip
(cd "$STAGE" && zip -qr - root-tutorial) > root-tutorial-participants.zip
rm -rf "$STAGE"
echo "Wrote root-tutorial-participants.zip"
