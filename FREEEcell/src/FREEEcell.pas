Program Freecell;
Uses CRT;


type
    TpCarta = record
              valor, naipe: integer;
			  // NAIPE 0 - PAUS, 1 - COPAS, 2 - ESPADAS, 3 - OURO
end;
type
	TpBanco = record
			carta: TpCarta;
			cheio: boolean;
end;

type
    TpBar = record
              carta: TpCarta;
              ok: boolean;
end;

type
    TpPilha = record
              elem: array [0..53] of TpBar;
              topo,tammax: integer;
end;
 var
	baralho: array [0..51] of TpBar;
    	coluna: array [0..7] of TpPilha;
	ending: array [0..3] of TpPilha;
	banco: array [0..3] of TpBanco;
	i: integer;
	CartaEscolhida: TpCarta;
	carta: TpBar;

Procedure printJogo;
var
	i, j, maiorcol, elem: integer;
	escrita: char;
begin
	clrscr;
	writeln('==========================================================');
	writeln();
	for j:= 0 to 3 do
	begin
		write('ENDING (', j, '): ');
		for i := 0 to ending[j].topo do
		begin
			case ending[j].elem[i].carta.valor of
		
				1:
				begin
					write('A');
					write(' -> ');
				end;

				11:
				begin
					write('J');
					write(' -> ');	
				end;

				12:
				begin
					write('Q');		
					write(' -> ');
				end;

				13:
				begin
					write('K');	
					write(' -> ');
				end;

				else
				begin
					write(ending[j].elem[i].carta.valor);
					write(' -> ');
				end;
			end;
		end;
		
		writeln();
	end;
	writeln();
	writeln('       0     1     2     3');
	write('BANCO: ');
	for i:= 0 to 3 do
	begin
		if(banco[i].cheio) then
		begin
			case banco[i].carta.valor of		
				1:
				begin
					write('A(', banco[i].carta.naipe,')');
					write('  ');
				end;

				11:
				begin
					write('J(', banco[i].carta.naipe,')');
					write('  ');	
				end;

				12:
				begin
					write('Q(', banco[i].carta.naipe,')');		
					write('  ');
				end;

				13:
				begin
					write('K(', banco[i].carta.naipe,')');	
					write('  ');
				end;

				else
				begin
					write('', banco[i].carta.valor, '(', banco[i].carta.naipe,')');
					
					if((banco[i].carta.valor div 10) = 0) then
					begin
						write('  ');
					end
					else
					begin
						write(' ');
					end;
				end;
			end;
		end
		else
		begin
			write('vazio ');
		end;
	end;
	
	writeln();
	writeln();

	maiorcol := 0;
	for i := 0 to 7 do
	begin
		if (coluna[i].topo > maiorcol) then
		begin
			maiorcol := coluna[i].topo;
		end;
	end;
	writeln('0     1     2     3     4     5     6     7');
	for elem := 0 to maiorcol do
	begin
		for i := 0 to 7 do
		begin
			if (coluna[i].topo >= elem) then
			begin
				case coluna[i].elem[elem].carta.valor of
					1:
					begin
						write('A(', coluna[i].elem[elem].carta.naipe,')');
						write('  ');
					end;

					11:
					begin
						write('J(', coluna[i].elem[elem].carta.naipe,')');
						write('  ');	
					end;

					12:
					begin
						write('Q(', coluna[i].elem[elem].carta.naipe,')');		
						write('  ');
					end;

					13:
					begin
						write('K(', coluna[i].elem[elem].carta.naipe,')');	
						write('  ');
					end;

					else
					begin
						write('', coluna[i].elem[elem].carta.valor, '(', coluna[i].elem[elem].carta.naipe,')');
						
						if((coluna[i].elem[elem].carta.valor div 10) = 0) then
						begin
							write('  ');
						end
						else
						begin
							write(' ');
						end;
					end;
				end;

			end
			else
			begin
				write('      ');
			end;
		end;
		writeln();
	end;
	writeln('==========================================================');
	writeln();
end;
// ============ FUNÇOES DE PILHA ============

Procedure InicializaPilha(var P: TpPilha);
          begin
               P.topo := -1;
	       P.tammax := 53;
          end;

Function PilhaVazia(P: TpPilha): boolean;
          begin
               PilhaVazia := (P.topo = -1);
          end;

Function PilhaCheia(P: TpPilha):boolean;
          begin
               PilhaCheia:= (P.topo = P.tammax)
          end;

Procedure Empilhar(var P: TpPilha; x: TpBar);
          begin
          if not PilhaCheia(P) then
          begin
               P.topo:= (P.topo+1);
               P.elem[P.topo]:= x;
          end;
          end;

Function Desempilha(var P: TpPilha): TpBar;
          begin
          if not PilhaVazia(P) then
          begin
               Desempilha:= P.elem[P.topo];
               P.topo:= (P.topo-1);
          end;
          end;

//Procedure ElementoDoTopo(P: TpPilha);
//          begin
//          if not PilhaVazia(P)then
//          begin
//               ElementoDoTopo:= P.Elem[P.topo];
//          else
//          end;

// =========== FUNÇÕES DO BANCO ===============

Function BancoVazio(B: TpBanco): boolean;
	begin
		BancoVazio:= (B.cheio = false);
	end;

Procedure ColocarNoBanco(cartax: TpBar; numcoluna: integer);
	begin
		banco[numcoluna].carta.valor:= cartax.carta.valor;
		banco[numcoluna].carta.naipe:= cartax.carta.naipe;
		banco[numcoluna].cheio:= true;
	end;

Function TirarDoBanco(numcoluna: integer): TpCarta;
	begin
			TirarDoBanco:= banco[numcoluna].carta;
			banco[numcoluna].cheio:= false;
	end;

// ========= POEM VALORES E NAIPES ===========

Procedure PreparaBaralho;
var
   i: integer;

begin
     for i:= 0 to 51 do
     begin
          baralho[i].carta.naipe:= (i div 13);
          baralho[i].carta.valor:= (i mod 13) + 1;
          baralho[i].ok := true;
     end;
end;

Procedure DarAsCartas;
var
    i, j, m,k: integer;

   begin
        i:= 0;
        k:= 0;
	randomize;
        while i<52 do
        begin
             j:= random(52);
             if (baralho[j].ok) then
             begin   
            	  Empilhar(coluna[k], baralho[j]);
	          baralho[j].ok:= false;
		  if (k = 7) then
			k := 0
		  else
		  	k:= k+1;
                  i:= i+1;
             end;
        end;

   end;
   
   Procedure MoverCarta;
   var 
		numcoluna, numcoluna2, bank: integer;		
		cartax: TpBar;
		cartaDest: TpBar;
	begin
		numcoluna := 10;
		while (numcoluna <> 0) and (numcoluna <> 1) and (numcoluna <> 2) and (numcoluna <> 3) and (numcoluna <> 4) and (numcoluna <> 5) and (numcoluna <> 6) and (numcoluna <> 7) and (numcoluna<> 9) do
		begin  
			writeln('> Digite o numero da COLUNA desejada (0-7). Digite 9 se quiser acessar o BANCO');
			write('> ');
			read(numcoluna);
		end;

		if (numcoluna = 9) then // BANCOOOOOOOOOOO
		begin
			bank := 10;
			while (bank <> 0) and (bank <> 1) and (bank <> 2) and (bank <> 3) do
			begin  
				writeln('> Digite o numero do BANCO desejado (0-3).');
				write('> ');
				read(bank);
			end;
			
			if not BancoVazio(banco[bank]) then
			begin
				cartax.carta := banco[bank].carta;
			end
			else
			begin
				writeln('> ERROR: dark hole detected! You are about to go to a parallel world!!!');
				delay(5000);
			end;
		
		end
		else
		begin
			cartax:= coluna[numcoluna].elem[coluna[numcoluna].topo];
		end;
 		
		numcoluna2 := 10;
		while (numcoluna2 <> 0) and (numcoluna2 <> 1) and (numcoluna2 <> 2) and (numcoluna2 <> 3) and (numcoluna2 <> 4) and (numcoluna2 <> 5) and (numcoluna2 <> 6) and (numcoluna2 <> 7) and (numcoluna2 <> 9) do
		begin  
			writeln('> Digite o numero da COLUNA desejada (0-7). Digite 9 se quiser acessar o BANCO');
			write('> ');
			read(numcoluna2);
		end;           
// checar para onde por(banco vazio), coluna, valor, naipe)		
		if (numcoluna2 = 9) and (numcoluna <> 9) then
		begin
			bank := 10;
			while (bank <> 0) and (bank <> 1) and (bank <> 2) and (bank <> 3) do
			begin  
				writeln('> Digite o numero do BANCO desejado (0-3).');
				write('> ');
				read(bank);
			end;
			if BancoVazio(banco[bank]) then
			begin
				ColocarNoBanco(cartax, bank);
				Desempilha(coluna[numcoluna]);
			end
			else
			begin
				writeln('> ERROR: come on! You are not blind, are you!?');
				delay(5000);
			end;
		end
		else
		begin
			if (numcoluna = 9) and (numcoluna2 = 9) then
			begin
				writeln('> ERROR: division by 0 ----------> YOU ARE ABOUT TO DIE!!');
				delay(5000);
			end
			else
			begin
				if (cartax.carta.valor = (coluna[numcoluna2].elem[coluna[numcoluna2].topo].carta.valor-1)) then
				begin
					cartaDest := coluna[numcoluna2].elem[coluna[numcoluna2].topo];
					if ((((cartax.carta.naipe mod 2) = 0) and ((cartaDest.carta.naipe mod 2) = 1)) or (((cartax.carta.naipe mod 2) = 1) and ((cartaDest.carta.naipe mod 2) = 0))) then
					begin
						if (numcoluna <> 9) then
						begin
							Desempilha(coluna[numcoluna]);
						end
						else
						begin
							TirarDoBanco(bank);
						end;
						Empilhar(coluna[numcoluna2], cartax);
					end
					else
					begin	
						writeln('> ERROR: r u fuckin kidding me!?');
						delay(5000);
					end;
				end
				else
				begin
					writeln('> ERROR: there are people in the world that want to watch the world BURN!!!');
					delay(5000);
				end;
			end;
		end;
	end;

Procedure verificaEmpilhaEnd;
var
	i, naipe: integer;
	cartaTopo: TpBar;
begin	
	for i:=0 to 7 do
	begin
		if (coluna[i].topo >= 0) then
		begin
			cartaTopo := coluna[i].elem[coluna[i].topo];
			naipe := cartaTopo.carta.naipe;
			if (((ending[naipe].topo = -1) and (cartaTopo.carta.valor = 1)) or ((ending[naipe].topo >= 0) and (ending[naipe].elem[ending[naipe].topo].carta.valor = cartaTopo.carta.valor -1))) then
			begin
				Desempilha(coluna[i]);
				Empilhar(ending[cartaTopo.carta.naipe], cartaTopo); 
			end;	
		end;
	end;
end;

Function verificaEnd: boolean;
var
	fim, i: integer;
begin
	fim := 0;
	for i:=0 to 3 do
	begin
		if pilhaCheia(ending[i]) then
		begin
			fim := fim + 1;
		end;
	end;

	if(fim = 4) then
	begin
		verificaEnd := true;
	end
	else
	begin
		verificaEnd := false;
	end;
end;

Procedure menu;
begin
	clrscr;
	writeln('================================================');
	writeln('=                   FREE CELL                  =');
	writeln('================================================');
	writeln();
	writeln();
	writeln('       > wait for me, GOKU! Mwahahhaha <');
	delay(5000);
end;

Procedure jogoGanho;
begin
	clrscr;
	writeln('         >YOU WON!!<');
	writeln('	(yet, Goku is dead)');
	delay(5000);
	clrscr;
end;
	

 begin
	menu;
	for i := 0 to 7 do
	begin
		InicializaPilha(coluna[i]);
	end;
	for i := 0 to 3 do
	begin
		InicializaPilha(ending[i]);
		ending[i].tammax := 12;
	end;
	PreparaBaralho;
	
	DarAsCartas;
	printJogo;

	while (not verificaEnd) do
	begin
		MoverCarta;
		verificaEmpilhaEnd;
		printJogo;
	end;

	jogoGanho;
	
 end.
