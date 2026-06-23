#include <iostream>
#include <string>
#include <sqlite3.h>
using namespace std;

class Funcionario {
private:
    string nome;
    double salario;
public:
    Funcionario() {
        nome = "";
        salario = 0;
    }
    Funcionario(string n, double s) {
        nome = n;
        salario = s;
    }
    string getNome() {
        return nome;
    }
    double getSalario() {
        return salario;
    }
    void mostrarDados() {
        cout << "Nome: " << nome << endl;
        cout << "Salario: R$ " << salario << endl;
        cout << "----------------------" << endl;
    }
};

class FuncionarioDAO {
private:
    sqlite3* db;

    void criarTabela() {
        const char* sql =
            "CREATE TABLE IF NOT EXISTS funcionario ("
            "  id      INTEGER PRIMARY KEY AUTOINCREMENT,"
            "  nome    TEXT    NOT NULL,"
            "  salario REAL    NOT NULL"
            ");";

        char* erro = nullptr;
        if (sqlite3_exec(db, sql, nullptr, nullptr, &erro) != SQLITE_OK) {
            cout << "Erro ao criar tabela: " << erro << endl;
            sqlite3_free(erro);
        }
    }

public:
    FuncionarioDAO(const string& caminho) {
        if (sqlite3_open(caminho.c_str(), &db) != SQLITE_OK) {
            cout << "Erro ao abrir o banco: " << sqlite3_errmsg(db) << endl;
            db = nullptr;
            return;
        }
        criarTabela();
    }

    ~FuncionarioDAO() {
        if (db) {
            sqlite3_close(db);
        }
    }

    void inserir(Funcionario f) {
        const char* sql =
            "INSERT INTO funcionario (nome, salario) VALUES (?, ?);";

        sqlite3_stmt* stmt;
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            cout << "Erro ao preparar insercao: " << sqlite3_errmsg(db) << endl;
            return;
        }

        sqlite3_bind_text(stmt, 1, f.getNome().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_double(stmt, 2, f.getSalario());

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            cout << "Erro ao inserir: " << sqlite3_errmsg(db) << endl;
        }
        sqlite3_finalize(stmt);
    }

    void listar() {
        const char* sql = "SELECT nome, salario FROM funcionario;";

        sqlite3_stmt* stmt;
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            cout << "Erro ao consultar: " << sqlite3_errmsg(db) << endl;
            return;
        }

        bool algum = false;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            if (!algum) {
                cout << "\n--- LISTA DE FUNCIONARIOS ---" << endl;
                algum = true;
            }
            const unsigned char* nome = sqlite3_column_text(stmt, 0);
            double salario = sqlite3_column_double(stmt, 1);
            Funcionario f(reinterpret_cast<const char*>(nome), salario);
            f.mostrarDados();
        }
        sqlite3_finalize(stmt);

        if (!algum) {
            cout << "\nNenhum funcionario cadastrado." << endl;
        }
    }

    bool buscar(const string& nomeBusca, Funcionario& achado) {
        const char* sql =
            "SELECT nome, salario FROM funcionario WHERE nome = ?;";

        sqlite3_stmt* stmt;
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            cout << "Erro ao buscar: " << sqlite3_errmsg(db) << endl;
            return false;
        }

        sqlite3_bind_text(stmt, 1, nomeBusca.c_str(), -1, SQLITE_TRANSIENT);

        bool encontrado = false;
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            const unsigned char* nome = sqlite3_column_text(stmt, 0);
            double salario = sqlite3_column_double(stmt, 1);
            achado = Funcionario(reinterpret_cast<const char*>(nome), salario);
            encontrado = true;
        }
        sqlite3_finalize(stmt);
        return encontrado;
    }
};

int main() {
    FuncionarioDAO dao("funcionarios.db");

    int op;

    do {
        cout << endl << "------------MENU------------" << endl;
        cout << "1- Cadastrar funcionario" << endl;
        cout << "2- Imprimir funcionarios" << endl;
        cout << "3- Buscar funcionario" << endl;
        cout << "4- Sair" << endl;
        cout << endl << "Digite uma opcao: ";
        cin >> op;

        switch (op) {

        case 1: {
            string novoNome;
            double novoSalario;

            cout << "\nNovo funcionario" << endl;
            cout << "Nome: ";
            cin >> novoNome;
            cout << "Salario: ";
            cin >> novoSalario;

            dao.inserir(Funcionario(novoNome, novoSalario));

            cout << "\nFuncionario cadastrado e salvo no banco com sucesso!" << endl;
            break;
        }

        case 2:
            dao.listar();
            break;

        case 3: {
            string nomeBusca;
            Funcionario achado;

            cout << "\nDigite o nome para buscar: ";
            cin >> nomeBusca;

            if (dao.buscar(nomeBusca, achado)) {
                cout << "\nFuncionario encontrado:" << endl;
                achado.mostrarDados();
            } else {
                cout << "\nFuncionario nao encontrado." << endl;
            }
            break;
        }

        case 4:
            cout << "\nSaindo do sistema..." << endl;
            break;

        default:
            cout << "\nOpcao invalida!" << endl;
        }

    } while (op != 4);

    return 0;
}
