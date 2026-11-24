# Contributing

Agents and people follow the same steps: the checklist in [AGENTS.md](AGENTS.md), the enforced rules in
[RULES.md](RULES.md) and the review rules in [docs/anti-patterns.md](docs/anti-patterns.md). This page adds
only what a person working by hand needs to know.

- **Clone on Windows** with `git clone -c core.longpaths=true -c core.autocrlf=input <URL>`; some Zero Hour
  paths exceed 260 characters, and the repository stores LF.
- **Pick work** from the [worklist](worklist/README.md), which also explains its columns.
- **Read the target** with any disassembler that opens a 32-bit Windows program (`objdump -d -M intel`).
  Cite the address, size and what the code does; never paste dumps, executables or game assets.
- **Commit with** a public address or your GitHub no-reply address.
- **Attribution:** when you copy or adapt text from the Electronic Arts release, put the line from
  [NOTICE](NOTICE) first in the file. Using a Zero Hour identifier as a name does not by itself require it.
- **Ask** in an issue whose title starts with `Evidence:`, `Path:` or `Ledger:`. The ledger and generated
  annotations are explained in [docs/ledger-and-checks.md](docs/ledger-and-checks.md).
- **Checks:** what each status means is in [ci/README.md](ci/README.md). Security reports go through
  [SECURITY.md](SECURITY.md).
