/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package hospitalbdii;

import Util.Convert;
import Util.DataReader;
import hospitalbdii.DataBase.Tabela;
import hospitalbdii.Entidade.Paciente;
import hospitalbdii.Entidade.Requisicao;
import hospitalbdii.Entidade.ResultadoExame;
import java.sql.Connection;
import java.text.ParseException;
import java.util.ArrayList;
import java.util.Date;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author yukiosir
 */
public class HospitalBDII {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        /* ------------------First Insertion----------------------*/
        DataBase.dropAllTables();
        DataBase.createTables();

        Connection thisCon = DataBase.startTransaction();
        ArrayList<Paciente> pacientes = DataReader.readPacientes("data/PACIENTE.txt");
        ArrayList<Requisicao> requisicoes = DataReader.readRequisicao("data/REQUISICAO.txt");
        ArrayList<ResultadoExame> resultados = DataReader.readResultados("data/RESULTADOEXAME.txt");

        for (Paciente p : pacientes) 
            DataBase.insert(p, thisCon);
        for (Requisicao r : requisicoes) {
            DataBase.insert(r, thisCon);
        }
        for (ResultadoExame re : resultados)
            DataBase.insert(re, thisCon);
        
        DataBase.commitTransaction(thisCon);

        System.out.println(resultados.size());

        
        /* ------------------Data Concurrency----------------------*/
        // O Screenshot mostra que o PostgreSQL faz uma especie de 
        // bloqueio exclusivo e compartilhado.
        //http://www.postgresql.org/docs/9.1/static/transaction-iso.html
        // Usando o "Repeatable Read Isolation"
//        
        // Ele bloqueia a firstThreadFunction, ja que a segunda acaba antes.
        // no site explica certinho.
//        
//        final int reqNum = 1261540, reqNum2 = 8513065;
//
//        Thread first = new Thread(new Runnable() {
//
//            @Override
//            public void run() {
//                firstThreadFunction(reqNum, reqNum2);
//            }
//        });
//
//        Thread second = new Thread(new Runnable() {
//
//            @Override
//            public void run() {
//                secondThreadFunction(reqNum);
//            }
//        });
//
//        first.start();
//        second.start();
//
//        try {
//            first.join();
//            second.join();
//        } catch (InterruptedException ex) {
//            ex.printStackTrace();
//        }
////        
//        Requisicao req = (Requisicao) DataBase.read(Tabela.REQUISICAO, "numprotocolo",
//                String.valueOf(reqNum), "");
//        System.out.println(req.getInsertString());

//        System.out.println(Convert.toDouble("24,5"));
        
        /*-----------------Crash Recovery--------------------*/
        //http://www.postgresql.org/docs/current/static/wal-intro.html
        //http://dba.stackexchange.com/questions/22799/postgresql-crash-recovery
        //O PostgreSQL usa WAL e Point-In-Time recovery. (pesquise)!
        //Nao da pra pegar o log. Ele eh "low level"
        //Unica mensagem do LOG \/
        //admin@hospitaldb LOG:  unexpected EOF on client connection with an open transaction
//        
        //Ao que tudo indica, ele faz uma atualizacao adiada, ja que ele copia
        //as coisas pro WAL antes de persistir no banco.
//        failure();
    }

    public static void firstThreadFunction(int reqNum, int reqNum2) {
        Connection con = DataBase.startTransaction();
        Requisicao req = (Requisicao) DataBase.read(Tabela.REQUISICAO, "numprotocolo",
                String.valueOf(reqNum), con, "");
        try {
            Thread.sleep(2000);
        } catch (InterruptedException ex) {
            Thread.currentThread().interrupt();
            ex.printStackTrace();
        }

        Requisicao req2 = (Requisicao) DataBase.read(Tabela.REQUISICAO, "numprotocolo",
                String.valueOf(reqNum2), con, "");

        req.total += req2.total;
        DataBase.write(req, con);
        DataBase.commitTransaction(con);
        System.out.println("FirstCommited!!");

    }

    public static void secondThreadFunction(int reqNum) {
        Connection con = DataBase.startTransaction();
        Requisicao req = (Requisicao) DataBase.read(Tabela.REQUISICAO, "numprotocolo",
                String.valueOf(reqNum), con, "");
        try {
            Thread.sleep(1000);
        } catch (InterruptedException ex) {
            Thread.currentThread().interrupt();
            ex.printStackTrace();
        }
        
        req.total += 100;
        DataBase.write(req, con);
        DataBase.commitTransaction(con);
        System.out.println("SecondCommited!!");
    }
    
    public static void failure() {
        Connection con = DataBase.startTransaction();
        Paciente pFail = null;
        Paciente p = (Paciente) DataBase.read(Tabela.PACIENTES, "numpaciente",
                "30", con, "");
        p.bairro = "Zona A";
        DataBase.write(p, con);
        
        // FAILING POINT
        System.out.println(pFail.bairro);
              
        DataBase.commitTransaction(con);
    
    }

}
