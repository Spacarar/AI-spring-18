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
public class GradientDescentMinimizer implements Minimizer{
    private double eps = 1e-6;
    public double getEps(){
        return eps;
    }
    public void setEps(double value){
        eps=value;
    }
    RealMin problem;
    
    @Override
    public void setProblem(RealMin _problem) {
       problem=_problem;
    }

    @Override
    public double min() {
        double minimum;
        int dim=problem.getRealParameterSize();
        double[] x0=new double[dim];
        double[] x1=new double[dim];
        problem.getRealParameters(x0);
        LineRealMin g = new LineRealMin(problem, eps);
        GoldenSectionMinimizer min1d = new GoldenSectionMinimizer();
        for(;;){
            g.setup();//calculate the gradient of f at x now
            //define g(t)=f(x+t*grad f)
            min1d.setProblem(g);
            min1d.eps=eps;
            g.config(min1d);//use first and second derivitive to estimate [a,b]
            minimum=min1d.min();
            problem.getRealParameters(x1);

            double err=0;
            for(int i=0;i<x0.length;++i){
                //System.out.println("error:"+err);
                err=Math.max(err,Math.abs(x0[i]-x1[i]));
            }
            if(err<=eps)break;
            double[] tmp=x1;
            x1=x0;
            x0=tmp;
        }
        return minimum;
    }
    
}
