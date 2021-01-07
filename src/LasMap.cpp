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
  std::string name_str = "";
  std::string association_str = "";
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
  n++;
}

//Finds the indices of the curvesets.
//Starts at the last section and works backward so it can stop quickly if it's an A
std::vector<std::vector<int> > LasMap::section_indices(std::string section){
  std::vector<std::vector<int> > indices;
  
  //If section is version or well, get the first section starting with V or W
  if((section == "VERSION") | (section == "WELL")){
    indices.push_back({-1, -1, -1});
    for(std::size_t i=0; i<name.size(); i++){
      if(name[i].substr(0,1) == section.substr(0,1)){
        indices[0][0] = i;
        break;
      }
    }
  }else{
    if(section == "LOG"){
      //The last section in all versions must be a log data section.
      //If in most cases it's an A with an associated P and C section.
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
        indices.push_back({parameter_index, curve_index, ascii_index});
      }
    }
    if(((section=="LOG") && (indices.size()==0))||(section!="LOG")){
      //In this case it's LAS 3.0 and you have to search for multiple data sections
      for(std::size_t i=0; i<name.size(); i++){
        if(name[i].rfind(section + "_DATA", 0) == 0){
          int log_parameter_index = -1;
          int log_definition_index = -1;
          std::string log_definition_name = association[i];
          std::string n_suffix = "";
          if(number[i]!=0){n_suffix = "[" + std::to_string(number[i]) + "]";}
          if(log_definition_name == ""){log_definition_name = section + "_DEFINITION" + n_suffix;}
          std::string log_parameter_name = section + "_PARAMETER" + n_suffix;
          for(std::size_t i=0; i<name.size(); i++){
            if(name[i]==log_definition_name){
              log_definition_index = i;
            }else if(name[i]==log_parameter_name){
              log_parameter_index = i;
            }
          }
          int log_data_index = i;
          indices.push_back({log_parameter_index, log_definition_index, log_data_index});
        }
      }
    }
  }
  return(indices);
}

//Gets the user defined section names
std::vector<std::string> LasMap::get_user_sections(){
  std::string suffix = "_DATA";
  std::string name_val;
  std::vector<std::string> sections;
  for(std::size_t i=0; i<name.size(); i++){
    name_val = name[i];
    std::size_t j = name_val.find(suffix);
    if(j<std::string::npos){
      name_val.erase(j, name_val.length());
      if((name_val!="LOG")&(name_val!="CORE")&(name_val!="INCLINOMETRY")&(name_val!="DRILLING")&(name_val!="TOPS")&(name_val!="TEST")){
        sections.push_back(name_val);
      }
    }
  }
  return(sections);
}

//This method is basically just for troubleshooting
Rcpp::DataFrame LasMap::dataframe(){
  Rcpp::DataFrame df = Rcpp::DataFrame::create(_["NAME"] = name, 
                                               _["ROOT_NAME"] = root_name,
                                               _["NUMBER"] = number,
                                               _["ASSOCIATION"] = association,
                                               _["START"] = start,
                                               _["END"] = end);
  return(df);
}