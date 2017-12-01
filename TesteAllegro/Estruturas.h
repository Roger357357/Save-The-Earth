enum IDS{Tiro, Adversario, Coisa};

typedef struct Inimigo
{
    int ID;
    int x;
    int y;
    bool vida;
    int velocidade;
    int limite_x;
    int limite_y;
}Inimigo;

typedef struct Objeto
{
    int ID;
    int x;
    int y;
    bool vida;
    int velocidade;
    int limite_x;
    int limite_y;
}Objeto;

typedef struct Bala
{
    int ID;
    int x;
    int y;
    bool vida;
    int velocidade;
}Bala;
