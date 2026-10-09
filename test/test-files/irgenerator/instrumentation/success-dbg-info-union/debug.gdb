# Do not print non-reproducible information
set print address off
set print thread-events off
set print inferior-events off

# Preparation
break source.spice:28
break source.spice:44
break source.spice:54
run

# Runtime
print value
print value.vec
continue
print value
print intBox
print doubleBox.value
continue
print holder.box
print holder.box.fallback
print *holder.value.next
continue

# Quit
quit
