lasr
================
Donny Keighley
1/1/2021

[lasr](https://github.com/donald-keighley/lasr) is a package designed
for reading and writing [Log Ascii Standard
(LAS)](https://www.cwls.org/products/) files in R. Currently it is in
the beta testing stages. As such, it is subject to significant ongoing
changes and is not complete. For instance it can’t write LAS files yet…

Goals
-----

lasr is primarily designed to import LAS files at high speed and in
large batches. To accomplish this, most of it is written in C++ and
connected to R with [Rcpp](http://www.rcpp.org/). It stores the data in
lists of [data.table’s](https://rdatatable.gitlab.io/data.table/) for
fast manipulation.

Currently, the focus is on supporting reading [LAS
3.0](https://www.cwls.org/wp-content/uploads/2014/09/LAS_3_File_Structure.pdf)
as fully as possible. There will be some effort to handle non-standard
LAS files but nothing too cute. lasr is being written to load files
using as little information from the header as possible which should
alleviate many common issues. Beyond that, the aim is to output helpful
error messages so that non-standard files can be fixed. Afterall, one of
the great things about LAS files is that they are human readable and can
often be fixed using a simple text editor.

[**lasr**](https://github.com/donald-keighley/lasr) is not designed to
accomplish traditional petrophysical workflows and there are no plans to
do so. There’s plenty of industry software for that. lasr is intended as
a building block to facilitate people doing new and creative things with
large volumes of log data. It’s also intended to help build up the
library of geoscience packages for R.

For a more traditional approach, or if you’d prefer to use Python, you
should check out the excellent packages
[**lasio**](https://lasio.readthedocs.io/en/latest/index.html) and
[**welly**](https://welly.readthedocs.io/en/latest/api/welly.html).

Installation
------------

You can install [**lasr**](https://github.com/donald-keighley/lasr) from
github using the
[`install_github`](https://www.rdocumentation.org/packages/devtools/versions/1.13.6/topics/install_github)
function from the [devtools](https://devtools.r-lib.org/) package.

``` r
if (!require('devtools')) install.packages('devtools')
library(devtools)
install_github('https://github.com/donald-keighley/lasr')
```

Currently, the only function is `read.las` which will import a vector of
LAS file paths into a multi-part list. Each section of the file is now
stored as a separate element. In order to accomodate LAS 3.0 files which
may have multiple log data sections, the log parameter, log definition,
and log data are combined into one element called a “curveset”. If your
vector of paths contains more than one file, the output will be a list
of the aformentioned lists.

Here is an example reading a single LAS file that is included with the
package:

``` r
library(lasr)
las = read.las(system.file("extdata", "las_3_cwls.las", package = "lasr"))

#Display the WELL section
head(las$well, 10)
```

    ##     mnemonic unit                    value           comment     format
    ##  1:     STRT    M                1660.1250 First Index Value           
    ##  2:     STOP    M                1660.8750  Last Index Value           
    ##  3:     STEP    M                   0.1250              STEP           
    ##  4:     NULL                       -999.25        NULL VALUE           
    ##  5:     COMP          ANY OIL COMPANY INC.           COMPANY           
    ##  6:     WELL         ANY ET AL 01-02-03-04              WELL           
    ##  7:      FLD                       WILDCAT             FIELD           
    ##  8:      LOC                    1-2-3-4W5M          LOCATION           
    ##  9:     SRVC      ANY LOGGING COMPANY INC.   SERVICE COMPANY           
    ## 10:     DATE                    13/12/1986      Service DATE DD/MM/YYYY
    ##     association
    ##  1:            
    ##  2:            
    ##  3:            
    ##  4:            
    ##  5:            
    ##  6:            
    ##  7:            
    ##  8:            
    ##  9:            
    ## 10:

``` r
#Display the log curves
head(las$log$log.1$data, 10)
```

    ##        DEPT     DT DPHI NPHI      YME                     CDES NMR[1] NMR[2]
    ## 1: 1660.125 123.45 0.11 0.37 1.45E+12          DOLOMITE W/VUGS     10     12
    ## 2: 1660.250 123.45 0.12 0.36 1.47E+12                LIMESTONE     12     15
    ## 3: 1660.375 123.45 0.13 0.35 2.85E+12            LOST INTERVAL     18     25
    ## 4: 1660.500 123.45 0.14 0.34 2.85E+12            LOST INTERVAL     18     25
    ## 5: 1660.625 123.45 0.15 0.33 2.85E+12            LOST INTERVAL     18     25
    ## 6: 1660.750 123.45 0.16 0.32 2.85E+12 SANDSTONE, SHALE STREAKS     18     25
    ## 7: 1660.875 123.45 0.17 0.31 2.85E+12            LOST INTERVAL     18     25
    ##    NMR[3] NMR[4] NMR[5]
    ## 1:     14     18     13
    ## 2:     21     35     25
    ## 3:     10      8     17
    ## 4:     10      8     17
    ## 5:     10      8     17
    ## 6:     10      8     17
    ## 7:     10      8     17

Speed Test
----------

Since the purpose of this package is to load LAS files as quickly as
possible, a speed test is included here with a comparison to python’s
[**lasio**](https://lasio.readthedocs.io/en/latest/index.html). First,
download a test dataset from the KGS website. In this case we’re using
the [2016 logs](http://www.kgs.ku.edu/PRS/Scans/Log_Summary/2016.zip)
data. Download and unzip them into a folder called *“C:/temp/logs”*, or
modify the code for wherever you put it.

Next, import the first 500 files. We’ll use 4 threads for this
comparison, although if you have more cores you can increase the number
of threads to speed it up further. Only use this option if you are
importing more than a handful of files, otherwise the parallel overhead
will slow it down.

``` r
files = list.files('C:/temp/logs', pattern = '.las?', full.names=TRUE)
start.time = Sys.time()
las = read.las(files[1:500],nthreads=4)
end.time = Sys.time()
time.taken = end.time - start.time
time.taken
```

    ## Time difference of 29.35815 secs

Now in Python in parallel using 4 cores:

``` python
import lasio, glob, datetime, multiprocessing
from joblib import Parallel, delayed

num_cores = 4
files = glob.glob('C:/temp/logs/*.las')
start_time = datetime.datetime.now()
if __name__ == "__main__":
    las = Parallel(n_jobs=num_cores)(delayed(lasio.read)(file) for file in files[0:499])
end_time = datetime.datetime.now()
print('Duration: {}'.format(end_time - start_time))
```

    ## Duration: 0:04:32.132885

Clearly, lasr is faster, however, please don’t take this as a shot at
lasio. The primary goal of this package is speed, and as such countless
hours have been put into speed testing, de-bottlenecking, and enduring
the pain of writing in C++. As with anything, there are tradeoffs, and
lasr errs toward speed where lasio tends more toward user convenience.
They are simply different products.

Good luck, and if you have any suggestions reach out!
