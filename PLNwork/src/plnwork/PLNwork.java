/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package plnwork;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import org.apache.pdfbox.cos.COSDocument;
import org.apache.pdfbox.pdfparser.PDFParser;
import org.apache.pdfbox.pdmodel.PDDocument;
import org.apache.pdfbox.util.PDFTextStripper;

/**
 *
 * @author yukiosir
 */
public class PLNwork {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        PrintWriter writer = null;
        String pdf = pdfToText("pdf/4.pdf");
        String pdfLine = pdf.replaceAll("[^A-Za-z0-9\\.]", " ")
                .replaceAll("\\s+", " ");
        ArrayList<String> st = getStatements(pdfLine);
        ArrayList<String> r = new ArrayList<>();
        System.out.println(st.size());
//        System.out.println(pdf);
        r.addAll(findAuthors(pdf));
//        r.addAll(findAddress(st));
//        r.addAll(findObjective(st));
//        r.addAll(findProblem(st));
//        r.addAll(findMethod(st));
//        r.addAll(findContribution(st));
//        r.addAll(extractReferences(pdf));

        try {
            writer = new PrintWriter("4-out.txt", "UTF-8");
            for (int i = 0; i < r.size(); i++) {
                writer.print(r.get(i));
                if (i != r.size() - 1) {
                    writer.println(";;");
                }
            }
            writer.close();
        } catch (FileNotFoundException ex) {
            ex.printStackTrace();
        } catch (UnsupportedEncodingException ex) {
            ex.printStackTrace();
        } finally {
            writer.close();
        }
    }

    public static String pdfToText(String path) {
        PDFTextStripper pdfStripper = null;
        PDDocument pdDoc = null;
        COSDocument cosDoc = null;
        String text = null;

        File file = new File(path);
        try {
            PDFParser parser = new PDFParser(new FileInputStream(file));
            parser.parse();
            cosDoc = parser.getDocument();
            pdfStripper = new PDFTextStripper();
            pdDoc = new PDDocument(cosDoc);

            text = pdfStripper.getText(pdDoc);
        } catch (IOException e) {
            e.printStackTrace();
        }
        return text;
    }

    public static ArrayList<String> getStatements(String pdf) {
        ArrayList<String> st = new ArrayList();

        int start = 0;
        for (int i = 0; i < pdf.length(); i++) {
            if (pdf.charAt(i) == '.') {
                st.add(pdf.substring(start, i));
                start = i + 1;
            }
        }
        st.add(pdf.substring(start, pdf.length() - 1));

        return st;
    }

    public static ArrayList<String> findAuthors(String txt) {
        String namePattern = "([A-Z][a-z]+)(\\s(([A-Z](\\.|[a-z]*))|(de|o)\\s[A-Z](\\.|[a-z]*)))*";
        String pattern = "\\A" + namePattern
                + "((\\sand|,)\\s" + namePattern + ")*\\z";

        String pdf[] = txt.split("\n");

        ArrayList<String> r = new ArrayList<>();

        Pattern p = Pattern.compile(pattern);
        for (String s : pdf) {
            s = s.replaceAll("[^A-Za-z0-9]", " ").replaceAll("\\s+", " ");
            Matcher match = p.matcher(s);
            while (match.find() && match.end() - match.start() > 8) {
                System.out.println("------------------------");
                System.out.println(s);
                r.add(s);
            }
        }

        return r;
    }

    public static ArrayList<String> findAddress(ArrayList<String> pdf) {
        String universityPattern = "(.*)[Uu]niversity(.*)";
        String departmentPattern = "(.*)[Dd]epartment(.*)";
        String corporationPattern = "(.*)[Cc]orporation(.*)";

        ArrayList<String> r = new ArrayList<>();

        String pattern = "(" + universityPattern + "|" + departmentPattern + "|"
                + corporationPattern + ")";

        Pattern p = Pattern.compile(pattern);

        for (String s : pdf) {
//            System.out.println(s);
            Matcher match = p.matcher(s);
            if (match.find()) {
                System.out.println("------------------------");
                r.add(s);
                System.out.println(s);
            }
        }
        return r;
    }

    public static ArrayList<String> findObjective(ArrayList<String> pdf) {
        String patternOne = "(.*)[Pp]urpose(.*)";
        String patternTwo = "(.*)[Oo]bjective(.*)";

        ArrayList<String> r = new ArrayList<>();

        String pattern = "(" + patternOne + "|" + patternTwo + ")";

        Pattern p = Pattern.compile(pattern);

        for (String s : pdf) {
            Matcher match = p.matcher(s);
            if (match.find()) {
                System.out.println("------------------------");
                r.add(s);
                System.out.println(s);
            }
        }
        return r;
    }

    public static ArrayList<String> findProblem(ArrayList<String> pdf) {
        String patternOne = "(.*)[Pp]roblem(.*)";
        String patternTwo = "(.*)[Oo]vercomes(.*)";
        String patternThree = "(.*)[Ss]olve(.*)";
        String patternFour = "(.*)[Ss]olution(.*)";

        ArrayList<String> r = new ArrayList<>();

        String pattern = "(" + patternOne + "|" + patternTwo + "|"
                + patternThree + "|" + patternFour + ")";

        Pattern p = Pattern.compile(pattern);

        for (String s : pdf) {
            Matcher match = p.matcher(s);
            if (match.find()) {
                System.out.println("------------------------");
                r.add(s);
                System.out.println(s);
            }
        }
        return r;
    }

    public static ArrayList<String> findMethod(ArrayList<String> pdf) {
        String patternOne = "(.*)[Mm]ethodology(.*)";
        String patternTwo = "(.*)[Aa]nalisys(.*)";
        String patternThree = "(.*)[Aa]nalise(.*)";
        String patternFour = "(.*)[Cc]ontent analisys(.*)";

        ArrayList<String> r = new ArrayList<>();

        String pattern = "(" + patternOne + "|" + patternTwo + "|"
                + patternThree + "|" + patternFour + ")";

        Pattern p = Pattern.compile(pattern);

        for (String s : pdf) {
            Matcher match = p.matcher(s);
            if (match.find()) {
                System.out.println("------------------------");
                r.add(s);
                System.out.println(s);
            }
        }
        return r;
    }

    public static ArrayList<String> findContribution(ArrayList<String> pdf) {
        String patternOne = "(.*)[Cc]ontributes to(.*)";
        String patternTwo = "(.*)[Hh]elps(.*)";
        String patternThree = "(.*)[Ss]olves(.*)";

        ArrayList<String> r = new ArrayList<>();

        String pattern = "(" + patternOne + "|" + patternTwo + "|"
                + patternThree + ")";

        Pattern p = Pattern.compile(pattern);

        for (String s : pdf) {
            Matcher match = p.matcher(s);
            if (match.find()) {
                System.out.println("------------------------");
                r.add(s);
                System.out.println(s);
            }
        }
        return r;
    }

    public static ArrayList<String> extractReferences(String pdf) {
        String patternOne = "(.*)\\([0-9]*\\),(\\s*)“(.*)”,.*";
        String patternTwo = "(.*)\\([0-9]*\\),(\\s*)“(.*)\\n(.*)”,(.*)\\.";

        ArrayList<String> r = new ArrayList<>();

        String pattern = "(" + patternOne + "|" + patternTwo + ")";

        Pattern p = Pattern.compile(pattern);
//        System.out.println(pdf);
        Matcher match = p.matcher(pdf);
        while (match.find()) {
            System.out.println("------------------------");
            r.add(pdf.substring(match.start(), match.end()));
            System.out.println(pdf.substring(match.start(), match.end()));
        }

        return r;
    }
}
