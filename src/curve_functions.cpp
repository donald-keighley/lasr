#include <Rcpp.h>

//' @name parse_curves
//' @title parse_curves
//' @description A function to parse delimited curve data from a LAS file.
//' @param lines A vector of strings containing log curve data
//' @param delim A string specifying the delimiter
//' @param curve_names A vector of strings containing the names of the log curves
//' @param format A vector of strings containing the formats of the log curves
//' @param null_val A double indicating what value to replace with NA
//' @param first_line An integer indicating the index of the first line in lines containing log data
//' @param last_line An integer indicating the last index of lines containing log data.
//' @return A dataframe containing the curves
//' @export
// [[Rcpp::export]]
Rcpp::List parse_curves(std::vector<std::string> const &lines,
                             Rcpp::CharacterVector const &curve_names,
                             Rcpp::CharacterVector const &format,
                             std::string delim = " ",
                             double null_val = -999.25,
                             int first_line = 0, 
                             int last_line = -1){
  //Sets the last line to the end of the vector if the line is not given
  if(last_line<0){last_line = lines.size();}
  
  //Handles some special cases of the delimiters
  if(delim == "COMMA"){delim = ",";}else if(delim == "TAB"){delim = "\t";}else{delim = " ";}
  
  //Vector of string vectors to hold results.  Will convert types later.
  int ncol = curve_names.size();
  int nrow = last_line - first_line + 1;
  std::vector<std::vector<std::string> > curves(ncol, std::vector<std::string>(nrow, ""));
  
  //Parses the curves.  Doesn't need to know if it's wrapped, will work either way.
  int row_index=0;
  int col_index=0;
  std::size_t start; std::size_t end; std::string line;
  //If the delimiter is a space, consecutive delimiters are treated as one:
  if(delim == " "){
    for(int line_index = first_line; line_index<=last_line; line_index++){
      start=0; end=0; line=lines[line_index];
      while(end<std::string::npos){
        start=line.find_first_not_of(delim,end);
        end=line.find_first_of(delim,start);
        if(end>start){
          //Handles the fact that quoted fields can contain the delimiter
          if(line.substr(start,1)!="\""){
            curves[col_index][row_index] = line.substr(start,end-start);
          }else{
            end=line.find_first_of("\"",start + 1);
            end=line.find_first_of(delim,end);
            curves[col_index][row_index] = line.substr(start+1,(end-1)-(start+1));
          }
          
        }
        col_index++;
        if(col_index>=ncol){col_index=0; row_index++; break;}
      }
    }
  //If the delimiter is not a space, consecutive delimiters are not treated as one:
  }else{
    for(int line_index = first_line; line_index<=last_line; line_index++){
      start=0; end=0; line=lines[line_index];
      while(end<std::string::npos){
        end=line.find(delim,start);
        if(end>start){
          //Handles the fact that quoted fields can contain the delimiter
          if(line.substr(start,1)!="\""){
            curves[col_index][row_index]=line.substr(start,end-start);
          }else{
            end=line.find_first_of("\"",start + 1);
            end=line.find_first_of(delim,end);
            curves[col_index][row_index] = line.substr(start+1,(end-1)-(start+1));
          }
        }
        start=end+1;
        col_index++;
        if(col_index>=ncol){col_index=0; row_index++; break;}
      }
    }
  }
  
  //If wrapped, there are less rows than lines so they need to be shrunk
  //Also, need to convert to correct type (assumed double unless given)
  Rcpp::List converted_curves(ncol);
  std::string token;
  double token_dbl;
  int token_int;
  std::string col_format;
  for(int ci=0; ci<ncol; ci++){
    col_format = format[ci];
    if((col_format=="")|(col_format[0]=='F')|(col_format[1]=='F')){
      Rcpp::NumericVector curve(row_index, NA_REAL);
      for(int ri = 0; ri<row_index; ri++){
        token = curves[ci][ri];
        if(token != ""){
          token_dbl = std::atof(token.c_str());
          if(token_dbl!=null_val){curve[ri] = token_dbl;}
        }
      }
      converted_curves[ci] = curve;
    }else if((col_format[0]=='I')|(col_format[1]=='I')){
      Rcpp::IntegerVector curve(row_index, NA_INTEGER);
      for(int ri = 0; ri<row_index; ri++){
        token = curves[ci][ri];
        if(token != ""){
          token_int = std::atoi(token.c_str());
          if(token_int!=null_val){curve[ri] = token_int;}
        }
      }
      converted_curves[ci] = curve;
    }else{
      Rcpp::CharacterVector curve(row_index, NA_STRING);
      for(int ri = 0; ri<row_index; ri++){
        token = curves[ci][ri];
        if(token != ""){curve[ri] = token;}
      }
      converted_curves[ci] = curve;
    }
  }
  converted_curves.attr("names") = curve_names;
  converted_curves.attr("class") = Rcpp::CharacterVector::create("data.table", "data.frame");
  return(converted_curves);
}