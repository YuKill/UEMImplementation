#lang racket

;; Você deve implementar as funções neste arquivo. Novas funções podem ser
;; criadas, mas todas as funções devem ter testes (no arquivo testes.rkt).
;;
;; Observe que algumas destas funções não tem testes, faz parte do trabalho
;; criar estes testes.
;;
;; Você não precisa se preocupar com ler a tecla pressionada ou desenhar o jogo
;; na tela. O arquivo main.rkt chama uma função que faz isso. Basta você
;; implementar as funções deste arquivo que o jogo funciona.
;;
;; Para ter uma ideia do processo de execução do jogo, execute o arquivo
;; main.rkt sem mudar nada neste arquivo. Uma janela irá aparecer. Pressione
;; algumas teclas e observe a saída no console do DrRacket. Veja o corpo
;; inicial das funções make-tetris-padrao, trata-tecla, trata-tick e desenha.

(require "base.rkt")
(require 2htdp/image)
(require 2htdp/universe)

(provide make-tetris-padrao
         tetramino->lista-pos
         lop-validas?
         lop-livres?
         fixa
         limpa
         trata-tecla
         trata-tick
         desenha
         rot-list
         lin->lista
         get-posn
         insert-t-linha
         insert-t-campo
         get-t4line
         verifica-linha
         verifica-campo
         add-blank-lin
         vira-tetra
         desl-r
         desl-l
         go-down
         desce
         after-timeout
         get-color
         game-over?)

;; -> Tetris
;; Cria o jogo inicial.
;; Esta função é chamada no arquivo main.rkt.
(define (make-tetris-padrao)
  (make-tetris LARGURA-PADRAO ALTURA-PADRAO (stream-tetraminos) (get-timeout 1)))

;; Tetris -> Tetris
;; (define (game-over? jogo))
;; Verifica se o jogo acabou. Ou seja, se não há como encaixar uma nova peça no começo do campo.
;; 1) Se acabou, será retornado um jogo com o nível 0 ("flag" para game over).
;; 2) Se não, será retornado um jogo com uma nova peça centraizada no começo do campo.
(define (game-over? jogo)
  (let ([next (stream-first (tetris-proximos jogo))]
        [resto (stream-rest (tetris-proximos jogo))])
    (cond
      [(lop-livres? (tetramino->lista-pos (centraliza next (tetris-largura jogo)))
                    (tetris-campo jogo))
       (struct-copy tetris jogo
                    (tetra (centraliza next
                                       (tetris-largura jogo)))
                    (proximos resto)
                    (timeout (get-timeout (tetris-nivel jogo))))]
      [else 
       (struct-copy tetris jogo
                    (nivel 0))])))
                                                 
;; Jogo String -> Jogo
;; Esta função é chamada quando uma tecla é pressionada.
;; Devolve um jogo com o tetraminó que está caindo movido de acordo com a tecla
;;   "right" - tenta mover para direita
;;   "left"  - tenta mover para esquerda
;;   "up"    - tenta rotacionar
;;   "down"  - tenta mover para baixo
;;
;; Se a tecla for "right", "left" ou "up" e o movimento não puder ser
;; realizado, o jogo é devolvido sem modificações.
;;
;; Se a tecla for "down" e o movimento não puder ser realizado, tetra é fixado
;; no campo, as linhas completas são removidas, o próximo tetraminó é
;; selecionada para cair e o contador de automovimento retorna ao valor
;; inicial.
;;
;; Se o movimento puder ser realizado, o jogo após o movimento é devolvido.
;;
;; Use a função key=? para comparar o tecla com os valores "right", "left, "up"
;; e "down".
(define (trata-tecla jogo tecla)
  (cond
    [(string=? tecla "up") (struct-copy tetris jogo 
                                        (tetra (vira-tetra (tetris-campo jogo) (tetris-tetra jogo))))]
    [(string=? tecla "right") (struct-copy tetris jogo
                                           (tetra (desl-r (tetris-campo jogo) (tetris-tetra jogo))))]
    [(string=? tecla "left") (struct-copy tetris jogo
                                          (tetra (desl-l (tetris-campo jogo) (tetris-tetra jogo))))]
    [(string=? tecla " ") (game-over? (limpa (fixa (struct-copy tetris jogo
                                                    (tetra (go-down (tetris-campo jogo) (tetris-tetra jogo)))))))]
    [(string=? tecla "down") (struct-copy tetris jogo
                                                    (tetra (desce (tetris-campo jogo) (tetris-tetra jogo))))]
    [else 
     jogo]))

;; Tetramino Campo->Tetramino
;; (define (vira-tetra campo tetra))
;; Verifica qual é a próxima rotação e, se possível, rotaciona o tetramino
;; senão, retorna o mesmo tetraminó.
(define (vira-tetra campo tetra)
  (define new-tetra (struct-copy tetramino tetra (rot (remainder (add1 (tetramino-rot tetra)) (length (tetramino-tipo tetra))))))
  (cond
    [(lop-livres? (tetramino->lista-pos new-tetra) campo) new-tetra]
    [else
     tetra]))

;; Tetramino Campo->Tetramino
;; (define (desl-r campo tetra))
;; Verifica se a posição à direita está livre. Se sim, é retornado um novo
;; tetramino deslocado 1 posição à direita. Se não, é retornado o mesmo tetramino.
(define (desl-r campo tetra)
  (let* ([pos-old (tetramino-pos tetra)]
         [new-tetra (struct-copy tetramino tetra (pos (posn (posn-lin pos-old)
                                                            (add1 (posn-col pos-old)))))])
    (cond
      [(lop-livres? (tetramino->lista-pos new-tetra) campo) new-tetra]
      [else
       tetra])))
                
;; Tetramino Campo->Tetramino
;; (define (desl-l campo tetra))
;; Verifica se a posição à esquerda está livre. Se sim, é retornado um novo
;; tetramino deslocado 1 posição à esquerda. Se não, é retornado o mesmo tetramino.
(define (desl-l campo tetra)
  (let* ([pos-old (tetramino-pos tetra)]
         [new-tetra (struct-copy tetramino tetra (pos (posn (posn-lin pos-old)
                                                            (sub1 (posn-col pos-old)))))])
    (cond
      [(lop-livres? (tetramino->lista-pos new-tetra) campo) new-tetra]
      [else
       tetra])))

;; Tetramino Campo->Tetramino
;; (define (go-down campo tetra))
;; É retornado um tetramino com a posição máxima abaixo. A posição onde ele estaria
;; caso o mesmo continuasse na mesma posição.
(define (go-down campo tetra)
  (let* ([pos-old (tetramino-pos tetra)]
         [new-tetra (struct-copy tetramino tetra (pos (posn (add1 (posn-lin pos-old))
                                                            (posn-col pos-old))))])
    (cond
      [(lop-livres? (tetramino->lista-pos new-tetra) campo) (go-down campo new-tetra)]
      [else
       tetra])))
                                             
;; Jogo -> Jogo
;; Função que trata um tick. Esta função é chamada 28 vezes por segundo, ela
;; deve mover o tetra para baixo depois que uma determinada quantidade
;; TIMEOUT_PADRAO de ticks. Se o jogador mover para baixo e fixar o
;; tetraminó, a contagem deve reiniciar.
(define (trata-tick jogo)
  (define jogo-t (tetris-timeout jogo))
  (cond
    [(zero? jogo-t) (after-timeout jogo)]
    [else (struct-copy tetris jogo
                       (timeout (sub1 jogo-t)))]))

;; Tetramino Campo->Tetramino
;; (define (desce campo tetra))
;; Se 1 posição abaixo da posição atual do tetramino estiver livre, então sua posição é movida conforme.
(define (desce campo tetra)
  (let* ([pos-t (tetramino-pos tetra)]
         [new-tetra (struct-copy tetramino tetra
                                 (pos (posn (add1 (posn-lin pos-t))
                                            (posn-col pos-t))))])
    (cond
      [(lop-livres? (tetramino->lista-pos new-tetra) campo) new-tetra]
      [else
       tetra])))

;; Tetris->Tetris
;; (define (after-timeout jogo))
;; Ao timeout se esgotar, essa função faz com que o tetramino desça 1 linha e, caso o mesmo já 
;; não conseguir descer mais, a peça é fixada e, uma nova peça é setada. O timeout também é resetado.
(define (after-timeout jogo) 
  (define new-tetra (desce (tetris-campo jogo)
                          (tetris-tetra jogo)))
    (cond
      [(equal? (tetris-tetra jogo) new-tetra) (game-over? (limpa (fixa jogo)))] 
      [else (struct-copy tetris jogo
                         (timeout (get-timeout (tetris-nivel jogo)))
                         (tetra new-tetra))]))

;; Tetris -> Imagem
;; Esta função é chamada quando o jogo precisa ser desenhado na tela. Devolve
;; uma imagem que representa o jogo.
;; Veja as funções pré-definidas rectangle, beside, above e overlay no pacote
;; 2htdp/image.
(define (desenha jogo)
  (cond
    [(= 0 (tetris-nivel jogo)) (overlay (text/font "GAME OVER" 50 (get-color CORES (random 9 (current-pseudo-random-generator)))
                                                   "Courier New" 'decorative 'normal 'bold #f)
                                        (desenha-tudo jogo))]
    [else
     (desenha-tudo jogo)]))

;; Lista ref->String
;; (define (get-color color-list num))
;; Retorna a cor representada, na lista "color-list", pelo numero "num".
(define (get-color color-list num)
  (cond
    [(zero? num) (first color-list)]
    [else
     (get-color (rest color-list) (sub1 num))]))

;; Cor->Imagem
;; (define (get-pix color))
;; Gera uma imagem customizada, conforme a cor selecionada.
(define (get-pix color)
  (cond
    [(equal? color "black") (square Q-LARGURA "solid" color)]
    [(equal? color "sombra") (overlay (square (- Q-LARGURA 4) "solid" "white")
                                      (square Q-LARGURA "solid" "black"))]
    [else
     (overlay (square (- Q-LARGURA 4) "solid" color)
              (square Q-LARGURA "solid" "white"))]))

;; Lista->Imagem
;; (define (print-linha lin))
;; Retorna a imagem de uma linha inteira, de acordo com os elementos da lista lin.
(define (print-linha lin)
  (define pix (get-pix (get-color CORES (first lin))))
  (cond
    [(empty? (rest lin)) pix]
    [else
     (beside pix
             (print-linha (rest lin)))]))

;; Campo->Imagem
;; (define (print-campo campo))
;; Retorna a imagem de um campo inteiro, de acordo com os elementos da lista campo.
(define (print-campo campo)
  (cond
    [(empty? (rest campo)) (print-linha (first campo))]
    [else
     (above (print-linha (first campo))
            (print-campo (rest campo)))]))

;; Tetramino->Imagem
;; (define (print-prox tetra))
;; Imprime a imagem da proxima peça, incluindo o fundo preto e a borda da esquerda branca.
(define (print-prox tetra)
  (let* ([prox-t (struct-copy tetramino tetra (pos (posn 1 0)))]
         [jogo-aux (tetris (make-side 5) 7 5 (centraliza prox-t 7) empty 10 0 0)])
    (print-campo (tetris-campo (fixa jogo-aux)))))

;; Jogo->Jogo
;; (define (fixa-sombra jogo))
;; Fixa a sombra da atual peça.
(define (fixa-sombra jogo)
  (let* ([mk-sombra (go-down (tetris-campo jogo) (tetris-tetra jogo))]
         [sombra (struct-copy tetramino mk-sombra
                              (cor 9))]
         [jogo-sombra (fixa (tetris (tetris-campo jogo)
                                    LARGURA-PADRAO
                                    ALTURA-PADRAO
                                    sombra
                                    empty
                                    10 0 0))])
    (struct-copy tetris jogo
                 (campo (tetris-campo jogo-sombra)))))

;; Jogo->Imagem
;; (define (desenha-tudo jogo))
;; Desenha o jogo inteiro, incluindo a "sidebar", onde as informações são mostradas.
(define (desenha-tudo jogo)
  (beside (print-campo (tetris-campo (fixa (fixa-sombra jogo))))
          (above (overlay (text/font "PROX: " Q-ALTURA "red"
                                     "Courier New" 'decorative 'normal 'bold #f)
                          (print-campo (make-side 1)))
                 (above (print-prox (stream-first (tetris-proximos jogo)))
                        (above (overlay (text/font (string-append "  SCORE: " (number->string (tetris-pontos jogo)))
                                                   (- Q-ALTURA 3) "red" "Courier New" 'decorative 'normal 'normal #f)
                                        (print-campo (make-side 1)))
                                        (above (print-campo (make-side 2))
                                               (above (overlay (text/font (string-append "NIVEL: " (number->string (tetris-nivel jogo)))
                                                                          Q-ALTURA "red" "Courier New" 'decorative 'normal 'normal #f)
                                                               (print-campo (make-side 1)))
                                                      (print-campo (make-side 9)))))))))
  
;; Tetramino -> Lista(Posn)
;; Devolve a lista de posições que t ocupa no campo considerando a rotação e a
;; posição (translação em relação a origem).
;; 
;; Por exemplo, seja TT1 definido como
;; (define TT1 (tetramino T_TIPOS 1 (posn 1 0) T_COR))
;; este tetraminó está na rotação 1 e na posição (posn 1 0). O elemento na
;; posição 1 de T_TIPOS é T1 que é a seguinte lista de listas (definina em
;; tetra-tipos.rkt)
;;    0 1 2     ; colunas
;;              ; linhas
;; '((0 1 0)    ; 0
;;   (0 1 1)    ; 1
;;   (0 1 0)))  ; 2
;;
;; As as posições ocupadas por T1 são marcadas com 1, ou seja, as posições
;; ocupadas por T1 são (posn 0 1) (posn 1 1) (posn 1 2) e (posn 2 1). Estas São
;; as posições em relação a (posn 0 0), mas o tetraminó está na posição
;; (posn 1 0), desta forma, precisamos fazer a translação das posições. Para
;; isto, somamos o ponto (posn 1 0) a cada ponto de T1, o que resulta em
;; (pos 1 1) (posn 2 0) (posn 2 2) (posn 3 1). Observe que é posível ter
;; um deslocamento em relação a origem negativa. Por exemplo, se a posição de
;; TT1 fosse (posn 0 -1), obteríamos como resposta da função a lista com
;; as posições (posn 0 0) (posn 1 0) (pos 1 1) (pos 2 0).
;;
;; Veja os testes para outros exemplos de como esta função deve funcionar.
(define (tetramino->lista-pos t)
  (define (junta-posn pos rotation lin)
    (cond
      [(empty? rotation) empty]
      [else
       (append
        (lin->lista pos (first rotation) 0 lin)
        (junta-posn pos (rest rotation) (add1 lin)))]))
  (let* ([tetra-tipo (tetramino-tipo t)]
        [rot (rot-list tetra-tipo (tetramino-rot t))]) 
    (junta-posn (tetramino-pos t) rot 0)))

;; Lista Number->Lista
;; (define (rot-list lst rot))
;; Retorna a matriz relativa ao número de rotação fornecido.
(define (rot-list lst rot)
  (cond
    [(zero? rot) (first lst)]
    [else 
     (rot-list (rest lst) (sub1 rot))]))

;; Lista Pos->Lista
;; (define (lin->lista pos vec col lin))
;; A partir das posições em que "vec" é igual a 1, é retornado uma lista
;; de posns (um para cada numero 1 em vec), somando a coluna de pos (posicao do tetramino)
;; com a coluna atual e, somando as linhas igualmente às colunas.
(define (lin->lista pos vec col lin)
  (cond
   [(empty? vec) empty]
   [else 
    (let ([call-rec (lin->lista pos (rest vec) (add1 col) lin)])
      (if (= 1 (first vec))
          (cons (posn (+ lin (posn-lin pos)) (+ col (posn-col pos)))
                call-rec)
          call-rec))]))
        
;; Lista(Posn) Natural Natural -> Boolean
;; Devolve verdadeiro se todas os posições de lp são válidas, isto é, estão
;; dentro de um campo de tamanho largura x altura. Devolve falso caso
;; contrário.
(define (lop-validas? lp largura altura)
  (cond [(empty? lp) #t]
        [else
         (let ([linha (posn-lin (first lp))]
               [coluna (posn-col (first lp))])
           (and
            (and (and (>= linha 0)
                      (>= coluna 0))
                 (and (< (posn-lin (first lp)) altura)
                      (< (posn-col (first lp)) largura))
                 (lop-validas? (rest lp) largura altura))))]))

;; Lista(Posn) Campo -> Boolean
;; Devolve verdadeiro se todas as posição de lp estão livres no campo. Devolve
;; falso caso contrário.
;; Requer que todas as posições em lp sejam válidas.
(define (lop-livres? lp campo) 
  (cond
    [(empty? lp) #t]
    [else
     (let ([pos (first lp)])
       (and (equal? 0 (get-posn (posn-col pos)
                                (get-posn (posn-lin pos) campo 0)
                                0))
            (lop-livres? (rest lp) campo)))]))

;; NumberDest Campo NumberAtual->Number
;; (define (get-posn lincol-target campo lincol))
;; Retorna "lincol-target" elemento(número ou linha) de uma lista(linha ou campo),
;; conforme lincol é incrementado.
(define (get-posn lincol-target campo lincol)
  (cond
    [(empty? campo) empty]
    [(= lincol-target lincol) (first campo)]
    [else
     (get-posn lincol-target (rest campo) (add1 lincol))]))

;; Jogo -> Jogo
;; Preenche as posições ocupadas pelo tetraminó (que está caindo) no campo do
;; jogo.
;; Requer que tetraminó não possa ser movido para baixo.
(define (fixa jogo) 
  (let ([campo (tetris-campo jogo)]
        [peca (tetris-tetra jogo)])
    (tetris (insert-t-campo (tetramino->lista-pos peca) campo 0 (tetramino-cor peca))
            (tetris-largura jogo)
            (tetris-altura jogo)
            (tetris-tetra jogo)
            (tetris-proximos jogo)
            (tetris-timeout jogo)
            (tetris-pontos jogo)
            (tetris-nivel jogo))))

;; ListaPos Lista Number NumberSet->Lista
;; (define (insert-t-linha t-pos linha col t-cor))
;; Constrói uma linha onde nas colunas dos posns "t-pos" é setado o valor "t-cor".
;; Nos espaços onde não pertencem às colunas de "t-pos", é setado o valor 0.
(define (insert-t-linha t-pos linha col t-cor)
  (cond
    [(empty? linha) empty]
    [(empty? t-pos) (cons (first linha)
                          (insert-t-linha empty (rest linha) (add1 col) t-cor))]
    [(= (posn-col (first t-pos)) col) (cons t-cor
                                            (insert-t-linha (rest t-pos) (rest linha) (add1 col) t-cor))]
    [else
     (cons (first linha)
           (insert-t-linha t-pos (rest linha) (add1 col) t-cor))]))

;; Lista Number->Lista
;; (define (get-t4line t-pos lin))
;; Retorna uma lista de posns dos posns correspondentes com a linha "lin".
(define (get-t4line t-pos lin) 
  (cond
    [(empty? t-pos) empty]
    [(= lin (posn-lin (first t-pos))) (cons (first t-pos)
                                            (get-t4line (rest t-pos) lin))]
    [else (get-t4line (rest t-pos) lin)]))

;; ListaPos Campo Number Cor->Campo
;; (define (insert-t-campo t-pos campo lin t-cor))
;; Constrói um campo onde as posições "t-pos" são preenchidas com "t-cor", e
;; as outras são preenchidas com 0.
(define (insert-t-campo t-pos campo lin t-cor)
  (cond
    [(empty? campo) empty]
    [else
     (cons (insert-t-linha (get-t4line t-pos lin) (first campo) 0 t-cor)
           (insert-t-campo t-pos (rest campo) (add1 lin) t-cor))]))

;; Jogo -> Jogo
;; Devolve um jogo sem as linhas que estão completas, isto é, as linhas que não
;; tem nenhum quadrado vazio. O jogo devolvido tem o mesmo tamanho do jogo de
;; entrada.
(define (limpa jogo)
  (let* ([novo-campo (verifica-campo (tetris-campo jogo))]
        [erased-lin (- (tetris-altura jogo) (length novo-campo))]
        [earn-points (calc-ponto (tetris-pontos jogo) erased-lin (tetris-nivel jogo))])
  (tetris (add-blank-lin novo-campo (- (tetris-altura jogo) (length novo-campo)))
          (tetris-largura jogo)
          (tetris-altura jogo)
          (tetris-tetra jogo)
          (tetris-proximos jogo)
          (tetris-timeout jogo)
          earn-points
          (verifica-nivel earn-points PONTUACAO-BASE NIVEL-BASE))))

;; Lista->Boolean
;; (define (verifica-linha lin))
;; Caso seja encontrado uma linha inteira sem 0s, é retornado True. Caso contrário,
;; é retornado False.
(define (verifica-linha lin)
  (cond
    [(empty? lin) #t]
    [(= (first lin) 0) #f]
    [else (verifica-linha (rest lin))]))

;; Campo Number->Campo
;; (define (add-blank-lin campo num-lin))
;; É adicionado ao campo "num-lin" linhas em branco.
(define (add-blank-lin campo num-lin)
  (cond
    [(zero? num-lin) campo]
    [else (cons (make-list (length (first campo)) 0)
                (add-blank-lin campo (sub1 num-lin)))]))

;; Campo->Campo
;; (define (verifica-campo campo))
;; Caso encontrado alguma linha inteira sem 0s, a linha é retirada do campo. Se não,
;; o mesmo campo é retornado.
(define (verifica-campo campo)
  (cond
    [(empty? campo) empty]
    [(verifica-linha (first campo)) (verifica-campo (rest campo))]
    [else (cons (first campo)
                (verifica-campo (rest campo)))]))
               
;; Number Number Number->Number
;; (define (verifica-nivel points ini nivel))
;; Retorna o nivel de acordo com os pontos do jogador e, da base "ini"(nivel inicial) para os níveis.
(define (verifica-nivel points ini nivel)
  (cond
    [(< points (expt 2 ini)) nivel]
    [else
     (verifica-nivel points (add1 ini) (add1 nivel))]))

;; Number Number Number->Number
;; (define (calc-ponto points lin niv))
;; Calcula os pontos conforme o tanto de linhas apagadas e o nivel.
(define (calc-ponto points lin niv)
  (+ (* (get-points niv) lin) points))

;; -> Stream(Tetramino)
;; Cria um stream randômico de tetraminós.
;; Esta função não precisa de testes.
;; Você tem que implementar esta função, o corpo incial deve ser descartado.
(define (stream-tetraminos)
  (define peca (random 7 (current-pseudo-random-generator)))
  (cond
    [(= peca 0) (stream-cons I (stream-tetraminos))]
    [(= peca 1) (stream-cons J (stream-tetraminos))]
    [(= peca 2) (stream-cons L (stream-tetraminos))]
    [(= peca 3) (stream-cons O (stream-tetraminos))]
    [(= peca 4) (stream-cons S (stream-tetraminos))]
    [(= peca 5) (stream-cons T (stream-tetraminos))]
    [(= peca 6) (stream-cons Z (stream-tetraminos))]))
