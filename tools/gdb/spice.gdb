# Installed by the spice package into GDB's system-wide init directory (see
# `--with-system-gdbinit-dir` in `info gdb`). Auto-loads the pretty printers for the
# std/data/ containers on distros whose GDB build honors that directory; a no-op
# elsewhere. Manual loading instructions: docs/docs/how-to/debugging-with-gdb.md.
source /usr/lib/spice/gdb/spice_printers.py
