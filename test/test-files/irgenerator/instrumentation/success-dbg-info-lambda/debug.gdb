# Do not print non-reproducible information
set print address off
set print thread-events off
set print inferior-events off

# Preparation
break source.spice:4
break source.spice:12
break source.spice:19
break source.spice:29
break source.spice:37
run

# Runtime
info locals
print counter
print offset
continue
info args
print value
print fct.captureSize
continue
info args
print x
continue
print x
continue
print counter
continue
print *counterPtr
continue

# Quit
quit
