/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package hospitalbdii.Entidade;

import Util.Convert;
import java.sql.ResultSet;
import java.sql.SQLException;

/**
 *
 * @author yukiosir
 */
public class ResultadoExame {
    public static int nof_fields = 17;

    public int numProtocolo;
    public String codExame;
    public int numItem;
    public int numAtributo;
    public String nomeAtributo;
    public String tipo;
    public int visivel;
    public int informado;
    public int calculado;
    public String formula;
    public String textoValorNormal;
    public String resultadoPadrao;
    public String unidade;
    public int normalidade;
    public String resultado;
    public int numExame;
    public int impresso;

    public static Object fromResultSet(ResultSet rs) {
        ResultadoExame re = new ResultadoExame();
        try {
            re.numProtocolo = rs.getInt(1);
            re.codExame = rs.getString(2);
            re.numItem = rs.getInt(3);
            re.numAtributo = rs.getInt(4);
            re.nomeAtributo = rs.getString(5);
            re.tipo = rs.getString(6);
            re.visivel = rs.getInt(7);
            re.informado = rs.getInt(8);
            re.calculado = rs.getInt(9);
            re.formula = rs.getString(10);
            re.textoValorNormal = rs.getString(11);
            re.resultadoPadrao = rs.getString(12);
            re.unidade = rs.getString(13);
            re.normalidade = rs.getInt(14);
            re.resultado = rs.getString(15);
            re.numExame = rs.getInt(16);
            re.impresso = rs.getInt(17);
        } catch (SQLException ex) {
            ex.printStackTrace();
        }
        return re;
    }

    public String getUpdateString() {
        return " NumProtocolo=" + numProtocolo + ", "
                + " CodExame='" + codExame + "', " 
//                + " NumItem=" + numItem + " "
                + " NumAtributo=" + numAtributo + ", "
                + " NomeAtributo='" + nomeAtributo + "', "
                + " Tipo='" + tipo + "', "
                + " Visivel=" + visivel + ", "
                + " Informado=" + informado + ", "
                + " Calculado=" + calculado + ", "
                + " Formula='" + formula + "', "
                + " TextoValorNormal='" + textoValorNormal + "', "
                + " ResultadoPadrao='" + resultadoPadrao + "', "
                + " Unidade='" + unidade + "', "
                + " Normalidade=" + normalidade + ", "
                + " Resultado='" + resultado + "', "
                + " NumExame=" + numExame + ", "
                + " Impresso=" + impresso;
    }

    public String getInsertString() {
        return numProtocolo + ",'" + codExame + "'," + numItem + "," + numAtributo
                + ",'" + nomeAtributo + "','" + tipo + "'," + visivel + "," + informado
                + "," + calculado + ",'" + formula + "','" + textoValorNormal + "','"
                + resultadoPadrao + "','" + unidade + "'," + normalidade + ",'"
                + resultado + "'," + numExame + "," + impresso;
    }

    public static ResultadoExame getFromFile(String line) {
        ResultadoExame re = new ResultadoExame();
        String fields[] = line.replaceAll("\"", "").replaceAll("\'", "").toUpperCase().split(";");

        re.numProtocolo = Convert.toInt(fields[0]);
        re.codExame = fields[1];
        re.numItem = Convert.toInt(fields[2]);
        re.numAtributo = Convert.toInt(fields[3]);
        re.nomeAtributo = fields[4];
        re.tipo = fields[5];
        re.visivel = Convert.toInt(fields[6]);
        re.informado = Convert.toInt(fields[7]);
        re.calculado = Convert.toInt(fields[8]);
        re.formula = fields[9];
        re.textoValorNormal = fields[10];
        re.resultadoPadrao = fields[11];
        re.unidade = fields[12];
        re.normalidade = Convert.toInt(fields[13]);
        re.resultado = fields[14];
        re.numExame = Convert.toInt(fields[15]);
        re.impresso = Convert.toInt(fields[16]);

        return re;
    }
}
