#!/usr/bin/env bash
set -Eeuo pipefail

# Resolve to the repository root (directory of this script)
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
BIN="${SCRIPT_DIR}/bin/Debug-linux-x86_64/TeapotApp/TeapotApp"

if [[ ! -x "$BIN" ]]; then
  if [[ -f "$BIN" ]]; then
    chmod +x "$BIN"
  else
    echo "Binary not found at: $BIN" >&2
    echo "Did you run 'premake5 gmake2 && make'?" >&2
    exit 1
  fi
fi

exec "$BIN"