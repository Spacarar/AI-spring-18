/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package Minimizer;

/**
 *
 * @author Wolfwood
 */
public class GoldenSectionMinimizer implements Minimizer{
    
    double a;
    double b;
    double eps;
    
    RealMin problem;
    
    @Override
    public void setProblem(RealMin _problem){
        problem=_problem;
        if(problem.getRealParameterSize()!=1){
            throw new IllegalArgumentException("Golden section search is 1d only");
        }
    }
    public static final double invphi = (Math.sqrt(5.0)-1)/2.0;
    public static final double invphi2 = invphi * invphi;
    
    double f(double x){
        problem.setRealParameterValue(0,x);
        double y=problem.getValue();
        return y;
    }
     @Override
    public double min() {
        double a = Math.min(this.a, this.b);
        double b = Math.max(this.a, this.b);
        double h = b - a;
        if (h <= eps) {
            return f((a + b) / 2.0);
        }
        //n is unused??
        int n = (int) (Math.ceil(Math.log(eps / h) / Math.log(invphi)));
        double c = a + invphi2 * h;
        double d = a + invphi * h;
        double yc = f(c);
        double yd = f(d);
        double ymin = yd;
        for (;;) {
            if (yc < yd) {
                b = d;
                d = c;
                yd = yc;
                h = b - a;
                c = a + invphi2 * h;
                if (h <= eps) {
                    break;
                }
                yc = f(c);
                ymin = yc;
            } else {
                a = c;
                c = d;
                yc = yd;
                h = b - a;
                d = a + invphi * h;
                if (h <= eps) {
                    break;
                }
                yd = f(d);
                ymin = yd;
            }
        }
        return ymin;
}
}
