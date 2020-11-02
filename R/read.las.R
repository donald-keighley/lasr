#' @name read.las.helper
#' @title read.las.helper
#' @description Imports an LAS file to R
#' @import data.table
#' @keyword internal
#' @param path The path to the LAS file
#' @return A two part list containing the LAS file and the header
read.las.helper = function(path){

  #Assumes the file is bad unless it proves not to be
  bad_las=TRUE
  if(any(is.null(path), is.na(path), is.nan(path), !is.character(path))){
    warning(paste0('File ', path, ' is not valid'))
  }else if(!file.exists(path)){
    warning(paste0('File ', path, ' does not exist'))
  }else{
    #Reads in the data as lines using fread for speed
    tryCatch({
      lines = suppressWarnings(fread(path, sep=NULL, header=FALSE, showProgress=FALSE, quote="", col.names='line')$line)
      if(length(lines)>0){
        las = splitHeaderLines(lines)
        bad_las = FALSE
      }
    }, error = function(e){
      warning(paste0('File ', path, 'failed to read or parse properly'))
    }) 
   }
  
  #Creates an empty dataset in case the file fails to read
  if(bad_las==TRUE){
    char_na = as.character(NA)
    header = data.frame("SECTION"=char_na, "MNEM"=char_na, "UNIT"=char_na,"VALUE"=char_na, "COMMENT"=char_na,"FORMAT"=char_na)
    las = list('header'=header, 'curves'=NULL)
  }
  
  #Converts to a data.table, names the list after the file path
  las$header=as.data.table(las$header)
  las$curves=as.data.table(las$curves)
  
  return(las)
}

#' @name read.las
#' @title read.las
#' @description Imports an LAS file to R
#' @import data.table
#' @import parallel
#' @export
#' @param paths A vector of LAS file paths
#' @param nthreads An integer indicating the number of threads to use
#' @return A two part list containing the LAS file and the header
#' @examples
#' las = read.las(system.file("extdata", "Jonah_Federal_20-5.las", package = "lasr"))
read.las = function(paths, nthreads=1){
  if(length(paths)>1){
    if(nthreads > 1){
      cores = min(detectCores(logical=TRUE), nthreads)
      cl = makeCluster(cores)
      clusterEvalQ(cl, {library(lasr)})
      clusterExport(cl, c("files", "read.las.helper"), envir=environment())
      las = parLapply(cl, paths, read.las.helper)
      stopCluster(cl)
    }else{
      las = lapply(paths, read.las.helper)
    }
    names(las) = paths
  }else{
    las = read.las.helper(paths[1])
  }
  return(las)
}