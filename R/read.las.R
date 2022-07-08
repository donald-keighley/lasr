#' @name list.to.dt
#' @title list.to.dt
#' @description Converts all data.frames in a list to data.tables in place
#' @import data.table
#' @keywords internal
#' @param l A list
#' @return Nothing, data.frames are converted in place

list.to.dt = function(l){
  if(inherits(l, 'data.frame')){
    setDT(l)
  }else if(inherits(l,'list')){
    lapply(l, list.to.dt)
  }
  return(NULL)
}


#' @name read.las.helper
#' @title read.las.helper
#' @description Imports a LAS file to R as a list.
#' @import data.table
#' @keywords internal
#' @param path The path to the LAS file
#' @param header_only TRUE or FALSE depending on whether you want to return only header data.
#' @param extra Return the non-log data sections? FALSE by default.
#' @param flatten Return just the first set of log data from a file.
#' @return A two part list containing the LAS file and the header
read.las.helper = function(path, header_only=FALSE, extra=FALSE, flatten=FALSE){
  
  #Assumes the file is bad unless it proves not to be
  bad_las=TRUE
  if(any(is.null(path), is.na(path), is.nan(path), !is.character(path))){
    warning(paste0(path, ': File path is not valid'))
  }else if(!file.exists(path)){
    warning(paste0(path, ':File does not exist'))
  }else if(file.size(path)==0){
    warning(paste0(path, ': File is empty'))
  }else{
    #Reads in the data as lines using fread for speed
    tryCatch({
      lines = fread(path, sep=NULL, header=FALSE, col.names = "line", 
                    strip.white=TRUE, blank.lines.skip = TRUE, 
                    showProgress = FALSE, colClasses = "character")$line
      las = read_las_cpp(lines, header_only, extra)
      invisible(list.to.dt(las))
      if(flatten){las$log = las$log$log.1}
      bad_las = FALSE
    }, error = function(e){
      return(warning(paste0(path, ': ', e$message)))
    }) 
  }
  
  #Returns a null value in case it fails to read
  if(bad_las==TRUE){las = NULL}
  las$path = path
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
#' @param extra LAS 3 files can have additional non-log data sections.  To import those
#' sections set this to TRUE.  By default it is FALSE because LAS 3 files with extra
#' data are rare in the wild.  Note that the extra data may have non-standard names so
#' referencing them may take some experimentation.
#' @param flatten If TRUE Return just the first set of log data from a file.  
#' This makes it easier to reference the log data since most files have only 
#' one set of log data.
#' @param pad_list If TRUE adds a level to the resulting list when the number of 
#' files is one.  This makes automation easier since referencing the data will
#' always be consistent.  FALSE by default to make working with single files easier.
#' @return A list containing a version, well, log, and path sections as well as 
#' any additional sections if extra is set to TRUE.
#' @examples
#' # load a single las file
#' f = system.file("extdata", "Jonah_Federal_20-5.las", package = "lasr")
#' las = read.las(f)
#' 
#' # get the version section
#' las$version
#' 
#' # get the well section
#' las$well
#' 
#' #' # get the original file path
#' las$path
#' 
#' # get the log data, definition, and parameters
#' las$log$log.1$data
#' las$log$log.1$definition
#' las$log$log.1$parameter
#' 
#' # use the flatten parameter to get just the first log data set
#' las = read.las(f, flatten = TRUE)
#' 
#' # referencing the log data is now shorter
#' las$log$data
#' 
#' # read multiple files.  consider setting nthreads > 1 if you have many files.
#' files = c(system.file("extdata", "Jonah_Federal_20-5.las", package='lasr'),
#' system.file("extdata", "las_3_cwls.las", package='lasr'))
#' las = read.las(files)
#' 
#' # in this case the imported las files are bound together in a list
#' # to reference the data you must first specify the element
#' # for instance, to get the log data from the second log
#' las[[2]]$log$log.1$data
#' 
#' # get extra data sections from a LAS 3 file
#' f = system.file("extdata", "las_3_cwls.las", package='lasr')
#' las = read.las(f, extra = TRUE)
#' 
#' # gets the core data as an example
#' las$core$core.1$data
#' 
#' # get just the header and not the actual data
#' # this is faster if you only need header data for many files
#' header = read.las(f, header_only=TRUE)
#' 
#' # using pad_list = TRUE so that accessing data from one file is the same
#' # as accessing it from many files (i.e. [[1]] is required)
#' las = read.las(f, pad_list = TRUE)
#' las[[1]]$log$log.1$data
read.las = function(paths, nthreads=1, header_only=FALSE, extra=FALSE, flatten=FALSE,
                    pad_list=FALSE){
  if(length(paths)>1){
    if(nthreads > 1){
      cores = min(detectCores(logical=TRUE), nthreads)
      cl = makeCluster(cores)
      clusterEvalQ(cl, {library(lasr)})
      clusterExport(cl, c("read.las.helper"), envir=environment())
      las = parLapply(cl, paths, read.las.helper, header_only = header_only, extra = extra, flatten = flatten)
      stopCluster(cl)
    }else{
      las = lapply(paths, read.las.helper, header_only = header_only, extra = extra, flatten = flatten)
    }
  }else{
    las = read.las.helper(paths[1], header_only = header_only, extra = extra, flatten = flatten)
    if(pad_list){las = list(las)}
  }
  return(las)
}
