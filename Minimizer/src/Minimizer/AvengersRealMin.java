/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package Minimizer;

import java.util.HashMap;

/**
 *
 * @author Wolfwood
 */
public class AvengersRealMin implements RealMin {

    double[] values = new double[3];
    private static final String[] names = new String[]{"jokes", "fights", "suspense"};
    private static final HashMap<String, Integer> indexes = new HashMap< String, Integer>();

    static {
        for (int i = 0; i < names.length; ++i) {
            indexes.put(names[i], i);
        }
    }
    public static final int IJOKES = indexes.get("jokes");
    public static final int IFIGHTS = indexes.get("fights");
    public static final int ISUSPENSE = indexes.get("suspense");

    @Override
    public int getRealParameterSize() {
        return names.length;
    }

    @Override
    public String getRealParameterName(int i) {
        return names[i];
    }

    @Override
    public int getRealParameterIndex(String name) {
        return indexes.get(name);
    }

    @Override
    public double getRealParameterValue(int index) {
        return values[index];
    }

    @Override
    public void setRealParameterValue(int index, double value) {
        values[index] = value;
    }

    public AvengersRealMin() {
    }

    public AvengersRealMin(AvengersRealMin copy) {
        System.arraycopy(copy.values, 0, copy.values, 0, values.length);
    }

    @Override
    public RealMin copy() {
        return new AvengersRealMin(this);
    }

    @Override
    public double getValue() {
        double jokes = values[IJOKES];
        double fights = values[IFIGHTS];
        double suspense = values[ISUSPENSE];
        double d2 = 0;
        boolean constrained = true;
        if (constrained) {
            if (jokes < 0) {
                d2 += jokes * jokes;
                jokes = 0;
            }
            if (fights < 0) {
                d2 += fights * fights;
                fights = 0;
            }
            if (suspense < 0) {
                d2 += suspense * suspense;
                suspense = 0;
            }
            if (suspense > 4) {
                d2 += (suspense - 4) * (suspense - 4);
                suspense = 4;
            }
            if (fights > 3) {
                d2 += (fights - 3) * (fights - 3);
                fights = 3;
            }
        }
        return Math.pow(jokes + fights + suspense - 7, 2) + Math.pow(fights - 4, 2) + Math.pow(suspense - 8, 2) + d2;
    }
}
