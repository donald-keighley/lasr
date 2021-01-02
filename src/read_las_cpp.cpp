#include <Rcpp.h>
#include <fstream>
#include <locale>
#include <codecvt>
#include <algorithm>
#include <string>
#include "header_functions.h"
#include "curve_functions.h"
#include "LasMap.h"

//' @useDynLib lasr
//' @importFrom Rcpp sourceCpp
//' @name read_las_cpp
//' @title read_las_cpp
//' @description A function to read LAS files.  Not really meant to be called by the user.
//' @param lines A vector of lines with blank lines and leading whitespace removed
//' @param header_only If true, will only return the header portions.
//' @return A two part list
// [[Rcpp::export]]
Rcpp::List read_las_cpp(std::vector<std::string>& lines, bool header_only = false){
  // Removes comments
  lines.erase(std::remove_if(lines.begin(), lines.end(), [](std::string& x) { return x.substr(0,1)=="#";}), lines.end());
  std::string line;
  LasMap las_map;
  std::string sect_string = "";
  int sect_begin = -1;
  int n = 0;
  for(n=0; n<(int)lines.size(); n++) {
    line = lines[n];
    if(line[0]=='~'){
      if((sect_string.size()>0) & ((n-1)>=sect_begin)){las_map.push_back(sect_string, sect_begin, n-1);}
      sect_string = line;
      sect_begin = n+1;
    }
  }
  if((sect_string.size()>0) & (n>=sect_begin)){las_map.push_back(sect_string, sect_begin, n);}
  if(las_map.size()==0){Rcpp::stop("No sections found, possibly not a LAS file?");}
  
  //Gets the indices of the various sections
  int vi = las_map.version_index();
  int wi = las_map.well_index();
  std::vector<std::vector<int> > ci = las_map.curveset_indices();
  int num_curves = ci.size();

  //Creates the outputs
  Rcpp::List las_list(3);
  las_list.attr("names") = Rcpp::CharacterVector({"VERSION","WELL","CURVESETS"});
  Rcpp::List curvesets(num_curves);
  Rcpp::DataFrame df_temp;

  //Gets the version section
  std::string delim = " ";
  if(vi>=0){
    df_temp = parse_header(lines, las_map.start_index(vi), las_map.end_index(vi));
    las_list[0] = df_temp;
    delim = get_header_string(df_temp, "DLM");
  }

  //Gets the well section and null value
  double null_val = -999.25;
  if(wi>=0){
    df_temp = parse_header(lines, las_map.start_index(wi), las_map.end_index(wi));
    las_list[1] = df_temp;
    null_val = get_header_double(df_temp, "NULL", -999.25);
  }

  //Gets the curvesets
  std::vector<std::string> curveset_names(num_curves);
  if(num_curves>0){
    for(int i=0; i<num_curves; i++){
      Rcpp::List curveset(3);
      curveset.attr("names") = Rcpp::CharacterVector({"LOG_PARAMETER","LOG_DEFINITION","LOG_DATA"});
      int pi = ci[i][0];
      if(pi>0){
        curveset[0] = parse_header(lines, las_map.start_index(pi), las_map.end_index(pi));
      }
      int li = ci[i][1];
      if(li>0){
        df_temp = parse_header(lines, las_map.start_index(li), las_map.end_index(li));
        curveset[1] = df_temp;
        int ai = ci[i][2];
        if((!header_only) & (ai>0)){
          curveset[2] = parse_curves(lines, df_temp["MNEMONIC"],  df_temp["FORMAT"],delim, null_val, las_map.start_index(ai), las_map.end_index(ai));
        }
      }
      curvesets[i] = curveset;
      curveset_names[i] = "CURVESET_" + std::to_string(i+1);
    }
    curvesets.attr("names") = curveset_names;
    las_list[2] = curvesets;
  }

  return(las_list);
}

