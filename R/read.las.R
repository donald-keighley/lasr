#' @name read.las.helper
#' @title read.las.helper
#' @description Imports an LAS file to R
#' @import data.table
#' @keywords internal
#' @param path The path to the LAS file
#' @param header_only TRUE or FALSE depending on whether you want to return only header data.
#' @param logs_only TRUE if you want to only return up to the header section.
#' @return A two part list containing the LAS file and the header
read.las.helper = function(path, header_only=FALSE, logs_only=TRUE){
  
  #Assumes the file is bad unless it proves not to be
  bad_las=TRUE
  if(any(is.null(path), is.na(path), is.nan(path), !is.character(path))){
    warning(paste0(path, ': File path is not valid'))
  }else if(!file.exists(path)){
    warning(paste0(path, ':File does not exist'))
  }else{
    #Reads in the data as lines using fread for speed
    tryCatch({
      lines = fread(path, sep=NULL, header=FALSE, col.names = "line", 
                    strip.white=TRUE, blank.lines.skip = TRUE, 
                    showProgress = FALSE, colClasses = "character")$line
      las = read_las_cpp(lines, header_only, logs_only)
      bad_las = FALSE
    }, error = function(e){
      return(warning(paste0(path, ': ', e$message)))
    }) 
  }
  
  #Returns a null value in case it fails to read
  if(bad_las==TRUE){las = NULL}
  
  return(las)
}

#' @name read.las
#' @title read.las
#' @description This function imports a LAS file to R.
#' @import data.table
#' @import parallel
#' @export
#' @param paths A vector of LAS file paths.
#' @param nthreads An integer indicating the number of threads to use.  If the number of 
#' threads and paths is greater than one it will run in parallel.  Default is one.
#' @param header_only TRUE or FALSE depending on whether you want to return only header data. 
#' If you only want headers, this will run faster.
#' @param logs_only TRUE (default) if you only want to return up to the log section.
#' @return A list containing a Version, Well, and Curveset sections.
#' @examples
#' las = read.las(system.file("extdata", "Jonah_Federal_20-5.las", package = "lasr"))
read.las = function(paths, nthreads=1, header_only=FALSE, logs_only=TRUE){
  if(length(paths)>1){
    if(nthreads > 1){
      cores = min(detectCores(logical=TRUE), nthreads)
      cl = makeCluster(cores)
      clusterEvalQ(cl, {library(lasr)})
      clusterExport(cl, c("files", "read.las.helper"), envir=environment())
      las = parLapply(cl, paths, read.las.helper, header_only = header_only, logs_only = logs_only)
      stopCluster(cl)
    }else{
      las = lapply(paths, read.las.helper, header_only = header_only, logs_only = logs_only)
    }
    names(las) = paths
  }else{
    las = read.las.helper(paths[1], header_only = header_only, logs_only = logs_only)
  }
  
  return(las)
}