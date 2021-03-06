// [[Rcpp::depends(hesim)]]
// [[Rcpp::depends(RcppArmadillo)]]

#include <RcppArmadillo.h>
#include <hesim.h>
using namespace Rcpp;

// Random Survival Times
// [[Rcpp::export]]
double rsurvC(double location, double anc1, std::string dist, double anc2 = 0.0) {
  double surv = 0.0;
  if (dist == "exponential" || dist == "exp"){
    double rate = exp(location);
    surv = R::rexp(1/rate);
  }
  else if (dist == "weibull"){
    double shape = exp(anc1);
    double scale = exp(location);
    surv = R::rweibull(shape, scale);
  }
  else if (dist == "gompertz"){
    double shape = anc1;
    double rate = exp(location);
    hesim::stats::gompertz gomp(shape, rate);
    surv = gomp.random();
  }
  else if (dist == "lnorm"){
    double meanlog = location;
    double sdlog = exp(anc1);
    surv = R::rlnorm(meanlog, sdlog);
  }
  else if (dist == "gamma"){
    double rate = exp(location);
    double shape = exp(anc1);
    surv = R::rgamma(shape, 1/rate);
  }
  else if (dist == "llogis"){
    double scale = exp(location);
    double shape = exp(anc1);
    hesim::stats::loglogistic llogis(shape, scale);
    surv = llogis.random();
  }
  else if (dist == "gengamma"){
    double mu = location;
    double sigma = exp(anc1);
    double Q = anc2;
    hesim::stats::gengamma gengamma(mu, sigma, Q);
    surv = gengamma.random();
  }
  return surv;
}

// Sample Conditional Distribution of y given x
double rbvcnormC(double &x, double &y_mean, double &x_mean, 
                    double &y_var, double &x_var, double &cor){
  double mean = y_mean + cor * sqrt(y_var/x_var) * (x - x_mean);
  double sd = sqrt((1 - pow(cor, 2)) * y_var);
  double sample = hesim::stats::rtruncnorm(mean, sd, 0, 100);
  return(sample);
}



