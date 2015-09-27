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
public class Paciente {
    public static int nof_fields = 12;

    public int numPaciente = 0;
    public String codGrupo;
    public String codSeguradora;
    public Date dataNascimento;
    public String sexo;
    public String bairro;
    public String cep;
    public String cidade;
    public String uf;
    public Date ultimaRequisicao;
    public int flagPaciente;
    public String nomePaciente;

    public static Object fromResultSet(ResultSet rs) {
        Paciente p = new Paciente();
        try {
            p.numPaciente = rs.getInt(1);
            p.codGrupo = rs.getString(2);
            p.codSeguradora = rs.getString(3);
            p.dataNascimento = rs.getDate(4);
            p.sexo = rs.getString(5);
            p.bairro = rs.getString(6);
            p.cep = rs.getString(7);
            p.cidade = rs.getString(8);
            p.uf = rs.getString(9);
            p.ultimaRequisicao = rs.getDate(10);
            p.flagPaciente = rs.getInt(11);
            p.nomePaciente = rs.getString(12);

        } catch (SQLException ex) {
            ex.printStackTrace();
        } 

        return p;
    }

    public String getUpdateString() {
        return " CodGrupo='" + codGrupo + "', "
                + " CodSeguradora='" + codSeguradora + "', "
                + " DataNascimento=" + Convert.dToString(dataNascimento, true, DataBase.sdfDB) + ", "
                + " Sexo=" + sexo + ", "
                + " Bairro='" + bairro + "', "
                + " Cep='" + cep + "', "
                + " Cidade='" + cidade + "', "
                + " UF='" + uf + "', "
                + " DataUltimaRequisicao=" + Convert.dToString(ultimaRequisicao, true, DataBase.sdfDB) + ", "
                + " FlagPaciente=" + flagPaciente + ", "
                + " NomePaciente='" + nomePaciente +"'";
    }

    public String getInsertString() {
        return numPaciente + ",'" + codGrupo + "','" + codSeguradora + "'," + Convert.dToString(dataNascimento, true, DataBase.sdfDB) + ",'"
                + sexo + "','" + bairro + "','" + cep + "','" + cidade + "','" + uf + "'," + Convert.dToString(ultimaRequisicao, true, DataBase.sdfDB) + ","
                + flagPaciente + ",'" + nomePaciente +"'";
    }

    public static Paciente getFromFile(String line) {
        Paciente p = new Paciente();
        String fields[] = line.replaceAll("\"", "").replaceAll("\'", "").toUpperCase().split(";");

        p.numPaciente = Convert.toInt(fields[0]);
        p.codGrupo = fields[1];
        p.codSeguradora = fields[2];
        p.dataNascimento = Convert.toDate(fields[3], DataBase.sdf);
        p.sexo = fields[4];
        p.bairro = fields[5];
        p.cep = fields[6];
        p.cidade = fields[7];
        p.uf = fields[8];
        p.ultimaRequisicao = Convert.toDate(fields[9], DataBase.sdf);
        p.flagPaciente = Convert.toInt(fields[10]);
        p.nomePaciente = fields[11];

        return p;
    }
}
