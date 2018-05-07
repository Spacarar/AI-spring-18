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
public class LineRealMin implements RealMin {
    RealMin problem;
    double eps;
    int dim;
    double[] x;
    double[] grad;
    double t;
    
    LineRealMin(RealMin _problem,double _eps){
        problem=_problem;
        eps=_eps;
        dim = problem.getRealParameterSize();
        x=new double[dim];
        grad = new double[dim];
    }
    
    LineRealMin(LineRealMin copy){
        System.arraycopy(copy.x, 0, x, 0, dim);
        System.arraycopy(copy.grad, 0, grad, 0, dim);
        t=copy.t;
    }
    
    void setup(){
        problem.getRealParameters(x);
        problem.getGradient(grad, eps);
        t=0;
    }
    @Override
    public LineRealMin copy(){
        return new LineRealMin(this);
    }
    
     @Override
    public double getValue() {
        for (int i = 0; i < dim; ++i) {
            problem.setRealParameterValue(i, x[i] + t * grad[i]);
        }
        double y = problem.getValue();
        return y;
    }

    @Override
    public int getRealParameterSize() {
        return 1;
    }

    @Override
    public String getRealParameterName(int index) {
        if (index == 0) {
            return "t";
        }
        throw new IndexOutOfBoundsException();
    }

    @Override
    public int getRealParameterIndex(String name) {
        if (name.equals("t")) {
            return 0;
        }
        throw new IndexOutOfBoundsException();
    }

    @Override
    public double getRealParameterValue(int index) {
        if (index == 0) {
            return t;
        }
        throw new IndexOutOfBoundsException();
    }

    @Override
    public void setRealParameterValue(int index, double value) {
        if (index == 0) {
            t = value;
            return;
        }
        throw new IndexOutOfBoundsException();
    }

    void config(GoldenSectionMinimizer minimizer) {
        problem.setRealParameterValue(0, 0);
        double y0 = problem.getValue();
        problem.setRealParameterValue(0, eps);
        double yp = problem.getValue();
        problem.setRealParameterValue(0, -eps);
        double ym = problem.getValue();
        double dy = (yp - ym) / (2 * eps);
        double dy2 = (-2 * y0 + yp + ym) / (eps * eps);
        double dx = Math.max(Math.abs(dy), Math.abs(dy / dy2));
        minimizer.a = -dx / GoldenSectionMinimizer.invphi;
        minimizer.b = 0;
}
    
    
    
}
