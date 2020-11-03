lasr
================
Donny Keighley
11/2/2020

[lasr](https://github.com/lordkeighley/lasr) is a package designed for
reading [Log Ascii Standard (LAS)](https://www.cwls.org/products/) files
in R and compiling the data into tables. Currently it is in the beta
testing stages.

## Goals

lasr is primarily designed to help build large log data tables for use
in machine learning workflows. To accomplish this, files are read using
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

[lasr](https://github.com/lordkeighley/lasr) is not designed to
accomplish traditional petrophysical workflows and there are no plans to
do so. Also, while efforts will be made to handle some non-standard and
erroneous LAS files, the goal is not to handle every edge case
perfectly. Rather, files that produce errors will produce the most
helpful error messages possible so that bad files can be checked and
fixed. Currently, it only pulls header and other curve data, though LAS
files can contain other data such as tops.

## Installation

You can install lasr from github using the
[*install\_github*](https://www.rdocumentation.org/packages/devtools/versions/1.13.6/topics/install_github)
function from the [devtools](https://devtools.r-lib.org/) package.

``` r
if (!require('devtools')) install.packages('devtools')
library(devtools)
install_github('https://github.com/lordkeighley/lasr')
```
