// BIBLIOTECAS
#include <gtk/gtk.h>
#include "structs.h"
#include "authWindow.h"
#include "cliente.h"
#include "medico.h"
#include "consulta.h"

// MAIN 
int main(int argc, char **argv){
  GtkApplication *app;
  
  ST_FUNCIONARIO funcionarios[MAX_FUNCIONARIOS] = {0};
  ST_CLIENTE clientes[MAX_CLIENTES] = {0};
  ST_MEDICO medicos[MAX_MEDICOS] = {0};
  ST_CONSULTA consultas[MAX_CONSULTAS] = {0};
  
  loadUserFile(funcionarios);
  carregarFicheiroCliente(clientes);
  carregarFicheiroMedico(medicos);
  carregarFicheiroConsulta(consultas, clientes, medicos);

  ST_APPLICATION application = {
    .staff = funcionarios,
    .clients = clientes,
    .doctors = medicos,
    .appointments = consultas,
    .theme = strdup(LIGHT_THEME_PATH)
  }; 
   
  srand(time(NULL));
  
  app = gtk_application_new("clinica.gestao", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(createAuthWindow), &application);
  int status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);
    
  return status;
}
