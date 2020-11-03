lasr
================
Donny Keighley
11/3/2020

[lasr](https://github.com/lordkeighley/lasr) is a package designed for
reading [Log Ascii Standard (LAS)](https://www.cwls.org/products/) files
in R and compiling the data into tables. Currently it is in the beta
testing stages.

## Goals

lasr is primarily designed to help build large log data tables for use
in non-standard workflows. It is optimized for speed. To accomplish
this, files are read using
[*fread*](https://www.rdocumentation.org/packages/data.table/versions/1.13.2/topics/fread)
from the [data.table](https://rdatatable.gitlab.io/data.table/) package.
The curves and headers are also stored as data.tables. Parsing of the
files is done using a custom C++ function connected with
[Rcpp](http://www.rcpp.org/).

Ultimately, functions to standardize the log curves for a well will be
added including:

  - Aliasing
  - Depth Merging
  - Unit Standardizing
  - Matrix Identification
  - Writing LAS files

[**lasr**](https://github.com/lordkeighley/lasr) is not designed to
accomplish traditional petrophysical workflows and there are no plans to
do so. Also, while efforts will be made to handle some non-standard and
erroneous LAS files, the goal is not to handle every edge case
perfectly. Rather, files that produce errors will produce the most
helpful error messages possible so that bad files can be checked and
fixed. If these are your goals, you should check out the excellent
python packages
[**lasio**](https://lasio.readthedocs.io/en/latest/index.html) and
[**welly**](https://welly.readthedocs.io/en/latest/api/welly.html).

## Installation

You can install [**lasr**](https://github.com/lordkeighley/lasr) from
github using the
[*install\_github*](https://www.rdocumentation.org/packages/devtools/versions/1.13.6/topics/install_github)
function from the [devtools](https://devtools.r-lib.org/) package.

``` r
if (!require('devtools')) install.packages('devtools')
library(devtools)
install_github('https://github.com/lordkeighley/lasr')
```

Currently, the only function is read.las which will import a vector of
LAS file paths into a two part list. The first element of the list is
named header and contains a data.table of header data. The second
element of the list is named curves and contains a data.table of the
associated log curve data. If your vector of paths contains more than
one file, the output will be a list of the aformentioned lists.

Here is an example reading a single LAS file that is included with the
package:

``` r
library(lasr)
las = read.las(system.file("extdata", "Jonah_Federal_20-5.las", package = "lasr"))

head(las$header, 10)
```

    ##     SECTION    MNEM UNIT                  VALUE
    ##  1: VERSION    VERS                         2.0
    ##  2: VERSION    WRAP                          NO
    ##  3: VERSION    PROD                Schlumberger
    ##  4: VERSION    PROG           DLIS to ASCII 2.2
    ##  5: VERSION    CREA            2015/01/05 09:49
    ##  6: VERSION  SOURCE      JF 20-5_Main Pass.DLIS
    ##  7: VERSION FILE-ID         SCMT_RST_PSP_012LUP
    ##  8:    WELL    STRT    F                12063.0
    ##  9:    WELL    STOP    F                 6915.0
    ## 10:    WELL    STEP    F                   -0.5
    ##                                   COMMENT           FORMAT
    ##  1: CWLS Log ASCII Standard - VERSION 2.0                 
    ##  2:               One Line per depth step                 
    ##  3:                          LAS Producer                 
    ##  4:          LAS Program name and version                 
    ##  5:                     LAS Creation date YYYY/MM/DD hh:mm
    ##  6:                        DLIS File Name                 
    ##  7:            File Identification Number                 
    ##  8:                           START DEPTH                 
    ##  9:                            STOP DEPTH                 
    ## 10:                                  STEP

``` r
head(las$curves, 10)
```

    ##        DEPT CIRN_FIL CIRF_FIL INND_FIL INFD_FIL   IRAT IRAT_FIL    SIGM
    ##  1: 12063.0       NA       NA       NA       NA     NA       NA      NA
    ##  2: 12062.5       NA       NA       NA       NA     NA       NA      NA
    ##  3: 12062.0       NA       NA       NA       NA     NA       NA      NA
    ##  4: 12061.5       NA       NA       NA       NA     NA       NA      NA
    ##  5: 12061.0       NA       NA       NA       NA     NA       NA      NA
    ##  6: 12060.5       NA       NA       NA       NA     NA       NA      NA
    ##  7: 12060.0   0.7474   2.2926 7129.522 3422.008 0.4800   0.4800 13.2762
    ##  8: 12059.5   0.7474   2.2926 7129.522 3422.008 0.4800   0.4800 13.2762
    ##  9: 12059.0   0.7487   2.3673 7176.949 3398.079 0.4726   0.4735 12.1268
    ## 10: 12058.5   0.7630   2.4401 7216.565 3402.010 0.4679   0.4714 11.6480
    ##     TSCN_FIL TSCF_FIL   TRAT TRAT_FIL   TPHI      GR     WTEP    CBL CCLD
    ##  1:       NA       NA     NA       NA     NA 25.8075 231.6347 2.2320    0
    ##  2:       NA       NA     NA       NA     NA 25.8075 231.6347 2.4066    0
    ##  3:       NA       NA     NA       NA     NA 25.8075 231.6347 2.5372    0
    ##  4:       NA       NA     NA       NA     NA 25.8075 231.6347 2.6092    0
    ##  5:       NA       NA     NA       NA     NA 25.8075 231.6347 2.6032    0
    ##  6:       NA       NA     NA       NA     NA 25.8075 231.6347 2.8285    0
    ##  7: 5328.310 7845.378 0.6792   0.6792 0.0674 25.8075 231.6347 3.0249    0
    ##  8: 5328.310 7845.378 0.6792   0.6792 0.0674 25.8075 231.6347 3.1866    0
    ##  9: 5381.087 8110.229 0.6524   0.6637 0.0610 25.8075 231.6347 3.2347    0
    ## 10: 5409.057 8309.733 0.6596   0.6510 0.0548 25.8075 231.6347 3.2273    0

## Speed Test

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

    ## Time difference of 37.43598 secs

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

## Compiling Tables

Eventually, this will probably built into a function, but for now if you
want to compile the curve and header data into tables you can use the
following code:

``` r
#Make a compiled curve table
curves = rbindlist(lapply(las, function(x) x$curves), fill=TRUE, use.names=TRUE)
curves[,path:=rep(names(las), do.call(c, lapply(las, function(x) nrow(x$curves))))]

#Make a compiled header table
headers = rbindlist(lapply(las, function(x) x$header), fill=TRUE, use.names=TRUE)
headers[,path:=rep(names(las), do.call(c, lapply(las, function(x) nrow(x$header))))]
```
