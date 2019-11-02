# Objetivo #
Encontrar a rota e o plano de coleta de máximo benefício, com as seguintes condições:
- Um mochileiro começa da cidade inicial e retorna para a mesma cidade. Não é obrigatória a visita em TODAS as cidades.
- A cidade pode ser visitada por mais de um mochileiro.
- Um item só pode ser pego por um mochileiro.
- Há um aluguel R por unidade de tempo com a mochila.
- A capacidade é de uma mochila coletiva.
- A velocidade do mochileiro depende do peso dos itens que carrega na mochila, variando linearmente entre uma velocidade ḿaxima vmax quando a mochila est́a vazia e uma velocidade ḿınima vmin quando a mochila est́a cheia.

# Uma solução deve setar: #
- thief.items
- thief.route
- thief.backpack_weight
- caught_items
- used_capacity

# Para calibrar os parâmetros #
- Gerar o executável
        make clean
        make
- Colocar o executável na pasta bin
- Rodar o irace
        sh run_irace.sh
- As instâncias de teste ficam na pasta bin
- Os parâmetros testados no arquivo parameters.txt
- Algumas outras configurações são encontradas em scenario.txt