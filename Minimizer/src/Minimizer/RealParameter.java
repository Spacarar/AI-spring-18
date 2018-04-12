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
public interface RealParameter {
    int getRealParameterSize();
    String getRealParameterName(int index);
    int getRealParameterIndex(String name);
    double getRealParameterValue(int index);
    void setRealParameterValue(int index, double value);
    
    default double[] getRealParameters(double[] x){
        int dim=getRealParameterSize();
        //check safety of x
        if(x==null){
            x=new double[dim];
        }
        //fill x with the values we have
        for(int i=0;i<dim;i++){
            x[i]=getRealParameterValue(i);
        }
        return x;
    }
    
    default void setRealParameters(double[] x){
        int dim = getRealParameterSize();
        for(int i=0;i<dim;i++){
            setRealParameterValue(i,x[i]);
        }
    }
}
