lasr
================
Donny Keighley
1/1/2021

[lasr](https://github.com/donald-keighley/lasr) is a package designed
for reading [Log Ascii Standard (LAS)](https://www.cwls.org/products/)
files in R and compiling the data into tables. Currently it is in the
beta testing stages. Recently it was modified to more fully support LAS
3.0.

Goals
-----

lasr is primarily designed to help build large log data tables for use
in non-standard workflows. It is optimized for speed. To accomplish
this, most of it is written in C++ and connected to R with
[Rcpp](http://www.rcpp.org/).

Ultimately, functions to standardize the log curves for a well will be
added including:

-   Aliasing
-   Depth Merging
-   Unit Standardizing
-   Matrix Identification
-   Writing LAS files

[**lasr**](https://github.com/donald-keighley/lasr) is not designed to
accomplish traditional petrophysical workflows and there are no plans to
do so. Also, while efforts will be made to handle some non-standard and
erroneous LAS files, the goal is not to handle every edge case
perfectly. Rather, files that produce errors will produce the most
helpful error messages possible so that bad files can be checked and
fixed. If these are your goals, you should check out the excellent
python packages
[**lasio**](https://lasio.readthedocs.io/en/latest/index.html) and
[**welly**](https://welly.readthedocs.io/en/latest/api/welly.html).

Installation
------------

You can install [**lasr**](https://github.com/donald-keighley/lasr) from
github using the
[*install\_github*](https://www.rdocumentation.org/packages/devtools/versions/1.13.6/topics/install_github)
function from the [devtools](https://devtools.r-lib.org/) package.

``` r
if (!require('devtools')) install.packages('devtools')
library(devtools)
install_github('https://github.com/donald-keighley/lasr')
```

Currently, the only function is read.las which will import a vector of
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
las = read.las(system.file("extdata", "Jonah_Federal_20-5.las", package = "lasr"))

#Display the WELL section
head(las$WELL, 10)
```

    ##     MNEMONIC UNIT                            VALUE     COMMENT FORMAT
    ##  1:     STRT    F                          12063.0 START DEPTH       
    ##  2:     STOP    F                           6915.0  STOP DEPTH       
    ##  3:     STEP    F                             -0.5        STEP       
    ##  4:     NULL                               -999.25  NULL VALUE       
    ##  5:     COMP           Encana Oil & Gas (USA) Inc.     COMPANY       
    ##  6:     WELL                    Jonah Federal 20-5        WELL       
    ##  7:      FLD                                 Jonah       FIELD       
    ##  8:      LOC      SHL: Lot-6, 690' FNL & 2203' FEL    LOCATION       
    ##  9:     CNTY                              Sublette      COUNTY       
    ## 10:     STAT                               Wyoming       STATE       
    ##     ASSOCIATION
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
head(las$CURVESETS$CURVESET_1$LOG_DATA, 10)
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

    ## Time difference of 31.80392 secs

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

Future work
-----------

Currently, the focus is on testing with as many files as possible in
order to find and fix bugs and add error messages. In terms of
functionality, writing a function to export LAS files will probably be
next. Adding additional functions to parse some of the other LAS 3.0
sections like Core and Tops is high on the list as well.
