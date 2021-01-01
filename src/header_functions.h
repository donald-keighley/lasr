#ifndef HEADER_FUNCTIONS_H
#define HEADER_FUNCTIONS_H

std::string trim_ws(std::string s);
Rcpp::DataFrame parse_header(std::vector<std::string> const &lines, int first_line = 0, int last_line = -1);
double get_header_double(Rcpp::DataFrame &header, std::string mnem, double default_val);
int get_header_int(Rcpp::DataFrame &header, std::string mnem, int default_val);
std::string get_header_string(Rcpp::DataFrame &header, std::string mnem);

#endif