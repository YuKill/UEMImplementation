/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package Util;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.util.ArrayList;
import hospitalbdii.Entidade.Paciente;
import hospitalbdii.Entidade.Requisicao;
import hospitalbdii.Entidade.ResultadoExame;
import java.io.IOException;

/**
 *
 * @author yukiosir
 */
public class DataReader {
    
    public static ArrayList<Paciente> readPacientes(String filename) {
        ArrayList<Paciente> pacientes = null;
        try {
            BufferedReader bf = new BufferedReader(new FileReader(filename));
            String line = bf.readLine();
            
            pacientes = new ArrayList<>();
            while ((line = bf.readLine()) != null) {
                if (line.split(";").length == Paciente.nof_fields)
                    pacientes.add(Paciente.getFromFile(line));
            }
        } catch (FileNotFoundException ex) {
            ex.printStackTrace();
        } catch (IOException ex) {
            ex.printStackTrace();
        }
        return pacientes;
    }
    
    public static ArrayList<Requisicao> readRequisicao(String filename) {
        ArrayList<Requisicao> requisicoes = null;
        try {
            BufferedReader bf = new BufferedReader(new FileReader(filename));
            String line = bf.readLine();
            
            requisicoes = new ArrayList<>();
            while ((line = bf.readLine()) != null) {
                if (line.split(";").length == Requisicao.nof_fields)
                    requisicoes.add(Requisicao.getFromFile(line));
            }
        } catch (FileNotFoundException ex) {
            ex.printStackTrace();
        } catch (IOException ex) {
            ex.printStackTrace();
        }
        return requisicoes;
    }
    
    public static ArrayList<ResultadoExame> readResultados(String filename) {
        ArrayList<ResultadoExame> resultados = null;
        try {
            BufferedReader bf = new BufferedReader(new FileReader(filename));
            String line = bf.readLine();
            
            resultados = new ArrayList<>();
            while ((line = bf.readLine()) != null) {
                if (line.split(";").length == ResultadoExame.nof_fields) 
                    resultados.add(ResultadoExame.getFromFile(line));
            }
        } catch (FileNotFoundException ex) {
            ex.printStackTrace();
        } catch (IOException ex) {
            ex.printStackTrace();
        }
        return resultados;
    }
}
