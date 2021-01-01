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
public:
  LasMap(){};
  int start_index(int i){return(start[i]);}
  int end_index(int i){return(end[i]);}
  int version_index();
  int well_index();
  std::vector<std::vector<int> > curveset_indices();
  void push_back(std::string section_string, int start_index, int end_index);
  Rcpp::List dataframe();
};

#endif