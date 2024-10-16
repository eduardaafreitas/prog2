Feito por Eduarda de Aguiar Freitas, GRR 20211799.

Primeiro trabalho pŕatico apresentado à disciplina de Programação II (CI-1002 2024/2) como requisito para obtenção de nota.
Título do trabalho: Estratégias de Comparação de Imagens.
Como objetivo do desenvolvimento, espera-se como resultado um programa executável em C chamado LBP com as seguintes funcionalidades:

1) Comparar uma imagem de teste com todas as imagens presentes em uma base de referência.
2) Gerar uma imagem LBP.

As funções presentes na biblioteca 'lbp.c' foram implementadas a partir das estruturas apresentadas no arquivo 'lbp.h' (estrutura LBP e estrutura image).

Logo após as estruturas serem alocadas adequadamente, segui o desenvolvimento das lógicas necessárias para a execução da funcionalidade 1:
-  Função alloc_pixels: a alocação de memória para a matriz de pixels presente na estrutura 'image *img_in' é iniciada
- Função read_image: lê e trata os dados do cabeçalho do arquivo, separando os tipos da imagem entre P2 e P5, onde cada tipo é tratado separadamente para popular a matriz de pixels através da função 'fill_pixels_p2' ou 'fill_pixels_p5'
- Ambas funções 'fill_pixels_p2' e 'fill_pixels_p5' percorrem o arquivo de imagem enviado pelo usuário e populam a matriz de pixels conforme o tipo da imagem

Depois da estrutura image estar devidamente alocada e inicializada, é realizada a cópia do cabeçalho para uma nova estrutura image chamada '*new_img', onde futuramente é utilizada para gerar a imagem LBP a partir da estrutura 'image *img_in'
- A matriz de pixels da estrutura 'new_img' é populada com base no cálculo para gerar imagens LBP apresentado na descrição do trabalho. Este cálculo é realizado na função 'math'

Após a matriz em 'new_img' estar preenchida, inicia-se o tratamento dela para a geração do histograma. É realizada a alocação da estrutura LBP para armazenar o histograma da imagem recebida do usuário.

- O histograma da imagem é gerado na função 'define_histogram'

Tendo o histograma da "imagem original", é iniciado o tratamento das imagens presentes no banco de dados através da função 'directory_read', onde cada arquivo de imagem recebe o mesmo tratamento feito na "imagem original", exceto que após fechar o arquivo do Banco de Dados, a memória alocada para as estruturas 'image' é liberada, mantendo apenas o histograma dela.

Com todos os histogramas gerados, o diretório do banco de dados é novamente percorrido, mas desta vez para abrir apenas os arquivos '.lbp' e realizar o cálculo de distancia euclidiana através da função 'euclidian_distance', que recebe como parâmetros o diretório a ser analizado, a estrutura LBP da "imagem original" (que contém o histograma da mesma), uma variável 'double' para armazenar o resultado da distancia euclidiana e uma string que armazenará o nome do arquivo com menor distancia euclidiana chamada 'shorter_distance'.

Depois de toda a execução, toda memória alocada é liberada.

A lógica da funcionalidade 2 é implementada com base no nome do arquivo de saída solicitado pelo usuário e é realizada na função 'out_img_generate', onde o arquivo de saída é escrito com base na matriz de pixels armazenada na estrutura 'new_img'.
