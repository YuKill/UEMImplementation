/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package Util;

import hospitalbdii.DataBase;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;

/**
 *
 * @author yukiosir
 */
public class Convert {
    
    public static String dToString(Date d, boolean quote, SimpleDateFormat sdf) {
        if (d == null) return "null";
        if (quote) return "'" + sdf.format(d) + "'";
        return sdf.format(d);
    }
    
    public static Date toDate(String s, SimpleDateFormat sdf) {
        try {
            return (s.equals("") || s.equals("null")) ? null : sdf.parse(s);
        } catch (ParseException ex) {
            ex.printStackTrace();
        }
        return null;
    }
    
    public static int toInt(String s) {
        s = s.replaceAll("[^0-9]", "");
        return (s.equals("")) ? 0 : Integer.parseInt(s);
    }
    
    public static double toDouble(String s) {
        s = s.replace(',', '.').replaceAll("[^0-9\\.]", "");
        return (s.equals("")) ? 0 : Double.parseDouble(s);
    }
}
