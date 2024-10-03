#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

struct produto {
    string nome;
    float preco;
    int stock;
    char status;  // 'A' para ativo, 'D' para eliminado
};

// Função para verificar se o arquivo database.txt existe
bool verificarDatabase() {
    ifstream file("database.txt");
    if (!file) {
        char escolha;
        cout << "O arquivo 'database.txt' não existe. Deseja criar um novo arquivo? (S/N): ";
        cin >> escolha;
        if (escolha == 'S' || escolha == 's') {
            ofstream newFile("database.txt");
            if (newFile.is_open()) {
                newFile.close();
                cout << "Arquivo 'database.txt' criado com sucesso!" << endl;
                return true;
            } else {
                cout << "Erro ao criar o arquivo!" << endl;
                return false;
            }
        } else {
            cout << "O programa será encerrado." << endl;
            return false;
        }
    }
    return true;
}

// Função para adicionar um produto no arquivo
void addproduto() {
    produto p;
    cout << "Nome do produto: ";
    cin >> p.nome;
    cout << "Preco do produto: ";
    cin >> p.preco;
    cout << "Quantidade em stock: ";
    cin >> p.stock;
    p.status = 'A';  // Produto será marcado como ativo

    ofstream file("database.txt", ios::app);
    if (file.is_open()) {
        file << p.nome << "," << p.preco << "," << p.stock << "," << p.status << endl;
        file.close();
        cout << "Produto adicionado com sucesso!" << endl;
    } else {
        cout << "Erro ao abrir o arquivo!" << endl;
    }
}

// Função para exibir todos os produtos no arquivo, incluindo ativos e eliminados
void exibirprodutos() {
    ifstream file("database.txt");
    if (!file.is_open()) {
        cout << "Erro ao abrir o arquivo!" << endl;
        return;
    }

    string linha;
    cout << "Lista de produtos:" << endl;
    while (getline(file, linha)) {
        stringstream ss(linha);
        produto p;
        string precoStr, stockStr;

        if (getline(ss, p.nome, ',') && getline(ss, precoStr, ',') && getline(ss, stockStr, ',') && ss >> p.status) {
            try {
                p.preco = stof(precoStr);
                p.stock = stoi(stockStr);
                cout << "Nome: " << p.nome << " | Preco: " << p.preco << " | Stock: " << p.stock 
                     << " | Status: " << (p.status == 'A' ? "Ativo" : "Eliminado") << endl;
            } catch (const invalid_argument& e) {
                cout << "Erro ao converter preço ou stock. Linha ignorada." << endl;
            }
        }
    }
    file.close();
}

// Função para calcular o valor total dos produtos ativos
float calcularvalortotal() {
    ifstream file("database.txt");
    if (!file.is_open()) {
        cout << "Erro ao abrir o arquivo!" << endl;
        return 0.0;
    }

    string linha;
    float total = 0.0;
    while (getline(file, linha)) {
        stringstream ss(linha);
        produto p;
        string precoStr, stockStr;

        if (getline(ss, p.nome, ',') && getline(ss, precoStr, ',') && getline(ss, stockStr, ',') && ss >> p.status) {
            if (p.status == 'A') {  // Só conta produtos ativos
                try {
                    p.preco = stof(precoStr);
                    p.stock = stoi(stockStr);
                    total += p.preco * p.stock;
                } catch (const invalid_argument& e) {
                    cout << "Erro ao converter preço ou stock. Linha ignorada." << endl;
                }
            }
        }
    }

    file.close();
    return total;
}

// Função para marcar um produto como "eliminado" (status 'D')
void excluirProduto() {
    string nomeParaExcluir;
    cout << "Escreva o nome do produto a ser eliminado: ";
    cin >> nomeParaExcluir;

    ifstream file("database.txt");
    if (!file.is_open()) {
        cout << "Erro ao abrir o arquivo!" << endl;
        return;
    }

    ofstream tempFile("temp.txt");  // Arquivo temporário
    string linha;
    bool produtoEliminado = false;

    while (getline(file, linha)) {
        stringstream ss(linha);
        string nome, precoStr, stockStr, status;
        
        if (getline(ss, nome, ',') && getline(ss, precoStr, ',') && getline(ss, stockStr, ',') && getline(ss, status)) {
            if (nome == nomeParaExcluir && status == "A") {
                tempFile << nome << "," << precoStr << "," << stockStr << ",D" << endl;  // Marca como 'D'
                produtoEliminado = true;
            } else {
                tempFile << linha << endl;  // Copia o restante como está
            }
        }
    }

    file.close();
    tempFile.close();

    remove("database.txt");
    rename("temp.txt", "database.txt");

    if (produtoEliminado) {
        cout << "Produto marcado como eliminado com sucesso." << endl;
    } else {
        cout << "Produto não encontrado ou já está eliminado." << endl;
    }
}

// Função para marcar todos os produtos como "eliminados" (status 'D')
void excluirTodosProdutos() {
    ifstream file("database.txt");
    if (!file.is_open()) {
        cout << "Erro ao abrir o arquivo" << endl;
        return;
    }

    ofstream tempFile("temp.txt");  // Arquivo temporário
    string linha;

    while (getline(file, linha)) {
        stringstream ss(linha);
        string nome, precoStr, stockStr, status;
        
        if (getline(ss, nome, ',') && getline(ss, precoStr, ',') && getline(ss, stockStr, ',') && getline(ss, status)) {
            if (status == "A") {
                tempFile << nome << "," << precoStr << "," << stockStr << ",D" << endl;  // Marca todos como 'D'
            } else {
                tempFile << linha << endl;  // Copia o restante como está
            }
        }
    }

    file.close();
    tempFile.close();

    remove("database.txt");
    rename("temp.txt", "database.txt");

    cout << "Todos os produtos foram marcados como eliminados." << endl;
}

// Função de menu para gerenciar as opções
void menu() {
    if (!verificarDatabase()) {
        return;
    }

    int choice;
    do {
        cout << endl << "Menu:" << endl;
        cout << "1. Adicionar produto" << endl;
        cout << "2. Exibir todos os produtos" << endl;
        cout << "3. Calcular valor total do stock" << endl;
        cout << "4. Eliminar um produto" << endl;
        cout << "5. Eliminar todos os produtos" << endl;
        cout << "6. Sair" << endl;
        cout << "Escolha uma opcao: ";
        cin >> choice;

        switch (choice) {
            case 1:
                addproduto();
                break;
            case 2:
                exibirprodutos();
                break;
            case 3: {
                float total = calcularvalortotal();
                cout << "Valor total do stock (somente produtos ativos): " << total << endl;
                break;
            }
            case 4:
                excluirProduto();
                break;
            case 5:
                excluirTodosProdutos();
                break;
            case 6:
                cout << "bye bye" << endl;
                break;
            default:
                cout << "Opcao invalida. Tente novamente." << endl;
        }
    } while (choice != 6);
}

int main() {
    menu();
    return 0;
}
