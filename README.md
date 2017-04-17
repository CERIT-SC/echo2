
# ECHO 2
Echo 2 is a tool for searching and correcting sequencing errors.

Echo 2 is a reimplementation of [ECHO short-read error corrector][orig echo].  
This new implementation was published in:
[ECHO NGS Error Correction Reimplemented][new echo].

[orig echo]: https://www.ncbi.nlm.nih.gov/pubmed/21482625
[new echo]: http://www.memics.cz/2015/download/memics15-proceedings.pdf#page=68

## System and hardware requirements
Echo is a UNIX-based application. It is built for parallel processing.
So a system with multiple cores is recommended. Let's use some large data
as an example: A file containing 300 million sequences of the length of 100
(60 GB [fastq] file).
Echo can utilize **up to 24 cores** for our example. The file should
be processed within a day.

Echo has large requirements for memory. Our example might take up about
**420 GB of RAM**. If your system does not have enough memory and
starts using swap, Echo will continue processing, *but* there will be a
rapid decrease of performance.

In our performance tests, the best speed was achieved using 16 cores
for 2.5 GB file size, and 24 cores for 14 GB or larger.

## Compilation and installation
GCC 4.8.1 or newer is required. You can alternatively use a different
compiler that supports C++11 standard.

**Installing Echo**:

1. On a command line, `cd` to the repository.
2. Run `make` to compile the program.
3. Run `make install` to install Echo to standard path (`/usr/local/bin`).
   You may need admin privileges for this step. In that case run
   `sudo make install` and enter admin password.
4. After step 3, Echo should be available using command `echo2`. You can
   test this by running `echo2 --version`. Examples in section "*How to use*"
   expect, that Echo has been installed using these steps.

Instead of step 3, you can alternatively run `make here` to move the program
to the main repository folder.

**Uninstalling Echo**:

- `make clean` will remove executable in repository
- `make uninstall` will uninstall the program from standard path. This may
  also need admin privileges. If so, run `sudo make uninstall`.

## How to use
Echo is a command line tool. You can get the description
of all options by running `echo2 --help`.

The simplest way to run Echo is to provide the file with sequences:

``` bash
echo2 file_to_be_corrected.fastq
```

But the recommended way is to specify the file *and* number of cores to use
(option `-n` or `--ncpu`). If you do not specify the number of cores, the
program will use only one.

``` bash
echo2 -n 8 file.fastq
```

### Input files
Echo supports 2 input file formats. If the file has the `.fastq` or `.fq`
extension, it is interpreted as a [FASTQ] file. If the file has a
different extension or doesn't have one at all, it is interpreted as a simple
text file where each sequence is on a separate line.

[FASTQ]: https://en.wikipedia.org/wiki/FASTQ_format

You can specify more than one input file. In this case, the application
will process all of them one by one:

``` bash
echo2 -n 8 first_file.fastq second.txt third.fq
```

### Output files
The output file name, where the corrected result is stored, can be specified
using the `-o` or `--output` option. If you specify multiple input files,
then the output files will be assigned to input files in the same order as
specified on the command line:

``` bash
echo2 file1 file2 file3 -o output1 -o output2
```

In the example above, the result from `file1` will be saved to `output1`, and
the result from `file2` will be saved to `output2`. The result for `file3`
will be saved to `file3-corrected` (as described below).

The name for the output file doesn't have to be specified. In this case,
Echo will save the result to a file with the same name, but adds the
suffix `-corrected`. This means that the result from file
`sequences.fastq` will be saved to `sequences.fastq-corrected`.

Output is always saved as [FASTQ].

### Log
The program prints log information to a standard output during processing.
If you want to preserve the log, you can specify the log file using
option `-l` or `--log`:

``` bash
echo2 -n 8 file.fq -l log.txt
```

### Hash table
A huge hash table is used during processing. Its size can be specified
using option `--hash_size`. This size can greatly influence performance and
memory requirements.

If the size of the hash table is not specified, an appropriate size will be
determined automatically (recommended). Using a larger table than estimated
would not necessarily improve performance, and can even degrade it.
A smaller hash table can be useful to save memory, but this will increase
runtime.

Hash table size can range from *20* to *32*. Value specifies
size exponent. Size 20 means, that the hash table can discern 2^20
different hashes.

``` bash
echo2 -n 8 file.fq --hash_size 25
```

### Other options
Echo specifies other options. You can list them by running `echo2 --help`.
You do not have to use them. The program will set them automatically.

Example of running a program using multiple options:
``` bash
echo2 sequences.fastq -o result.fastq -n 8 --hash_size 28 -l log.txt --kmer 20
```

## Author
This implementation was created by **Miloš Šimek** at
[Masaryk University](https://www.muni.cz/en).  
You can email me at <simek@ics.muni.cz>.

## Acknowledgments
I would like to thank Aleš Křenek who made this project possible.

## Copyright
This project is licensed under Creative Commons 4.0 International
license (CC BY 4.0). Summary of the license can be found
[here][lshort], the license itself can be found [here][llong].

[lshort]: https://creativecommons.org/licenses/by/4.0/
[llong]:  https://creativecommons.org/licenses/by/4.0/legalcode
