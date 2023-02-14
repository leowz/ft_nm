# ft_nm
My implementation of 42 NM. Works on x86, x64, object files and dynamic lib .so files.
Similar to the GNU system nm command with differences.

## Background knowledage

below are all informations I found useful in developing this project.

## Tools to use to develope
- readelf: display information about ELF files, useful to check headers and symtabs.
- objdump: display information from object files, similar to readelf. 
- hexdump: see the dump of a binary file, useful for check real value of a binary.

## GNU NM
GNU nm lists the symbols from object files objfile....  If no
object files are listed as arguments, nm assumes the file a.out.
For each symbol, nm shows:

- The symbol value, in the radix selected by options (see below), or hexadecimal by default.

- The symbol type.  At least the following types are used; others are, as well, depending on the object file format.  If lowercase, the symbol is usually local; if uppercase, the symbol is global (external).  There are however a few lowercase symbols that are shown for special global symbols ("u", "v" and "w").

- The symbol name.  If a symbol has version information associated with it, then the version information is displayed as well.  

## ELF
format of Executable and Linking Format (ELF) files

- below from: man elf (https://man7.org/linux/man-pages/man5/elf.5.html)
- an example of elf.h (https://codebrowser.dev/linux/include/elf.h.html)

Compilers produce and linkers and loaders operate on object files. The have specific formats like: assembler output (a.out). Mach-O, or ELF.

- executable (a.out)
- core. Virtual address space and register state of a process (a.out.core)
- Relocatable. Can be linked together with others to produce a shared library or an executable (foo.o)
- Share Object files. Position independent code; used by dynamic linker to create a process image (libfoo.so)

![ELF format](https://linuxhint.com/wp-content/uploads/2019/09/1-7.png)

The program header is defining segments (in the address space of a process running executable) projected in virtual memory (The executable point of view).
The sections header is defining sections (the linkable point of view for ld etc). Each section belongs to a segment. The ELF file header tells where program header table and section header table are.

### ELF header (Ehdr)
The ELF header is described by the type Elf32_Ehdr or Elf64_Ehdr:

```
    #define EI_NIDENT 16

    typedef struct {
        unsigned char e_ident[EI_NIDENT];
        uint16_t      e_type;
        uint16_t      e_machine;
        uint32_t      e_version;
        ElfN_Addr     e_entry;
        ElfN_Off      e_phoff;
        ElfN_Off      e_shoff;
        uint32_t      e_flags;
        uint16_t      e_ehsize;  //elf header size
        uint16_t      e_phentsize; // program header entry size
        uint16_t      e_phnum;     //program header number
        uint16_t      e_shentsize; //section header entry size
        uint16_t      e_shnum;     //section header number
        uint16_t      e_shstrndx;  //section header string table index, in section header table
    } ElfN_Ehdr;
```

### ps:
```
    e_shstrndx
            This member holds the section header table index of the
            entry associated with the section name string table.  If
            the file has no section name string table, this member
            holds the value SHN_UNDEF.
            If the index of section name string table section is
            larger than or equal to SHN_LORESERVE (0xff00), this
            member holds SHN_XINDEX (0xffff) and the real index of the
            section name string table section is held in the sh_link
            member of the initial entry in section header table.
            Otherwise, the sh_link member of the initial entry in
            section header table contains the value zero.
```

### Program header (Phdr)

An executable or shared object file's program header table is an
array of structures, each describing a segment or other
information the system needs to prepare the program for
execution.  An object file segment contains one or more sections.
Program headers are meaningful only for executable and shared
object files.  A file specifies its own program header size with
the ELF header's e_phentsize and e_phnum members.  The ELF
program header is described by the type Elf32_Phdr or Elf64_Phdr
depending on the architecture.

### Section header (Shdr)
A file's section header table lets one locate all the file's
sections.  The section header table is an array of Elf32_Shdr or
Elf64_Shdr structures.  The ELF header's e_shoff member gives the
byte offset from the beginning of the file to the section header
table.  e_shnum holds the number of entries the section header
table contains.  e_shentsize holds the size in bytes of each
entry.

A section header table index is a subscript into this array.
Some section header table indices are reserved: the initial entry
and the indices between SHN_LORESERVE and SHN_HIRESERVE.  The
initial entry is used in ELF extensions for e_phnum, e_shnum, and
e_shstrndx; in other cases, each field in the initial entry is
set to zero.  An object file does not have sections for these
special indices:

```
typedef struct {
               uint32_t   sh_name;  // index to the .shstrtab section
               uint32_t   sh_type;
               uint32_t   sh_flags;
               Elf32_Addr sh_addr;
               Elf32_Off  sh_offset;
               uint32_t   sh_size;
               uint32_t   sh_link;
               uint32_t   sh_info;
               uint32_t   sh_addralign;
               uint32_t   sh_entsize;  // size of each entry, if it hold a table of fixed size entry like symtab
           } Elf32_Shdr;
```

#### ps:
```
  sh_name
              This member specifies the name of the section.  Its value
              is an index into the section header string table section,
              giving the location of a null-terminated string.

  sh_type
              This member categorizes the section's contents and
              semantics.
              SHT_SYMTAB
                     This section holds a symbol table.  Typically,
                     SHT_SYMTAB provides symbols for link editing,
                     though it may also be used for dynamic linking.  As
                     a complete symbol table, it may contain many
                     symbols unnecessary for dynamic linking.  An object
                     file can also contain a SHT_DYNSYM section.

              SHT_STRTAB
                     This section holds a string table.  An object file
                     may have multiple string table sections.

some sections:
       .shstrtab  （names for section table）
              This section holds section names.  This section is of type
              SHT_STRTAB.  No attribute types are used.

       .strtab     （names for symbol table）
              This section holds strings, most commonly the strings that
              represent the names associated with symbol table entries.
              If the file has a loadable segment that includes the
              symbol string table, the section's attributes will include
              the SHF_ALLOC bit.  Otherwise, the bit will be off.  This
              section is of type SHT_STRTAB.
       .symtab
              This section holds a symbol table.  If the file has a
              loadable segment that includes the symbol table, the
              section's attributes will include the SHF_ALLOC bit.
              Otherwise, the bit will be off.  This section is of type
              SHT_SYMTAB.
```


### String and symbol tables
(Symbols are a symbolic reference to some type of data or code such as a global variable or function)

String table sections hold null-terminated character sequences,
commonly called strings.  The object file uses these strings to
represent symbol and section names.  One references a string as
an index into the string table section.  The first byte, which is
index zero, is defined to hold a null byte ('\0').  Similarly, a
string table's last byte is defined to hold a null byte, ensuring
null termination for all strings.

An object file's symbol table holds information needed to locate
and relocate a program's symbolic definitions and references.  A
symbol table index is a subscript into this array.

```
           typedef struct {
               uint32_t      st_name;  // index to the .strtab section
               Elf32_Addr    st_value;  // value means different things, in different scenario
               uint32_t      st_size;
               unsigned char st_info;   // bind and type
               unsigned char st_other;  // visibility
               uint16_t      st_shndx; // index to the section, section header index
           } Elf32_Sym;
```

### Symbol TYPE: 
- A/a: The symbol's value is absolute. SHN_ABS
- B/a: The symbol is in the BSS data section. .bss
- C/c: The symbol is common.  Common symbols are uninitialized data. SHN_COMMON
- D/d: The symbol is in the initialized data section. .data
- G/g: The symbol is in an initialized data section for small objects.
- i: For PE format files this indicates that the symbol is in a section specific to the implementation of DLLs..
- I: The symbol is an indirect reference to another symbol.
- N: The symbol is a debugging symbol.
- n: The symbol is in the read-only data section.
- p: The symbol is in a stack unwind section.
- R/r: The symbol is in a read only data section.
- S/s: The symbol is in an uninitialized or zero-initialized data section for small objects.
- T/t: The symbol is in the text (code) section.
- U: The symbol is undefined.
- u: The symbol is a unique global symbol.
- V/v: The symbol is a weak object.  When a weak defined symbol is linked with a normal defined symbol, the normal defined symbol is used with no error. 
- W/w: The symbol is a weak symbol that has not been specifically tagged as a weak object symbol. 

### Symbol Values

Symbol table entries for different object file types have slightly different interpretations for the st_value 

member.

In relocatable files, st_value holds alignment constraints for a symbol whose section index is SHN_COMMON.

In relocatable files, st_value holds a section offset for a defined symbol. st_value is an offset from

the beginning of the section that st_shndx identifies.

In executable and shared object files, st_value holds a virtual address. To make these files' symbols

more useful for the runtime linker, the section offset (file interpretation) gives way to a virtual

address (memory interpretation) for which the section number is irrelevant.

Although the symbol table values have similar meanings for different object files, the data allow efficient

access by the appropriate programs.

## Endian
When reading and parsing directly the binary, endian must be taken into consideration. Only 8 bits data or a Byte length data can be spared.

Big-endian: Stores the Most Significant Byte (MSB) of the data word in the smallest address in memory.

Little-endian: Stores the Least Significant Byte (LSB) of the data word in the smallest address in memory.

If file endian is not the same endian with the machine, a endian reverse must be applied to the data read from the file to make sure the data is correct. The key point is the all data with a size over 8bits must perform this reverse(because endian only affect data over 8bits). Macros imported from machine is the same endian with the machine so when doing macro comparision, the endian need to take into consideration. 

## PS
The type of my ft_nm does not correspond exactly like GNU NM. And the sorting also. Because I failed to find the exact reference to the type and sorting of NM. I guess it depends on the implementation.