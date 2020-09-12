#' @name read.las
#' @title read.las
#' @description Imports an LAS file to R
#' @import data.table
#' @import stringi
#' @importFrom zoo na.locf
#' @export
#' @param path The path to the LAS file
#' @return A two part list containing the LAS file and the header
#' @examples
#' #path = 'C:/Users/donal/Documents/R/3522229A.las'
#' #lasFile = read.las()
#' # http://pipeline.wyo.gov/whatups/whatuplas.cfm?nautonum=80871
read.las = function(path){
  #Resolves 'no visible binding for global variable' errors
  MNEM <- SECT <- line <- NULL
  
  #Reads in the data as lines using fread for speed
  lines = fread(path, sep=NULL, header=FALSE, showProgress=FALSE, quote="", col.names='line')
  
  #Parses the header using Rcpp for speed
  header = splitHeaderLines(lines$line)
  header = header[MNEM!='',]
  
  #Gets the wrap, null, and delimiter
  wrap = header$VALUE[header$MNEM=='WRAP'][1]
  nullVal = as.numeric(header$VALUE[header$MNEM=='NULL'][1])

  #Gets the log data
  ld = lines[(grep('~A',line)+1):length(line)]$line
  ncurves = sum(header$SECT=='C')
  curveNames = header$MNEM[header$SECT=='C']
  logData = getLogData(ld, curveNames, nullVal)
  
  # logData = strsplit(lines[(grep('~A',line)+1):length(line)]$line, split=delim)
  # if(isTRUE(wrap=='YES')){
  #   do.call(c, lapply(logData, length))
  # }else{
  #   logData = data.table(do.call(rbind, logData))
  # }
  # 
  # #Adds names to curves
  # names(logData) = header[SECT=='C', ]$MNEM[1:length(names(logData))]
  # 
  # #Converts to numeric
  # logData = logData[, lapply(.SD, as.numeric),]
  # 
  # #Replaces nulls
  # if(!is.na(nullVal)){
  #   logData[logData==nullVal]=NA
  # }

  list(header,logData)
}