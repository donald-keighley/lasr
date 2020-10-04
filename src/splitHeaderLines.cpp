#include <Rcpp.h>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <algorithm>

using namespace Rcpp;
using namespace std;

//' @name trim_ws
//' @title trim_ws
//' @description Removes leading and trailing spaces from a string
//' @param s A single string
//' @return A string with no leading or trailing spaces
std::string trim_ws(std::string s)
{
  s.erase(0, s.find_first_not_of(" "));
  s.erase(s.find_last_not_of(" ") + 1);
  return s;
}

//' @name to_upper
//' @title to_upper
//' @description Converts a string to upper case
//' @param str A single string
//' @return An upper case string
std::string to_upper(std::string str)
{
  std::transform(str.begin(), str.end(),str.begin(), ::toupper);
  return str;
}

//' @name splitHeaderLines
//' @title splitHeaderLines
//' @description Splits lines from an LAS header
//' @param lines Vector of LAS header strings
//' @return A data.frame of parsed header values
//' @export
//' @useDynLib lasr
//' @importFrom Rcpp sourceCpp
// [[Rcpp::export]]
List splitHeaderLines(std::vector<std::string> lines) {
  int headerLines=0;
  std::string firstchar;
  std::string sect="";
  std::string::size_type log_data_start=0;
  
  //Finds the index of the line where the log data starts
  int nlines=lines.size();
  for(int j = 0; j<nlines; j++){
    firstchar=(lines[j].substr(0,1));
    if((lines[j].substr(0,2))=="~A"){
      for(int v = j+1; v<nlines; v++){
        if((lines[v].substr(0,1)!="#")&(lines[v].substr(0,1)!="")){
          log_data_start=v;
          break;
        }
      }
      break;
    }
    if((firstchar!="~") & (firstchar!="#") & (firstchar!="")){headerLines++;}
  }

  //Creates a vector of vectors the size of the header
  std::vector<std::vector<std::string>> parts(5, std::vector<std::string>(headerLines,""));
  int k = 0;
  
  //Sets the null value in case it's not found
  double nullVal=-999.25;
  
  //Creates the curve name vector
  std::vector<std::string> curveNames;
  
  //Loops through and parses the header
  for(int j = 0; j<nlines; j++){
    int d1=0; int d2=0; int d3=0;
    std::string line = lines[j];
    int numchar = line.size();
    std::string firstOne = line.substr(0,1);
    std::string firstTwo = line.substr(0,2);
    if((firstOne!="~") & (firstOne!="#") & (firstOne!="") & (firstTwo!="..")){
      for(int i = 0; i < numchar; i++){
        if((line[i]=='.') & (d1==0)){d1=i;}
        if((line[i]==' ') & (d1!=0) & (d2==0)){d2=i;}
        if(line[i]==':'){d3=i;}
      }
      if((d1!=0) & (d2!=0) & (d3!=0) & (d1<d2) & (d2<d3) & (d3<numchar) & (sect!="")){
        parts[0][k]=sect;
        std::string mnem = trim_ws(line.substr(0,d1));
        parts[1][k]=mnem;
        parts[2][k]=trim_ws(line.substr(d1+1,(d2-1)-d1));
        std::string val=trim_ws(line.substr(d2+1,(d3-1)-d2));
        parts[3][k]=val;
        parts[4][k]=trim_ws(line.substr(d3+1,line.size()-d3));
        //Gets the null value
        if((sect=="W") & (to_upper(mnem)=="NULL")){nullVal=std::atof(val.c_str());}
        //Gets the curve names
        if(sect=="C"){curveNames.push_back(mnem);}
        k++;
      }
    }else if((firstOne=="~")|(firstTwo=="..")){
      sect=to_upper(line.substr(1,1));
      if(((sect!="C")&(sect!="P")&(sect!="W")&(sect!="V"))|(to_upper(line.substr(1,2))=="CO")){
        sect="";
      }
    }
  }
  
  //Loops through the header again to resize it if necessary, makes DataFrame
  DataFrame header;
  if(k<headerLines){
    std::vector<std::vector<std::string>> parts_shrink(5, std::vector<std::string>(k,""));
    for(int j = 0; j<k; j++){
      parts_shrink[0][j]=parts[0][j];
      parts_shrink[1][j]=parts[1][j];
      parts_shrink[2][j]=parts[2][j];
      parts_shrink[3][j]=parts[3][j];
      parts_shrink[4][j]=parts[4][j];
    }
    parts.clear();
    header=wrap(parts_shrink);
    parts_shrink.clear();
  }else{
    header=wrap(parts);
    parts.clear();
  }
  
  //Adds names to DataFrame
  header.attr("names")  = CharacterVector::create("SECT", "MNEM", "UNIT","VALUE", "COMMENT");
  
  //Creates the log data vector
  int ncol = curveNames.size();
  int nrow = nlines-log_data_start;
  std::vector<std::vector<double>> strVect(ncol, std::vector<double>(nrow,NA_REAL));
  
  //Creates the curve delimiter
  std::string delim=" ,\t\n\r";
  int l=0;
  int m=0;
  k=0;
  for(int i = log_data_start; i<nlines; i++){
    int start=0;
    int end=0;
    double token;
    std::string line=lines[i];
    int strLen = line.size();
    while(end<=strLen){
      start=line.find_first_not_of(delim,end); //Gets the position of the first non-delimiter character
      end=line.find_first_of(delim,start);     //Gets the position of the first delimiter character
      if(start!=end){
        token = std::atof(line.substr(start,end-start).c_str()); //Gets the value
        if(token!=nullVal){strVect[k][m]=token;}         //Records the value if not null
        k++;
        if(k>=ncol){k=0; m++; break;}
      }else{
        break;
      }
    }
    l++;
  }
  
  //Shrinks the curve data in the case that it's wrapped
  DataFrame df;
  if(m<nrow){
    std::vector<std::vector<double>> strVect_shrink(ncol, std::vector<double>(m,NA_REAL));
    for(int r = 0; r<m; r++){
      for(int c = 0; c<ncol; c++){
        strVect_shrink[c][r]=strVect[c][r];
      }
    }
    strVect.clear();
    df=wrap(strVect_shrink);
    strVect_shrink.clear();
  }else{
    df=wrap(strVect);
    strVect.clear();
  }
  df.attr("names")  = curveNames;
  
  List las_out=List::create(header, df);
  las_out.attr("names")  = CharacterVector::create("header", "curves");
  return las_out;
}
