# DDL Manager
A simple command line tool to manage your DDLs (Deadlines).
## Build
``` bash
    cmake -B build && cmake --build build
```
## Useage
Basic command to add a DDL:
``` bash
    ./build/ddl add "ddl title" --due "yyyy-mm-dd" <--tag "tag title">
```
Explanation of the command:
The context in the `<>` means that they are optional

`""` is required for ddl title and tag title with spaces.
For the titles without spaces, `""` is not required.

If the command does not have `--tag` and tag title, the default tag will be `Undetermined`.
`yyyy-mm-dd` is the due date of the DDL, format: year-month-day
`tag title` is the tag of the DDL, for example: `math`, `cs`, `project`, etc.

For example, to add a DDL with title `finish homework`, due date `2024-06-30`, and tag `math`, the command is:
``` bash
    ./build/ddl add "finish homework" --due "2024-06-30" --tag "math"
```
and to add a DDL with title `finish homework`, due date `2024-06-30`, and default tag `Undetermined`, the command is:
``` bash
    ./build/ddl add "finish homework" --due "2024-06-30"
```
After the DDL is added, the program will print the DDL information, with the format:
```
    <id>\t<title>\t<due date>\t<tag>\t<status>
```
where `\t` is Tab, `<id>` is the id of the DDL, `<title>` is the title of the DDL, `<due date>` is the due date of the DDL, `<tag>` is the tag of the DDL, and `<status>` is the status of the DDL, which can be `undone` or `done`.
For example, the output of the first command is:
```
    1	finish homework	2024-06-30	math	undone
```

All the DDLs will be saved in a file named `data.tsv` in the directory `~/.ddl/` in Linux or `C:\Users\<username>\.ddl\` in Windows, where `<username>` is your username in the system. (This haven't been finished yet, but will be finished in the future)