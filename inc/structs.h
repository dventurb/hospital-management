#ifndef STRUCTS_H
#define STRUCTS_H

// BIBLIOTECAS
#include <stdbool.h>

// CONSTANTES
#define STRING_MAX 100
#define PASSWORD_MAX 256

#define MAX_FUNCIONARIOS 50
#define MAX_CLIENTES 1000
#define MAX_MEDICOS 100
#define MAX_CONSULTAS 5000

// ESTRUTURAS E ENUMERADORES
typedef enum {
  Cancelado = 0,  // Canceled  🔴
  Agendado  = 1,  // Scheduled 🟡
  Realizado = 2   // Completed 🟢
}ESTADO;

typedef enum {
 TYPE_CLIENTS,
 TYPE_DOCTORS,
 TYPE_APPOINTMENTS,
 TYPE_STAFF
}TYPE_STRUCT;

typedef struct {
  char rua[STRING_MAX];
  unsigned long int codigo_postal;
  char cidade[STRING_MAX];
}ST_MORADA;

typedef struct {
  unsigned int semana;
  unsigned int hora;
  unsigned int dia;
  unsigned int mes;
  unsigned int ano;
}ST_DATA;

typedef struct {
  unsigned int ID;
  char nome[STRING_MAX];
  char email[STRING_MAX];
  ST_MORADA morada;
  ST_DATA data_nascimento;
  unsigned long int NIF;
  unsigned long int SNS;
  bool estado;
}ST_CLIENTE;

typedef struct {
  unsigned int ID;
  char nome[STRING_MAX];
  char email[STRING_MAX];
  unsigned int cedula;
  char especialidade[STRING_MAX];
  bool estado;
}ST_MEDICO;

typedef struct {
  unsigned int ID;
  ST_CLIENTE *cliente;
  ST_MEDICO *medico;
  ST_DATA data_inicial;
  ST_DATA data_final;
  ESTADO estado;
}ST_CONSULTA;

typedef struct {
  unsigned int ID; // counter the number of users 
  char nome[STRING_MAX];
  char username[STRING_MAX];
  char password[PASSWORD_MAX];
  char pathToImage[STRING_MAX];
}ST_FUNCIONARIO;

typedef struct {
  ST_FUNCIONARIO *staff;
  ST_CLIENTE *clients;
  ST_MEDICO *doctors;
  ST_CONSULTA *appointments;
}ST_APPLICATION;

#endif
