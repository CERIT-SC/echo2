//
//  Options.cpp
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 09/09/13.
//

#include "Options.hpp"

#define VALUE argv[i+1]
#define HELP_SPACE 26
#define LAUNCH_LOG_SPACE 25
#define SUCC_TEST if (i+1 == argc) { errInput("Last flag requires value."); return; }

Options::Options(int argc, const char* argv[]) {
    //initial values
    numOfCPU = 1;
    kmerLen = 0;
    mine = maxe = minh = maxh = 0;
    h_rate = 0;
    sampleSize = 0;
    hashSize = 0;
    
    this->argv = argv;
    this->argc = argc;
    
    state = ST_OK;
    
    loadOptions();
}

void Options::loadOptions() {
    if (argc < 2) {
        state = ST_INP_ERROR;
        errInput("Too few arguments.");
        return;
    }
    
    for (int i=1; i<argc; i+=2) {
        
        if (testFor(i, "--help")) {
            state = ST_HELP;
            return;
        }
        
        //option --
        if (testFor(i, "--")) {
            noFlagLoading(i+1);
            return;
        }
        
        //get version
        if (testFor(i, "--version")) {
            state = ST_VERSION;
            return;
        }
        
        //options
        if (argv[i][0] != '-') { inputFiles.push_back(argv[i]); i-=1; continue; }
        if (testFor(i, "-o", "--output")){ SUCC_TEST; outputFiles.push_back(VALUE); continue; }
        if (testFor(i, "-l", "--log"))   { SUCC_TEST; logFile = VALUE;    continue; }
        
        if (testFor(i, "-n", "--ncpu")) {
            SUCC_TEST;
            numOfCPU = toInt(VALUE);
            if (numOfCPU < 1) errInput("Incorrect value for cpu count.");
            continue;
        }
        
        if (testFor(i, "-k", "--kmer")) {
            SUCC_TEST;
            kmerLen = toInt(VALUE);
            if (kmerLen < 0) errInput("Incorrect value for k-mer length.");
            continue;
        }
        
        if (testFor(i, "-e", "--min_error_tolerance")) {
            SUCC_TEST;
            mine = toDouble(VALUE);
            if (mine < 0) errInput("Incorrect value for minimum error tolerance.");
            continue;
        }
        
        if (testFor(i, "-E", "--max_error_tolerance")) {
            SUCC_TEST;
            maxe = toDouble(VALUE);
            if (maxe < 0) errInput("Incorrect value for maximum error tolerance.");
            continue;
        }
        
        if (testFor(i, "-h", "--min_min_overlap")) {
            SUCC_TEST;
            minh = toInt(VALUE);
            if (minh < 0) errInput("Incorrect value for minimum minimum overlap length.");
            continue;
        }
        
        if (testFor(i, "-H", "--max_min_overlap")) {
            SUCC_TEST;
            maxh = toInt(VALUE);
            if (maxh < 0) errInput("Incorrect value for maximum minimum overlap length.");
            continue;
        }
        
        if (testFor(i, "-r", "--heterozygous_rate")) {
            SUCC_TEST;
            h_rate = toDouble(VALUE);
            if (h_rate < 0) errInput("Incorrect heterozygous rate value.");
            continue;
        }
        
        if (testFor(i, "-s", "--sample_size")) {
            SUCC_TEST;
            sampleSize = toInt(VALUE);
            if (sampleSize < 0) errInput("Incorrect sample value.");
            continue;
        }
        
        if (testFor(i, "--hash_size")) {
            SUCC_TEST;
            hashSize = toInt(VALUE);
            if (hashSize < HASH_TABLE_SIZE_EXP_MIN) errInput("Incorrect hash size value.");
            if (hashSize > HASH_TABLE_SIZE_EXP_MAX) errInput("Incorrect hash size value.");
            continue;
        }
        
        string message = (string("Unrecognised argument: ") + argv[i]);
        errInput(message.c_str());
    }
    
    testFiles();
}

void Options::noFlagLoading(int startIndex) {
    for (int i = startIndex; i<argc; i++) {
        inputFiles.push_back(argv[i]);
    }
}

void Options::printHelp() const {
    cout << endl;
    cout << APPLICATION_NAME_STRING << " - version " << APPLICATION_VERSION_STRING << endl;
    string usage = string("Usage: ") + argv[0] + " [options] [file ...]";
    P(usage.c_str());
    
    cout << endl;
    P("Options:");
    P("Only input file(s) are required.");
    P("Input file has no flag.");
    P("Output files are assigned to input files in same order.");
    P("If there is no output file name set for input file, then output file name will be");
    P(string("input file name extended by \"") + OUTPUT_FILE_NAME_EXTENSION + "\".");
    P("To setup number of cores is recommended (implicitly 1).");
    cout << endl;
    
    P("   --help", "Prints this help.");
    P("-o --output", "Output file name.");
    P("-l --log", "Log file name.");
    cout << endl;
    
    P("-n --ncpu", "Number of cores the aplication will use.");
    P("-k --kmer", "K-mer length used for hashing.");
    cout << endl;
    
    P("-e --min_error_tolerance", "Minimum error tolerance for parameter searching.");
    P("-E --max_error_tolerance", "Maximum error tolerance for parameter searching.");
    P("-h --min_min_overlap", "Minimum minimum overlap length for parameter searching.");
    P("-H --max_min_overlap", "Maximum minimum overlap length for parameter searching.");
    cout << endl;
    
    P("-r --heterozygous_rate", "Rate for heterozygous site.");
    P("-s --sample_size", "Size of sample for parameter selection.");
    
    P("   --hash_size", "Size of hash table. Value x is interpreted as 2^x.");
    P("", string("Minimum value is ") + to_string(HASH_TABLE_SIZE_EXP_MIN) +
      " and maximum is " + to_string(HASH_TABLE_SIZE_EXP_MAX) + ".");
    
    P("   --version", "Prints application version.");
    cout << endl;
    P("--", "End of all options. Everything behind this flag will be treated as input");
    P("", "file even if it begins with - .");
    cout << endl;
    
    P("Return codes:");
    P(to_string(RETURN_SUCCESS) + "   Success.");
    P(to_string(RETURN_INC_OPT) + "   Incorrect options.");
    P(to_string(RETURN_FILES_SKIPPED) + "   Some files weren't processed.");
    P(to_string(RETURN_LOG_FILE_ERROR) + "   Cannot create log file.");
    P(to_string(RETURN_MEMORY_ERROR) + "   Error while allocatling memory. Operation was terminated.");
    P(to_string(RETURN_UNKNOWN_ERROR) + "   Unexpected error. This should normally never happen and means, that program");
    P("    is broken in some way.");
    P(to_string(RETURN_COMPATIBILITY_ERROR) + "   Compatibility error. Program must be compiled in a way, that variables have correct size.");
    
    cout << endl;
}

bool Options::testFor(int index, const char *opt1, const char *opt2) {
    int res1 = strcmp(argv[index], opt1);
    int res2 = 1;
    if(opt2) res2 = strcmp(argv[index], opt2);
    
    if (res1 == 0 || res2 == 0) return true;
    return false;
}

void Options::errInput(const char * message) {
    if (errMessage != "") return;
    
    ostringstream os;
    os << message << endl;
    os << "For help, run with: --help" << endl;
    
    errMessage = os.str();
    if (state == ST_OK) state = ST_INP_ERROR;
}

void Options::testFiles() {
    if (!inputFiles.size()) {
        errInput("At least one input file must be set.");
    }
    
    if (inputFiles.size() < outputFiles.size()) {
        errInput("Incorrect input: There are more output files, than input files.");
    }
}

void Options::P(const char *options, const char *description) const {
    cout << options;
    
    if (!description) {
        cout << endl;
        return;
    }
    
    long space = HELP_SPACE - strlen(options);
    for (long a = 0; a<space; a++) cout << ' ';
    cout << description << endl;
}

void Options::P(const string options, const string description) const {
    cout << options;
    
    if (description == "") {
        cout << endl;
        return;
    }
    
    long space = HELP_SPACE - options.length();
    for (long a = 0; a<space; a++) cout << ' ';
    cout << description << endl;
}

int Options::toInt(const char * str) {
    istringstream is;
    int result = 0;
    
    is.str(str);
    is >> result;
    
    if (is.fail()) errInput("Incorrect value of parameter.");
    return result;
}

double Options::toDouble(const char * str) {
    istringstream is;
    double result = 0.0;
    
    is.str(str);
    is >> result;
    
    if (is.fail()) errInput("Incorrect value of parameter.");
    return result;
}

//logging
void Options::logLaunchOptions(MessageLogger& logger) {
    stringstream msg;
    
    msg << "Launched with parameters:" << endl;
    
    //input file(s)
    if (inputFiles.size() == 1) indent("Input file: ", inputFileNames()[0], msg);
    else {
        string fileNames;
        for (int i=0; i<inputFiles.size(); i++) {
            fileNames += inputFiles[i] + " ";
        }
        indent("Input files: ", fileNames, msg);
    }
    
    //output file(s)
    if (outputFiles.size()) {
        if (outputFiles.size() == 1) indent("Output file: ", outputFileNames()[0], msg);
        else {
            string fileNames;
            for (int i=0; i<outputFiles.size(); i++) {
                fileNames += outputFiles[i] + " ";
            }
            indent("Output files: ", fileNames, msg);
        }
    }
    
    if (logFileName().length()) {
        indent("Log file: ", logFileName(), msg);
    }
    
    //other options
    indent("Number of cores: ", numberOfCores(), msg);
    if (kmerLenght())  indent("K-mer length: ", kmerLenght(), msg);
    if (minErrToler()) indent("Minimum error tolerance: ", minErrToler(), msg);
    if (maxErrToler()) indent("Maximum error tolerance: ", maxErrToler(), msg);
    if (minOverlap())  indent("Min min overlap: ", minOverlap(), msg);
    if (maxOverlap())  indent("Max min overlap: ", maxOverlap(), msg);
    if (heterRate())   indent("Heterozygous rate: ", heterRate(), msg);
    if (paramSampleSize()) indent("Parameter sample size: ", paramSampleSize(), msg);
    if (hashTableSize())   indent("Hash table size:", "2^", hashTableSize(), msg);
    
    logger.log(msg.str());
}

template<class T>
void Options::indent(const char *descr, T value, stringstream& msg) {
    msg << descr;
    long space = LAUNCH_LOG_SPACE - strlen(descr);
    for (int a=0; a<space; a++) msg << ' ';
    msg << value << endl;
}

template<class T>
void Options::indent(const char *descr, const char *prefix, T value, stringstream& msg) {
    msg << descr;
    long space = LAUNCH_LOG_SPACE - strlen(descr);
    for (int i=0; i<space; i++) msg << ' ';
    msg << prefix << value << endl;
}
