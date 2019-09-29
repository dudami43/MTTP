# Objetivo #
Encontrar a rota e o plano de coleta de máximo benefício, com as seguintes condições:
- Um mochileiro começa da cidade inicial e retorna para a mesma cidade. Não é obrigatória a visita em TODAS as cidades.
- A cidade pode ser visitada por mais de um mochileiro.
- Um item só pode ser pego por um mochileiro.
- Há um aluguel R por unidade de tempo com a mochila.
- A capacidade é de uma mochila coletiva.
- A velocidade do mochileiro depende do peso dos itens que carrega na mochila, variando linearmente entre uma velocidade ḿaxima vmax quando a mochila est́a vazia e uma velocidade ḿınima vmin quando a mochila est́a cheia.

# Estrutura #
#### classe instancia ####
	grafo (lista de lista de pair<vertice, peso>) com 'dimension' vertices e uma aresta entre cada par de vertices
	capacidade da mochila
	[float]min speed
	[float]max speed
	[int]num mochileiros
	[float]aluguel
	- o índice dos itens a seguir indica o mochileiro referente a cada um
	lista[lista] de rotas
	lista[lista] de itens pegos
#### classe vertice ####
*Ao ler cada vértice, atualizar o peso no grafo*
	
	lista de itens presentes no lugar
#### classe item ####
	valor
	peso

# Avaliação #

	total = 0
	para cada mochileiro:
		valor = 0
		aluguel = 0
		f_v = (vmax - vmin)/capacidade da mochila

		para i de 2 a n: //n = tamanho da rota
			para j de 1 a mi: //mi = numero de itens na cidade
				valor += peso do item*valor do item

		para i de 1 a n-1: //n = tamanho da rota
			aluguel += R*(((distância entre i e i+1)/(vmax - (f_v*peso da mochila ao sair de i)))
					 +  ((distância entre n e 1)/(vmax - (f_v*peso da mochila ao sair de n))))

		valor -= aluguel
		total += valor
