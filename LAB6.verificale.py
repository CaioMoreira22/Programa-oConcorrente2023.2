/* Disciplina: Programacao Concorrente */
/* Nome: Caio Moraes Santos Moreira */
/* DRE: 121050809*/
/* LAB 6 - Código */
/* -------------------------------------------------------------------*/

#Define funcoes para verificar a logica de execucao de uma aplicacao leitor/escritor

class LE:
	def __init__(self):
		self.escritores = 0
		self.leitores = 0
		self.esp_escr = 0

	def leitorBloqueado(self,id):
		'''Recebe o id do leitor. Verifica se a decisao de bloqueio esta correta.'''
		if(self.escritores==0):
			print("ERRO: leitor " + str(id) + " bloqueado quando nao ha escritores!")

	def escritorBloqueado(self,id):
		'''Recebe o id do escritor. Verifica se a decisao de bloqueio esta correta.'''
		if(self.escritores==0) and (self.leitores==0):
			print("ERRO: escritor " + str(id) + " bloqueado quando nao ha escritores nem leitores!")
		self.esp_escr+=1

	def leitorLendo(self,id):
		'''Recebe o id do leitor, verifica se pode ler e registra que esta lendo.'''
		if(self.escritores>0):
			print("ERRO: leitor " + str(id) + " esta lendo quando ha escritor escrevendo!")
		elif(self.esp_escr>0):
			print("ERRO: leitor " + str(id) + " esta lendo quando ha escritores na espera da fila!"))
		self.leitores+=1

	def escritorEscrevendo(self,id):
		'''Recebe o id do escritor, verifica se pode escrever e registra que esta escrevendo'''
		if(self.escritores>0 or self.leitores>0):
			print("ERRO: escritor " + str(id) + " esta escrevendo quando ha outro escritor ou leitores!")
		self.escritores+=1

	def leitorSaindo(self,id):
		'''Recebe o id do leitor e registra que terminou a leitura.'''
		self.leitores-=1

	def escritorSaindo(self,id):
		'''Recebe o id do escritor e registra que terminou a leitura.'''
		self.escritores-=1
	
	def escritorDesbloqueado(self,id):
	    '''Recebe o id do escritor e registra que esse escritor foi desbloqueado do bloqueio.'''
	    self.esp_escr-=1

##acrescentar uma sequencia de escritas e leituras
