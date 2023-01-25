# Imports design document

We aim to get rid of the explicit scope, that programmers have to write when accessing external symbols, similar to
`using` in C++. This can be beneficial for programmer productivity, code readability and might reduce the complexity
in the compiler.

This design works for functions, procedures, structs, interfaces and global variables.

##  Design

The `Import Collector` will check every source file for potential dependencies. If an import statement was found, the
Import Collector will create the dependency in the current source file and triggers the compile steps 1-7 for the
newly imported source file. <br>
After that, the Import Collector registers all symbols from the global scope of the imported source file, to the
`ExternalSymbolRegistry` of the current source file. If there is already a symbol with that name in the registry,
this symbol gets removed and 

**Example:** <br>
Source file A imports source file B as `sourceB`. B contains a struct with the name `TestStruct`. Formerly, you would
have written `sourceB::TestStruct` to access the struct from source file A. Now you only need to write `TestStruct`.

Registry:

| Name                | Pointer to SymbolTableEntry | Pointer to scope of symbol | Import entry        | Predecessor name     |
|---------------------|-----------------------------|----------------------------|---------------------|----------------------|
| sourceB::TestStruct | Pointer to struct entry     | Ptr to struct body scope   | Ptr to import entry |                      |
| TestStruct          | Pointer to struct entry     | Ptr to struct body scope   | Ptr to import entry | sourceB::TestStruct  |

However, if A also imports source file C, which also exposes a struct with the name `TestStruct`, there would be an
ambiguity between `sourceB::TestStruct` and `sourceC::TestStruct` when you just write `TestStruct`. Therefore, when
such a collision is detected, the compiler only makes available `sourceB::TestStruct` and `sourceC::TestStruct` as is.
Accessing `TestStruct` without a scope identifier, leads to a compile error.

Registry:

| Name                | Pointer to SymbolTableEntry | Pointer to scope of symbol   | Import entry        | Predecessor name |
|---------------------|-----------------------------|------------------------------|---------------------|------------------|
| sourceB::TestStruct | Pointer to struct entry     | Pointer to struct body scope | Ptr to import entry |                  |
| sourceC::TestStruct | Pointer to struct entry     | Pointer to struct body scope | Ptr to import entry |                  |

When source file C now imports source file D, which expose another struct `AnotherStruct` the registry looks like so:

| Name                            | Pointer to SymbolTableEntry | Pointer to scope of symbol   | Import entry        | Predecessor name                |
|---------------------------------|-----------------------------|------------------------------|---------------------|---------------------------------|
| sourceB::TestStruct             | Pointer to struct entry     | Pointer to struct body scope | Ptr to import entry |                                 |
| sourceC::TestStruct             | Pointer to struct entry     | Pointer to struct body scope | Ptr to import entry |                                 |
| sourceC::sourceD::AnotherStruct | Pointer to struct entry     | Pointer to struct body scope | Ptr to import entry |                                 |
| sourceD::AnotherStruct          | Pointer to struct entry     | Pointer to struct body scope | Ptr to import entry | sourceC::sourceD::AnotherStruct |
| AnotherStruct                   | Pointer to struct entry     | Pointer to struct body scope | Ptr to import entry | sourceD::AnotherStruct          |