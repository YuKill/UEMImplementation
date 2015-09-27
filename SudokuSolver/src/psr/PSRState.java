/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package psr;

import java.util.ArrayList;
import java.util.HashMap;

/**
 *
 * @author yukiosir
 */
public class PSRState {
    private HashMap<String, ArrayList<Integer>> domain = new HashMap<>();

    public PSRState() {
    
    }

    public PSRState(HashMap<String, ArrayList<Integer>> domain) {
        this.domain = domain;
    }

    public PSRState(PSRState psrState) {
        for (HashMap.Entry<String, ArrayList<Integer>> entry : 
                psrState.domain.entrySet()) {
            domain.put(entry.getKey(), new ArrayList<>(entry.getValue()));
        }
    }
    
    public ArrayList<Integer> getDomain(String key) {
        return domain.get(key);
    }
    
    public boolean printState() {
        for (HashMap.Entry<String, ArrayList<Integer>> entry : domain.entrySet()) {
            PSR.logDebug(entry.getKey() + " - " + entry.getValue() + " -> " + entry.getValue().size());
        }
        return true;
    }
}
