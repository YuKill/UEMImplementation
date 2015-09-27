# fatLU
Programa desenvolvido para fins educacionais.

### 1. Compilando
Para compilar o programa, execute o seguinte comando no terminal:

```
$ make clean
$ make
```

### 2. Executando
Após compilar o programa, você terá dois executáveis. Um para execução
sequencial e o outro para execução paralela.

##### 2.1. Sequencial
Execute os seguintes comandos:
```
$ ./seq <parâmetros>
```

##### 2.2. Paralelo
Execute os seguintes comandos:
```
$ mpirun -np <number_machines> --hostfile <machine_ips> par <parâmetros>
```

##### 2.3. Parâmetros
Para executar esse programa, sao necessarios os seguintes parametros:
- i: especifica o arquivo de entrada;
- n: especifica a quantidade de equacoes.

Parametros opcionais:
- o: especifica o arquivo de entrada.
