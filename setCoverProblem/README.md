setCoverProblem
===============

Para executar:

   $ make clean
   $ make

Será criado um arquivo chamado "run.sh". Execute ele da seguinte maneira:

   $ ./run.sh a b c d e f g h

Onde as variáveis representam:

   a: caminho do arquivo de teste (tabela fornecida)
   b: tamanho da populacao inicial
   c: pode ser "-t" ou "-p"
      -t: descreve que o proximo argumento sera o tempo máximo de execucao
      -p: descreve a populacao maxima que o programa pode ter
   d: argumento pertencente à variavel "c"
   e: tempo que o programa irá esperar para observar se o melhor individuo da populacao mudou
   f: 0 < x < 1. É um valor menor que 1. Representa a chance de um individuo sofrer mutacao
   g: Igualmente o argumento "f", é um valor menor que 1. Representa a chance de um gene do individuo
      ser mutado.
   h: a quantidade de vizinhanças que serão geradas para cada individuo gerado (VND).

Como Output, será gerado o peso do melhor resutado, e os genes que o compoe. Também será
impresso na tela o tempo de execucao.

Para salvar os resultados em algum arquivo, faça:

   $ ./run.sh a b c d e f g h > caminho-do-arquivo

ou, para não reescreve-lo:

   $ ./run.sh a b c d e f g h >> caminho-do-arquivo


ESSE PROGRAMA FOI ESCRITO NA LINGUAGEM SCALA.

Portanto, você deve ter o programa instalado em sua máquina. Para verificar,
digite o comando:

   $ scala -version

Para executar no netbeans, você deve ir à pasta de instalação do netbeans e:

   $ cd pasta-do-netbeans
   $ cd etc
   $ gedit netbeans.conf

Na opção "netbeans-default-options" adicione a seguinte configuraçao, para que o mesmo fique
da seguinte forma:

   netbeans-default-options="algumas-opções-pre-definidas -J-Dscala.home=pasta-de-instalacao-do-scala"

Voce também deve ter o plugin scala para netbeans instalado.
Para isso, vá em:

   tools->plugins->available plugins

Procure por "scala" e instale todos os plugins.
