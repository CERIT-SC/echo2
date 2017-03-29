
# ECHO 2 Error Corrector

This tool finds and corrects sequencing errors in DNA data.
DNA sequencers introduce errors in sequenced data. This tool
aims to reduce as many of them as possible.

Echo 2 is reimplementation of [ECHO Error Corrector][orig echo].  
This new implementation was published in:
[ECHO NGS Error Correction Reimplemented][new echo].

[orig echo]: https://www.ncbi.nlm.nih.gov/pubmed/21482625
[new echo]: http://www.memics.cz/2015/download/memics15-proceedings.pdf#page=68

## System and hardware requirements
Echo is UNIX based application. It is build for parallel processing.
So system with multiple cores is recommended. Let's take an example
of a large data: File containing 300 million sequences of length 100
(60 GB [fastq] file).
Echo can utilize up to **24 cores** for our example. The file should
be processed within a day.

Echo has large requirements for memory. Our example might take about **420 GB of RAM**. If you're system doesn't have enough memory and
starts using swapping, Echo will continue processing the data *but*
there will be rapid decrease of performance.

In our performance tests, 2,5 GB fastq file was processed fastest
using 16 cores and 14 GB file was processed fastest using 24 cores.

## Compilation instructions
GCC 4.8.1 or newer is required. You can alternatively use different
compiler that supports C++11 standard.

There are 2 ways to compile Echo:
- You can simply run `compile.sh` script (this will be replaced by
  makefile in the future). The script will compile Echo and
  save it to `Build` folder.
- Alternatively, you can do manual compilation – go to `Echo` folder
  where the source files are. And run this command:
  `g++ -std=c++11 -pthread -O3 -m64 *.cpp -o Echo`

The best runtime is achieved by compiling with O3 optimization.

## How to use
Program is being run through command line. You can get description
of all options by running `./Echo --help`.

Simplest way, how to run Echo is to provide file with sequences:

``` bash
./Echo file_to_be_corrected.fastq
```

Recommended way is to specify not only the file, but also number of
cores to use (option `-n` or `--ncpu`):

``` bash
./Echo -n 8 file.fastq
```

### Input files
Echo supports 2 input file formats. If the file has `.fastq` or `.fq`
extension, it is interpreted as [FASTQ] file. If the file does have
different extension or doesn't have one, it is interpreted as simple
text file where each sequence is on separate line.

[FASTQ]: https://en.wikipedia.org/wiki/FASTQ_format

You can specify more than one imput file. In this case application
will process all of them one by one:

``` bash
./Echo -n 8 first_file.fastq second.txt third.fq
```

### Output files
Output file name (where corrected result is stored) can be specified
using `-o` or `--output` option. In case of multiple input files -
output files will be assigned to input files in the same order as
specified on command line:

``` bash
./Echo file1 file2 file3 -o output1 -o output2
```

In the example above, result from `file1` will be saved to `output1` and result from `file2` will be saved to `output2`. Result for `file3`
will be saved to `file3-corrected` (as described below).

Names for output files doesn't have to be specified. In this case
Echo will save the result to a file with the same name, but adds
suffix `-corrected`. Which means that result from file
`sequences.fastq` will be saved to `sequences.fastq-corrected`.

Output is always saved as [FASTQ].

Program prints log information to standard output during processing.
If you want to preserve the log, you can specify the log file using
option `-l` or `--log`:

``` bash
./Echo -n 8 file.fq -l log.txt
```

### Hash table
Huge hash table is used during processing. It's size can be specified
using option `--hash_size`. Size of hash table can greatly influence
the performance and the memory requirements.

If you don't specify the size of the hash table, program will determine
appropriate size of its own. It is recommended to leave it that way.
Making hash table larger (than what would program decide) wouldn't
necessarily improve performance. There are cases where performance
can even degrade. Making hash table smaller can be useful if you want
to save memory but it will cause longer runtime.

Hash table size can range from *20* to *32*. Value specifies
size exponent. Size 20 means, that the hash can discern 2^20
different hashes.

``` bash
./Echo -n 8 file.fq --hash_size 25
```

### Other options
Echo specifies other options. You can list them by running
`./Echo --help`. You don't have to use them. Program will set
them automatically.

Example of running the program:
``` bash
./Echo sequences.fastq -o result.fastq -n 8 --hash_size 28 -l log.txt --kmer 20
```

## Author
This implementation was created by Miloš Šimek at
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
