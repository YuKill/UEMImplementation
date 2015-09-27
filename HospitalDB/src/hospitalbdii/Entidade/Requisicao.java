/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package hospitalbdii.Entidade;

import Util.Convert;
import hospitalbdii.DataBase;
import java.util.Date;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.text.ParseException;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author yukiosir
 */
public class Requisicao {
    public static int nof_fields = 16;

    public int numPaciente;
    public int numProtocolo = 0;
    public Date dataRequisicao;
    public String sexo;
    public int idade;
    public String idadeInformada;
    public int codGrupoConvenio;
    public int codSeguradoraConvenio;
    public double total;
    public double desconto;
    public double valPago;
    public int idPosto;
    public double ysnOrcamento;
    public int codMedico;
    public int campoVariavel2;
    public double descontoPercent;

    public static Object fromResultSet(ResultSet rs) {
        Requisicao req = new Requisicao();
        try {
            req.numPaciente = rs.getInt(1);
            req.numProtocolo = rs.getInt(2);
            req.dataRequisicao = rs.getDate(3);
            req.sexo = rs.getString(4);
            req.idade = rs.getInt(5);
            req.idadeInformada = rs.getString(6);
            req.codGrupoConvenio = rs.getInt(7);
            req.codSeguradoraConvenio = rs.getInt(8);
            req.total = rs.getDouble(9);
            req.desconto = rs.getDouble(10);
            req.valPago = rs.getDouble(11);
            req.idPosto = rs.getInt(12);
            req.ysnOrcamento = rs.getDouble(13);
            req.codMedico = rs.getInt(14);
            req.campoVariavel2 = rs.getInt(15);
            req.descontoPercent = rs.getInt(16);
            
        } catch (SQLException ex) {
            ex.printStackTrace();
        } 
        return req;
    }

    public String getUpdateString() {
        return " NumPaciente=" + numPaciente + ", "
                + " DataRequisicao=" + Convert.dToString(dataRequisicao, true, DataBase.sdfDB) + ", "
                + " Sexo='" + sexo + "', "
                + " Idade='" + idade + "', "
                + " IdadeInformada='" + idadeInformada + "', "
                + " CodGrupoConvenio=" + codGrupoConvenio + ", "
                + " CodSeguradoraConvenio=" + codSeguradoraConvenio + ", "
                + " Total=" + total + ", "
                + " Desconto=" + desconto + ", "
                + " ValPago=" + valPago + ", "
                + " IdPosto=" + idPosto + ", "
                + " ysnOrcamento=" + ysnOrcamento + ", "
                + " CodMedico=" + codMedico + ", "
                + " CampoVariavel2=" + campoVariavel2 + ", "
                + " DescontoPercent=" + descontoPercent;
    }

    public String getInsertString() {
        return numPaciente + "," + numProtocolo + "," + Convert.dToString(dataRequisicao, true, DataBase.sdfDB) + ",'" + sexo + "',"
                + idade + ",'" + idadeInformada + "'," + codGrupoConvenio + ","
                + codSeguradoraConvenio + "," + total + "," + desconto
                + "," + valPago + "," + idPosto + "," + ysnOrcamento + ","
                + codMedico + "," + campoVariavel2 + ","
                + descontoPercent;
    }

    public static Requisicao getFromFile(String line) {
        Requisicao req = new Requisicao();
        String fields[] = line.replaceAll("\"", "").replaceAll("\'", "").toUpperCase().split(";");

        req.numPaciente = Convert.toInt(fields[0]);
        req.numProtocolo = Convert.toInt(fields[1]);
        req.dataRequisicao = Convert.toDate(fields[2], DataBase.sdf);
        req.sexo = fields[3];
        req.idade = Convert.toInt(fields[4]);
        req.idadeInformada = fields[5];
        req.codGrupoConvenio = Convert.toInt(fields[6]);
        req.codSeguradoraConvenio = Convert.toInt(fields[7]);
        req.total = Convert.toDouble(fields[8]);
        req.desconto = Convert.toDouble(fields[9]);
        req.valPago = Convert.toDouble(fields[10]);
        req.idPosto = Convert.toInt(fields[11]);
        req.ysnOrcamento = Convert.toDouble(fields[12]);
        req.codMedico = Convert.toInt(fields[13]);
        req.campoVariavel2 = Convert.toInt(fields[14]);
        req.descontoPercent = Convert.toDouble(fields[15]);

        return req;
    }
}
