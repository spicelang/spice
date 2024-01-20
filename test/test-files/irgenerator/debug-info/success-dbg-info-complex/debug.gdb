# Do not print non-reproducible information
set print address off
set print thread-events off
set print inferior-events off

# Preparation
break source.spice:42
run

# Runtime
info locals
print vi
print vi.size
continue

# Quit
quit