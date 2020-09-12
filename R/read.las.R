#' @name read.las
#' @title read.las
#' @description Imports an LAS file to R
#' @importFrom data.table fread
#' @export
#' @param path The path to the LAS file
#' @return A two part list containing the LAS file and the header
#' @examples
#' #read.las(system.file("extdata", "Jonah_Federal_20-5.las", package = "lasr"))
read.las = function(path){

  #Reads in the data as lines using fread for speed
  lines = fread(path, sep=NULL, header=FALSE, showProgress=FALSE, quote="", col.names='line')
  
  #Parses the data
  las = splitHeaderLines(lines$line)
  
  return(las)
}