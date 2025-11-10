//* * Favor ler o README.md para melhor entendimento
// Sistema de gerenciamento de Idiomas
// André Santos Gomes
// Bernardo Abrahão Mantovani

#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>

using namespace std;

// ? Estruturas de dados

struct idioma
{
    string nome, famLing, principalPais;
    int id = 0;
    double numfalantes = 0;
    bool del = false;   // Flag usada para ignorar campos deletados na hora de salvar no arquivo
    bool valido = true; // Flag usada para sinalizar que o idioma retornado é proveniente de um erro
};

// Lista para idiomas - Recomenda-se usar o construtor newArrayListIdioma para instanciar
struct arrayListIdioma
{
    int capacidade = 0, tamanho = 0;
    idioma *db = nullptr;
    bool valido = true;
};

// ? Controle de erros na criação de instâncias das Estruturas de dados

idioma error()
{
    idioma error;
    error.valido = false;
    return error;
}

arrayListIdioma errorArray()
{
    arrayListIdioma newInst;
    newInst.valido = false;
    newInst.db = nullptr;
    newInst.capacidade = 0;
    newInst.tamanho = 0;

    return newInst;
}

// ? Controle de instâncias de arrayListIdioma

// Construtores de arrayListIdioma - Garante instâncias inicializadas
arrayListIdioma newArrayListIdioma(int size)
{
    arrayListIdioma newInst;
    if (size < 0)
    {
        newInst = errorArray();
    }
    else
    {
        newInst.db = new idioma[size];
        newInst.capacidade = size;
        newInst.tamanho = 0;
    }

    return newInst;
}

// Usado para cópias
arrayListIdioma newArrayListIdioma(int size, int tamanho, idioma *db)
{
    if (db == nullptr || size < tamanho || size < 0 || tamanho < 0)
        return errorArray();

    arrayListIdioma newInst;
    newInst.db = db;
    newInst.capacidade = size;
    newInst.tamanho = tamanho;

    return newInst;
}

// Limpa a memória quando chega ao fim da vida de uma instância de arrayListIdioma
void clearArrayListIdioma(arrayListIdioma &vet)
{
    if (vet.db != nullptr)
        delete[] vet.db;
}

//? CRUD

// Aumenta o espaço do vetor conforme o necessário. Obs: Libera a memória do vetor antigo, in-place
void resize(arrayListIdioma &vet)
{
    if (vet.capacidade < 0)
        vet.capacidade = 0;

    if (vet.tamanho > vet.capacidade)
        vet.tamanho = vet.capacidade;

    idioma *dbNovo = new idioma[vet.capacidade + 10];
    for (int i = 0; i < vet.tamanho; i++)
        dbNovo[i] = vet.db[i];

    vet.capacidade += 10;
    clearArrayListIdioma(vet);

    vet.db = dbNovo;
    vet.valido = true;
}

// Troca instancias de idioma dentro do vetor por outras
// Considera que o vetor está ordenado com base no id
idioma update(arrayListIdioma &lista, int at, idioma idiomaNovo)
{
    if (!lista.valido || at >= lista.tamanho)
        return error();

    lista.db[at] = idiomaNovo;
    return idiomaNovo;
}

// Adiciona um idioma no final do vetor
// Considera que o vetor está ordenado com base no id
idioma add(arrayListIdioma &lista, idioma idiomaNovo)
{
    if (!idiomaNovo.valido || lista.tamanho < 0)
        return error();

    idiomaNovo.id = lista.db[lista.tamanho - 1].id + 1;

    while (lista.tamanho >= lista.capacidade)
        resize(lista);

    lista.db[lista.tamanho] = idiomaNovo;
    lista.tamanho++;

    return idiomaNovo;
}

// Marca um idioma como deletado
// Considera que o vetor está ordenado com base no id
idioma removeIdioma(arrayListIdioma &lista, int id)
{
    if (lista.tamanho >= id)
    {
        lista.db[id - 1].del = true;
        return lista.db[id - 1];
    }

    return error();
}

// Acha um idioma pelo seu Identificador
// OBS: considera-se que o vetor esteja ordenado de acordo com o atributo específico da busca
idioma findById(arrayListIdioma &lista, int id)
{
    if (id > lista.tamanho || id <= 0)
        return error();

    return lista.db[id - 1];
}

//? Busca Binária

// Acha a primeira instância no vetor - retorna -1 caso não encontre
// OBS: considera-se que o vetor está ordenado de acordo com o atributo específico da busca
int findFirst(const arrayListIdioma &arr, string target, bool param)
{
    int left = 0, right = arr.tamanho - 1;
    int result = -1;

    // Implementação iterativa de busca binária
    while (left <= right)
    {
        int mid = left + (right - left) / 2;

        if (param)
        {
            if (arr.db[mid].famLing == target)
            {
                result = mid;
                // Anda para a esquerda mesmo que o valor tenha sido achado. Assim achando sempre a instância mais à esquerda
                right = mid - 1;
            }
            else if (arr.db[mid].famLing < target)
                left = mid + 1;
            else
                right = mid - 1;
        }
        else
        {
            if (arr.db[mid].principalPais == target)
            {
                result = mid;
                // Anda para a esquerda mesmo que o valor tenha sido achado. Assim achando sempre a instância mais à esquerda
                right = mid - 1;
            }
            else if (arr.db[mid].principalPais < target)
                left = mid + 1;
            else
                right = mid - 1;
        }
    }
    return result;
}

// Acha a última instância no vetor. Retorna -1 caso falhe em encontrar
// OBS: considera-se que o vetor está ordenado de acordo com o atributo específico da busca
int findLast(const arrayListIdioma &arr, string target, bool param)
{
    int left = 0, right = arr.tamanho - 1;
    int result = -1;

    // Implementação iterativa de busca binária
    while (left <= right)
    {
        int mid = left + (right - left) / 2;

        if (param)
        {
            if (arr.db[mid].famLing == target)
            {
                result = mid;
                // Anda para a direita mesmo que o valor tenha sido achado. Assim achando sempre a instância mais à direita
                left = mid + 1;
            }
            else if (arr.db[mid].famLing < target)
                left = mid + 1;
            else
                right = mid - 1;
        }
        else
        {
            if (arr.db[mid].principalPais == target)
            {
                result = mid;
                // Anda para a direita mesmo que o valor tenha sido achado. Assim achando sempre a instância mais à direita
                left = mid + 1;
            }
            else if (arr.db[mid].principalPais < target)
                left = mid + 1;
            else
                right = mid - 1;
        }
    }
    return result;
}

// Busca binária válida para Principal País e Grupo Linguístico - Retorna um grupo de resultados válidos
// ppaisOuFamling = false -> ppais, true -> famLing - Aglutinação de duas funções em uma
// OBS: considera-se que o vetor está ordenado de acordo com o atributo específico da busca
arrayListIdioma binSearchGroups(const arrayListIdioma &lista, string target, bool ppaisOuFamling)
{
    int primeiraInst = findFirst(lista, target, ppaisOuFamling);
    int ultimaInst = findLast(lista, target, ppaisOuFamling);

    int range = ultimaInst + 1 - primeiraInst;
    int deletados = 0;

    idioma *correspondencias = new idioma[range];

    if (range == 0 || primeiraInst < 0 || ultimaInst < 0)
    {
        arrayListIdioma a = newArrayListIdioma(-1);
        return a;
    }

    if (primeiraInst > -1)
    {
        for (int i = 0; i < range; i++)
        {
            if (!lista.db[i + primeiraInst].del)
                correspondencias[i] = lista.db[i + primeiraInst];
            else
                deletados++;
        }
    }

    // Diminuindo o vetor caso tenha algum idioma deletado nele
    if (deletados)
    {
        int j = 0;
        idioma *correspondenciasResize = new idioma[range - deletados];
        for (int i = 0; i < range; i++)
            if (!correspondencias[i].del)
            {
                correspondenciasResize[j] = correspondencias[i];
                j++;
            }

        delete[] correspondencias;
        correspondencias = correspondenciasResize;
        range = range - deletados;
    }

    arrayListIdioma retorno = newArrayListIdioma(range, range, correspondencias);

    return retorno;
}

// Faz exatamente o que parece que faz
// OBS: considera-se que o vetor está ordenado de acordo com o atributo específico da busca
idioma binSearchNome(arrayListIdioma &lista, string target)
{
    int left = 0;
    int right = lista.tamanho - 1;

    while (left <= right)
    {
        int mid = left + (right - left) / 2;

        if (lista.db[mid].nome == target && !lista.db[mid].del)
            return lista.db[mid];

        else if (lista.db[mid].nome < target)
            left = mid + 1;

        else
            right = mid - 1;
    }

    return error();
}

//? Organização dos dados no vetor

// Controle de partição quicksort. OBS: in-place.
// Opc -> escolha entre os atributos `nome`, `famLing` e `ppais`: 0, 1, 2 respectivamente
int partitionString(arrayListIdioma &arr, int left, int right, int opc)
{
    string pivot;
    int sortedIndex = left - 1;

    switch (opc)
    {
    case 0:
        pivot = arr.db[right].nome;
        for (int j = left; j < right; j++)
        {
            if (arr.db[j].nome < pivot)
            {
                sortedIndex++;
                swap(arr.db[sortedIndex], arr.db[j]);
            }
        }

        break;

    case 1:
        pivot = arr.db[right].famLing;
        for (int j = left; j < right; j++)
        {
            if (arr.db[j].famLing < pivot)
            {
                sortedIndex++;
                swap(arr.db[sortedIndex], arr.db[j]);
            }
        }

        break;

    case 2:
        pivot = arr.db[right].principalPais;
        for (int j = left; j < right; j++)
        {
            if (arr.db[j].principalPais < pivot)
            {
                sortedIndex++;
                swap(arr.db[sortedIndex], arr.db[j]);
            }
        }

        break;

    default:
        return -1;

        break;
    }

    swap(arr.db[sortedIndex + 1], arr.db[right]);
    return sortedIndex + 1;
}

// É exatamente o que parece. OBS: in-place
// Opc -> escolha entre os atributos `nome`, `famLing` e `ppais`: 0, 1, 2 respectivamente
void quicksortString(arrayListIdioma &arr, int left, int right, int opc = 0)
{
    if (opc >= 0 && opc <= 2)
        if (left < right)
        {
            int pivot = partitionString(arr, left, right, opc);

            if (pivot < 0)
                return;

            quicksortString(arr, left, pivot - 1, opc);
            quicksortString(arr, pivot + 1, right, opc);
        }
}

// Controle de partição quicksort. OBS: in-place.
int partitionId(arrayListIdioma &arr, int left, int right)
{
    int pivot = arr.db[right].id;
    int sortedIndex = left - 1;

    for (int j = left; j < right; j++)
    {
        if (arr.db[j].id < pivot)
        {
            sortedIndex++;
            swap(arr.db[sortedIndex], arr.db[j]);
        }
    }

    swap(arr.db[sortedIndex + 1], arr.db[right]);
    return sortedIndex + 1;
}

// É exatamente o que parece. OBS: in-place
void quicksortId(arrayListIdioma &arr, int left, int right)
{
    if (left < right)
    {
        int pivot = partitionId(arr, left, right);

        quicksortId(arr, left, pivot - 1);
        quicksortId(arr, pivot + 1, right);
    }
}

// Controle de partição quicksort. OBS: in-place.
int partitionNumFal(arrayListIdioma &arr, int left, int right, bool cres)
{
    double pivot = arr.db[right].numfalantes;
    int sortedIndex = left - 1;

    if (cres)
        for (int j = left; j < right; j++)
        {
            if (arr.db[j].numfalantes < pivot)
            {
                sortedIndex++;
                swap(arr.db[sortedIndex], arr.db[j]);
            }
        }
    else
        for (int j = left; j < right; j++)
        {
            if (arr.db[j].numfalantes > pivot)
            {
                sortedIndex++;
                swap(arr.db[sortedIndex], arr.db[j]);
            }
        }

    swap(arr.db[sortedIndex + 1], arr.db[right]);
    return sortedIndex + 1;
}

// É exatamente o que parece. OBS: in-place
// cres -> true para crescente, false para decrescente
void quicksortNumFal(arrayListIdioma &arr, int left, int right, bool cres)
{
    if (left < right)
    {
        int pivot;

        pivot = partitionNumFal(arr, left, right, cres);

        quicksortNumFal(arr, left, pivot - 1, cres);
        quicksortNumFal(arr, pivot + 1, right, cres);
    }
}

//? Interface com o Usuário

// Escreve os dados o idioma escolhido em uma única linha
void printIdioma(idioma &i, int atributoEspecifico)
{
    switch (atributoEspecifico)
    {
    case 0:
        cout << "Identificador: " << i.id << "; "
             << ' ' << "Nome: " << i.nome << "; "
             << ' ' << "Família Linguistica: " << i.famLing << "; "
             << fixed << setprecision(4) << ' ' << "Número de Falantes (Em milhões): " << i.numfalantes << "; "
             << ' ' << "Principal País: " << i.principalPais << "; "
             << endl;
        break;
    case 1:
        cout << "Identificador: " << i.id << "; "
             << ' ' << "Família Linguistica: " << i.famLing << "; "
             << ' ' << "Nome: " << i.nome << "; "
             << fixed << setprecision(4) << ' ' << "Número de Falantes (Em milhões): " << i.numfalantes << "; "
             << ' ' << "Principal País: " << i.principalPais << "; "
             << endl;
        break;
    case 2:
        cout << "Identificador: " << i.id << "; "
             << ' ' << "Principal País: " << i.principalPais << "; "
             << ' ' << "Nome: " << i.nome << "; "
             << ' ' << "Família Linguistica: " << i.famLing << "; "
             << fixed << setprecision(4) << ' ' << "Número de Falantes (Em milhões): " << i.numfalantes << "; "
             << endl;
        break;
    case 3:
        cout << "Identificador: " << i.id << "; "
             << fixed << setprecision(4) << ' ' << "Número de Falantes (Em milhões): " << i.numfalantes << "; "
             << ' ' << "Nome: " << i.nome << "; "
             << ' ' << "Família Linguistica: " << i.famLing << "; "
             << ' ' << "Principal País: " << i.principalPais << "; "
             << endl;
        break;
    default:
        break;
    }
}

// Mostra o array na tela, a ordem dos dados depende do valor do parâmetro `atributoEspecifico`
// valores possíveis 0 -> id, 1 -> FamLing, 2 -> pPais, 3 -> numFal
void printArr(arrayListIdioma &lista, int start, int finish, int atributoEspecifico)
{
    // Checagem para invalidação
    if (start < 0 || start > lista.tamanho - 1)
        start = 0;

    if (finish < start || finish > lista.tamanho)
        finish = lista.tamanho;

    if (atributoEspecifico < 0 || atributoEspecifico > 3)
        atributoEspecifico = 0;

    if (!lista.valido)
    {
        cout << "Lista inválida! Não é possível acessá-la" << endl;
        return;
    }

    // Loop que pula instâncias inválidas ou deletadas
    int escritos = 0;
    for (int i = start; i < finish && i < lista.tamanho; i++)
    {
        if (!lista.db[i].del && lista.db[i].valido)
        {
            printIdioma(lista.db[i], atributoEspecifico);
            escritos++;
        }
    }

    if (!escritos)
        cout << "Nenhum idioma encontrado!" << endl;
}

// Limpa o buffer do cin - criado a fim de evitar bugs que acontecem por causa de inputs inválidos do usuário
//! Use apenas depois de >>, usar antes disso ou após um getline limpa o buffer sem precisar
void clearCin()
{
    cin.clear(); // Resetando flags de erro

    while (cin.get() != '\n' && !cin.eof())
        ;
    // Iterando pelo buffer como uma queue, liberando caracteres enquanto não encontra o fim da linha '\n'
    // também checando se cin.eof() não é verdadeiro a fim de evitar bugs nichados, como no caso o usuário fechar o input com ctrl+D (ou ctrl+Z+enter no windows)
    // não, eu não tinha ideia do que é eof, descobri através do stack Overflow enquanto pesquisava sobre como limpar o buffer do cin
    // uma explicação mais detalhada que eu achei foi essa https://softwareengineering.stackexchange.com/questions/259974/is-checking-for-cin-eof-really-necessary
}

// Checa se a string recebida possui apenas elementos numéricos de inteiros (+, -, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9)
bool isNumeric(string s)
{
    if (s.empty())
        return false;

    int start = 0;
    if (s.at(start) == '+' || s.at(start) == '-')
        if (s.size() == 1)
            return false;
        else
            start = 1;

    for (int i = start; i < s.size(); i++)
    {
        if (!isdigit(s.at(i)))
            return false;
    }

    return true;
}

// Recebe um input numérico do usuário, sendo este input entre min e max. Range: [min, max] (inclusivo tanto para min quanto para max)
// Perigoso no caso de min ou max atingirem o número limite de um inteiro
int input(int min, int max)
{
    string text;
    int value = 0;
    do
    {
        value = min - 1;
        cin >> text;
        clearCin();
        if (isNumeric(text))
            value = stoi(text);

    } while (value < min || value > max);

    return value;
}

// Interface para leitura de dados
void opcoesLerDados(arrayListIdioma &lista)
{
    // Mensagens da interface.
    cout << "Deseja:" << endl
         << "(0) Ver todos os dados" << endl
         << "(1) Ver dados filtrados" << endl;
    bool entrada = input(0, 1);

    if (!entrada)
    {
        cout << "Deseja começar de qual linha?" << endl;
        int start = input(1, lista.tamanho - 1) - 1;
        cout << "Em qual deseja terminar? (Digite -1 caso deseje todo o resto)" << endl;
        int finish = input(-1, lista.tamanho);

        if (finish == -1)
            finish = lista.tamanho;

        cout << endl
             << endl;
        printArr(lista, start, finish, 0);
        cout << endl;
    }
    else
    {
        cout << "Quer filtrar com base em qual dado?" << endl
             << "(0) Nome" << endl
             << "(1) Família Linguística" << endl
             << "(2) Número de falantes" << endl
             << "(3) Principal País" << endl;
        int opc = input(0, 3);

        if (opc == 0)
        {
            cout << "Qual é o nome do idioma que você procura?" << endl;
            string nome;
            do
            {
                getline(cin, nome);
            } while (nome.empty());

            quicksortString(lista, 0, lista.tamanho - 1, 0);
            idioma target = binSearchNome(lista, nome);
            quicksortId(lista, 0, lista.tamanho - 1);

            if (!target.valido || !target.id || target.del)
            {
                cout << "Idioma não encontrado!" << endl;
            }
            else
            {
                cout << endl
                     << "Identificador: " << target.id << endl
                     << "Nome: " << target.nome << endl
                     << "Família Linguistica: " << target.famLing << endl
                     << fixed << setprecision(4) << "Número de Falantes (Em milhões): " << target.numfalantes << endl
                     << "Principal País: " << target.principalPais << endl
                     << endl;
            }
        }
        else if (opc == 1)
        {
            cout << "Qual é a família linguística do(s) idioma(s) que você procura?" << endl;
            string famLing;
            do
            {
                getline(cin, famLing);
            } while (famLing.empty());

            quicksortString(lista, 0, lista.tamanho - 1, 1);
            arrayListIdioma correspondentes = binSearchGroups(lista, famLing, true);
            quicksortId(lista, 0, lista.tamanho - 1);

            if (!correspondentes.valido || correspondentes.capacidade == 0)
                cout << "Família Linguística não encontrada!" << endl;
            else
            {
                if (correspondentes.capacidade != 1)
                {
                    cout << "Deseja começar de qual linha?" << endl;
                    int start = input(1, correspondentes.tamanho - 1) - 1;
                    cout << "Em qual deseja terminar? (Digite -1 caso deseje todo o resto)" << endl;
                    int finish = input(-1, correspondentes.tamanho);

                    if (finish == -1)
                        finish = correspondentes.tamanho;

                    cout << endl
                         << endl;
                    printArr(correspondentes, start, finish, 1);
                    cout << endl;
                }
                else
                {
                    idioma target = correspondentes.db[0];
                    cout << endl
                         << "Identificador: " << target.id << endl
                         << "Nome: " << target.nome << endl
                         << "Família Linguistica: " << target.famLing << endl
                         << "Número de Falantes (Em milhões): " << target.numfalantes << endl
                         << "Principal País: " << target.principalPais << endl
                         << endl;
                }
            }
            clearArrayListIdioma(correspondentes);
        }
        else if (opc == 2)
        {
            cout << "Deseja ver em ordem crescente (0) ou descrescente(1)?" << endl;
            bool qual = input(0, 1);
            quicksortNumFal(lista, 0, lista.tamanho - 1, !qual);

            cout << "Deseja começar de qual linha?" << endl;
            int start = input(1, lista.tamanho - 1) - 1;
            cout << "Em qual deseja terminar? (Digite -1 caso deseje todo o resto)" << endl;
            int finish = input(-1, lista.tamanho);

            if (finish == -1)
                finish = lista.tamanho;

            printArr(lista, start, finish, 3);
        }
        else if (opc == 3)
        {
            cout << "Qual é o Principal País falante do(s) idioma(s) que você procura?" << endl;
            string ppais;
            do
            {
                getline(cin, ppais);
            } while (ppais.empty());

            quicksortString(lista, 0, lista.tamanho - 1, 2);
            arrayListIdioma correspondentes = binSearchGroups(lista, ppais, false);
            quicksortId(lista, 0, lista.tamanho - 1);

            if (!correspondentes.valido || correspondentes.capacidade == 0)
                cout << "País não encontrado!" << endl;
            else
            {
                if (correspondentes.capacidade != 1)
                {
                    cout << "Deseja começar de qual linha?" << endl;
                    int start = input(1, correspondentes.tamanho - 1) - 1;
                    cout << "Em qual deseja terminar? (Digite -1 caso deseje todo o resto)" << endl;
                    int finish = input(-1, correspondentes.tamanho);

                    if (finish == -1)
                        finish = correspondentes.tamanho;

                    cout << endl
                         << endl;
                    printArr(correspondentes, start, finish, 2);
                    cout << endl;
                }
                else
                {
                    idioma target = correspondentes.db[0];
                    cout << endl
                         << "Identificador: " << target.id << endl
                         << "Nome: " << target.nome << endl
                         << "Família Linguistica: " << target.famLing << endl
                         << "Número de Falantes (Em milhões): " << target.numfalantes << endl
                         << "Principal País: " << target.principalPais << endl
                         << endl;
                }
            }
            clearArrayListIdioma(correspondentes);
        }
    }
}

// Interface para escrita de dados
void novoIdioma(arrayListIdioma &lista)
{
    cout << "Qual é o nome do novo idioma?" << endl
         << "OBS1: Escreva nesse formato - Inglês, Português - com a primeira letra maiúscula e com acentuação. (Você pode por espaços se precisar)" << endl
         << "OBS2: Não são aceitos nomes repetidos" << endl;

    string nome;
    idioma lixo;
    quicksortString(lista, 0, lista.tamanho - 1, 0);
    do
    {
        getline(cin, nome);
        lixo = binSearchNome(lista, nome);
    } while (nome.empty() || lixo.valido);
    quicksortId(lista, 0, lista.tamanho - 1);

    cout << "Qual é a família linguística do idioma?" << endl
         << "OBS: Escreva nesse formato - Japonês isolado, Indo-europeia (românica) - com a primeira letra maiúscula e com acentuação." << endl;

    string famLing;
    do
    {
        getline(cin, famLing);
    } while (famLing.empty());

    cout << "Qual é o número de falantes do idioma? (Em milhões)" << endl;

    double numFal;
    do
    {
        cin >> numFal;
        clearCin();
    } while (numFal < 0);

    cout << "Qual é o Principal País que fala o idioma?" << endl
         << "OBS: Escreva nesse formato - Estados Unidos, Índia - com a primeira letra maiúscula e com acentuação." << endl;
    string ppais;

    do
    {
        getline(cin, ppais);
    } while (ppais.empty());

    idioma i;
    i.nome = nome;
    i.famLing = famLing;
    i.numfalantes = numFal;
    i.principalPais = ppais;

    i = add(lista, i);

    if (i.id)
        cout << "Idioma adicionado com sucesso!" << endl;
    else
        cout << "Idioma não pode ser adicionado." << endl;
}

// Interface para atualização de dados
void updateIdioma(arrayListIdioma &lista)
{
    if (lista.tamanho < 0)
    {
        cout << "Adicione algum idioma antes!" << endl;
        return;
    }

    cout << "Qual é o ID do idioma que deseja atualizar? ( ID = Número de identificaçao || Identificador)" << endl;
    int id = input(1, lista.tamanho);

    idioma antigo = findById(lista, id);

    if (antigo.del == true || !antigo.valido)
    {
        cout << "Não existe idioma com esse id!" << endl;
        return;
    }

    cout << "Agora vamos atualizar os dados desse idioma:" << endl;
    cout << "Qual é o novo nome do idioma?" << endl
         << "OBS1: Escreva nesse formato - Inglês, Português - com a primeira letra maiúscula e com acentuação. (Você pode por espaços se precisar)" << endl
         << "OBS2: Não são aceitos nomes repetidos" << endl;

    string nome;
    idioma lixo;
    quicksortString(lista, 0, lista.tamanho - 1, 0);
    do
    {
        getline(cin, nome);
        lixo = binSearchNome(lista, nome);
    } while (nome.empty() || lixo.valido);
    quicksortId(lista, 0, lista.tamanho - 1);

    cout << "Qual é a nova família linguística do idioma?" << endl
         << "OBS: Escreva nesse formato - Japonês isolado, Indo-europeia (românica) - com a primeira letra maiúscula e com acentuação." << endl;

    string famLing;
    do
    {
        getline(cin, famLing);
    } while (famLing.empty());

    cout << "Qual é o novo número de falantes do idioma?" << endl
         << "(Limite de até 2.147.483.647)" << endl;

    double numFal;
    do
    {
        cin >> numFal;
        clearCin();
    } while (numFal < 0);

    cout << "Qual é o novo Principal País que fala o idioma?" << endl
         << "OBS: Escreva nesse formato - Estados Unidos, Índia - com a primeira letra maiúscula e com acentuação." << endl;
    string ppais;

    do
    {
        getline(cin, ppais);
    } while (ppais.empty());

    idioma i;
    i.id = id;
    i.nome = nome;
    i.famLing = famLing;
    i.numfalantes = numFal;
    i.principalPais = ppais;

    i = update(lista, id - 1, i);
    if (i.valido)
        cout << "Idioma atualizado com sucesso!" << endl;
    else
        cout << "Idioma não pode ser atualizado." << endl;
}

// Interface para remoção de dados
void deleteIdioma(arrayListIdioma &lista)
{
    if (lista.tamanho <= 0)
    {
        cout << "Adicione algum idioma antes!" << endl;
        return;
    }

    cout << "Qual é o ID do idioma que deseja remover? ( ID = Número de identificaçao || Identificador)" << endl;
    int id = input(1, lista.tamanho);

    idioma antigo = findById(lista, id);

    if (antigo.del == true || !antigo.valido)
    {
        cout << "Não existe idioma com esse id!" << endl;
        return;
    }

    idioma i = removeIdioma(lista, id);
    if (i.id)
        cout << "Idioma removido com sucesso!" << endl;
    else
        cout << "Idioma não pode ser removido." << endl;
}

// ! Assinatura de save definida aqui para ser usada em `run()`, mas faz parte do grupo de funções "Sistema" (Próximo grupo)

void save(arrayListIdioma &lista);

// Interface inicial com o usuário
void run(arrayListIdioma &lista)
{
    bool running = true;
    cout << "Sistema iniciado." << endl;

    while (running)
    {
        cout << endl
             << "O que deseja fazer?" << endl
             << "(0) Ver dados" << endl
             << "(1) Inserir dados" << endl
             << "(2) Atualizar dados" << endl
             << "(3) Excluir dados" << endl
             << "(4) Sair" << endl
             << endl;
        int entrada = input(0, 4);

        switch (entrada)
        {
        case 0:
            opcoesLerDados(lista);
            break;

        case 1:
            novoIdioma(lista);
            break;

        case 2:
            updateIdioma(lista);
            break;

        case 3:
            deleteIdioma(lista);
            break;

        default:
            running = false;
        }
    }

    cout << "Deseja salvar as mudanças feitas? [(0) sim, (1) não]" << endl;
    bool salvar = input(0, 1);

    if (!salvar)
        save(lista);

    cout << "Fechando sistema..." << endl;
}

//? Sistema

// Inicialização do sistema
arrayListIdioma init()
{
    // Instanciando o arquivo
    ifstream arquivo("db.csv");
    int capacidade_file;
    arquivo >> capacidade_file; // achando o tamanho do vetor
    string lixo;
    arquivo >> lixo;

    arrayListIdioma lista = newArrayListIdioma(40);

    getline(arquivo, lixo); // removendo o header

    for (int i = 0; i < capacidade_file; i++) // input dos dados
    {
        string linha;
        getline(arquivo, linha, ';');

        if (!linha.empty())
        {
            idioma a;

            a.id = stoi(linha);

            getline(arquivo, linha, ';');
            linha.erase(0, 1);
            a.nome = linha;

            getline(arquivo, linha, ';');
            linha.erase(0, 1);
            a.famLing = linha;

            getline(arquivo, linha, ';');
            a.numfalantes = stod(linha);

            getline(arquivo, linha, ';');
            linha.erase(0, 1);
            a.principalPais = linha;

            add(lista, a);
        }
    }

    arquivo.close();
    return lista;
}

// Faz exatamente o que você imagina
void save(arrayListIdioma &lista)
{
    int deletados = 0;
    for (int i = 0; i < lista.tamanho; i++)
        if (lista.db[i].del)
            deletados++;

    ofstream arquivo("db.csv");
    arquivo << lista.tamanho - deletados << endl;
    arquivo << "#id; nome; familiaLinguistica; numFalantes; pricipalPais;" << endl;
    for (int i = 0; i < lista.tamanho; i++)
    {
        if (!lista.db[i].del && lista.db[i].valido)
            arquivo << lista.db[i].id << "; "
                    << lista.db[i].nome << "; "
                    << lista.db[i].famLing << "; "
                    << fixed << setprecision(4) << lista.db[i].numfalantes << "; "
                    << lista.db[i].principalPais << "; " << endl;
    }

    arquivo.close();
}

//? Main

int main(int argc, char const *argv[])
{

    arrayListIdioma lista = init();
    run(lista);

    clearArrayListIdioma(lista);
}
