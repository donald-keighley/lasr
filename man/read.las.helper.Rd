% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/read.las.R
\name{read.las.helper}
\alias{read.las.helper}
\title{read.las.helper}
\usage{
read.las.helper(
  path,
  header_only = FALSE,
  extra = FALSE,
  flatten = FALSE,
  comments = FALSE
)
}
\arguments{
\item{path}{The path to the LAS file}

\item{header_only}{TRUE or FALSE depending on whether you want to return only header data.}

\item{extra}{Return the non-log data sections? FALSE by default.}

\item{flatten}{Return just the first set of log data from a file.}

\item{comments}{If TRUE comments will be appended to the list.}
}
\value{
A two part list containing the LAS file and the header.
}
\description{
Imports a LAS file to R as a list.
}
\keyword{internal}
