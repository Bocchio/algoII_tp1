#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cfloat>
#include <string>

#include <ctime>  // for timing purposes
#include <vector>
#include <complex>

#include "cmdline.hpp"
#include "vector.hpp"
#include "complex.hpp"
#include "ft.hpp"
#include "errors.hpp"
#include "dictionary.hpp"

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::fstream;
using std::ios;
using std::istringstream;
using std::string;

static void opt_input(string const &);
static void opt_output(string const &);
static void opt_method(string const &);
static void opt_help(string const &);

// syntax: {needs argument,
//          short name,
//          long name,
//          default value,
//          parser,
//          flag}
static option_t options[] = {
    {1, "i", "input", "-", opt_input, OPT_DEFAULT},
    {1, "o", "output", "-", opt_output, OPT_DEFAULT},
    {1, "m", "method", "FFT", opt_method, OPT_DEFAULT},
    {0, "h", "help", NULL, opt_help, OPT_DEFAULT},
    {0, },
};

static istream *iss = 0;
static ostream *oss = 0;
typedef Vector<Complex> ( *transform_method)(const Vector<Complex> &v);
//static transform_method transform;
static fstream ifs;
static fstream ofs;
FourierTransform * FT;
static void opt_input(string const &arg) {
    if (arg == "-") {
        iss = &cin;
    } else {
        ifs.open(arg.c_str(), ios::in);
        iss = &ifs;
    }

    if (!iss->good()) {
        cerr << ERROR_MSG_CANT_OPEN_FILE
             << arg
             << "."
             << endl;
        exit(1);
    }
}

static void opt_output(string const &arg) {
    if (arg == "-") {
        oss = &cout;
    } else {
        ofs.open(arg.c_str(), ios::out);
        oss = &ofs;
    }

    if (!oss->good()) {
        cerr << ERROR_MSG_CANT_OPEN_FILE
             << arg
             << "."
             << endl;
        exit(1);
    }
}

static void opt_method(const string& method) {
    // Dictionary<FourierTransform *> method_lookup_table;

    // method_lookup_table.append("FFT", new FFT());
    // method_lookup_table.append("IFFT", new IFFT());
    // method_lookup_table.append("DFT", new DFT());
    // method_lookup_table.append("DFT", new IDFT());

    // FT = method_lookup_table[method];
    if(method == "DFT"){
        FT = new DFT();
    } else if (method == "IDFT"){
        FT = new IDFT();
    } else if(method == "FFT"){
        FT = new FFT();
    } else if(method == "IFFT"){
        FT = new IFFT();
    } else {
        cerr << ERROR_MSG_UNKNOWN_METHOD << endl;
        opt_help("");
    }
}

static void opt_help(string const &arg) {
    cout << HELP_MSG
         << endl;
    exit(0);
}

int main(int argc, char * const argv[]) {
    cmdline cmdl(options);
    cmdl.parse(argc, argv);

    Vector<Complex> v;

    if ((*iss >> v).bad()) {
        cerr << ERROR_MSG_CORRUPTED_DATA << endl;
    }

    *oss << FT->transform(v) << endl;

    return 0;
}
