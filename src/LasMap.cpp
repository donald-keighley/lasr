#include "LasMap.h"
#include <Rcpp.h>
#include <algorithm>
using namespace Rcpp;

namespace{
  std::string trim_ws(std::string s){ 
    s.erase(0, s.find_first_not_of(" \t"));
    s.erase(s.find_last_not_of(" \t") + 1);
    return s;
  }
}

void LasMap::push_back(std::string section_string, int start_index, int end_index){
  std::transform(section_string.begin(), section_string.end(), section_string.begin(), ::toupper);
  std::size_t association_delim = section_string.find_first_of("|",1);
  std::string name_str;
  std::string association_str;
  if(association_delim != std::string::npos){
    name_str = trim_ws(section_string.substr(1,association_delim-1));
    association_str = trim_ws(section_string.substr(association_delim + 1, section_string.size()));
  }else{
    name_str = trim_ws(section_string.substr(1, section_string.size()));
    association_str = "";
  }
  int number_val = 0;
  std::string root_name_str;
  if(name_str.back()==']'){
    int delim_pos = name_str.find_first_of("[");
    number_val = std::atoi(name_str.substr(delim_pos+1, (name_str.size()-2)-delim_pos).c_str());
    root_name_str = name_str.substr(0,delim_pos);
  }
  
  name.emplace_back(name_str);
  root_name.emplace_back(root_name_str);
  number.emplace_back(number_val);
  association.emplace_back(association_str);
  start.emplace_back(start_index);
  end.emplace_back(end_index);
  
}

//Finds the first section name that begins with a V
int LasMap::version_index(){
  int output = -1;
  for(std::size_t i=0; i<name.size(); i++){
    if(name[i].substr(0,1)=="V"){
      output = i;
      break;
    }
  }
  return(output);
}

//Finds the first section name that begins with a W
int LasMap::well_index(){
  int output = -1;
  for(std::size_t i=0; i<name.size(); i++){
    if(name[i].substr(0,1)=="W"){
      output = i;
      break;
    }
  }
  return(output);
}

//Finds the indices of the curvesets.
//Starts at the last section and works backward so it can stop quickly if it's an A
std::vector<std::vector<int> > LasMap::curveset_indices(){
  std::vector<std::vector<int> > output;
  //The last section in all versions must be a log data section.
  //In the vast majority of cases this will be an A with an associated P and C section.
  if(name.back().substr(0,1)=="A"){
    int parameter_index = -1;
    int curve_index = -1;
    //Gets the last section that starts with P, but stops if it finds PARAM
    for(std::size_t i=0; i<name.size(); i++){
      if(name[i].substr(0,1) == "P"){
        parameter_index = i;
        if(name[i].substr(0,5) == "PARAM"){break;}
      }
    }
    //Gets the last section that starts with C, but stops if it finds CURVE
    for(std::size_t i=0; i<name.size(); i++){
      if(name[i].substr(0,1) == "C"){
        curve_index = i;
        if(name[i].substr(0,5) == "CURVE"){break;}
      }
    }
    int ascii_index = name.size()-1;
    output.push_back({parameter_index, curve_index, ascii_index});
  }else{
    //In this case it's LAS 3.0 and can have multiple log data sections
    //This will loop through in reverse to get the log sections and their associates
    for(std::size_t i=name.size()-1; i>=0; i--){
      if(name[i].substr(0,8)=="LOG_DATA"){
        int log_parameter_index = -1;
        int log_definition_index = -1;
        std::string log_definition_name = association[i];
        std::string n_suffix = "";
        if(number[i]!=0){n_suffix = "[" + std::to_string(number[i]) + "]";}
        if(log_definition_name == ""){log_definition_name = "LOG_DEFINITION" + n_suffix;}
        std::string log_parameter_name = "LOG_PARAMETER" + n_suffix;
        for(std::size_t i=0; i<name.size(); i++){
          if(name[i]==log_definition_name){
            log_definition_index = i;
          }else if(name[i]==log_parameter_name){
            log_parameter_index = i;
          }
        }
        int log_data_index = name.size()-1;
        output.push_back({log_parameter_index, log_definition_index, log_data_index});
      }else{
        break;
      }
    }
  }
  return(output);
}

//This method is basically just for troubleshooting
Rcpp::List LasMap::dataframe(){
  Rcpp::List df;
  df.push_back(name);
  df.push_back(root_name);
  df.push_back(number);
  df.push_back(association);
  df.push_back(start);
  df.push_back(end);
  df.attr("names") = Rcpp::CharacterVector::create("NAME", "ROOT_NAME", "NUMBER", "ASSOCIATION", "START", "END");
  df.attr("class") = Rcpp::CharacterVector::create("data.frame");
  return(df);
}