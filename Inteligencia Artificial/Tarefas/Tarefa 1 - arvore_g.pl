progenitor(teobaldo,leonel). 	
progenitor(ines,leonel). 		
progenitor(teobaldo,neusa).		
progenitor(ines,augusto).
progenitor(teobaldo,augusto).
progenitor(ines,neusa).
progenitor(neusa,greici).
progenitor(paulo,greici).
progenitor(neusa,willian).
progenitor(paulo,willian).
progenitor(augusto,wagner).
progenitor(ivali,wagner).
progenitor(lourdes,deividi).
progenitor(leonel,deividi).
progenitor(lourdes,silvia).
progenitor(leonel,silvia).

sexo(ines,feminino).			/*avó*/
sexo(lourdes,feminino).			/*mãe*/
sexo(silvia,feminino).			/*irmã*/	
sexo(neusa,feminino).			/*tia*/
sexo(greici,feminino).			/*prima*/
sexo(ivali,feminino).			/*tia casada com irmão do pai*/

sexo(teobaldo,masculino).		/*avô*/
sexo(leonel,masculino).			/*pai*/
sexo(deividi,masculino).		/*eu*/
sexo(paulo,masculino).			/*tio casada com irmã do pai*/
sexo(willian,masculino).		/*primo*/
sexo(augusto,masculino).		/*tio*/
sexo(wagner,masculino).			/*primo*/
     
irmao(X,Y):- progenitor(A,X), progenitor(A,Y), X\==Y, sexo(X,masculino).
irma(X,Y):- progenitor(A,X), progenitor(A,Y), X\==Y, sexo(X,feminino).

descendente(X,Y):- progenitor(X,Y).
descendente(X,Y):- progenitor(X,A), descendente(A,Y).

pai(X,Y):- progenitor(X,Y), sexo(X,masculino).
mae(X,Y):- progenitor(X,Y), sexo(X,feminino).
avô(X,Y):- progenitor(X,A), progenitor(A,Y), sexo(X,masculino).
avó(X,Y):- progenitor(X,A), progenitor(A,Y), sexo(X,feminino).
tia(X,Y):- irma(X,A), progenitor(A,Y).
tio(X,Y):- irmao(X,A), progenitor(A,Y).
primo(X,Y):-irmao(A,B), progenitor(A,X), progenitor(B,Y), X\==Y.
prima(X,Y):-irma(A,B), progenitor(A,X), progenitor(B,Y), X\==Y.