*Objetivo*
Encontrar a rota e o plano de coleta de máximo benefício, com as seguintes condições:
- Um mochileiro começa da cidade inicial e retorna para a mesma cidade. Não é obrigatória a visita em TODAS as cidades.
- A cidade pode ser visitada por mais de um mochileiro.
- Um item só pode ser pego por um mochileiro.
- Há um aluguel R por unidade de tempo com a mochila.
- A capacidade é de uma mochila coletiva.
- A velocidade do mochileiro depende do peso dos itens que carrega na mochila, variando linearmente entre uma velocidade ḿaxima vmax quando a mochila est́a vazia e uma velocidade ḿınima vmin quando a mochila est́a cheia.

classe instancia
	grafo (lista de lista de pair<vertice, peso>)
	 - 'dimension' vertices e uma aresta entre cada par de vertices
	capacidade da mochila
	[float]min speed
	[float]max speed
	[int]num mochileiros
	[float]aluguel
	- o índice dos itens a seguir indica o mochileiro referente a cada um
	lista[lista] de rotas
	lista[lista] de itens pegos
classe vertice - Ao ler cada vértice, atualizar o peso no grafo
	[int]indice
	lista[bool] de itens presentes no lugar
classe item
	indice
	valor
	peso