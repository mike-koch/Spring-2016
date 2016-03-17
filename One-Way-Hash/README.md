# One-Way-Hash

One-Way-Hash calculates the SHA-256 checksum for a file or list of files.

### Execution

- Make the project by entering `make` into the command line. This project requires g++ and C++11 to compile.
- Run the application by running `./One-Way-Hash <args>`. `<args>` is a list of files you wish to calculate the SHA-256 checksum of, separated with a single space. This application takes advantage of `pthreads` to quickly hash multiple files at the same time.
