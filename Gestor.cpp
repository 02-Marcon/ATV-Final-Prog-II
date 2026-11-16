#include <iostream>
#include <string>
#include <limits>
#include <cctype>

using namespace std;

struct Tarefa //Todas as informações da tarefa
{
    int codigo;
    string nome;
    string descricao;
    int dia;
    int mes;
    int ano;
    int prioridade;
    Tarefa *prox;
};

int proximoCodigo = 1;

int lerInteiro(string mensagem, int minimo, int maximo)
{
    int valor;

    while (true)
    {
        cout << mensagem;

        if (cin >> valor)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (valor >= minimo && valor <= maximo)
            {
                return valor;
            }
        }
        else
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        cout << "Valor invalido! Tente novamente.\n";
    }
}

bool anoBissexto(int ano)//Conferir se o ano é bissexto
{
    return (ano % 400 == 0) || (ano % 4 == 0 && ano % 100 != 0);
}

bool dataValida(int dia, int mes, int ano)//Conferir se a data é válida
{
    if (ano < 1900 || ano > 9999 || mes < 1 || mes > 12)
        return false;

    int diasMes[] = {
        31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (anoBissexto(ano))
        diasMes[1] = 29;

    return dia >= 1 && dia <= diasMes[mes - 1];
}

bool dataMenor(Tarefa *a, Tarefa *b)//Comparar datas para ordenar 
{
    if (a->ano != b->ano)
        return a->ano < b->ano;

    if (a->mes != b->mes)
        return a->mes < b->mes;

    return a->dia < b->dia;
}

void adicionarTarefa(Tarefa *&lista)//Criar uma nova tarefa e adicioná-la à lista de tarefas pendentes
{
    Tarefa *nova = new Tarefa;

    nova->codigo = proximoCodigo++;

    cout << "\nNome da tarefa: ";
    getline(cin, nova->nome);

    while (nova->nome.empty())
    {
        cout << "O nome nao pode ficar vazio: ";
        getline(cin, nova->nome);
    }

    cout << "Descricao: ";
    getline(cin, nova->descricao);

    do
    {
        cout << "Data de prazo (dia mes ano): ";

        cin >> nova->dia >> nova->mes >> nova->ano;

        if (!cin)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Data invalida!\n";
            nova->dia = 0;
        }
        else
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (!dataValida(nova->dia, nova->mes, nova->ano))
                cout << "Data inexistente! Tente novamente.\n";
        }

    } while (!dataValida(nova->dia, nova->mes, nova->ano));

    nova->prioridade = lerInteiro(
        "Prioridade (1 a 5): ", 1, 5);

    nova->prox = nullptr;

    if (lista == nullptr)
    {
        lista = nova;
    }
    else
    {
        Tarefa *atual = lista;

        while (atual->prox != nullptr)
        {
            atual = atual->prox;
        }

        atual->prox = nova;
    }

    cout << "\nTarefa cadastrada com sucesso!\n";
    cout << "Codigo: " << nova->codigo << endl;
}

void exibirTarefa(Tarefa *t)//Mostrar as informações de uma tarefa específica
{
    cout << "\n----------------------------------\n";
    cout << "Codigo: " << t->codigo << endl;
    cout << "Nome: " << t->nome << endl;
    cout << "Descricao: " << t->descricao << endl;

    cout << "Prazo: ";

    if (t->dia < 10)
        cout << "0";
    cout << t->dia << "/";

    if (t->mes < 10)
        cout << "0";
    cout << t->mes << "/" << t->ano << endl;

    cout << "Prioridade: " << t->prioridade << "/5\n";
    cout << "----------------------------------\n";
}

void listarTarefas(Tarefa *lista)//Mostrar todas as tarefas
{
    if (lista == nullptr)
    {
        cout << "\nNenhuma tarefa cadastrada.\n";
        return;
    }

    cout << "\n===== LISTA DE TAREFAS =====\n";

    Tarefa *atual = lista;

    while (atual != nullptr)
    {
        exibirTarefa(atual);
        atual = atual->prox;
    }
}

void trocarDados(Tarefa *a, Tarefa *b)
{
    int codigo = a->codigo;
    a->codigo = b->codigo;
    b->codigo = codigo;

    string nome = a->nome;
    a->nome = b->nome;
    b->nome = nome;

    string descricao = a->descricao;
    a->descricao = b->descricao;
    b->descricao = descricao;

    int dia = a->dia;
    a->dia = b->dia;
    b->dia = dia;

    int mes = a->mes;
    a->mes = b->mes;
    b->mes = mes;

    int ano = a->ano;
    a->ano = b->ano;
    b->ano = ano;

    int prioridade = a->prioridade;
    a->prioridade = b->prioridade;
    b->prioridade = prioridade;
}

void ordenarTarefas(Tarefa *lista, int criterio)
{
    if (lista == nullptr || lista->prox == nullptr)
    {
        cout << "\nNao existem tarefas suficientes para ordenar.\n";
        return;
    }

    bool trocou;

    do
    {
        trocou = false;

        Tarefa *atual = lista;

        while (atual->prox != nullptr)
        {
            bool trocar = false;

            if (criterio == 1)
            {
                if (dataMenor(atual->prox, atual))
                    trocar = true;

                else if (!dataMenor(atual, atual->prox) && !dataMenor(atual->prox, atual) && atual->prox->prioridade > atual->prioridade)
                    trocar = true;
            }

            else if (criterio == 2)
            {
                if (atual->prioridade < atual->prox->prioridade)
                    trocar = true;

                else if (atual->prioridade == atual->prox->prioridade && dataMenor(atual->prox, atual))
                    trocar = true;
            }

            if (trocar)
            {
                trocarDados(atual, atual->prox);
                trocou = true;
            }

            atual = atual->prox;
        }

    } while (trocou);

    cout << "\nTarefas ordenadas com sucesso!\n";
    listarTarefas(lista);
}

bool removerTarefa(Tarefa *&lista, int codigo)
{
    Tarefa *atual = lista;
    Tarefa *anterior = nullptr;

    while (atual != nullptr)
    {
        if (atual->codigo == codigo)
        {
            if (anterior == nullptr)
                lista = atual->prox;
            else
                anterior->prox = atual->prox;

            delete atual;
            return true;
        }

        anterior = atual;
        atual = atual->prox;
    }

    return false;
}

bool moverTarefa(Tarefa *&origem, Tarefa *&destino, int codigo)
{
    Tarefa *atual = origem;
    Tarefa *anterior = nullptr;

    while (atual != nullptr)
    {
        if (atual->codigo == codigo)
        {
            if (anterior == nullptr)
                origem = atual->prox;
            else
                anterior->prox = atual->prox;

            atual->prox = nullptr;

            if (destino == nullptr)
            {
                destino = atual;
            }
            else
            {
                Tarefa *fim = destino;

                while (fim->prox != nullptr)
                    fim = fim->prox;

                fim->prox = atual;
            }

            return true;
        }

        anterior = atual;
        atual = atual->prox;
    }

    return false;
}

void excluirTarefa(Tarefa *&pendentes, Tarefa *&concluidas)
{
    int codigo = lerInteiro("Codigo da tarefa: ", 1, 999999999);

    if (removerTarefa(pendentes, codigo) ||
        removerTarefa(concluidas, codigo))
    {
        cout << "\nTarefa excluida com sucesso!\n";
    }
    else
    {
        cout << "\nCodigo nao encontrado.\n";
    }
}

void concluirTarefa(Tarefa *&pendentes, Tarefa *&concluidas)
{
    int codigo = lerInteiro("Codigo da tarefa: ", 1, 999999999);

    if (moverTarefa(pendentes, concluidas, codigo))
        cout << "\nTarefa concluida com sucesso!\n";
    else
        cout << "\nTarefa nao encontrada nas pendentes.\n";
}

void reabrirTarefa(Tarefa *&concluidas, Tarefa *&pendentes)
{
    int codigo = lerInteiro("Codigo da tarefa: ", 1, 999999999);

    if (moverTarefa(concluidas, pendentes, codigo))
        cout << "\nTarefa reaberta com sucesso!\n";
    else
        cout << "\nTarefa nao encontrada nas concluidas.\n";
}

void liberarLista(Tarefa *&lista)
{
    while (lista != nullptr)
    {
        Tarefa *auxiliar = lista;
        lista = lista->prox;

        delete auxiliar;
    }
}

int main()
{
    Tarefa *pendentes = nullptr;
    Tarefa *concluidas = nullptr;

    int opcao;

    do
    {
        cout << "\n=================================\n";
        cout << "       GESTOR DE TAREFAS\n";
        cout << "=================================\n";
        cout << "1 - Adicionar tarefa\n";
        cout << "2 - Excluir tarefa\n";
        cout << "3 - Ver lista de tarefas\n";
        cout << "4 - Ordenar por prazo\n";
        cout << "5 - Ordenar por prioridade\n";
        cout << "6 - Concluir tarefa\n";
        cout << "7 - Ver tarefas desfeitas/concluidas\n";
        cout << "8 - Reabrir tarefa\n";
        cout << "0 - Sair\n";
        cout << "=================================\n";

        opcao = lerInteiro("Escolha uma opcao: ", 0, 9);

        switch (opcao)
        {
        case 1:
            adicionarTarefa(pendentes);
            break;

        case 2:
            excluirTarefa(pendentes, concluidas);
            break;

        case 3:
            listarTarefas(pendentes);
            break;

        case 4:
            ordenarTarefas(pendentes, 1);
            break;

        case 5:
            ordenarTarefas(pendentes, 2);
            break;

        case 6:
            concluirTarefa(pendentes, concluidas);
            break;

        case 7:
            cout << "\n===== TAREFAS CONCLUIDAS =====\n";
            listarTarefas(concluidas);
            break;

        case 8:
            reabrirTarefa(concluidas, pendentes);
            break;

        case 0:
            cout << "\nEncerrando o Gestor de Tarefas...\n";
            break;
        }

    } while (opcao != 0);

    liberarLista(pendentes);
    liberarLista(concluidas);

    cout << "Memoria liberada com sucesso!\n";

    return 0;
}