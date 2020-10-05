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
  bad_las=FALSE
  if(file.exists(path)){
    lines = suppressWarnings(fread(path, sep=NULL, header=FALSE, showProgress=FALSE, quote="", col.names='line'))
  }else{
    bad_las=TRUE
    warning('File does not exist.')
  }
  
  #Parses the data
  if(nrow(lines)>0){
    las = splitHeaderLines(lines$line)
  }else{
    bad_las=TRUE
    warning('File contains no data.')
  }
  
  if(isTRUE(bad_las)){
    #Creates a default empty dataset
    char_na = as.character(NA)
    header = data.frame("SECTION"=char_na, "MNEM"=char_na, "UNIT"=char_na,"VALUE"=char_na, "COMMENT"=char_na,"FORMAT"=char_na)
    las = list('header'=header, 'curves'=NULL)
  }
  
  return(las)
}