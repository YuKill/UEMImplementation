#lang racket

(require rackunit)
(require rackunit/text-ui)
(require "tetra-tipos.rkt")
(require "base.rkt")
(require "tetris.rkt")

;; Constantes usadas nos testes
(define TIMEOUT 14)

(define TT1 (tetramino T_TIPOS 1 (posn 1 0) T_COR))
(define TT1_FIX (tetramino T_TIPOS 1 (posn 4 -1) T_COR))
(define TT1_VIRA (tetramino T_TIPOS 1 (posn 0 2) T_COR))
(define TT1_DOWN (tetramino T_TIPOS 1 (posn 0 0) T_COR))
(define TT1_POS (list (posn 1 1)
                      (posn 2 1) (posn 2 2)
                      (posn 3 1)))
(define TT1_CENTRA_10 (tetramino T_TIPOS 1 (posn 1 3) T_COR))

(define TZ2 (tetramino Z_TIPOS 2 (posn 2 3) Z_COR))
(define TZ2_POS (list (posn 3 3) (posn 3 4)
                      (posn 4 4) (posn 4 5)))
(define TZ2_CENTRA_15 (tetramino Z_TIPOS 2 (posn 2 6) Z_COR))

(define TI0 (tetramino I_TIPOS 0 (posn -1 1) I_COR))
(define TI0_D (tetramino I_TIPOS 0 (posn 0 0) I_COR))
(define TI0_POS (list (posn 0 1) (posn 0 2) (posn 0 3) (posn 0 4)))
(define TI0_CENTRA_12 (tetramino I_TIPOS 0 (posn -1 4) I_COR))

(define C1 (list (list 0 0 0 0 0 0 0)   ; 0
                 (list 0 0 0 0 0 0 0)   ; 1
                 (list 6 0 0 0 0 0 0)   ; 2
                 (list 4 0 2 4 6 1 1)   ; 3
                 (list 3 4 0 0 0 0 0)   ; 4
                 (list 1 2 4 3 2 5 6))) ; 5
                 ;     0 1 2 3 4 5 6

(define C1_LARGURA 7)
(define C1_ALTURA 6)
;; algumas posições ocupadas em C1
(define C1_OCUPADAS (list (posn 2 0) (posn 3 2) (posn 4 1)))
;; algumas posições livres em C1
(define C1_LIVRES (list (posn 0 0) (posn 3 1) (posn 4 2)))

(define C T_COR)

; Representa C1 com o tetraminó TT1 fixado no campo
(define C1_FIXA_TT1 (list (list 0 0 0 0 0 0 0)   ; 0
                          (list 0 C 0 0 0 0 0)   ; 1
                          (list 6 C C 0 0 0 0)   ; 2
                          (list 4 C 2 4 6 1 1)   ; 3
                          (list 3 4 0 0 0 0 0)   ; 4
                          (list 1 2 4 3 2 5 6))) ; 5
                          ;     0 1 2 3 4 5 6

; Representa C1 com o tetraminó TT1 fixado no campo
(define C1_FIXA_TT1_ (list (list 0 0 0 0 0 0 0)   ; 0
                          (list 0 C 1 1 1 1 0)   ; 1
                          (list 6 C C 0 0 0 0)   ; 2
                          (list 4 C 2 4 6 1 1)   ; 3
                          (list 3 4 0 0 0 0 0)   ; 4
                          (list 1 2 4 3 2 5 6))) ; 5
                          ;     0 1 2 3 4 5 6

; Representa C1_FIXA_TT1 sem as linha completas
(define C1_FIXA_TT1_LIMPA (list (list 0 0 0 0 0 0 0)   ; 0
                                (list 0 0 0 0 0 0 0)   ; 1
                                (list 0 0 0 0 0 0 0)   ; 2
                                (list 0 C 0 0 0 0 0)   ; 3
                                (list 6 C C 0 0 0 0)   ; 4
                                (list 3 4 0 0 0 0 0))) ; 5
                                ;     0 1 2 3 4 5 6

(define C2 (list (list 0 0 0 0 0)
                 (list 0 0 0 0 0)
                 (list 0 0 0 0 0)
                 (list 0 0 0 0 0)
                 (list 0 0 0 0 0)
                 (list 0 0 0 0 0)
                 (list 0 0 0 0 0)))
(define C2_FIX (list (list 0 0 0 0 0)
                 (list 0 0 0 0 0)
                 (list 0 0 0 0 0)
                 (list 0 0 0 0 0)
                 (list C 0 0 0 0)
                 (list C C 0 0 0)
                 (list C 0 0 0 0)))

(define C2_LARGURA 5)
(define C2_ALTURA 7)

(define make-linha-tests
  (test-suite
   "make-linha tests"
   (check-equal? (make-linha 0) empty)
   (check-equal? (make-linha 5) (list 0 0 0 0 0))))

(define make-campo-tests
  (test-suite
   "make-campo tests"
   (check-equal? (make-campo C2_LARGURA C2_ALTURA) C2)))

(define centraliza-tests
  (test-suite
   "centraliza tests"
   (check-equal? (centraliza TT1 10)
                 TT1_CENTRA_10)
   (check-equal? (centraliza TZ2 15)
                 TZ2_CENTRA_15)
   (check-equal? (centraliza TI0 12)
                 TI0_CENTRA_12)))

(define make-tetris-tests
  (test-suite
   "make-tetris tests"
   (check-equal? (make-tetris C2_LARGURA C2_ALTURA (list TT1 TZ2 TI0) TIMEOUT)
                 (tetris C2
                         C2_LARGURA
                         C2_ALTURA
                         (centraliza TT1 C2_LARGURA)
                         (list TZ2 TI0)
                         TIMEOUT
                         0 1))))

(define tetramino->pos-tests
  (test-suite
   "tetramino->pos tests"
   (check-equal? (tetramino->lista-pos TT1) TT1_POS)
   (check-equal? (tetramino->lista-pos TZ2) TZ2_POS)
   (check-equal? (tetramino->lista-pos TI0) TI0_POS)))

(define lop-validas?-tests
  (test-suite
   "lop-validas? tests"
   (check-equal? (lop-validas? empty 5 8)
                 #t)
   ;; testa os extremos
   (check-equal? (lop-validas? (list (posn 0 0)
                                     (posn (sub1 C1_ALTURA) 0)
                                     (posn 0 (sub1 C1_LARGURA))
                                     (posn (sub1 C1_ALTURA) (sub1 C1_LARGURA)))
                               C1_LARGURA
                               C1_ALTURA)
                 #t)
   (check-equal? (lop-validas? (list (posn 0 0)
                                     (posn C1_ALTURA 0) ; linha inválida
                                     (posn 1 2))
                               C1_LARGURA
                               C1_ALTURA)
                 #f)
   (check-equal? (lop-validas? (list (posn  2 3)
                                     (posn -1 3)) ; linha inválida
                               C1_LARGURA
                               C1_ALTURA)
                 #f)
   (check-equal? (lop-validas? (list (posn 0 0)
                                     (posn 0 C1_LARGURA) ; coluna inválida
                                     (posn 1 2))
                               C1_LARGURA
                               C1_ALTURA)
                 #f)
   (check-equal? (lop-validas? (list (posn 0 0)
                                     (posn 1 -1)) ; coluna inválida
                               C1_LARGURA
                               C1_ALTURA)
                 #f)))

(define lop-livres?-tests
  (test-suite
   "lop-livres? tests"
   (check-equal? (lop-livres? C1_LIVRES C1) #t)
   (check-equal? (lop-livres? C1_OCUPADAS C1) #f)
   (check-equal? (lop-livres? (append C1_LIVRES (list (first C1_OCUPADAS))) C1) #f)))

(define fixa-tests
  (test-suite
   "fixa tests"
   (check-equal? (fixa (tetris C1 C1_LARGURA C1_ALTURA TT1 empty TIMEOUT 0 1))
                 (tetris C1_FIXA_TT1 C1_LARGURA C1_ALTURA TT1 empty TIMEOUT 0 1))
   (check-equal? (fixa (tetris C2 C2_LARGURA C2_ALTURA TT1_FIX empty TIMEOUT 0 1))
                 (tetris C2_FIX C2_LARGURA C2_ALTURA TT1_FIX empty TIMEOUT 0 1))))

(define limpa-tests
  (test-suite
   "limpa tests"
   (check-equal? (limpa (tetris C1_FIXA_TT1 C1_LARGURA C1_ALTURA TT1 empty TIMEOUT 0 1))
                 (tetris C1_FIXA_TT1_LIMPA C1_LARGURA C1_ALTURA TT1 empty TIMEOUT 50 1))))

(define rot-list-tests 
  (test-suite
   "rot-list tests"
   (check-equal? (rot-list I_TIPOS 0) I0)
   (check-equal? (rot-list I_TIPOS 1) I1)
   (check-equal? (rot-list S_TIPOS 2) S2)))

(define lin->lista-tests
  (test-suite
   "Linha para Lista de Posn"
   (check-equal? (lin->lista (posn 1 0) '(0 1 0) 0 0) (list (posn 1 1)))
   (check-equal? (lin->lista (posn 1 0) '(0 1 1) 0 1) (list (posn 2 1) (posn 2 2)))
   (check-equal? (lin->lista (posn 1 0) '(0 1 0) 0 2) (list (posn 3 1)))
   (check-equal? (lin->lista (posn 2 3) '(1 1 0) 0 1) (list (posn 3 3) (posn 3 4)))
   (check-equal? (lin->lista (posn 4 7) '(0 1 0 0) 0 3) (list (posn 7 8)))
   (check-equal? (lin->lista (posn 4 7) '(1 1 1 1) 0 2) (list (posn 6 7) (posn 6 8) (posn 6 9) (posn 6 10)))))

(define get-posn-tests
  (test-suite
   "get-posn testes"
   (check-equal? (get-posn -1 C1 0) empty)
   (check-equal? (get-posn 3 C1 0) (list 4 0 2 4 6 1 1))
   (check-equal? (get-posn 5 (get-posn 5 C1 0) 0) 5)))

(define insert-t-linha-tests
  (test-suite
   "insert-t-linha testes"
   (check-equal? (insert-t-linha empty (list 1 2 4 3 2 5 6) 0 C) (list 1 2 4 3 2 5 6)) 
   (check-equal? (insert-t-linha (list (posn 2 1) (posn 2 2)) (list 6 0 0 0 0 0 0) 0 C)
                 (list 6 C C 0 0 0 0))))

(define get-t4line-tests
  (test-suite
   "get-t4line testes"
   (check-equal? (get-t4line TT1_POS 0) empty)
   (check-equal? (get-t4line TT1_POS 2) (list (posn 2 1) (posn 2 2)))))

(define insert-t-campo-tests
  (test-suite
   "insert-t-campo testes"
    (check-equal? (insert-t-campo empty C1 0 C) C1)
    (check-equal? (insert-t-campo TT1_POS C1 0 C) C1_FIXA_TT1)))

(define verifica-linha-tests
  (test-suite
   "verifica-linha testes"
   (check-equal? (verifica-linha empty) #t)
   (check-equal? (verifica-linha (list 9 8 3 2 8 4 9)) #t)
   (check-equal? (verifica-linha (list 8 3 29 8 91 2 0)) #f)))

(define verifica-campo-tests
  (test-suite
   "verifica-campo testes"
   (check-equal? (verifica-campo C1_FIXA_TT1) (list (list 0 0 0 0 0 0 0) 
                                                    (list 0 C 0 0 0 0 0) 
                                                    (list 6 C C 0 0 0 0) 
                                                    (list 3 4 0 0 0 0 0)))))

(define add-blank-lin-tests
  (test-suite
   "add-blank-lin testes"
   (check-equal? (add-blank-lin empty 3) (list (list 0 0 0 0 0 0 0)
                                               (list 0 0 0 0 0 0 0)
                                               (list 0 0 0 0 0 0 0)))
   (check-equal? (add-blank-lin (list (list 1 2 3 4 5 6 7)
                                      (list 1 2 3 4 5 6 7)
                                      (list 1 2 3 4 5 6 7)) 2) (list (list 0 0 0 0 0 0 0)
                                                                     (list 0 0 0 0 0 0 0)
                                                                     (list 1 2 3 4 5 6 7)
                                                                     (list 1 2 3 4 5 6 7)
                                                                     (list 1 2 3 4 5 6 7)))))

(define vira-tetra-tests
  (test-suite
   "vira-tetra testes"
   (check-equal? (vira-tetra C1 TT1) TT1)
   (check-equal? (vira-tetra C1 TT1_VIRA) (struct-copy tetramino TT1_VIRA
                                                  (rot 2)))))

(define desl-r-tests
  (test-suite
   "desl-r testes"
   (check-equal? (desl-r C1 TT1) TT1)
   (check-equal? (desl-r C1 TT1_VIRA) (struct-copy tetramino TT1_VIRA
                                                       (pos (posn 0 3))))))

(define desl-l-tests
  (test-suite
   "desl-l testes"
   (check-equal? (desl-l C1 TT1) TT1)
   (check-equal? (desl-l C1 TT1_VIRA) (struct-copy tetramino TT1_VIRA
                                                       (pos (posn 0 1))))))

(define go-down-tests
  (test-suite
   "go-down testes"
   (check-equal? (go-down C1 TT1) TT1)
   (check-equal? (go-down C1 TT1_DOWN) (struct-copy tetramino TT1_DOWN
                                                    (pos (posn 1 0))))))

(define desce-tests
  (test-suite
   "desce testes"
   (check-equal? (desce C1 TT1) TT1)
   (check-equal? (go-down C1 TT1_DOWN) (struct-copy tetramino TT1_DOWN
                                                    (pos (posn 1 0))))))
(define jogo-t (tetris C1_FIXA_TT1 C1_LARGURA C1_ALTURA TT1 (list TI0_D) 0 1 1))
(define after-timeout-tests
  (test-suite
   "after-timeout testes"
   (check-equal? (after-timeout (tetris C2 C2_LARGURA C2_ALTURA TT1 (list TI0_D) 0 0 1))
                 (tetris C2 C2_LARGURA C2_ALTURA (struct-copy tetramino TT1
                                                                 (pos (posn (add1 (posn-lin (tetramino-pos TT1)))
                                                                            (posn-col (tetramino-pos TT1)))))
                         (list TI0_D) 20 0 1))
   (check-equal? (after-timeout jogo-t) (limpa (struct-copy tetris (fixa jogo-t)
                                                     (tetra (centraliza (stream-first (tetris-proximos jogo-t)) C1_LARGURA))
                                                     (proximos (stream-rest (tetris-proximos jogo-t)))
                                                     (timeout 20))))))

(define trata-tick-tests
  (test-suite
   "trata-tick testes"
   (check-equal? (trata-tick (tetris C2 C2_LARGURA C2_ALTURA TT1 (list TI0_D) 0 0 1))
                 (tetris C2 C2_LARGURA C2_ALTURA (struct-copy tetramino TT1
                                                                 (pos (posn (add1 (posn-lin (tetramino-pos TT1)))
                                                                            (posn-col (tetramino-pos TT1)))))
                         (list TI0_D) 20 0 1))
   (check-equal? (after-timeout jogo-t) (limpa (struct-copy tetris (fixa jogo-t)
                                                            (tetra (centraliza (stream-first (tetris-proximos jogo-t)) C1_LARGURA))
                                                            (proximos (stream-rest (tetris-proximos jogo-t)))
                                                            (timeout 20))))
   (check-equal? (trata-tick (tetris C1_FIXA_TT1 C1_LARGURA C1_ALTURA TT1 (list TI0_D) 10 0 1))
                 (tetris C1_FIXA_TT1 C1_LARGURA C1_ALTURA TT1 (list TI0_D) 9 0 1))))
(define get-color-tests
  (test-suite
   "get-color testes"
   (check-equal? (get-color CORES 0) "black")
   (check-equal? (get-color CORES 1) "cyan")
   (check-equal? (get-color CORES 2) "blue")
   (check-equal? (get-color CORES 3) "orange")
   (check-equal? (get-color CORES 4) "yellow")
   (check-equal? (get-color CORES 5) "green")
   (check-equal? (get-color CORES 6) "purple")
   (check-equal? (get-color CORES 7) "red")))

(define game-over?-tests
  (test-suite
   "game-over? testes"
   (check-equal? (game-over? (tetris C2_FIX C2_LARGURA C2_ALTURA TT1_FIX (list TT1_DOWN) 20 0 1))
                 (tetris C2_FIX C2_LARGURA C2_ALTURA (centraliza TT1_DOWN C2_LARGURA) empty 20 0 1))))
                 
;; ---------------------------------------------------------------------

;; Função que executa um grupo de testes.
(define (executar-testes . testes)
  (run-tests (test-suite "Todos os testes" testes))
  (void))

;; Chama a função para executar os testes.
(executar-testes make-linha-tests
                 make-campo-tests
                 centraliza-tests
                 make-tetris-tests
                 tetramino->pos-tests
                 lop-validas?-tests
                 lop-livres?-tests
                 fixa-tests
                 limpa-tests
                 rot-list-tests
                 lin->lista-tests
                 insert-t-linha-tests
                 insert-t-campo-tests
                 get-t4line-tests
                 verifica-linha-tests
                 vira-tetra-tests
                 desl-r-tests
                 desl-l-tests
                 go-down-tests
                 desce-tests
                 after-timeout-tests
                 trata-tick-tests
                 get-color-tests
                 game-over?-tests)

;(run-test (test-suite "teste" game-over?-tests))