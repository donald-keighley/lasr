#include <Rcpp.h>
#include <vector>
#include <sstream>
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
  int k=0;
  std::string firstchar;
  std::string sect="";
  std::string::size_type log_data_start=0;
  
  //Finds the index of the line where the log data starts
  int nlines=lines.size();
  for(int j = 0; j<nlines; j++){
    firstchar=(lines[j].substr(0,1));
    if((lines[j].substr(0,2))=="~A"){log_data_start=j; break;}
    if((firstchar!="~") & (firstchar!="#") & (firstchar!="")){k++;}
  }

  //Creates a vector of vectors the size of the header
  std::vector<std::vector<std::string>> parts(5, std::vector<std::string>(k,""));
  k=0;
  std::string wrapped="NO";
  double nullVal=-999.25;
  
  //Creates the curve name vector
  std::vector<std::string> curveNames;
  
  //Loops through and parses the header
  for(int j = 0; j<nlines; j++){
    int d1=0; int d2=0; int d3=0;
    std::string line = lines[j];
    int numchar = line.size();
    if((line.substr(0,1)!="~") & (line.substr(0,1)!="#") & (line.substr(0,1)!="")){
      for(int i = 0; i < numchar; i++){
        if((line[i]=='.') & (d1==0)){d1=i;}
        if((line[i]==' ') & (d1!=0) & (d2==0)){d2=i;}
        if(line[i]==':'){d3=i;}
      }
      if((d1!=0) & (d2!=0) & (d3!=0) & (d1<d2) & (d2<d3) & (d3<numchar)){
        parts[0][k]=sect;
        std::string mnem = trim_ws(line.substr(0,d1));
        parts[1][k]=mnem;
        parts[2][k]=trim_ws(line.substr(d1+1,(d2-1)-d1));
        std::string val=trim_ws(line.substr(d2+1,(d3-1)-d2));
        parts[3][k]=val;
        parts[4][k]=trim_ws(line.substr(d3+1,line.size()-d3));
        //Gets the wrap
        if((sect=="V") & (mnem=="WRAP")){wrapped=val;}
        //Gets the null value
        if((sect=="W") & (mnem=="NULL")){nullVal=std::stod(val);}
        //Gets the curve names
        if(sect=="C"){curveNames.push_back(mnem);}
        k++;
      }
    }else if(line.substr(0,1)=="~"){
      sect=line.substr(1,1);
    }
  }
  DataFrame header=wrap(parts);
  header.attr("names")  = CharacterVector::create("SECT", "MNEM", "UNIT","VALUE", "COMMENT");
  
  //Creates the log data vector
  int ncol = curveNames.size();
  int nrow = nlines-(log_data_start+1);
  std::vector<std::vector<double>> strVect(ncol, std::vector<double>(nrow,NA_REAL));
  
  //Creates the curve delimiter
  std::string delim=" ,\t\n\r";
  int l=0;
  for(int i = (log_data_start+1); i<nlines; i++){
    int start=0;
    int end=0;
    int k=0;
    double token;
    std::string line=lines[i];
    int strLen = line.size();
    while(end<(strLen-1)){
      start=line.find_first_not_of(delim,end);
      end=line.find_first_of(delim,start);
      token = std::stod(line.substr(start,end-start));
      if(token!=nullVal){strVect[k][l]=token;}
      k++;
      if(k>=ncol){break;}
    }
    l++;
  }
  DataFrame df=wrap(strVect);
  df.attr("names")  = curveNames;
  
  List las_out=List::create(header, df);
  return las_out;
}
