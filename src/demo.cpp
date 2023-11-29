#include "swzplot.hpp"
#include<functional>
#include<cmath>


// -*-
int main(int argc, char **argv){
    //using Fun = std::function<double(double)>;
    typedef double (*Fun)(double);
    
    auto PI = M_PI;
    auto xdata = swzplot::linspace(-2.0*PI, 2.0*PI, 100);
    swzplot::Vector<double> ydata(xdata.size());
    for(auto i=0; i < xdata.size(); ++i){
        auto x = xdata[i];
        ydata[i] = std::sin(x);
    }
    swzplot::glut::initilalize(argc, argv);
    swzplot::set("lw", 2.0);
    swzplot::set("color", "c");
    swzplot::plot( ydata);
    swzplot::xlabel("x");
    swzplot::ylabel("sin(x)");
    swzplot::title("f(x) = sin(x)");
    swzplot::show();

    // try{
        
    // }catch(const std::exception& err){
    //     std::cerr << err.what() << std::endl;
    // }

    
    return 0;
}