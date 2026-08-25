# Do not print non-reproducible information
set print address off
set print thread-events off
set print inferior-events off

# Load the Spice pretty printers, resolving the script relative to the repo root
python
import os
repo_root = os.path.dirname(os.environ["SPICE_STD_DIR"])
gdb.execute("source " + os.path.join(repo_root, "tools", "gdb", "spice_printers.py"))
end

# Preparation
break source.spice:127
run

# Runtime
print vec
print stack
print queue
print deque
print pq
print list
print dList
print table
print tree
print bTree
print map
print set
print uMap
print uSet
print pair
print triple
print some
print none
print bits
print trie
print graph
print nested
print myErr
print okResult
print errResult
continue

# Quit
quit
