#include <Rcpp.h>

//' @name trim_ws
//' @title trim_ws
//' @description A function to trim the whitespace from the ends of a string
//' @param s A string
//' @return A string with no whitespace
// [[Rcpp::export]]
std::string trim_ws(std::string s){ 
  s.erase(0, s.find_first_not_of(" \t"));
  s.erase(s.find_last_not_of(" \t") + 1);
  return s;
}

//' @name parse_header
//' @title parse_header
//' @description A function to parse header lines from a LAS file.
//' @param lines A vector of strings containing header data.
//' @param first_line An integer indicating the index of the first line in lines containing header data.
//' @param last_line An integer indicating the last index of lines containing header data.
//' @return A dataframe containing the header data.
// [[Rcpp::export]]
Rcpp::DataFrame parse_header(std::vector<std::string> const &lines, int first_line = 0, int last_line = -1){
  //Sets the last line to the end of the vector if the line is not given
  if(last_line<0){last_line = lines.size();}
  
  //Vectors to hold the header data
  std::size_t nrow = last_line - first_line + 1;
  std::vector<std::vector<std::string> > header(6, std::vector<std::string>(nrow));
  std::size_t n = 0;
  
  if(first_line>=0){
    for(int j=first_line; j<=last_line; j++){
      std::string line = lines[j];
      //These are the delimiter positions for the mnemonic, unit, value, comment, and format
      int d1=0; int d2=0; int d3=0; int d4=0; int d5=0; int d6=0;
      int numchar = line.size(); //Gets the number of characters in the line
      
      //Checks if the line has data
      bool inside_brackets=false;
      for(int i = 0; i < numchar; i++){
        if((line[i]=='.') & (d1==0)){d1=i;}                      //Gets mnemonic delimiter
        if((line[i]==' ') & (d1!=0) & (d2==0)){d2=i;}            //Gets units delimiter
        if((line[i]==':') & (inside_brackets==false)){d3=i;}     //Gets value delimiter
        if(line[i]=='{'){d4=i; inside_brackets=true;}            //Gets comment delimiter
        if(line[i]=='}'){d5=i; inside_brackets=false;}           //Gets format delimiter (may not exist)
        if((line[i]=='|') & (d3!=0) & (inside_brackets==false)){d6=i;} //Gets the association delimiter
      }
      
      //Checks if enough delimiters were found to parse a line.
      if((d1!=0) & (d2!=0) & (d3!=0) & (d1<d2) & (d2<d3) & (d3<numchar)){
        header[0][n] = trim_ws(line.substr(0,d1));               //Mnemonic
        header[1][n] = trim_ws(line.substr(d1+1,(d2-1)-d1));     //Units
        header[2][n] = trim_ws(line.substr(d2+1,(d3-1)-d2));     //Value
        
        //Gets the comment.  May stop at a { or the end of the line.
        if(d4>d3){
          header[3][n] = trim_ws(line.substr(d3+1,(d4-1)-d3));
        }else if((d6>d3) & (d4<d3)){
          header[3][n] = trim_ws(line.substr(d3+1,(d6-1)-d3));
        }else{
          header[3][n] = trim_ws(line.substr(d3+1,line.size()-d3));
        }
        
        //Gets the format if it exists
        if((d4>0) & (d5>(d4+1))){
          header[4][n] = trim_ws(line.substr(d4+1,d5-(d4+1)));
        }
        
        //Gets the association if it exists
        if((d6>0) & (d6>d5)){
          header[5][n] = trim_ws(line.substr(d6+1,line.size()-d6));
        }
        n++;
      }
    }
  }
  //Shrinks the output if any unparseable lines were found
  if(n<nrow){for(std::size_t i=0; i<6; i++){header[i].resize(n);}}
  
  Rcpp::DataFrame headerDF = Rcpp::wrap(header);
  headerDF.attr("names") = Rcpp::CharacterVector::create("mnemonic", "unit", "value", "comment", "format", "association");
  headerDF.attr("class") = Rcpp::CharacterVector::create("data.table", "data.frame");
  return(headerDF);
}

//' @name get_header_double
//' @title get_header_double
//' @description A function to get a double value from a header
//' @param header A DataFrame containing header data
//' @param mnem A string containing the desired mnemonic
//' @param default_val A double to be used if the value can't be found or converted
//' @return A double containing the value
//' @export
// [[Rcpp::export]]
double get_header_double(Rcpp::DataFrame &header, std::string mnem, double default_val){
  Rcpp::CharacterVector mnem_col = header["mnemonic"];
  Rcpp::CharacterVector val_col = header["value"];
  int n = mnem_col.size();
  std::string val_str;
  for(int i=0; i<n; i++){
    if(mnem_col[i]==mnem){
      val_str=val_col[i];
      break;
    }
  }
  double val;
  try {val = std::stod(val_str);} catch (...) {val = default_val;}
  return(val);
}

//' @name get_header_int
//' @title get_header_int
//' @description A function to get an int value from a header
//' @param header A DataFrame containing header data
//' @param mnem A string containing the desired mnemonic
//' @param default_val An int to be used if the value can't be found or converted
//' @return An int containing the value
//' @export
// [[Rcpp::export]]
int get_header_int(Rcpp::DataFrame &header, std::string mnem, int default_val){
  Rcpp::CharacterVector mnem_col = header["mnemonic"];
  Rcpp::CharacterVector val_col = header["value"];
  int n = mnem_col.size();
  std::string val_str;
  for(int i=0; i<n; i++){
    if(mnem_col[i]==mnem){
      val_str=val_col[i];
      break;
    }
  }
  int val;
  try {val = std::stoi(val_str);} catch (...) {val = default_val;}
  return(val);
}

//' @name get_header_string
//' @title get_header_string
//' @description A function to get a string value from a header
//' @param header A DataFrame containing header data
//' @param mnem A string containing the desired mnemonic
//' @return A string containing the value
//' @export
// [[Rcpp::export]]
std::string get_header_string(Rcpp::DataFrame &header, std::string mnem){
  Rcpp::CharacterVector mnem_col = header["mnemonic"];
  Rcpp::CharacterVector val_col = header["value"];
  int n = mnem_col.size();
  std::string val;
  for(int i=0; i<n; i++){
    if(mnem_col[i]==mnem){
      val = val_col[i];
      break;
    }
  }
  return(val);
}