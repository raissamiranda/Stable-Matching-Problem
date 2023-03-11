#include <iostream>
#include <queue>
#include <stack>
#include <string>
#include <vector>

// ****************************************** CLASSES
// ****************************************** Esta eh a interface de uma
// bicicleta
class bicicleta {
public:
  // Ela eh caracterizada por um id
  int id_bicicleta;
  // Distancias da bicicleta aos vertices de pessoas
  std::vector<int> distancias;
  // Um vetor de ids das pessoas preferidas
  std::vector<int> preferencias;
  // Variavel que informa se a bicicleta ja esta alocada com alguma pessoa
  bool esta_alocada = false;
  // id da pessoa que esta alocada a ela
  int id_pessoa_alocada;

  // Metodo que ordena os dois vetores de acordo com a distancia
  bicicleta() {}
  void ordena_distancias();
};

// Esta eh a interface de uma pessoa
class pessoa {
public:
  // Ela eh caracterizada por um id
  int id_pessoa;
  // Seu vetor de preferencias com os ids das bicicletas
  std::vector<int> pesos;
  // Vetor com os ids das bicicletas preferidas
  std::vector<int> ids_preferidos;
  // Numero de propostas que essa pessoa fez
  int n_propostas = 0;
  // Variavel que informa se pessoa esta alocada com alguma bicicleta
  bool esta_alocada = false;

  // Construtores
  pessoa() {}
  pessoa(int id) { id_pessoa = id; }
  void ordena_pesos();
};

// ****************************************** MAIN
// ****************************************** Prototipos funcoes utilizadas
std::vector<int> bfs(int fonte,
                     std::vector<std::vector<int>> &vetor_adjacencia);
std::vector<int> gale_shapley(std::vector<pessoa> &vetor_pessoas,
                              std::vector<bicicleta> &vetor_bicicletas);
bool prefere_a_primeira(int id_pessoa_propondo, int id_pessoa_alocada,
                        int id_bicicleta_proposta,
                        std::vector<bicicleta> vetor_bicicleta);
pessoa pessoa_com_id(int id_pessoa_rejeitada,
                     std::vector<pessoa> &vetor_pessoas);

// Funcao principal
int main() {

  // Leitura do numero de alocacoes que serao realizadas e as dimensoes do mapa
  int num_alocacoes = 0, num_linhas = 0, num_colunas = 0;
  std::cin >> num_alocacoes;
  std::cin >> num_linhas >> num_colunas;

  // ************************************* CRIACAO LISTA ADJACENCIA
  // ************************************* Criacao da vetor de adjacencia pra
  // cada bicicleta (vertices)
  int num_vertices = num_linhas * num_colunas;
  // Vetor com todos os caracteres do mapa
  std::vector<char> vetor_mapa;
  for (int i = 0; i < num_vertices; i++) {
    char v;
    std::cin >> v;
    vetor_mapa.push_back(v);
  }
  // Vetores de adjacencia, indices e ids de pessoas e bicicletas
  std::vector<std::vector<int>> vetor_adjacencia(num_vertices);
  std::vector<int> indices_bicicletas;
  std::vector<int> indices_pessoas;
  /* Analise dos vizinhos validos seguirao a seguinte logica:
      (*) Os vizinhos de vetor_mapa[i] sao:
                       _______________
                      |vetor_mapa[i-n]|
                      |_______________|
    ______________      _____________      _______________
   |vetor_mapa[i-1|    |vetor_mapa[i]|    |vetor_mapa[i+1]|
   |______________|    |_____________|    |_______________|
                       _______________
                      |vetor_mapa[i+n]|
                      |_______________|
      Sendo:
          - n = num_colunas
          - primeira linha = i - num_colunas < 0
          - ultima linha = i + num_colunas > num_vertices
          - primeira coluna = i%num_colunas = 0
          - ultima coluna = (i+1)%num_colunas = 0
  */
  for (int i = 0; i < num_vertices; i++) {
    // Vetor que guarda o indice e id das pessoas, na mesma posicao
    if (vetor_mapa[i] > 96 && vetor_mapa[i] < 107) {
      indices_pessoas.push_back(i);
    }
    // Vetor que guarda o indice e id das bicicletas, na mesma posicao
    if (vetor_mapa[i] > 47 && vetor_mapa[i] < 58) {
      indices_bicicletas.push_back((int)i);
    }
    // Coloca no vetor da posicao i do vetor de adjacencia seus vizinhos
    // alcancaveis: Vertices que nao sao da primeira linha
    if (i - num_colunas >= 0 && vetor_mapa[i - num_colunas] != '-') {
      vetor_adjacencia[i].push_back(i -
                                    num_colunas); // Adiciona vizinho de cima
    }
    // Vertices que nao sao da ultima linha
    if (i + num_colunas < num_vertices && vetor_mapa[i + num_colunas] != '-') {
      vetor_adjacencia[i].push_back(i +
                                    num_colunas); // Adiciona vizinho de baixo
    }
    // Vertices que nao sao da primeira coluna
    if (i % num_colunas != 0 && vetor_mapa[i - 1] != '-') {
      vetor_adjacencia[i].push_back(i - 1); // Adiciona vizinho da esquerda
    }
    // Vertices que nao sao da ultima coluna
    if ((i + 1) % num_colunas != 0 && vetor_mapa[i + 1] != '-') {
      vetor_adjacencia[i].push_back(i + 1); // Adiciona vizinho da direita
    }
  }
  /*
  // (TESTE) Printar vetor de adjacencia
  for(int i = 0; i < num_vertices; i++){
      std::cout << "Vertice[" << i << "]: "<< vetor_mapa[i] << "   Vizinhos: ";
      for(int j = 0; j < vetor_adjacencia[i].size(); j++){
          std::cout << vetor_adjacencia[i][j] << " ";
      }
      std::cout << std::endl;
  } std::cout << std::endl;

  // Printa vertices das bicicletas
  std:: cout << "vertices bicicletas: " << std::endl;
  for(int i = 0; i < indices_bicicletas.size(); i++){
      std::cout << indices_bicicletas[i] << " ";
  } std::cout << std::endl << std::endl;

  // Printa vertices das bicicletas
  std:: cout << "vertices pessoas: " << std::endl;
  for(int i = 0; i < indices_pessoas.size(); i++){
      std::cout << indices_pessoas[i] << " ";
  } std::cout << std::endl;
  std::cout << std::endl;*/

  // ************************************* CALCULO DISTANCIAS
  // ************************************* Cria um vetor de bicicleta para
  // guardar o id das bicicletas, a distancia ate cada pessoa e o id dessas
  // pessoas
  std::vector<bicicleta> vetor_bicicletas;
  for (int i = 0; i < num_alocacoes; i++) {
    bicicleta pb;
    // Pega os vertices com bicicleta
    int vertice_bicicleta = indices_bicicletas[i];
    // Calcula o id da bicicleta
    int id_bici = vetor_mapa[vertice_bicicleta] - 48;
    pb.id_bicicleta = id_bici;
    // Calcula a distancia dessa bicicleta ate todos os vertices
    std::vector<int> dist = bfs(vertice_bicicleta, vetor_adjacencia);
    // Pega os vertices com pessoas
    for (int j = 0; j < num_alocacoes; j++) {
      int vertice_pessoa = indices_pessoas[j];
      // Calcula o id da pessoa
      int id_pessoa = vetor_mapa[vertice_pessoa] - 96;
      pb.preferencias.push_back(id_pessoa);
      pb.distancias.push_back(dist[vertice_pessoa]);
    }
    vetor_bicicletas.push_back(pb);
  }

  // Ordena o vetor de ids preferidos de cada bicicleta de acordo com a
  // distancia
  for (int i = 0; i < vetor_bicicletas.size(); i++) {
    vetor_bicicletas[i].ordena_distancias();
  }
  /*
  // (TESTE) Printar o vetor bicicleta
  for(int i = 0; i < vetor_bicicletas.size(); i++){
      std::cout << "ID_BICICLETA: " << vetor_bicicletas[i].id_bicicleta <<
  std::endl; std::cout << "IDS PESSOAS PREFERIDAS: "; for(int j = 0;
  j<vetor_bicicletas[i].preferencias.size(); j++){ std::cout <<
  vetor_bicicletas[i].preferencias[j] << " ";
      }
      std::cout << std::endl;
      std::cout << "DISTANCIAS: ";
      for(int j = 0; j<vetor_bicicletas[i].distancias.size(); j++){
          std::cout << vetor_bicicletas[i].distancias[j] << " ";
      }
      std::cout << std::endl << std::endl;
  }*/

  // ************************************* CALCULO PESOS
  // ************************************* Criacao do vetor de pessoas de acordo
  // com suas preferencias
  std::vector<pessoa> vetor_visitantes;
  // Para cada pessoa
  for (int i = 0; i < num_alocacoes; i++) {
    int id_p = i + 1;
    pessoa p(id_p);
    std::vector<int> vetor_pesos;

    // Guardar em um vetor o peso de cada bicicleta pra essa pessoa
    for (int j = 0; j < num_alocacoes; j++) {
      int peso;
      std::cin >> peso;
      vetor_pesos.push_back(peso);
    }
    p.pesos = vetor_pesos;
    p.ordena_pesos();

    vetor_visitantes.push_back(p);

    /*
    // (TESTE)  Imprimir pessoas e suas preferencias de bicicleta pelos pesos
    std::cout << "ID PESSOA: " << p.id_pessoa << std::endl;
    std::cout << "IDS BICICLETAS PREFERIDAS: ";
    for(int i = 0; i < p.ids_preferidos.size(); i++){
        std::cout << p.ids_preferidos[i] << " ";
    } std::cout << std::endl;
    std::cout << "PESOS: ";
    for(int i = 0; i < p.pesos.size(); i++){
        std::cout << p.pesos[i] << " ";
    } std::cout << std::endl << std::endl;*/
  }

  // ************************************* GALE-SHAPLEY
  // *************************************
  std::vector<int> casamentos =
      gale_shapley(vetor_visitantes, vetor_bicicletas);

  // Printa o vetor de casamentos final
  for (int i = 0; i < casamentos.size(); i++) {
    char pessoa = casamentos[i] + 96;
    std::cout << pessoa << " " << i << std::endl;
  }

  return 0;
}

std::vector<int> bfs(int vertice_fonte,
                     std::vector<std::vector<int>> &vetor_adjacencia) {
  // Vetor booleanos de vertices visitados
  std::vector<bool> visitados(vetor_adjacencia.size(), 0);
  // Vetor com a distancia da fonte ate cada vertice
  std::vector<int> distancias(vetor_adjacencia.size(), 99999);
  // O vertice fonte eh visitado
  visitados[vertice_fonte] = true;
  // Vertice fonte tem distancia 0 ate ele mesmo
  distancias[vertice_fonte] = 0;
  // Fila de vertices que serao visitados possui apenas o vertice fonte
  std::queue<int> fila_vertices;
  fila_vertices.push(vertice_fonte);
  // Enquanto a fila nao estiver vazia, os vertices serao visitados
  while (!fila_vertices.empty()) {
    // Retira o primeiro vertice da fila
    int primeiro = fila_vertices.front();
    fila_vertices.pop();
    // Adiciona na fila os vizinhos desse vertice verificando sua lista de
    // adjacencia
    for (int i = 0; i < vetor_adjacencia[primeiro].size(); i++) {
      int vizinho = vetor_adjacencia[primeiro][i];
      if (!visitados[vizinho]) {
        fila_vertices.push(vizinho);
        visitados[vizinho] = true;
        // Calcula a distancia como a distancia do seu pai + 1
        distancias[vizinho] = distancias[primeiro] + 1;
      }
    }
  }
  return distancias;
}

// ************************************* IMPLEMENTACOES METODOS
// ************************************* Metodo que ordena os vetores de acordo
// com a distancia
void bicicleta::ordena_distancias() {
  std::vector<int> novas_distancias;
  std::vector<int> novas_preferencias;

  for (int i = 0; i < distancias.size(); i++) {
    int menor_distancia = 9999;
    int id_pessoa = -1, indice_menor = -1;
    for (int j = 0; j < distancias.size(); j++) {
      if (distancias[j] < menor_distancia) {
        indice_menor = j;
        menor_distancia = distancias[j];
        id_pessoa = preferencias[j];
      }
      // Se a distancia ate duas pessoas for a mesma, a preferencia eh para a de
      // menor id
      else if (distancias[j] == menor_distancia) {
        if (preferencias[j] < preferencias[indice_menor]) {
          indice_menor = j;
          menor_distancia = distancias[j];
          id_pessoa = preferencias[j];
        }
      }
    }
    novas_distancias.push_back(menor_distancia);
    novas_preferencias.push_back(id_pessoa);
    distancias[indice_menor] = 9999;
    preferencias[indice_menor] = -1;
  }
  distancias = novas_distancias;
  preferencias = novas_preferencias;
}

// Metodo que ordena os vetores de acordo com o peso
void pessoa::ordena_pesos() {
  std::vector<int> novos_pesos;
  std::vector<int> novas_preferencias;

  for (int i = 0; i < pesos.size(); i++) {
    int maior_peso = -1;
    int id_bike = -1, indice_maior = -1;
    for (int j = 0; j < pesos.size(); j++) {
      if (pesos[j] > maior_peso) {
        indice_maior = j;
        maior_peso = pesos[j];
        id_bike = j;
      }
      // Se o peso de duas bicicletas for o mesmo, a preferencia eh para a de
      // menor id
      else if (pesos[j] == maior_peso) {
        if (ids_preferidos[j] < ids_preferidos[indice_maior]) {
          indice_maior = j;
          maior_peso = pesos[j];
          id_bike = j;
        }
      }
    }
    novos_pesos.push_back(maior_peso);
    novas_preferencias.push_back(id_bike);
    pesos[indice_maior] = -2;
  }
  pesos = novos_pesos;
  ids_preferidos = novas_preferencias;
}

std::vector<int> gale_shapley(std::vector<pessoa> &vetor_pessoas,
                              std::vector<bicicleta> &vetor_bicicletas) {

  // Pilha de pessoas que nao estao alocadas
  // Coloca todas as pessoas na pilha
  std::vector<pessoa> pilha_pessoas;
  for (int i = 0; i < vetor_pessoas.size(); i++) {
    pessoa p = vetor_pessoas[i];
    pilha_pessoas.push_back(p);
  }

  // Vetor com casamentos
  std::vector<int> casamento_bicicleta_pessoa(vetor_pessoas.size(), -1);
  // Vetor com pessoas ja alocadas
  std::vector<pessoa> vetor_pessoas_alocadas;

  // Enquanto houver pessoas nao alocadas
  while (!pilha_pessoas.empty()) {
    // Seleciona a primeira pessoa da pilha pra propor
    pessoa &pessoa_propondo = pilha_pessoas.back();
    // Seleciona a primeira bicicleta da lista de preferencias dela
    int id_bicicleta_proposta =
        pessoa_propondo.ids_preferidos[pessoa_propondo.n_propostas];
    // Se a bicicleta nao estiver alocada a ninguem
    if (casamento_bicicleta_pessoa[id_bicicleta_proposta] <
        0) {                               // Bicicleta nao alocada
      pessoa_propondo.esta_alocada = true; // Aloca pessoa (acho q da pra tirar)
      casamento_bicicleta_pessoa[id_bicicleta_proposta] =
          pessoa_propondo.id_pessoa; // Aloca bicicleta
      pessoa_propondo.n_propostas++;
      vetor_pessoas_alocadas.push_back(pilha_pessoas.back());
      pilha_pessoas.pop_back();

      // Se a bicicleta ja estiver alocada, analisa a preferencia dela
    } else {
      // pessoa_que_ja_esta_com_a_bike =
      // o_quanto_pessoa_que_ja_esta_com_a_bike_prefere_essa_bike =
      // pessoa_que_ja_esta_com_a_bike.pesos[] Pessoa que já está prefere mais
      // o_quanto_pessoa_propondo_prefere_essa_bike >=
      if (!prefere_a_primeira(pessoa_propondo.id_pessoa,
                              casamento_bicicleta_pessoa[id_bicicleta_proposta],
                              id_bicicleta_proposta, vetor_bicicletas)) {
        pessoa_propondo.n_propostas++;

        // Pressoa que propôs prefere mais
      } else {
        vetor_pessoas_alocadas.push_back(pilha_pessoas.back());
        pilha_pessoas.pop_back();
        pessoa_propondo.n_propostas++;
        pessoa pessoa_rejeitada =
            pessoa_com_id(casamento_bicicleta_pessoa[id_bicicleta_proposta],
                          vetor_pessoas_alocadas);
        // Tirar a pessoa do vetor de p alocadas
        // Por de volta na pilha de pessoas
        pessoa_rejeitada.esta_alocada = false;
        pilha_pessoas.push_back(pessoa_rejeitada);
        casamento_bicicleta_pessoa[id_bicicleta_proposta] =
            pessoa_propondo.id_pessoa;
      }
    }
  }
  return casamento_bicicleta_pessoa;
}

// Procura na lista de preferencias da bicicleta qual pessoa aparece primeiro
bool prefere_a_primeira(int id_pessoa_propondo, int id_pessoa_alocada,
                        int id_bicicleta_proposta,
                        std::vector<bicicleta> vetor_bicicleta) {
  for (int i = 0; i < vetor_bicicleta.size(); i++) {
    if (vetor_bicicleta[id_bicicleta_proposta].preferencias[i] ==
        id_pessoa_propondo) {
      return true;
    } else if (vetor_bicicleta[id_bicicleta_proposta].preferencias[i] ==
               id_pessoa_alocada) {
      return false;
    }
  }
  return false;
}

// Retorna uma pessoa de acordo com seu atributo
pessoa pessoa_com_id(int id_pessoa_rejeitada,
                     std::vector<pessoa> &vetor_pessoas) {
  for (auto it = vetor_pessoas.begin(); it != vetor_pessoas.end(); it++) {
    if (it->id_pessoa == id_pessoa_rejeitada) {
      pessoa encontrada = *it;
      vetor_pessoas.erase(it);
      return encontrada;
    }
  }
  std::cout << "ERRO\n" << vetor_pessoas.size() << std::endl;
  exit(1);
}
