# debugger-project
My debugger is a GDB-like debugger written in C for Linux x86_64, supporting breakpoints, memory inspection, ELF parsing, register inspection, live process attachment, ASLR handling and soon Rewind feature.

---

# Features
- **Execution Control:** run, continue (c), step into (si), step over (ni)
- **Breakpoints:** Software (INT3) & hardware, set by address or symbol, delete and manage
- **Backtrace:** Stack unwinding via RBP frame-pointer chain (bt)
- **ELF & Symbols:** Parse .symtab / .dynsym, resolve addresses to functions
- **Memory & Registers:** Inspect process memory and x86_64 registers
- **Attach:** Debug already running processes
- **ASLR Support:** Dynamically resolve memory addresses by parsing /proc/<pid>/maps
- **Disassembly:** Function-level disassembly or arbitrary memory ranges (e.g., x/10i $rip)
- **Soon Reverse Debugging:** Rewind execution using Copy-On-Write snapshots
