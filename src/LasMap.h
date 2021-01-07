#ifndef LASMAP_H
#define LASMAP_H
#include <Rcpp.h>
#include <string>
#include <vector>

class LasMap {
private:
  std::vector<std::string> name;
  std::vector<std::string> root_name;
  std::vector<int> number;
  std::vector<std::string> association;
  std::vector<int> start;
  std::vector<int> end;
  std::size_t n=0;
public:
  LasMap(){};
  int start_index(int i){return(start[i]);}
  int end_index(int i){return(end[i]);}
  std::size_t size(){return(n);}
  std::vector<std::string> get_name(){return(name);}
  std::vector<std::string> get_root_name(){return(root_name);}
  std::vector<int> get_number(){return(number);}
  std::vector<std::string> get_association(){return(association);}
  std::vector<int> get_start(){return(start);}
  std::vector<int> get_end(){return(end);}
  std::vector<std::vector<int> > section_indices(std::string section);
  std::vector<std::string> get_user_sections();
  void push_back(std::string section_string, int start_index, int end_index);
  Rcpp::DataFrame dataframe();
};


#endif