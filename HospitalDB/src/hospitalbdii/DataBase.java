/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package hospitalbdii;

import hospitalbdii.Entidade.Paciente;
import hospitalbdii.Entidade.Requisicao;
import hospitalbdii.Entidade.ResultadoExame;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.text.SimpleDateFormat;
import java.util.ArrayList;

/**
 *
 * @author yukiosir
 */
public class DataBase {

    private static boolean debugging = false;

    private static final String dbName = "hospitaldb";

    public enum Tabela {
        PACIENTES, REQUISICAO, RESULTADOS
    }
           

    private static final String[][] campos = {
        {
            "NumPaciente", "CodGrupo", "CodSeguradora", "DataNascimento",
            "Sexo", "Bairro", "Cep", "Cidade", "UF", "DataUltimaRequisicao",
            "FlagPaciente", "NomePaciente"
        },
        {
            "NumPaciente", "NumProtocolo", "DataRequisicao", "Sexo", "Idade",
            "IdadeInformada", "CodGrupoConvenio", "CodSeguradoraConvenio",
            "Total", "Desconto", "ValPago", "IdPosto", "ysnOrcamento", "CodMedico",
            "CampoVariavel2", "DescontoPercent"
        },
        {
            "NumProtocolo", "CodExame", "NumItem", "NumAtributo", "NomeAtributo",
            "Tipo", "Visivel", "Informado", "Calculado", "Formula",
            "TextoValorNormal", "ResultadoPadrao", "Unidade", "Normalidade",
            "Resultado", "NumExame", "Impresso"
        }
    };

    private static final int port = 5432;
    private static final String host = "localhost";
    private static final String url = "jdbc:postgresql://" + host + ":" + port + "/" + dbName;

    private static final String user = "admin";
    private static final String pass = "1234";
    
    public static SimpleDateFormat sdf = new SimpleDateFormat("d/M/yy H:mm:ss");
    public static SimpleDateFormat sdfDB = new SimpleDateFormat("yyyy-MM-dd");

    public static void createTables() {
        logDebug("Creating tables.");
        String[] queries = {
            // Pacientes
            //time psql -h localhost -p 5432 -d hospitaldb -U admin -c "select resultados.tipo,date_part('month', requisicao.datarequisicao) as mes, date_part('year', requisicao.datarequisicao) as ano, count(resultados.tipo) from resultados,requisicao where resultados.numprotocolo=requisicao.numprotocolo group by tipo,mes,ano order by ano,mes;" -w
            //time psql -h localhost -p 5432 -d hospitaldb -U admin -c "select p.NomePaciente, extract( year from age( CURRENT_DATE,p.DataNascimento)) as idade,p.CodSeguradora, p.CodGrupo, r.DataRequisicao, res.Tipo from Pacientes p, Requisicao r, Resultados res where extract( year from age( CURRENT_DATE,p.DataNascimento))>30 and p.NumPaciente = r.NumPaciente and r.NumProtocolo=res.NumProtocolo" -w
               //time psql -h localhost -p 5432 -d hospitaldb -U admin -c "SET enable_seqscan = off; select resultados.tipo,date_part('month', requisicao.datarequisicao) as mes, date_part('year', requisicao.datarequisicao) as ano, count(resultados.tipo) from resultados,requisicao where resultados.numprotocolo=requisicao.numprotocolo group by tipo,mes,ano order by ano,mes;" -w
                //select resultados.tipo,date_part('month', requisicao.datarequisicao) as mes, date_part('year', requisicao.datarequisicao) as ano, count(resultados.tipo) from resultados,requisicao where resultados.numprotocolo=requisicao.numprotocolo group by tipo,mes,ano order by ano,mes;
            "NumPaciente SERIAL PRIMARY KEY NOT NULL,"
            + "CodGrupo VARCHAR(20),"
            + "CodSeguradora VARCHAR(20),"
            + "DataNascimento DATE,"
            + "Sexo VARCHAR(4),"
            + "Bairro VARCHAR(50)," 
            + "Cep VARCHAR(20),"
            + "Cidade VARCHAR(50),"
            + "UF VARCHAR(50),"
            + "DataUltimaRequisicao DATE,"
            + "FlagPaciente INT,"
            + "NomePaciente VARCHAR(100)",
            // 
            // Requisicoes
            "NumPaciente INT NOT NULL,"
            + "NumProtocolo SERIAL PRIMARY KEY NOT NULL,"
            + "DataRequisicao DATE,"
            + "Sexo VARCHAR(4),"
            + "Idade INT,"
            + "IdadeInformada VARCHAR(20),"
            + "CodGrupoConvenio INT,"
            + "CodSeguradoraConvenio INT,"
            + "Total REAL," 
            + "Desconto REAL,"
            + "ValPago REAL,"
            + "IdPosto INT,"
            + "ysnOrcamento REAL,"
            + "CodMedico INT,"
            + "CampoVariavel2 INT,"
            + "DescontoPercent REAL",
            //
            // Resultados
            "NumProtocolo INT NOT NULL,"
            + "CodExame VARCHAR(50),"
            + "NumItem SERIAL PRIMARY KEY NOT NULL,"
            + "NumAtributo INT,"
            + "NomeAtributo VARCHAR(200),"
            + "Tipo VARCHAR(20),"
            + "Visivel INT,"
            + "Informado INT,"
            + "Calculado INT,"
            + "Formula VARCHAR(200),"
            + "TextoValorNormal VARCHAR(200),"
            + "ResultadoPadrao VARCHAR(200),"
            + "Unidade VARCHAR(50),"
            + "Normalidade INT,"
            + "Resultado VARCHAR(200),"
            + "NumExame INT,"
            + "Impresso INT"
        };

        Connection con = null;
        Statement st = null;
        String baseString = "CREATE TABLE ";
        try {
            con = getConnection();
            st = con.createStatement();

            for (int i = 0; i < 3; i++) {
                String q = baseString + Tabela.values()[i].toString()
                        + "(" + queries[i] + ")";
                System.out.println(q);
                st.executeUpdate(q);
            }

        } catch (SQLException ex) {
            ex.printStackTrace();
        } finally {
            try {
                if (st != null) {
                    st.close();
                }
                if (con != null) {
                    con.close();
                }
            } catch (SQLException ex) {
                ex.printStackTrace();
            }
        }
    }

    public static void dropAllTables() {
        logDebug("Dropping tables.");
        Connection con = null;
        Statement st = null;
        String baseString = "DROP TABLE ";
        try {
            con = getConnection();
            st = con.createStatement();

            for (int i = 0; i < 3; i++) {
                String q = baseString + Tabela.values()[i].toString();
                st.executeUpdate(q);
            }

        } catch (SQLException ex) {
            ex.printStackTrace();
        } finally {
            try {
                if (st != null) {
                    st.close();
                }
                if (con != null) {
                    con.close();
                }
            } catch (SQLException ex) {
                ex.printStackTrace();
            }
        }
    }

    public static Object read(Tabela table, String field, String value,
            String beyondWhere) {
        logDebug("Reading table " + table.toString() + ": " + field + " = " + value + ".");
        Statement st = null;
        ResultSet rs = null;
        Connection con = null;
        String querie = "SELECT * FROM " + table.toString() + " WHERE "
                + field + " = " + value + " " + beyondWhere;

        logDebug("Function Read()", querie);
        ArrayList<Object> list = new ArrayList<>();
        try {
            con = getConnection();
            st = con.createStatement();
            rs = st.executeQuery(querie);

            while (rs.next()) {
                switch (table) {
                    case PACIENTES:
                        list.add(Paciente.fromResultSet(rs));
                        break;
                    case REQUISICAO:
                        list.add(Requisicao.fromResultSet(rs));
                        break;
                    case RESULTADOS:
                        list.add(ResultadoExame.fromResultSet(rs));
                        break;
                }
            }

        } catch (SQLException ex) {
            ex.printStackTrace();
        } finally {
            try {
                if (rs != null) {
                    rs.close();
                }
                if (st != null) {
                    st.close();
                }
                if (con != null) {
                    con.close();
                }
            } catch (SQLException ex) {
                ex.printStackTrace();
            }
        }

        if (list.size() > 1) {
            return list;
        }
        return list.get(0);
    }

    public static void write(Object obj) {
        logDebug("Function Write()");
        String update = "UPDATE ";

        if (obj instanceof Paciente) {
            update += Tabela.PACIENTES.toString() + " SET "
                    + ((Paciente) obj).getUpdateString() + " WHERE "
                    + "NumPaciente=" + ((Paciente) obj).numPaciente;
        } else if (obj instanceof Requisicao) {
            update += Tabela.REQUISICAO.toString() + " SET "
                    + ((Requisicao) obj).getUpdateString() + " WHERE "
                    + "NumProtocolo=" + ((Requisicao) obj).numProtocolo;
        } else {
            update += Tabela.RESULTADOS.toString() + " SET "
                    + ((ResultadoExame) obj).getUpdateString() + " WHERE "
                    + "NumItem=" + ((ResultadoExame) obj).numItem;
        }

        Statement st = null;
        ResultSet rs = null;
        Connection con = null;
        logDebug("Function Write()", update);
        try {
            con = getConnection();
            st = con.createStatement();
            st.executeUpdate(update);

        } catch (SQLException ex) {
            ex.printStackTrace();
        } finally {
            try {
                if (rs != null) {
                    rs.close();
                }
                if (st != null) {
                    st.close();
                }
                if (con != null) {
                    con.close();
                }
            } catch (SQLException ex) {
                ex.printStackTrace();
            }
        }
    }
    
    public static Object read(Tabela table, String field, String value, Connection con,
            String beyondWhere) {
        logDebug("Reading table " + table.toString() + ": " + field + " = " + value + ".");
        Statement st = null;
        ResultSet rs = null;
        String querie = "SELECT * FROM " + table.toString() + " WHERE "
                + field + " = " + value + " " + beyondWhere;

        logDebug("Function Read()", querie);
        ArrayList<Object> list = new ArrayList<>();
        try {
            st = con.createStatement();
            rs = st.executeQuery(querie);

            while (rs.next()) {
                switch (table) {
                    case PACIENTES:
                        list.add(Paciente.fromResultSet(rs));
                        break;
                    case REQUISICAO:
                        list.add(Requisicao.fromResultSet(rs));
                        break;
                    case RESULTADOS:
                        list.add(ResultadoExame.fromResultSet(rs));
                        break;
                }
            }

        } catch (SQLException ex) {
            ex.printStackTrace();
        } finally {
            try {
                if (rs != null) {
                    rs.close();
                }
                if (st != null) {
                    st.close();
                }
            } catch (SQLException ex) {
                ex.printStackTrace();
            }
        }

        if (list.size() > 1) {
            return list;
        }
        return list.get(0);
    }

    public static void write(Object obj, Connection con) {
        logDebug("Function Write()");
        String update = "UPDATE ";

        if (obj instanceof Paciente) {
            update += Tabela.PACIENTES.toString() + " SET "
                    + ((Paciente) obj).getUpdateString() + " WHERE "
                    + "NumPaciente=" + ((Paciente) obj).numPaciente;
        } else if (obj instanceof Requisicao) {
            update += Tabela.REQUISICAO.toString() + " SET "
                    + ((Requisicao) obj).getUpdateString() + " WHERE "
                    + "NumProtocolo=" + ((Requisicao) obj).numProtocolo;
        } else {
            update += Tabela.RESULTADOS.toString() + " SET "
                    + ((ResultadoExame) obj).getUpdateString() + " WHERE "
                    + "NumItem=" + ((ResultadoExame) obj).numItem;
        }

        Statement st = null;
        ResultSet rs = null;
        logDebug("Function Write()", update);
        try {
            st = con.createStatement();
            st.executeUpdate(update);

        } catch (SQLException ex) {
            ex.printStackTrace();
        } finally {
            try {
                if (rs != null) {
                    rs.close();
                }
                if (st != null) {
                    st.close();
                }
            } catch (SQLException ex) {
                ex.printStackTrace();
            }
        }
    }

    public static void insert(Object obj, Connection con) {
        logDebug("Function Insert()");
        String insert = "INSERT INTO ";

        if (obj instanceof Paciente) {
            insert += Tabela.PACIENTES.toString() + " VALUES ("
                    + ((Paciente) obj).getInsertString() + ")";
        } else if (obj instanceof Requisicao) {
            insert += Tabela.REQUISICAO.toString() + " VALUES ("
                    + ((Requisicao) obj).getInsertString() + ")";
        } else {
            insert += Tabela.RESULTADOS.toString() + " VALUES ("
                    + ((ResultadoExame) obj).getInsertString() + ")";
        }

        Statement st = null;
        ResultSet rs = null;

        logDebug("Function Insert()", insert);
        while (true) {
            try {
                st = con.createStatement();
                st.executeUpdate(insert);
                break;
            } catch (SQLException ex) {
                ex.printStackTrace();
                continue;
            } finally {
                try {
                    if (rs != null) {
                        rs.close();
                    }
                    if (st != null) {
                        st.close();
                    }
                } catch (SQLException ex) {
                    ex.printStackTrace();
                }
            }
        }
    }

    public static Connection startTransaction() {
        Connection con = null;

        try {
            con = getConnection();
            con.setAutoCommit(false);
            
            con.setTransactionIsolation(4);
        } catch (SQLException ex) {
            ex.printStackTrace();
        }
        return con;
    }

    public static void commitTransaction(Connection con) {
        try {
            con.commit();
        } catch (SQLException ex) {
            ex.printStackTrace();
        } finally {
            try {
                if (con != null) {
                    con.close();
                }
            } catch (SQLException ex) {
                ex.printStackTrace();
            }
        }
    }

    private static Connection getConnection() throws SQLException {
        return DriverManager.getConnection(url, user, pass);
    }

    private static void logDebug(String message) {
        if (debugging) {
            System.out.println(message);
        }
    }

    private static void logDebug(String tag, String message) {
        if (debugging) {
            System.out.println(tag + ": " + message);
        }
    }
}
