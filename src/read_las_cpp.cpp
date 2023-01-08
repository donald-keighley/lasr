#include <Rcpp.h>
#include <fstream>
#include <locale>
#include <codecvt>
#include <algorithm>
#include <string>
#include "header_functions.h"
#include "curve_functions.h"
#include "LasMap.h"

Rcpp::List get_datasets(const std::vector<std::string> &lines, 
                        const std::vector<std::vector<int> > &indices, 
                        const std::string section,
                        LasMap &las_map, 
                        const std::string delim, const std::string null_str, 
                        const bool header_only){

  int num_datasets = indices.size();
  std::vector<std::string> dataset_names(num_datasets);
  Rcpp::List datasets(num_datasets);
  Rcpp::DataFrame df_temp;
  if(num_datasets>0){
    for(int i=0; i<num_datasets; i++){
      Rcpp::List dataset(3);
      dataset.attr("names") = Rcpp::CharacterVector({"parameter","definition","data"});
      int param_index = indices[i][0];
      if(param_index>0){
        dataset[0] = parse_header(lines, las_map.start_index(param_index), las_map.end_index(param_index));
      }
      int def_index = indices[i][1];
      if(def_index>0){
        df_temp = parse_header(lines, las_map.start_index(def_index), las_map.end_index(def_index));
        dataset[1] = df_temp;
        int data_index = indices[i][2];
        if((!header_only) & (data_index>0) & (df_temp.nrows()>0)){
          dataset[2] = parse_curves(lines, df_temp["mnemonic"],  df_temp["format"],
                                    delim, null_str, las_map.start_index(data_index),
                                    las_map.end_index(data_index));
        }
      }
      datasets[i] = dataset;
      dataset_names[i] = section + "." + std::to_string(i+1);
    }
    datasets.attr("names") = dataset_names;
  }
  return(datasets);
}

//' @useDynLib lasr
//' @importFrom Rcpp sourceCpp
//' @name read_las_cpp
//' @title read_las_cpp
//' @description A function to read LAS files.  Not really meant to be called by the user.
//' @param lines A vector of lines with blank lines and leading whitespace removed
//' @param header_only If true, will only return the header portions.
//' @param extra If true, will return the extra data sections beyond just logs.
//' @param comments If true, will return all the comments appended to the list.
//' @return A two part list
// [[Rcpp::export]]
Rcpp::List read_las_cpp(std::vector<std::string>& lines, bool header_only = false, bool extra = false, bool comments = false){
  
  std::string line;
  LasMap las_map;
  std::string sect_string = "";
  std::vector<std::string> comments_vector;
  int sect_begin = -1;
  int n = 0;
  int good_line_index = 0;
  for(n=0; n<(int)lines.size(); n++) {
    line = lines[n];
    if(line[0]=='~'){
      if((sect_string.size()>0) & ((good_line_index-1)>=sect_begin)){las_map.push_back(sect_string, sect_begin, good_line_index-1);}
      sect_string = line;
      sect_begin = good_line_index+1;
      good_line_index++;
    }else if(line[0]=='#'){
      if(comments){
        comments_vector.push_back(line);
      }
    }else if(!line.empty()){
      good_line_index++;
    }
  }
  if((sect_string.size()>0) & (good_line_index>=sect_begin)){las_map.push_back(sect_string, sect_begin, good_line_index-1);}
  if(las_map.size()==0){Rcpp::stop("No sections found, possibly not a LAS file?");}
  
  // Removes comments and blank lines
  lines.erase(std::remove_if(lines.begin(), lines.end(), [](std::string& x) { std::string fc = x.substr(0,1); return ((fc=="#")|(fc==""));}), lines.end());
  
  //Gets the indices of the various sections
  int version_index = las_map.section_indices("VERSION")[0][0];
  int well_index = las_map.section_indices("WELL")[0][0];

  //Creates the outputs
  Rcpp::List las_list(3);
  las_list.attr("names") = Rcpp::CharacterVector({"version","well","log"});
  if(extra){
    Rcpp::List las_list_extra(9);
    las_list_extra.attr("names") = Rcpp::CharacterVector({"version","well","log","core","inclinometry","drilling","tops","test","user"});
    las_list = las_list_extra;
  }
  if(comments){las_list.push_back(comments_vector, "comments");}
  Rcpp::DataFrame df_temp;

  //Gets the version section
  std::string delim = " ";
  if(version_index>=0){
    df_temp = parse_header(lines, las_map.start_index(version_index), 
                           las_map.end_index(version_index));
    las_list[0] = df_temp;
    delim = get_header_string(df_temp, "DLM");
  }
  
  //Gets the well section and null value
  std::string null_str;
  if(well_index>=0){
    df_temp = parse_header(lines, las_map.start_index(well_index), 
                           las_map.end_index(well_index));
    las_list[1] = df_temp;
    null_str = get_header_string(df_temp, "NULL");
  }
  if(null_str==""){null_str=="-999.25";}
  
  //Gets the log data sections
  std::vector<std::vector<int> > indices = las_map.section_indices("LOG");
  if(indices.size()>0){las_list[2] = get_datasets(lines, indices, "log", las_map, delim, null_str, header_only);}
  
  //Gets additional data sections if required
  if(extra){
    
    //Gets the core data section
    indices = las_map.section_indices("CORE");
    if(indices.size()>0){las_list[3] = get_datasets(lines, indices,
       "core", las_map, delim, null_str, header_only);}
    
    //Gets the inclination data section
    indices = las_map.section_indices("INCLINOMETRY");
    if(indices.size()>0){las_list[4] = get_datasets(lines, indices,
       "inclinometry", las_map, delim, null_str, header_only);}
    
    //Gets the drilling data section
    indices = las_map.section_indices("DRILLING");
    if(indices.size()>0){las_list[5] = get_datasets(lines, indices,
       "drilling", las_map, delim, null_str, header_only);}
    
    //Gets the tops data section
    indices = las_map.section_indices("TOPS");
    if(indices.size()>0){las_list[6] = get_datasets(lines, indices,
       "tops", las_map, delim, null_str, header_only);}
    
    //Gets the test data section
    indices = las_map.section_indices("TEST");
    if(indices.size()>0){las_list[7] = get_datasets(lines, indices,
       "test", las_map, delim, null_str, header_only);}
    
    //Gets the user strings
    std::vector<std::string> user_sections = las_map.get_user_sections();
    if(user_sections.size()>0){
      Rcpp::List sectionList;
      for(std::size_t i=0; i<user_sections.size(); i++){
        indices = las_map.section_indices(user_sections[i]);
        if(indices.size()>0){
          sectionList.push_back(get_datasets(lines, indices,user_sections[i], las_map, delim, null_str, header_only));
        }
      }
      sectionList.attr("names") = user_sections;
      las_list[8] = sectionList;
    }
  }
  return(las_list);
}

