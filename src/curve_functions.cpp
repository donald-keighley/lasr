#include <Rcpp.h>

//' @name parse_curves
//' @title parse_curves
//' @description A function to parse delimited curve data from a LAS file.
//' @param lines A vector of strings containing log curve data
//' @param delim A string specifying the delimiter
//' @param curve_names A vector of strings containing the names of the log curves
//' @param null_val A double indicating what value to replace with NA
//' @param first_line An integer indicating the index of the first line in lines containing log data
//' @param last_line An integer indicating the last index of lines containing log data.
//' @return A dataframe containing the curves
// [[Rcpp::export]]
Rcpp::DataFrame parse_curves(std::vector<std::string> const &lines,
                             Rcpp::CharacterVector const &curve_names,
                             std::string delim = " ",
                             double null_val = -999.25,
                             int first_line = 0, 
                             int last_line = -1){
  //Sets the last line to the end of the vector if the line is not given
  if(last_line<0){last_line = lines.size();}
  
  //Handles some special cases of the delimiters
  if(delim == "COMMA"){
    delim = ",";
  }else if(delim == "TAB"){
    delim = "\t";
  }else{
    delim = " ";
  }
  
  //Vector of Rcpp NumericVectors to contain the curve data.  Does this so we can use R NA values.
  int ncol = curve_names.size();
  int nrow = last_line - first_line;
  std::vector<Rcpp::NumericVector> curves;
  for(int cols=0; cols<ncol; cols++){
    Rcpp::NumericVector curve(nrow, NA_REAL);
    curves.emplace_back(curve);
  }
  
  //Parses the curves.  Doesn't need to know if it's wrapped, will work either way.
  int row_index=0;
  int col_index=0;
  std::size_t start; std::size_t end; double token; std::string line;
  //If the delimiter is a space, consecutive delimiters are treated as one:
  if(delim == " "){
    for(int line_index = first_line; line_index<last_line; line_index++){
      start=0; end=0; token = null_val; line=lines[line_index];
      while(end<std::string::npos){
        start=line.find_first_not_of(delim,end);                 //Gets the position of the first non-delimiter character
        end=line.find_first_of(delim,start);                     //Gets the position of the first delimiter character
        token = std::atof(line.substr(start,end-start).c_str()); //Gets the value
        if(token!=null_val){curves[col_index][row_index]=token;} //Records the value if not null
        col_index++;
        if(col_index>=ncol){col_index=0; row_index++; break;}
      }
    }
  //If the delimiter is not a space, consecutive delimiters are not treated as one:
  }else{
    for(int line_index = first_line; line_index<last_line; line_index++){
      start=0; end=0; std::string token_str = ""; token = null_val; line=lines[line_index];
      while(end<std::string::npos){
        start=end+1;                               //Sets the position of the start to the last end
        end=line.find(delim,start);                //Finds the position of the next delimiter
        token_str = line.substr(start,end-start);  //Gets the token string
        if(token_str != ""){
          token = std::atof(token_str.c_str());                    //Gets the value
          if(token!=null_val){curves[col_index][row_index]=token;} //Records the value if not null
        }
        col_index++;
        if(col_index>=ncol){col_index=0; row_index++; break;}
      }
    }
  }
  //If wrapped, there are less rows than lines so they need to be shrunk
  int lines_to_delete = nrow-row_index;
  if(lines_to_delete>0){
    for(int ci=0; ci<ncol; ci++){
      curves[ci].erase(curves[ci].end() - lines_to_delete, curves[ci].end());
    }
  }
  Rcpp::DataFrame curvesDF = Rcpp::wrap(curves);
  curvesDF.attr("names") = curve_names;
  curvesDF.attr("class") = Rcpp::CharacterVector::create("data.table", "data.frame");
  return(curvesDF);
}