#ifndef CURVE_FUNCTIONS_H
#define CURVE_FUNCTIONS_H

Rcpp::DataFrame parse_curves(std::vector<std::string> const &lines,
                             Rcpp::CharacterVector const &curve_names, 
                             Rcpp::CharacterVector const &format, 
                             std::string delim = " ", 
                             std::string null_str = "-999.25", 
                             int first_line = 0, 
                             int last_line = -1);

#endif
